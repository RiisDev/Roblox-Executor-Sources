#include <util/util.hpp>
#include "../../rbx.hpp"
#include "../../../mem/writable/writable.hpp"
#include "boundary.hpp"

#include <capstone-wrapper/wrapper.hpp>

using c_boundary = rbx::ac::c_boundary;

void c_boundary::init( ) const
{
	const capstone_instance_t capstone{ };

	util::logger( )->info( "bypassing boundary checks" );
	const ud::segment_t text( ".text" );

	const auto check = *text.find_pattern
		( "A1 ?? ?? ?? ?? ?? ?? 3B 0D" );

	util::logger( )->debug( "initial boundary check -> 0x{:x}", check );

	const auto boundary 
		= *reinterpret_cast< std::uintptr_t** >( check + 0x9 );

	const auto begin
		= *reinterpret_cast< std::uintptr_t** >( check + 0x1 );

	util::logger( )->debug( "roblox boundary minimum -> {}", static_cast< void* >( begin ) );
	util::logger( )->debug( "roblox boundary maximum -> {}", static_cast< void* >( boundary ) );

	static auto spoofed 
		= std::numeric_limits< std::uintptr_t >::max( );

	std::size_t patched_bounds = 0;
	for ( auto idx = text.start; idx < text.end; ++idx )
	{
		const auto value =
			reinterpret_cast< std::uintptr_t** >( idx );

		if ( *value == boundary )
		{
			const auto start = idx - 2;

			if ( const auto instruction = capstone.decode( reinterpret_cast< std::uint8_t* >( start ) ) )
			{
				if ( instruction->mnemonic_is( "cmp" ) && instruction->type_is( 0, x86_op_type::X86_OP_REG ) && instruction->type_is( 1, x86_op_type::X86_OP_MEM ) )
				{
					const auto mem = instruction->get_memory_operand( 1 );

					if ( mem->disp == reinterpret_cast< std::uintptr_t >( boundary ) )
					{
						if ( const auto next = instruction->get_next_instruction( capstone ) )
						{
							if ( next->mnemonic_is( "jb" ) )
							{
								const auto after_jb = next->get_next_instruction( capstone );

								if ( after_jb )
								{
									if ( ( after_jb->mnemonic_is( "mov" ) && after_jb->type_is( 0, x86_op_type::X86_OP_REG ) && after_jb->type_is( 1, x86_op_type::X86_OP_MEM ) ) || after_jb->mnemonic_is( "or" ) )
									{
										patched_bounds++;

										const auto eip = next->get_eip( );

										mem::c_writeable boundary( eip );

										*eip = 0xEB;
									}
									else
									{
										std::printf( "isnt a 'mov' or 'or' after 'jb' %s %p\n", after_jb->get_mnemonic( ), start - ( std::uintptr_t )GetModuleHandleA( nullptr ) );
									}
								}
								else
								{
									std::printf( "failed after jb" );
								}
							}
							else if ( next->mnemonic_is( "jae" ) )
							{
								patched_bounds++;

								const auto eip = next->get_eip( );

								mem::c_writeable boundary( eip );

								std::memset( eip, 0x90, next->get_length( ) );
							}
							else
							{
								patched_bounds++;

								mem::c_writeable boundary( value );

								*value = &spoofed;
							}
						}
						else
						{
							std::printf( "cant get next\n" );
						}
					}
					else
					{
						std::printf( "not imm %p %p\n", mem->disp, reinterpret_cast< std::uintptr_t >( boundary ) );
					}
				}
				else if ( instruction->mnemonic_is( "mov" ) && instruction->type_is( 0, x86_op_type::X86_OP_REG ) && instruction->type_is( 1, x86_op_type::X86_OP_MEM ) )
				{
					patched_bounds++;
					mem::c_writeable boundary( value );

					*value = &spoofed;
				}
				else
				{
					std::printf( "unknown %s %p\n", instruction->get_mnemonic( ), start - ( std::uintptr_t )GetModuleHandleA( nullptr ) );
				}
			}
		}
	}

	util::logger( )->debug( "patched {} boundaries", patched_bounds );
}

//0xd86ff9
#pragma once

#include <capstone/capstone.h>

#include <optional>
#include <functional>
#include <unordered_map>
#include <algorithm>
#include <array>

#include <udlib/ud.hpp>
#include <libloaderapi.h>

const auto text = ud::segment_t{ ".text" };
const auto rdata = ud::segment_t{ ".rdata" };

class capstone_instance_t
{
	csh handle;

	mutable std::unordered_map< std::string, std::uint8_t* > strings;

public:

	class instruction_t
	{
		cs_insn instruction;

	public:

		instruction_t( cs_insn instruction )
			: instruction{ instruction }
		{
		}

		operator cs_insn( ) { return instruction; }

		std::optional< instruction_t > get_next_instruction( const capstone_instance_t& capstone ) const
		{
			return capstone.decode( get_eip( ) + get_length( ) );
		}

		std::optional< x86_op_mem > get_memory_operand( std::size_t operand_index ) const
		{
			if ( operand_index >= instruction.detail->x86.op_count )
				return std::nullopt;

			return instruction.detail->x86.operands[ operand_index ].mem;
		}

		std::optional< std::uintptr_t > get_imm_operand( std::size_t operand_index ) const
		{
			if ( operand_index >= instruction.detail->x86.op_count )
				return std::nullopt;

			return instruction.detail->x86.operands[ operand_index ].imm;
		}

		std::size_t get_length( ) const
		{
			return instruction.size;
		}

		std::size_t get_operand_count( ) const
		{
			return instruction.detail->x86.op_count;
		}

		std::optional< cs_x86_op > get_operand( std::size_t operand_index ) const
		{
			if ( operand_index >= instruction.detail->x86.op_count )
				return std::nullopt;

			return instruction.detail->x86.operands[ operand_index ];
		}

		std::uint8_t* get_eip( ) const
		{
			return reinterpret_cast< std::uint8_t* >( instruction.address );
		}

		bool mnemonic_is( const char* mnemonic ) const
		{
			return !std::strcmp( instruction.mnemonic, mnemonic );
		}

		bool type_is( std::size_t operand_index, x86_op_type type ) const
		{
			if ( operand_index >= instruction.detail->x86.op_count )
				return false;

			return instruction.detail->x86.operands[ operand_index ].type == type;
		}

		bool reg_is( std::size_t operand_index, x86_reg reg )  const
		{
			if ( operand_index >= instruction.detail->x86.op_count )
				return false;

			return instruction.detail->x86.operands[ operand_index ].reg == reg;
		}

		x86_reg get_reg( std::size_t operand_index ) const
		{
			if ( operand_index >= instruction.detail->x86.op_count )
				return X86_REG_INVALID;

			return instruction.detail->x86.operands[ operand_index ].reg;
		}

		const char* get_mnemonic( ) const
		{
			return instruction.mnemonic;
		}
	};


	capstone_instance_t( )
	{
		cs_open( CS_ARCH_X86, CS_MODE_32, &handle );
		cs_option( handle, cs_opt_type::CS_OPT_DETAIL, CS_OPT_ON );
		cs_option( handle, cs_opt_type::CS_OPT_SKIPDATA, CS_OPT_OFF );
	}

	~capstone_instance_t( )
	{
		cs_close( &handle );
	}

	struct callback_param_t
	{
		instruction_t instruction;
		std::size_t iterator;
		std::uint8_t* data;
	};

	enum class callback_result_t
	{
		CONTINUE,
		STOP,
		ADVANCE
	};

	std::uint8_t* debase( std::uint8_t* data ) const
	{
		static auto mod = reinterpret_cast< std::uint8_t* >( GetModuleHandleA( nullptr ) );

		return reinterpret_cast< std::uint8_t* >( data - mod );
	}

	template< size_t string_count >
	void prepare_string_map( std::array< const char*, string_count > to_compare ) const
	{
		for ( auto rdata_start = reinterpret_cast< std::uint8_t* >( rdata.start ); rdata_start < reinterpret_cast< std::uint8_t* >( rdata.end ); rdata_start++ )
		{
			for ( auto& string : to_compare )
			{
				if ( std::strlen( string ) > reinterpret_cast< std::uint8_t* >( rdata.end ) - rdata_start )
					return;

				if ( std::memcmp( rdata_start, string, std::strlen( string ) ) )
					continue;

				strings[ string ] = rdata_start;

				break;
			}
		}
	}

	std::vector< std::uint8_t* > get_strings( const char* string ) const
	{
		std::vector< std::uint8_t* > strings;

		for ( auto rdata_start = reinterpret_cast< std::uint8_t* >( rdata.start ); rdata_start < reinterpret_cast< std::uint8_t* >( rdata.end ); rdata_start++ )
		{
			if ( std::memcmp( rdata_start, string, std::strlen( string ) ) )
				continue;

			strings.push_back( rdata_start );
		}

		return strings;
	}

	std::uint8_t* get_string( const char* string ) const
	{
		return strings.at( string );
	}

	std::uint8_t* calculate_absolute_address( std::uint8_t* base, std::size_t instruction_size ) const
	{
		return base + *reinterpret_cast< std::uintptr_t* >( base + instruction_size - sizeof( std::uintptr_t ) ) + instruction_size;
	}

	[[nodiscard]] std::vector< std::uint8_t* > get_xrefs( std::uint8_t* address, ud::segment_t segment = text ) const
	{
		std::vector< std::uint8_t* > xrefs;

		for ( auto i = 0u; i < segment.size; ++i )
		{
			auto instruction = reinterpret_cast< std::uint8_t* >( segment.start ) + i;

			if ( *reinterpret_cast< std::uint8_t** >( instruction ) == address || calculate_absolute_address( instruction, 5 ) == address )
				xrefs.push_back( instruction );
		}

		return xrefs;
	}

	std::optional< instruction_t > decode( std::uint8_t* code, std::optional< std::size_t > size = std::nullopt ) const
	{
		cs_insn* instruction;

		if ( cs_disasm( handle, code, size ? *size : 16, reinterpret_cast< std::uintptr_t >( code ), 1, &instruction ) )
			return instruction_t{ *instruction };

		return std::nullopt;
	}

	void decode_until( std::function< callback_result_t( callback_param_t ) > callback, std::uint8_t* code, std::optional< std::size_t > size = std::nullopt ) const
	{
		std::size_t iterator{};

		while ( const auto instruction = decode( code + iterator, size ? *size - iterator : 16 ) )
		{
			switch ( callback( { *instruction, iterator, code } ) )
			{
				case callback_result_t::CONTINUE:
					break;

				case callback_result_t::STOP:
					return;
			}

			iterator += instruction->get_length( );
		}

		std::printf( "broke %p\n", iterator + code );
	}

	std::uint8_t* find_current_prologue( std::uint8_t* code ) const
	{
		constexpr std::uint8_t prologue_bytes[] = { 0x55, 0x8B, 0xEC };

		for ( auto iterator = code; iterator > reinterpret_cast< std::uint8_t* >( text.start ); --iterator )
		{
			if ( !std::memcmp( iterator, prologue_bytes, sizeof( prologue_bytes ) ) )
				return iterator;
		}

		return nullptr;
	}

	std::uint8_t* find_next_prologue( std::uint8_t* code, std::optional< std::size_t > size = std::nullopt ) const
	{
		std::uint8_t* result = nullptr;

		decode_until( [ & ]( callback_param_t param )
		{
			if ( param.instruction.mnemonic_is( "push" ) && param.instruction.reg_is( 0, x86_reg::X86_REG_EBP ) )
			{
				if ( const auto proceeding_instruction = decode( param.data + param.iterator + param.instruction.get_length( ) ) )
				{
					if ( proceeding_instruction->mnemonic_is( "mov" ) && proceeding_instruction->reg_is( 0, x86_reg::X86_REG_EBP ) && proceeding_instruction->reg_is( 1, x86_reg::X86_REG_ESP ) )
					{
						result = param.data + param.iterator;

						return callback_result_t::STOP;
					}
				}
			}

			return callback_result_t::CONTINUE;
		}, code, size );

		return result;
	}
};
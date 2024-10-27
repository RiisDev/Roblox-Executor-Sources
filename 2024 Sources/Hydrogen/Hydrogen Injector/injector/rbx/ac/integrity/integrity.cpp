#include <minhook/minhook.hpp>
#include <util/util.hpp>
#include "../../rbx.hpp"
#include "integrity.hpp"

using c_integrity = rbx::ac::c_integrity;

void c_integrity::calc_second( 
	const void* const a1, const std::uint16_t a2, 
		const std::size_t key, const std::uint32_t& hash )
{
	static_cast< decltype( &calc_second ) >
		( g_integrity.m_calc_second )
			( a1, a2, 0, hash );
}

void __declspec( naked ) c_integrity::calc_first( )
{
	__asm
	{
		mov g_integrity.m_addr, ebx;

		sub esp, 0x1000;
		pushad;
	}

	if ( g_integrity.m_addr >= g_integrity.text.first.start 
		&& g_integrity.m_addr <= g_integrity.text.first.end )
	{
		g_integrity.m_addr = std::uintptr_t
			( g_integrity.text.second + 
				( g_integrity.m_addr 
					- g_integrity.text.first.start ) );
	}

	__asm
	{
		popad;

		push edi
		mov edi, g_integrity.m_addr
		mov eax, [ edi ]
		pop edi

		add eax, ebx
		imul eax, eax, 0x1594FE2D
		add eax, edi
		rol eax, 0x13
		imul edi, eax, 0xCBB4ABF7
		lea eax, [ ebx + 0x04 ]

		push edi
		mov edi, g_integrity.m_addr
		sub eax, [ edi + 0x04 ]
		pop edi

		add ebx, 0x08
		imul eax, eax, 0x344B5409
		add eax, [ ebp - 0x10 ]
		rol eax, 0x11
		imul eax, eax, 0x1594FE2D
		mov [ ebp - 0x10 ], eax

		push edi
		mov edi, g_integrity.m_addr
		add edi, 0x08
		mov eax, [ edi ]
		pop edi

		xor eax, ebx
		add ebx, 0x04
		imul eax, eax, 0x1594FE2D
		add eax, [ ebp - 0x0C ]
		rol eax, 0x0D
		imul eax, eax, 0xCBB4ABF7
		mov [ ebp - 0x0C ], eax

		push edi
		mov edi, g_integrity.m_addr
		add edi, 0x0C
		mov eax, [ edi ]
		pop edi

		add esp, 0x1000;
		jmp g_integrity.m_jmp_calc_first;
	}
}

void __declspec( naked ) c_integrity::second_check_bypass( )
{
	__asm
	{
		lea esp, [ esp + 0x4 ]
		btr eax, eax
		btr ax, 0x6B;
		mov     eax, esp
		cmc
		stc
		mov eax, [eax + 0x8]
		rol eax, 0x5
		clc
		mov[ ebp - 0x48 ], eax
		movsx   ax, cl
		lea     esp, [ esp + 0x0C ]

		mov g_integrity.m_addr, ebx;

		pushad;
	}

	if ( g_integrity.m_addr >= g_integrity.text.first.start
		&& g_integrity.m_addr <= g_integrity.text.first.end )
	{
		g_integrity.m_addr = std::uintptr_t
		( g_integrity.text.second +
			( g_integrity.m_addr
				- g_integrity.text.first.start ) );
	}

	__asm
	{
		popad;
		sub esp, 0x4;
		push edi
		mov edi, g_integrity.m_addr
		mov eax, [ edi - 4 ]
		pop edi
		add esp, 0x4


		jmp g_integrity.shit;
	}
}

void c_integrity::init( )
{
	util::logger( )->info( "bypassing integrity checks" );
	const auto clone = VirtualAlloc( nullptr, text.first.size, 
		MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );

	text.second = static_cast< std::uint8_t* >
		( std::memcpy( clone, reinterpret_cast< void* >
			( text.first.start ), text.first.size ) );

	util::logger( )->debug( ".text -> 0x{:x} | 0x{:x}", text.first.start, text.first.size );

	MH_CreateHook( *proc.find_pattern< void* >
		( "55 8B EC 51 53 56 8B 75 08 57 8B F9 6A" ), 
			&calc_second, &m_calc_second );

	const auto calc_first_addr = 
		*proc.find_pattern< std::uint8_t* >
			( "8B 03 03 C3 69 C0 2D FE 94 15" );

	util::logger( )->debug( "core hasher -> {}", static_cast< void* >( calc_first_addr ) );

	void* tramp;
	MH_CreateHook( calc_first_addr, 
			&calc_first, &tramp );

	m_jmp_calc_first = calc_first_addr + 0x51;

	const auto second_check = calc_first_addr + 0x7c; //0x3396bc 12/1/2022

	shit = reinterpret_cast< std::uint8_t* >( GetModuleHandleA( nullptr ) ) + 0x3fec3fc;

	void* tramp2;
	MH_CreateHook( second_check,
		&second_check_bypass, &tramp2 );

	util::logger( )->debug( "second_check -> {}", static_cast< void* >( second_check ) );
}
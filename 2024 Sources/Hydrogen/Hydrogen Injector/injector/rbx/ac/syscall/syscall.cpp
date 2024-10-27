#include <filesystem>
#include <minhook/minhook.hpp>
#include <util/util.hpp>
#include "../../rbx.hpp"
#include "syscall.hpp"

using c_syscall = rbx::ac::c_syscall;

void* c_syscall::find_hook( const std::uint32_t id )
{
	const auto& hook = g_syscall.m_hooks.find( id );

	return hook != g_syscall.m_hooks.cend( ) 
		? hook->second : nullptr;
}

std::uint8_t* c_syscall::create_dispatcher( )
{
	MH_DisableHook( g_syscall.m_create_dispatcher );

	const auto proxy_dispatcher =
		static_cast< decltype( &create_dispatcher ) >
			( g_syscall.m_create_dispatcher )( );

	*reinterpret_cast< void** >
		( proxy_dispatcher + 0x6 ) 
			= &dispatcher;

	g_syscall.m_jmp_dispatcher = proxy_dispatcher + 0xA;

	return proxy_dispatcher;
}

void __declspec( naked ) c_syscall::dispatcher( )
{
	__asm
	{
		push eax;
		mov ecx, eax;
		call find_hook;
		mov ecx, eax;
		pop eax;

		test ecx, ecx;
		jz real;

	hook:
		add esp, 0x4;
		jmp ecx;

	real:
		jmp g_syscall.m_jmp_dispatcher;
	}
}

void*& c_syscall::operator[]( const std::string_view sys_fn )
{
	static HMODULE ntdll;
	if ( !ntdll )
	{
		static std::string sys_dir;
		sys_dir.reserve( MAX_PATH );

		GetSystemWindowsDirectoryA( sys_dir.data( ), sys_dir.capacity( ) );

		const auto sys_path = ( std::filesystem::path( sys_dir )
			/ "System32" / "ntdll.dll" );

		ntdll = GetModuleHandleW( sys_path.c_str( ) );
		_ntdll = ntdll;
	}

	return m_hooks[ *( reinterpret_cast< std::uint8_t* >( 
		GetProcAddress( ntdll , sys_fn.data( ) ) ) + 1 ) ];
}

HMODULE rbx::ac::c_syscall::ntdll( ) const
{
	return _ntdll;
}

void c_syscall::init( )
{
	util::logger( )->info( "initializing syscall controller" );
	m_create_dispatcher = *proc.find_pattern< void* >
		( "55 8B EC A1 ?? ?? ?? ?? 83 EC 08 35" );

	util::logger( )->info( "syscall dispatcher: {}", m_create_dispatcher );

	void* tramp;
	MH_CreateHook( m_create_dispatcher, 
		&create_dispatcher, &tramp );
}
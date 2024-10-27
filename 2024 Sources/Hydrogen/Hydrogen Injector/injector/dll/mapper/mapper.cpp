#include <Windows.h>
#include <ntstatus.h>
#include <minhook/minhook.hpp>
#include "../../rbx/ac/syscall/syscall.hpp"
#include "mapper.hpp"

using c_mapper = dll::c_mapper;

NTSTATUS c_mapper::call_entry
	( HANDLE h_thread, LPCONTEXT lp_context )
{
	reinterpret_cast< void( * )( ) >
		( GetProcAddress( g_mapper.dll, "entry" ) )( );

	GetThreadContext( h_thread, lp_context );

	rbx::ac::g_syscall[ "ZwGetContextThread" ] = nullptr;

	return STATUS_SUCCESS;
}

NTSTATUS c_mapper::create_user_process
	(
		PHANDLE process, PHANDLE thread,
		ACCESS_MASK process_access, ACCESS_MASK thread_access,
		PVOID process_attr, PVOID thread_attr,
		ULONG process_flags, ULONG thread_flags,
		PVOID process_params, PVOID create_info,
		PVOID attribute_list 
	)
{
	return 0;

	const auto result = reinterpret_cast< decltype( &create_user_process ) >( original_create_user_process )
		(
			process, thread,
			process_access, thread_access,
			process_attr, thread_attr,
			process_flags, thread_flags,
			process_params, create_info,
			attribute_list
		);

	const std::string_view injector = "injector.dll";
	DWORD buff;

	const auto alloc = VirtualAllocEx( *process, nullptr, injector.size( ) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
	WriteProcessMemory( *process, alloc, injector.data( ), injector.size( ) + 1, &buff );

	CreateRemoteThread( *process, nullptr, 0, reinterpret_cast< LPTHREAD_START_ROUTINE >( &LoadLibraryA ), alloc, 0, nullptr );
	return result;
}

void c_mapper::init( )
{
	dll = LoadLibraryA( "hydrogen.dll" );

	rbx::ac::g_syscall[ "ZwGetContextThread" ] = &call_entry;

	MH_CreateHook
		( 
			GetProcAddress
				( 
					rbx::ac::g_syscall.ntdll( ), "ZwCreateUserProcess" 
				), 
			&create_user_process, &original_create_user_process
		);

	dll = LoadLibraryA( "hydrogen.dll" );
}

c_mapper::c_mapper( ) {}

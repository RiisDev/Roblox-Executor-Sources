#ifndef _SYSCALL_
#define _SYSCALL_

#include <Windows.h>
#include <unordered_map>

namespace rbx::ac
{
	class c_syscall
	{
		HMODULE _ntdll{ };

		std::unordered_map
			< std::size_t, void* > m_hooks;
		static void* __fastcall find_hook
			( const std::uint32_t id );

		void* m_create_dispatcher;
		static std::uint8_t* create_dispatcher( );

		void* m_jmp_dispatcher;
		static void dispatcher( );

	public:
		void*& operator[]( const std::string_view sysfn );

		HMODULE ntdll( ) const;
		void init( );
	} inline g_syscall;
}

#endif
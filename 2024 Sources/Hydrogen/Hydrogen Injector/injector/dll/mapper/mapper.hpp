#ifndef _MAPPER_
#define _MAPPER_

#include <Windows.h>

namespace dll
{
	class c_mapper
	{
		HMODULE dll{ };

		static NTSTATUS call_entry
			( HANDLE h_thread, LPCONTEXT lp_context );

		inline static void* original_create_user_process;
		static NTSTATUS __stdcall create_user_process
			( 
				PHANDLE process, PHANDLE thread,
				ACCESS_MASK process_access, ACCESS_MASK thread_access,
				PVOID process_attr, PVOID thread_attr,
				ULONG proccess_flags, ULONG thread_flags,
				PVOID process_params, PVOID create_info,
				PVOID attribute_list
			);
	public:
		void init( );

		c_mapper( );
	} inline g_mapper;
}

#endif
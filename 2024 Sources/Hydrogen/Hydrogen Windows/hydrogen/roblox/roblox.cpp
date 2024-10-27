#include "roblox.hpp"

#include <common_includes.hpp>

#include <managers/interface/interface.hpp>
#include <roblox/scheduler/scheduler.hpp>
#include <managers/hook/hook.hpp>
#include <utilities/rand.hpp>

#include "memory/luau.hpp"
#include "env/env.hpp"

#include <Luau/Compiler.h>
#include <Luau/BytecodeBuilder.h>
#include <Luau/lstate.h>
#include <Luau/lua.h>
#include <Luau/lapi.h>
#include <Luau/lualib.h>

#include <TlHelp32.h>
#include <psapi.h>
#include <lmem.h>

namespace roblox
{
	std::once_flag init;
	std::uint32_t  __fastcall on_waiting_scripts_job_hk( const job_t* job, std::uintptr_t edx, const std::uintptr_t stats )
	{
		std::call_once( init, [ ] ( )
			{
				logger( )->info( "Hooking renderer\n" );

				managers::interface_manager_t::get( ).initiate( );

				logger( )->info( "Preparing state" );

				rbx_manager_t::get( ).prepare_hydrogen_state( );

			} );

		rbx_manager_t::get( ).execute_queued_script( );

		return managers::hook_mgr.hooks.on_waiting_script_job_step.call_original( job, edx, stats );
	}

	void rbx_manager_t::initiate( )
	{
		logger( )->info( "\"Bypassing Yara\" :trollface:" );

		HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
		if ( snapshot == INVALID_HANDLE_VALUE )
			return;

		bool first = true;

		PROCESSENTRY32 process_info;
		process_info.dwSize = sizeof( process_info );

		if ( Process32First( snapshot, &process_info ) )
		{
			do
			{
				if ( !lstrcmp( process_info.szExeFile, L"RobloxPlayerBeta.exe" ) )
				{
					auto handle = OpenProcess( PROCESS_ALL_ACCESS, FALSE, process_info.th32ProcessID );

					PROCESS_MEMORY_COUNTERS pmc;

					if ( GetProcessMemoryInfo( handle, &pmc, sizeof( pmc ) ) )
					{
						if ( handle && ( pmc.PeakPagefileUsage / 1000000 ) < 80 )
						{
							TerminateProcess( handle, 0 );

							CloseHandle( handle );
						}
					}
				}
			} while ( Process32Next( snapshot, &process_info ) );
		}
		CloseHandle( snapshot );

		logger( )->info( "Starting scheduler\n" );

		auto scheduler = roblox::task_scheduler_t::get( );

		logger( )->info( "Getting job\n" );

		auto whsjob = scheduler->get_job( "WaitingHybridScriptsJob" );

		logger( )->info( "Job {}", whsjob->name );

		script_context = *reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( whsjob ) + off::script_context::wsj_offset );
		rbx_thread = *reinterpret_cast< sc_globalstate<lua_State*> * >( script_context + off::script_context::global_state );

		managers::hook_mgr.hooks.on_waiting_script_job_step.hook_virtual_function(
			{ reinterpret_cast< std::uintptr_t >( whsjob ), on_waiting_scripts_job_hk },
			{ 0x5, 0x7 } );
	}

	bool rbx_manager_t::push_script( lua_State* state, const std::string& script, const char* chunkname ) const
	{
		struct : Luau::BytecodeEncoder
		{
			uint8_t encodeOp( const std::uint8_t op ) override
			{
				return op * 227;
			}
		} bytecode_encoder;

		const auto bytecode = Luau::compile( script, { 1, 2, 0 }, { }, &bytecode_encoder );

		const auto res = luau_load( state, chunkname ? chunkname : utilities::rand::generate_random( 15 ).c_str( ), bytecode.c_str( ), bytecode.size( ), 0 ) != 1;

		//lua_ref( state, -1 );

		return res;
	}

	void rbx_manager_t::execute_script( const std::string& script ) const
	{
		try
		{
			auto new_thread = lua_newthread( hyd_thread );
			luaL_sandboxthread( new_thread );
			fn::set_system_identity( new_thread, hydro_context );

			lua_getglobal( new_thread, "Instance" );
			lua_getfield( new_thread, -1, "new" );
			lua_pushstring( new_thread, "LocalScript" );
			lua_call( new_thread, 1, 1 );
			lua_setglobal( new_thread, "script" );
			lua_pop( new_thread, 1 );

			new_thread = lua_newthread( new_thread );

			if ( !push_script( new_thread, script ) )
			{
				const auto err = lua_tostring( new_thread, -1 );

				logger( )->error( err );

				return;
			}

			//set_thread_identity( new_thread, 8 );

			if ( managers::off_manager_t::get( ).resume( script_context, new_thread, 0 ) == LUA_ERRRUN )
			{
				const auto err = lua_tostring( new_thread, -1 );

				logger( )->error( err );
			}
		}
		catch ( const std::exception& e )
		{
			logger( )->error( e.what( ) );
		}
	}

	void rbx_manager_t::execute_queued_script( )
	{
		std::unique_lock< std::mutex > locker{ script_mutex };

		if ( !script_queue.empty( ) )
		{
			execute_script( script_queue.front( ) );

			script_queue.pop_front( );
		}

		locker.unlock( );

		/*std::lock_guard< std::mutex > yield_locker{ yield_mutex };

		if ( !yield_queue.empty( ) )
		{
			yield_queue.front( )( );

			yield_queue.pop( );
		}*/

	}

	void rbx_manager_t::prepare_hydrogen_state( )
	{
		hyd_thread = lua_newthread( rbx_thread );

		luaL_sandboxthread( hyd_thread );

		env::init( hyd_thread );
	}

	void rbx_manager_t::queue_script( const std::string& script )
	{
		std::unique_lock< std::mutex > locker{ script_mutex };

		script_queue.push_back( script );

		locker.unlock( );
	}

	lua_State* rbx_manager_t::get_rbx_thread( ) const
	{
		return rbx_thread;
	}

	lua_State* rbx_manager_t::get_hydro_thread( ) const
	{
		return hyd_thread;
	}

	std::uintptr_t rbx_manager_t::get_script_context( ) const
	{
		return script_context;
	}

	rbx_manager_t& rbx_manager_t::get( )
	{
		static rbx_manager_t mgr;

		return mgr;
	}

}
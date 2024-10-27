#include <algorithm>
#include <minhook/minhook.hpp>
#include <luau/lualib.h>
#include <luau/lstate.h>
#include <luau/bytecode/bytecode.hpp>
#include "environment.hpp"
#include "utilities/thread/thread.hpp"
#include "utilities/instance/instance.hpp"

#include <luau/lua.h>
#include <luau/lobject.h>
#include <rbx/rbx_structs.hpp>

using c_environment = rbx::c_environment;

std::int32_t c_environment::wsj_step( std::uint8_t* const job,
	void* reg, const std::int32_t stats )
{
	/* TODO: Refresh On Tp */
	if ( auto& hydrogen_state = g_environment.m_hydrogen_state; !hydrogen_state )
	{
		g_environment.m_script_context 
			= *reinterpret_cast< std::uint8_t** >
				( job + task_scheduler::offs::script_context );

		g_environment.m_roblox_state
			= *reinterpret_cast< sc_globalstate< lua_State* >* >
				( g_environment.m_script_context +
					task_scheduler::offs::global_state );

		const auto hook_game_metamethod = [ ]( lua_State* L, const char* metamethod, lua_CFunction func, lua_CFunction& orig )
		{
			lua_getglobal( L, "game" );
			luaL_getmetafield( L, -1, metamethod );

			Closure* closure = reinterpret_cast< Closure* >( const_cast< void* >( lua_topointer( L, -1 ) ) );

			orig = closure->c.f;

			closure->c.f = func;
			lua_pop( L, 2 );
		};

		hook_game_metamethod( 
			g_environment.m_roblox_state, 
			"__namecall", 
			rbx::environment::c_env::on_namecall_bridge, 
			rbx::environment::c_env::m_on_namecall_bridge 
		);

		hook_game_metamethod(
			g_environment.m_roblox_state,
			"__index",
			rbx::environment::c_env::on_index_bridge,
			rbx::environment::c_env::m_on_index_bridge
		);

		hook_game_metamethod(
			g_environment.m_roblox_state,
			"__newindex",
			rbx::environment::c_env::on_newindex_bridge,
			rbx::environment::c_env::m_on_newindex_bridge
		);

		hydrogen_state = lua_newthread( g_environment.m_roblox_state );
		lua_ref( g_environment.m_roblox_state, -1 );
		lua_pop( g_environment.m_roblox_state, 1 );

		/* TODO: Wrapper func */
		rbx::environment::utilities
			::c_thread::set_thread_identity( hydrogen_state, 8 );

		luaL_sandboxthread( hydrogen_state );

		sol::state_view sv{ hydrogen_state };
		sv[ "_G" ] = sv.create_table( );

		// TODO: Use a damn initializer list or smth
		g_environment.m_env.init( hydrogen_state );
		g_environment.m_table.init( hydrogen_state );
		g_environment.m_script.init( hydrogen_state );
		g_environment.m_input.init( hydrogen_state );
		g_environment.m_reflection.init( hydrogen_state );
		g_environment.m_console.init( hydrogen_state );
		g_environment.m_file.init( hydrogen_state );
		g_environment.m_cache.init( hydrogen_state );
		g_environment.m_misc.init( hydrogen_state );
		g_environment.m_debug.init( hydrogen_state );
		g_environment.m_hooking.init( hydrogen_state );
		g_environment.m_drawing.init( hydrogen_state );
	}

	g_environment.m_scripts.process( );
	g_environment.m_yields.process( );

	return reinterpret_cast< decltype( &wsj_step ) >
		( g_environment.orig_wsj.at( 5 ) )( job, reg, stats );
}

std::int32_t __fastcall rbx::c_environment::whsj_ctor( const task_scheduler::structs::job_t* const job, void* sc )
{
	const auto res = reinterpret_cast< decltype( &whsj_ctor ) >( g_environment.whsj_ctor_orig )( job, sc );

	g_environment.m_script_context = nullptr;
	g_environment.m_hydrogen_state = nullptr;
	g_environment.m_roblox_state = nullptr;

	g_environment.init( job );

	return res;
}

lua_Status c_environment::resume( lua_State* L, const std::size_t nargs ) const
{
	refs::ThreadRef thread_ref{ L };

	return lua_Status( script_context::offs
		::resume( m_script_context, reinterpret_cast< void** >( &thread_ref ), nargs, 0, 0 ) );
}

std::uint8_t* rbx::c_environment::get_script_context( ) const
{
	return m_script_context;
}

lua_State* c_environment::get_hydrogen_state( ) const
{
	return m_hydrogen_state;
}

lua_State* c_environment::get_roblox_state( ) const
{
	return m_roblox_state;
}

void c_environment::execute( const std::string& script, const std::optional< lua_State* > state ) const
{
	const auto L = state ? *state : lua_newthread( m_hydrogen_state );

	if ( !state )
	{
		lua_pop( m_hydrogen_state, 1 );

		luaL_sandboxthread( L );

		sol::state_view sv{ L };

		// TODO: store orig addr at start for Instance.new as it can be hooked and script can fail to be made
		sv[ "script" ]
			= sv[ "Instance" ][ "new" ]( "LocalScript" );
	}

	if ( ::luau::g_bytecode.load( L, script, std::nullopt ) )
	{
		resume( L, 0 );
		lua_pop( L, lua_gettop( L ) );
	}
}

void c_environment::init( const task_scheduler::structs::job_t* const job )
{
	static std::once_flag flag;
	std::call_once( flag, [ & ]( )
	{
		MH_CreateHook( task_scheduler::offs::whsj_ctor, &whsj_ctor, &whsj_ctor_orig );
		MH_EnableHook( task_scheduler::offs::whsj_ctor );
	} );

	const PLH::VFuncMap map = {
		{ 5, reinterpret_cast< uint64_t >( &wsj_step ) },
	};

	wsj_hook = std::make_unique<
		PLH::VTableSwapHook >( reinterpret_cast< uint64_t >( job ),
			map, &orig_wsj );

	if ( !wsj_hook->hook( ) )
		MessageBoxA( nullptr, "Please open a support ticket\nHydrogen Error Code: 10", "Hydrogen Error", MB_OK | MB_ICONERROR );
}

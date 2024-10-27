#pragma once

#include <optional>
#include <unordered_map>
#include <luau/lua.h>
#include <rbx/rbx.hpp>
#include "scripts/scripts.hpp"
#include "yields/yields.hpp"
#include "functions/script/script.hpp"
#include "functions/env/env.hpp"
#include "functions/table/table.hpp"
#include "functions/input/input.hpp"
#include "functions/reflection/reflection.hpp"
#include "functions/console/console.hpp"
#include "functions/file/file.hpp"
#include "functions/cache/cache.hpp"
#include "functions/misc/misc.hpp"
#include "functions/debug_lib/debug_lib.hpp"
#include "functions/hooking/hooking.hpp"
#include "functions/drawing/drawing.hpp"

#include <sol/sol.hpp>

#include <polyhook2/Virtuals/VTableSwapHook.hpp>

namespace rbx
{
	class c_environment
	{
		std::uint8_t* m_script_context;
		lua_State *m_hydrogen_state, *m_roblox_state;

		environment::c_env m_env;
		environment::c_table m_table;
		environment::c_script m_script;
		environment::c_input m_input;
		environment::c_reflection m_reflection;
		environment::c_console m_console;
		environment::c_file m_file;
		environment::c_cache m_cache;
		environment::c_misc m_misc;
		environment::c_debug m_debug;
		environment::c_hooking m_hooking;
		environment::c_drawing m_drawing;

		static std::int32_t __fastcall wsj_step( std::uint8_t* const job,
			void* reg, const std::int32_t stats );

		static std::int32_t __fastcall whsj_ctor( const task_scheduler::structs::job_t* const job, void* sc );

		void* whsj_ctor_orig;

		PLH::VFuncMap orig_wsj{ };

		std::unique_ptr< PLH::VTableSwapHook > wsj_hook;

	public:
		environment::c_scripts m_scripts;
		environment::c_yields m_yields;

		lua_Status resume( lua_State* state, const std::size_t nargs ) const;
		
		/* TODO: Create Script Context Class */
		[[ nodiscard ]] std::uint8_t* get_script_context( ) const;

		[[ nodiscard ]] lua_State* get_hydrogen_state( ) const;
		[[ nodiscard ]] lua_State* get_roblox_state( ) const;

		void execute( const std::string& script, const std::optional< lua_State* > state = {} ) const;

		void init( const task_scheduler::structs::job_t* const job );
	} inline g_environment;
}
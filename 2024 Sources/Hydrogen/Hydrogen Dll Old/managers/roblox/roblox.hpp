#pragma once

#include <dependencies/common_includes.hpp>

#include <queue>
#include <unordered_map>

#include <functional>

struct lua_State;
struct Closure;
typedef int ( *lua_CFunction )( lua_State* L );
typedef int ( *lua_Continuation )( lua_State* L, int status );

namespace managers
{
	class roblox_manager_t
	{
		std::queue< std::string > queued_scripts;

		std::queue< std::function< void( ) > > yield_queue;

		std::unordered_map< Closure*, Closure* > newcclosure_map;

	public:
		std::mutex script_mutex, yield_mutex;

		std::unordered_map< Closure*, lua_CFunction > function_map;
		std::unordered_map< std::uintptr_t, std::uintptr_t > registered_connections;

		struct
		{
			lua_State* state;

			lua_State* thread;

		} globals;

		void* script_context;

		void initiate( );

		void queue_yield_callback( std::function< void( ) > function );

		void execute_queued_script( );

		void setup_main_thread( );

		void set_state_identity( std::size_t identity ) const;

		void set_thread_identity( lua_State* state, std::size_t identity ) const;

		std::size_t get_thread_identity( lua_State* state ) const;

		bool push_script( lua_State* state, const std::string& script, const char* chunkname = nullptr ) const;

		void execute_script( const std::string& script ) const;

		void queue_script( const std::string& script );

		void pushcclosure( lua_State* state, lua_CFunction fn, int nup );

		void pushcclosurek( lua_State* state, lua_CFunction fn, int nup, lua_Continuation cont );

		int resume( lua_State* state, int narg ) const;

		void add_newcclosure( Closure* closure, Closure* value_closure );

		void swap_newcclosure( Closure* closure, Closure* value_closure );

		std::optional< Closure* > get_closure( Closure* closure ) const;
	};
}
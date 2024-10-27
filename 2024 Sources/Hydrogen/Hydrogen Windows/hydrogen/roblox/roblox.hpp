#pragma once
#include <string>
#include <deque>
#include <mutex>

#include <lua.h>

constexpr auto hydro_context = 8;

namespace roblox
{
	class rbx_manager_t
	{
		std::mutex script_mutex;

		std::deque<std::string> script_queue;
		std::uintptr_t script_context = 0;

		lua_State* rbx_thread = nullptr;
		lua_State* hyd_thread = nullptr;

	public:
		void initiate( );

		[[nodiscard]] std::uintptr_t get_script_context( ) const;
		[[nodiscard]] lua_State* get_rbx_thread( ) const;
		[[nodiscard]] lua_State* get_hydro_thread( ) const;
		[[nodiscard]] static rbx_manager_t& get( );

		bool push_script( lua_State* state, const std::string& script, const char* chunkname = nullptr ) const;
		void execute_script( const std::string& script ) const;
		void queue_script( const std::string& );
		void execute_queued_script( );

		void prepare_hydrogen_state( );
	};
}
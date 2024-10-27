#pragma once

#include <dependencies/common_includes.hpp>

#include <Luau/lstate.h>
#include <Luau/lua.h>
#include <Luau/lapi.h>
#include <Luau/luacode.h>
#include <Luau/lualib.h>

#include <Luau/Luau/BytecodeBuilder.h>
#include <Luau/Luau/Compiler.h>

#include <array>
#include <queue>
#include <tuple>
#include <unordered_map>

#include <dependencies/ghc/filesystem.hpp>

namespace fs = ghc::filesystem;

namespace managers
{
    class roblox_manager_t
    {
        void* menu_job, *current_job;

        std::queue< std::string > queued_scripts;

        std::queue< std::string > teleport_queue_script;

        std::queue< std::function< void() > > yield_queue;

        std::unordered_map< Closure*, Closure* > newcclosure_map;

    public: //TODO CLEAR AT TELEPORT
        std::unordered_map< std::uintptr_t, std::uintptr_t > registered_connections;
        std::unordered_map< Closure*, lua_CFunction > function_map;

        std::vector<Proto*> protos;

        std::mutex script_mutex, yield_mutex, teleport_mutex, keyboard_mutex;

        void* script_context;

        lua_State* state, *thread;

        int textbox_ref;
        std::uintptr_t textbox_ptr;

        void initiate();

        void focus_keyboard( bool propagate_to_rbx = true );
		
		void unfocus_keyboard( bool propagate_to_rbx = true );

        void set_keyboard_text( const std::string& text );

        void queue_yield_callback(std::function< void() > function);

        bool is_keyboard_focused( );

        void setup_environment(void* waiting_scripts_job);

        void callback(void* waiting_scripts_job);

        void set_thread_identity(lua_State* state, std::size_t identity) const;

        std::size_t get_thread_identity(lua_State* state) const;

        void push_teleport_script(const std::string& script);

        bool push_script(lua_State* state, const std::string& script, const char* chunkname = nullptr) const;

        void execute_script(const std::string& script) const;

        void queue_script( const std::string& script );

        void sandbox_thread(lua_State* state) const;

        template< std::size_t sz >
        bool check_types(lua_State* state, int index, std::array< lua_Type, sz > arr) const
        {
            for (auto i = 0u; i < sz; i++)
            {
                if (lua_type(state, index) == arr[i]) 
                    return true;                    
            }

            return false;
        }

        int resume(lua_State* state, int narg) const;

        void add_newcclosure(Closure* closure, Closure* value_closure);

        void swap_newcclosure(Closure* closure, Closure* value_closure);

        std::optional< Closure* > get_closure(Closure* closure) const;

        std::uintptr_t get_property(std::uintptr_t class_descriptor, std::uintptr_t property_descriptor);

        fs::path hydrogen_dir;

        std::atomic_bool game_joined = false;
        std::atomic_bool keyboard_focused = false;
    };

    extern roblox_manager_t roblox_mgr;
}


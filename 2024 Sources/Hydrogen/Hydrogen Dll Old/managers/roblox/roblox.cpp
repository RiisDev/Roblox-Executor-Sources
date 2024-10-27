#include "roblox.hpp"

#include <managers/roblox/structures/script_context/script_context.hpp>
#include <managers/roblox/structures/task_scheduler/task_scheduler.hpp>

#include <managers/managers.hpp>

#include <Luau/lstate.h>
#include <Luau/lua.h>
#include <Luau/lapi.h>
#include <Luau/luacode.h>

#include <Luau/Luau/BytecodeBuilder.h>
#include <Luau/Luau/Compiler.h>
#include <utilities/io.hpp>
#include <dependencies/Luau/lualib.h>

std::uint32_t  __fastcall on_waiting_scripts_job_hk( const job_t* job, std::uintptr_t edx, const std::uintptr_t stats )
{
    managers::mgrs.roblox_mgr.execute_queued_script( );

    return managers::mgrs.hook_mgr.hooks.on_waiting_script_job_step.call_original( job, edx, stats );
}

void managers::roblox_manager_t::initiate()
{
    //const auto sc = roblox::structures::script_context_t::get( );
    const auto task_scheduler = roblox::structures::task_scheduler_t::get();

    const auto waiting_script_job = roblox::structures::task_scheduler_t::get()
        ->get_jobs_by_name("WaitingHybridScriptsJob").back();

    const auto sc = *reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(waiting_script_job) + 0x130);

    /* Todo auto update this/move to offset manager */
    globals.state = reinterpret_cast<lua_State*>(*reinterpret_cast<std::uintptr_t*>(sc + 0x14C) - (sc + 0x14C));
    script_context = reinterpret_cast<void*>(sc);

    managers::mgrs.hook_mgr.hooks.on_waiting_script_job_step.hook_virtual_function(
        { reinterpret_cast<std::uintptr_t>(waiting_script_job), on_waiting_scripts_job_hk },
        { 0x5, 0x7 });
}

void managers::roblox_manager_t::queue_yield_callback( std::function< void() > function )
{
    std::unique_lock< std::mutex > locker { yield_mutex };
    yield_queue.push( function );
}

void managers::roblox_manager_t::execute_queued_script( )
{
    std::unique_lock< std::mutex > locker{ script_mutex };

    if ( !queued_scripts.empty( ) )
    {
        execute_script( queued_scripts.front( ) );

        queued_scripts.pop( );
    }

    locker.unlock( );

    std::lock_guard< std::mutex > yield_locker{ yield_mutex };

    if ( !yield_queue.empty( ) )
    {
        yield_queue.front( )( );

        yield_queue.pop( );
    }

}

void managers::roblox_manager_t::execute_script( const std::string& script ) const
{
    try
    {
        const auto& offset_mgr = managers::mgrs.offset_mgr;

        const auto new_thread = lua_newthread( globals.thread );

        luaL_sandboxthread( new_thread );

        lua_getglobal( new_thread, "Instance" );
        lua_getfield( new_thread, -1, "new" );
        lua_pushstring( new_thread, "LocalScript" );
        lua_call( new_thread, 1, 1 );
        lua_setglobal( new_thread, "script" );
        lua_pop( new_thread, 1 );

        if ( !push_script( new_thread, script ) )
        {
            const auto err = lua_tostring( new_thread, -1 );

            utilities::io::log( utilities::io::log_options_t::error, err );

            return;
        }

        set_thread_identity( new_thread, 8 );

        if ( resume( new_thread, 0 ) == LUA_ERRRUN )
        {
            const auto err = lua_tostring( new_thread, -1 );

            utilities::io::log( utilities::io::log_options_t::error, err );
        }
    }
    catch ( const std::exception& e )
    {
        utilities::io::log( utilities::io::log_options_t::error, e.what( ) );
    }
}

void managers::roblox_manager_t::queue_script( const std::string& script )
{
	std::unique_lock< std::mutex > locker{ script_mutex };

	queued_scripts.push( script );
	
	locker.unlock( );
}

void managers::roblox_manager_t::pushcclosure( lua_State* state, lua_CFunction fn, int nup )
{
    lua_pushcclosure( state, fn, nullptr, nup );
}

void managers::roblox_manager_t::pushcclosurek( lua_State* state, lua_CFunction fn, int nup, lua_Continuation cont )
{
    lua_pushcclosurek( state, fn, nullptr, nup, cont );
}

class LiveThreadRef
{
    int threadId;
    lua_State* L;
    friend class WeakThreadRef;
    friend class ThreadRef;
public:
    LiveThreadRef(lua_State* thread)
        :L(thread)
    {
        lua_pushthread(L);

        threadId = lua_ref(L, -1);

        lua_pop(L, 1);
    }
    ~LiveThreadRef()
    {
        //lua_unref(ScriptContext::getGlobalState(L), LUA_REGISTRYINDEX, threadId);
    }
    bool empty() const {
        return L == NULL;
    }
    lua_State* thread() const {
        return L;
    }

};

class ThreadRef
{
    LiveThreadRef* liveThreadRef;
    friend class WeakThreadRef;
public:

    ThreadRef(LiveThreadRef* liveThreadRef) :liveThreadRef(liveThreadRef) {}

    ThreadRef() {}
    ThreadRef(lua_State* thread)
        :liveThreadRef(new LiveThreadRef(thread)) {}
    lua_State* get() const {
        return liveThreadRef ? liveThreadRef->thread() : NULL;
    }
    operator lua_State* () const
    {
        return get();
    }
    bool empty() const {
        return liveThreadRef && liveThreadRef->thread() != NULL;
    }
};

int managers::roblox_manager_t::resume(lua_State* state, int narg) const
{
    LiveThreadRef ltr{ state };

    ThreadRef ref{ &ltr };

    return mgrs.offset_mgr.roblox.scriptcontext_resume(script_context, &ref, narg, 0, 0);
}

void managers::roblox_manager_t::add_newcclosure( Closure* key_closure, Closure* value_closure )
{
    newcclosure_map.insert( { key_closure, value_closure } );
}

void managers::roblox_manager_t::swap_newcclosure( Closure* closure, Closure* value_closure )
{
    newcclosure_map[ closure ] = value_closure;
}

std::optional< Closure* > managers::roblox_manager_t::get_closure( Closure* closure ) const
{
    if ( const auto cl = newcclosure_map.find( closure ); cl != newcclosure_map.end( ) )
        return cl->second;
        
   return std::nullopt;
}

void managers::roblox_manager_t::set_state_identity( std::size_t level ) const
{
   *reinterpret_cast< std::size_t* >(  
           static_cast< std::uint8_t* >( globals.thread->userdata ) + 0x18 ) = level;
}

void managers::roblox_manager_t::set_thread_identity( lua_State* state, std::size_t identity ) const
{
    *reinterpret_cast< std::size_t* >(
        static_cast< std::uint8_t* >( state->userdata ) + 0x18 ) = identity;

    /* set camera movement mode for touch devices */
    *reinterpret_cast< std::uintptr_t* >( *reinterpret_cast< std::uintptr_t* >( __readfsdword( 0x2C ) ) + 0xC8 ) = identity;
}

std::size_t managers::roblox_manager_t::get_thread_identity( lua_State* state ) const
{
    return *reinterpret_cast< std::size_t* >(
        static_cast< std::uint8_t* >( state->userdata ) + 0x18 );
}

bool managers::roblox_manager_t::push_script( lua_State* state, const std::string& script, const char* chunkname ) const
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

void managers::roblox_manager_t::setup_main_thread( )
{
    const auto new_thread = lua_newthread( globals.state );

    luaL_sandboxthread( new_thread );

    lua_pushnil(new_thread);
    lua_setglobal(new_thread, "Version" );

    lua_newtable(new_thread);
    lua_setglobal(new_thread, "shared");
    lua_newtable(new_thread);
    lua_setglobal(new_thread, "_G");

    globals.thread = new_thread;
}
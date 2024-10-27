#include "roblox.hpp"

#include "environment/environment.hpp"

#include "checks/networked_dm/networked_dm.hpp"
#include "checks/trust_check/trust_check.hpp"

#include <managers/managers.hpp>
#include <dependencies/Luau/ldo.h>

#include <managers/roblox/environment/security/security.hpp>

std::uint32_t on_waiting_scripts_job_hk(void* job, const std::uintptr_t stats)
{
	managers::roblox_mgr.callback(job);

	return managers::hook_mgr.hooks.on_waiting_script_job_step(job, stats);
}

int on_game_leave( int a1, uint64_t a2 ) {
	
	managers::menu_mgr.game_joined = false;

	return managers::hook_mgr.hooks.on_game_leave( a1, a2 );
}

int on_unfocus_keyboard( void* textbox, bool submitted, int a3, int a4 ) {

	managers::utility_mgr.log( "Unfocus invoked %p %i", textbox, submitted );

	auto ptr = reinterpret_cast< std::uintptr_t >( textbox );
	if ( ptr == managers::roblox_mgr.textbox_ptr && submitted ) {
		return 0;
	}
	return managers::hook_mgr.hooks.unfocus_keyboard_internal( textbox, submitted, a3, a4 );
}

void managers::roblox_manager_t::initiate()
{
	roblox::checks::networked_dm::bypass();
	roblox::checks::trust_check::bypass();

	const auto addr = managers::memory_mgr.rebase(offsets::wsj_step);

	managers::hook_mgr.hooks.on_waiting_script_job_step.hook(
		reinterpret_cast<void*>(managers::memory_mgr.rebase(offsets::wsj_step)),
			reinterpret_cast<void*>(&on_waiting_scripts_job_hk) );

	managers::hook_mgr.hooks.on_game_leave.hook(
		reinterpret_cast< void* >( managers::memory_mgr.rebase( offsets::on_game_leave ) ),
		reinterpret_cast< void* >( &on_game_leave ) );

	//managers::hook_mgr.hooks.unfocus_keyboard_internal.hook(
	//	reinterpret_cast< void* >( managers::memory_mgr.rebase( offsets::unfocus_internal ) ),
	//	reinterpret_cast< void* >( &on_unfocus_keyboard ) );

	//managers::hook_mgr.hooks.focus_keyboard.hook(
	//	reinterpret_cast< void* >( managers::memory_mgr.rebase( offsets::focus_textbox ) ),
	//	reinterpret_cast< void* >( &on_focus_keyboard ) );
}

void managers::roblox_manager_t::focus_keyboard( bool propagate_to_rbx )
{
	std::unique_lock<std::mutex> locker{ keyboard_mutex };
	if ( keyboard_focused ) return;

	keyboard_focused = true;

	locker.unlock( );

	queue_yield_callback(
		[ & ]( )
	{
		managers::utility_mgr.log<ANDROID_LOG_ERROR>( "CaptureFocus" );
		//managers::function_mgr.focus_textbox( ( void* )textbox_ptr );
	} );
}

void managers::roblox_manager_t::unfocus_keyboard( bool propagate_to_rbx )
{
	std::unique_lock<std::mutex> locker{ keyboard_mutex };
	if ( !keyboard_focused ) return;

	keyboard_focused = false;

	locker.unlock( );

	queue_yield_callback(
		[ & ]( )
	{
		managers::utility_mgr.log<ANDROID_LOG_ERROR>( "ReleaseFocus" );
		//managers::function_mgr.unfocus_textbox( ( void* )textbox_ptr, false );
	} );
}

void managers::roblox_manager_t::set_keyboard_text( const std::string& text )
{
	//queue_yield_callback(
	//	[ & ] ( )
	//	{
	//		const auto top = lua_gettop( thread );
	//		lua_rawgeti( thread, LUA_REGISTRYINDEX, textbox_ref );
	//		lua_pushstring( thread, text.c_str( ) );
	//		lua_setfield( thread, -2, "Text" );
	//		lua_settop( thread, top );
	//	} );
}

void managers::roblox_manager_t::queue_yield_callback(std::function<void()> function)
{
	std::unique_lock< std::mutex > locker{ yield_mutex };
	yield_queue.push(function);
}

bool managers::roblox_manager_t::is_keyboard_focused( )
{
	std::unique_lock< std::mutex > locker{ keyboard_mutex };
	return keyboard_focused;
}

void managers::roblox_manager_t::setup_environment(void* waiting_scripts_job)
{
	hydrogen_dir = managers::jni_mgr.get_external_file_dir( ) + "/hydrogen";
	managers::utility_mgr.log( "path %s", hydrogen_dir.c_str( ) );

	if ( !fs::exists( hydrogen_dir ) )
		fs::create_directory( hydrogen_dir );

	const auto sc = *reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(waiting_scripts_job) + offsets::wsj_sc_offset);

	script_context = reinterpret_cast<void*>(sc);

	state = reinterpret_cast<lua_State*>((sc + offsets::sc_lua_state) + *reinterpret_cast< std::uintptr_t* >(sc + offsets::sc_lua_state));

	const auto new_thread = lua_newthread(state);

	sandbox_thread(new_thread);

	lua_newtable(new_thread);
	lua_setglobal(new_thread, "_G");

	set_thread_identity(new_thread, 8);

	thread = new_thread;

	roblox::environment::initiate();

	managers::utility_mgr.log( "Environment Setup" );

	managers::menu_mgr.game_joined = true;

	if ( !roblox::environment::security::authed )
	{
		std::ifstream stream( hydrogen_dir / "key" );

		managers::utility_mgr.log( "1" );

		if ( stream.is_open( ) )
		{
			managers::utility_mgr.log( "2" );

			std::string output;
			std::copy( std::istreambuf_iterator< char >( stream ), std::istreambuf_iterator< char >( ), std::back_inserter( output ) );

			std::string key{ output.data( ), output.size( ) };

			lua_pushstring( thread, key.c_str( ) );
			lua_setfield( thread, LUA_REGISTRYINDEX, "PEOU2IGKB4VKSBGPYQADA" );
		}
	}

	execute_script(
#include <scripts/bitlib.lua>
	);


	//execute_script( R"(
	//	while not game:GetService"Players".LocalPlayer do wait() end

	//	game:GetService"Players".LocalPlayer.Chatted:Connect(function(msg)
	//		-- make sure msg begins with '/e ' and strip it from the message
	//		if msg:sub(1, 3) == '/e ' then
	//			msg = msg:sub(4)
	//		else
	//			return
	//		end

	//		local f,err = loadstring(msg)
	//		if not f then
	//			warn("Compilation failed bozo:",err)
	//		else
	//			f()
	//		end
	//	end)

	//	print("hacker script initiated!")
	//)" );



	managers::utility_mgr.log( "Authed %i", roblox::environment::security::authed );

	if ( roblox::environment::security::authed )
	{

		managers::function_mgr.lua_pushcclosurek( managers::roblox_mgr.thread, roblox::environment::security::run_script, nullptr, 0, nullptr );
		lua_setfield( managers::roblox_mgr.thread, LUA_REGISTRYINDEX, "GTX4CHQYWURD3CR12JF" );

		managers::roblox_mgr.execute_script(
#include <scripts/ui.lua>
		);
	}
	else
	{
		execute_script(
#include <scripts/keysys.lua>
		);
	}
}

void managers::roblox_manager_t::callback(void* waiting_scripts_job)
{
	if (!menu_job)
		menu_job = waiting_scripts_job;

	if (waiting_scripts_job == menu_job)
		return;

	if (!current_job)
	{
		current_job = waiting_scripts_job;

		setup_environment(waiting_scripts_job);
	}

	if (current_job != waiting_scripts_job)
	{
		current_job = waiting_scripts_job;

		setup_environment(waiting_scripts_job);

		managers::utility_mgr.log("Teleport Handled");

		std::unique_lock< std::mutex > locker{ teleport_mutex };

		std::unique_lock< std::mutex > locker2{ script_mutex };

		while (!teleport_queue_script.empty())
		{
			queued_scripts.push(teleport_queue_script.front());

			teleport_queue_script.pop();
		}

		return; //handle tp
	}

	std::unique_lock< std::mutex > locker{ script_mutex };

	if (!queued_scripts.empty())
	{
		execute_script(queued_scripts.front());

		queued_scripts.pop();
	}

	locker.unlock();

	std::lock_guard< std::mutex > yield_locker{ yield_mutex };

	if (!yield_queue.empty())
	{
		yield_queue.front()();

		yield_queue.pop();
	}
}

void managers::roblox_manager_t::set_thread_identity(lua_State* state, std::size_t identity) const
{
	*reinterpret_cast<std::size_t*>(
		static_cast<std::uint8_t*>(state->userdata) + offsets::identity) = identity;

	*managers::function_mgr.get_thread_identity() = identity;
}

std::size_t managers::roblox_manager_t::get_thread_identity(lua_State* state) const
{
	return *reinterpret_cast<std::size_t*>(
		static_cast<std::uint8_t*>(state->userdata) + offsets::identity);
}

void managers::roblox_manager_t::push_teleport_script(const std::string& script)
{
	std::unique_lock< std::mutex > locker{ teleport_mutex };

	teleport_queue_script.push(script);
}

//todo random chunkname with optional chunkname
bool managers::roblox_manager_t::push_script(lua_State* state, const std::string& script, const char* chunkname) const
{
	struct : Luau::BytecodeEncoder
	{
		uint8_t encodeOp(const std::uint8_t op) override
		{
			return op * 227;
		}
	} bytecode_encoder;

	const auto bytecode = Luau::compile(script, { 1, 1, 0 }, { }, &bytecode_encoder);

	const auto res = managers::function_mgr.luau_load(state, "@hydrogen", bytecode.c_str(), bytecode.size(), 0) != 1;

	if (res)
	{
		const auto closure = clvalue(managers::function_mgr.index2addr(state, -1));
		managers::roblox_mgr.function_map.insert({ closure, nullptr });

		std::function<void(Proto*)> add_protos = [&](Proto* p)
		{
			for (auto i = 0u; i < p->sizep; ++i)
				add_protos(p->p[i]);

			managers::roblox_mgr.protos.push_back(p);
		};

		add_protos(closure->l.p);
	}

	return res;
}

void managers::roblox_manager_t::execute_script(const std::string& script) const
{
	try
	{
		const auto new_thread = lua_newthread(thread);

		sandbox_thread(new_thread);

		lua_getglobal(new_thread, "Instance");
		lua_getfield(new_thread, -1, "new");
		lua_pushstring(new_thread, "LocalScript");
		lua_call(new_thread, 1, 1);
		lua_setglobal(new_thread, "script");
		lua_pop(new_thread, 1);

		if (!push_script(new_thread, script))
		{
			const auto err = lua_tostring(new_thread, -1);

			utility_mgr.log<ANDROID_LOG_ERROR>(err);

			return;
		}

		set_thread_identity(new_thread, 8);

		if (resume(new_thread, 0) == LUA_ERRRUN)
		{
			const auto err = lua_tostring(new_thread, -1);

			managers::utility_mgr.log<ANDROID_LOG_ERROR>(err);
		}
	}
	catch (const std::exception& e)
	{
		utility_mgr.log<ANDROID_LOG_ERROR>(e.what());
	}
}

void managers::roblox_manager_t::queue_script( const std::string& script )
{
	std::unique_lock< std::mutex > locker{ script_mutex };
	queued_scripts.push( script );
}

void managers::roblox_manager_t::sandbox_thread(lua_State* state) const
{
	lua_createtable(state, 0, 0);
	lua_createtable(state, 0, 0);

	lua_pushstring(state, "The metatable is locked");
	lua_setfield(state, -2, "__metatable");

	lua_pushstring(state, "__index");
	lua_pushvalue(state, LUA_GLOBALSINDEX);

	lua_settable(state, -3);
	lua_setmetatable(state, -2); 

	lua_replace(state, LUA_GLOBALSINDEX);
}

int managers::roblox_manager_t::resume(lua_State* state, int narg) const
{
	struct live_thread_ref_t {
		int id;
		lua_State* thread;
	} live_thread_ref{ 0, state };

	struct {
		live_thread_ref_t* live_thread_ref;
	} thread_ref{ &live_thread_ref };

	return managers::function_mgr.script_context_resume(script_context, &thread_ref, narg, 0, 0);
}

void managers::roblox_manager_t::add_newcclosure(Closure* key_closure, Closure* value_closure)
{
	newcclosure_map.insert({ key_closure, value_closure });
}

void managers::roblox_manager_t::swap_newcclosure(Closure* closure, Closure* value_closure)
{
	newcclosure_map[closure] = value_closure;
}

std::optional< Closure* > managers::roblox_manager_t::get_closure(Closure* closure) const
{
	if (const auto cl = newcclosure_map.find(closure); cl != newcclosure_map.end())
		return cl->second;

	return std::nullopt;
}

std::uintptr_t managers::roblox_manager_t::get_property(std::uintptr_t class_descriptor, std::uintptr_t property_descriptor)
{
	auto end = *(std::uintptr_t*)(class_descriptor + 0x1CC);
	auto iter = *(std::uintptr_t*)(class_descriptor + 0x1C8);
	auto is_end = iter == end;
	std::uintptr_t v4 = 0;
	std::uintptr_t v16 = 0;
	if (iter != end)
	{
		v4 = *(std::uintptr_t*)(class_descriptor + 0x1D8);
		is_end = v4 == property_descriptor;
	}
	if (is_end)
		return 0;
	std::uintptr_t v12 = 0;
	std::uintptr_t v13 = 0xAAAAAAAB * ((end - iter) >> 2) - 1;
	std::uintptr_t v14 = property_descriptor + (property_descriptor >> 3);
	std::uintptr_t v15 = 0;
	while (1)
	{
		v15 = v14 & v13;
		v16 = *(std::uintptr_t*)(iter + 12 * v15);
		if (v16 == property_descriptor)
			break;
		if (v16 != v4)
		{
			++v12;
			v14 = v15 + v12;
			if (v12 <= v13)
				continue;
		}
		return 0;
	}
	auto v17 = iter + 12 * v15;
	if (!v17)
		return 0;
	
	return v17 + 4;
}


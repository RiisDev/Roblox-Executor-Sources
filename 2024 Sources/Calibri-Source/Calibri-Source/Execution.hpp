#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>
#include <queue>
#include <map>

#include "LuaInit.hpp"
#include "Enviroment.hpp"


namespace Execution
{
	static __int64 script_context = 0;

	static auto execute_script(__int64 state, std::string source) -> void
	{
		std::string bytecode = Luau::compile(source);

		if (bytecode.at(0) == 0)
		{
			Defs::rbx_print(3, bytecode.c_str() + 1);
			return;
		}
		else
		{
			std::string compressed = Utils::compress_bytecode(bytecode);

			lua_State* execution_thread = lua_newthread((lua_State*)state);

			Defs::rbx_luavmload((__int64)execution_thread, &compressed, "@Calibri", 0);
			auto function = *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>((__int64)execution_thread + Offsets::LuaState::top) - 16);
			auto proto = ProtoThings::deobfuscate_proto(function);
			ProtoThings::raise_proto(proto);

			static roblox_function spawn{};

			lua_getfield(execution_thread, LUA_GLOBALSINDEX, "spawn");

			Closure* cl = (Closure*)lua_topointer(execution_thread, -1);
			lua_CFunction func = cl->c.f;

			spawn = reinterpret_cast<roblox_function>(reinterpret_cast<std::intptr_t>(func));
			spawn(reinterpret_cast<std::uintptr_t>(execution_thread));
		}
	}


	static int scheduler_cycle()
	{
		if (!coderun_queue.empty())
		{
			std::string script = coderun_queue.front();
			execute_script(Enviroment::calibri_state, script);
			coderun_queue.pop();
		}

		return 0;
	}


	static auto scheduler_hook(lua_State* rl) -> int
	{
		scheduler_cycle();
		return 0;
	}


	static auto initialize_api(lua_State* script_state) -> void
	{
		lua_newtable(script_state);
		lua_setglobal(script_state, "_G");

		lua_newtable(script_state);
		lua_setglobal(script_state, "shared");

		lua_getfield(script_state, LUA_GLOBALSINDEX, "bit32");
		lua_setfield(script_state, LUA_GLOBALSINDEX, "bit");

		lua_pushboolean(script_state, true);
		lua_setfield(script_state, LUA_GLOBALSINDEX, "IS_LOADED");

		lua_pushboolean(script_state, true);
		lua_setfield(script_state, LUA_GLOBALSINDEX, "IS_CALIBRI");

		lua_pushboolean(script_state, true);
		lua_setfield(script_state, LUA_GLOBALSINDEX, "IsCalibri");

		lua_pushboolean(script_state, true);
		lua_setfield(script_state, LUA_GLOBALSINDEX, "CALIBRI_LOADED");

		lua_pushboolean(script_state, true);
		lua_setfield(script_state, LUA_GLOBALSINDEX, "CalibriLoaded");

		lua_pushboolean(script_state, true);
		lua_setfield(script_state, LUA_GLOBALSINDEX, "Calibri");

		Enviroment::push_global(script_state, xorstr_("getgenv"), Enviroment::Functions::Env::getgenv);
		Enviroment::push_global(script_state, xorstr_("getreg"), Enviroment::Functions::Env::getreg);
		Enviroment::push_global(script_state, xorstr_("getrenv"), Enviroment::Functions::Env::getrenv);
		Enviroment::push_global(script_state, xorstr_("getgc"), Enviroment::Functions::Env::getgc);
		Enviroment::push_global(script_state, xorstr_("get_gc_collection"), Enviroment::Functions::Env::getgc);
		Enviroment::push_global(script_state, xorstr_("get_gc_objects"), Enviroment::Functions::Env::getgc);
		Enviroment::push_global(script_state, xorstr_("get_garbage_collection"), Enviroment::Functions::Env::getgc);
		Enviroment::push_global(script_state, xorstr_("gettenv"), Enviroment::Functions::Env::getttenv);

		Enviroment::push_global(script_state, xorstr_("is_synapse_function"), Enviroment::Functions::Closures::is_executor_closure);
		Enviroment::push_global(script_state, xorstr_("is_synapse_closure"), Enviroment::Functions::Closures::is_executor_closure);
		Enviroment::push_global(script_state, xorstr_("is_calibri_closure"), Enviroment::Functions::Closures::is_executor_closure);
		Enviroment::push_global(script_state, xorstr_("is_calibriclosure"), Enviroment::Functions::Closures::is_executor_closure);
		Enviroment::push_global(script_state, xorstr_("iscalibri_closure"), Enviroment::Functions::Closures::is_executor_closure);
		Enviroment::push_global(script_state, xorstr_("iscalibriclosure"), Enviroment::Functions::Closures::is_executor_closure);
		Enviroment::push_global(script_state, xorstr_("isexecutorclosure"), Enviroment::Functions::Closures::is_executor_closure);
		Enviroment::push_global(script_state, xorstr_("isourclosure"), Enviroment::Functions::Closures::is_executor_closure);
		Enviroment::push_global(script_state, xorstr_("checkclosure"), Enviroment::Functions::Closures::is_executor_closure);
		Enviroment::push_global(script_state, xorstr_("clonefunction"), Enviroment::Functions::Closures::clonefunction);
		Enviroment::push_global(script_state, xorstr_("clonefunc"), Enviroment::Functions::Closures::clonefunction);
		Enviroment::push_global(script_state, xorstr_("newcclosure"), Enviroment::Functions::Closures::newcclosure);
		Enviroment::push_global(script_state, xorstr_("new_c_closure"), Enviroment::Functions::Closures::newcclosure);
		Enviroment::push_global(script_state, xorstr_("hookfunction"), Enviroment::Functions::Closures::hookfunction);
		Enviroment::push_global(script_state, xorstr_("replaceclosure"), Enviroment::Functions::Closures::hookfunction);
		Enviroment::push_global(script_state, xorstr_("hookfunc"), Enviroment::Functions::Closures::hookfunction);
		Enviroment::push_global(script_state, xorstr_("iscclosure"), Enviroment::Functions::Closures::iscclosure);
		Enviroment::push_global(script_state, xorstr_("is_c_closure"), Enviroment::Functions::Closures::iscclosure);
		Enviroment::push_global(script_state, xorstr_("islclosure"), Enviroment::Functions::Closures::islclosure);
		Enviroment::push_global(script_state, xorstr_("is_l_closure"), Enviroment::Functions::Closures::islclosure);
		Enviroment::push_global(script_state, xorstr_("checkcaller"), Enviroment::Functions::Closures::checkcaller);
		Enviroment::push_global(script_state, xorstr_("loadstring"), Enviroment::Functions::Closures::loadstring);
		Enviroment::push_global(script_state, xorstr_("Loadstring"), Enviroment::Functions::Closures::loadstring);

		Enviroment::push_global(script_state, xorstr_("isrbxactive"), Enviroment::Functions::Input::isrbxactive);
		Enviroment::push_global(script_state, xorstr_("isgameactive"), Enviroment::Functions::Input::isrbxactive);
		Enviroment::push_global(script_state, xorstr_("iswindowactive"), Enviroment::Functions::Input::isrbxactive);
		Enviroment::push_global(script_state, xorstr_("mouse1press"), Enviroment::Functions::Input::mouse1press);
		Enviroment::push_global(script_state, xorstr_("Mouse1Press"), Enviroment::Functions::Input::mouse1press);
		Enviroment::push_global(script_state, xorstr_("mouse2press"), Enviroment::Functions::Input::mouse2press);
		Enviroment::push_global(script_state, xorstr_("Mouse2Press"), Enviroment::Functions::Input::mouse2press);
		Enviroment::push_global(script_state, xorstr_("mouse1release"), Enviroment::Functions::Input::mouse1release);
		Enviroment::push_global(script_state, xorstr_("Mouse1Release"), Enviroment::Functions::Input::mouse1release);
		Enviroment::push_global(script_state, xorstr_("mouse2release"), Enviroment::Functions::Input::mouse2release);
		Enviroment::push_global(script_state, xorstr_("Mouse2Release"), Enviroment::Functions::Input::mouse2release);
		Enviroment::push_global(script_state, xorstr_("mouse1click"), Enviroment::Functions::Input::mouse1click);
		Enviroment::push_global(script_state, xorstr_("Mouse1Click"), Enviroment::Functions::Input::mouse1click);
		Enviroment::push_global(script_state, xorstr_("mouse2click"), Enviroment::Functions::Input::mouse2click);
		Enviroment::push_global(script_state, xorstr_("Mouse2Click"), Enviroment::Functions::Input::mouse2click);
		Enviroment::push_global(script_state, xorstr_("mousescroll"), Enviroment::Functions::Input::mousescroll);
		Enviroment::push_global(script_state, xorstr_("MouseScroll"), Enviroment::Functions::Input::mousescroll);
		Enviroment::push_global(script_state, xorstr_("mousemoverel"), Enviroment::Functions::Input::mousemoverel);
		Enviroment::push_global(script_state, xorstr_("MouseMoveRel"), Enviroment::Functions::Input::mousemoverel);
		Enviroment::push_global(script_state, xorstr_("MouseMoveRelative"), Enviroment::Functions::Input::mousemoverel);
		Enviroment::push_global(script_state, xorstr_("mousemoveabs"), Enviroment::Functions::Input::mousemoveabs);
		Enviroment::push_global(script_state, xorstr_("MouseMoveABS"), Enviroment::Functions::Input::mousemoveabs);
		Enviroment::push_global(script_state, xorstr_("MouseMoveAbs"), Enviroment::Functions::Input::mousemoveabs);
		Enviroment::push_global(script_state, xorstr_("keypress"), Enviroment::Functions::Input::keypress);
		Enviroment::push_global(script_state, xorstr_("KeyPress"), Enviroment::Functions::Input::keypress);
		Enviroment::push_global(script_state, xorstr_("keyrelease"), Enviroment::Functions::Input::keyrelease);
		Enviroment::push_global(script_state, xorstr_("KeyRelease"), Enviroment::Functions::Input::keyrelease);
		Enviroment::push_global(script_state, xorstr_("iskeydown"), Enviroment::Functions::Input::iskeydown);
		Enviroment::push_global(script_state, xorstr_("IsKeyDown"), Enviroment::Functions::Input::iskeydown);
		Enviroment::push_global(script_state, xorstr_("KeyDown"), Enviroment::Functions::Input::iskeydown);

		Enviroment::push_global(script_state, xorstr_("readfile"), Enviroment::Functions::Filesystem::readfile);
		Enviroment::push_global(script_state, xorstr_("writefile"), Enviroment::Functions::Filesystem::writefile);
		Enviroment::push_global(script_state, xorstr_("makefolder"), Enviroment::Functions::Filesystem::makefolder);
		Enviroment::push_global(script_state, xorstr_("appendfile"), Enviroment::Functions::Filesystem::appendfile);
		Enviroment::push_global(script_state, xorstr_("isfile"), Enviroment::Functions::Filesystem::isfile);
		Enviroment::push_global(script_state, xorstr_("isfolder"), Enviroment::Functions::Filesystem::isfolder);
		Enviroment::push_global(script_state, xorstr_("delfile"), Enviroment::Functions::Filesystem::delfile);
		Enviroment::push_global(script_state, xorstr_("delfolder"), Enviroment::Functions::Filesystem::delfolder);
		Enviroment::push_global(script_state, xorstr_("listfiles"), Enviroment::Functions::Filesystem::listfiles);

		lua_newtable(script_state);
		Enviroment::push_member(script_state, xorstr_("invalidate"), Enviroment::Functions::Cache::invalidate);
		Enviroment::push_member(script_state, xorstr_("iscached"), Enviroment::Functions::Cache::iscached);
		Enviroment::push_member(script_state, xorstr_("replace"), Enviroment::Functions::Cache::replace);
		lua_setfield(script_state, LUA_GLOBALSINDEX, (xorstr_("cache")));

		Enviroment::push_global(script_state, xorstr_("cloneref"), Enviroment::Functions::Cache::cloneref);
		Enviroment::push_global(script_state, xorstr_("compareinstances"), Enviroment::Functions::Cache::compareinstances);
		Enviroment::push_global(script_state, xorstr_("checkinst"), Enviroment::Functions::Cache::compareinstances);

		Enviroment::push_global(script_state, xorstr_("hookmetamethod"), Enviroment::Functions::Metatable::hookmetamethod);
		Enviroment::push_global(script_state, xorstr_("getrawmetatable"), Enviroment::Functions::Metatable::getrawmetatable);
		Enviroment::push_global(script_state, xorstr_("setrawmetatable"), Enviroment::Functions::Metatable::setrawmetatable);
		Enviroment::push_global(script_state, xorstr_("isreadonly"), Enviroment::Functions::Metatable::isreadonly);
		Enviroment::push_global(script_state, xorstr_("getnamecallmethod"), Enviroment::Functions::Metatable::getnamecallmethod);
		Enviroment::push_global(script_state, xorstr_("setnamecallmethod"), Enviroment::Functions::Metatable::setnamecallmethod);
		Enviroment::push_global(script_state, xorstr_("setreadonly"), Enviroment::Functions::Metatable::setreadonly);
		Enviroment::push_global(script_state, xorstr_("make_readonly"), Enviroment::Functions::Metatable::make_readonly);
		Enviroment::push_global(script_state, xorstr_("make_writable"), Enviroment::Functions::Metatable::make_writable);

		Enviroment::push_global(script_state, xorstr_("base64_encode"), Enviroment::Functions::Crypt::base64encode);
		Enviroment::push_global(script_state, xorstr_("base64_decode"), Enviroment::Functions::Crypt::base64decode);

		lua_newtable(script_state);
		Enviroment::push_member(script_state, xorstr_("encode"), Enviroment::Functions::Crypt::base64encode);
		Enviroment::push_member(script_state, xorstr_("decode"), Enviroment::Functions::Crypt::base64decode);
		lua_setfield(script_state, LUA_GLOBALSINDEX, xorstr_("base64"));

		lua_newtable(script_state);
		Enviroment::push_member(script_state, xorstr_("base64encode"), Enviroment::Functions::Crypt::base64encode);
		Enviroment::push_member(script_state, xorstr_("base64decode"), Enviroment::Functions::Crypt::base64decode);
		Enviroment::push_member(script_state, xorstr_("base64_encode"), Enviroment::Functions::Crypt::base64encode);
		Enviroment::push_member(script_state, xorstr_("base64_decode"), Enviroment::Functions::Crypt::base64decode);

		lua_newtable(script_state);
		Enviroment::push_member(script_state, xorstr_("encode"), Enviroment::Functions::Crypt::base64encode);
		Enviroment::push_member(script_state, xorstr_("decode"), Enviroment::Functions::Crypt::base64decode);
		lua_setfield(script_state, -2, xorstr_("base64"));

		Enviroment::push_member(script_state, xorstr_("encrypt"), Enviroment::Functions::Crypt::encrypt);
		Enviroment::push_member(script_state, xorstr_("decrypt"), Enviroment::Functions::Crypt::decrypt);
		Enviroment::push_member(script_state, xorstr_("generatebytes"), Enviroment::Functions::Crypt::generatebytes);
		Enviroment::push_member(script_state, xorstr_("generatekey"), Enviroment::Functions::Crypt::generatekey);
		Enviroment::push_member(script_state, xorstr_("hash"), Enviroment::Functions::Crypt::hash);

		lua_setfield(script_state, LUA_GLOBALSINDEX, xorstr_("crypt"));

		lua_newtable(script_state);
		Enviroment::push_member(script_state, xorstr_("getregistry"), Enviroment::Functions::Env::getreg);
		Enviroment::push_member(script_state, xorstr_("getmetatable"), Enviroment::Functions::Metatable::getrawmetatable);
		Enviroment::push_member(script_state, xorstr_("setmetatable"), Enviroment::Functions::Metatable::setrawmetatable);
		Enviroment::push_member(script_state, xorstr_("getupvalue"), Enviroment::Functions::Debug::debug_getupvalue);
		Enviroment::push_member(script_state, xorstr_("getupvalues"), Enviroment::Functions::Debug::debug_getupvalues);
		Enviroment::push_member(script_state, xorstr_("setupvalue"), Enviroment::Functions::Debug::debug_setupvalue);
		Enviroment::push_member(script_state, xorstr_("getconstant"), Enviroment::Functions::Debug::debug_getconstant);
		Enviroment::push_member(script_state, xorstr_("getconstants"), Enviroment::Functions::Debug::debug_getconstants);
		Enviroment::push_member(script_state, xorstr_("setconstant"), Enviroment::Functions::Debug::debug_setconstant);
		Enviroment::push_member(script_state, xorstr_("getinfo"), Enviroment::Functions::Debug::debug_getinfo);
		Enviroment::push_member(script_state, xorstr_("getstack"), Enviroment::Functions::Debug::debug_getstack);
		Enviroment::push_member(script_state, xorstr_("setstack"), Enviroment::Functions::Debug::debug_setstack);
		Enviroment::push_member(script_state, xorstr_("getproto"), Enviroment::Functions::Debug::debug_getproto);
		Enviroment::push_member(script_state, xorstr_("getprotos"), Enviroment::Functions::Debug::debug_getprotos);
		lua_setfield(script_state, LUA_GLOBALSINDEX, xorstr_("debug"));

		Enviroment::push_global(script_state, xorstr_("gethui"), Enviroment::Functions::Scripts::gethui);
		Enviroment::push_global(script_state, xorstr_("setclipboard"), Enviroment::Functions::Scripts::setclipboard);
		Enviroment::push_global(script_state, xorstr_("toclipboard"), Enviroment::Functions::Scripts::setclipboard);
		Enviroment::push_global(script_state, xorstr_("write_clipboard"), Enviroment::Functions::Scripts::setclipboard);
		Enviroment::push_global(script_state, xorstr_("getclipboard"), Enviroment::Functions::Scripts::getclipboard);
		Enviroment::push_global(script_state, xorstr_("read_clipboard"), Enviroment::Functions::Scripts::getclipboard);
		Enviroment::push_global(script_state, xorstr_("HttpGet"), Enviroment::Functions::Scripts::httpget);
		Enviroment::push_global(script_state, xorstr_("HttpGetAsync"), Enviroment::Functions::Scripts::httpget);
		Enviroment::push_global(script_state, xorstr_("GetObjects"), Enviroment::Functions::Scripts::getobjects);
		Enviroment::push_global(script_state, xorstr_("identifyexecutor"), Enviroment::Functions::Scripts::identifyexecutor);
		Enviroment::push_global(script_state, xorstr_("getexecutorname"), Enviroment::Functions::Scripts::identifyexecutor);
		Enviroment::push_global(script_state, xorstr_("request"), Enviroment::Functions::Scripts::request);
		Enviroment::push_global(script_state, xorstr_("http_request"), Enviroment::Functions::Scripts::request);
		Enviroment::push_global(script_state, xorstr_("getcustomasset"), Enviroment::Functions::Scripts::getcustomasset);

		Enviroment::push_global(script_state, xorstr_("setfps"), Enviroment::Functions::Scripts::setfps);
		Enviroment::push_global(script_state, xorstr_("setfpscap"), Enviroment::Functions::Scripts::setfps);
		Enviroment::push_global(script_state, xorstr_("set_fps_cap"), Enviroment::Functions::Scripts::setfps);

		Enviroment::push_global(script_state, xorstr_("getfps"), Enviroment::Functions::Scripts::getfps);
		Enviroment::push_global(script_state, xorstr_("getfpscap"), Enviroment::Functions::Scripts::getfps);
		Enviroment::push_global(script_state, xorstr_("get_fps_cap"), Enviroment::Functions::Scripts::getfps);

		Enviroment::push_global(script_state, xorstr_("getcallingscript"), Enviroment::Functions::Scripts::getcallingscript);
		Enviroment::push_global(script_state, xorstr_("get_calling_script"), Enviroment::Functions::Scripts::getcallingscript);
		Enviroment::push_global(script_state, xorstr_("getcurrentscript"), Enviroment::Functions::Scripts::getcallingscript);
		Enviroment::push_global(script_state, xorstr_("getrunningscripts"), Enviroment::Functions::Scripts::getrunningscripts);
		Enviroment::push_global(script_state, xorstr_("getloadedmodules"), Enviroment::Functions::Scripts::getloadedmodules);
		Enviroment::push_global(script_state, xorstr_("get_loaded_modules"), Enviroment::Functions::Scripts::getloadedmodules);
		Enviroment::push_global(script_state, xorstr_("GetLoadedModules"), Enviroment::Functions::Scripts::getloadedmodules);
		Enviroment::push_global(script_state, xorstr_("getmodules"), Enviroment::Functions::Scripts::getloadedmodules);

		lua_newtable(script_state);
		Enviroment::push_member(script_state, xorstr_("request"), Enviroment::Functions::Scripts::request);
		lua_setfield(script_state, LUA_GLOBALSINDEX, xorstr_("http"));

		Enviroment::push_global(script_state, xorstr_("firetouchinterest"), Enviroment::Functions::Scripts::firetouchinterest);
		Enviroment::push_global(script_state, xorstr_("fireclickdetector"), Enviroment::Functions::Scripts::fireclickdetector);

		Enviroment::push_global(script_state, xorstr_("getscriptbytecode"), Enviroment::Functions::Instance::getscriptbytecode);
		Enviroment::push_global(script_state, xorstr_("isscriptable"), Enviroment::Functions::Instance::isscriptable);
		Enviroment::push_global(script_state, xorstr_("setscriptable"), Enviroment::Functions::Instance::setscriptable);
		Enviroment::push_global(script_state, xorstr_("gethiddenproperty"), Enviroment::Functions::Instance::gethiddenproperty);
		Enviroment::push_global(script_state, xorstr_("sethiddenproperty"), Enviroment::Functions::Instance::sethiddenproperty);

		Enviroment::push_global(script_state, xorstr_("getidentity"), Enviroment::Functions::Scripts::getcontext);
		Enviroment::push_global(script_state, xorstr_("getthreadcontext"), Enviroment::Functions::Scripts::getcontext);
		Enviroment::push_global(script_state, xorstr_("getthreadidentity"), Enviroment::Functions::Scripts::getcontext);
		Enviroment::push_global(script_state, xorstr_("get_thread_identity"), Enviroment::Functions::Scripts::getcontext);
		Enviroment::push_global(script_state, xorstr_("setidentity"), Enviroment::Functions::Scripts::setcontext);
		Enviroment::push_global(script_state, xorstr_("setthreadcontext"), Enviroment::Functions::Scripts::setcontext);
		Enviroment::push_global(script_state, xorstr_("setthreadidentity"), Enviroment::Functions::Scripts::setcontext);
		Enviroment::push_global(script_state, xorstr_("set_thread_identity"), Enviroment::Functions::Scripts::setcontext);

		Enviroment::push_global(script_state, xorstr_("getconnections"), Enviroment::Functions::Signals::getconnections);

		Enviroment::push_global(script_state, xorstr_("decompile"), Enviroment::Decompiler::decompile);
	}


	static auto initialize() -> void
	{
		script_context = TaskScheduler::get_script_context();
		auto newthread = lua_newthread((lua_State*)TaskScheduler::get_lua_state());
		Enviroment::calibri_state = (__int64)newthread;
		Identity::set_context(Enviroment::calibri_state, 8);

		luaL_sandboxthread((lua_State*)Enviroment::calibri_state);
		initialize_api((lua_State*)Enviroment::calibri_state);
		Enviroment::MetaHooks::initialize((lua_State*)Enviroment::calibri_state);
		luaL_sandboxthread((lua_State*)Enviroment::calibri_state);

		lua_getglobal((lua_State*)Enviroment::calibri_state, "Instance");
		lua_getfield((lua_State*)Enviroment::calibri_state, -1, "new");
		lua_pushstring((lua_State*)Enviroment::calibri_state, "LocalScript");
		lua_pcall((lua_State*)Enviroment::calibri_state, 1, 1, 0);

		lua_pushstring((lua_State*)Enviroment::calibri_state, "LocalScript");
		lua_setfield((lua_State*)Enviroment::calibri_state, -2, "Name");
		lua_setglobal((lua_State*)Enviroment::calibri_state, "script");
		lua_settop((lua_State*)Enviroment::calibri_state, 0);

		lua_getglobal((lua_State*)Enviroment::calibri_state, "game");
		lua_getfield((lua_State*)Enviroment::calibri_state, -1, "GetService");
		lua_pushvalue((lua_State*)Enviroment::calibri_state, -2);
		lua_pushstring((lua_State*)Enviroment::calibri_state, "RunService");
		lua_pcall((lua_State*)Enviroment::calibri_state, 2, 1, 0);

		lua_getfield((lua_State*)Enviroment::calibri_state, -1, "RenderStepped");
		lua_getfield((lua_State*)Enviroment::calibri_state, -1, "Connect");
		lua_pushvalue((lua_State*)Enviroment::calibri_state, -2);
		lua_pushcclosure((lua_State*)Enviroment::calibri_state, scheduler_hook, nullptr, 0);
		lua_pcall((lua_State*)Enviroment::calibri_state, 2, 0, 0);
		lua_pop((lua_State*)Enviroment::calibri_state, 2);

		lua_getglobal((lua_State*)Enviroment::calibri_state, "game");
		lua_getfield((lua_State*)Enviroment::calibri_state, -1, "GetService");
		lua_pushvalue((lua_State*)Enviroment::calibri_state, -2);
		lua_pushstring((lua_State*)Enviroment::calibri_state, "RuntimeScriptService");
		lua_pcall((lua_State*)Enviroment::calibri_state, 2, 1, 0);

		Enviroment::runtime_script_service = *reinterpret_cast<uintptr_t*>(lua_touserdata((lua_State*)Enviroment::calibri_state, -1));
		lua_pop((lua_State*)Enviroment::calibri_state, 2);

		coderun_queue.push(::crypt::base64::decode(Utils::init_script));
		coderun_queue.push(drawing_library);
		coderun_queue.push(::crypt::base64::decode(Utils::lz4_library));
	}
}
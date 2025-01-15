#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>
#include <queue>
#include <map>

#include "LuaInit.hpp"
#include "Enviroment.hpp"
#include <shared_mutex>



namespace Execution
{
	static __int64 script_context = 0;


	std::shared_mutex execution_lock;

	static auto execute_script(__int64 state, std::string source) -> void
	{
		//execution_lock.lock();

		std::string bytecode = Luau::compile(source);

		if (bytecode.at(0) == 0)
		{

			Defs::rbx_print(3, bytecode.c_str() + 1);
			//execution_lock.unlock();
			return;
		}
		else
		{
			std::string compressed = Utils::compress_bytecode(bytecode);

			lua_State* execution_thread = lua_newthread((lua_State*)state);
			lua_pop((lua_State*)state, 1); /* pops the lua state pushed onto the stack */


			Defs::rbx_luavmload((__int64)execution_thread, &compressed, xorstr_("@"), 0);

			//setting caps...

			Closure* pClosure = (Closure*)lua_topointer(execution_thread, -1);

			LOGD("Closure: %llX", pClosure);
			LOGD("Proto: %llX", pClosure->l.p);

			ProtoThings::raise_proto((Proto*)pClosure->l.p);

			//Defs::rbx_spawn((__int64)execution_thread);



			//const auto shared_context = get_extraspace(execution_thread)->shared->script_context;

			auto extraSpace = static_cast<ExtraSpace*>(execution_thread->userdata);

			auto scriptContext = extraSpace->sharedExtraSpace->scriptContext;

			weak_thread_ref_t2 weak_thread_ref_object = { execution_thread };
			lua_pushthread(execution_thread);
			weak_thread_ref_object.thread_ref = lua_ref(execution_thread, -1);
			lua_pop(execution_thread, 1);

			weak_thread_ref_t2* weak_thread_ref = &weak_thread_ref_object;
			debugger_result_t debugger_results = { 0 };

			LOGD("thread: %p", execution_thread);

	

			Defs::scriptcontext_resume((uintptr_t)scriptContext + 0x610, &debugger_results, &weak_thread_ref, 0, 0, nullptr);
			LOGD("userdata: %p", extraSpace);
			LOGD("resumed");
			////execution_lock.unlock();
		}
	}

	static auto blacklist_job() -> void
	{
		if (!TaskScheduler::is_loaded())
		{
			blacklisted_job = TaskScheduler::get_all_jobs_by_name("WaitingHybridScriptsJob")[0];
		}
		else
		{
			//my genis method XD
			auto jobs = TaskScheduler::get_all_jobs_by_name("WaitingHybridScriptsJob");
			for (int i = 0; i < jobs.size(); i++)
			{
				auto script_context = *reinterpret_cast<__int64*>(jobs[i] + 504);

				__int64 arg1 = 0;
				__int64 arg2 = 0;
				__int64 state = Defs::rbx_getstate(script_context + 280, &arg1, &arg2);

				__int64 rstate = Defs::decryptstate(state + 136);
				lua_pushvalue((lua_State*)rstate, LUA_REGISTRYINDEX);

				auto table = hvalue(index2addr((lua_State*)rstate, -1));
				if (table->sizearray == 4)
					blacklisted_job = jobs[i];
			}
		}

	}



	static int scheduler_cycle_original()
	{
		if (!coderun_queue.empty())
		{
			std::string script = coderun_queue.front();
			execute_script(Enviroment::atlantis_state, script);
			coderun_queue.pop();
		}

		if (!mYields.empty())
		{
			const auto pending_yield = mYields.dequeue();
			pending_yield();

			LOGD("Resumed a yield from yield scheduler.");
		}


		return 0;
	}

	static auto scheduler_hook(lua_State* rl) -> int
	{
		scheduler_cycle_original();
		return 0;
	}

	
	int run_on_actor(lua_State*);


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
		lua_setfield(script_state, LUA_GLOBALSINDEX, "IS_Pluto");

		lua_pushboolean(script_state, true);
		lua_setfield(script_state, LUA_GLOBALSINDEX, "IsPluto");

		lua_pushboolean(script_state, true);
		lua_setfield(script_state, LUA_GLOBALSINDEX, "Pluto_LOADED");

		lua_pushboolean(script_state, true);
		lua_setfield(script_state, LUA_GLOBALSINDEX, "PlutoLoaded");

		lua_pushboolean(script_state, true);
		lua_setfield(script_state, LUA_GLOBALSINDEX, "Pluto");

		lua_pushboolean(script_state, true);
		lua_setfield(script_state, LUA_GLOBALSINDEX, "unlockmodulescript");

		Enviroment::push_global(script_state, xorstr_("getgenv"), Enviroment::Functions::Env::getgenv);
		Enviroment::push_global(script_state, xorstr_("getreg"), Enviroment::Functions::Env::getreg);
		Enviroment::push_global(script_state, xorstr_("getrenv"), Enviroment::Functions::Env::getrenv);
		Enviroment::push_global(script_state, xorstr_("getgc"), Enviroment::Functions::Env::getgc);
		Enviroment::push_global(script_state, xorstr_("get_gc_collection"), Enviroment::Functions::Env::getgc);
		Enviroment::push_global(script_state, xorstr_("get_gc_objects"), Enviroment::Functions::Env::getgc);
		Enviroment::push_global(script_state, xorstr_("get_garbage_collection"), Enviroment::Functions::Env::getgc);
		Enviroment::push_global(script_state, xorstr_("gettenv"), Enviroment::Functions::Env::getttenv);

		Enviroment::push_global(script_state, xorstr_("yieldtest"), Enviroment::Functions::Env::YieldingTest);
		Enviroment::push_global(script_state, xorstr_("is_synapse_function"), Enviroment::Functions::Closures::is_executor_closure);
		Enviroment::push_global(script_state, xorstr_("is_synapse_closure"), Enviroment::Functions::Closures::is_executor_closure);
		Enviroment::push_global(script_state, xorstr_("is_atlantis_closure"), Enviroment::Functions::Closures::is_executor_closure);
		Enviroment::push_global(script_state, xorstr_("is_atlantisclosure"), Enviroment::Functions::Closures::is_executor_closure);
		Enviroment::push_global(script_state, xorstr_("isatlantis_closure"), Enviroment::Functions::Closures::is_executor_closure);
		Enviroment::push_global(script_state, xorstr_("isatlantisclosure"), Enviroment::Functions::Closures::is_executor_closure);
		Enviroment::push_global(script_state, xorstr_("isexecutorclosure"), Enviroment::Functions::Closures::is_executor_closure);
		Enviroment::push_global(script_state, xorstr_("isourclosure"), Enviroment::Functions::Closures::is_executor_closure);
		Enviroment::push_global(script_state, xorstr_("checkclosure"), Enviroment::Functions::Closures::is_executor_closure);
		Enviroment::push_global(script_state, xorstr_("clonefunction"), Enviroment::Functions::Closures::clonefunction);
		Enviroment::push_global(script_state, xorstr_("clonefunc"), Enviroment::Functions::Closures::clonefunction);
		Enviroment::push_global(script_state, xorstr_("newcclosure"), Enviroment::Functions::Closures::newcclosure);
		Enviroment::push_global(script_state, xorstr_("new_c_closure"), Enviroment::Functions::Closures::newcclosure);
		//Enviroment::push_global(script_state, xorstr_("newlclosure"), Enviroment::Functions::Closures::newlclosure);
		//Enviroment::push_global(script_state, xorstr_("new_l_closure"), Enviroment::Functions::Closures::newlclosure);
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
	//	Enviroment::push_global(script_state, xorstr_("isfile"), Enviroment::Functions::Filesystem::isfile);
		Enviroment::push_global(script_state, xorstr_("isfolder"), Enviroment::Functions::Filesystem::isfolder);
		Enviroment::push_global(script_state, xorstr_("delfile"), Enviroment::Functions::Filesystem::delfile);
		Enviroment::push_global(script_state, xorstr_("delfolder"), Enviroment::Functions::Filesystem::delfolder);
		Enviroment::push_global(script_state, xorstr_("listfiles"), Enviroment::Functions::Filesystem::listfiles);
		Enviroment::push_global(script_state, xorstr_("loadfile"), Enviroment::Functions::Filesystem::loadfile);

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


		Enviroment::push_global(script_state, xorstr_("getregistry"), Enviroment::Functions::Env::getreg); // ya
		Enviroment::push_global(script_state, xorstr_("getupvalue"), Enviroment::Functions::Debug::debug_getupvalue); // ya
		Enviroment::push_global(script_state, xorstr_("getupvalues"), Enviroment::Functions::Debug::debug_getupvalues); // ya
		Enviroment::push_global(script_state, xorstr_("setupvalue"), Enviroment::Functions::Debug::debug_setupvalue); // ya
		Enviroment::push_global(script_state, xorstr_("getconstant"), Enviroment::Functions::Debug::debug_getconstant); // ya
		Enviroment::push_global(script_state, xorstr_("getconstants"), Enviroment::Functions::Debug::debug_getconstants); // ya
		Enviroment::push_global(script_state, xorstr_("setconstant"), Enviroment::Functions::Debug::debug_setconstant); // ya
		Enviroment::push_global(script_state, xorstr_("getinfo"), Enviroment::Functions::Debug::debug_getinfo); // ya
		Enviroment::push_global(script_state, xorstr_("getstack"), Enviroment::Functions::Debug::debug_getstack); // ya
		Enviroment::push_global(script_state, xorstr_("setstack"), Enviroment::Functions::Debug::debug_setstack); // ya
		Enviroment::push_global(script_state, xorstr_("getproto"), Enviroment::Functions::Debug::debug_getproto); // ya
		Enviroment::push_global(script_state, xorstr_("getprotos"), Enviroment::Functions::Debug::debug_getprotos); // ya

		lua_newtable(script_state);
		Enviroment::push_member(script_state, xorstr_("connect"), Enviroment::Functions::WebSocket::websocket_connect);
		lua_setfield(script_state, LUA_GLOBALSINDEX, xorstr_("WebSocket"));

		Enviroment::push_global(script_state, xorstr_("gethui"), Enviroment::Functions::Scripts::gethui);
		Enviroment::push_global(script_state, xorstr_("get_hidden_gui"), Enviroment::Functions::Scripts::gethui);
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
		Enviroment::push_global(script_state, xorstr_("downloadasset"), Enviroment::Functions::Scripts::getcustomasset);
		Enviroment::push_global(script_state, xorstr_("queue_on_teleport"), Enviroment::Functions::Scripts::queue_on_teleport);
		Enviroment::push_global(script_state, xorstr_("queueonteleport"), Enviroment::Functions::Scripts::queue_on_teleport);
		Enviroment::push_global(script_state, xorstr_("queueteleport"), Enviroment::Functions::Scripts::queue_on_teleport);

		Enviroment::push_global(script_state, xorstr_("setfps"), Enviroment::Functions::Scripts::setfps);
		Enviroment::push_global(script_state, xorstr_("set_fps"), Enviroment::Functions::Scripts::setfps);
		Enviroment::push_global(script_state, xorstr_("setfpscap"), Enviroment::Functions::Scripts::setfps);
		Enviroment::push_global(script_state, xorstr_("set_fps_cap"), Enviroment::Functions::Scripts::setfps);

		Enviroment::push_global(script_state, xorstr_("getfps"), Enviroment::Functions::Scripts::getfps);
		Enviroment::push_global(script_state, xorstr_("get_fps"), Enviroment::Functions::Scripts::getfps);
		Enviroment::push_global(script_state, xorstr_("getfpscap"), Enviroment::Functions::Scripts::getfps);
		Enviroment::push_global(script_state, xorstr_("get_fps_cap"), Enviroment::Functions::Scripts::getfps);

		Enviroment::push_global(script_state, xorstr_("getcallingscript"), Enviroment::Functions::Scripts::getcallingscript);
		Enviroment::push_global(script_state, xorstr_("get_calling_script"), Enviroment::Functions::Scripts::getcallingscript);
		Enviroment::push_global(script_state, xorstr_("getcurrentscript"), Enviroment::Functions::Scripts::getcallingscript);
		//Enviroment::push_global(script_state, xorstr_("getrunningscripts"), Enviroment::Functions::Scripts::getrunningscripts);
		//Enviroment::push_global(script_state, xorstr_("getloadedmodules"), Enviroment::Functions::Scripts::getloadedmodules);
		//Enviroment::push_global(script_state, xorstr_("get_loaded_modules"), Enviroment::Functions::Scripts::getloadedmodules);
		//Enviroment::push_global(script_state, xorstr_("GetLoadedModules"), Enviroment::Functions::Scripts::getloadedmodules);
		//Enviroment::push_global(script_state, xorstr_("getmodules"), Enviroment::Functions::Scripts::getloadedmodules);

		lua_newtable(script_state);
		Enviroment::push_member(script_state, xorstr_("request"), Enviroment::Functions::Scripts::request);
		lua_setfield(script_state, LUA_GLOBALSINDEX, xorstr_("http"));

		Enviroment::push_global(script_state, xorstr_("getsenv"), Enviroment::Functions::Scripts::getsenv);
		Enviroment::push_global(script_state, xorstr_("getmenv"), Enviroment::Functions::Scripts::getsenv);

		Enviroment::push_global(script_state, xorstr_("getrunningscripts"), Enviroment::Functions::Scripts::getrunningscripts);
		Enviroment::push_global(script_state, xorstr_("getscriptswiththreads"), Enviroment::Functions::Scripts::getrunningscripts);


		Enviroment::push_global(script_state, xorstr_("firetouchinterest"), Enviroment::Functions::Scripts::firetouchinterest);
		Enviroment::push_global(script_state, xorstr_("fireclickdetector"), Enviroment::Functions::Scripts::fireclickdetector);


		Enviroment::push_global(script_state, xorstr_("getinstances"), Enviroment::Functions::Instance::getinstances);
		Enviroment::push_global(script_state, xorstr_("getnilinstances"), Enviroment::Functions::Instance::getnilinstances);
		Enviroment::push_global(script_state, xorstr_("get_instances"), Enviroment::Functions::Instance::getinstances);
		Enviroment::push_global(script_state, xorstr_("get_nil_instances"), Enviroment::Functions::Instance::getnilinstances);
		Enviroment::push_global(script_state, xorstr_("GetInstances"), Enviroment::Functions::Instance::getinstances);
		Enviroment::push_global(script_state, xorstr_("GetNilInstances"), Enviroment::Functions::Instance::getnilinstances);

		Enviroment::push_global(script_state, xorstr_("getscripthash"), Enviroment::Functions::Instance::getscripthash);
		Enviroment::push_global(script_state, xorstr_("getscriptbytecode"), Enviroment::Functions::Instance::getscriptbytecode);
		Enviroment::push_global(script_state, xorstr_("dumpstring"), Enviroment::Functions::Instance::getscriptbytecode);
		Enviroment::push_global(script_state, xorstr_("isscriptable"), Enviroment::Functions::Instance::isscriptable);
		Enviroment::push_global(script_state, xorstr_("setscriptable"), Enviroment::Functions::Instance::setscriptable);
		Enviroment::push_global(script_state, xorstr_("getproperties"), Enviroment::Functions::Instance::getproperties);
		Enviroment::push_global(script_state, xorstr_("getcallbackvalue"), Enviroment::Functions::Instance::getcallbackvalue);
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
		Enviroment::push_global(script_state, xorstr_("setcontext"), Enviroment::Functions::Scripts::setcontext);
		Enviroment::push_global(script_state, xorstr_("set_context_"), Enviroment::Functions::Scripts::setcontext);

		Enviroment::push_global(script_state, xorstr_("getscripts"), Enviroment::Functions::Instance::getscripts);
		
		Enviroment::push_global(script_state, xorstr_("getscriptclosure"), Enviroment::Functions::Closures::getscriptclosure);
		Enviroment::push_global(script_state, xorstr_("getscriptfunction"), Enviroment::Functions::Closures::getscriptclosure);
		

		Enviroment::push_global(script_state, xorstr_("getconnections"), Enviroment::Functions::Signals::get_connections);
				Enviroment::push_global(script_state, xorstr_("get_signal_cons"), Enviroment::Functions::Signals::get_connections);
		Enviroment::push_global(script_state, xorstr_("firesignal"), Enviroment::Functions::Signals::firesignal);


		Enviroment::push_global(script_state, xorstr_("getfflag"), Enviroment::Functions::FFlags::getfflag);
		Enviroment::push_global(script_state, xorstr_("setfflag"), Enviroment::Functions::FFlags::setfflag);

		Enviroment::push_global(script_state, xorstr_("lz4compress"), Enviroment::Functions::Closures::lz4compress);
		Enviroment::push_global(script_state, xorstr_("lz4decompress"), Enviroment::Functions::Closures::lz4decompress);

		Enviroment::push_global(script_state, xorstr_("fireproximityprompt"), Enviroment::Functions::Scripts::fireproximityprompt);

		Enviroment::push_global(script_state, xorstr_("decompile"), Enviroment::Functions::Scripts::decompile);

		Enviroment::push_global(script_state, xorstr_("getfunctionhash"), Enviroment::Functions::Closures::getfunctionbytecode);

		Enviroment::push_global(script_state, xorstr_("restorefunction"), Enviroment::Functions::Closures::restorefunction);

		Enviroment::push_global(script_state, xorstr_("isfunctionhooked"), Enviroment::Functions::Closures::isfunctionhooked);

		Enviroment::push_global(script_state, xorstr_("iscustomcclosure"), Enviroment::Functions::Closures::iscustomcclosure);
		Enviroment::push_global(script_state, xorstr_("isnewcclosure"), Enviroment::Functions::Closures::iscustomcclosure);

		// CONSOLE
		Enviroment::push_global(script_state, xorstr_("rconsoleprint"), Enviroment::Functions::Console::rconsoleprint);
		Enviroment::push_global(script_state, xorstr_("consoleprint"), Enviroment::Functions::Console::rconsoleprint);

		Enviroment::push_global(script_state, xorstr_("getloadedmodules"), Enviroment::Functions::Scripts::getloadedmodules);

	
		Enviroment::push_global((lua_State*)Enviroment::atlantis_state, xorstr_("run_on_actor"), run_on_actor);
		Enviroment::push_global((lua_State*)Enviroment::atlantis_state, xorstr_("getactors"), Enviroment::Functions::Actors::getactors);
		Enviroment::push_global((lua_State*)Enviroment::atlantis_state, xorstr_("require"), Enviroment::Functions::Scripts::require);

	}


	auto run_on_actor(lua_State* rl) -> int
	{
		luaL_checktype(rl, 1, LUA_TUSERDATA);
		luaL_checktype(rl, 2, LUA_TSTRING);

		auto actor = *reinterpret_cast<__int64*>(lua_touserdata(rl, 1));
		auto source = lua_tostring(rl, 2);

		lua_getglobal(rl, "Instance");
		lua_getfield(rl, -1, "new");
		lua_pushstring(rl, "LocalScript");
		lua_call(rl, 1, 1);

		auto local_script = *reinterpret_cast<__int64*>(lua_touserdata(rl, -1));
		lua_pushboolean(rl, false);
		lua_setfield(rl, -2, "Enabled");
		Defs::rbx_pushinstance((__int64)rl, &actor);
		lua_setfield(rl, -2, "Parent");

		__int64 level = 0;
		auto script_context = TaskScheduler::get_script_context();
		auto state = Defs::rbx_getstate(script_context + 280, &level, &local_script);
		auto actor_state = Defs::decryptstate(state + 136);

		auto execution_thread = lua_newthread((lua_State*)actor_state);
		Identity::set_context((__int64)execution_thread, 8);



		luaL_sandboxthread(execution_thread);
		initialize_api(execution_thread);
		luaL_sandboxthread(execution_thread);

		auto script = skibidi + source;
		auto bytecode = Utils::compress_bytecode(Luau::compile(script));

		if (Defs::rbx_luavmload((__int64)execution_thread, &bytecode, "=", 0) == 0) // Atlantis-Actor
		{
			Closure* pClosure = const_cast<Closure*>(static_cast<const Closure*>(lua_topointer((lua_State*)state, -1)));
			ProtoThings::raise_proto(pClosure->l.p);

			static roblox_function spawn{};

			lua_getfield(execution_thread, LUA_GLOBALSINDEX, "spawn");

			Closure* cl = (Closure*)lua_topointer(execution_thread, -1);
			lua_CFunction func = cl->c.f;

			spawn = reinterpret_cast<roblox_function>(reinterpret_cast<std::intptr_t>(func));
			spawn(reinterpret_cast<std::uintptr_t>(execution_thread));

			return 0;
		}

		lua_pushnil(rl);
		lua_insert(rl, -2);
		return 2;
	}

	static auto initialize() -> void
	{
		setupLogger();
		HelpFunctions::Init();
		LOGD("initializing 1/3");
		script_context = TaskScheduler::get_script_context();
		//uintptr_t whjs = TaskScheduler::get_jobs_by_name("WaitingHybridScriptsJob");
		lua_State* parent = (lua_State*)TaskScheduler::get_lua_state();

		lua_newtable(parent);
		lua_setglobal(parent, "_G");

		lua_newtable(parent);
		lua_setglobal(parent, "shared");

		lua_settop(parent, 0);

		const auto old_top = lua_gettop(parent);

		auto newthread = lua_newthread(parent); 
		//LOGD("lua state: %llX", newthread);
		//newthread->gt = luaH_clone(parent, parent->gt);

		lua_pop(parent, 1); /* pops the lua state pushed onto the stack */
;
	//	Defs::rbx_print(1, "lua state: %llX", newthread);

		LOGD("initialized 1/3");

		Enviroment::atlantis_state = (__int64)newthread;
		Identity::set_context(Enviroment::atlantis_state, 8);

		//lua_settop(parent, old_top);

		LOGD("passed 1");
		lua_getglobal(newthread, "require");
		LOGD("passed 2");
		Enviroment::Functions::Scripts::old_require = lua_tocfunction(newthread, -1);
		LOGD("passed 3");

		lua_pop(newthread, 1);
		LOGD("passed 4");

		LOGD("initializing 2/3");
		luaL_sandboxthread((lua_State*)Enviroment::atlantis_state);
		initialize_api((lua_State*)Enviroment::atlantis_state);

		LOGD("initializing 3/3");
		Enviroment::MetaHooks::initialize((lua_State*)Enviroment::atlantis_state);
		LOGD("initialized 3/3");
		//luaL_sandboxthread((lua_State*)Enviroment::atlantis_state);

	/*	lua_getglobal((lua_State*)Enviroment::atlantis_state, xorstr_("Instance"));
		lua_getfield((lua_State*)Enviroment::atlantis_state, -1, xorstr_("new"));
		lua_pushstring((lua_State*)Enviroment::atlantis_state, xorstr_("LocalScript"));
		lua_pcall((lua_State*)Enviroment::atlantis_state, 1, 1, 0);

		lua_pushstring((lua_State*)Enviroment::atlantis_state, xorstr_("LocalScript"));
		lua_setfield((lua_State*)Enviroment::atlantis_state, -2, xorstr_("Name"));
		lua_setglobal((lua_State*)Enviroment::atlantis_state, xorstr_("script"));
		lua_settop((lua_State*)Enviroment::atlantis_state, 0);*/

		
		lua_getglobal((lua_State*)Enviroment::atlantis_state, xorstr_("game"));
		lua_getfield((lua_State*)Enviroment::atlantis_state, -1, xorstr_("GetService"));
		lua_pushvalue((lua_State*)Enviroment::atlantis_state, -2);
		lua_pushstring((lua_State*)Enviroment::atlantis_state, xorstr_("RunService"));
		lua_pcall((lua_State*)Enviroment::atlantis_state, 2, 1, 0);

		lua_getfield((lua_State*)Enviroment::atlantis_state, -1, xorstr_("Stepped"));
		lua_getfield((lua_State*)Enviroment::atlantis_state, -1, xorstr_("Connect"));
		lua_pushvalue((lua_State*)Enviroment::atlantis_state, -2);
		lua_pushcclosure((lua_State*)Enviroment::atlantis_state, scheduler_hook, nullptr, 0);
		lua_pcall((lua_State*)Enviroment::atlantis_state, 2, 0, 0);
		lua_settop((lua_State*)Enviroment::atlantis_state, 0);



		//lua_getglobal((lua_State*)Enviroment::atlantis_state, xorstr_("game"));
		//lua_getfield((lua_State*)Enviroment::atlantis_state, -1, xorstr_("GetService"));
		//lua_pushvalue((lua_State*)Enviroment::atlantis_state, -2);
		//lua_pushstring((lua_State*)Enviroment::atlantis_state, xorstr_("RuntimeScriptService"));
		//lua_pcall((lua_State*)Enviroment::atlantis_state, 2, 1, 0);

		//Enviroment::runtime_script_service = *reinterpret_cast<uintptr_t*>(lua_touserdata((lua_State*)Enviroment::atlantis_state, -1));
		//lua_pop((lua_State*)Enviroment::atlantis_state, 2);

		//init script encrypted to base64, placed in Utils.hpp file, you can make better method of hiding init if you dont like mine

		console_logs.push_back("[ATLANTIS] Loading init script...");

		coderun_queue.push(skibidi);
		//execute_script(Enviroment::atlantis_state, "print'hi,works'");

		console_logs.push_back("[ATLANTIS] Loaded init script");
		//coderun_queue.push(drawing_library);
		
//		coderun_queue.push(::crypt::base64::decode(Utils::lz4_library));

		std::filesystem::path path = Utils::AutoExecute::get();

		for (auto& file_iter : std::filesystem::directory_iterator(path))
		{
			std::ifstream f(file_iter.path(), std::ios::in | std::ios::binary);
			const auto sz = std::filesystem::file_size(file_iter.path());
			std::string result(sz, '\0');
			f.read(result.data(), sz);

			coderun_queue.push(result);
		}
	}
}
#define _WINSOCKAPI_

#define WIN32_LEAN_AND_MEAN


#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>
#include <queue>
#include <regex>
#include <map>
#include <format>
#include <set>


#include "json.hpp"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>

//#include <curl/curl.h>
#include <cpr/cpr.h>

#include "crypt.h"

#include "Utils.hpp"
#include "Roblox.hpp"

#include "Compiler/include/BytecodeBuilder.h"
#include "Compiler/include/Compiler.h"


#include <charconv>

#include <psapi.h>
#include "lz4/lz4.h"

#include "schedule_t.hpp"

#include "oxorany/oxorany_include.h"
#include "lua_websocket.hpp"
#include "miscshit/closures.hpp"
#include "miscshit/hooks.hpp"
#include "miscshit/wraps.hpp"
#include "rbx_connection.hpp"

#pragma comment(lib, "psapi.lib")


std::ofstream logFile;

void setupLogger()
{
	//SPOOF_FUNC;
	logFile.open(Utils::Workspace::get() / "log.txt");
}

void LOGD(const char* fmt, ...)
{
	if (!logFile.is_open())
		return;

	va_list args;
	va_start(args, fmt);

	va_list args_copy;
	va_copy(args_copy, args);
	int length = vsnprintf(nullptr, 0, fmt, args_copy);
	va_end(args_copy);

	std::string result;
	result.resize(length);
	vsnprintf(&result[0], length + 1, fmt, args);

	va_end(args);

	std::time_t nowTimeT = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm* tm = std::localtime(&nowTimeT);
	logFile << "[" << std::put_time(tm, "%d-%m-%Y %H:%M:%S") << "] " << result << "\n"; // %Y-%m-%d
	logFile.flush();
}


namespace fs = std::filesystem;

std::filesystem::path workspaceDir;
std::filesystem::path assetsDir;

namespace HelpFunctions {

	std::unordered_set<std::string> blacklistedExtensions = { ".csh", ".wsh", ".dll", ".bat", ".cmd", ".scr", ".vbs", ".js",
															 ".ts",  ".wsf", ".msi", ".com", ".lnk", ".ps1", ".py", "vb", ".vbe",
															 ".py3", ".pyc", ".pyw", ".vb", ".html", ".ws", ".psy", ".hta",
	};

	static bool IsExtensionBlacklisted(const fs::path& path) {
		const auto extension = path.extension().string();
		return blacklistedExtensions.find(extension) != blacklistedExtensions.end();
	}

	static bool IsPathSafe(const std::string& relativePath) {
		const fs::path base = fs::absolute(workspaceDir);
		const fs::path combined = base / relativePath;

		const fs::path normalizedBase = base.lexically_normal();
		const fs::path normalizedCombined = combined.lexically_normal();

		const auto baseStr = normalizedBase.string();
		const auto combinedStr = normalizedCombined.string();

		if (combinedStr.compare(0, baseStr.length(), baseStr) != 0)
			return false;

		std::string lowerRelativePath = relativePath;
		std::ranges::transform(lowerRelativePath, lowerRelativePath.begin(), ::tolower);

		if (lowerRelativePath.find("..") != std::string::npos)
			return false;

		if (IsExtensionBlacklisted(combined)) {
			return false;
		}

		return true;
	}

	static __forceinline std::filesystem::path CheckPath(lua_State* L) {
		const auto relativePath = lua_tostring(L, 1);

		if (!IsPathSafe(relativePath))
			luaG_runerror(L, "attempt to escape directory or access a blacklisted file type");

		return workspaceDir / relativePath;
	}

	static void Init() {

		wchar_t Path[MAX_PATH];
		wchar_t Path2[MAX_PATH];

		GetModuleFileNameW(GetModuleHandleA("trk30S1JhnwG.dll"), Path, MAX_PATH);
		const auto currentDirectory = fs::path(std::wstring(Path)).parent_path();

		workspaceDir = currentDirectory / "workspace";

		GetModuleFileNameW(GetModuleHandle(NULL), Path2, MAX_PATH);

		std::filesystem::path fullPath(Path2);
		std::filesystem::path contentDir = fullPath.parent_path() / "content";
		if (!std::filesystem::exists(contentDir)) {
			std::filesystem::create_directory(contentDir);
		}
		std::filesystem::path gcaDir = contentDir / "gca";
		if (!std::filesystem::exists(gcaDir)) {
			std::filesystem::create_directory(gcaDir);
		}

		std::wstring gcaPath = gcaDir.wstring();
		assetsDir = gcaDir;

	}

}


static atlantis::schedule_t<std::function<void()>> mYields;

namespace atlantis {
	namespace yielding {
		using yielded_t = std::function< int(lua_State*) >;




		auto begin_yield(lua_State* ls, const std::function<yielded_t()>& yielding_closure) -> int
		{
			// prevent the thread from being gced as were using it
			lua_pushthread(ls);
			const auto yielded_thread_ref = lua_ref(ls, -1);
			lua_pop(ls, 1);

			// https://github.com/Acrillis/SynapseX/blob/master/Synapse/Src/Exploit/Execution/RbxYield.cpp
			std::thread([=]
				{
					yielded_t resume_func;

					try
					{
						resume_func = yielding_closure();
					}
					catch (const std::exception& ex)
					{
						Defs::rbx_print(3, E("C++ Exception while yielding: %s"), ex.what());
						LOGD(E("C++ Exception while yielding: %s"), ex.what());

						return;
					}

					mYields.enqueue([=]() -> void
						{
							int nres{ 0 };
							try
							{
								nres = resume_func(ls);
							}
							catch (const std::exception ex)
							{
								auto extraSpace = static_cast<ExtraSpace*>(ls->userdata);

								auto scriptContext = extraSpace->sharedExtraSpace->scriptContext;

								//Defs::rbx_print(1, "offsetof: %x", offsetof(extraSpace, sharedExtraSpace));

								weak_thread_ref_t2 weak_thread_ref_object = { ls };
								lua_pushthread(ls);
								weak_thread_ref_object.thread_ref = lua_ref(ls, -1);
								lua_pop(ls, 1);

								weak_thread_ref_t2* weak_thread_ref = &weak_thread_ref_object;
								debugger_result_t debugger_results = { 0 };
								Defs::scriptcontext_resume((uintptr_t)scriptContext + 0x610, &debugger_results, &weak_thread_ref, 0, true, ex.what());

								lua_unref(ls, yielded_thread_ref);
								return;
							}

							auto extraSpace = static_cast<ExtraSpace*>(ls->userdata);

							auto scriptContext = extraSpace->sharedExtraSpace->scriptContext;

							weak_thread_ref_t2 weak_thread_ref_object = { ls };
							lua_pushthread(ls);
							weak_thread_ref_object.thread_ref = lua_ref(ls, -1);
							lua_pop(ls, 1);

							weak_thread_ref_t2* weak_thread_ref = &weak_thread_ref_object;
							debugger_result_t debugger_results = { 0 };

							LOGD("pluto::yielding::begin_yield, thread: %p", ls);


							Defs::scriptcontext_resume((uintptr_t)scriptContext + 0x610, &debugger_results, &weak_thread_ref, nres, 0, nullptr);

							LOGD("pluto::yielding::begin_yield: Resumed");

							lua_unref(ls, yielded_thread_ref);
						});
				}).detach();

			return lua_yield(ls, 0);
		}

		auto resume_error(lua_State* L, const char* fmt, ...) -> void
		{
	
			auto extraSpace = static_cast<ExtraSpace*>(L->userdata);

			auto scriptContext = extraSpace->sharedExtraSpace->scriptContext;

			//Defs::rbx_print(1, "offsetof: %x", offsetof(extraSpace, sharedExtraSpace));

			weak_thread_ref_t2 weak_thread_ref_object = { L };
			lua_pushthread(L);
			weak_thread_ref_object.thread_ref = lua_ref(L, -1);
			lua_pop(L, 1);

			weak_thread_ref_t2* weak_thread_ref = &weak_thread_ref_object;
			debugger_result_t debugger_results = { 0 };
			Defs::scriptcontext_resume((uintptr_t)scriptContext + 0x610, &debugger_results, &weak_thread_ref, 0, 1, fmt);

		}

	}
}



namespace Enviroment
{
	static std::uintptr_t runtime_script_service;
	inline __int64 atlantis_state = 0;
	static std::vector<Closure*> function_array;
	static std::map<Closure*, Closure*> newcclosure_map;
	static std::map<Closure*, Closure*> hooked_functions;
	static std::map<lua_State*, int> newcclosure_threads;
	static std::vector<std::string> teleport_queue;
	static __int64 loadmodule_ref{ 0 };

	


	enum type
	{
		roblox_c_closure,
		module_c_closure,
		module_c_wrap,
		l_closure,
		not_set
	};

	template< class t >
	class c_storage
	{
		std::mutex m_l;

	protected:
		t m_container;
	public:

		auto safe_request(auto request, auto... args)
		{
			std::unique_lock l{ m_l };

			return request(args...);
		};

		void clear()
		{
			safe_request([&]()
				{ m_container.clear(); });
		}
	};

	using instance_cache_t
		= std::unordered_map< void*,
		std::unordered_map< std::string, bool > >;

	class c_instance_cache : public c_storage< instance_cache_t >
	{
	public:
		void toggle(void* instance, const std::string& prop, bool enabled)
		{
			safe_request([&]()
				{
					m_container[instance][prop] = enabled;
				});
		}

		std::optional< bool > is_scriptable(void* instance, const std::string& prop)
		{
			return safe_request([&]() -> std::optional< bool >
				{
					if (m_container.contains(instance))
					{
						const auto properties = m_container[instance];

						if (properties.contains(prop))
							return properties.at(prop);
					}

					return std::nullopt;
				});
		}
	} inline g_instance;

	namespace Yielding
	{
		using YieldReturn = std::function<int(lua_State* L)>;

		void ThreadFunc(const std::function<YieldReturn()>& YieldedFunction, lua_State* L)
		{
			YieldReturn ret_func;

			try
			{
				ret_func = YieldedFunction();
			}
			catch (std::exception ex)
			{
				lua_pushstring(L, ex.what());
				lua_error(L);
			}

			lua_State* l_new = lua_newthread(L);

			const auto returns = ret_func(L);

			lua_getglobal(l_new, E("task"));
			lua_getfield(l_new, -1, E("defer"));

			lua_pushthread(L);
			lua_xmove(L, l_new, 1);

			for (int i = returns; i >= 1; i--)
			{
				lua_pushvalue(L, -i);
				lua_xmove(L, l_new, 1);
			}

			lua_pcall(l_new, returns + 1, 0, 0);
			lua_settop(l_new, 0);
		}

		int DoYield(lua_State* L, const std::function<YieldReturn()>& YieldedFunction)
		{
			lua_pushthread(L);
			lua_ref(L, -1);
			lua_pop(L, 1);

			std::thread(ThreadFunc, YieldedFunction, L).detach();

			L->base = L->top;
			L->status = LUA_YIELD;

			L->ci->flags |= 1;
			return -1;
		}
	}

	namespace Handler
	{
		static std::map<Closure*, lua_CFunction> cfunction_map = {};

		static int cfunction_handler(lua_State* rl)
		{
			auto found = cfunction_map.find(curr_func(rl));

			if (found != cfunction_map.end())
			{
				return found->second(rl);
			}
			return 0;
		}


		static lua_CFunction get(Closure* cl)
		{
			return cfunction_map[cl];
		}


		static void set(Closure* cl, lua_CFunction cf)
		{
			cfunction_map[cl] = cf;
		}


		static void push(lua_State* rl, lua_CFunction fn, const char* debugname, int nup)
		{
			lua_pushcclosurek(rl, cfunction_handler, debugname, nup, 0);
			//Closure* closure = managers::closures::pushcclosure(rl, cfunction_handler, debugname, nup, 0);
			Closure* closure = *reinterpret_cast<Closure**>(index2addr(rl, -1));
			cfunction_map[closure] = fn;
		}

		static void push_newcc(lua_State* rl, lua_CFunction fn, const char* debugname, int nup, lua_Continuation count)
		{
			lua_pushcclosurek(rl, cfunction_handler, debugname, nup, count);
			Closure* closure = *reinterpret_cast<Closure**>(index2addr(rl, -1));
			cfunction_map[closure] = fn;
		}


		namespace wraps
		{
			static Closure* get(Closure* c)
			{
				return newcclosure_map.find(c)->second;
			}

			static void set(Closure* c, Closure* l)
			{
				newcclosure_map[c] = l;
			}
		}
	}


	static void push_global(lua_State* rl, const char* globalname, lua_CFunction function)
	{
		//managers::closures::pushcclosure(rl, (lua_CFunction)function, (const char*)globalname, 0, nullptr);
		Handler::push(rl, function, nullptr, 0);
		Closure* closure = *reinterpret_cast<Closure**>(index2addr(rl, -1));
		function_array.push_back(closure);
		lua_setfield(rl, LUA_GLOBALSINDEX, globalname);
	}


	static void push_member(lua_State* rl, const char* globalname, lua_CFunction function)
	{
		Handler::push(rl, function, nullptr, 0);
		Closure* closure = *reinterpret_cast<Closure**>(index2addr(rl, -1));
		function_array.push_back(closure);
		//managers::closures::pushcclosure(rl, (lua_CFunction)function, (const char*)globalname, 0, nullptr);
		lua_setfield(rl, -2, globalname);
	}

	namespace Functions
	{
		namespace Env
		{
			auto getgenv(lua_State* rl) -> int
			{
				LOGD("getgenv");

				lua_pushvalue(rl, LUA_ENVIRONINDEX);
				return 1;
			}

			auto getreg(lua_State* rl) -> int
			{
				LOGD("getreg");

				lua_pushvalue(rl, LUA_REGISTRYINDEX);
				return 1;
			}

			auto getrenv(lua_State* rl) -> int
			{
				LOGD("getrenv");

				setobj(rl, rl->top, index2addr(rl->global->mainthread, LUA_GLOBALSINDEX));
				incr_top(rl);
				return 1;
			}

			/*auto getgc(lua_State* state) -> int
			{
				bool full = false;

				if (lua_gettop(state) > 0 && lua_type(state, 1) == LUA_TBOOLEAN)
					full = lua_toboolean(state, 1);

				lua_newtable(state);

				auto cur_page = state->global->allgcopages;
				int idx{};

				while (cur_page)
				{
					char* start = 0;
					char* end = 0;
					auto block = 0;
					auto size = 0;

					luaM_getpagewalkinfo(cur_page, &start, &end, &block, &size);

					for (auto pos = start; pos != end; pos += size)
					{
						const auto gco = reinterpret_cast<GCObject*>(pos);

						if (gco->gch.tt == LUA_TFUNCTION || ((gco->gch.tt == LUA_TTABLE || gco->gch.tt == LUA_TUSERDATA) && full))
						{

							state->top->value.gc = gco;
							state->top->tt = gco->gch.tt;
							state->top++;

							lua_rawseti(state, -2, ++idx);
						}
					}

					cur_page = cur_page->listnext;
				}

				return 1;
			}*/

			int getgc(lua_State* L) {
				LOGD("getgc");

				const bool addTables = luaL_optboolean(L, 1, false);
				lua_newtable(L);

				typedef struct {
					lua_State* pLua;
					bool accessTables;
					int itemsFound;
				} GCOContext;

				auto gcCtx = GCOContext{ L, addTables, 0 };

				const auto ullOldThreshold = L->global->GCthreshold;
				L->global->GCthreshold = SIZE_MAX;
				// Never return true. We aren't deleting shit.
				luaM_visitgco(L, &gcCtx, [](void* ctx, lua_Page* pPage, GCObject* pGcObj) -> bool {
					const auto pCtx = static_cast<GCOContext*>(ctx);
					const auto ctxL = pCtx->pLua;

					if (isdead(ctxL->global, pGcObj))
						return false; // The object is being collected/checked. Skip it.

					if (const auto gcObjType = pGcObj->gch.tt;
						gcObjType == LUA_TFUNCTION || gcObjType == LUA_TUSERDATA || gcObjType == LUA_TBUFFER ||
						gcObjType == LUA_TLIGHTUSERDATA || gcObjType == LUA_TTABLE && pCtx->accessTables) {
						// Push copy to top of stack.
						ctxL->top->value.gc = pGcObj;
						ctxL->top->tt = gcObjType;
						ctxL->top++;

						// Store onto GC table.
						const auto tIndx = pCtx->itemsFound++;
						lua_rawseti(ctxL, -2, tIndx + 1);
					}
					return false;
					});
				L->global->GCthreshold = ullOldThreshold;

				return 1;
			}



			auto getttenv(lua_State* rl) -> int
			{
				LOGD("gettenv");

				luaL_checktype(rl, 1, LUA_TTHREAD);
				lua_State* ls = (lua_State*)lua_topointer(rl, 1);
				Table* tab = hvalue(luaA_toobject(ls, LUA_GLOBALSINDEX));

				sethvalue(rl, rl->top, tab);
				rl->top++;

				return 1;
			}

			int YieldingTest(lua_State* ls)
			{
				LOGD("yieldingtest");

				return atlantis::yielding::begin_yield(ls, []() -> atlantis::yielding::yielded_t
					{
						int real = 0;

						Sleep(10000);

						return ([real](lua_State* ls) {

							lua_pushstring(ls, "omg");
							return 1;
							});
			});
			}
		}

		namespace Console {
			auto rconsoleprint(lua_State* L) -> int {
				luaL_checktype(L, 1, LUA_TSTRING);

				std::string txt = lua_tostring(L, 1);

				console_logs.push_back(txt);

				return 0;
			}
		}


		namespace Closures
		{
			auto is_executor_closure(lua_State* rl) -> int
			{
				LOGD("isexecutorclosure");
				if (lua_type(rl, 1) != LUA_TFUNCTION) { lua_pushboolean(rl, false); return 1; }

				Closure* closure = clvalue(index2addr(rl, 1));
				bool value = false;

				if (lua_isLfunction(rl, 1))
				{
					value = closure->l.p->linedefined;
				}
				else
				{
					for (int i = 0; i < function_array.size(); i++)
					{
						if (function_array[i]->c.f == closure->c.f)
						{
							value = true;
							break;
						}
					}
				}

				lua_pushboolean(rl, value);
				return 1;
			}

			__forceinline int getfunctionbytecode(lua_State* L) {
				LOGD("getfunctionhash");

				luaL_checktype(L, 1, LUA_TFUNCTION);

				Closure* cl = (Closure*)lua_topointer(L, 1);

				// closure info
				uint8_t nupvalues = cl->nupvalues;
				Proto* p = (Proto*)cl->l.p;
				
				std::string result =
					std::to_string((int)p->sizep) + "," +
					std::to_string((int)p->sizelocvars) + "," +
					std::to_string((int)p->sizeupvalues) + "," +
					std::to_string((int)p->sizek) + "," +
					std::to_string((int)p->sizelineinfo) + "," +
					std::to_string((int)p->linegaplog2) + "," +
					std::to_string((int)p->linedefined) + "," +
					std::to_string((int)p->bytecodeid) + "," +
					std::to_string((int)p->sizetypeinfo) + "," +
					std::to_string(nupvalues);

				std::string hash = ::crypt::hash(result, "sha256");

				lua_pushstring(L, hash.c_str());

				return 1;
			}

			__forceinline int lz4compress(lua_State* L) {
				LOGD("lz4compress");

				luaL_checktype(L, 1, LUA_TSTRING);
				const char* data = lua_tolstring(L, 1,0);
				int nMaxCompressedSize = LZ4_compressBound(strlen(data));
				char* out_buffer = new char[nMaxCompressedSize];

				LZ4_compress(data, out_buffer, strlen(data));
				lua_pushlstring(L, out_buffer, nMaxCompressedSize);
				return 1;
			}

			__forceinline int lz4decompress(lua_State* L) {
				LOGD("lz4decompress");

				luaL_checktype(L, 1, LUA_TSTRING);
				luaL_checktype(L, 2, LUA_TNUMBER);
				const char* data = lua_tolstring(L, 1,0);
				int data_size = lua_tointeger(L, 2);

				char* pszUnCompressedFile = new char[data_size];

				LZ4_uncompress(data, pszUnCompressedFile, data_size);
				lua_pushlstring(L, pszUnCompressedFile, data_size);
				return 1;
			}


			using luau_function_t = int(__fastcall*)(lua_State* L); /* Compiler automatically compiles __cdecl, __thiscall, etc into __fastcall */

			auto getscriptclosure(lua_State* L) -> int
			{
				LOGD("getscriptclosure");

				auto script = *(uintptr_t*)(lua_touserdata(L, 1));
				auto voidptr_script = lua_touserdata(L, 1);

				const char* classname = *reinterpret_cast<const char**>(*reinterpret_cast<uintptr_t*>(script + 24) + 8);
				std::string protectedstring_bytecode;

				if (strcmp(classname, xorstr_("LocalScript")) == 0) {
					protectedstring_bytecode = *reinterpret_cast<std::string*>(*reinterpret_cast<uintptr_t*>(script + Offsets::Bytecode::localscript) + 16);



					if (Defs::rbx_luavmload((__int64)L, &protectedstring_bytecode, xorstr_("@"), 0) != LUA_OK) {
						lua_pushnil(L);
						return 1;
					}


					Closure* pClosure = const_cast<Closure*>(static_cast<const Closure*>(lua_topointer(L, -1)));
					ProtoThings::raise_proto(pClosure->l.p);

					return 1;
				}
				else if (strcmp(classname, xorstr_("ModuleScript")) == 0) {
					*(bool*)(Offsets::ModuleScript::EnableLoadModule) = true;



					lua_getglobal(L, "debug");
					lua_getfield(L, -1, "loadmodule");
					if (lua_isfunction(L, -1)) {
						lua_pushvalue(L, 1);
						lua_call(L, 1, 1);

						*(bool*)(Offsets::ModuleScript::EnableLoadModule) = false;

						if (!lua_isnoneornil(L, -1))
						{
							return 1;
						}
						lua_settop(L, 0);
						lua_pushnil(L);
						return 0;
					}
					else {
						lua_settop(L, 0);
						lua_pushnil(L);
						return 1;
					}
				}
				else
				{
					luaL_error(L, xorstr_("LocalScript or ModuleScript expected."));
					return 0;
				}
			}

			auto iscclosure(lua_State* rl) -> int
			{
				LOGD("iscclosure");

				luaL_checktype(rl, 1, LUA_TFUNCTION);

				lua_pushboolean(rl, lua_iscfunction(rl, 1));
				return 1;
			}

			auto islclosure(lua_State* rl) -> int
			{
				LOGD("islclosure");

				luaL_checktype(rl, 1, LUA_TFUNCTION);

				lua_pushboolean(rl, lua_isLfunction(rl, 1));
				return 1;
			}

			auto checkcaller(lua_State* rl) -> int
			{
				LOGD("checkcaller");

				//const auto passed = (*(std::uintptr_t*)((std::uintptr_t)(rl->userdata) + 0x48) & (1ull << 48ull)) == (1ull << 48ull);

				const auto script_ptr = *(std::uintptr_t*)((std::uintptr_t)((rl->userdata)) + 0x50);
					 
				lua_pushboolean(rl, !script_ptr);
				return 1;
			}

			auto loadstring(lua_State* rl) -> int
			{
				LOGD("loadstring");

				luaL_checktype(rl, 1, LUA_TSTRING);

				std::string chunk;
				std::string source = luaL_checkstring(rl, 1);

				if (lua_tostring(rl, 2).size() == 0)
					chunk = std::string("=" + Utils::random_str(16));
				else
					chunk = luaL_checkstring(rl, 2);

				auto bytecode = Utils::compress_bytecode(Luau::compile(source));
				
				if (Defs::rbx_luavmload((__int64)rl, &bytecode, chunk.c_str(), 0) != LUA_OK)
				{
					std::string err = lua_tostring(rl, -1);
					lua_pop(rl, 1);

					lua_pushnil(rl);
					lua_pushlstring(rl, err.c_str(), err.size());
					return 2;
				}
		/*		if (luau_load(rl, chunk.c_str(), bytecode.c_str(), bytecode.size(), 0) != lua_Status::LUA_OK) {
					lua_pushnil(rl);
					lua_pushvalue(rl, -2);
					return 2;
				}*/

				Closure* pClosure = const_cast<Closure*>(static_cast<const Closure*>(lua_topointer(rl, -1)));
				ProtoThings::raise_proto(pClosure->l.p);

				return 1;
			}

			auto newcclosure_handler(lua_State* rl) -> int {
				const auto nargs = lua_gettop(rl);
				rl->ci->flags |= LUA_CALLINFO_HANDLE;
				void* real_closure = reinterpret_cast<void*>(newcclosure_map.find(clvalue(rl->ci->func))->second);

				if (real_closure == nullptr)
					return 0;

				rl->top->value.p = real_closure;
				rl->top->tt = LUA_TFUNCTION;
				rl->top++;

				lua_insert(rl, 1);

				const char* error = nullptr;
				rl->baseCcalls++;
				const int res = lua_pcall(rl, nargs, LUA_MULTRET, 0);
				rl->baseCcalls--;

				if (res == 0 && (rl->status == LUA_YIELD || rl->status == LUA_BREAK))
					return -1;

				return lua_gettop(rl);
			}

			static type get_type(Closure* cl)
			{
				auto cl_type = not_set;

				if (!cl->isC)
					cl_type = l_closure;
				else
				{
					if (reinterpret_cast<lua_CFunction>((lua_CFunction)cl->c.f) == Handler::cfunction_handler)
					{
						if (Handler::get(cl) == newcclosure_handler)
							cl_type = module_c_wrap;
						else
							cl_type = module_c_closure;
					}
					else
						cl_type = roblox_c_closure;
				}

				return cl_type;
			}


			__forceinline int iscustomcclosure(lua_State* L) {
				LOGD("iscustomcclosure");

				luaL_checktype(L, 1, LUA_TFUNCTION);

				Closure* cl = (Closure*)lua_topointer(L, 1);

				if (newcclosure_map.find(cl) != newcclosure_map.end()) {
					lua_pushboolean(L, true);
				}
				else {
					lua_pushboolean(L, false);
				}


				return 1;
			}

			__forceinline int isfunctionhooked(lua_State* L) {
				LOGD("isfunctionhooked");

				luaL_checktype(L, 1, LUA_TFUNCTION);

				Closure* cl = (Closure*)lua_topointer(L, 1);

				if (hooked_functions.find(cl) != hooked_functions.end()) {
					lua_pushboolean(L, true);
				}
				else {
					lua_pushboolean(L, false);
				}

				return 1;
			}

			auto clonefunction(lua_State* rl) -> int
			{
				LOGD("clonefunction");

				luaL_checktype(rl, 1, LUA_TFUNCTION);

				switch (get_type(clvalue(index2addr(rl, 1))))
				{
				case roblox_c_closure:
					lua_clonecfunction(rl, 1);
					break;
				case module_c_closure:
					lua_clonecfunction(rl, 1);
					Handler::set(clvalue(index2addr(rl, -1)), Handler::get(clvalue(index2addr(rl, 1))));
					break;
				case module_c_wrap:
					lua_clonecfunction(rl, 1);
					Handler::set(clvalue(index2addr(rl, -1)), Handler::get(clvalue(index2addr(rl, 1))));
					Handler::wraps::set(clvalue(index2addr(rl, -1)), Handler::wraps::get(clvalue(index2addr(rl, 1))));
					break;
				case l_closure:
					lua_clonefunction(rl, 1);
					break;
				}

				return 1;
			}

			int newcclosure_cont(lua_State* rl, int status) {
				if (!status) return lua_gettop(rl);
				luaL_error(rl, lua_tolstring(rl, -1, nullptr));
				return 0;
			}

			auto newcclosure(lua_State* rl) -> int
			{
				LOGD("newcclosure");
				luaL_checktype(rl, 1, LUA_TFUNCTION);

				if (lua_iscfunction(rl, 1))
					luaL_error(rl, "L closure expected.");

				auto tval1 = (TValue*)index2addr(rl, 1);

				lua_ref(rl, 1);
				Handler::push_newcc(rl, newcclosure_handler, 0, 0, newcclosure_cont);
				Closure* catched = clvalue(index2addr(rl, -1));
				function_array.push_back(catched);

				newcclosure_map[catched] = &tval1->value.gc->cl;

				return 1;
			}

			static int hookfunction(lua_State* rl)
			{


				LOGD("hookfunction");

				luaL_checktype(rl, 1, LUA_TFUNCTION);
				luaL_checktype(rl, 2, LUA_TFUNCTION);

				const auto cl1 = clvalue(index2addr(rl, 1));
				const auto cl2 = clvalue(index2addr(rl, 2));
				int nups1 = cl1->nupvalues;
				int nups2 = cl2->nupvalues;


				if (lua_iscfunction(rl, 1)) {
					lua_clonecfunction(rl, 1);
				}
				else {
					lua_clonefunction(rl, 1);
				}

				hooked_functions[cl1] = (Closure*)lua_topointer(rl, -1);

				lua_pushvalue(rl, 1);
				lua_setfield(rl, LUA_REGISTRYINDEX, Utils::random_str(32).c_str());

				if (get_type(cl1) == roblox_c_closure && get_type(cl2) == roblox_c_closure)
				{
					if (nups1 >= nups2)
					{
						lua_clonecfunction(rl, 1);

						cl1->c.f = (lua_CFunction)cl2->c.f;
						cl1->c.cont = (lua_Continuation)cl2->c.cont;
						cl1->env = (Table*)cl2->env;
						cl1->stacksize = cl2->stacksize;


						for (int i = 0; i < nups1; i++)
							setobj2n(rl, &cl1->c.upvals[i], luaO_nilobject);

						for (int i = 0; i < nups2; i++)
							setobj2n(rl, &cl1->c.upvals[i], &cl2->c.upvals[i]);
					}
					else
						luaL_error(rl, "Too many upvalues");
				}

				else if (get_type(cl1) == module_c_closure && get_type(cl2) == module_c_closure)
				{
					if (nups1 >= nups2)
					{
						lua_clonecfunction(rl, 1);
						Handler::set(clvalue(index2addr(rl, -1)), Handler::get(cl1));

						Handler::set(cl1, Handler::get(cl2));
						cl1->c.f = (lua_CFunction)cl2->c.f;
						cl1->c.cont = (lua_Continuation)cl2->c.cont;
						cl1->env = (Table*)cl2->env;
						cl1->stacksize = cl2->stacksize;

						for (int i = 0; i < nups1; i++)
							setobj2n(rl, &cl1->c.upvals[i], luaO_nilobject);

						for (int i = 0; i < nups2; i++)
							setobj2n(rl, &cl1->c.upvals[i], &cl2->c.upvals[i]);
					}
					else
						luaL_error(rl, "Too many upvalues");
				}

				else if (get_type(cl1) == module_c_wrap && get_type(cl2) == module_c_wrap)
				{
					lua_clonecfunction(rl, 1);
					Handler::set(clvalue(index2addr(rl, -1)), Handler::get(cl1));

					Handler::wraps::set(clvalue(index2addr(rl, -1)), Handler::wraps::get(cl1));
					Handler::wraps::set(cl1, Handler::wraps::get(cl2));

					cl1->env = (Table*)cl2->env;
					cl1->c.cont = (lua_Continuation)cl2->c.cont;
					cl1->stacksize = cl2->stacksize;
					cl1->nupvalues = cl2->nupvalues;
				}

				else if (get_type(cl1) == l_closure && get_type(cl2) == l_closure)
				{
					if (nups1 >= nups2)
					{
						lua_clonefunction(rl, 1);
						lua_ref(rl, 1);

						cl1->l.p = (Proto*)cl2->l.p;
						cl1->env = (Table*)cl2->env;

						for (int i = 0; i < nups1; i++)
							setobj2n(rl, &cl1->l.uprefs[i], luaO_nilobject);

						for (int i = 0; i < nups2; i++)
							setobj2n(rl, &cl1->l.uprefs[i], &cl2->l.uprefs[i]);
					}
					else
						luaL_error(rl, "Too many upvalues");
				}

				else if (get_type(cl1) == roblox_c_closure && get_type(cl2) == module_c_closure)
				{
					if (nups1 >= nups2)
					{
						lua_clonecfunction(rl, 1);

						Handler::set(cl1, Handler::get(cl2));

						cl1->c.f = (lua_CFunction)cl2->c.f;
						cl1->c.cont = (lua_Continuation)cl2->c.cont;
						cl1->env = (Table*)cl2->env;
						cl1->stacksize = cl2->stacksize;

						for (int i = 0; i < nups1; i++)
							setobj2n(rl, &cl1->c.upvals[i], luaO_nilobject);

						for (int i = 0; i < nups2; i++)
							setobj2n(rl, &cl1->c.upvals[i], &cl2->c.upvals[i]);
					}
					else
						luaL_error(rl, "Too many upvalues");
				}

				else if (get_type(cl1) == roblox_c_closure && get_type(cl2) == module_c_wrap)
				{
					lua_clonecfunction(rl, 1);

					Handler::set(cl1, Handler::get(cl2));
					Handler::wraps::set(cl1, Handler::wraps::get(cl2));

					cl1->c.f = (lua_CFunction)cl2->c.f;
					cl1->c.cont = (lua_Continuation)cl2->c.cont;
					cl1->env = (Table*)cl2->env;
					cl1->stacksize = cl2->stacksize;
				}

				else if (get_type(cl1) == roblox_c_closure && get_type(cl2) == l_closure)
				{
					lua_clonecfunction(rl, 1);
					lua_ref(rl, 2);

					cl1->c.f = (lua_CFunction)Handler::cfunction_handler;
					Handler::set(cl1, newcclosure_handler);
					Handler::wraps::set(cl1, cl2);
				}

				else if (get_type(cl1) == module_c_closure && get_type(cl2) == roblox_c_closure)
				{
					if (nups1 >= nups2)
					{
						lua_clonecfunction(rl, 1);
						Handler::set(clvalue(index2addr(rl, -1)), Handler::get(cl1));

						cl1->env = (Table*)cl2->env;
						cl1->c.f = (lua_CFunction)cl2->c.f;
						cl1->c.cont = (lua_Continuation)cl2->c.cont;
						cl1->stacksize = cl2->stacksize;

						for (int i = 0; i < nups1; i++)
							setobj2n(rl, &cl1->c.upvals[i], luaO_nilobject);

						for (int i = 0; i < nups2; i++)
							setobj2n(rl, &cl1->c.upvals[i], &cl2->c.upvals[i]);
					}
					else
						luaL_error(rl, "Too many upvalues");
				}

				else if (get_type(cl1) == module_c_closure && get_type(cl2) == module_c_wrap)
				{
					lua_clonecfunction(rl, 1);
					Handler::set(clvalue(index2addr(rl, -1)), Handler::get(cl1));

					Handler::set(cl1, newcclosure_handler);
					Handler::wraps::set(cl1, cl2);
				}

				else if (get_type(cl1) == module_c_closure && get_type(cl2) == l_closure)
				{
					lua_clonecfunction(rl, 1);
					Handler::set(clvalue(index2addr(rl, -1)), Handler::get(cl1));
					lua_ref(rl, 2);

					Handler::set(cl1, newcclosure_handler);
					Handler::wraps::set(cl1, cl2);
				}

				else if (get_type(cl1) == module_c_wrap && get_type(cl2) == roblox_c_closure)
				{
					if (nups1 >= nups2)
					{
						lua_clonecfunction(rl, 1);
						Handler::set(clvalue(index2addr(rl, -1)), Handler::get(cl1));
						Handler::wraps::set(clvalue(index2addr(rl, -1)), Handler::wraps::get(cl1));

						cl1->env = (Table*)cl2->env;
						cl1->c.f = (lua_CFunction)cl2->c.f;
						cl1->c.cont = (lua_Continuation)cl2->c.cont;
						cl1->stacksize = cl2->stacksize;

						for (int i = 0; i < nups1; i++)
							setobj2n(rl, &cl1->c.upvals[i], luaO_nilobject);

						for (int i = 0; i < nups2; i++)
							setobj2n(rl, &cl1->c.upvals[i], &cl2->c.upvals[i]);
					}
					else
						luaL_error(rl, "Too many upvalues");
				}

				else if (get_type(cl1) == module_c_wrap && get_type(cl2) == module_c_closure)
				{
					if (nups1 >= nups2)
					{
						lua_clonecfunction(rl, 1);
						Handler::set(clvalue(index2addr(rl, -1)), Handler::get(cl1));
						Handler::wraps::set(clvalue(index2addr(rl, -1)), Handler::wraps::get(cl1));

						Handler::set(cl1, Handler::get(cl2));

						cl1->env = (Table*)cl2->env;
						cl1->c.cont = (lua_Continuation)cl2->c.cont;

						for (int i = 0; i < nups2; i++)
							setobj2n(rl, &cl1->c.upvals[i], &cl2->c.upvals[i]);
					}
					else
						luaL_error(rl, "Too many upvalues");
				}

				else if (get_type(cl1) == module_c_wrap && get_type(cl2) == l_closure)
				{
					lua_clonecfunction(rl, 1);
					Handler::set(clvalue(index2addr(rl, -1)), Handler::get(cl1));
					Handler::wraps::set(clvalue(index2addr(rl, -1)), Handler::wraps::get(cl1));

					lua_ref(rl, 2);
					Handler::wraps::set(cl1, cl2);
				}

				else if (get_type(cl1) == l_closure && (get_type(cl2) == roblox_c_closure || get_type(cl2) == module_c_closure))
				{
					lua_clonefunction(rl, 1);
					const auto& spoof = Luau::compile(E("local f = function() end; return f(...); "));
					luau_load(rl, cl1->l.p->source->data, spoof.c_str(), spoof.size(), 0);

					Closure* clspoof = clvalue(index2addr(rl, -1));
					setobj(rl, &clspoof->l.p->k[0], index2addr(rl, 2));
					clspoof->l.p->linedefined = (__int64)cl1->l.p->linedefined;
					cl1->l.p = (Proto*)clspoof->l.p;

					lua_pop(rl, 1);
				}

				else if (get_type(cl1) == l_closure && get_type(cl2) == module_c_wrap)
				{
					const Closure* l = Handler::wraps::get(cl2);

					if (nups1 >= l->nupvalues)
					{
						lua_clonefunction(rl, 1);

						cl1->env = (Table*)l->env;
						cl1->l.p = (Proto*)l->l.p;

						for (int i = 0; i < cl1->nupvalues; i++)
							setobj2n(rl, &cl1->l.uprefs[i], luaO_nilobject);

						for (int i = 0; i < l->nupvalues; i++)
							setobj2n(rl, &cl1->l.uprefs[i], &l->l.uprefs[i]);
					}
					else
						luaL_error(rl, "Too many upvalues");
				}
				else
					luaL_error(rl, "Failed to hook");


				return 1;
			}

			static int restorefunction(lua_State* rl)
			{
				LOGD("restorefunction");

				luaL_checktype(rl, 1, LUA_TFUNCTION);
				const auto cl1 = clvalue(index2addr(rl, 1));

				if (hooked_functions.find(cl1) == hooked_functions.end()) {
					luaL_error(rl, "closure not hooked");
					return 0;
				}

				const auto cl2 = hooked_functions[cl1];

				int nups1 = cl1->nupvalues;
				int nups2 = cl2->nupvalues;



				lua_pushvalue(rl, 1);
				lua_setfield(rl, LUA_REGISTRYINDEX, Utils::random_str(32).c_str());

				if (get_type(cl1) == roblox_c_closure && get_type(cl2) == roblox_c_closure)
				{
					if (nups1 >= nups2)
					{
						lua_clonecfunction(rl, 1);

						cl1->c.f = (lua_CFunction)cl2->c.f;
						cl1->c.cont = (lua_Continuation)cl2->c.cont;
						cl1->env = (Table*)cl2->env;

						for (int i = 0; i < nups2; i++)
							setobj2n(rl, &cl1->c.upvals[i], &cl2->c.upvals[i]);
					}
					else
						luaL_error(rl, "Too many upvalues");
				}

				else if (get_type(cl1) == module_c_closure && get_type(cl2) == module_c_closure)
				{
					if (nups1 >= nups2)
					{
						lua_clonecfunction(rl, 1);
						Handler::set(clvalue(index2addr(rl, -1)), Handler::get(cl1));

						Handler::set(cl1, Handler::get(cl2));
						cl1->c.cont = (lua_Continuation)cl2->c.cont;
						cl1->env = (Table*)cl2->env;

						for (int i = 0; i < nups2; i++)
							setobj2n(rl, &cl1->c.upvals[i], &cl2->c.upvals[i]);
					}
					else
						luaL_error(rl, "Too many upvalues");
				}

				else if (get_type(cl1) == module_c_wrap && get_type(cl2) == module_c_wrap)
				{
					lua_clonecfunction(rl, 1);
					Handler::set(clvalue(index2addr(rl, -1)), Handler::get(cl1));

					Handler::wraps::set(clvalue(index2addr(rl, -1)), Handler::wraps::get(cl1));
					Handler::wraps::set(cl1, Handler::wraps::get(cl2));

					cl1->env = (Table*)cl2->env;
				}

				else if (get_type(cl1) == l_closure && get_type(cl2) == l_closure)
				{
					if (nups1 >= nups2)
					{
						lua_clonefunction(rl, 1);

						cl1->l.p = (Proto*)cl2->l.p;
						cl1->env = (Table*)cl2->env;

						for (int i = 0; i < nups2; i++)
							setobj2n(rl, &cl1->l.uprefs[i], &cl2->l.uprefs[i]);
					}
					else
						luaL_error(rl, "Too many upvalues");
				}

				else if (get_type(cl1) == roblox_c_closure && get_type(cl2) == module_c_closure)
				{
					if (nups1 >= nups2)
					{
						lua_clonecfunction(rl, 1);

						Handler::set(cl1, Handler::get(cl2));

						cl1->c.f = (lua_CFunction)cl2->c.f;
						cl1->c.cont = (lua_Continuation)cl2->c.cont;
						cl1->env = (Table*)cl2->env;

						for (int i = 0; i < nups2; i++)
							setobj2n(rl, &cl1->c.upvals[i], &cl2->c.upvals[i]);
					}
					else
						luaL_error(rl, "Too many upvalues");
				}

				else if (get_type(cl1) == roblox_c_closure && get_type(cl2) == module_c_wrap)
				{
					lua_clonecfunction(rl, 1);

					Handler::set(cl1, Handler::get(cl2));
					Handler::wraps::set(cl1, Handler::wraps::get(cl2));

					cl1->c.f = (lua_CFunction)cl2->c.f;
					cl1->c.cont = (lua_Continuation)cl2->c.cont;
					cl1->env = (Table*)cl2->env;
				}

				else if (get_type(cl1) == roblox_c_closure && get_type(cl2) == l_closure)
				{
					lua_clonecfunction(rl, 1);
					lua_ref(rl, 2);

					cl1->c.f = (lua_CFunction)Handler::cfunction_handler;
					Handler::set(cl1, newcclosure_handler);
					Handler::wraps::set(cl1, cl2);
				}

				else if (get_type(cl1) == module_c_closure && get_type(cl2) == roblox_c_closure)
				{
					if (nups1 >= nups2)
					{
						lua_clonecfunction(rl, 1);
						Handler::set(clvalue(index2addr(rl, -1)), Handler::get(cl1));

						cl1->env = (Table*)cl2->env;
						cl1->c.f = (lua_CFunction)cl2->c.f;
						cl1->c.cont = (lua_Continuation)cl2->c.cont;

						for (int i = 0; i < nups2; i++)
							setobj2n(rl, &cl1->c.upvals[i], &cl2->c.upvals[i]);
					}
					else
						luaL_error(rl, "Too many upvalues");
				}

				else if (get_type(cl1) == module_c_closure && get_type(cl2) == module_c_wrap)
				{
					lua_clonecfunction(rl, 1);
					Handler::set(clvalue(index2addr(rl, -1)), Handler::get(cl1));

					Handler::set(cl1, newcclosure_handler);
					Handler::wraps::set(cl1, cl2);
				}

				else if (get_type(cl1) == module_c_closure && get_type(cl2) == l_closure)
				{
					lua_clonecfunction(rl, 1);
					Handler::set(clvalue(index2addr(rl, -1)), Handler::get(cl1));
					lua_ref(rl, 2);

					Handler::set(cl1, newcclosure_handler);
					Handler::wraps::set(cl1, cl2);
				}

				else if (get_type(cl1) == module_c_wrap && get_type(cl2) == roblox_c_closure)
				{
					if (nups1 >= nups2)
					{
						lua_clonecfunction(rl, 1);
						Handler::set(clvalue(index2addr(rl, -1)), Handler::get(cl1));
						Handler::wraps::set(clvalue(index2addr(rl, -1)), Handler::wraps::get(cl1));

						cl1->env = (Table*)cl2->env;
						cl1->c.f = (lua_CFunction)cl2->c.f;
						cl1->c.cont = (lua_Continuation)cl2->c.cont;

						for (int i = 0; i < nups2; i++)
							setobj2n(rl, &cl1->c.upvals[i], &cl2->c.upvals[i]);
					}
					else
						luaL_error(rl, "Too many upvalues");
				}

				else if (get_type(cl1) == module_c_wrap && get_type(cl2) == module_c_closure)
				{
					if (nups1 >= nups2)
					{
						lua_clonecfunction(rl, 1);
						Handler::set(clvalue(index2addr(rl, -1)), Handler::get(cl1));
						Handler::wraps::set(clvalue(index2addr(rl, -1)), Handler::wraps::get(cl1));

						Handler::set(cl1, Handler::get(cl2));

						cl1->env = (Table*)cl2->env;
						cl1->c.cont = (lua_Continuation)cl2->c.cont;

						for (int i = 0; i < nups2; i++)
							setobj2n(rl, &cl1->c.upvals[i], &cl2->c.upvals[i]);
					}
					else
						luaL_error(rl, "Too many upvalues");
				}

				else if (get_type(cl1) == module_c_wrap && get_type(cl2) == l_closure)
				{
					lua_clonecfunction(rl, 1);
					Handler::set(clvalue(index2addr(rl, -1)), Handler::get(cl1));
					Handler::wraps::set(clvalue(index2addr(rl, -1)), Handler::wraps::get(cl1));

					lua_ref(rl, 2);
					Handler::wraps::set(cl1, cl2);
				}

				else if (get_type(cl1) == l_closure && (get_type(cl2) == roblox_c_closure || get_type(cl2) == module_c_closure))
				{
					lua_clonefunction(rl, 1);
					const auto& spoof = Luau::compile(E("local f = function() end; return f(...); "));
					luau_load(rl, cl1->l.p->source->data, spoof.c_str(), spoof.size(), 0);

					Closure* clspoof = clvalue(index2addr(rl, -1));
					setobj(rl, &clspoof->l.p->k[0], index2addr(rl, 2));
					clspoof->l.p->linedefined = (__int64)cl1->l.p->linedefined;
					cl1->l.p = (Proto*)clspoof->l.p;

					lua_pop(rl, 1);
				}

				else if (get_type(cl1) == l_closure && get_type(cl2) == module_c_wrap)
				{
					const Closure* l = Handler::wraps::get(cl2);

					if (nups1 >= l->nupvalues)
					{
						lua_clonefunction(rl, 1);

						cl1->env = (Table*)l->env;
						cl1->l.p = (Proto*)l->l.p;

						for (int i = 0; i < l->nupvalues; i++)
							setobj2n(rl, &cl1->l.uprefs[i], &l->l.uprefs[i]);
					}
					else
						luaL_error(rl, "Too many upvalues");
				}
				else
					luaL_error(rl, "Failed to unhook");

				return 1;
			}


		}


		namespace Input
		{
			int isrbxactive(lua_State* rl)
			{
				LOGD("isrbxactive");

				HWND window;
				window = FindWindowA(NULL, "Roblox");
				lua_pushboolean(rl, GetForegroundWindow() == window);
				return 1;
			}


			int mouse1press(lua_State* rl)
			{
				LOGD("mouse1press");

				HWND window = FindWindowA(NULL, "Roblox");
				if (GetForegroundWindow() == window)
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				return 0;
			}


			int mouse1release(lua_State* rl)
			{
				LOGD("mouse1release");

				HWND window = FindWindowA(NULL, "Roblox");
				if (GetForegroundWindow() == window)
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				return 0;
			}


			int mouse1click(lua_State* rl)
			{
				LOGD("mouse1click");

				HWND window = FindWindowA(NULL, "Roblox");
				if (GetForegroundWindow() == window)
					mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				return 0;
			}


			int mouse2press(lua_State* rl)
			{
				LOGD("mouse2press");

				HWND window = FindWindowA(NULL, "Roblox");
				if (GetForegroundWindow() == window)
					mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
				return 0;
			}


			int mouse2release(lua_State* rl)
			{
				LOGD("mouse2release");

				HWND window = FindWindowA(NULL, "Roblox");
				if (GetForegroundWindow() == window)
					mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
				return 0;
			}


			int mouse2click(lua_State* rl)
			{
				LOGD("mouse2click");

				HWND window = FindWindowA(NULL, "Roblox");
				if (GetForegroundWindow() == window)
					mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
				return 0;
			}


			int keypress(lua_State* rl)
			{
				LOGD("keypress");

				HWND window = FindWindowA(NULL, "Roblox");
				UINT key = luaL_checkinteger(rl, 1);

				if (GetForegroundWindow() == window)
					keybd_event(0, (BYTE)MapVirtualKeyA(key, MAPVK_VK_TO_VSC), KEYEVENTF_SCANCODE, 0);
				return 0;
			}


			int iskeydown(lua_State* rl)
			{
				LOGD("iskeydown");

				luaL_checktype(rl, 1, LUA_TNUMBER);

				UINT key = luaL_checkinteger(rl, 1);

				SHORT state = GetAsyncKeyState(key);
				lua_pushboolean(rl, (bool)((state & 0x8000) != 0));
				return 1;
			}


			int keyrelease(lua_State* rl)
			{
				LOGD("keyrelease");

				HWND window = FindWindowA(NULL, "Roblox");
				UINT key = luaL_checkinteger(rl, 1);

				if (GetForegroundWindow() == window)
					keybd_event(0, (BYTE)MapVirtualKeyA(key, MAPVK_VK_TO_VSC), KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP, 0);
				return 0;
			}


			int mousemoverel(lua_State* rl)
			{
				LOGD("mousemoverel");

				HWND window = FindWindowA(NULL, "Roblox");

				DWORD x = luaL_checkinteger(rl, 1);
				DWORD y = luaL_checkinteger(rl, 2);

				if (GetForegroundWindow() == window)
					mouse_event(MOUSEEVENTF_MOVE, x, y, 0, 0);
				return 0;
			}


			int mousemoveabs(lua_State* rl)
			{
				LOGD("mousemoveabs");

				HWND window = FindWindowA(NULL, "Roblox");

				DWORD x = luaL_checkinteger(rl, 1);
				DWORD y = luaL_checkinteger(rl, 2);

				if (GetForegroundWindow() != window) return 0;

				int width = GetSystemMetrics(SM_CXSCREEN) - 1;
				int height = GetSystemMetrics(SM_CYSCREEN) - 1;

				RECT CRect;
				GetClientRect(GetForegroundWindow(), &CRect);

				POINT Point{ CRect.left, CRect.top };
				ClientToScreen(GetForegroundWindow(), &Point);

				x = (x + (DWORD)Point.x) * (65535 / width);
				y = (y + (DWORD)Point.y) * (65535 / height);

				mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, x, y, 0, 0);

				return 0;
			}


			int mousescroll(lua_State* rl)
			{
				LOGD("mousescroll");

				HWND window = FindWindowA(NULL, "Roblox");
				DWORD scroll_amount = luaL_checkinteger(rl, 1);

				if (GetForegroundWindow() == window)
					mouse_event(MOUSEEVENTF_WHEEL, 0, 0, scroll_amount, 0);
				return 0;
			}
		}

		namespace Filesystem
		{
			
			int readfile(lua_State* L) {
				luaL_checkstring(L, 1);

				auto absolutePath = HelpFunctions::CheckPath(L);

				if (!fs::exists(absolutePath) || !fs::is_regular_file(absolutePath)) {
					luaG_runerror(L, "file doesnt exist");
				}

				std::ifstream file(absolutePath, std::ios::binary | std::ios::ate);
				if (!file.is_open()) {
					luaG_runerror(L, "failed to read file content");
				}

				std::streamsize size = file.tellg();
				file.seekg(0, std::ios::beg);
				std::vector<char> buffer(size);
				if (!file.read(buffer.data(), size)) {
					luaG_runerror(L, "failed to read file");
				}

				lua_pushlstring(L, buffer.data(), size);

				return 1;
			}

			int listfiles(lua_State* L) {
				luaL_checkstring(L, 1);

				auto absolutePath = HelpFunctions::CheckPath(L);

				if (!fs::exists(absolutePath) || !fs::is_directory(absolutePath)) {
					luaG_runerror(L, "folder doesn't exist");
				}

				lua_newtable(L);
				int currentIndex = 1;
				for (const auto& entry : fs::directory_iterator(absolutePath)) {
					lua_pushstring(L, entry.path().string().c_str());
					lua_rawseti(L, -2, currentIndex);
					currentIndex++;
				}

				return 1;
			}

			int writefile(lua_State* L) {
				luaL_checkstring(L, 1);
				luaL_checkstring(L, 2);

				auto absolutePath = HelpFunctions::CheckPath(L);
				size_t contentLength;
				auto fileContent = lua_tolstring(L, 2, &contentLength);

				if (fs::is_directory(absolutePath)) {
					luaG_runerror(L, "argument 1 is not a valid file");
				}

				fs::create_directories(absolutePath.parent_path());

				std::ofstream file(absolutePath, std::ios::binary | std::ios::beg);
				if (!file.is_open()) {
					luaG_runerror(L, "failed to open file");
				}

				file.write(fileContent, contentLength);
				if (file.fail()) {
					luaG_runerror(L, "failed to write content to file");
				}

				file.close();
				if (file.fail()) {
					luaG_runerror(L, "failed to close file");
				}

				return 0;
			}

			int appendfile(lua_State* L) {
				luaL_checkstring(L, 1);
				luaL_checkstring(L, 2);

				auto relativePath = lua_tolstring(L, 1,0);
				auto contentToAppend = lua_tolstring(L, 2,0);
				auto absolutePath = HelpFunctions::CheckPath(L);

				if (!fs::exists(absolutePath) || !fs::is_regular_file(absolutePath)) {
					luaG_runerror(L, "file doesn't exist");
				}

				lua_pushcfunction(L, readfile, "AppendFile");
				lua_pushstring(L, relativePath);
				lua_call(L, 1, 1);
				auto currentContent = _strdup(lua_tolstring(L, -1, NULL));
				lua_pop(L, 1);
				auto newContent = std::format("{}{}", currentContent, contentToAppend).c_str();
				lua_pushcfunction(L, writefile, "AppendFile");
				lua_pushstring(L, relativePath);
				lua_pushstring(L, newContent);
				lua_call(L, 2, 0);

				return 0;
			}

			int makefolder(lua_State* L) {
				luaL_checkstring(L, 1);

				auto absolutePath = HelpFunctions::CheckPath(L);

				if (fs::is_directory(absolutePath)) {
					return 0;
				}

				if (fs::is_regular_file(absolutePath)) {
					luaG_runerror(L, "There is a file on this path!");
				}

				if (fs::exists(absolutePath)) {
					luaG_runerror(L, "There is already something on this path!");
				}

				fs::create_directories(absolutePath);
				return 0;
			}

			int isfile(lua_State* L) {
				luaL_checkstring(L, 1);

				auto absolutePath = HelpFunctions::CheckPath(L);

				lua_pushboolean(L, fs::exists(absolutePath) && fs::is_regular_file(absolutePath));
				return 1;
			}

			int isfolder(lua_State* L) {
				luaL_checkstring(L, 1);

				auto absolutePath = HelpFunctions::CheckPath(L);

				lua_pushboolean(L, fs::exists(absolutePath) && fs::is_directory(absolutePath));
				return 1;
			}

			int delfile(lua_State* L) {
				luaL_checkstring(L, 1);

				auto absolutePath = HelpFunctions::CheckPath(L);

				if (!fs::exists(absolutePath) || !fs::is_regular_file(absolutePath)) {
					luaG_runerror(L, "file doesn't exist!");
				}

				fs::remove(absolutePath);
				return 0;
			}

			int delfolder(lua_State* L) {
				luaL_checkstring(L, 1);

				auto absolutePath = HelpFunctions::CheckPath(L);

				if (!fs::exists(absolutePath) || !fs::is_directory(absolutePath)) {
					luaG_runerror(L, "folder doesn't exist");
				}

				fs::remove_all(absolutePath);
				return 0;
			}


			auto loadfile(lua_State* rl) -> int
			{
				LOGD("loadfile");

				luaL_checktype(rl, 1, LUA_TSTRING);

				luaL_checkstring(rl, 1);

				auto absolutePath = HelpFunctions::CheckPath(rl);

				if (!std::filesystem::is_regular_file(absolutePath)) {
					luaL_error(rl, "invalid path");
					return 0;

				}

				std::ifstream f(absolutePath, std::ios::in | std::ios::binary);
				const auto sz = std::filesystem::file_size(absolutePath);
				std::string result(sz, '\0');
				f.read(result.data(), sz);

				auto bytecode = Utils::compress_bytecode(Luau::compile(result));

				if (Defs::rbx_luavmload((__int64)rl, &bytecode, xorstr_("readfile"), 0) != LUA_OK)
				{
					std::string err = lua_tostring(rl, -1);
					lua_pop(rl, 1);

					lua_pushnil(rl);
					lua_pushlstring(rl, err.c_str(), err.size());
					return 2;
				}
				/*		if (luau_load(rl, chunk.c_str(), bytecode.c_str(), bytecode.size(), 0) != lua_Status::LUA_OK) {
							lua_pushnil(rl);
							lua_pushvalue(rl, -2);
							return 2;
						}*/

				Closure* pClosure = const_cast<Closure*>(static_cast<const Closure*>(lua_topointer(rl, -1)));
				ProtoThings::raise_proto(pClosure->l.p);

				return 1;
			}


		}


		namespace Cache
		{
			auto invalidate(lua_State* rl) -> int
			{
				LOGD("cache.invalidate");

				luaL_checktype(rl, 1, LUA_TUSERDATA);

				uintptr_t instance = *reinterpret_cast<uintptr_t*>(lua_touserdata(rl, 1));

				lua_pushlightuserdata(rl, reinterpret_cast<void*>(Offsets::pushinstance));
				lua_rawget(rl, LUA_REGISTRYINDEX);
				lua_pushlightuserdata(rl, reinterpret_cast<void*>(instance));

				lua_pushnil(rl);
				lua_rawset(rl, -3);
				return 0;
			}


			auto iscached(lua_State* rl) -> int
			{
				LOGD("cache.iscached");

				luaL_checktype(rl, 1, LUA_TUSERDATA);

				uintptr_t instance = *reinterpret_cast<uintptr_t*>(lua_touserdata(rl, 1));

				lua_pushlightuserdata(rl, reinterpret_cast<void*>(Offsets::pushinstance));
				lua_rawget(rl, LUA_REGISTRYINDEX);
				lua_pushlightuserdata(rl, reinterpret_cast<void*>(instance));
				lua_rawget(rl, -2);

				lua_pushboolean(rl, !lua_isnil(rl, -1));
				return 1;
			}


			auto replace(lua_State* rl) -> int
			{
				LOGD("cache.replace");

				luaL_checktype(rl, 1, LUA_TUSERDATA);
				luaL_checktype(rl, 2, LUA_TUSERDATA);

				uintptr_t instance = *reinterpret_cast<uintptr_t*>(lua_touserdata(rl, 1));

				lua_pushlightuserdata(rl, reinterpret_cast<void*>(Offsets::pushinstance));
				lua_rawget(rl, LUA_REGISTRYINDEX);
				lua_pushlightuserdata(rl, reinterpret_cast<void*>(instance));

				lua_pushvalue(rl, 2);
				lua_rawset(rl, -3);
				return 0;
			}


			auto cloneref(lua_State* rl) -> int
			{
				LOGD("cloneref");

				luaL_checktype(rl, 1, LUA_TUSERDATA);

				const auto userdata = lua_touserdata(rl, 1);
				const auto rawUserdata = *static_cast<void**>(userdata);

				lua_pushlightuserdata(rl, (void*)Offsets::pushinstance);

				lua_rawget(rl, LUA_REGISTRYINDEX);

				lua_pushlightuserdata(rl, rawUserdata);
				lua_rawget(rl, -2);

				lua_pushlightuserdata(rl, rawUserdata);
				lua_pushnil(rl);
				lua_rawset(rl, -4);

				Defs::rbx_pushinstance2(rl,
					userdata);
				lua_pushlightuserdata(rl, rawUserdata);
				lua_pushvalue(rl, -3);
				lua_rawset(rl, -5);
				return 1;
			}


			auto compareinstances(lua_State* rl) -> int
			{
				LOGD("compareinstances");

				luaL_checktype(rl, 1, LUA_TUSERDATA);
				luaL_checktype(rl, 2, LUA_TUSERDATA);

				uintptr_t instance_one = *reinterpret_cast<uintptr_t*>(lua_touserdata(rl, 1));
				uintptr_t instance_two = *reinterpret_cast<uintptr_t*>(lua_touserdata(rl, 2));

				lua_pushboolean(rl, instance_one == instance_two);
				return 1;
			}
		}

		namespace Metatable
		{
			int hookmetamethod(lua_State* ls)
			{
				LOGD("hookmetamethod");

				luaL_checkany(ls, 1);
				luaL_checktype(ls, 2, LUA_TSTRING);
				luaL_checktype(ls, 3, LUA_TFUNCTION);

				if (!luaL_getmetafield(ls, 1, lua_tolstring(ls, 2, NULL)))
					luaL_argerror(ls, 2, E("metamethod not found"));

				lua_getglobal(ls, "hookfunction");
				{
					lua_pushvalue(ls, -2);
					lua_pushvalue(ls, 3);
				}
				lua_call(ls, 2, 1);
				return 1;
			}


			auto getrawmetatable(lua_State* rl) -> int
			{
				LOGD("getrawmetatable");

				lua_getmetatable(rl, 1);
				return 1;
			}


			template< std::size_t sz >
			bool check_types(lua_State* state, int index, std::array< lua_Type, sz > arr)
			{
				for (auto i = 0u; i < sz; i++)
				{
					if (lua_type(state, index) == arr[i])
						return true;
				}

				return false;
			}

			auto setrawmetatable(lua_State* rl) -> int
			{
				LOGD("setrawmetatable");

				if (!check_types<2>(rl, 1, { LUA_TTABLE, LUA_TUSERDATA }))
					luaL_argerrorL(rl, 1, "table or userdata expected");

				luaL_checktype(rl, 2, LUA_TTABLE);

				lua_setmetatable(rl, 1);
				return 1;
			}


			auto isreadonly(lua_State* rl) -> int
			{
				LOGD("isreadonly");

				lua_pushboolean(rl, lua_getreadonly(rl, 1));
				return 1;
			}


			auto getnamecallmethod(lua_State* L) -> int
			{
				LOGD("getnamecallmethod");

				const auto szNamecall = lua_namecallatom(L, nullptr);

				if (szNamecall == nullptr)
					lua_pushnil(L);
				else
					lua_pushstring(L, szNamecall);

				return 1;
			}


			auto setnamecallmethod(lua_State* rl) -> int
			{
				LOGD("setnamecallmethod");

				rl->namecall = reinterpret_cast<TString*>(index2addr(rl, 1)->value.p);
				return 0;
			}


			auto setreadonly(lua_State* rl) -> int
			{
				LOGD("setreadonly");

				luaL_checktype(rl, 2, LUA_TBOOLEAN);

				lua_setreadonly(rl, 1, lua_toboolean(rl, 2));
				return 0;
			}


			auto make_writable(lua_State* rl) -> int
			{

				LOGD("make_writable");

				lua_setreadonly(rl, 1, false);
				return 0;
			}


			auto make_readonly(lua_State* rl) -> int
			{
				LOGD("make_readonly");

				lua_setreadonly(rl, 1, true);
				return 0;
			}
		}

		namespace Crypt
		{
			static int base64encode(lua_State* rl)
			{
				LOGD("base64encode");

				luaL_checktype(rl, 1, LUA_TSTRING);

				const auto& encoded = ::crypt::base64::encode(lua_tostring(rl, 1));

				lua_pushlstring(rl, encoded.data(), encoded.length());
				return 1;
			}


			static int base64decode(lua_State* rl)
			{
				LOGD("base64decode");

				luaL_checktype(rl, 1, LUA_TSTRING);

				const auto& decoded = ::crypt::base64::decode(lua_tostring(rl, 1));

				lua_pushlstring(rl, decoded.data(), decoded.length());
				return 1;
			}


			static int encrypt(lua_State* rl)
			{
				LOGD("crypt.encrypt");

				luaL_checktype(rl, 1, LUA_TSTRING);
				luaL_checktype(rl, 2, LUA_TSTRING);
				luaL_checktype(rl, 4, LUA_TSTRING);

				const auto& [encrypted, iv] = ::crypt::encrypt(lua_tostring(rl, 1), lua_tostring(rl, 2), lua_isnil(rl, 3) ? "" : lua_tostring(rl, 3), lua_tostring(rl, 4));

				lua_pushlstring(rl, encrypted.data(), encrypted.length());
				lua_pushlstring(rl, iv.data(), iv.length());
				return 2;
			}


			static int decrypt(lua_State* rl)
			{
				LOGD("crypt.decrypt");

				luaL_checktype(rl, 1, LUA_TSTRING);
				luaL_checktype(rl, 2, LUA_TSTRING);
				luaL_checktype(rl, 3, LUA_TSTRING);
				luaL_checktype(rl, 4, LUA_TSTRING);

				const auto& decrypted = ::crypt::decrypt(lua_tostring(rl, 1), lua_tostring(rl, 2), lua_tostring(rl, 3), lua_tostring(rl, 4));

				lua_pushlstring(rl, decrypted.data(), decrypted.length());
				return 1;
			}


			static int generatebytes(lua_State* rl)
			{
				LOGD("crypt.generatebytes");

				luaL_checktype(rl, 1, LUA_TNUMBER);

				const auto& bytes = ::crypt::generatebytes(lua_tointeger(rl, 1));

				lua_pushlstring(rl, bytes.data(), bytes.length());
				return 1;
			}


			static int generatekey(lua_State* rl)
			{
				LOGD("crypt.generatekey");

				const auto& key = ::crypt::generatebytes(32);

				lua_pushlstring(rl, key.data(), key.length());
				return 1;
			}


			static int hash(lua_State* rl)
			{
				LOGD("crypt.hash");

				luaL_checktype(rl, 1, LUA_TSTRING);
				luaL_checktype(rl, 2, LUA_TSTRING);

				const auto& hash = ::crypt::hash(lua_tostring(rl, 1), lua_tostring(rl, 2));

				lua_pushlstring(rl, hash.data(), hash.length());
				return 1;
			}
		}

		namespace Debug
		{
			auto debug_header_getclosure(lua_State* ls, bool allowCclosure = false, bool popcl = true) -> Closure*
			{
				luaL_checkany(ls, 1);

				if (!(lua_isfunction(ls, 1) || lua_isnumber(ls, 1)))
				{
					luaL_argerror(ls, 1, E("function or number"));
				}

				int level = 0;
				if (lua_isnumber(ls, 1))
				{
					level = lua_tointeger(ls, 1);

					if (level <= 0)
					{
						luaL_argerror(ls, 1, E("invalid stack level."));
					}
				}
				else if (lua_isfunction(ls, 1))
				{
					level = -lua_gettop(ls);
				}

				lua_Debug ar;
				if (!lua_getinfo(ls, level, "f", &ar))
				{
					luaL_argerror(ls, 1, E("invalid stack level."));
				}

				if (!lua_isfunction(ls, -1))
				{
					luaL_argerror(ls, 1, E("stack does not point to a function."));
				}

				if (!allowCclosure && lua_iscfunction(ls, -1))
				{
					luaL_argerror(ls, 1, E("expected Lua Closure!"));
				}

				auto closure = clvalue(luaA_toobject(ls, -1));
				if (popcl) lua_pop(ls, 1);

				return closure;
			}

			static int debug_getupvalues(lua_State* ls)
			{
				LOGD("debug.getupvalues");

				const auto closure = debug_header_getclosure(ls, true, false);

				// Create a table with space for the number of upvalues ( if any )
				lua_createtable(ls, closure->nupvalues, 0);
				for (int i = 0; i < closure->nupvalues; i++)
				{
					if (lua_getupvalue(ls, -2, ++i)) lua_rawseti(ls, -2, i);
				}

				return 1;
			}

			static int debug_getupvalue(lua_State* ls)
			{
				LOGD("debug.getupvalue");

				const auto closure = debug_header_getclosure(ls, true, false);
				const auto idx = luaL_checkinteger(ls, 2);

				if (!(closure->nupvalues > 0))
				{
					luaL_argerror(ls, 1, E("function does not have upvalues."));
				}

				if (!(idx >= 1 && idx <= closure->nupvalues))
				{
					luaL_argerror(ls, 2, E("index out of range [1, nupvalues]."));
				}

				if (!lua_getupvalue(ls, -1, idx)) lua_pushnil(ls);
				return 1;
			}

			static int debug_setupvalue(lua_State* ls)
			{
				LOGD("debug.setupvalue");

				// dont allow this in C Functions
				const auto closure = debug_header_getclosure(ls, false, false);
				const auto idx = luaL_checkinteger(ls, 2);

				if (!(closure->nupvalues > 0))
				{
					luaL_argerror(ls, 1, E("function does not have upvalues."));
				}

				if (!(idx >= 1 && idx <= closure->nupvalues))
				{
					luaL_argerror(ls, 2, E("index out of range [1, nupvalues]."));
				}

				lua_pushvalue(ls, 3);
				lua_setupvalue(ls, -2, idx);
				return 1;
			}

			static int debug_getconstants(lua_State* ls)
			{
				LOGD("debug.getconstants");

				Closure* closure = debug_header_getclosure(ls);
				Proto* p = (Proto*)closure->l.p;

				// Create a table with space for the number of constants ( if any )
				lua_createtable(ls, (int)p->sizek, 0);
				for (int i = 0; i < (int)p->sizek; i++)
				{
					TValue k = p->k[i];

					if (k.tt == LUA_TFUNCTION || k.tt == LUA_TTABLE)
					{
						lua_pushnil(ls);
					}
					else
					{
						luaC_threadbarrier(ls) luaA_pushobject(ls, &k);
					}

					lua_rawseti(ls, -2, i + 1);
				}

				return 1;
			}

			 int debug_getconstant(lua_State* ls)
			{
				LOGD("debug.getconstant");

				const auto closure = debug_header_getclosure(ls);
				const auto idx = luaL_checkinteger(ls, 2);
				Proto* p = (Proto*)closure->l.p;

				if ((int)p->sizek == 0)
				{
					luaL_argerror(ls, 1, E("function does not have constants."));
				}

				if (!(idx >= 1 && idx <= p->sizek))
				{
					luaL_argerror(ls, 2, E("index out of range [1, sizek]."));
				}

				TValue k = (TValue)p->k[idx - 1];

				if (k.tt == LUA_TFUNCTION || k.tt == LUA_TTABLE)
				{
					lua_pushnil(ls);
				}
				else
				{
					luaC_threadbarrier(ls) luaA_pushobject(ls, &k);
				}

				return 1;
			}

			static int debug_setconstant(lua_State* ls)
			{
				LOGD("debug.setconstant");

				luaL_checkany(ls, 3);

				const auto closure = debug_header_getclosure(ls);
				int idx = luaL_checkinteger(ls, 2);
				Proto* p = (Proto*)closure->l.p;

				if ((int)p->sizek == 0)
				{
					luaL_argerror(ls, 1, E("function does not have constants."));
				}

				if (!(idx >= 1 && idx <= (int)p->sizek))
				{
					luaL_argerror(ls, 2, E("index out of range [1, sizek]."));
				}

				TValue* k = (TValue*) & p->k[idx - 1];

				if (k->tt == LUA_TFUNCTION || k->tt == LUA_TTABLE)
				{
					return 0;
				}
				else
				{
					if (k->tt == luaA_toobject(ls, 3)->tt)
					{
						setobj2s(ls, k, luaA_toobject(ls, 3));
					}
				}

				return 0;
			}

			// Clones proto and returns the clone.
			// Only important information is cloned.
			auto debug_helper_clone_proto(lua_State* ls, Proto* proto) -> Proto*
			{
				// Make a new proto.
				// We gotta make new GCObjects for the proto.
				Proto* clone = luaF_newproto(ls);

				// Copy constants
				clone->sizek = proto->sizek;
				clone->k = luaM_newarray(ls, proto->sizek, TValue, proto->memcat);
				for (int j = 0; j < proto->sizek; ++j)
				{
					clone->k[j].tt = proto->k[j].tt;
					clone->k[j].value = proto->k[j].value;
				}

				clone->lineinfo = 0;
				clone->locvars = 0;
				clone->nups = 0;
				clone->sizeupvalues = 0;
				clone->sizelineinfo = 0;
				clone->linegaplog2 = 0;
				clone->sizelocvars = 0;
				clone->linedefined = 0;

				// Copy debugname and source, make new TStrings
				if (proto->debugname)
				{
					const auto debugname = getstr(proto->debugname);
					const auto sz = strlen(debugname);

					clone->debugname = luaS_newlstr(ls, debugname, sz);
				}

				if (proto->source)
				{
					const auto source = getstr(proto->source);
					const auto sz = strlen(source);

					clone->source = luaS_newlstr(ls, source, sz);
				}

				clone->numparams = proto->numparams;
				clone->is_vararg = proto->is_vararg;
				clone->maxstacksize = proto->maxstacksize;
				clone->bytecodeid = proto->bytecodeid;

				// Set the code to return nothing.
				clone->sizecode = 1;
				clone->code = luaM_newarray(ls, clone->sizecode, Instruction, proto->memcat);
				clone->code[0] = 0x10082; // RETURN [ insn, enc 227 ]
				clone->codeentry = clone->code;
				clone->debuginsn = 0;

				// Copy protos
				clone->sizep = proto->sizep;
				clone->p = luaM_newarray(ls, proto->sizep, Proto*, proto->memcat);
				for (int j = 0; j < proto->sizep; ++j)
				{
					clone->p[j] = debug_helper_clone_proto(ls, proto->p[j]);
				}

				return clone;
			}

			static int debug_getprotos(lua_State* ls)
			{
				LOGD("debug.getprotos");

				const auto closure = debug_header_getclosure(ls);
				const auto p = debug_helper_clone_proto(ls, closure->l.p); // Proto Clone

				// Create a table with space for the number of protos ( if any )
				lua_createtable(ls, p->sizep, 0);
				for (int i = 0; i < p->sizep; i++)
				{
					// Make a LClosure for them
					// We want to avoid vulns, hence the clones with empty code
					Proto* proto = p->p[i];
					Closure* pcl = luaF_newLclosure(ls, closure->nupvalues, closure->env, proto);

					luaC_threadbarrier(ls) setclvalue(ls, ls->top, pcl) ls->top++;

					lua_rawseti(ls, -2, i + 1);
				}

				return 1;
			}

			int debug_getproto(lua_State* L) {
				LOGD("debug.getproto");
				Closure* cl;
				if (lua_isnumber(L, 1)) {
					lua_Debug ar;
					if (!lua_getinfo(L, luaL_checkinteger(L, 1), "f", &ar))
						luaL_argerror(L, 1, "level out of range");
					if (lua_iscfunction(L, -1))
						luaL_argerror(L, 1, "level points to cclosure");
					cl = (Closure*)lua_topointer(L, -1);
				}
				else if (lua_isfunction(L, 1)) {
					luaL_checktype(L, 1, LUA_TFUNCTION);
					cl = (Closure*)lua_topointer(L, 1);
					if (cl->isC)
						luaL_argerror(L, 1, "lclosure expected");
				}
				else {
					luaL_argerror(L, 1, "function or number expected");
				}
				int index = luaL_checkinteger(L, 2);
				bool active = false;
				if (!lua_isnoneornil(L, 3))
					active = luaL_checkboolean(L, 3);
				if (!active) {
					if (index < 1 || index > cl->l.p->sizep)
						luaL_argerror(L, 2, "index out of range");
					Proto* p = cl->l.p->p[index - 1];
					std::unique_ptr<TValue> function(new TValue{});
					setclvalue(L, function.get(), luaF_newLclosure(L, 0, cl->env, p));
					luaA_pushobject(L, function.get());
				}
				else {
					lua_newtable(L);

					struct Ctx {
						lua_State* L;
						int count;
						Closure* cl;
					} ctx{ L, 0, cl };

					luaM_visitgco(L, &ctx, [](void* pctx, lua_Page* page, GCObject* gco) -> bool {
						Ctx* ctx = static_cast<Ctx*>(pctx);
						if (!((gco->gch.marked ^ WHITEBITS) & otherwhite(ctx->L->global)))
							return false;

						uint8_t tt = gco->gch.tt;
						if (tt == LUA_TFUNCTION) {
							Closure* cl = (Closure*)gco;
							if (!cl->isC && cl->l.p == ctx->cl->l.p->p[ctx->count]) {
								setclvalue(ctx->L, ctx->L->top, cl);
								ctx->L->top++;
								lua_rawseti(ctx->L, -2, ++ctx->count);
							}
						}
						return false;
						});
				}
				return 1;
			}

			static int debug_getstack(lua_State* ls)
			{
				LOGD("debug.getstack");

				luaL_checkany(ls, 1);

				if (!(lua_isfunction(ls, 1) || lua_isnumber(ls, 1)))
				{
					luaL_argerror(ls, 1, E("function or number"));
				}

				int level = 0;
				if (lua_isnumber(ls, 1))
				{
					level = lua_tointeger(ls, 1);

					if (level <= 0)
					{
						luaL_argerror(ls, 1, E("invalid stack level."));
					}
				}
				else if (lua_isfunction(ls, 1))
				{
					level = -lua_gettop(ls);
				}

				lua_Debug ar;
				if (!lua_getinfo(ls, level, "f", &ar))
				{
					luaL_argerror(ls, 1, E("invalid stack level."));
				}

				if (!lua_isfunction(ls, -1))
				{
					luaL_argerror(ls, 1, E("stack does not point to a function."));
				}

				if (lua_iscfunction(ls, -1))
				{
					luaL_argerror(ls, 1, E("expected Lua Closure"));
				}

				lua_pop(ls, 1); // pop the closure since we dont need it

				auto ci = ls->ci[-level];

				if (lua_isnumber(ls, 2))
				{
					const auto idx = lua_tointeger(ls, 2) - 1;

					if (idx >= cast_int(ci.top - ci.base) || idx < 0)
					{
						luaL_argerror(ls, 2, E("Invalid stack index."));
					}

					auto val = ci.base + idx;
					luaC_threadbarrier(ls) luaA_pushobject(ls, val);
				}
				else
				{
					int idx = 0;
					lua_newtable(ls);

					for (auto val = ci.base; val < ci.top; val++)
					{
						lua_pushinteger(ls, idx++ + 1);

						luaC_threadbarrier(ls) luaA_pushobject(ls, val);

						lua_settable(ls, -3);
					}
				}

				return 1;
			}

			static int debug_setstack(lua_State* ls)
			{
				LOGD("debug.setstack");

				luaL_checkany(ls, 1);

				if (!(lua_isfunction(ls, 1) || lua_isnumber(ls, 1)))
				{
					luaL_argerror(ls, 1, E("function or number"));
				}

				int level = 0;
				if (lua_isnumber(ls, 1))
				{
					level = lua_tointeger(ls, 1);

					if (level <= 0)
					{
						luaL_argerror(ls, 1, E("invalid stack level."));
					}
				}
				else if (lua_isfunction(ls, 1))
				{
					level = -lua_gettop(ls);
				}

				lua_Debug ar;
				if (!lua_getinfo(ls, level, "f", &ar))
				{
					luaL_argerror(ls, 1, E("invalid stack level."));
				}

				if (!lua_isfunction(ls, -1))
				{
					luaL_argerror(ls, 1, E("stack does not point to a function."));
				}

				if (lua_iscfunction(ls, -1))
				{
					luaL_argerror(ls, 1, E("expected Lua Closure"));
				}

				lua_pop(ls, 1); // pop the closure since we dont need it

				luaL_checkany(ls, 3);

				auto ci = ls->ci[-level];

				const auto idx = luaL_checkinteger(ls, 2) - 1;
				if (idx >= cast_int(ci.top - ci.base) || idx < 0)
				{
					luaL_argerror(ls, 2, E("Invalid stack index."));
				}

				if ((ci.base + idx)->tt != luaA_toobject(ls, 3)->tt)
				{
					luaL_argerror(ls, 3, E("Source type does not match the Target type."));
				}

				setobj2s(ls, (ci.base + idx), luaA_toobject(ls, 3))
					return 0;
			}

			static int debug_getinfo(lua_State* ls)
			{
				LOGD("debug.getinfo");

				luaL_checkany(ls, 1);

				if (!(lua_isfunction(ls, 1) || lua_isnumber(ls, 1)))
				{
					luaL_argerror(ls, 1, E("function or number"));
				}

				int level;
				if (lua_isnumber(ls, 1))
				{
					level = lua_tointeger(ls, 1);
				}
				else
				{
					level = -lua_gettop(ls);
				}

				auto desc = lua_isstring(ls, 2) ? lua_tolstring(ls, 2, NULL) : "sluanf";

				lua_Debug ar;
				if (!lua_getinfo(ls, level, desc, &ar))
				{
					luaL_argerror(ls, 1, E("invalid stack level."));
				}

				if (!lua_isfunction(ls, -1))
				{
					luaL_argerror(ls, 1, E("stack does not point to a function."));
				}

				lua_newtable(ls);
				{
					if (std::strchr(desc, 's'))
					{
						lua_pushstring(ls, ar.source);
						lua_setfield(ls, -2, "source");

						lua_pushstring(ls, ar.short_src);
						lua_setfield(ls, -2, "short_src");

						lua_pushstring(ls, ar.what);
						lua_setfield(ls, -2, "what");

						lua_pushinteger(ls, ar.linedefined);
						lua_setfield(ls, -2, "linedefined");
					}

					if (std::strchr(desc, 'l'))
					{
						lua_pushinteger(ls, ar.currentline);
						lua_setfield(ls, -2, "currentline");
					}

					if (std::strchr(desc, 'u'))
					{
						lua_pushinteger(ls, ar.nupvals);
						lua_setfield(ls, -2, "nups");
					}

					if (std::strchr(desc, 'a'))
					{
						lua_pushinteger(ls, ar.isvararg);
						lua_setfield(ls, -2, "is_vararg");

						lua_pushinteger(ls, ar.nparams);
						lua_setfield(ls, -2, "numparams");
					}

					if (std::strchr(desc, 'n'))
					{
						lua_pushstring(ls, ar.name);
						lua_setfield(ls, -2, "name");
					}

					if (std::strchr(desc, 'f'))
					{
						lua_pushvalue(ls, -2);
						lua_remove(ls, -3);
						lua_setfield(ls, -2, "func");
					}
				}

				return 1;
			}

			static int debug_isvalidlevel(lua_State* ls)
			{
				LOGD("debug.isvalidlevel");

				int level = 0;
				if (lua_isnumber(ls, 1))
				{
					level = lua_tointeger(ls, 1);

					if (level <= 0)
					{
						luaL_argerror(ls, 1, E("invalid stack level."));
					}
				}
				else if (lua_isfunction(ls, 1))
				{
					level = -lua_gettop(ls);
				}

				lua_Debug ar;
				if (!lua_getinfo(ls, level, "f", &ar))
				{
					luaL_argerror(ls, 1, E("invalid stack level."));
				}

				lua_pushboolean(ls, lua_isfunction(ls, -1));
				return 1;
			}

			static int getcurrentline(lua_State* ls)
			{
				LOGD("debug.getcurrentline");

				int level{ 1 };
				if (lua_isnumber(ls, 1))
				{
					level = lua_tointeger(ls, 1);

					if (level <= 0)
					{
						luaL_argerror(ls, 1, E("invalid stack level."));
					}
				}

				lua_Debug ar;
				if (!lua_getinfo(ls, level, "l", &ar))
				{
					luaL_argerror(ls, 1, E("invalid stack level."));
				}

				lua_pushinteger(ls, ar.currentline);
				return 1;
			}

			static int debug_getcallstack(lua_State* ls)
			{
				LOGD("debug.getcallstack");

				lua_State* thread = lua_isthread(ls, 1) ? lua_tothread(ls, 1) : ls;

				int idx = 1;
				lua_newtable(ls);
				{
					while (idx < lua_stackdepth(thread))
					{
						lua_Debug ar;
						lua_getinfo(thread, idx, "f", &ar);
						/*
						lua_newtable(ls);
						{
							lua_pushstring(ls, ar.source);
							lua_setfield(ls, -2, "source");

							lua_pushstring(ls, ar.short_src);
							lua_setfield(ls, -2, "short_src");

							lua_pushstring(ls, ar.what);
							lua_setfield(ls, -2, "what");

							lua_pushinteger(ls, ar.linedefined);
							lua_setfield(ls, -2, "linedefined");

							if ( strcmp(ar.what, "Lua") == 0 )
							{
								lua_pushinteger(ls, ar.currentline);
								lua_setfield(ls, -2, "currentline");
							}

							lua_pushinteger(ls, ar.nupvals);
							lua_setfield(ls, -2, "nups");

							lua_pushinteger(ls, ar.isvararg);
							lua_setfield(ls, -2, "is_vararg");

							lua_pushinteger(ls, ar.nparams);
							lua_setfield(ls, -2, "numparams");

							lua_pushstring(ls, ar.name);
							lua_setfield(ls, -2, "name");

							lua_pushvalue(thread, -2);
							lua_remove(thread, -3);
							lua_xmove(thread, ls, 1);
							lua_setfield(ls, -2, "func");
						}
						*/
						lua_xmove(thread, ls, 1);
						lua_rawseti(ls, -2, idx++);
					}
				}

				return 1;
			}

		}



		namespace WebSocket {
			

			std::unordered_map<void*, std::shared_ptr<atlantis::lua_objects::lua_websocket>> socket_map{ };

			static int websocket_send(lua_State* ls)
			{
				LOGD("WebSocket.send");

				luaL_checktype(ls, 1, LUA_TUSERDATA);
				luaL_checktype(ls, 2, LUA_TSTRING);

				void* userdata = lua_touserdatatagged(ls, lua_upvalueindex(1), 69);
				if (socket_map.find(userdata) == socket_map.end())
				{
					luaL_error(ls, "%s", E("Could not find socket"));
				}

				auto socket = socket_map[userdata];

				if (socket->is_connected())
				{
					socket->send_msg(lua_tostring(ls, 2));
				}
				else
				{
					socket_map.erase(userdata);
				}

				return 0;
			}

			static int websocket_close(lua_State* ls)
			{
				LOGD("WebSocket.Close");

				luaL_checktype(ls, 1, LUA_TUSERDATA);

				void* userdata = lua_touserdatatagged(ls, lua_upvalueindex(1), 69);
				if (socket_map.find(userdata) == socket_map.end())
				{
					luaL_error(ls, "%s", E("Could not find socket"));
				}

				auto socket = socket_map[userdata];

				socket->stop();

				socket_map.erase(userdata);

				return 0;
			}

			static int websocket_index(lua_State* ls)
			{
				void* userdata = lua_touserdatatagged(ls, 1, 69);
				const char* key = lua_tolstring(ls, 2, NULL);

				if (socket_map.find(userdata) == socket_map.end())
				{
					luaL_error(ls, "Could not find socket");
				}

				auto socket = socket_map[userdata];

				if (strcmp(key, "Url") == 0)
				{
					lua_pushstring(ls, socket->get_url().c_str());
					return 1;
				}
				else if (strcmp(key, "Send") == 0)
				{
					lua_pushvalue(ls, 1);
					lua_pushcclosure(ls, websocket_send, 0, 1);

					return 1;
				}
				else if (strcmp(key, "Close") == 0)
				{
					lua_pushvalue(ls, 1);
					lua_pushcclosure(ls, websocket_close, 0, 1);

					return 1;
				}
				else if (strcmp(key, "OnMessage") == 0)
				{
					lua_getref(ls, socket->onmessage_refid);
					lua_getfield(ls, -1, "Event");

					return 1;
				}
				else if (strcmp(key, "OnClose") == 0)
				{
					lua_getref(ls, socket->onclose_refid);
					lua_getfield(ls, -1, "Event");

					return 1;
				}
				else if (strcmp(key, "OnError") == 0)
				{
					lua_getref(ls, socket->onerr_ref);
					lua_getfield(ls, -1, "Event");

					return 1;
				}

				return 0;
			}

			static int websocket_connect(lua_State* ls)
			{
				LOGD("Websocket.connect");

				std::string url = luaL_checkstring(ls, 1);
				if (url.find("ws://") == std::string::npos && url.find("wss://") == std::string::npos)
				{
					luaL_error(ls, "%s", E("Invalid protocol ( Expected 'ws://' or 'wss://' )"));
				}

				LOGD("creating bindablevent 1");

				lua_getglobal(ls, E("Instance"));
				lua_getfield(ls, -1, E("new"));
				{
					lua_pushstring(ls, E("BindableEvent"));
				}
				if (lua_pcall(ls, 1, 1, 0))
				{
					auto error_str = lua_tostring(ls, -1);

					LOGD("Error creating an instance! %s", error_str);
					//luaL_error(ls, "%s", error_str);
				}

				const auto ud = reinterpret_cast<std::int64_t>(lua_touserdata(ls, -1));

				lua_remove(ls, -2);

				const auto onmessage_refid = lua_ref(ls, -1);

				LOGD("creating bindablevent 2");

				lua_getglobal(ls, E("Instance"));
				lua_getfield(ls, -1, E("new"));
				{
					lua_pushstring(ls, E("BindableEvent"));
				}
				if (lua_pcall(ls, 1, 1, 0))
				{
					auto error_str = lua_tostring(ls, -1);

					LOGD("Error creating an instance! %s", error_str);
					//luaL_error(ls, "%s", error_str);
				}

				const auto udddd = reinterpret_cast<std::int64_t>(lua_touserdata(ls, -1));

				lua_remove(ls, -2);
				const auto onclose_refid = lua_ref(ls, -1);

				LOGD("creating bindablevent 3");

				lua_getglobal(ls, E("Instance"));
				lua_getfield(ls, -1, E("new"));
				{
					lua_pushstring(ls, E("BindableEvent"));
				}
				if (lua_pcall(ls, 1, 1, 0))
				{
					auto error_str = lua_tostring(ls, -1);

					LOGD("Error creating an instance! %s", error_str);
					//luaL_error(ls, "%s", error_str);
				}

				const auto udd = reinterpret_cast<std::int64_t>(lua_touserdata(ls, -1));

				lua_remove(ls, -2);
				const auto onerr_ref = lua_ref(ls, -1);

				const auto socket = std::make_shared< atlantis::lua_objects::lua_websocket >();
				if (!socket->setupSocket(url))
				{
					luaL_error(ls, E("Failed to connect websocket to URL ('%s')!"), url.c_str());
					return 0;
				}

				socket->L = lua_newthread(ls); // new Lua thread for this socket
				socket->L_refid = lua_ref(ls, -1); // mkake a registry reference id for the thread to avoid garbage collection
				lua_pop(ls, 1); // pop the new Lua Thread

				// event refds
				socket->onmessage_refid = onmessage_refid;
				socket->onclose_refid = onclose_refid;
				socket->onerr_ref = onerr_ref;

				void* userdata = lua_newuserdatatagged(ls, sizeof(atlantis::lua_objects::lua_websocket), 69);
				lua_newtable(ls);
				{

					lua_pushstring(ls, E("WebSocket"));
					lua_setfield(ls, -2, E("__type"));

					lua_pushcclosure(ls, websocket_index, nullptr, 0);
					lua_setfield(ls, -2, E("__index"));
				}
				lua_setmetatable(ls, -2);

				socket_map.insert({ userdata, socket });
				return 1;

				//return atlantis::yielding::begin_yield(ls, [onmessage_refid, onerr_ref, onclose_refid, url]() -> atlantis::yielding::yielded_t
				//	{
				//		const auto socket = std::make_shared< atlantis::lua_objects::lua_websocket >();
				//		if (!socket->setupSocket(url))
				//		{
				//			return ([url](lua_State* ls) -> int
				//				{
				//					atlantis::yielding::resume_error(ls, E("Failed to connect websocket to URL ('%s')!"), url.c_str());
				//					return 0;
				//				});
				//			//return 0;
				//		}

				//		return ([onclose_refid, onerr_ref, onmessage_refid, socket](lua_State* ls)
				//			{
				//				socket->L = lua_newthread(ls); // new Lua thread for this socket
				//				socket->L_refid = lua_ref(ls, -1); // mkake a registry reference id for the thread to avoid garbage collection
				//				lua_pop(ls, 1); // pop the new Lua Thread

				//				// event refds
				//				socket->onmessage_refid = onmessage_refid;
				//				socket->onclose_refid = onclose_refid;
				//				socket->onerr_ref = onerr_ref;

				//				void* userdata = lua_newuserdatatagged(ls, sizeof(atlantis::lua_objects::lua_websocket), 69);
				//				lua_newtable(ls);
				//				{
			
				//					lua_pushstring(ls, E("WebSocket"));
				//					lua_setfield(ls, -2, E("__type"));

				//					lua_pushcclosure(ls, websocket_index, nullptr, 0);
				//					lua_setfield(ls, -2, E("__index"));
				//				}
				//				lua_setmetatable(ls, -2); 

				//				socket_map.insert({ userdata, socket });
				//				return 1;
				//			});
				//	});
			}


		}

		namespace Scripts
		{
			auto queue_on_teleport(lua_State* rl) -> int
			{
				LOGD("queue_on_teleport");

				luaL_checktype(rl, 1, LUA_TSTRING);

				std::string script = lua_tostring(rl, 1);
				teleport_queue.push_back(script);
				return 0;
			}

			int decompile(lua_State* L) {
				LOGD("decompile");

				luaL_checktype(L, 1, LUA_TUSERDATA);

				lua_getglobal(L, xorstr_("typeof"));
				lua_pushvalue(L, 1); 
				lua_call(L, 1, 1);
				const bool isInstance = (strcmp(lua_tolstring(L, -1, 0), xorstr_("Instance")) == 0);
				lua_pop(L, 1);

				if (!isInstance)
					luaL_argerror(L, 1, xorstr_("Expected Instance"));


				uintptr_t script = *reinterpret_cast<uintptr_t*>(lua_touserdata(L, 1));
				const char* classname = *reinterpret_cast<const char**>(*reinterpret_cast<uintptr_t*>(script + 0x18) + 0x8);

				int bytecodeSize{ 0 };

				if (strcmp(classname, "LocalScript") == 0)
					bytecodeSize = *reinterpret_cast<int*>(*reinterpret_cast<uintptr_t*>(script + Offsets::Bytecode::localscript) + 0x20);
				else if (strcmp(classname, "ModuleScript") == 0)
					bytecodeSize = *reinterpret_cast<int*>(*reinterpret_cast<uintptr_t*>(script + Offsets::Bytecode::modulescript) + 0x20);
				else {

					luaL_error(L, "Local/Module scripts expected.");
					return 0;
				}


				std::uintptr_t bytecodePtr{};

				if (strcmp(classname, "LocalScript") == 0)
					bytecodePtr = *reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<uintptr_t*>(script + Offsets::Bytecode::localscript) + 0x10);
				else if (strcmp(classname, "ModuleScript") == 0)
					bytecodePtr = *reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<uintptr_t*>(script + Offsets::Bytecode::modulescript) + 0x10);
				else {
					luaL_error(L, "failed to obtain bytecode ptr");
					return 0;
				}

				std::string bytecodeBuffer;
				bytecodeBuffer.resize(bytecodeSize);

				unsigned char* bytecodeData = (unsigned char*)(bytecodePtr);

				std::memcpy(bytecodeBuffer.data(), bytecodeData, bytecodeSize);

				const std::string bytecode = Utils::decompress_bytecode(bytecodeBuffer);

				std::string encoded = ::crypt::base64::encode(bytecode);

				return atlantis::yielding::begin_yield(L, [encoded]() -> atlantis::yielding::yielded_t
					{
						cpr::Response response = cpr::Post(
							cpr::Url{ xorstr_("http://getatlantis.xyz/lxzp/decompile") },
							cpr::Body{ nlohmann::json{{"bytecode", encoded.data()}}.dump() },
							cpr::Header{ {"Content-Type", "application/json"} }
						);

						return ([response](lua_State* L) {

							if (response.status_code == 200) {
								lua_pushstring(L, response.text.c_str());
							}
							else {
								lua_pushstring(L, "-- failed to decompile script");
							}
							return 1;
							});
					});
			}



			auto setcontext(lua_State* rl) -> int
			{
				LOGD("setidentity");

				luaL_checktype(rl, 1, LUA_TNUMBER);

				int level = lua_tointeger(rl, 1);

				if (level > 8) {
					luaL_error(rl," You may not set your identity above 8!!");
					return 0;
				}

				auto userdata = reinterpret_cast<std::int64_t>(rl->userdata);
				*reinterpret_cast<std::int64_t*>(userdata + 48) = level;

	
				std::int64_t arr[128];
				Defs::rbx_impersonator(arr, &level, *reinterpret_cast<std::int64_t*>(userdata + 72));

				return 0;
			}


			auto getcontext(lua_State* rl) -> int
			{
				LOGD("getindetity");

				auto state = (__int64)rl;
				uintptr_t userdata = *reinterpret_cast<uintptr_t*>(state + 120);

				lua_pushinteger(rl, *reinterpret_cast<uintptr_t*>(userdata + 0x30));
				return 1;
			}



			int getcallingscript(lua_State* L) {
				LOGD("getcallingscript");

				try {
					lua_getglobal(L, "script");
				}
				catch (...) {
					lua_pushnil(L);
				}

				return 1;
			}
			auto getloadedmodules(lua_State* L) -> int
			{
				LOGD("getloadedmodules");

				lua_newtable(L);

				typedef struct {
					lua_State* pLua;
					int itemsFound;
					std::map< uintptr_t, bool > map;
				} GCOContext;

				auto gcCtx = GCOContext{ L, 0 };

				const auto ullOldThreshold = L->global->GCthreshold;
				L->global->GCthreshold = SIZE_MAX;

				// Visit garbage-collected objects
				luaM_visitgco(L, &gcCtx, [](void* ctx, lua_Page* pPage, GCObject* pGcObj) -> bool {
					const auto pCtx = static_cast<GCOContext*>(ctx);
					const auto ctxL = pCtx->pLua;

					if (isdead(ctxL->global, pGcObj))
						return false; 

					if (const auto gcObjType = pGcObj->gch.tt;
						gcObjType == LUA_TFUNCTION) {
						// Push the function onto the stack
						ctxL->top->value.gc = pGcObj;
						ctxL->top->tt = gcObjType;
						ctxL->top++;

						lua_getfenv(ctxL, -1);

						if (!lua_isnil(ctxL, -1)) {
							lua_getfield(ctxL, -1, "script"); 

							if (!lua_isnil(ctxL, -1)) {
								uintptr_t script_addr = *(uintptr_t*)lua_touserdata(ctxL, -1);

								std::string class_name = **(std::string**)(*(uintptr_t*)(script_addr + 0x18) + 0x8);

								if (pCtx->map.find(script_addr) == pCtx->map.end() && class_name == "ModuleScript") {
									pCtx->map.insert({ script_addr, true });
									lua_rawseti(ctxL, -4, ++pCtx->itemsFound);
								}
								else {
									lua_pop(ctxL, 1);
								}
							}
							else {
								lua_pop(ctxL, 1); 
							}
						}

						lua_pop(ctxL, 2);
					}
					return false;
					});

				L->global->GCthreshold = ullOldThreshold;

				return 1;
			}



			int gethui(lua_State* L) 
			{
				LOGD("gethui");

				lua_getglobal(L, E("cloneref"));
				lua_getglobal(L, E("game"));
				lua_call(L, 1, 1);
				lua_getfield(L, -1, E("GetService"));
				lua_pushvalue(L, -2);
				lua_pushstring(L, E("CoreGui"));
				lua_call(L, 2, 1);
				lua_remove(L, 1); // CoreGui is alone on the stack.
				lua_getglobal(L, E("cloneref"));
				lua_pushvalue(L, 1);
				lua_call(L, 1, 1);
				lua_getfield(L, -1, E("RobloxGui"));
				lua_getglobal(L, E("cloneref"));
				lua_pushvalue(L, -2);
				lua_call(L, 1, 1);
				return 1;
			}


			auto setclipboard(lua_State* rl) -> int
			{
				LOGD("setclipboard");

				luaL_checktype(rl, 1, LUA_TSTRING);

				std::string content = lua_tostring(rl, 1);

				HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, content.size() + 1);
				memcpy(GlobalLock(hMem), content.data(), content.size());
				GlobalUnlock(hMem);
				OpenClipboard(0);
				EmptyClipboard();
				SetClipboardData(CF_TEXT, hMem);
				CloseClipboard();
				return 0;
			}


			auto getclipboard(lua_State* rl) -> int
			{
				LOGD("getclipboard");

				OpenClipboard(NULL);
				std::string clipboard = reinterpret_cast<char*>(GetClipboardData(CF_TEXT));

				lua_pushlstring(rl, clipboard.data(), clipboard.length());
				return 1;
			}

			auto httpget(lua_State* rl) -> int
			{
				LOGD("HttpGet");

				luaL_checktype(rl, 1, LUA_TUSERDATA);
				luaL_checktype(rl, 2, LUA_TSTRING);

				std::string url = lua_tostring(rl, 2);

				if (url.find("http") == std::string::npos)
					luaL_error(rl, "Invalid url, (expected 'http://' or 'https://')");

				cpr::Header Headers{  };

				Headers.insert({ "User-Agent", "Roblox/WinInet" });

				auto result = cpr::Get(cpr::Url{ url }, Headers);

				if (Utils::is_errors(result.status_code))
				{
					std::string http_error;
					http_error += "[HttpGet] ";
					http_error += std::to_string(result.status_code);
					http_error += " ";
					http_error += Utils::write_error(result.status_code);

					luaL_error(rl, http_error.c_str());
				}

				lua_pushlstring(rl, result.text.data(), result.text.size());
				return 1;

				//return atlantis::yielding::begin_yield(rl, [url, Headers]() -> atlantis::yielding::yielded_t
				//	{
				//		auto result = cpr::Get(cpr::Url{ url }, Headers);

				//		return ([result](lua_State* rl) {

				//			if (Utils::is_errors(result.status_code))
				//			{
				//				std::string http_error;
				//				http_error += "[HttpGet] ";
				//				http_error += std::to_string(result.status_code);
				//				http_error += " ";
				//				http_error += Utils::write_error(result.status_code);

				//				luaL_error(rl, http_error.c_str());
				//			}

				//			lua_pushlstring(rl, result.text.data(), result.text.size());
				//			return 1;;
				//			});
				//	});
			}

			int request(lua_State* rl)
			{
				LOGD("request");

				lua_getfield(rl, 1, "Url");
				auto url = luaL_checkstring(rl, -1);
				lua_pop(rl, 1);

				lua_getfield(rl, 1, "Method");
				auto method = std::string(luaL_checkstring(rl, -1));
				lua_pop(rl, 1);

				std::string body;
				lua_getfield(rl, 1, "Body");
				if (lua_isstring(rl, -1))
				{
					body = luaL_checkstring(rl, -1);
				}
				lua_pop(rl, 1);

				cpr::Header headers;
				lua_getfield(rl, 1, "Headers");
				if (lua_istable(rl, -1))
				{
					lua_pushnil(rl);
					while (lua_next(rl, -2))
					{
						headers[luaL_checkstring(rl, -2)] = luaL_checkstring(rl, -1);
						lua_pop(rl, 1);
					}
				}
				lua_pop(rl, 1);

	/*			cpr::Cookies cookies;
				lua_getfield(rl, 1, "Cookies");
				if (lua_istable(rl, -1))
				{
					lua_pushnil(rl);
					while (lua_next(rl, -2))
					{
						std::string CookieKey = luaL_checkstring(rl, -2);
						std::string CookieValue = luaL_checkstring(rl, -1);

						cookies[CookieKey] = CookieValue;
						lua_pop(rl, 1);
					}
				}
				lua_pop(rl, 1);*/

				headers["User-Agent"] = "Pluto"; // Atlantis

				HW_PROFILE_INFO hwProfileInfo;
				if (!GetCurrentHwProfile(&hwProfileInfo))
					luaL_error(rl, "Invalid HWID.");

				std::string hwid = hwProfileInfo.szHwProfileGuid;
				Utils::replace(hwid, "}", "");
				Utils::replace(hwid, "{", "");

				headers["Pluto-Fingerprint"] = ::crypt::hash(hwid, "sha256");
				headers["Pluto-Identifier"] = ::crypt::hash(hwid, "sha256");


				return atlantis::yielding::begin_yield(rl, [method, url, body,headers,rl]() -> atlantis::yielding::yielded_t
					{
						cpr::Response response;

						if (method == "GET" || method == "Get")
						{
							response = cpr::Get(cpr::Url(url), headers /*, cookies*/);
						}
						else if (method == "POST" || method == "Post")
						{
							response = cpr::Post(cpr::Url(url), cpr::Body(body), headers/*, cookies*/);
						}
						else if (method == "PATCH" || method == "Patch")
						{
							response = cpr::Patch(cpr::Url(url), cpr::Body(body), headers/*, cookies*/);
						}
						else if (method == "PUT" || method == "Put")
						{
							response = cpr::Put(cpr::Url(url), cpr::Body(body), headers/*, cookies*/);
						}
						else
						{
							luaL_error(rl, "invalid method");
						}

						return ([response](lua_State* rl) {

							lua_newtable(rl);

							lua_pushlstring(rl, response.text.data(), response.text.length());
							lua_setfield(rl, -2, "Body");

							lua_pushinteger(rl, response.status_code);
							lua_setfield(rl, -2, "StatusCode");

							lua_pushlstring(rl, response.status_line.data(), response.status_line.length());
							lua_setfield(rl, -2, "StatusMessage");

							lua_pushboolean(rl, response.status_code >= 200 && response.status_code < 300);
							lua_setfield(rl, -2, "Success");

							lua_newtable(rl);
							for (const auto& header : response.header)
							{
								lua_pushlstring(rl, header.first.data(), header.first.length());
								lua_pushlstring(rl, header.second.data(), header.second.length());
								lua_settable(rl, -3);
							}
							lua_setfield(rl, -2, "Headers");

							return 1;
							});
					});
			}


			auto setfps(lua_State* rl) -> int
			{
				LOGD("setfps");

				luaL_checktype(rl, 1, LUA_TNUMBER);

				double fps = lua_tonumber(rl, 1);
				if (fps <= 0)
					TaskScheduler::set_fps(100);
				else
					TaskScheduler::set_fps(fps);

				return 0;
			}


			int messagebox(lua_State* rl) {
				LOGD("messagebox");

				const char* text = lua_tostring(rl, 1).c_str();
				const char* caption = lua_tostring(rl, 2).c_str();

				MessageBoxA(NULL, text, caption, NULL);
				return 0;
			}


			auto getfps(lua_State* rl) -> int
			{
				LOGD("getfps");

				lua_pushnumber(rl, TaskScheduler::get_fps());
				return 1;
			}


			auto getobjects(lua_State* rl) -> int
			{
				LOGD("getobjects");

				luaL_checktype(rl, 2, LUA_TSTRING);
				std::string asset = lua_tostring(rl, 2);

				lua_getglobal(rl, "game");
				lua_getfield(rl, -1, "GetService");
				lua_pushvalue(rl, -2);
				lua_pushstring(rl, "InsertService");
				lua_call(rl, 2, 1);

				lua_getfield(rl, -1, "LoadLocalAsset");
				lua_pushvalue(rl, -2);
				lua_pushstring(rl, asset.data());
				lua_call(rl, 2, 1);

				lua_createtable(rl, 0, 0);
				lua_pushvalue(rl, -2);
				lua_rawseti(rl, -2, 1);

				return 1;
			}


			auto firetouchinterest(lua_State* rl) -> int
			{
				LOGD("firetouchinterest");

				luaL_checktype(rl, 1, LUA_TUSERDATA);
				luaL_checktype(rl, 2, LUA_TUSERDATA);
				
				if (lua_type(rl, 3) != LUA_TNUMBER && lua_type(rl, 3) != LUA_TBOOLEAN) {
					luaL_error(rl, "invalid type at argument 3");
					return 0;
				}

				uintptr_t part = *(uintptr_t*)lua_touserdata(rl, 1);
				uintptr_t transmitter = *(uintptr_t*)lua_touserdata(rl, 2);
				int state = lua_tointeger(rl, 3);

				uintptr_t part_primitive = *reinterpret_cast<uintptr_t*>(part + 352);
				uintptr_t transmitter_primitive = *reinterpret_cast<uintptr_t*>(transmitter + 352);

				if (!part_primitive || !transmitter_primitive || *(uintptr_t*)(part_primitive + 464) != *(uintptr_t*)(transmitter_primitive + 464))
					luaL_error(rl, "new overlap in different world.");

				Defs::rbx_touch(*reinterpret_cast<uintptr_t*>(part_primitive + 464), part_primitive, transmitter_primitive, state);
				return 0;
			}


			auto fireclickdetector(lua_State* rl) -> int
			{
				LOGD("fireclickdetector");

				luaL_checktype(rl, 1, LUA_TUSERDATA);

				uintptr_t click_detector = *reinterpret_cast<uintptr_t*>(lua_touserdata(rl, 1));
				float distance = lua_tonumber(rl, 2);

				lua_getglobal(rl, "game");
				lua_getfield(rl, -1, "GetService");
				lua_pushvalue(rl, -2);
				lua_pushstring(rl, "Players");
				lua_pcall(rl, 2, 1, 0);
				lua_getfield(rl, -1, "LocalPlayer");

				uintptr_t local_player = *reinterpret_cast<uintptr_t*>(lua_touserdata(rl, -1));
				Defs::rbx_click(click_detector, distance, local_player);
				return 0;
			}

			auto fireproximityprompt(lua_State* rl) -> int
			{
				LOGD("fireproximityprompt");

				luaL_checktype(rl, 1, LUA_TUSERDATA);

				lua_getglobal(rl, xorstr_("typeof"));
				lua_pushvalue(rl, 1);
				lua_call(rl, 1, 1);
				const bool isInstance = (strcmp(lua_tolstring(rl, -1, 0), xorstr_("Instance")) == 0);
				lua_pop(rl, 1);

				if (!isInstance)
					luaL_argerror(rl, 1, xorstr_("Expected Instance"));

				lua_getfield(rl, 1, xorstr_("IsA"));
				lua_pushvalue(rl, 1);
				lua_pushstring(rl, xorstr_("ProximityPrompt"));
				lua_call(rl, 2, 1);
				const bool isExpectedClass = lua_toboolean(rl, -1);
				lua_pop(rl, 1);

				if (!isExpectedClass)
					luaL_argerror(rl, 1, xorstr_("Expected an ProximityPrompt"));


				reinterpret_cast<int(__thiscall*)(std::uintptr_t)>(Offsets::fireproximityprompt)(*reinterpret_cast<std::uintptr_t*>(lua_touserdata(rl, 1)));
				return 0;
			}



			using roblox_function_t = int(__fastcall*)(int L);

			int getrunningscripts(lua_State* L) {
				LOGD("getrunningscripts");

					std::map< uintptr_t, bool > map;

					lua_pushvalue(L, LUA_REGISTRYINDEX);

					lua_newtable(L);

					lua_pushnil(L);

					auto c = 0u;
					while (lua_next(L, -3))
					{
						if (lua_isthread(L, -1))
						{
							const auto thread = lua_tothread(L, -1);

							if (thread)
							{
								if (const auto script_ptr = reinterpret_cast<std::uintptr_t>(thread->userdata) + 0x50; *reinterpret_cast<std::uintptr_t*>(script_ptr))
								{
									if (map.find(*(uintptr_t*)script_ptr) == map.end())
									{
										map.insert({ *(uintptr_t*)script_ptr, true});

										Defs::rbx_pushinstance2(L, (void*)script_ptr);

										lua_rawseti(L, -4, ++c);
									}
								}
							}
						}
						
						lua_pop(L, 1);
					}

					return 1;
				}

				auto getsenv(lua_State * L) -> int
				{

				luaL_checktype(L, 1, LUA_TUSERDATA);
				uintptr_t script_instance = *(uintptr_t*)lua_touserdata(L, 1);

			//	Defs::rbx_print(1, "script: %llX", script_instance);

				lua_getglobal(L, xorstr_("typeof"));
				lua_pushvalue(L, 1);
				lua_call(L, 1, 1);
				const bool isInstance = (strcmp(lua_tolstring(L, -1, 0), xorstr_("Instance")) == 0);
				lua_pop(L, 1);

				if (!isInstance)
					luaL_argerror(L, 1, xorstr_("Expected Instance"));

				lua_getfield(L, 1, xorstr_("ClassName"));
				const char* class_name = lua_tolstring(L, -1, NULL);

				lua_pop(L, 1);

				//lua_settop(L, 0);

				if (strcmp(class_name, "LocalScript") && strcmp(class_name, "ModuleScript")) {
					luaL_argerror(L, 1, xorstr_("LocalScript or ModuleScript"));
				}

				if (!strcmp(class_name, "LocalScript")) {
					auto node = *reinterpret_cast<Node_t**>(script_instance + Offsets::BaseScript::ScriptNode);

					if (node == nullptr)
					{
						LOGD("Script Node was nullptr");

						lua_pushnil(L);
						return 1;
					}

					auto wtr = node->wtr;
					if (wtr == nullptr)
					{
						LOGD("WeakThreadRef was nullptr");

						lua_pushnil(L);
						return 1;
					}

					for (auto it = wtr; it; it = it->next)
					{
						auto thref = it->liveThreadRef;
						if (thref == nullptr)
						{
							continue;
						}

						auto thread = thref->th;
						if (thread == nullptr)
						{
							continue;
						}

						const auto extraspace = reinterpret_cast<int64_t>(lua_getthreaddata(thread));

						uintptr_t thread_scr;

						if (extraspace)
						{
							thread_scr = *(uintptr_t*)(extraspace + 0x50);
						}

						if (thread_scr)
						{
							if (script_instance == thread_scr)
							{
								lua_pushvalue(thread, LUA_GLOBALSINDEX);
								lua_xmove(thread, L, 1);
								return 1;
							}
						}
						else
						{
							continue;
						}
					}
				}
				else if (!strcmp(class_name, "ModuleScript")) {
					getloadedmodules(L);

					bool is_loaded = false;

					if (lua_istable(L, -1)) {
						lua_pushnil(L);
						while (lua_next(L, -2) != 0) {

							if (lua_isuserdata(L, -1)) {
								uintptr_t loaded_module = *(uintptr_t*)lua_touserdata(L, -1);

								if (loaded_module == script_instance) {
									is_loaded = true;
								}
							}
							lua_pop(L, 1); 
						}
					}
					lua_pop(L, 1);

					if (!is_loaded) {
						luaL_error(L,"ModuleScript not loaded");
						return 0;
					}

					lua_getglobal(L, "getfenv");
					{
						*(bool*)(Offsets::ModuleScript::EnableLoadModule) = true;



						lua_getglobal(L, "debug");
						lua_getfield(L, -1, "loadmodule");
						{
							lua_pushvalue(L, 1);
						}
						lua_call(L, 1, 1);
						lua_remove(L, -2);

						*(bool*)(Offsets::ModuleScript::EnableLoadModule) = false;
					}
					lua_call(L, 1, 1);

					if (!lua_isnoneornil(L, -1))
					{
						return 1;
					}
				}

				lua_pushnil(L);
				return 1;
			}

			auto identifyexecutor(lua_State* rl) -> int
			{
				LOGD("identifyexecutor");

				lua_pushstring(rl, xorstr_("Pluto")); // atlantis
				lua_pushstring(rl, xorstr_("1.0.0"));
				return 2;
			}

			int (*old_require) (lua_State* L);

			__forceinline int require(lua_State* L) {
				LOGD("require");

				luaL_checktype(L, 1, LUA_TUSERDATA);

				const auto script = *reinterpret_cast<std::uintptr_t*>(lua_touserdata(L, 1));

				lua_getglobal(L, xorstr_("typeof"));
				lua_pushvalue(L, 1);
				lua_call(L, 1, 1);
				const bool isInstance = (strcmp(lua_tolstring(L, -1, 0), xorstr_("Instance")) == 0);
				lua_pop(L, 1);

				if (!isInstance)
					luaL_argerror(L, 1, xorstr_("Expected Instance"));


				lua_getfield(L, 1, xorstr_("IsA"));
				lua_pushvalue(L, 1);
				lua_pushstring(L, xorstr_("ModuleScript"));
				lua_call(L, 2, 1);
				const bool isExpectedClass = lua_toboolean(L, -1);
				lua_pop(L, 1);

				if (!isExpectedClass)
					luaL_argerror(L, 1, xorstr_("Expected an ModuleScript"));

				LOGD("work till this");

				*reinterpret_cast<std::uint8_t*>(script + Offsets::ModuleScript::RequireFlag) = 1;

				const auto res = old_require(L);

				*reinterpret_cast<std::uint8_t*>(script + Offsets::ModuleScript::RequireFlag) = 0;


				return res;
			}

			auto getcustomasset(lua_State* L) -> int
			{
				LOGD("getcustomasset");

				luaL_checktype(L, 1, LUA_TSTRING);

				std::string assetname = lua_tostring(L, 1);
				std::filesystem::path workspace_path = workspaceDir / assetname;
				std::filesystem::path asset_directory = assetsDir / workspace_path.filename();

				try {
					std::filesystem::copy_file(workspace_path, asset_directory, std::filesystem::copy_options::update_existing);
					lua_pushstring(L, std::string("rbxasset://gca/" + asset_directory.filename().string()).data());
					return 1;
				}
				catch (std::exception& e) {
					luaL_error(L, xorstr_("failed to get custom asset"));
					return 0;
				}

				return 1;
			}
		}


		namespace Instance
		{
			auto getinstances(lua_State* rl) -> int
			{
				LOGD("getinstances");

				struct instance_context {
					lua_State* rl;
					std::intptr_t n;
				} context = { rl, 0 };

				lua_newtable(rl);
				for (lua_Page* page = rl->global->allgcopages; page;) {
					lua_Page* next{ page->listnext };

					luaM_visitpage(page, &context,
						[](void* context, lua_Page* page, GCObject* gco) -> bool {
							instance_context* gcContext{ reinterpret_cast<instance_context*>(context) };
							auto type = gco->gch.tt;

							if (type == LUA_TUSERDATA) {


								TValue* top = gcContext->rl->top;
								top->value.p = reinterpret_cast<void*>(gco);
								top->tt = type;
								gcContext->rl->top++;

								if (!strcmp(luaL_typename(gcContext->rl, -1), "Instance")) {
									gcContext->n++;
									lua_rawseti(gcContext->rl, -2, gcContext->n);
								}
								else {
									lua_pop(gcContext->rl, 1);
								}
							}

							return true;
						}
					);
					page = next;
				}
				return 1;
			}


			auto getnilinstances(lua_State* rl) -> int
			{
				LOGD("getnilinstances");

				struct instance_context {
					lua_State* rl;
					std::intptr_t n;
				} context = { rl, 0 };

				lua_newtable(rl);

				for (lua_Page* page = rl->global->allgcopages; page;) {
					lua_Page* next{ page->listnext };

					luaM_visitpage(page, &context,
						[](void* context, lua_Page* page, GCObject* gco) -> bool {
							instance_context* gcContext{ reinterpret_cast<instance_context*>(context) };
							auto type = gco->gch.tt;

							if (type == LUA_TUSERDATA) {


								TValue* top = gcContext->rl->top;
								top->value.p = reinterpret_cast<void*>(gco);
								top->tt = type;
								gcContext->rl->top++;

								if (!strcmp(luaL_typename(gcContext->rl, -1), "Instance")) {
									lua_getfield(gcContext->rl, -1, "Parent");
									bool nullParent = lua_isnoneornil(gcContext->rl, -1);

									if (nullParent) {
										lua_pop(gcContext->rl, 1);
										gcContext->n++;
										lua_rawseti(gcContext->rl, -2, gcContext->n);
									}
									else {
										lua_pop(gcContext->rl, 2);
									}
								}
								else {
									lua_pop(gcContext->rl, 1);
								}
							}

							return true;
						}
					);

					page = next;
				}
				return 1;
			}

			__forceinline int getscripts(lua_State* L) {
				//luaL_stackcheck(L, 0, 0);
				LOGD("getscripts");

				struct instancecontext {
					lua_State* L;
					__int64 n;
				} Context = { L, 0 };

				lua_createtable(L, 0, 0);

				const auto ullOldThreshold = L->global->GCthreshold;
				L->global->GCthreshold = SIZE_MAX; // Disable GC temporarily

				luaM_visitgco(L, &Context, [](void* ctx, lua_Page* page, GCObject* gco) -> bool {
					auto gCtx = static_cast<instancecontext*>(ctx);
					const auto type = gco->gch.tt;

					if (isdead(gCtx->L->global, gco))
						return false;

					if (type == LUA_TUSERDATA) {

						TValue* top = gCtx->L->top;
						top->value.p = reinterpret_cast<void*>(gco);
						top->tt = type;
						gCtx->L->top++;

						if (!strcmp(luaL_typename(gCtx->L, -1), "Instance")) { // instance check
							lua_getfield(gCtx->L, -1, "ClassName"); // check if instance has no parent / basically nil instance

							const char* inst_class = lua_tolstring(gCtx->L, -1,0);
							if (!strcmp(inst_class, "LocalScript") || !strcmp(inst_class, "ModuleScript"))
							{
								lua_pop(gCtx->L, 1);
								gCtx->n++;
								lua_rawseti(gCtx->L, -2, gCtx->n);
							}
							else
								lua_pop(gCtx->L, 2);

						}
						else {
							lua_pop(gCtx->L, 1);
						}
					}

					return true;
					});

				L->global->GCthreshold = ullOldThreshold;

				return 1;
			}

			auto isscriptable(lua_State* rl) -> int
			{
				LOGD("isscriptable");

				luaL_checktype(rl, 1, LUA_TUSERDATA);
				luaL_checktype(rl, 2, LUA_TSTRING);

				__int64 instance = *reinterpret_cast<__int64*>(lua_touserdata(rl, 1));
				void* instance2 = lua_touserdata(rl, 1);

				std::string property_name = lua_tostring(rl, 2);


				lua_getglobal(rl, xorstr_("typeof"));
				lua_pushvalue(rl, 1);
				lua_call(rl, 1, 1);
				const bool isInstance = (strcmp(lua_tolstring(rl, -1, 0), xorstr_("Instance")) == 0);
				lua_pop(rl, 1);

				if (!isInstance) {
					luaL_argerror(rl, 1, xorstr_("Expected Instance"));
					return 0;
				}

				__int64 property = 0;
				std::vector<__int64> properties = *reinterpret_cast<std::vector<__int64>*>(*reinterpret_cast<__int64*>(instance + 24) + 40);

				for (int i = 0; i < properties.size(); i++)
				{
					std::string prop_name = *reinterpret_cast<std::string*>(*reinterpret_cast<__int64*>(properties[i] + 8));
					if (prop_name == property_name)
					{
						property = properties[i];
						break;
					}
				}

				if (property == NULL)
					luaL_error(rl, "Property not exists.");

				const auto was_scriptable
					= g_instance.is_scriptable(instance2, property_name);

				lua_pushboolean(rl, was_scriptable.value_or(lua_isscriptable(rl, property)));
				return 1;
			}

			auto setscriptable(lua_State* rl)
			{
				LOGD("setscriptable");

				luaL_checktype(rl, 1, LUA_TUSERDATA);
				luaL_checktype(rl, 2, LUA_TSTRING);
				luaL_checktype(rl, 3, LUA_TBOOLEAN);

				__int64 instance = *reinterpret_cast<__int64*>(lua_touserdata(rl, 1));
				void* instance2 = lua_touserdata(rl, 1);
				std::string property_name = lua_tostring(rl, 2);
				bool enabled = lua_toboolean(rl, 3);

				//lua_getmetatable(rl, 1);
				//lua_rawgetfield(rl, -1, "__index");

				//Closure* index = (Closure*)lua_topointer(rl, -1);
				//lua_CFunction index_f = index->c.f;
				//int64_t old_index = (__int64)index_f;
				//index->c.f = [](lua_State* L) -> int {
				//	return reinterpret_cast<__int64(__fastcall*)(__int64)>(old_instance_index)((__int64)L);
				//};

				for (__int64 property : *reinterpret_cast<std::vector<__int64>*>(*reinterpret_cast<__int64*>(instance + 24) + 40))
				{
					std::string name = *reinterpret_cast<std::string*>(*reinterpret_cast<__int64*>(property + 8));
					if (name == property_name)
					{
						const auto was_scriptable = g_instance.is_scriptable(instance2, property_name);

						g_instance.toggle(instance2, property_name, enabled);

						lua_pushboolean(rl, was_scriptable.value_or(lua_isscriptable(rl, property)));
						return 1;
					}
				}
				luaL_error(rl, "property not found");
				return 0;
			}

			auto gethiddenproperty(lua_State* rl) -> int
			{
				LOGD("gethiddenproperty");

				luaL_checktype(rl, 1, LUA_TUSERDATA);
				luaL_checktype(rl, 2, LUA_TSTRING);

				uintptr_t instance = *reinterpret_cast<uintptr_t*>(lua_touserdata(rl, 1));
				std::string property_name = lua_tostring(rl, 2);

				uintptr_t object;
				bool found;
				std::vector<uintptr_t> properties = *reinterpret_cast<std::vector<uintptr_t>*>(*reinterpret_cast<uintptr_t*>(instance + 24) + 40);
				for (int i = 0; i < properties.size(); i++)
				{
					uintptr_t property = properties[i];
					if (property_name == *reinterpret_cast<std::string*>(*reinterpret_cast<uintptr_t*>(property + 8)))
					{
						object = property;

						found = true;
						break;
					}
				}

				if (found == false)
					luaL_error(rl, "Property not exists.");

				std::string propertyTypeName = *(std::string*)(*(uintptr_t*)(object + 0x48) + 0x8);

				int propertyType = *(int*)(*(uintptr_t*)(object + 0x48) + 0x30);
				if (!strcmp(propertyTypeName.c_str(), "BinaryString")) {
					*(int*)(*(uintptr_t*)(object + 0x48) + 0x30) = 6;
				}

				bool scriptable = lua_isscriptable(rl, object);
				int original_value = *reinterpret_cast<int*>(object + 64);
				if (scriptable == true)
				{
					lua_getfield(rl, 1, property_name.c_str());
					lua_pushboolean(rl, false);
					return 2;
				}
				else
				{
					lua_setscriptable(rl, object, true);
					lua_getfield(rl, 1, property_name.c_str());
					lua_pushboolean(rl, true);
					lua_setscriptable(rl, object, false);
					return 2;
				}
			}

			auto sethiddenproperty(lua_State* rl) -> int
			{
				LOGD("sethiddenproperty");

				luaL_checktype(rl, 1, LUA_TUSERDATA);
				luaL_checktype(rl, 2, LUA_TSTRING);

				uintptr_t instance = *reinterpret_cast<uintptr_t*>(lua_touserdata(rl, 1));
				std::string property_name = lua_tostring(rl, 2);

				uintptr_t object;
				bool found;
				std::vector<uintptr_t> properties = *reinterpret_cast<std::vector<uintptr_t>*>(*reinterpret_cast<uintptr_t*>(instance + 24) + 40);
				for (int i = 0; i < properties.size(); i++)
				{
					uintptr_t property = properties[i];
					if (property_name == *reinterpret_cast<std::string*>(*reinterpret_cast<uintptr_t*>(property + 8)))
					{
						object = property;
						found = true;
						break;
					}
				}

				if (found == false)
					luaL_error(rl, "Property not exists.");

				bool scriptable = lua_isscriptable(rl, object);
				int original_value = *reinterpret_cast<int*>(object + 64);
				if (scriptable == true)
				{
					lua_pushvalue(rl, 3);
					lua_setfield(rl, 1, property_name.c_str());
					lua_pushboolean(rl, false);
					lua_setscriptable(rl, object, false);
					return 1;
				}
				else
				{
					lua_setscriptable(rl, object, true);
					lua_pushvalue(rl, 3);
					lua_setfield(rl, 1, property_name.c_str());
					lua_pushboolean(rl, true);
					lua_setscriptable(rl, object, false);
					return 1;
				}
			}

			auto getscriptbytecode_handler(lua_State* rl) -> int
			{
				uintptr_t script = *reinterpret_cast<uintptr_t*>(lua_touserdata(rl, 1));
				const char* classname = *reinterpret_cast<const char**>(*reinterpret_cast<uintptr_t*>(script + 24) + 8);
				std::string compressed_bytecode;

				if (strcmp(classname, "LocalScript") == 0)
					compressed_bytecode = *reinterpret_cast<std::string*>(*reinterpret_cast<uintptr_t*>(script + Offsets::Bytecode::localscript) + 16);
				else if (strcmp(classname, "ModuleScript") == 0)
					compressed_bytecode = *reinterpret_cast<std::string*>(*reinterpret_cast<uintptr_t*>(script + Offsets::Bytecode::modulescript) + 16);
				else
					luaL_error(rl, "Local/Module scripts expected.");

				std::string bytecode = Utils::decompress_bytecode(compressed_bytecode);
				if (bytecode.at(0) == 0)
					luaL_error(rl, "Invalid bytecode.");

				lua_pushlstring(rl, bytecode.data(), bytecode.length());
				return 1;
			}


			auto getscriptbytecode(lua_State* rl) -> int
			{
				LOGD("getscriptbytecode");

				luaL_checktype(rl, 1, LUA_TUSERDATA);

				lua_pushcclosure(rl, getscriptbytecode_handler, nullptr, 0);
				lua_pushvalue(rl, 1);
				lua_call(rl, 1, 1);

				return 1;
			}

			auto getscripthash(lua_State* rl) -> int
			{
				LOGD("getscripthash");

				luaL_checktype(rl, 1, LUA_TUSERDATA);

				uintptr_t script = *reinterpret_cast<uintptr_t*>(lua_touserdata(rl, 1));
				const char* classname = *reinterpret_cast<const char**>(*reinterpret_cast<uintptr_t*>(script + 24) + 8);
				std::string compressed_bytecode;

				if (strcmp(classname, "LocalScript") == 0)
					compressed_bytecode = *reinterpret_cast<std::string*>(*reinterpret_cast<uintptr_t*>(script + Offsets::Bytecode::localscript) + 16);
				else if (strcmp(classname, "ModuleScript") == 0)
					compressed_bytecode = *reinterpret_cast<std::string*>(*reinterpret_cast<uintptr_t*>(script + Offsets::Bytecode::modulescript) + 16);
				else
					luaL_error(rl, "Local/Module scripts expected.");

				auto decompressed = Utils::decompress_bytecode(compressed_bytecode);
				auto hash = ::crypt::hash(decompressed, "sha384");
				lua_pushlstring(rl, hash.c_str(), hash.size());

				return 1;
			}

			auto getproperties(lua_State* rl) -> int
			{
				LOGD("getproperties");

				luaL_checktype(rl, 1, LUA_TUSERDATA);

				uintptr_t script = *reinterpret_cast<uintptr_t*>(lua_touserdata(rl, 1));
				uintptr_t descriptor = *reinterpret_cast<uintptr_t*>(script + 24);


				lua_newtable(rl);

				int position = 0;
				std::vector<uintptr_t> properties = *reinterpret_cast<std::vector<uintptr_t>*>(descriptor + 40);
				for (int i = 0; i < properties.size(); i++)
				{
					std::string property_name = *reinterpret_cast<std::string*>(*reinterpret_cast<uintptr_t*>(properties[i] + 8));
					lua_pushlstring(rl, property_name.c_str(), property_name.size());
					lua_rawseti(rl, -2, ++position);
				}

				return 1;
			}

			auto getcallbackvalue(lua_State* rl) -> int
			{
				LOGD("getcallbackvalue");

				luaL_checktype(rl, 1, LUA_TUSERDATA);

				uintptr_t object = *reinterpret_cast<uintptr_t*>(lua_touserdata(rl, 1));

				std::int32_t atom{};
				auto property = lua_tostringatom(rl, 2, &atom);

				auto map = Offsets::map_descriptor[atom];
				auto descriptor = Defs::rbx_getcallback(*reinterpret_cast<__int64*>(object + 24) + 944, &map);
				auto prop = *reinterpret_cast<__int64*>(descriptor);

				int idx = *(__int64*)(*(uintptr_t*)(*(uintptr_t*)(*(uintptr_t*)((object + *(uintptr_t*)(prop + 120)) + 24) + 56) + 40) + 20);
				lua_getref(rl, idx);

				if (lua_iscfunction(rl, -1) || lua_isLfunction(rl, -1))
					return 1;

				lua_pop(rl, 1);
				return 0;
			}
		}

		namespace Signals
		{
			//struct connection_object {
			//	signal_t* signal;
			//	uint64_t state;
			//	uint64_t metatable;
			//	uint64_t root;
			//};

			//std::unordered_map<signal_t*, connection_object> connection_table;

			//int connection_blank(lua_State* rl) {
			//	return 0;
			//}

			//int disable_connection(lua_State* rl) {
			//	//auto connection = (connection_object*)lua_touserdata(rl, 1);
			//	//if (connection->signal->state != 0)
			//	//	connection->state = connection->signal->state;

			//	//connection->signal->state = 0;
			//	return 0;
			//}

			//int enable_connection(lua_State* rl) {
			//	//auto connection = (connection_object*)lua_touserdata(rl, 1);
			//	//connection->signal->state = connection->state;
			//	return 0;
			//}

			//int disconnect_connection(lua_State* rl) {
			//	//auto connection = (connection_object*)lua_touserdata(rl, 1);
			//	//auto root = (signal_t*)connection->root;
			//	//if ((uint64_t)root == (uint64_t)connection) {
			//	//	luaL_error(rl, "Failed to disconnect.");
			//	//}

			//	//while (root->next && root->next != connection->signal) {
			//	//	root = root->next;
			//	//}

			//	//if (!root->next) {
			//	//	// dont error bc redz shit gonna break
			//	//	// luaL_error(rl, "Already disconnected.");
			//	//	return 0;
			//	//}

			//	//root->next = root->next->next;
			//	//connection->signal->state = 0;
			//	return 0;
			//}

			//int connection_index(lua_State* rl) {
			//	std::string key = std::string(lua_tolstring(rl, 2, nullptr));
			//	auto connection = (connection_object*)lua_touserdata(rl, 1);
			//	uintptr_t connection2 = *(uintptr_t*)lua_touserdata(rl, 1);

			//	if (key == "Enabled" || key == "enabled") {
			//		lua_pushboolean(rl, !(connection->signal->state == 0));
			//		return 1;
			//	}

			//	if (key == "Function" || key == "function" || key == "Fire" || key == "fire" || key == "Defer" || key == "defer") {
			//		int signal_data = *(int*)&connection->signal->signal_data;
			//		if (signal_data && *(int*)&connection->signal->signal_data->connection_data) {
			//			int index = connection->signal->signal_data->connection_data->func_idx;
			//			lua_rawgeti(rl, LUA_REGISTRYINDEX, index);

			//			if (lua_type(rl, -1) != LUA_TFUNCTION)
			//				lua_pushcclosure(rl, connection_blank, 0, 0, 0);

			//			return 1;
			//		}

			//		lua_pushcclosure(rl, connection_blank, 0, 0, 0);
			//		return 1;
			//	}

			//	if (key == "LuaConnection") {
			//		int signal_data = *(int*)&connection->signal->signal_data;
			//		if (signal_data && *(int*)&connection->signal->signal_data->connection_data) {
			//			int index = connection->signal->signal_data->connection_data->func_idx;

			//			lua_rawgeti(rl, LUA_REGISTRYINDEX, index);
			//			auto func_tval = (TValue*)index2addr(rl, -1);
			//			auto cl = (Closure*)func_tval->value.gc;
			//			bool lua = !cl->isC;

			//			lua_pop(rl, 1);
			//			lua_pushboolean(rl, lua);
			//			return 1;
			//		}

			//		lua_pushboolean(rl, false);
			//		return 1;
			//	}

			//	if (key == "ForeignState") {
			//		int signal_data = *(int*)&connection->signal->signal_data;
			//		if (signal_data && *(int*)&connection->signal->signal_data->connection_data) {
			//			int index = connection->signal->signal_data->connection_data->thread_idx;
			//			lua_rawgeti(rl, LUA_REGISTRYINDEX, index);

			//			if (!lua_isthread(rl, -1))
			//			{
			//				lua_pushboolean(rl, true);
			//				return 1;
			//			}
			//			else
			//			{
			//				lua_State* th = lua_tothread(rl, -1);
			//				lua_pop(rl, 1);

			//				lua_pushboolean(rl, (th->global != rl->global));

			//				return 1;
			//			}
			//		}

			//		lua_pushboolean(rl, false);
			//		return 1;
			//	}

			//	if (key == "Disconnect" || key == "disconnect") {
			//		lua_pushcclosure(rl, disconnect_connection, 0, 0, 0);
			//		return 1;
			//	}

			//	if (key == "Disable" || key == "disable") {
			//		lua_pushcclosure(rl, disable_connection, 0, 0, 0);
			//		return 1;
			//	}

			//	if (key == "Enable" || key == "enable") {
			//		lua_pushcclosure(rl, enable_connection, 0, 0, 0);
			//		return 1;
			//	}

			//	if (key == "Thread") {
			//		int signal_data = *(int*)&connection->signal->signal_data;
			//		if (signal_data && *(int*)&connection->signal->signal_data->connection_data) {
			//			int index = connection->signal->signal_data->connection_data->thread_idx;
			//			lua_rawgeti(rl, LUA_REGISTRYINDEX, index);

			//			if (lua_type(rl, -1) != LUA_TTHREAD)
			//				lua_pushthread(rl);

			//			return 1;
			//		}
			//	}

			//	//luaL_error(rl, "invalid index");
			//	return 0;
			//}

			//int connection_newindex(lua_State* rl) {
			//	std::string key = std::string(lua_tolstring(rl, 2, nullptr));
			//	auto connection = (connection_object*)lua_touserdata(rl, 1);
			//	uintptr_t connection2 = *(uintptr_t*)lua_touserdata(rl, 1);

			//	if (key == "Enabled" || key == "enabled") {
			//		const auto enabled = luaL_checkboolean(rl, 3);
			//		if (enabled) {
			//			connection->signal->state = connection->state;
			//		}
			//		else {
			//			connection->signal->state = 0;
			//		}
			//	}

			////	luaL_error(rl, "invalid index");
			//	return 0;
			//}

			//int getconnections_handler(lua_State* rl) {
			//	auto signal = *(signal_t**)lua_touserdata(rl, 1);
			//	signal = signal->next;

			//	lua_createtable(rl, 0, 0);
			//	auto signal_root = signal;
			//	int index = 1;

			//	while (signal) {
			//		int func_idx = signal->signal_data->connection_data->func_idx;

			//		if (!connection_table.count(signal)) {
			//			connection_object new_connection;
			//			new_connection.signal = signal;
			//			new_connection.root = (uint64_t)signal_root;
			//			new_connection.state = signal->state;
			//			connection_table[signal] = new_connection;
			//		}

			//		auto connection = (connection_object*)lua_newuserdata(rl, sizeof(connection_object), 0);
			//		*connection = connection_table[signal];

			//		lua_createtable(rl, 0, 0);
			//		lua_pushcclosure(rl, connection_index, 0, 0, 0);
			//		lua_setfield(rl, -2, "__index");

			//		lua_pushcclosure(rl, connection_newindex, 0, 0, 0);
			//		lua_setfield(rl, -2, "__newindex");


			//		lua_pushstring(rl, "SerialityConnection");
			//		lua_setfield(rl, -2, "__type");
			//		lua_setmetatable(rl, -2);

			//		lua_rawseti(rl, -2, index++);
			//		signal = signal->next;
			//	}

			//	return 1;
			//}

			//int getconnections(lua_State* L)
			//{
			//	LOGD("getconnections");

			//	luaL_checktype(L, 1, LUA_TUSERDATA);
			//	if (strcmp(luaL_typename(L, 1), E("RBXScriptSignal")) != 0)
			//	{
			//		luaL_typeerror(L, 1, E("RBXScriptSignal"));
			//	}


			//	lua_getfield(L, 1, "Connect");
			//	if (!lua_isfunction(L, -1)) {
			//		luaL_error(L, "Signal does not have 'Connect' method");
			//	}
			//	lua_pushvalue(L, 1);

			//	lua_pushcfunction(L, connection_blank, nullptr);

			//	if (lua_pcall(L, 2, 1, 0) != LUA_OK) {
			//		luaL_error(L, "Error calling 'Connect': %s", lua_tostring(L, -1));
			//	}

			//	lua_pushcfunction(L, getconnections_handler, nullptr);
			//	lua_pushvalue(L, -2);

			//	if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
			//		luaL_error(L, "Error calling 'getconnections': %s", lua_tostring(L, -1));
			//	}

			//	lua_getfield(L, -2, "Disconnect");
			//	if (lua_isfunction(L, -1)) {
			//		lua_pushvalue(L, -3);
			//		if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
			//			luaL_error(L, "Error calling 'Disconnect': %s", lua_tostring(L, -1));
			//		}
			//	}

			//	return 1;
			//}

int TRBXScriptConnection = 0;

static int disable_connection(lua_State* ls)
{
	LOGD("connection.disable");

	const auto pInfo = reinterpret_cast<argon::lua_objects::RBXConnection_t*>(lua_touserdata(ls, lua_upvalueindex(1)));

	pInfo->disable();

	return 0;
}

static int enable_connection(lua_State* ls)
{
	LOGD("connection.enable");

	const auto pInfo = reinterpret_cast<argon::lua_objects::RBXConnection_t*>(lua_touserdata(ls, lua_upvalueindex(1)));

	pInfo->enable();

	return 0;
}

static int fire_connection(lua_State* ls)
{
	LOGD("connection.Fire");

	const auto nargs = lua_gettop(ls);

	lua_pushvalue(ls, lua_upvalueindex(1));
	if (lua_isfunction(ls, -1))
	{
		lua_insert(ls, 1);
		lua_call(ls, nargs, 0);
	}

	return 0;
}

static int defer_connection(lua_State* ls)
{
	LOGD("connection.defer");

	const auto nargs = lua_gettop(ls);

	lua_getglobal(ls, xorstr_("task"));
	lua_getfield(ls, -1, xorstr_("defer"));
	lua_remove(ls, -2);
	{
		lua_pushvalue(ls, lua_upvalueindex(1));
		if (!lua_isfunction(ls, -1))
		{
			return 0;
		}

		lua_insert(ls, 1); // func to index 1
		lua_insert(ls, 1); // defer to index 1
		/*
		stack before:
		args... nargs
		defer   nargs + 1
		func    nargs + 2

		stack now:
		defer   1
		func    2
		args... 3 + nargs
		*/
	}
	lua_call(ls, nargs + 1, 1); // args + func, return.

	return 1;
}

static int disconnect_connection(lua_State* ls)
{
	LOGD("disconnect connection");

	const auto pInfo = reinterpret_cast<argon::lua_objects::RBXConnection_t*>(lua_touserdata(ls, lua_upvalueindex(1)));
	if (!pInfo->is_enabled())
	{
		luaL_error(ls, "%s", xorstr_("Cannot Disconnect a Disabled connection! ( Enable it first before Disconnecting. )"));
	}

	// roblox pushes a node obj just like this during the ud creation
	const auto pUd = reinterpret_cast<SignalConnectionBridge*>(lua_newuserdatatagged(ls, sizeof(SignalConnectionBridge), TRBXScriptConnection));
	pUd->islot = pInfo->get_node();
	pUd->unk0 = 0;
	pUd->unk1 = 0;

	lua_getfield(ls, LUA_REGISTRYINDEX, xorstr_("RBXScriptConnection"));
	lua_setmetatable(ls, -2);

	lua_getfield(ls, -1, xorstr_("Disconnect"));
	{
		lua_pushvalue(ls, -2);
	}
	if (lua_pcall(ls, 1, 0, 0) && lua_isstring(ls, -1))
	{
		const auto err = lua_tostring(ls, -1);
		luaL_error(ls, xorstr_("Error while disconnecting connection (%s)"), err);
	}

	return 0;
}

static int mt_newindex(lua_State* ls)
{
	LOGD("connection.__newindex");

	const auto pInfo = reinterpret_cast<argon::lua_objects::RBXConnection_t*>(lua_touserdata(ls, 1));
	const char* key = luaL_checkstring(ls, 2);

	if (strcmp(key, xorstr_("Enabled")) == 0)
	{
		const auto enabled = luaL_checkboolean(ls, 3);
		if (enabled)
		{
			pInfo->enable();
		}
		else
		{
			pInfo->disable();
		}
	}

	return 0;
}

static int mt_index(lua_State* ls)
{
	LOGD("connection.__index");

	const auto pInfo = reinterpret_cast<argon::lua_objects::RBXConnection_t*>(lua_touserdata(ls, 1));
	const char* key = luaL_checkstring(ls, 2);

	bool is_l_connection = true;

	Defs::rbx_print(1, "connection: %llX");

	if (strcmp(key, xorstr_("Enabled")) == 0 || strcmp(key, xorstr_("Connected")) == 0)
	{
		lua_pushboolean(ls, pInfo->is_enabled());
		return 1;
	}
	else if (strcmp(key, xorstr_("Disable")) == 0)
	{
		lua_pushvalue(ls, 1);
		lua_pushcclosure(ls, disable_connection, nullptr, 1);
		return 1;
	}
	else if (strcmp(key, xorstr_("Enable")) == 0)
	{
		lua_pushvalue(ls, 1);
		lua_pushcclosure(ls, enable_connection, nullptr, 1);
		return 1;
	}
	else if (strcmp(key, "LuaConnection") == 0)
	{
		const auto ref = pInfo->get_function_ref();

		lua_getref(ls, ref);
		if (!lua_isfunction(ls, -1))
		{
			lua_pushboolean(ls, false);
			return 1;
		}
		else
		{
			lua_pushboolean(ls, lua_isLfunction(ls, -1));
			return 1;
		}
		lua_pushnil(ls);
		return 1;
	}
	else if (strcmp(key, "ForeignState") == 0)
	{
		LOGD("wowzers 1");
		auto th = pInfo->get_luathread();
		LOGD("wowzers 2");
		if (!th)
		{
			LOGD("failed to get lua thread");
			const auto ref = pInfo->get_thread_ref();
			lua_getref(ls, ref);
			if (!lua_isthread(ls, -1))
			{
				LOGD("fall back");
				lua_pushboolean(ls, true);
				return 1;
			}
			else
			{
				LOGD("erm");
				th = lua_tothread(ls, -1);
				lua_pop(ls, 1);

				lua_pushboolean(ls, (th->global != ls->global));
				return 1;
			}
		}

		LOGD("rizzler");
		// globalStates are only created once for multiple threads.
		// corescripts, localscripts and i forgot the other kknd of script have different globalstates
		lua_pushboolean(ls, (th->global != ls->global));
		return 1;
	} // According to UNC, if ForeignState is true then Function and Thread are nil
	else if (strcmp(key, xorstr_("Function")) == 0)
	{
		auto th = pInfo->get_luathread();
		if (!th)
		{
			const auto ref = pInfo->get_thread_ref();
			lua_getref(ls, ref);
			if (!lua_isthread(ls, -1))
			{
				lua_pushnil(ls);
				return 1;
			}
			else
			{
				th = lua_tothread(ls, -1);
				lua_pop(ls, 1);
			}
		}

		if (th->global != ls->global)
		{
			lua_pushnil(ls);
			return 1;
		}

		const auto ref = pInfo->get_function_ref();

		lua_getref(ls, ref);
		if (!lua_isfunction(ls, -1))
		{
			lua_pushnil(ls);
		}

		return 1;
	}
	else if (strcmp(key, xorstr_("Thread")) == 0)
	{
		auto th = pInfo->get_luathread();
		if (!th)
		{
			const auto ref = pInfo->get_thread_ref();
			lua_getref(ls, ref);
			if (!lua_isthread(ls, -1))
			{
				lua_pushnil(ls);
				return 1;
			}
			else
			{
				return 1;
			}
		}

		lua_pushthread(th);
		lua_xmove(th, ls, 1);

		return 1;
	}
	else if (strcmp(key, xorstr_("Fire")) == 0)
	{
		const auto ref = pInfo->get_function_ref();

		lua_getref(ls, ref);
		if (!lua_isfunction(ls, -1))
		{
			lua_pushnil(ls);
		}

		lua_pushcclosure(ls, fire_connection, nullptr, 1);
		return 1;
	}
	else if (strcmp(key, xorstr_("Defer")) == 0)
	{
		const auto ref = pInfo->get_function_ref();

		lua_getref(ls, ref);
		if (!lua_isfunction(ls, -1))
		{
			lua_pushnil(ls);
		}

		lua_pushcclosure(ls, defer_connection, nullptr, 1);
		return 1;
	}
	else if (strcmp(key, xorstr_("Disconnect")) == 0)
	{
		lua_pushvalue(ls, 1);
		lua_pushcclosure(ls, disconnect_connection, nullptr, 1);
		return 1;
	}

	return 0;
}

static int get_connections(lua_State* ls)
{
	LOGD("getconnections");

	luaL_checktype(ls, 1, LUA_TUSERDATA);
	if (strcmp(luaL_typename(ls, 1), xorstr_("RBXScriptSignal")) != 0)
	{
		luaL_typeerror(ls, 1, xorstr_("RBXScriptSignal"));
	}

	// create a stub conn to get access to the nodes easily
	lua_getfield(ls, 1, xorstr_("Connect"));
	{
		lua_pushvalue(ls, 1);
		lua_pushcfunction(ls, [](lua_State*) -> int { return 0; }, nullptr);
	}
	lua_pcall(ls, 2, 1, 0);

	// connections, theyre all related with a linked list
	const auto sigconbr = reinterpret_cast<SignalConnectionBridge*>(lua_touserdata(ls, -1));
	auto Node = sigconbr->islot->next;

	if (!TRBXScriptConnection)
	{
		TRBXScriptConnection = lua_userdatatag(ls, -1); // steal ud tag
	}

	lua_getfield(ls, -1, xorstr_("Disconnect"));
	{
		lua_insert(ls, -2);
	}
	lua_pcall(ls, 1, 0, 0);

	int idx = 1;
	lua_newtable(ls);

	while (Node)
	{
		auto conn = reinterpret_cast<argon::lua_objects::RBXConnection_t*>(lua_newuserdatatagged(ls, sizeof(argon::lua_objects::RBXConnection_t), 71));

		*conn = argon::lua_objects::RBXConnection_t(Node);

		lua_newtable(ls);
		{
			lua_pushcfunction(ls, mt_index, nullptr);
			lua_setfield(ls, -2, xorstr_("__index"));

			lua_pushcfunction(ls, mt_newindex, nullptr);
			lua_setfield(ls, -2, xorstr_("__newindex"));

			lua_pushstring(ls, xorstr_("Event"));
			lua_setfield(ls, -2, xorstr_("__type"));
		}
		lua_setmetatable(ls, -2);
		lua_rawseti(ls, -2, idx++);

		Node = Node->next;
	}

	return 1;
}

			int firesignal(lua_State* L)
			{
				LOGD("firesignal");

				int nargs = lua_gettop(L);
				lua_pushcfunction(L, get_connections, "");
				lua_pushvalue(L, 1);
				lua_pcall(L, 1, 1, 0);
				lua_pushnil(L);

				while (lua_next(L, -2)) {
					lua_getfield(L, -1, "Function");
					for (int i = 0; i < nargs - 1; i++)
						lua_pushvalue(L, i + 2);
					lua_pcall(L, nargs - 1, 0, 0);
					lua_pop(L, 1);
				}
				return 0;
			}
		}


		namespace FFlags
		{
			auto getfflag(lua_State* rl) -> int {
				LOGD("getfflag");

luaL_checktype(rl, 1, LUA_TSTRING);

std::string buffer;
std::string flag_name = lua_tostring(rl, 1);

if (reinterpret_cast<bool(__fastcall*)(bool a1, std::string * buffer, std::string * a3)>(Offsets::getfflag)(true, &buffer, &flag_name)) {
	if (buffer == "false") {
		lua_pushboolean(rl, false);
		return 1;
	}
	else if (buffer == "true") {
		lua_pushboolean(rl, true);
		return 1;
	}

	char* end = nullptr;
	double number = std::strtod(buffer.c_str(), &end);
	if (end == buffer.c_str() + buffer.size()) {
		std::stringstream ss(buffer);
		double num;
		ss >> num;

		lua_pushnumber(rl, num);
		return 1;
	}

	lua_pushstring(rl, buffer.c_str());
	return 1;
}
else
luaL_error(rl, "An error unexpected while getting FFlag.");

return 0;
			}


			auto setfflag(lua_State* rl) -> int {
				LOGD("setfflag");

				luaL_checktype(rl, 1, LUA_TSTRING);
				luaL_checkany(rl, 2);

				std::string name = lua_tostring(rl, 1);

				std::string value = "";

				if (lua_type(rl, 2) == LUA_TBOOLEAN) {
					bool resss = lua_toboolean(rl, 2);

					if (resss) {

						value = "true";
					}
					else {
						value = "false";
					}
				}
				else if (lua_type(rl, 2) == LUA_TNUMBER) {
					int erm = lua_tonumber(rl, 2);

					value = std::to_string(erm);
				}
				else if (lua_type(rl, 2) == LUA_TSTRING) {
					value = lua_tostring(rl, 2);
				}
				else {
					luaL_error(rl, "unsupported type");
					return 0;
				}


				auto flog_data_bank = *reinterpret_cast<uintptr_t*>(Offsets::flog_data_bank);
				auto result = reinterpret_cast<__int64(__fastcall*)(__int64, const char*, std::string*, __int64)>(Offsets::setfflag)(flog_data_bank, name.c_str(), &value, true);

				return 0;
			}
		}

		namespace Actors
		{
			int getactors(lua_State* L) {
				LOGD("getactors");

				std::map< uintptr_t, bool > map;

				lua_pushvalue(L, LUA_REGISTRYINDEX);

				lua_newtable(L);

				lua_pushnil(L);

				auto c = 0u;
				while (lua_next(L, -3))
				{
					if (lua_isuserdata(L, -1))
					{
						const auto userdata = lua_touserdata(L, -1);

						if (userdata)
						{
							if (!strcmp(luaL_typename(L, -1), "Instance")) {
								const auto instance = *(uintptr_t*)(userdata);
								if (instance) {
									const auto class_descriptor =*(uintptr_t*)(instance + 0x18);

									if (class_descriptor) {
										std::string class_name = **(std::string**)(class_descriptor + 0x8);

										if (!strcmp(class_name.c_str(), "Actor")) {
											if (map.find(instance) == map.end())
											{
												map.insert({ instance, true });

												Defs::rbx_pushinstance2(L, userdata);

												lua_rawseti(L, -4, ++c);
											}
										}
									}
								}
							}
						}
					}

					lua_pop(L, 1);
				}

				return 1;
			}

			int isparallel(lua_State* L)
			{

			}
		}
	}


	namespace MetaHooks
	{
		static __int64 old_namecall;
		static __int64 old_index;
		static __int64 old_newindex;

		std::vector<const char*> dangerous_functions =
		{
			"OpenVideosFolder", "OpenScreenshotsFolder",
			"GetRobuxBalance", "PerformPurchase", "PromptBundlePurchase", "PromptNativePurchase",
			"PromptProductPurchase", "PromptPurchase", "PromptThirdPartyPurchase", "Publish",
			"GetMessageId", "OpenBrowserWindow", "RequestInternal", "ExecuteJavaScript",
			"openvideosfolder", "openscreenshotsfolder", "getrobuxbalance", "performpurchase",
			"promptbundlepurchase", "promptnativepurchase", "promptproductpurchase",
			"promptpurchase", "promptthirdpartypurchase", "publish", "getmessageid",
			"openbrowserwindow", "requestinternal", "executejavascript", "openVideosFolder",
			"openScreenshotsFolder", "getRobuxBalance", "performPurchase", "promptBundlePurchase",
			"promptNativePurchase", "promptProductPurchase", "promptPurchase",
			"promptThirdPartyPurchase", "publish", "getMessageId", "openBrowserWindow",
			"requestInternal", "executeJavaScript",
			"ToggleRecording", "TakeScreenshot", "HttpRequestAsync", "GetLast",
			"SendCommand", "GetAsync", "GetAsyncFullUrl", "RequestAsync", "MakeRequest",
			"togglerecording", "takescreenshot", "httprequestasync", "getlast",
			"sendcommand", "getasync", "getasyncfullurl", "requestasync", "makerequest",
			"toggleRecording", "takeScreenshot", "httpRequestAsync", "getLast",
			"sendCommand", "getAsync", "getAsyncFullUrl", "requestAsync", "makeRequest", "openurl",
			"OpenUrl", "openUrl", "Openurl",
			"openlink",
			"OpenLink", "openLink", "Openlink",
			"SaveScriptProfilingData"
		};

		auto dangerous_function(lua_State* L) -> int
		{
			lua_pushboolean(L, false);
			lua_pushstring(L, "Disabled for security reasons.");
			return 2;
		}

		auto namecall_hook(lua_State* L) -> int
		{
			//const auto script_ptr = *reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(L->userdata) + 80);

			if (L->namecall)
			{
				std::string loweredNamecall = L->namecall->data;

				for (auto& x : loweredNamecall) {
					x = std::tolower(x); // NOLINT(*-narrowing-conversions)
				}

				if (loweredNamecall.find("httpget") != std::string::npos ||
					loweredNamecall.find("httpgetasync") != std::string::npos) {
					return Enviroment::Functions::Scripts::httpget(L);
				}

				if (loweredNamecall.find("getobjects") != std::string::npos) {
					return Enviroment::Functions::Scripts::getobjects(L);;
				}


				//for (unsigned int i = 0; i < dangerous_functions.size(); i++) {
				//	if (loweredNamecall.find(dangerous_functions[i]) != std::string::npos) {
				//		lua_pushboolean(L, false);
				//		lua_pushstring(L, xorstr_("Disabled for security reasons."));
				//		return 2;
				//	}
				//}
			}

			return reinterpret_cast<__int64(__fastcall*)(__int64)>(old_namecall)((__int64)L);
		}

		auto index_hook(lua_State* L) -> int
		{
			if (lua_isstring(L, 2 )) 
			{
				std::string key = lua_tostring(L, 2);
				std::string loweredIndex = luaL_checkstring(L, 2);

				uintptr_t instance = *(uintptr_t*)(lua_touserdata(L, 1));
				void* instance2 = lua_touserdata(L, 1);

				for (auto& x : loweredIndex) {
					x = std::tolower(x); // NOLINT(*-narrowing-conversions)
				}

				
				if (loweredIndex.find("httpget") != std::string::npos ||
					loweredIndex.find("httpgetasync") != std::string::npos) {
					lua_getglobal(L, "HttpGet");
					return 1;
				}

				if (loweredIndex.find("getobjects") != std::string::npos) {
					lua_getglobal(L, "GetObjects");
					return 1;
				}


				__int64 property = 0;
				std::vector<__int64> properties = *reinterpret_cast<std::vector<__int64>*>(*reinterpret_cast<__int64*>(instance + 24) + 40);

				for (int i = 0; i < properties.size(); i++)
				{
					std::string prop_name = *reinterpret_cast<std::string*>(*reinterpret_cast<__int64*>(properties[i] + 8));
					if (prop_name == key)
					{
						property = properties[i];
						break;
					}
				}

				if (property == NULL) {
				//	Defs::rbx_print(1, "property not found");
					return reinterpret_cast<__int64(__fastcall*)(__int64)>(old_index)((__int64)L);
				}

				//Defs::rbx_print(1, "here 1");

				if (const auto cache_scriptable = g_instance.is_scriptable(instance2, key))
				{
					//Defs::rbx_print(1, "here 2, cached_scriptable: %d", cache_scriptable.value());

					const auto scriptable = lua_isscriptable(L, property);
					//Defs::rbx_print(1, "isscriptable: %d", scriptable);

				   // Defs::rbx_print(1, "here 3");
					lua_setscriptable(L, property, *cache_scriptable);

					const auto res = reinterpret_cast<__int64(__fastcall*)(__int64)>(old_index)((__int64)L);

					lua_setscriptable(L, property, scriptable);

					return res;
				}

	/*			for (unsigned int i = 0; i < dangerous_functions.size(); i++) {
					if (loweredIndex.find(dangerous_functions[i]) != std::string::npos) {
						lua_pushcclosure(L, dangerous_function, NULL, 0);
						return 1;
					}
				}*/
			}

			return reinterpret_cast<__int64(__fastcall*)(__int64)>(old_index)((__int64)L);
		}

		auto newindex_hook(lua_State* L) -> int
		{
			if (lua_isstring(L, 2) && (!*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(L->userdata) + 80)) == true) {
				uintptr_t instance = *(uintptr_t*)(lua_touserdata(L, 1));
				void* instance2 = lua_touserdata(L, 1);

				const std::string key = luaL_checkstring(L, 2);

				__int64 property = 0;
				std::vector<__int64> properties = *reinterpret_cast<std::vector<__int64>*>(*reinterpret_cast<__int64*>(instance + 24) + 40);

				for (int i = 0; i < properties.size(); i++)
				{
					std::string prop_name = *reinterpret_cast<std::string*>(*reinterpret_cast<__int64*>(properties[i] + 8));
					if (prop_name == key)
					{
						property = properties[i];
						break;
					}
				}

				if (property == NULL)
					return reinterpret_cast<__int64(__fastcall*)(__int64)>(old_newindex)((__int64)L);

				if (const auto cache_scriptable = g_instance.is_scriptable(instance2, key))
				{
					//Defs::rbx_print(1, "here 2, cached_scriptable: %d", cache_scriptable.value());

					const auto scriptable = lua_isscriptable(L, property);
					//Defs::rbx_print(1, "isscriptable: %d", scriptable);

				   // Defs::rbx_print(1, "here 3");
					lua_setscriptable(L, property, *cache_scriptable);

					const auto res = reinterpret_cast<__int64(__fastcall*)(__int64)>(old_newindex)((__int64)L);

					lua_setscriptable(L, property, scriptable);

					return res;
				}

			}
			
			return reinterpret_cast<__int64(__fastcall*)(__int64)>(old_newindex)((__int64)L);
		}

		auto initialize(lua_State* L) -> void
		{
	
			//setupLogger();

			lua_getglobal(L, "game");
			lua_getmetatable(L, -1);
			lua_rawgetfield(L, -1, "__namecall");

			Closure* namecall = (Closure*)lua_topointer(L, -1);
			lua_CFunction namecall_f = namecall->c.f;
			old_namecall = (__int64)namecall_f;
			namecall->c.f = namecall_hook;

			lua_settop(L, 0);

			lua_getglobal(L, "game");
			lua_getmetatable(L, -1);
			lua_rawgetfield(L, -1, "__index");

			Closure* index = (Closure*)lua_topointer(L, -1);
			lua_CFunction index_f = index->c.f;
			old_index = (__int64)index_f;
			index->c.f = index_hook;

			lua_settop(L, 0);

			lua_getglobal(L, "game");
			lua_getmetatable(L, -1);
			lua_rawgetfield(L, -1, "__newindex");

			Closure* newindex = (Closure*)lua_topointer(L, -1);
			lua_CFunction newindex_f = newindex->c.f;
			old_newindex = (__int64)newindex_f;
			newindex->c.f = newindex_hook;

			lua_settop(L, 0);


		}
	}
}
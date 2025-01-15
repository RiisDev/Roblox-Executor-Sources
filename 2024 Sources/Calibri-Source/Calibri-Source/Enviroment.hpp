#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>
#include <queue>
#include <regex>
#include <map>
#include <set>

#include "crypt.h"

#include "Utils.hpp"
#include "Roblox.hpp"

#include "Compiler/include/BytecodeBuilder.h"
#include "Compiler/include/Compiler.h"


namespace Enviroment
{
	static std::uintptr_t runtime_script_service;
	static __int64 calibri_state = 0;
	static std::vector<Closure*> function_array;
	static std::map<Closure*, Closure*> newcclosure_map;

	enum type
	{
		roblox_c_closure,
		module_c_closure,
		module_c_wrap,
		l_closure,
		not_set
	};


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
		lua_setfield(rl, -2, globalname);
	}

	namespace Functions
	{
		namespace Env
		{
			auto getgenv(lua_State* rl) -> int
			{
				lua_pushvalue(rl, LUA_ENVIRONINDEX);
				return 1;
			}

			auto getreg(lua_State* rl) -> int
			{
				lua_pushvalue(rl, LUA_REGISTRYINDEX);
				return 1;
			}

			auto getrenv(lua_State* rl) -> int
			{
				setobj(rl, rl->top, index2addr(rl->global->mainthread, LUA_GLOBALSINDEX));
				incr_top(rl);
				return 1;
			}

			auto getgc(lua_State* state) -> int
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
			}


			auto getttenv(lua_State* rl) -> int
			{
				luaL_checktype(rl, 1, LUA_TTHREAD);
				lua_State* ls = (lua_State*)lua_topointer(rl, 1);
				Table* tab = hvalue(luaA_toobject(ls, LUA_GLOBALSINDEX));

				sethvalue(rl, rl->top, tab);
				rl->top++;

				return 1;
			}
		}


		namespace Closures
		{
			auto is_executor_closure(lua_State* rl) -> int
			{
				luaL_checktype(rl, 1, LUA_TFUNCTION);
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

			auto iscclosure(lua_State* rl) -> int
			{
				luaL_checktype(rl, 1, LUA_TFUNCTION);

				lua_pushboolean(rl, lua_iscfunction(rl, 1));
				return 1;
			}

			auto islclosure(lua_State* rl) -> int
			{
				luaL_checktype(rl, 1, LUA_TFUNCTION);

				lua_pushboolean(rl, lua_isLfunction(rl, 1));
				return 1;
			}

			auto checkcaller(lua_State* rl) -> int
			{
				auto state = (__int64)rl;
				uintptr_t userdata = *reinterpret_cast<uintptr_t*>(state + 120);
				int level = *reinterpret_cast<uintptr_t*>(userdata + 0x30);

				lua_pushboolean(rl, (bool)(level >= 3));
				return 1;
			}

			auto loadstring(lua_State* rl) -> int
			{
				size_t l = 0;
				auto s = luaL_checklstring(rl, 1, &l);

				auto bytecode = Utils::compress_bytecode(Luau::compile(std::string(s, l)));

				if (Defs::rbx_luavmload((__int64)rl, &bytecode, std::string("@" + lua_tostring(rl, 2)).c_str(), 0) == 0)
				{
					auto function = *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>((__int64)rl + Offsets::LuaState::top) - 16);
					auto proto = ProtoThings::deobfuscate_proto(function);
					ProtoThings::raise_proto(proto);

					return 1;
				}

				lua_pushnil(rl);
				lua_insert(rl, -2);
				return 2;
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

			auto clonefunction(lua_State* rl) -> int
			{
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
				luaL_checktype(rl, 1, LUA_TFUNCTION);
				luaL_checktype(rl, 2, LUA_TFUNCTION);

				const auto cl1 = clvalue(index2addr(rl, 1));
				const auto cl2 = clvalue(index2addr(rl, 2));
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
					const auto& spoof = Luau::compile("local f = function() end; return f(...); ");
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
					luaL_error(rl, "Failed to hook");

				return 1;
			}
		}

		namespace Input
		{
			int isrbxactive(lua_State* rl)
			{
				HWND window;
				window = FindWindowA(NULL, "Roblox");
				lua_pushboolean(rl, GetForegroundWindow() == window);
				return 1;
			}


			int mouse1press(lua_State* rl)
			{
				HWND window = FindWindowA(NULL, "Roblox");
				if (GetForegroundWindow() == window)
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				return 0;
			}


			int mouse1release(lua_State* rl)
			{
				HWND window = FindWindowA(NULL, "Roblox");
				if (GetForegroundWindow() == window)
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				return 0;
			}


			int mouse1click(lua_State* rl)
			{
				HWND window = FindWindowA(NULL, "Roblox");
				if (GetForegroundWindow() == window)
					mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				return 0;
			}


			int mouse2press(lua_State* rl)
			{
				HWND window = FindWindowA(NULL, "Roblox");
				if (GetForegroundWindow() == window)
					mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
				return 0;
			}


			int mouse2release(lua_State* rl)
			{
				HWND window = FindWindowA(NULL, "Roblox");
				if (GetForegroundWindow() == window)
					mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
				return 0;
			}


			int mouse2click(lua_State* rl)
			{
				HWND window = FindWindowA(NULL, "Roblox");
				if (GetForegroundWindow() == window)
					mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
				return 0;
			}


			int keypress(lua_State* rl)
			{
				HWND window = FindWindowA(NULL, "Roblox");
				UINT key = luaL_checkinteger(rl, 1);

				if (GetForegroundWindow() == window)
					keybd_event(0, (BYTE)MapVirtualKeyA(key, MAPVK_VK_TO_VSC), KEYEVENTF_SCANCODE, 0);
				return 0;
			}


			int iskeydown(lua_State* rl)
			{
				luaL_checktype(rl, 1, LUA_TNUMBER);

				UINT key = luaL_checkinteger(rl, 1);

				SHORT state = GetAsyncKeyState(key);
				lua_pushboolean(rl, (bool)((state & 0x8000) != 0));
				return 1;
			}


			int keyrelease(lua_State* rl)
			{
				HWND window = FindWindowA(NULL, "Roblox");
				UINT key = luaL_checkinteger(rl, 1);

				if (GetForegroundWindow() == window)
					keybd_event(0, (BYTE)MapVirtualKeyA(key, MAPVK_VK_TO_VSC), KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP, 0);
				return 0;
			}


			int mousemoverel(lua_State* rl)
			{
				HWND window = FindWindowA(NULL, "Roblox");

				DWORD x = luaL_checkinteger(rl, 1);
				DWORD y = luaL_checkinteger(rl, 2);

				if (GetForegroundWindow() == window)
					mouse_event(MOUSEEVENTF_MOVE, x, y, 0, 0);
				return 0;
			}


			int mousemoveabs(lua_State* rl)
			{
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
				HWND window = FindWindowA(NULL, "Roblox");
				DWORD scroll_amount = luaL_checkinteger(rl, 1);

				if (GetForegroundWindow() == window)
					mouse_event(MOUSEEVENTF_WHEEL, 0, 0, scroll_amount, 0);
				return 0;
			}
		}

		namespace Filesystem
		{
			auto readfile(lua_State* rl) -> int
			{
				luaL_checktype(rl, 1, LUA_TSTRING);

				std::string filename = lua_tostring(rl, 1);
				auto filepath = Utils::Workspace::get() / filename;

				if (filepath.string().find("..") != std::string::npos)
					luaL_error(rl, "Attempt to exit directory.");


				std::ifstream f(filepath, std::ios::in | std::ios::binary);
				const auto sz = std::filesystem::file_size(filepath);
				std::string result(sz, '\0');
				f.read(result.data(), sz);

				lua_pushstring(rl, result.c_str());
				return 1;
			}


			auto writefile(lua_State* rl) -> int
			{
				luaL_checktype(rl, 1, LUA_TSTRING);
				luaL_checktype(rl, 2, LUA_TSTRING);

				std::string filename = lua_tostring(rl, 1);
				std::string content = lua_tostring(rl, 2);
				auto filepath = Utils::Workspace::get() / filename;

				if (filepath.string().find("..") != std::string::npos)
					luaL_error(rl, "Attempt to exit directory.");

				std::vector<std::string> disallowed =
				{
					".exe", ".scr", ".bat", ".com", ".csh", ".msi", ".vb", ".vbs", ".vbe", ".ws", ".wsf", ".wsh", ".ps1"
				};

				for (std::string& current : disallowed)
					if (filepath.string().find(current) != std::string::npos)
						luaL_error(rl, "Disallowed type.");


				std::ofstream output;
				output.open(filepath, std::ios::out | std::ios::binary);
				output.write(content.data(), content.length());
				output.close();

				return 0;
			}


			auto makefolder(lua_State* rl) -> int
			{
				luaL_checktype(rl, 1, LUA_TSTRING);

				std::string foldername = lua_tostring(rl, 1);
				auto path = Utils::Workspace::get() / foldername;

				if (path.string().find("..") != std::string::npos)
					luaL_error(rl, "Attempt to exit directory.");

				std::filesystem::create_directories(path);
				return 0;
			}


			auto appendfile(lua_State* rl) -> int
			{
				luaL_checktype(rl, 1, LUA_TSTRING);
				luaL_checktype(rl, 2, LUA_TSTRING);

				std::string filename = lua_tostring(rl, 1);
				std::string content = lua_tostring(rl, 2);
				auto filepath = Utils::Workspace::get() / filename;

				if (filepath.string().find("..") != std::string::npos)
					luaL_error(rl, "Attempt to exit directory.");

				std::vector<std::string> disallowed =
				{
					".exe", ".scr", ".bat", ".com", ".csh", ".msi", ".vb", ".vbs", ".vbe", ".ws", ".wsf", ".wsh", ".ps1"
				};

				for (std::string& current : disallowed)
					if (filepath.string().find(current) != std::string::npos)
						luaL_error(rl, "Disallowed type.");

				std::ofstream output;
				output.open(filepath, std::ios_base::app | std::ios_base::binary);
				output.write(content.data(), content.length());
				output.close();

				return 0;
			}


			auto isfile(lua_State* rl) -> int
			{
				luaL_checktype(rl, 1, LUA_TSTRING);

				std::string filename = lua_tostring(rl, 1);
				auto filepath = Utils::Workspace::get() / filename;

				lua_pushboolean(rl, std::filesystem::is_regular_file(filepath));
				return 1;
			}


			auto isfolder(lua_State* rl) -> int
			{
				luaL_checktype(rl, 1, LUA_TSTRING);

				std::string filename = lua_tostring(rl, 1);
				auto filepath = Utils::Workspace::get() / filename;

				lua_pushboolean(rl, std::filesystem::is_directory(filepath));
				return 1;
			}


			auto delfile(lua_State* rl) -> int
			{
				luaL_checktype(rl, 1, LUA_TSTRING);

				std::string filename = lua_tostring(rl, 1);
				auto filepath = Utils::Workspace::get() / filename;

				if (filepath.string().find("..") != std::string::npos)
					luaL_error(rl, "Attempt to exit directory.");

				if (!std::filesystem::remove(filepath))
					luaL_error(rl, "Not exists.");

				return 0;
			}


			auto delfolder(lua_State* rl) -> int
			{
				luaL_checktype(rl, 1, LUA_TSTRING);

				std::string filename = lua_tostring(rl, 1);
				auto filepath = Utils::Workspace::get() / filename;

				if (filepath.string().find("..") != std::string::npos)
					luaL_error(rl, "Attempt to exit directory.");

				if (!std::filesystem::remove_all(filepath))
					luaL_error(rl, "Not exists.");

				return 0;
			}


			auto listfiles(lua_State* rl) -> int
			{
				luaL_checktype(rl, 1, LUA_TSTRING);

				std::string foldername = lua_tostring(rl, 1);
				auto path = Utils::Workspace::get() / foldername;

				if (path.string().find("..") != std::string::npos)
					luaL_error(rl, "Attempt to exit directory.");

				lua_createtable(rl, 0, 0);
				auto count = 0;

				for (auto& file_iter : std::filesystem::directory_iterator(path))
				{
					lua_pushnumber(rl, ++count);
					lua_pushstring(rl, file_iter.path().string().substr((Utils::Workspace::get().string()).length() + 1).c_str());
					lua_settable(rl, -3);
				}

				return 1;
			}
		}


		namespace Cache
		{
			auto invalidate(lua_State* rl) -> int
			{
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
				luaL_checktype(rl, 1, LUA_TUSERDATA);

				const auto original_userdata = lua_touserdata(rl, 1);
				const auto returned_userdata = *reinterpret_cast<std::uintptr_t*>(original_userdata);

				lua_pushlightuserdata(rl, reinterpret_cast<void*>(Offsets::pushinstance));

				lua_rawget(rl, -10000);
				lua_pushlightuserdata(rl, reinterpret_cast<void*>(returned_userdata));
				lua_rawget(rl, -2);

				lua_pushlightuserdata(rl, reinterpret_cast<void*>(returned_userdata));
				lua_pushnil(rl);
				lua_rawset(rl, -4);

				reinterpret_cast<void(__fastcall*)(lua_State*, void*)>(Offsets::pushinstance)(rl, lua_touserdata(rl, 1));

				lua_pushlightuserdata(rl, reinterpret_cast<void*>(returned_userdata));
				lua_pushvalue(rl, -3);
				lua_rawset(rl, -5);

				return 1;
			}


			auto compareinstances(lua_State* rl) -> int
			{
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
			int hookmetamethod(lua_State* state)
			{
				if (!lua_isuserdata(state, 1) && !lua_istable(state, 1))
					luaL_argerror(state, 1, "Hookmetamethod - wrong arguments.");

				const auto metamethod = luaL_checklstring(state, 2, nullptr);

				luaL_checktype(state, 3, LUA_TFUNCTION);
				lua_getfield(state, -10002, "hookfunction");

				if (!luaL_getmetafield(state, 1, metamethod))
					luaL_error(state, "Metatable for instance could not be found.");

				lua_pushvalue(state, 3);
				lua_pcall(state, 2, 1, 0);
				return 1;
			}


			auto getrawmetatable(lua_State* rl) -> int
			{
				lua_getmetatable(rl, 1);
				return 1;
			}


			auto setrawmetatable(lua_State* rl) -> int
			{
				lua_setmetatable(rl, 1);
				return 0;
			}


			auto isreadonly(lua_State* rl) -> int
			{
				lua_pushboolean(rl, lua_getreadonly(rl, 1));
				return 1;
			}


			auto getnamecallmethod(lua_State* rl) -> int
			{
				if (auto namecall = rl->namecall)
				{
					lua_pushstring(rl, namecall->data);
					return 1;
				}
				return 0;
			}


			auto setnamecallmethod(lua_State* rl) -> int
			{
				rl->namecall = reinterpret_cast<TString*>(index2addr(rl, 1)->value.p);
				return 0;
			}


			auto setreadonly(lua_State* rl) -> int
			{
				luaL_checktype(rl, 2, LUA_TBOOLEAN);

				lua_setreadonly(rl, 1, lua_toboolean(rl, 2));
				return 0;
			}


			auto make_writable(lua_State* rl) -> int
			{

				lua_setreadonly(rl, 1, false);
				return 0;
			}


			auto make_readonly(lua_State* rl) -> int
			{
				lua_setreadonly(rl, 1, true);
				return 0;
			}
		}

		namespace Crypt
		{
			static int base64encode(lua_State* rl)
			{
				luaL_checktype(rl, 1, LUA_TSTRING);

				const auto& encoded = ::crypt::base64::encode(lua_tostring(rl, 1));

				lua_pushlstring(rl, encoded.data(), encoded.length());
				return 1;
			}


			static int base64decode(lua_State* rl)
			{
				luaL_checktype(rl, 1, LUA_TSTRING);

				const auto& decoded = ::crypt::base64::decode(lua_tostring(rl, 1));

				lua_pushlstring(rl, decoded.data(), decoded.length());
				return 1;
			}


			static int encrypt(lua_State* rl)
			{
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
				luaL_checktype(rl, 1, LUA_TNUMBER);

				const auto& bytes = ::crypt::generatebytes(lua_tointeger(rl, 1));

				lua_pushlstring(rl, bytes.data(), bytes.length());
				return 1;
			}


			static int generatekey(lua_State* rl)
			{
				const auto& key = ::crypt::generatebytes(32);

				lua_pushlstring(rl, key.data(), key.length());
				return 1;
			}


			static int hash(lua_State* rl)
			{
				luaL_checktype(rl, 1, LUA_TSTRING);
				luaL_checktype(rl, 2, LUA_TSTRING);

				const auto& hash = ::crypt::hash(lua_tostring(rl, 1), lua_tostring(rl, 2));

				lua_pushlstring(rl, hash.data(), hash.length());
				return 1;
			}
		}

		namespace Debug
		{
			static int debug_getupvalue(lua_State* rl)
			{
				luaL_checktype(rl, 1, LUA_TFUNCTION);

				int level;
				if (lua_isnumber(rl, 1))
				{
					level = lua_tointeger(rl, 1);
					luaL_argcheck(rl, level >= 0, 1, "level can't be negative");
				}
				else if (lua_isfunction(rl, 1))
				{
					level = lua_tointeger(rl, 1);
				}
				else
					luaL_argerror(rl, 1, "function or level expected");

				lua_Debug ar;
				if (lua_getinfo(rl, level, "f", &ar) == 0)
					luaL_argerror(rl, 1, "invalid level");

				const auto closure = *reinterpret_cast<Closure**>(index2addr(rl, 1));

				if (!closure->isC)
				{
					const auto idx = lua_tointeger(rl, 2);

					if (idx < 1 || idx > closure->nupvalues)
						luaL_error(rl, "upvalue index out of range");

					lua_getupvalue(rl, 1, idx);

					return 1;
				}

				return 0;
			}


			static int debug_getupvalues(lua_State* rl)
			{
				luaL_checktype(rl, 1, LUA_TFUNCTION);

				int level;
				if (lua_isnumber(rl, 1))
				{
					level = lua_tointeger(rl, 1);
					luaL_argcheck(rl, level >= 0, 1, "level can't be negative");
				}
				else if (lua_isfunction(rl, 1))
				{
					level = lua_tointeger(rl, 1);
				}
				else
					luaL_argerror(rl, 1, "function or level expected");

				lua_Debug ar;
				if (lua_getinfo(rl, level, "f", &ar) == 0)
					luaL_argerror(rl, 1, "invalid level");

				const auto closure = *reinterpret_cast<Closure**>(index2addr(rl, 1));

				if (!closure->isC)
				{
					lua_createtable(rl, 0, 0);

					for (auto i = 0u; i < closure->nupvalues; ++i)
					{
						lua_getupvalue(rl, 1, i + 1);
						lua_rawseti(rl, -2, i + 1);
					}

					return 1;
				}

				return 0;
			}


			static int debug_setupvalue(lua_State* rl)
			{
				luaL_checktype(rl, 1, LUA_TFUNCTION);

				int level;
				if (lua_isnumber(rl, 1))
				{
					level = lua_tointeger(rl, 1);
					luaL_argcheck(rl, level >= 0, 1, "level can't be negative");
				}
				else if (lua_isfunction(rl, 1))
				{
					level = lua_tointeger(rl, 1);
				}
				else
					luaL_argerror(rl, 1, "function or level expected");

				lua_Debug ar;
				if (lua_getinfo(rl, level, "f", &ar) == 0)
					luaL_argerror(rl, 1, "invalid level");

				const auto closure = *reinterpret_cast<Closure**>(index2addr(rl, 1));

				if (!closure->isC)
				{
					const auto idx = lua_tointeger(rl, 2);

					if (idx < 1 || idx > closure->nupvalues)
						luaL_error(rl, "upvalue index out of range");

					lua_pushvalue(rl, 3);
					lua_setupvalue(rl, 1, idx);
				}

				return 0;
			}


			static int debug_getconstant(lua_State* rl)
			{
				luaL_checktype(rl, 1, LUA_TFUNCTION);

				const auto closure = *reinterpret_cast<Closure**>(index2addr(rl, 1));

				if (!closure->isC)
				{
					const std::uint32_t idx = lua_tonumber(rl, 2) - 1;
					const auto constant = closure->l.p->k[idx];

					if (constant.tt == LUA_TTABLE || constant.tt == LUA_TFUNCTION)
						lua_pushnil(rl);
					else
					{
						rl->top->value = constant.value;
						rl->top->tt = constant.tt;

						rl->top++;
					}
				}
				else
					luaL_error(rl, "expected <lclosure> arg1");

				return 1;
			}


			static int debug_getconstants(lua_State* rl)
			{
				luaL_checktype(rl, 1, LUA_TFUNCTION);

				const auto closure = clvalue(index2addr(rl, 1));

				if (!closure->isC)
				{
					lua_newtable(rl);

					for (auto i = 0; i < closure->l.p->sizek; ++i)
					{
						const auto constant = closure->l.p->k[i];

						if (constant.tt == LUA_TTABLE || constant.tt == LUA_TFUNCTION)
							lua_pushnil(rl);
						else
						{
							setobj(rl, rl->top, &constant);
							rl->top++;
						}

						lua_rawseti(rl, -2, i + 1);
					}
				}
				else
					luaL_error(rl, "expected <lclosure> arg1");

				return 1;
			}


			static int debug_setconstant(lua_State* rl)
			{
				luaL_checktype(rl, 1, LUA_TFUNCTION);

				const auto closure = *reinterpret_cast<Closure**>(index2addr(rl, 1));

				if (!closure->isC)
				{
					const std::uint32_t idx = lua_tonumber(rl, 2) - 1;
					const auto constant = &closure->l.p->k[idx];

					setobj(rl, constant, index2addr(rl, 3));
				}

				return 0;
			}


			static int debug_getinfo(lua_State* rl)
			{
				lua_Debug debug_info;
				std::size_t size;
				const auto optional_arg = luaL_optlstring(rl, 2, "flnas", &size);
				auto debug_options = std::string(optional_arg, size);

				std::int32_t level;
				if (lua_isnumber(rl, 1))
				{
					level = lua_tointeger(rl, 1);
					luaL_argcheck(rl, level >= 0, 1, "level can't be negative");
				}
				else if (lua_isfunction(rl, 1))
					level = lua_tointeger(rl, 1);
				else
					luaL_argerror(rl, 1, "function or level expected");

				if (!lua_getinfo(rl, level, debug_options.data(), &debug_info))
					luaL_error(rl, "getinfo failed");

				lua_newtable(rl);

				for (std::int8_t c : debug_options)
				{
					switch (c)
					{
					case 'a':
					{
						lua_pushnumber(rl, debug_info.isvararg);
						lua_setfield(rl, -2, ("is_vararg"));

						lua_pushinteger(rl, debug_info.nparams);
						lua_setfield(rl, -2, ("numparams"));

						lua_pushnumber(rl, debug_info.nupvals);
						lua_setfield(rl, -2, ("nups"));
						break;
					}
					case 'f':
					{
						lua_pushvalue(rl, -2);
						lua_setfield(rl, -2, ("func"));
						break;
					}
					case 'l':
					{
						lua_pushnumber(rl, debug_info.currentline);
						lua_setfield(rl, -2, ("currentline"));
						break;
					}
					case 'n':
					{
						lua_pushstring(rl, debug_info.name ? debug_info.name : "");
						lua_setfield(rl, -2, ("name"));
						break;
					}
					case 's':
					{
						lua_pushstring(rl, debug_info.source);
						lua_setfield(rl, -2, ("source"));

						lua_pushstring(rl, debug_info.short_src);
						lua_setfield(rl, -2, ("short_src"));

						lua_pushstring(rl, debug_info.what);
						lua_setfield(rl, -2, ("what"));
						break;
					}
					default:
						luaL_error(rl, "invalid debug option");
						break;
					}
				}

				return 1;
			}


			static int debug_getstack(lua_State* rl)
			{
				luaL_checktype(rl, 1, LUA_TNUMBER);

				const auto level = lua_tointeger(rl, 1);
				const auto index = luaL_optinteger(rl, 2, -1);

				lua_newtable(rl);

				if (level >= rl->ci - rl->base_ci || level < 0)
					luaL_argerror(rl, 1, "level out of range");

				const auto frame = reinterpret_cast<CallInfo*>(rl->ci - level);
				std::size_t top = (frame->top - frame->base);

				if (clvalue(frame->func)->isC)
					luaL_argerror(rl, 1, "level points to a cclosure, lclosure expected");

				if (index == -1)
				{
					for (int iter = 0; iter < top; iter++)
					{
						setobj2s(rl, rl->top, &frame->base[iter]);
						rl->top++;

						lua_rawseti(rl, -2, iter + 1);
					}
				}
				else
				{
					if (index < 1 || index > top)
						luaL_argerror(rl, 2, "index out of range");

					setobj2s(rl, rl->top, &frame->base[index - 1]);
					rl->top++;
				}

				return 1;
			}


			static int debug_setstack(lua_State* rl)
			{
				luaL_checktype(rl, 1, LUA_TNUMBER);
				luaL_checktype(rl, 2, LUA_TNUMBER);
				luaL_checkany(rl, 3);

				const auto level = lua_tointeger(rl, 1);
				const auto index = lua_tointeger(rl, 2);

				if (level >= rl->ci - rl->base_ci || level < 0)
					luaL_argerror(rl, 1, "level out of range");

				const auto frame = reinterpret_cast<CallInfo*>(rl->ci - level);
				std::size_t top = (frame->top - frame->base);

				if (clvalue(frame->func)->isC)
					luaL_argerror(rl, 1, "level points to a cclosure, lclosure expected");

				if (index < 1 || index > top)
					luaL_argerror(rl, 2, "index out of range");

				setobj2s(rl, &frame->base[index - 1], index2addr(rl, 3));

				return 0;
			}


			static int debug_getproto(lua_State* rl)
			{
				luaL_checktype(rl, 1, LUA_TFUNCTION);

				const auto closure = *reinterpret_cast<Closure**>(index2addr(rl, 1));
				bool reximode = luaL_optboolean(rl, 3, false);

				if (!closure->isC)
				{
					if (reximode)
						lua_newtable(rl);

					const std::uint32_t idx = lua_tonumber(rl, 2) - 1;
					const auto proto = closure->l.p->p[idx];

					setclvalue(rl, rl->top, luaF_newLclosure(rl, proto->nups, closure->env, proto));
					rl->top++;

					if (reximode)
						lua_rawseti(rl, -2, 1);
				}
				else
					luaL_error(rl, "expected <lclosure> arg1");

				return 1;
			}


			static int debug_getprotos(lua_State* rl)
			{
				luaL_checktype(rl, 1, LUA_TFUNCTION);

				const auto closure = *reinterpret_cast<Closure**>(index2addr(rl, 1));

				if (!closure->isC)
				{
					lua_createtable(rl, 0, 0);

					for (auto i = 0u; i < closure->l.p->sizep; ++i)
					{
						const auto proto = closure->l.p->p[i];

						setclvalue(rl, rl->top, luaF_newLclosure(rl, proto->nups, closure->env, proto));
						rl->top++;

						lua_rawseti(rl, -2, i + 1);
					}
				}
				else
					luaL_error(rl, "expected <lclosure> arg1");

				return 1;
			}
		}

		namespace Scripts
		{
			auto setcontext(lua_State* rl) -> int
			{
				int level = lua_tointeger(rl, 1);

				auto state = (__int64)rl;
				uintptr_t userdata = *reinterpret_cast<uintptr_t*>(state + 120);

				*reinterpret_cast<uintptr_t*>(userdata + 0x30) = level;
				*reinterpret_cast<uintptr_t*>(userdata + 0x48) = max_capabilities;

				return 0;
			}


			auto getcontext(lua_State* rl) -> int
			{
				auto state = (__int64)rl;
				uintptr_t userdata = *reinterpret_cast<uintptr_t*>(state + 120);

				lua_pushinteger(rl, *reinterpret_cast<uintptr_t*>(userdata + 0x30));
				return 1;
			}

			auto getrunningscripts(lua_State* rl) -> int
			{
				int i = 0;
				lua_newtable(rl);

				for (const auto& scripts : *reinterpret_cast<std::set<std::weak_ptr<std::uintptr_t>>*>(runtime_script_service + 216))
				{
					if (!scripts.expired())
					{
						auto lock = scripts.lock();
						Defs::rbx_e_pushinstance((__int64)rl, lock);
						lua_rawseti(rl, -2, i++);
					}
				}
				return 1;
			}

			auto getcallingscript(lua_State* rl) -> int
			{
				uintptr_t userdata = *reinterpret_cast<uintptr_t*>((__int64)rl + 120);
				__int64 scriptptr = *reinterpret_cast<uintptr_t*>(userdata + 0x50);

				if (scriptptr > 0)
					Defs::rbx_pushinstance((__int64)rl, reinterpret_cast<__int64*>(userdata + 0x50));
				else
					lua_pushnil(rl);

				return 1;
			}

			auto getloadedmodules(lua_State* rl) -> int
			{
				std::vector<std::shared_ptr<uintptr_t>> instances;

				const auto loaded_modules = *reinterpret_cast<std::set<std::weak_ptr<uintptr_t>>*>(TaskScheduler::get_script_context() + 1400 + 64);
				for (auto module : loaded_modules)
				{
					if (!module.expired())
					{
						instances.push_back(module.lock());
					}
				}

				lua_newtable(rl);

				for (int i = 0; i < instances.size(); i++)
				{
					lua_pushinteger(rl, i + 1);
					Defs::rbx_e_pushinstance((__int64)rl, instances[i]);
					lua_settable(rl, -3);
				}

				return 1;
			}

			auto require(lua_State* rl) -> int
			{
				luaL_checktype(rl, 1, LUA_TUSERDATA);

				uintptr_t instance = *reinterpret_cast<uintptr_t*>(lua_touserdata(rl, 1));
				const char* class_name = *reinterpret_cast<const char**>(*reinterpret_cast<uintptr_t*>(instance + 24) + 8);

				if (class_name != "ModuleScript")
					luaL_error(rl, "Attempt to require a non-ModuleScript.");

				auto thread = (__int64)rl;
				uintptr_t userdata = *reinterpret_cast<uintptr_t*>(thread + 120);

				int old_level = *reinterpret_cast<uintptr_t*>(userdata + 0x30);

				*reinterpret_cast<uintptr_t*>(userdata + 0x30) = 8;
				*reinterpret_cast<uintptr_t*>(userdata + 0x48) = max_capabilities;

				lua_getglobal(rl, "require");
				lua_pushvalue(rl, 1);
				if (lua_pcall(rl, 1, 1, 0) != LUA_OK) {
					const char* error_msg = lua_tostring(rl, -1).c_str();
					*reinterpret_cast<uintptr_t*>(userdata + 0x30) = old_level;
					*reinterpret_cast<uintptr_t*>(userdata + 0x48) = max_capabilities;
					luaL_error(rl, error_msg);
					return 0;
				}

				*reinterpret_cast<uintptr_t*>(userdata + 0x30) = old_level;
				*reinterpret_cast<uintptr_t*>(userdata + 0x48) = max_capabilities;

				return 1;
			}


			auto gethui(lua_State* rl) -> int
			{
				lua_getfield(rl, LUA_GLOBALSINDEX, "game");
				lua_getfield(rl, -1, "CoreGui");
				return 1;
			}


			auto setclipboard(lua_State* rl) -> int
			{
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
				OpenClipboard(NULL);
				std::string clipboard = reinterpret_cast<char*>(GetClipboardData(CF_TEXT));

				lua_pushlstring(rl, clipboard.data(), clipboard.length());
				return 1;
			}


			auto httpget(lua_State* rl) -> int
			{
				luaL_checktype(rl, 2, LUA_TSTRING);

				double oofjgubqjulniz = 27474;
				double fmfzkqrycpvsu = 17683;
				bool tyylwkmbovrsgv = false;
				std::string tsbvnqqhpgid = "ylxllqtlqivyiewppdspxwruqzxrezykbvzwlqfynjytyxcrzaht";
				int lmtvjnprifq = 603;

				auto url = lua_tostring(rl, 2);

				std::string rblygbzzty = "jqxnrabtfncjfsywhavzxfxzygbjertfwcktutqzpnjn";
				double yhweysv = 12520;
				bool bvwxizeshxiuiu = false;
				bool uduwelfuwiko = true;
				bool xeijpbtmf = false;
				double hzpbonibqgawg = 26229;
				std::string zpqodpf = "aimmzefnfuddlmdfltqiorhnhefvfghzbzxkxluqxymnkxcnzenhrkmpnolzivtpmorzvlakhxswwzvauzw";
				if (false != false) {
					int bewak;
					for (bewak = 85; bewak > 0; bewak--) {
						continue;
					}
				}

				lua_pushstring(rl, Utils::download_string(url).c_str());
				return 1;
			}

			int request(lua_State* rl)
			{
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

				lua_getfield(rl, 1, "Headers");
				bool hasHeaders = lua_istable(rl, -1);
				std::string headers;
				if (hasHeaders)
				{
					lua_pushnil(rl);
					while (lua_next(rl, -2))
					{
						headers += luaL_checkstring(rl, -2);
						headers += ": ";
						headers += luaL_checkstring(rl, -1);
						headers += "\r\n";
						lua_pop(rl, 1);
					}
				}
				lua_pop(rl, 1);

				lua_getfield(rl, 1, "Cookies");
				bool hasCookies = lua_istable(rl, -1);
				std::string cookies;
				if (hasCookies)
				{
					lua_pushnil(rl);
					while (lua_next(rl, -2))
					{
						cookies += luaL_checkstring(rl, -2);
						cookies += "=";
						cookies += luaL_checkstring(rl, -1);
						cookies += "; ";
						lua_pop(rl, 1);
					}
				}
				lua_pop(rl, 1);

				headers += "User-Agent: Calibri\r\n";

				HW_PROFILE_INFO hwProfileInfo;
				if (!GetCurrentHwProfile(&hwProfileInfo))
					luaL_error(rl, "Invalid HWID.");

				std::string hwid = hwProfileInfo.szHwProfileGuid;
				Utils::replace(hwid, "}", "");
				Utils::replace(hwid, "{", "");

				std::string fingerprint = ::crypt::hash(hwid, "sha256");
				headers += "Calibri-Fingerprint: " + fingerprint + "\r\n";
				headers += "Calibri-Fingerprint: " + fingerprint + "\r\n";

				HINTERNET hSession = InternetOpen("Calibri", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
				if (!hSession)
					luaL_error(rl, "Failed to open Internet session.");

				HINTERNET hConnect = InternetOpenUrl(hSession, url, headers.c_str(), headers.length(), INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0);
				if (!hConnect)
				{
					InternetCloseHandle(hSession);
					luaL_error(rl, "Failed to connect to URL.");
				}

				DWORD statusCode = 0;
				DWORD length = sizeof(DWORD);

				if (!HttpQueryInfo(hConnect, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &length, NULL))
				{
					InternetCloseHandle(hConnect);
					InternetCloseHandle(hSession);
					luaL_error(rl, "Failed to retrieve status code.");
				}

				lua_newtable(rl);

				std::string responseText;
				char buffer[4096];
				DWORD bytesRead;
				while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead != 0)
				{
					responseText.append(buffer, bytesRead);
				}

				lua_pushlstring(rl, responseText.data(), responseText.length());
				lua_setfield(rl, -2, "Body");

				lua_pushinteger(rl, statusCode);
				lua_setfield(rl, -2, "StatusCode");

				lua_pushboolean(rl, statusCode >= 200 && statusCode < 300);
				lua_setfield(rl, -2, "Success");

				lua_newtable(rl);
				if (hasHeaders)
				{
					lua_pushlstring(rl, "Headers", 7);
					lua_newtable(rl);
					lua_pushlstring(rl, headers.c_str(), headers.length());
					lua_setfield(rl, -2, "AllHeaders");
					lua_settable(rl, -3);
				}
				lua_setfield(rl, -2, "Headers");

				InternetCloseHandle(hConnect);
				InternetCloseHandle(hSession);

				return 1;
			}


			auto setfps(lua_State* rl) -> int
			{
				luaL_checktype(rl, 1, LUA_TNUMBER);

				double fps = lua_tonumber(rl, 1);
				if (fps <= 0)
					TaskScheduler::set_fps(100);
				else
					TaskScheduler::set_fps(fps);

				return 0;
			}


			int messagebox(lua_State* rl) {
				const char* text = lua_tostring(rl, 1).c_str();
				const char* caption = lua_tostring(rl, 2).c_str();

				MessageBoxA(NULL, text, caption, NULL);
				return 0;
			}


			auto getfps(lua_State* rl) -> int
			{
				lua_pushnumber(rl, TaskScheduler::get_fps());
				return 1;
			}


			auto getobjects(lua_State* rl) -> int
			{
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
				luaL_checktype(rl, 1, LUA_TUSERDATA);
				luaL_checktype(rl, 2, LUA_TUSERDATA);
				luaL_checktype(rl, 3, LUA_TNUMBER);

				uintptr_t part = *(uintptr_t*)lua_touserdata(rl, 1);
				uintptr_t transmitter = *(uintptr_t*)lua_touserdata(rl, 2);
				int state = lua_tointeger(rl, 3);

				uintptr_t part_primitive = *reinterpret_cast<uintptr_t*>(part + 344);
				uintptr_t transmitter_primitive = *reinterpret_cast<uintptr_t*>(transmitter + 344);

				if (!part_primitive || !transmitter_primitive || *(uintptr_t*)(part_primitive + 464) != *(uintptr_t*)(transmitter_primitive + 464))
					luaL_error(rl, "new overlap in different world.");

				Defs::rbx_touch(*reinterpret_cast<uintptr_t*>(part_primitive + 464), part_primitive, transmitter_primitive, state);
				return 0;
			}


			auto fireclickdetector(lua_State* rl) -> int
			{
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


			auto identifyexecutor(lua_State* rl) -> int
			{
				lua_pushstring(rl, "Calibri");
				lua_pushstring(rl, "1.0.0");
				return 2;
			}


			auto getcustomasset(lua_State* rl) -> int
			{
				luaL_checktype(rl, 1, LUA_TSTRING);

				std::string assetname = lua_tostring(rl, 1);
				std::filesystem::path workspace_path = Utils::Workspace::get() / assetname;
				std::filesystem::path asset_directory = Utils::Assets::get() / workspace_path.filename();

				std::filesystem::copy_file(workspace_path, asset_directory, std::filesystem::copy_options::update_existing);
				lua_pushstring(rl, std::string("rbxasset://CALIBRI/" + asset_directory.filename().string()).data());
				return 1;
			}
		}


		namespace Instance
		{
			auto isscriptable(lua_State* rl) -> int
			{
				luaL_checktype(rl, 1, LUA_TUSERDATA);
				luaL_checktype(rl, 2, LUA_TSTRING);

				__int64 instance = *reinterpret_cast<__int64*>(lua_touserdata(rl, 1));
				std::string property_name = lua_tostring(rl, 2);

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

				lua_pushboolean(rl, lua_isscriptable(rl, property));
				return 1;
			}

			auto setscriptable(lua_State* rl)
			{
				luaL_checktype(rl, 1, LUA_TUSERDATA);
				luaL_checktype(rl, 2, LUA_TSTRING);
				luaL_checktype(rl, 3, LUA_TBOOLEAN);

				__int64 instance = *reinterpret_cast<__int64*>(lua_touserdata(rl, 1));
				std::string property_name = lua_tostring(rl, 2);
				bool enabled = lua_toboolean(rl, 3);

				for (__int64 property : *reinterpret_cast<std::vector<__int64>*>(*reinterpret_cast<__int64*>(instance + 24) + 40))
				{
					std::string name = *reinterpret_cast<std::string*>(*reinterpret_cast<__int64*>(property + 8));
					if (name == property_name)
					{
						bool isscriptable = lua_isscriptable(rl, property);
						lua_setscriptable(rl, property, enabled);
						lua_pushboolean(rl, isscriptable);
						return 1;
					}
				}
				return 0;
			}

			auto gethiddenproperty(lua_State* rl) -> int
			{
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


			auto getscripthash_handler(lua_State* rl) -> int
			{
				uintptr_t script = *reinterpret_cast<uintptr_t*>(lua_touserdata(rl, 1));
				const char* classname = *reinterpret_cast<const char**>(*reinterpret_cast<uintptr_t*>(script + 24) + 8);
				std::string compressed_bytecode;

				if (strcmp(classname, "LocalScript") == 0)
					compressed_bytecode = *reinterpret_cast<std::string*>(*reinterpret_cast<uintptr_t*>(script + Offsets::Bytecode::localscript) + 16);
				else if (strcmp(classname, "ModuleScript") == 0)
					compressed_bytecode = *reinterpret_cast<std::string*>(*reinterpret_cast<uintptr_t*>(script + Offsets::Bytecode::localscript) + 16);
				else
					luaL_error(rl, "Local/Module scripts expected.");

				std::string bytecode = Utils::decompress_bytecode(compressed_bytecode);
				if (bytecode.at(0) == 0)
					luaL_error(rl, "Invalid bytecode.");

				auto hash = ::crypt::hash(bytecode, "sha384");
				lua_pushlstring(rl, hash.data(), hash.length());
				return 1;
			}


			auto getscriptbytecode(lua_State* rl) -> int
			{
				luaL_checktype(rl, 1, LUA_TUSERDATA);

				lua_pushcclosure(rl, getscriptbytecode_handler, nullptr, 0);
				lua_pushvalue(rl, 1);
				lua_call(rl, 1, 1);

				return 1;
			}
		}

		namespace Signals
		{
			struct connection_object {
				signal_t* signal;
				uint64_t state;
				uint64_t metatable;
				uint64_t root;
			};

			std::unordered_map<signal_t*, connection_object> connection_table;

			int connection_blank(lua_State* rl) {
				return 0;
			}

			int disable_connection(lua_State* rl) {
				auto connection = (connection_object*)lua_touserdata(rl, 1);
				if (connection->signal->state != 0)
					connection->state = connection->signal->state;

				connection->signal->state = 0;
				return 0;
			}

			int enable_connection(lua_State* rl) {
				auto connection = (connection_object*)lua_touserdata(rl, 1);
				connection->signal->state = connection->state;
				return 0;
			}

			int disconnect_connection(lua_State* rl) {
				auto connection = (connection_object*)lua_touserdata(rl, 1);
				auto root = (signal_t*)connection->root;
				if ((uint64_t)root == (uint64_t)connection) {
					luaL_error(rl, "Failed to disconnect.");
				}

				while (root->next && root->next != connection->signal) {
					root = root->next;
				}

				if (!root->next) {
					luaL_error(rl, "Already disconnected.");
				}

				root->next = root->next->next;
				connection->signal->state = 0;
				return 0;
			}

			int connection_index(lua_State* rl) {
				std::string key = std::string(lua_tolstring(rl, 2, nullptr));
				auto connection = (connection_object*)lua_touserdata(rl, 1);

				if (key == "Enabled" || key == "enabled") {
					lua_pushboolean(rl, !(connection->signal->state == 0));
					return 1;
				}

				if (key == "Function" || key == "function" || key == "Fire" || key == "fire" || key == "Defer" || key == "defer") {
					int signal_data = *(int*)&connection->signal->signal_data;
					if (signal_data && *(int*)&connection->signal->signal_data->connection_data) {
						int index = connection->signal->signal_data->connection_data->func_idx;
						lua_rawgeti(rl, LUA_REGISTRYINDEX, index);

						if (lua_type(rl, -1) != LUA_TFUNCTION)
							lua_pushcclosure(rl, connection_blank, 0, 0, 0);

						return 1;
					}

					lua_pushcclosure(rl, connection_blank, 0, 0, 0);
					return 1;
				}

				if (key == "LuaConnection") {
					int signal_data = *(int*)&connection->signal->signal_data;
					if (signal_data && *(int*)&connection->signal->signal_data->connection_data) {
						int index = connection->signal->signal_data->connection_data->func_idx;

						lua_rawgeti(rl, LUA_REGISTRYINDEX, index);
						auto func_tval = (TValue*)index2addr(rl, -1);
						auto cl = (Closure*)func_tval->value.gc;
						bool lua = !cl->isC;

						lua_pop(rl, 1);
						lua_pushboolean(rl, lua);
						return 1;
					}

					lua_pushboolean(rl, false);
					return 1;
				}

				if (key == "ForeignState") {
					int signal_data = *(int*)&connection->signal->signal_data;
					if (signal_data && *(int*)&connection->signal->signal_data->connection_data) {
						int index = connection->signal->signal_data->connection_data->func_idx;

						lua_rawgeti(rl, LUA_REGISTRYINDEX, index);
						auto func_tval = (TValue*)index2addr(rl, -1);
						auto cl = (Closure*)func_tval->value.gc;
						bool c = cl->isC;

						lua_pop(rl, 1);
						lua_pushboolean(rl, c);
						return 1;
					}

					lua_pushboolean(rl, false);
					return 1;
				}

				if (key == "Disconnect" || key == "disconnect") {
					lua_pushcclosure(rl, disconnect_connection, 0, 0, 0);
					return 1;
				}

				if (key == "Disable" || key == "disable") {
					lua_pushcclosure(rl, disable_connection, 0, 0, 0);
					return 1;
				}

				if (key == "Enable" || key == "enable") {
					lua_pushcclosure(rl, enable_connection, 0, 0, 0);
					return 1;
				}

				if (key == "Thread") {
					lua_pushthread(rl);
					return 1;
				}

				luaL_error(rl, "Invalid index.");
				return 0;
			}

			int getconnections_handler(lua_State* rl) {
				auto signal = *(signal_t**)lua_touserdata(rl, 1);
				signal = signal->next;

				lua_createtable(rl, 0, 0);
				auto signal_root = signal;
				int index = 1;

				while (signal) {
					int func_idx = signal->signal_data->connection_data->func_idx;

					if (!connection_table.count(signal)) {
						connection_object new_connection;
						new_connection.signal = signal;
						new_connection.root = (uint64_t)signal_root;
						new_connection.state = signal->state;
						connection_table[signal] = new_connection;
					}

					auto connection = (connection_object*)lua_newuserdata(rl, sizeof(connection_object), 0);
					*connection = connection_table[signal];

					lua_createtable(rl, 0, 0);
					lua_pushcclosure(rl, connection_index, 0, 0, 0);
					lua_setfield(rl, -2, "__index");

					lua_pushstring(rl, "Connection");
					lua_setfield(rl, -2, "__type");
					lua_setmetatable(rl, -2);

					lua_rawseti(rl, -2, index++);
					signal = signal->next;
				}

				return 1;
			}

			int getconnections(lua_State* L)
			{
				lua_getfield(L, 1, "Connect");
				if (!lua_isfunction(L, -1)) {
					luaL_error(L, "Signal does not have 'Connect' method");
				}
				lua_pushvalue(L, 1);

				lua_pushcfunction(L, connection_blank, nullptr);

				if (lua_pcall(L, 2, 1, 0) != LUA_OK) {
					luaL_error(L, "Error calling 'Connect': %s", lua_tostring(L, -1));
				}

				lua_pushcfunction(L, getconnections_handler, nullptr);
				lua_pushvalue(L, -2);

				if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
					luaL_error(L, "Error calling 'getconnections': %s", lua_tostring(L, -1));
				}

				lua_getfield(L, -2, "Disconnect");
				if (lua_isfunction(L, -1)) {
					lua_pushvalue(L, -3);
					if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
						luaL_error(L, "Error calling 'Disconnect': %s", lua_tostring(L, -1));
					}
				}

				return 1;
			}
		}
	}


	namespace Decompiler
	{
#define decode_A(inst) ((inst >> 8) & 0xFF) 
#define decode_B(inst) ((inst >> 16) & 0xFF)
#define decode_C(inst) ((inst >> 24) & 0xFF)
#define decode_D(inst) (std::int32_t (inst) >> 16)
#define decode_E(inst) (std::int32_t (inst) >> 8)

		static std::string indent(int depth) {
			return std::string(depth * 4, ' ');
		}

		static std::string getVar(int index) {
			return "var" + std::to_string(index);
		}

		std::string generateLuaCode(Proto* proto, int depth) {
			std::stringstream output;

			output << indent(depth) << "function(";
			for (int i = 0; i < proto->numparams; i++) {
				output << "arg" << i;
				if (i < proto->numparams - 1) {
					output << ", ";
				}
			}
			if (proto->is_vararg) {
				if (proto->numparams > 0) {
					output << ", ";
				}
				output << "...";
			}
			output << ")\n";

			for (int i = 0; i < proto->sizecode; i++) {
				Instruction instruction = proto->code[i];
				uint64_t opcode = instruction & 0xFF;  // Опкод занимает 6 бит

				output << indent(depth + 1);

				switch (opcode) {
				case LOP_LOADK: {
					uint64_t A = decode_A(instruction);
					uint32_t Bx = decode_B(instruction);

					if (proto->k[Bx + 1].tt == LUA_TSTRING)
					{
						output << "local var" << A << " = " << "'" << proto->k[Bx + 1].value.gc->ts.data << "'" << "\n";
					}
					else if (proto->k[Bx + 1].tt == LUA_TNUMBER)
					{
						output << "local var" << A << " = " << std::to_string(proto->k[Bx + 1].value.n) << "\n";
					}
					else if (proto->k[Bx + 1].tt == LUA_TNIL)
					{
						output << "local var" << A << " = nil\n";
					}
					else
					{
						output << "local var" << A << " = unknown_value[" << Bx << "]\n";
					}

					break;
				}
				case LOP_RETURN: { // Пример: LOP_RETURN
					output << "return\n";
					break;
				}
				case LOP_NOP: {
					output << "\n";
					break;
				}
				case LOP_MOVE: {
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					output << getVar(A) << " = " << getVar(B) << "\n";
					break;
				}
				case LOP_LOADB: {
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint64_t C = decode_C(instruction);
					output << "local " << getVar(A) << " = " << (B ? "true" : "false") << "\n";
					if (C) output << indent(depth + 1) << "goto next\n";
					break;
				}
				case LOP_LOADNIL: { // LOADNIL
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					output << "local ";
					for (uint32_t j = A; j <= B; j++) {
						output << getVar(j);
						if (j < B) output << ", ";
					}
					output << " = nil\n";
					break;
				}
				case LOP_GETUPVAL: { // GETUPVAL
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					output << getVar(A) << " = upvalues[" << B << "]\n";
					break;
				}
				case LOP_GETGLOBAL: { // GETGLOBAL
					uint32_t A = decode_A(instruction);
					uint32_t Bx = decode_B(instruction);

					if (proto->k[Bx + 1].tt == LUA_TSTRING)
					{
						output << getVar(A) << " = _G['" << proto->k[Bx + 1].value.gc->ts.data << "']\n";
					}
					else if (proto->k[Bx + 1].tt == LUA_TNUMBER)
					{
						output << getVar(A) << " = _G[" << std::to_string(proto->k[Bx + 1].value.n) << "]\n";
					}
					else if (proto->k[Bx + 1].tt == LUA_TFUNCTION)
					{
						output << getVar(A) << " = _G[function" << Bx << "]\n";
					}
					else if (proto->k[Bx + 1].tt == LUA_TNIL)
					{
						output << getVar(A) << " = _G[nil]\n";
					}
					else if (proto->k[Bx + 1].tt == LUA_TTABLE)
					{
						output << getVar(A) << " = _G[table" << Bx << "]\n";
					}
					else
					{
						output << getVar(A) << " = _G[unknown_value" << Bx << "]\n";
					}

					break;
				}
				case LOP_GETTABLE: { // GETTABLE
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint64_t C = decode_C(instruction);
					output << getVar(A) << " = " << getVar(B) << "[" << getVar(C) << "]\n";
					break;
				}
				case LOP_SETGLOBAL: { // SETGLOBAL
					uint32_t A = decode_A(instruction);
					uint32_t Bx = decode_B(instruction);

					if (proto->k[Bx + 1].tt == LUA_TSTRING)
					{
						output << "_G['" << proto->k[Bx + 1].value.gc->ts.data << "'] = " << getVar(A) << "\n";
					}
					else if (proto->k[Bx + 1].tt == LUA_TNUMBER)
					{
						output << "_G[" << std::to_string(proto->k[Bx + 1].value.n) << "] = " << getVar(A) << "\n";
					}
					else if (proto->k[Bx + 1].tt == LUA_TFUNCTION)
					{
						output << "_G[function" << Bx << "] = " << getVar(A) << "\n";
					}
					else if (proto->k[Bx + 1].tt == LUA_TNIL)
					{
						output << "_G[nil] = " << getVar(A) << "\n";
					}
					else if (proto->k[Bx + 1].tt == LUA_TTABLE)
					{
						output << "_G[table" << Bx << "] = " << getVar(A) << "\n";
					}
					else
					{
						output << "_G[unknown_value" << Bx << "] = " << getVar(A) << "\n";
					}

					break;
				}
				case LOP_SETUPVAL: { // SETUPVAL
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					output << "upvalues[" << B << "] = " << getVar(A) << "\n";
					break;
				}
				case LOP_SETTABLE: { // SETTABLE
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint64_t C = decode_C(instruction);
					output << getVar(B) << "[" << getVar(C) << "] = " << getVar(A) << "\n";
					break;
				}
				case LOP_NEWTABLE: { // NEWTABLE
					uint64_t A = decode_A(instruction);
					output << getVar(A) << " = {}\n";
					break;
				}
				case LOP_ADD: { // ADD
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint64_t C = decode_C(instruction);
					output << getVar(A) << " = " << getVar(B) << " + " << getVar(C) << "\n";
					break;
				}
				case LOP_SUB: { // SUB
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint64_t C = decode_C(instruction);
					output << getVar(A) << " = " << getVar(B) << " - " << getVar(C) << "\n";
					break;
				}
				case LOP_MUL: { // MUL
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint64_t C = decode_C(instruction);
					output << getVar(A) << " = " << getVar(B) << " * " << getVar(C) << "\n";
					break;
				}
				case LOP_DIV: { // DIV
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint64_t C = decode_C(instruction);
					output << getVar(A) << " = " << getVar(B) << " / " << getVar(C) << "\n";
					break;
				}
				case LOP_MOD: { // MOD
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint64_t C = decode_C(instruction);
					output << getVar(A) << " = " << getVar(B) << " % " << getVar(C) << "\n";
					break;
				}
							//before

				case LOP_BREAK: {
					output << "break\n";
					break;
				}
				case LOP_LOADN: {
					uint64_t A = decode_A(instruction);
					uint32_t Bx = (instruction >> 24);
					output << getVar(A) << " = " << std::to_string(Bx) << "\n";
					break;
				}
				case LOP_GETIMPORT: {
					uint64_t A = decode_A(instruction);
					uint32_t Bx = decode_B(instruction);

					output << getVar(A) << " = IMPORT[" << Bx << "]\n";
					break;
				}
				case LOP_GETTABLEKS: {
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint32_t Bx = decode_C(instruction);

					if (proto->k[Bx + 1].tt == LUA_TSTRING)
					{
						output << getVar(A) << " = " << getVar(B) << "['" << proto->k[Bx + 1].value.gc->ts.data << "']\n";
					}
					else if (proto->k[Bx + 1].tt == LUA_TNUMBER)
					{
						output << getVar(A) << " = " << getVar(B) << "[" << std::to_string(proto->k[Bx + 1].value.n) << "]\n";
					}
					else if (proto->k[Bx + 1].tt == LUA_TBOOLEAN)
					{
						output << getVar(A) << " = " << getVar(B) << "[" << std::to_string(proto->k[Bx + 1].value.b) << "]\n";
					}
					else if (proto->k[Bx + 1].tt == LUA_TFUNCTION)
					{
						output << getVar(A) << " = " << getVar(B) << "[function[" << Bx << "]]\n";
					}
					else if (proto->k[Bx + 1].tt == LUA_TTABLE)
					{
						output << getVar(A) << " = " << getVar(B) << "table[" << Bx << "]]\n";
					}
					else
					{
						output << getVar(A) << " = " << getVar(B) << "[unknown_value[" << Bx << "]]\n";
					}

					break;
				}
				case LOP_SETTABLEKS: {
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint32_t Bx = decode_C(instruction);

					if (proto->k[Bx + 1].tt == LUA_TSTRING)
					{
						output << getVar(B) << "['" << proto->k[Bx + 1].value.gc->ts.data << "'] = " << getVar(A) << "\n";
					}
					else if (proto->k[Bx + 1].tt == LUA_TNUMBER)
					{
						output << getVar(B) << "[" << std::to_string(proto->k[Bx + 1].value.n) << "] = " << getVar(A) << "\n";
					}
					else if (proto->k[Bx + 1].tt == LUA_TBOOLEAN)
					{
						output << getVar(B) << "[" << std::to_string(proto->k[Bx + 1].value.b) << "] = " << getVar(A) << "\n";
					}
					else if (proto->k[Bx + 1].tt == LUA_TFUNCTION)
					{
						output << getVar(B) << "[function[" << Bx << "]] = " << getVar(A) << "\n";
					}
					else if (proto->k[Bx + 1].tt == LUA_TTABLE)
					{
						output << getVar(B) << "[table[" << Bx << "]] = " << getVar(A) << "\n";
					}
					else
					{
						output << getVar(B) << "[unknown_value[" << Bx << "]] = " << getVar(A) << "\n";
					}

					break;
				}
				case LOP_DUPTABLE: {
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint32_t Bx = instruction >> 16;
					output << getVar(A) << " = table.clone(" << getVar(B) << ")\n";
					break;
				}
				case LOP_SETLIST: {
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint64_t C = decode_C(instruction);
					output << "list.set(" << getVar(A) << ", " << B << ", " << C << ")\n";
					break;
				}
				case LOP_FORNPREP: {
					uint64_t A = decode_A(instruction);
					uint32_t Bx = instruction >> 16;
					output << "for " << getVar(A) << " = " << getVar(A + 1) << ", " << getVar(A + 2) << ", " << getVar(A + 3) << " do\n";
					output << indent(depth + 1) << "goto " << Bx << "\n";
					break;
				}
				case LOP_FORNLOOP: {
					uint64_t A = decode_A(instruction);
					uint32_t Bx = instruction >> 16;
					output << "end\n";
					output << indent(depth + 1) << "goto " << Bx << "\n";
					break;
				}
				case LOP_JUMP: {
					uint32_t sBx = (int32_t)(instruction >> 16) - 0xFFFF;
					output << "goto " << sBx << "\n";
					break;
				}
				case LOP_JUMPBACK: {
					uint32_t sBx = (int32_t)(instruction >> 16) - 0xFFFF;
					output << "goto " << sBx << "\n";
					break;
				}
				case LOP_JUMPIF: {
					uint64_t A = decode_A(instruction);
					uint32_t sBx = (int32_t)(instruction >> 16) - 0xFFFF;

					output << "if " << getVar(A) << " then goto " << sBx << " end\n";
					break;
				}
				case LOP_JUMPIFNOT: {
					uint64_t A = decode_A(instruction);
					uint32_t sBx = (int32_t)(instruction >> 16) - 0xFFFF;
					output << "if not " << getVar(A) << " then goto " << sBx << " end\n";
					break;
				}
				case LOP_JUMPIFEQ: {
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint32_t sBx = (int32_t)(instruction >> 16) - 0xFFFF;
					output << "if " << getVar(A) << " == " << getVar(B) << " then goto " << sBx << " end\n";
					break;
				}
				case LOP_JUMPIFLE: {
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint32_t sBx = (int32_t)(instruction >> 16) - 0xFFFF;
					output << "if " << getVar(A) << " <= " << getVar(B) << " then goto " << sBx << " end\n";
					break;
				}
				case LOP_JUMPIFLT: {
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint32_t sBx = (int32_t)(instruction >> 16) - 0xFFFF;
					output << "if " << getVar(A) << " < " << getVar(B) << " then goto " << sBx << " end\n";
					break;
				}
				case LOP_JUMPIFNOTEQ: {
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint32_t sBx = (int32_t)(instruction >> 16) - 0xFFFF;
					output << "if " << getVar(A) << " ~= " << getVar(B) << " then goto " << sBx << " end\n";
					break;
				}
				case LOP_JUMPIFNOTLE: {
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint32_t sBx = (int32_t)(instruction >> 16) - 0xFFFF;
					output << "if " << getVar(A) << " > " << getVar(B) << " then goto " << sBx << " end\n";
					break;
				}
				case LOP_JUMPIFNOTLT: {
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint32_t sBx = (int32_t)(instruction >> 16) - 0xFFFF;
					output << "if " << getVar(A) << " >= " << getVar(B) << " then goto " << sBx << " end\n";
					break;
				}
				case LOP_NEWCLOSURE: {
					uint64_t A = decode_A(instruction);
					uint32_t Bx = instruction >> 16;
					output << getVar(A) << " = closure[" << Bx << "]\n";
					break;
				}
				case LOP_CAPTURE: {
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint32_t C = (instruction >> 14) & 0x3FF;
					output << "capture[" << A << "][" << B << "] = " << getVar(C) << "\n";
					break;
				}
				case LOP_CALL: {
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint64_t C = decode_C(instruction);
					if (C > 1) {
						//output << " = ";
						for (uint32_t i = 0; i < C; i++) {
							output << getVar(A + i);
							if (i < C - 1) output << ", ";
						}
						output << " = ";
					}
					output << getVar(A) << "(";
					for (uint32_t i = 0; i < B; i++) {
						output << getVar(A + i);
						if (i < B - 1) output << ", ";
					}
					output << ")";
					output << "\n";
					break;
				}
				case LOP_FORGLOOP: {
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					uint64_t C = decode_C(instruction);
					output << "for " << getVar(A) << ", " << getVar(B) << ", " << getVar(C) << " do\n";
					break;
				}
				case LOP_FORGPREP: {
					uint64_t A = decode_A(instruction);
					uint64_t B = decode_B(instruction);
					output << "for " << getVar(A) << " = " << getVar(B) << " do\n";
					break;
				}
				case LOP_FASTCALL: {
					uint64_t A = decode_A(instruction);
					output << "__fastcall[" << getVar(A) << "]()\n";
					break;
				}
				default:
					output << "";
				}
			}

			output << indent(depth) << "end\n";
			return output.str();
		}

		std::string fixVariableNames(const std::string& code) {
			std::string fixedCode = code;

			std::regex varRegex("local\\s+(var[0-9]+)\\s*=\\s*['\"](\\w+)['\"]");
			std::smatch match;

			std::unordered_map<std::string, std::string> varMap;

			std::string::const_iterator searchStart(fixedCode.cbegin());
			while (std::regex_search(searchStart, fixedCode.cend(), match, varRegex)) {
				std::string varName = match[1];
				std::string value = match[2];

				std::string newVarName;
				for (char ch : value) {
					if (isupper(ch)) {
						newVarName += '_';
						newVarName += tolower(ch);
					}
					else {
						newVarName += ch;
					}
				}

				varMap[varName] = newVarName;
				searchStart = match.suffix().first;
			}

			for (const auto& entry : varMap) {
				fixedCode = std::regex_replace(fixedCode, std::regex(entry.first), entry.second);
			}

			return fixedCode;
		}

		std::string fixLuaCode(const std::string& code) {
			std::string fixedCode = code;

			fixedCode = std::regex_replace(fixedCode, std::regex("\\]"), "]");
			fixedCode = std::regex_replace(fixedCode, std::regex("\\[\\["), "[");

			fixedCode = std::regex_replace(fixedCode, std::regex("var([0-9]+) = IMPORT\\[([0-9]+)\\]"), "local var$1 = IMPORT[$2]");
			fixedCode = std::regex_replace(fixedCode, std::regex("var([0-9]+), var([0-9]+) = var([0-9]+)\\((.*)\\)"), "local var$1, var$2 = var$3($4)");

			fixedCode = std::regex_replace(fixedCode, std::regex("var([0-9]+)\\[unknown_value\\[([0-9]+)\\]\\]"), "var$1[unknown_value[$2]]");

			fixedCode = std::regex_replace(fixedCode, std::regex("return\\s*end"), "return\nend");

			fixedCode = std::regex_replace(fixedCode, std::regex("if\\s+not\\s+(var[0-9]+)\\s+then\\s+goto\\s+([0-9]+)"), "if not $1 then\n\tgoto label_$2\nend");
			fixedCode = std::regex_replace(fixedCode, std::regex("goto\\s+([0-9]+)"), "goto label_$1");

			fixedCode = std::regex_replace(fixedCode, std::regex("label_([0-9]+)"), "::label_$1::");

			fixedCode = fixVariableNames(fixedCode);

			return fixedCode;
		}

		auto decompile(lua_State* rl) -> int
		{
			luaL_checktype(rl, 1, LUA_TUSERDATA);

			uintptr_t script = *reinterpret_cast<uintptr_t*>(lua_touserdata(rl, 1));
			const char* classname = *reinterpret_cast<const char**>(*reinterpret_cast<uintptr_t*>(script + 24) + 8);
			std::string watermark;

			if (strcmp(classname, xorstr_("LocalScript")) == 0)
				watermark = xorstr_("--Decompiled by Calibri decompiler!\n--Version: 0.01\n--Type: Local Script\n\n");

			else if (strcmp(classname, xorstr_("ModuleScript")) == 0)
				watermark = xorstr_("--Decompiled by Calibri decompiler!\n--Version: 0.01\n--Type: Module Script\n\n");
			else
				luaL_error(rl, xorstr_("At the moment, the decompiler only works with 'LocalScript' and 'ModuleScript'."));


			lua_pushcclosure(rl, Enviroment::Functions::Instance::getscriptbytecode_handler, nullptr, 0);
			lua_pushvalue(rl, 1);
			lua_call(rl, 1, 1);

			auto bytecode = lua_tostring(rl, -1);
			lua_settop(rl, 0);
			;
			luau_load(rl, "", bytecode.c_str(), bytecode.size(), 0);

			auto function = *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>((__int64)rl + Offsets::LuaState::top) - 16);
			auto proto = ProtoThings::deobfuscate_proto(function);
			lua_settop(rl, 0);

			std::string lua_code = std::string(watermark + fixLuaCode(generateLuaCode((Proto*)proto, 0)));
			lua_pushstring(rl, lua_code.c_str());

			return 1;
		}
	}


	namespace MetaHooks
	{
		static __int64 old_namecall;
		static __int64 old_index;

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
			"sendCommand", "getAsync", "getAsyncFullUrl", "requestAsync", "makeRequest"
		};

		auto dangerous_function(lua_State* L) -> int
		{
			lua_pushboolean(L, false);
			lua_pushstring(L, "Disabled for security reasons.");
			return 2;
		}

		auto namecall_hook(lua_State* L) -> int
		{
			auto state = (__int64)L;
			uintptr_t userdata = *reinterpret_cast<uintptr_t*>(state + 120);
			int level = *reinterpret_cast<uintptr_t*>(userdata + 0x30);

			if (L->namecall && level >= 3)
			{
				const char* data = L->namecall->data;

				if (!strcmp(data, "HttpGet") || !strcmp(data, "HttpGetAsync"))
				{
					return Enviroment::Functions::Scripts::httpget(L);
				}

				if (!strcmp(data, "GetObjects") || !strcmp(data, "GetObjectsAsync"))
				{
					return Enviroment::Functions::Scripts::getobjects(L);
				}

				for (unsigned int i = 0; i < dangerous_functions.size(); i++) {
					if (!strcmp(data, dangerous_functions[i])) {
						lua_pushboolean(L, false);
						lua_pushstring(L, "Disabled for security reasons.");
						return 2;
					}
				}
			}

			return reinterpret_cast<__int64(__fastcall*)(__int64)>(old_namecall)((__int64)L);
		}

		auto index_hook(lua_State* L) -> int
		{
			auto state = (__int64)L;
			uintptr_t userdata = *reinterpret_cast<uintptr_t*>(state + 120);
			int level = *reinterpret_cast<uintptr_t*>(userdata + 0x30);

			if (lua_isstring(L, 2) && level >= 3)
			{
				const char* data = luaL_checkstring(L, 2);

				if (!strcmp(data, "HttpGet") || !strcmp(data, "HttpGetAsync"))
				{
					lua_getglobal(L, "HttpGet");
					return 1;
				}

				if (!strcmp(data, "GetObjects") || !strcmp(data, "GetObjectsAsync"))
				{
					lua_getglobal(L, "GetObjects");
					return 1;
				}

				for (unsigned int i = 0; i < dangerous_functions.size(); i++) {
					if (!strcmp(data, dangerous_functions[i])) {
						lua_pushcclosure(L, dangerous_function, NULL, 0);
						return 1;
					}
				}
			}

			return reinterpret_cast<__int64(__fastcall*)(__int64)>(old_index)((__int64)L);
		}

		auto initialize(lua_State* L) -> void
		{
			lua_getglobal(L, "game");
			lua_getmetatable(L, -1);
			lua_getfield(L, -1, "__namecall");

			Closure* namecall = (Closure*)lua_topointer(L, -1);
			lua_CFunction namecall_f = namecall->c.f;
			old_namecall = (__int64)namecall_f;
			namecall->c.f = namecall_hook;

			lua_settop(L, 0);

			lua_getglobal(L, "game");
			lua_getmetatable(L, -1);
			lua_getfield(L, -1, "__index");

			Closure* index = (Closure*)lua_topointer(L, -1);
			lua_CFunction index_f = index->c.f;
			old_index = (__int64)index_f;
			index->c.f = index_hook;
		}
	}
}
#pragma once
#include "Includes.h"
#include "Offsets.h"
extern "C" {
#include "Lua\lua.h"
#include "Lua\lua.hpp"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\llimits.h"
#include "Lua/ldo.h"
}

#include "CustomDataTypeHandler.h"

int registry;

namespace Wrapper
{
	DWORD RLS;
	lua_State* LS;

	void Wrap(lua_State* L, DWORD rL, int index);
	void Wrap(DWORD rL, lua_State* L, int index);
	int VCallHandler(lua_State* L);
	int RCallHandler(DWORD rL);
	std::vector<int> int3bp;
}



namespace Wrapper
{


	LONG WINAPI CallCheck(PEXCEPTION_POINTERS ex)
	{
		switch (ex->ExceptionRecord->ExceptionCode)
		{
		case (DWORD)0x80000003L:
		{
			if (ex->ContextRecord->Eip == int3bp[0])
			{
				ex->ContextRecord->Eip = (DWORD)(RCallHandler);
				return EXCEPTION_CONTINUE_EXECUTION;
			}
			return -1;
		}
		default: return 0;
		}
		return 0;
	}

	DWORD locateINT3() {
		DWORD _s = x(0x400000);
		const char i3_8opcode[8] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
		for (int i = 0; i < MAX_INT; i++) {
			if (memcmp((void*)(_s + i), i3_8opcode, sizeof(i3_8opcode)) == 0) {
				return (_s + i);
			}
		}
		return NULL;
	}

	VOID CallCheckBypass()
	{
		int3bp.push_back(locateINT3());
		int3bp.push_back(locateINT3());
		AddVectoredExceptionHandler(1, CallCheck);
	}

	static int CustomEq(lua_State* L) {
		lua_pushboolean(L, (*(DWORD *)lua_touserdata(L, 1) == *(DWORD *)lua_touserdata(L, 2)));
		//std::cout << "Eq called" << std::endl;
		//lua_pushvalue(L, -2);
		//lua_getfield(L, LUA_GLOBALSINDEX, "tostring");
		//lua_pushvalue(L, -2);
		//lua_pcall(L, 1, 1, 0);
		//const char* s1 = lua_tolstring(L, -1, NULL);
		//lua_settop(L, -3);
		//lua_pushvalue(L, -1);
		//lua_getfield(L, LUA_GLOBALSINDEX, "tostring");
		//lua_pushvalue(L, -2);
		//lua_pcall(L, 1, 1, 0);
		//const char* s2 = lua_tolstring(L, -1, NULL);
		//lua_settop(L, -3);
		//if (strcmp(s1, s2) != 0) {
		//	lua_pushboolean(L, false);
		//	return 1;
		//}
		//lua_pushvalue(L, -2);
		//lua_getfield(L, LUA_GLOBALSINDEX, "typeof");
		//lua_pushvalue(L, -2);
		//lua_pcall(L, 1, 1, 0);
		//const char* t1 = lua_tolstring(L, -1, NULL);
		//lua_settop(L, -3);
		//lua_pushvalue(L, -1);
		//lua_getfield(L, LUA_GLOBALSINDEX, "typeof");
		//lua_pushvalue(L, -2);
		//lua_pcall(L, 1, 1, 0);
		//const char* t2 = lua_tolstring(L, -1, NULL);
		//lua_settop(L, -3);
		//if (strcmp(t1, t2) != 0) {
		//	lua_pushboolean(L, false);
		//	return 1;
		//}
		//lua_pushboolean(L, true);
		return 1;
	}


	static TValue *lua_index2adr(lua_State *L, int idx) {
		if (idx > 0) {
			TValue *o = L->base + (idx - 1);
			api_check(L, idx <= L->ci->top - L->base);
			if (o >= L->top) return cast(TValue *, luaO_nilobject);
			else return o;
		}
		else if (idx > LUA_REGISTRYINDEX) {
			api_check(L, idx != 0 && -idx <= L->top - L->base);
			return L->top + idx;
		}
		else switch (idx) {  /* pseudo-indices */
		case LUA_REGISTRYINDEX: return registry(L);
		case LUA_ENVIRONINDEX: {
			Closure *func = curr_func(L);
			sethvalue(L, &L->env, func->c.env);
			return &L->env;
		}
		case LUA_GLOBALSINDEX: return gt(L);
		default: {
			Closure *func = curr_func(L);
			idx = LUA_GLOBALSINDEX - idx;
			return (idx <= func->c.nupvalues)
				? &func->c.upvalue[idx - 1]
				: cast(TValue *, luaO_nilobject);
		}
		}
	}

	typedef void*(__cdecl *rrawseti)(DWORD rL, int idx, int n);
	rrawseti r_lua_rawseti = (rrawseti)(unprotect(x(0x812730)));

	static int CollectGarbage(lua_State* L) {
		void *UD = lua_touserdata(L, 1);
		lua_pushvalue(L, 1);
		if (!lua_isnil(L, -1)) {
			int ref = lua_tointeger(L, -1);
			r_lua_pushnil(RLS);
			r_lua_rawseti(RLS, LUA_REGISTRYINDEX, ref);
			r_lua_settop(RLS, 0);
		}
		lua_pop(L, 1);
		return 0;
	}

	struct DataModelUserData {
		int ref;
	};

	void Wrap(lua_State* L, DWORD rL, int index)
	{
		switch (lua_type(L, index))
		{
		case LUA_TLIGHTUSERDATA:
			r_lua_pushlightuserdata(rL, nullptr);
			break;
		case LUA_TNIL:
			r_lua_pushnil(rL);
			break;
		case LUA_TNUMBER:
			r_lua_pushnumber(rL, lua_tonumber(L, index));
			break;
		case LUA_TBOOLEAN:
			r_lua_pushboolean(rL, lua_toboolean(L, index));
			break;
		case LUA_TSTRING:
			r_lua_pushstring(rL, lua_tostring(L, index));
			break;
		case LUA_TTHREAD:
			r_lua_newthread(rL);
			break;
		case LUA_TFUNCTION:
			lua_pushvalue(L, index);
			r_lua_pushnumber(rL, luaL_ref(L, LUA_REGISTRYINDEX));
			r_lua_pushcclosure(rL, int3bp[0], 1);
			break;
		case LUA_TTABLE:{
			lua_pushvalue(L, index);
			lua_getfield(L, LUA_GLOBALSINDEX, "tostring");
			lua_pushvalue(L, -2);
			lua_pcall(L, 1, 1, 0);
			const char* a = lua_tolstring(L, -1, NULL);
			lua_settop(L, -2);
			r_lua_createtable(rL, 0, 0);
			lua_pushnil(L);
			while (lua_next(L, -2)) {
				lua_pushvalue(L, -1);
				lua_getfield(L, LUA_GLOBALSINDEX, "tostring");
				lua_pushvalue(L, -2);
				lua_pcall(L, 1, 1, 0);
				const char* b = lua_tolstring(L, -1, NULL);
				lua_settop(L, -3);
				if (strcmp(a, b) == 0) {
					lua_settop(L, -2);
					continue;
				}
				Wrap(L, rL, -2);
				Wrap(L, rL, -1);
				r_lua_settable(rL, -3);
				lua_settop(L, -2);
			}
			lua_settop(L, -2);
			break;
		}
		case LUA_TUSERDATA:{
			r_lua_rawgeti(rL, LUA_REGISTRYINDEX, reinterpret_cast<DataModelUserData*>(lua_touserdata(L, index))->ref);
			break;
		}
		default: break;
		}
	}
	void Wrap(DWORD rL, lua_State* L, int index)
	{
		switch (r_lua_type(rL, index))
		{
		case R_LUA_TLIGHTUSERDATA:
			lua_pushlightuserdata(L, nullptr);
			break;
		case R_LUA_TNIL:
			lua_pushnil(L);
			break;
		case R_LUA_TNUMBER:
			lua_pushnumber(L, r_lua_tonumber(rL, index));
			break;
		case R_LUA_TBOOLEAN:
			lua_pushboolean(L, r_lua_toboolean(rL, index));
			break;
		case R_LUA_TSTRING:
			lua_pushstring(L, r_lua_tostring(rL, index));
			break;
		case R_LUA_TTHREAD:
			lua_newthread(L);
			break;
		case R_LUA_TFUNCTION:
			r_lua_pushvalue(rL, index);
			lua_pushnumber(L, r_luaL_ref(rL, LUA_REGISTRYINDEX));
			lua_pushcclosure(L, VCallHandler, 1);
			break;
		case R_LUA_TTABLE:
			r_lua_pushvalue(rL, index);
			lua_newtable(L);
			r_lua_pushnil(rL);
			while (r_lua_next(rL, -2) != R_LUA_TNIL)
			{
				Wrap(rL, L, -2);
				Wrap(rL, L, -1);
				lua_settable(L, -3);
				r_lua_pop(rL, 1);
			}
			r_lua_pop(rL, 1);
			break;
		case R_LUA_TUSERDATA:{
			int passedUD = (int)r_lua_touserdata(rL, index);
			lua_rawgeti(L, LUA_REGISTRYINDEX, passedUD);

			if (!lua_type(L, -1))
			{
				lua_settop(L, -2);
				r_lua_pushvalue(rL, index);
				reinterpret_cast<DataModelUserData*>(lua_newuserdata(L, sizeof(DataModelUserData)))->ref = r_luaL_ref(rL, LUA_REGISTRYINDEX);
				r_lua_getmetatable(rL, index);
				Wrap(rL, L, -1);
				lua_setmetatable(L, -2);
				lua_pushvalue(L, -1);
				lua_rawseti(L, -10000, passedUD);
				r_lua_settop(rL, -2);
			}
			break;
		}
		default: break;
		}
	}

	static int LuaYield(lua_State* thread)
	{
		lua_State* L = lua_tothread(thread, lua_upvalueindex(1));
		const int nargs = lua_gettop(thread);
		lua_settop(thread, -2);
		lua_xmove(thread, L, nargs);
		return lua_resume(L, nargs);
		lua_close(L);

	}

	int VCallHandler(lua_State* L)
	{
		DWORD rL = r_lua_newthread(RLS);
		r_lua_settop(RLS, -2);

		int key = lua_tonumber(L, lua_upvalueindex(1));

		r_lua_rawgeti(rL, LUA_REGISTRYINDEX, key);

		for (int arg = 1; arg <= lua_gettop(L); ++arg)
			Wrap(L, rL, arg);

		if (r_lua_pcall(rL, lua_gettop(L), LUA_MULTRET, 0))
		{
			std::string ErrM = r_lua_tostring(rL, -1, 0);

		if (ErrM.find("yield") != std::string::npos)
		{
			r_lua_pop(rL, 1);
			lua_pushthread(L);
			lua_pushcclosure(L, &LuaYield, 1);
			Wrap(L, rL, -1);
			return lua_yield(L, 0);
		}
		return 0;
	}  

		int args = 0;

		for (int arg = 1; arg <= r_lua_gettop(rL); ++arg, ++args)
			Wrap(rL, L, arg);

		r_lua_settop(rL, 0);

		return args;
		lua_close(L);
	}

	int RCallHandler(DWORD rL)
	{

		lua_State* L = lua_newthread(LS);
		lua_settop(LS, -2);

		int key = r_lua_tonumber(rL, lua_upvalueindex(1));

		luai_userstatethread(LS, L);
		setclvalue(LS, L->top, r_lua_touserdata(rL, lua_upvalueindex(1)));
		incr_top(LS);

		lua_rawgeti(L, LUA_REGISTRYINDEX, key);

		for (int arg = 1; arg <= r_lua_gettop(rL); ++arg)
			Wrap(rL, L, arg);

		lua_pcall(L, r_lua_gettop(rL), LUA_MULTRET, 0);

		int args = 0;

		for (int arg = 1; arg <= lua_gettop(L); ++arg, ++args)
			Wrap(L, rL, arg);

		lua_settop(L, 0);

		return args;
		lua_close(L);

	}
}
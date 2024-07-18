#pragma once
#include "r_lua.h"

uintptr_t globalState;

uintptr_t g_rL;
lua_State* g_L;

int registryIndex = 0;

#define int3breakpoint format(0x401016)

namespace Bridge
{
	void push(lua_State* L, uintptr_t rL, int index);
	void push(uintptr_t rL, lua_State* L, int index);

	int rbxFunctionBridge(uintptr_t rL);
	int vanillaFunctionBridge(lua_State* L);
}

TValue* index2adr(lua_State *L, int idx)
{
	if (idx > 0)
	{
		TValue* o = L->base + (idx - 1);
		if (o >= L->top)
			return cast(TValue*, luaO_nilobject);
		else
			return o;
	}
	else if (idx > LUA_REGISTRYINDEX)
		return L->top + idx;
	else switch (idx)
	{  /* pseudo-indices */
	case LUA_REGISTRYINDEX:
		return registry(L);
	case LUA_ENVIRONINDEX:
		sethvalue(L, &L->env, curr_func(L)->c.env);
		return &L->env;
	case LUA_GLOBALSINDEX:
		return gt(L);
	default:
		idx = LUA_GLOBALSINDEX - idx;
		return (idx <= curr_func(L)->c.nupvalues)
			? &curr_func(L)->c.upvalue[idx - 1]
			: cast(TValue*, luaO_nilobject);
	}
}

void fakeChain(DWORD* chain)
{
	chain[1] = 0x1555555;
	((DWORD*)chain[0])[1] = 0x1555555;
}

void restoreChain(DWORD* chain, DWORD unk, DWORD nextUnk)
{
	chain[1] = unk;
	((DWORD*)chain[0])[1] = nextUnk;
}



int r_lua_pcall(int rL, int nargs, int err, int fake)
{
	DWORD* exceptionChain = (DWORD*)__readfsdword(0);
	DWORD unk = exceptionChain[1], nextUnk = ((DWORD*)exceptionChain[0])[1];
	fakeChain(exceptionChain);
	int ret = RBX::lua_pcall(rL, nargs, err, fake);
	restoreChain(exceptionChain, unk, nextUnk);

	return ret;
}

namespace Bridge
{
	struct Userdata
	{
		int32_t reference;
	};

	void cacheObject(uintptr_t rL, lua_State* L, int index)
	{
		uintptr_t rawInstancePtr = RBX::lua_touserdata(rL, index);

		lua_rawgeti(L, LUA_REGISTRYINDEX, rawInstancePtr);

		if (!lua_type(L, -1))
		{
			lua_settop(L, -2);

			RBX::lua_pushvalue(rL, index);
			reinterpret_cast<Userdata*>(lua_newuserdata(L, sizeof(Userdata)))->reference = RBX::luaL_ref(rL, -10000);

			RBX::lua_getmetatable(rL, index);
			Bridge::push(rL, L, -1);
			lua_setmetatable(L, -2);

			lua_pushvalue(L, -1);
			lua_rawseti(L, -10000, rawInstancePtr);

			RBX::lua_settop(rL, -2);
		}
	}

	void push(lua_State* L, uintptr_t rL, int index)
	{
		switch (lua_type(L, index))
		{
		case LUA_TNIL:
			RBX::lua_pushnil(rL);
			break;
		case LUA_TNUMBER:
			RBX::lua_pushnumber(rL, lua_tonumber(L, index));
			break;
		case LUA_TBOOLEAN:
			RBX::lua_pushboolean(rL, lua_toboolean(L, index));
			break;
		case LUA_TSTRING:
			RBX::lua_pushstring(rL, lua_tolstring(L, index, NULL));
			break;
		case LUA_TTHREAD:
			RBX::lua_newthread(rL);
			break;
		case LUA_TFUNCTION:

			RBX::lua_pushlightuserdata(rL, (void*)(index2adr(L, index)->value.gc));
			RBX::lua_pushcclosure(rL, int3breakpoint, 1);
			break;
		case LUA_TTABLE:
			lua_pushvalue(L, index);
			RBX::lua_createtable(rL, 0, 0);
			lua_pushnil(L);
			while (lua_next(L, -2) != LUA_TNIL)
			{
				Bridge::push(L, rL, -2);
				Bridge::push(L, rL, -1);
				RBX::lua_settable(rL, -3);
				lua_settop(L, -2);
			}
			lua_settop(L, -2);
			break;
		case LUA_TUSERDATA:
			RBX::lua_rawgeti(rL, -10000, reinterpret_cast<Userdata*>(lua_touserdata(L, index))->reference);
			if (!RBX::lua_type(rL, -1))
			{
				RBX::lua_settop(rL, -2);
				RBX::lua_newuserdata(rL, 0);
			}
			break;
		default:
			lua_assert(0);
			break;
		}
	}

	void push(uintptr_t rL, lua_State* L, int index)
	{
		switch (RBX::lua_type(rL, index))
		{
		case RLUA_TNIL:
			lua_pushnil(L);
			break;
		case RLUA_TNUMBER:
			lua_pushnumber(L, r_tonumber(rL, index));
			break;
		case RLUA_TBOOLEAN:
			lua_pushboolean(L, RBX::lua_toboolean(rL, index));
			break;
		case RLUA_TSTRING:
			lua_pushstring(L, RBX::lua_tolstring(rL, index, NULL));
			break;
		case RLUA_TTHREAD:
			lua_newthread(L);
			break;
		case RLUA_TFUNCTION:
			RBX::lua_pushvalue(rL, index);
			lua_pushnumber(L, RBX::luaL_ref(rL, -10000));
			lua_pushcclosure(L, vanillaFunctionBridge, 1);
			break;
		case RLUA_TTABLE:
			RBX::lua_pushvalue(rL, index);
			lua_createtable(L, 0, 0);
			RBX::lua_pushnil(rL);
			while (RBX::lua_next(rL, -2) != RLUA_TNIL)
			{
				Bridge::push(rL, L, -2);
				Bridge::push(rL, L, -1);
				lua_settable(L, -3);
				RBX::lua_settop(rL, -2);
			}
			RBX::lua_settop(rL, -2);
			break;
		case RLUA_TUSERDATA:
			cacheObject(rL, L, index);
			break;
		default:
			lua_assert(0);
			break;
		}
	}

	int resume(lua_State* thread)
	{
		lua_State* L = lua_tothread(thread, lua_upvalueindex(1));
		int nargs = lua_gettop(thread);
		lua_xmove(thread, L, nargs);
		return lua_resume(L, nargs);
	}

	int registry = 0;

	int vanillaFunctionBridge(lua_State* L)
	{
		RBX::lua_pushstring(g_rL, std::to_string(++registry).c_str());
		DWORD rL = RBX::lua_newthread(g_rL);
		RBX::lua_settable(g_rL, LUA_REGISTRYINDEX);

		int key = lua_tonumber(L, lua_upvalueindex(1));

		RBX::lua_rawgeti(rL, LUA_REGISTRYINDEX, key);

		for (int arg = 1; arg <= lua_gettop(L); ++arg)
			Bridge::push(L, rL, arg);

		if (r_lua_pcall(rL, lua_gettop(L), LUA_MULTRET, 0))
		{
			const char* errormessage = RBX::lua_tolstring(rL, -1, 0);

			if (!errormessage || strlen(errormessage) == 0)
				errormessage = "Error occoured, no output from Lua\n";

			if (strcmp(errormessage, "attempt to yield across metamethod/C-call boundary") == 0)
			{

				RBX::lua_settop(rL, -2);
				lua_pushthread(L);
				lua_pushcclosure(L, &resume, 1);
				Bridge::push(L, rL, -1);

				return lua_yield(L, 0);
			}

			return 0;
		}

		int args = 0;

		for (int arg = 1; arg <= RBX::lua_gettop(rL); ++arg, ++args)
			Bridge::push(rL, L, arg);

		RBX::lua_settop(rL, 0);
		return args;
	}

	int rbxFunctionBridge(uintptr_t rL)
	{
		lua_State* L = lua_newthread(g_L);
		luai_userstatethread(g_L, L);



		setclvalue(L, L->top,
			RBX::lua_touserdata(rL, lua_upvalueindex(1)));
		incr_top(L);

		int nargs = RBX::lua_gettop(rL);

		for (int i = 1; i <= nargs; i++)
			Bridge::push(rL, L, i);

		if (lua_pcall(L, nargs, LUA_MULTRET, 0))
			printf("Error Closure: %s\n",
				lua_isstring(L, -1) ? lua_tolstring(L, -1, NULL) : "Error occurred, no output from Lua.");

		int nresults = lua_gettop(L);

		for (int i = 1; i <= nresults; i++)
			Bridge::push(L, rL, i);

		lua_settop(L, 0);

		return nresults;
	}
}

void registerGlobal(uintptr_t rL, lua_State* L, const char* name)
{

	


	RBX::lua_getfield(rL, -10002, name);

	Bridge::push(rL, L, -1);
	lua_setglobal(L, name);

	RBX::lua_settop(rL, -2);
}
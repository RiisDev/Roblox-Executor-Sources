#include "rlua_wrap.h"
#include "wrap_lua.h"

const char *danghui_threadaccesskey = "k";

using namespace std; // shit lol

unsigned int getrLfromuserstate(lua_State* L)
{
	lua_pushlightuserdata(L, (void*)&danghui_threadaccesskey);
	lua_gettable(L, LUA_REGISTRYINDEX);
	unsigned int rL = lua_tointeger(L, -1);
	lua_pop(L, 1);
	return rL;
}

static void assignrL(lua_State* L, unsigned int rL)
{
	lua_pushlightuserdata(L, (void*)&danghui_threadaccesskey);
	lua_pushinteger(L, rL);
	lua_settable(L, LUA_REGISTRYINDEX);
	return;
}

static int CallMethod(lua_State* L) {
	unsigned int RobloxLuaState = getrLfromuserstate(L);
	printf("CallMethod\n");
	int ArgCount, fArgs = 0;
	try {
		printf("UnWrap Args\n");
		for (int x = lua_gettop(L); x != 0; x -= 1) {
			l_unwrap(L, RobloxLuaState, x * -1);
			ArgCount += 1;
		}
		printf("PCall Function\n");
		rlua_pcall(RobloxLuaState, ArgCount, LUA_MULTRET, 0);
		printf("Wrap Returns\n");
		for (int x = rlua_gettop(RobloxLuaState); x != 0; x -= 1) {
			r_wrap(RobloxLuaState, L, x * -1);
			fArgs += 1;
		}
	}
	catch (std::exception e) {
		std::string err = "C ERROR: ";
		err += e.what();
		std::cout << err.c_str() << std::endl;
	}
	return fArgs;
}

static int r_wrap(unsigned int rL, lua_State* L, int lastidx)
{
	switch (rlua_type(rL, lastidx))
	{
	case RLUA_TNIL:
		rlua_pushnil(rL);
	case RLUA_TSTRING:
	{
		const char* str = rlua_tostring(rL, lastidx);
		lua_pushstring(L, str);
		break;
	}
	case RLUA_TNUMBER:
	{
		printf("Wrapping at: %s\n", lua_typename(L, lastidx));
		double idx = rlua_tonumber(rL, lastidx);
		lua_pushnumber(L, idx);
		break;
	}
	case RLUA_TBOOLEAN:
	{
		bool idxswap = rlua_toboolean(rL, lastidx);
		lua_pushboolean(L, idxswap);
		break;
	}

	case RLUA_TTABLE:
	{
		rlua_pushvalue(rL, lastidx);
		lua_newtable(L);
		while (rlua_next(rL, -2) != LUA_TTABLE)
		{
			r_wrap(rL, L, -2);
			r_wrap(rL, L, -1);
			lua_settable(L, -3);
			rlua_pop(rL, 1);
		}
		rlua_pop(rL, 0);
	}

	case RLUA_TUSERDATA:
	{
		break;
	}

	case RLUA_TFUNCTION:
	{
		lua_pushcclosure(L, CallMethod, 0);
	}


	default: break;
	}

	return 1;

}

static int l_unwrap(lua_State* L, unsigned int rL, int idx)
{
	switch (lua_type(L, idx))
	{
	case LUA_TNIL:
		lua_pushnil(L);
		break;
	case LUA_TSTRING:
	{
		const char* str = lua_tostring(L, idx);
		rlua_pushstring(rL, str);
		break;
	}
	case LUA_TNUMBER:
	{
		double idxnumb = lua_tonumber(L, idx);
		rlua_pushnumber(rL, idxnumb);
		break;
	}
	case LUA_TBOOLEAN:
	{
		bool idxbool = lua_toboolean(L, idx);
		rlua_pushboolean(rL, idxbool);
		break;
	}
	case LUA_TTABLE:
	{
		break;
	}
	case LUA_TFUNCTION:
	{
		break;
	}

	}
	return 1;
}

static int Index(lua_State* L)
{
	unsigned int rL = getrLfromuserstate(L);
	lua_pop(L, 0);
	const char* Key = lua_tostring(L, -1);
	lua_pop(L, 1);
	printf("Key: %s\n", Key);
	l_unwrap(L, rL, -1);
	rlua_getfield(rL, -1, Key);
	r_wrap(rL, L, -1);
	rlua_pop(rL, 1);
	return 1;
}

static int NewIndex(lua_State* L)
{
	unsigned int rL = getrLfromuserstate(L);
	lua_pop(L, 0);
	int Type = lua_type(L, -1);
	const char* Key = lua_tostring(L, -2);
	printf("[newindex] // Key: %s\n", Key);
	l_unwrap(L, rL, -3);
	l_unwrap(L, rL, -2);
	rlua_setfield(rL, -2, Key);
	rlua_pop(rL, 1);
	return 1;
}

static int grabMetamethods(lua_State* L, const char* Type)
{
	unsigned int T = getrLfromuserstate(L);
	rlua_rawgeti(T, R_LUA_REGISTRYINDEX, *(int*)lua_touserdata(L, -2));
	printf("Rawgeti.\n");
	rlua_getmetafield(T, -1, Type);
	rlua_pushvalue(T, -2);
	rlua_rawgeti(T, R_LUA_REGISTRYINDEX, *(int*)lua_touserdata(L, -1));
	rlua_pcall(T, 2, 1, 0);
	r_wrap(T, L, -1);
	return 1;
}

void AddDataType(lua_State* L, const char* Type) {
	printf("Add data type.\n");
	unsigned int rL = RobloxLuaState;
	assignrL(L, rL);
	lua_newtable(L);
	printf("AssignrL.\n");
	lua_pushcfunction(L, Index);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, NewIndex);
	lua_setfield(L, -2, "__newindex");
	lua_pushstring(L, "The metatable is locked!");
	lua_setfield(L, -2, "__metatable");
	printf("Grabbed Index\\NewIndex.\n");
	grabMetamethods(L, "__add");
	grabMetamethods(L, "__mul");
	grabMetamethods(L, "__sub");
	grabMetamethods(L, "__pow");
	grabMetamethods(L, "__div");
	grabMetamethods(L, "__call");
	printf("Grabbed metamethods.\n");
	lua_setfield(L, -10000, Type);
}

static void WrapRobloxGlobal(int T, lua_State* L, const char* Source)
{
	rlua_getfield(T, -10002, Source);
	r_wrap(T, L, -1);
	lua_setfield(L, -10002, Source);
}

lua_State* grabLuaMainVainlla()
{
	lua_State* L = lua_open();
	luaL_openlibs(L);
	AddDataType(L, "Object");
	AddDataType(L, "CFrame");
	AddDataType(L, "Vector3");
	AddDataType(L, "UDim");
	AddDataType(L, "UDim2");
	AddDataType(L, "BrickColor");
	AddDataType(L, "Color3");
	AddDataType(L, "RBXScriptSignal");

	unsigned int T = RobloxLuaState;
	assignrL(L, T);

	

	WrapRobloxGlobal(T, L, "print");
	WrapRobloxGlobal(T, L, "printidentity");
	WrapRobloxGlobal(T, L, "warn");
	printf("Strings.\n");

	printf("[Instances].\n");
	WrapRobloxGlobal(T, L, "Instance");
	WrapRobloxGlobal(T, L, "UDim");
	WrapRobloxGlobal(T, L, "UDim2");
	WrapRobloxGlobal(T, L, "Vector3");
	WrapRobloxGlobal(T, L, "Vector2");
	WrapRobloxGlobal(T, L, "CFrame");
	WrapRobloxGlobal(T, L, "Region3");
	WrapRobloxGlobal(T, L, "PhysicalProperties");
	WrapRobloxGlobal(T, L, "BrickColor");
	WrapRobloxGlobal(T, L, "Color3");
	return L;
}

BOOL ExecuteScript(string Source)
{
	lua_State* L = grabLuaMainVainlla();
	luaL_openlibs(L);
	int ref = luaL_dostring(L, Source.c_str());
	if (ref)
	{
		printf("[LUA] ERROR: %s\n", lua_tostring(L, -1));
	}
	return FALSE;
}


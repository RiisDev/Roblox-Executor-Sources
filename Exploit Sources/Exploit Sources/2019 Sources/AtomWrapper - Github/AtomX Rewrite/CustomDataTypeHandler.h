#pragma once
namespace DataTypeHandler{

int ThreadedDataType(int LuaDataType, int RLuaDataType, lua_State* L, DWORD rL, int index) {
	if (LuaDataType == LUA_TLIGHTUSERDATA) {
		r_lua_pushlightuserdata(rL, nullptr);
	}
	else if (RLuaDataType == R_LUA_TLIGHTUSERDATA) {
		lua_pushlightuserdata(L, nullptr);
	}
	if (LuaDataType == LUA_TNIL) {
		r_lua_pushnil(rL);
	}
	else if (RLuaDataType == R_LUA_TNIL) {
		lua_pushnil(L);
	}
	if (LuaDataType == LUA_TNUMBER) {
		r_lua_pushnumber(rL, lua_tonumber(L, index));
	}
	else if (RLuaDataType == R_LUA_TNUMBER) {
		lua_pushnumber(L, r_lua_tonumber(rL, index));
	}
	if (LuaDataType == LUA_TBOOLEAN) {
		r_lua_pushboolean(rL, lua_toboolean(L, index));
	}
	else if (RLuaDataType == R_LUA_TBOOLEAN) {
		lua_pushboolean(L, r_lua_toboolean(rL, index));
	}
	return 1;
}

void WrapDataType(lua_State* L, DWORD rL, int index) {
	int LuaDataType = lua_type(L, index);
	int RLuaDataType = r_lua_type(rL, index);
	HANDLE mainHandle = NULL;
	mainHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadedDataType(LuaDataType, RLuaDataType, lua_newthread(L), rL, index), NULL, NULL, NULL);
	WaitForSingleObject(mainHandle, INFINITE);
	CloseHandle(mainHandle);
}

}
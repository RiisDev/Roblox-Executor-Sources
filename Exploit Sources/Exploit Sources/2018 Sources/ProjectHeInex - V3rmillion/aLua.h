// Defines Lua Functions (obv)
//no it isnt obv

#include <iostream>
#include <string>
#include <Windows.h>

using std::cout;
using std::endl;
using std::cin;


typedef void* (__cdecl			*lua_GetField)(int lua_State, int idx, const char *k);
typedef int(__cdecl				*lua_PCall)(int lua_State, int nargs, int nresults, int errfunc);
typedef void(__cdecl			*lua_PushString)(int lua_State, const char *s);
typedef const char *(__cdecl	*lua_ToLString)(int lua_State, int idx, size_t *len);
typedef void* (__cdecl			*lua_PushValue)(int lua_State, int idx);
typedef int(__cdecl				*lua_GetTop)(int lua_State);
typedef void(__cdecl			*lua_SetTop)(int lua_State, int n);
typedef void(__cdecl			*lua_SetField)(int lua_State, int idx, const char *k);
typedef void* (__cdecl			*lua_PushCClosure)(int lua_State, int fn(), int n);
typedef void* (__cdecl			*lua_Remove)(int lua_State, int idx);
typedef void* (__cdecl			*lua_PushNumber)(int lua_State, double Number);
typedef int(__cdecl				*lua_Type)(int lua_State, int idx);
typedef bool(__cdecl			*lua_pushboolean)(int lua_State, int a1);
typedef bool(__cdecl			*lua_toboolean)(int lua_State, int idx);
typedef int(__cdecl				*lua_pushnil)(int lua_State);
typedef int(__cdecl				*lua_next)(int lua_State, int idx);



class CLua {
public:
	void Init();
	lua_toboolean toboolean = (lua_toboolean)0x5174A0; //done
	lua_pushnil pushnil = (lua_pushnil)0x516010; //done
	lua_pushboolean pushboolean = (lua_pushboolean)0x5160A0; //done
	lua_GetTop gettop = (lua_GetTop)0x00516EA0;
	lua_GetField getfield = (lua_GetField)0x00515ED0; //done
	lua_PCall pcall = (lua_PCall)0x00517340;
	lua_PushString pushstring = (lua_PushString)0x005176B0;
	lua_ToLString tolstring = (lua_ToLString)0x00517F70;
	lua_PushValue pushvalue = (lua_PushValue)0x00517740;
	lua_PushNumber pushnumber = (lua_PushNumber)0x00516830; //done
	lua_SetField setfield = (lua_SetField)0x00516930; //done
	lua_Type type = (lua_Type)0x00518250;
	lua_SetTop settop = (lua_SetTop)0x005164E0; //done
	lua_next next = (lua_next)0x517250;

	void getglobal(int ls, const char* k)
	{
		getfield(ls, -10002, k);
	}
	bool isnil(int luaState, int n) {
		return (type(luaState, (n)) == 0);
	};
	void pop(int luaState, int n) {
		return settop(luaState, -(n)-1);
	};
	const char* tostring(int luaState, int idx) {
		return tolstring(luaState, idx, NULL);
	};
};



extern CLua lua;
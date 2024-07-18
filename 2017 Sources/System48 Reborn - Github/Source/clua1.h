#include "Includes\SysIncludes.h"
#include "retcheck.h"

bool ScanFailed = false;
Retcheck ret;

#define ScanAddress(address) (address - 0x400000 + (DWORD)GetModuleHandle(0))

#define adr_ScriptContext 0xfff7e8
#define addr_lua_getfield 0x752bb0
#define addr_lua_setfield 0x7548d0
#define addr_luaV_settable 0x751f90
#define addr_lua_pushvalue 0x754030
#define addr_lua_pcall 0x7539a0
#define addr_lua_resume 0x7546f0
#define addr_lua_rawget 0x754190
#define addr_lua_pushstring 0x753f60
#define addr_lua_remove 0x754540
#define addr_lua_setfenv 0x7547f0
#define addr_lua_pushboolean 0x753a70
#define addr_luaL_where 0x74e640
#define addr_lua_touserdata 0x7552f0
#define addr_lua_topointer 0x755240
#define addr_lua_tolstring 0x754ef0
#define addr_lua_getmetatable 0x752e80
#define addr_luaL_getmetafield 0x74dec0
#define addr_lua_setmetatable 0x754a80
#define addr_lua_pushnumber 0x753ed0
#define addr_lua_settop 0x754cb0
#define addr_lua_type 0x755320
#define addr_luaS_newlstr 0x74ed50
#define addr_lua_checkstack 0x7525c0
#define addr_luaL_ref 0x74e360
#define addr_lua_rawgeti 0x754240
#define addr_lua_objlen 0x7538b0
#define addr_lua_tonumber 0x7551c0
#define addr_lua_toboolean 0x754e30

#define SCRIPTCONTEXT 0xfff7e8
#define GETFIELD 0x752bb0
#define PUSHSTRING 0x753f60
#define PUSHVALUE 0x754030
#define PCALL 0x7539a0
#define TOBOOLEAN 0x754e30
#define PUSHNUMBER 0x753ed0
#define SETFIELD 0x7548d0
#define NEWTHREAD 0x761170
#define PUSHCCLOSURE 0x7615B0
#define TOLSTRING 0x754ef0
#define SETTOP 0x754cb0
#define TONUMBER 0x7551c0

namespace CLua {
	DWORD State;

	typedef void(__cdecl *lua_getfield)(DWORD luaState, DWORD index, const char *k);
	typedef void(__cdecl *lua_pushstring)(DWORD luaState, const char *s);
	typedef void(__cdecl *lua_pushvalue)(DWORD luaState, DWORD index);
	typedef DWORD(__cdecl *lua_pcall)(DWORD luaState, DWORD nargs, DWORD nresults, DWORD errfunc);
	typedef DWORD(__cdecl *lua_toboolean)(DWORD luaState, DWORD index);
	typedef void(__cdecl *lua_pushnumber)(DWORD luaState, double n);
	typedef void(__cdecl *lua_setfield)(DWORD luaState, DWORD index, const char *k);
	typedef DWORD(__cdecl *lua_newthread)(DWORD luaState);
	typedef void(__cdecl *lua_pushcclosure)(DWORD luaState, LPVOID hi, DWORD n);
	typedef const char*(__cdecl *lua_tolstring)(DWORD lua_State, DWORD index, size_t *len);
	typedef void(__cdecl *clua_pushbool)(DWORD, bool);
	typedef int(__cdecl* rbx_lua_settop_def)(int, int);
	typedef double(__cdecl *rbx_lua_tonumber_def)(int lua_State, int idx);


	lua_getfield _getfield = (lua_getfield)ret.unprotect((BYTE*)ScanAddress(GETFIELD));
	lua_pushstring _pushstring = (lua_pushstring)ret.unprotect((BYTE*)ScanAddress(PUSHSTRING));
	lua_pushvalue _pushvalue = (lua_pushvalue)ret.unprotect((BYTE*)ScanAddress(PUSHVALUE));
	lua_pcall _pcall = (lua_pcall)ret.unprotect((BYTE*)ScanAddress(PCALL));
	lua_toboolean toboolean = (lua_toboolean)ScanAddress(TOBOOLEAN);
	lua_pushnumber _pushnumber = (lua_pushnumber)ret.unprotect((BYTE*)ScanAddress(PUSHNUMBER));
	lua_setfield _setfield = (lua_setfield)ret.unprotect((BYTE*)ScanAddress(SETFIELD));
	lua_newthread _newthread = (lua_newthread)ret.unprotect((BYTE*)ScanAddress(NEWTHREAD));
	rbx_lua_tonumber_def _tonumber = (rbx_lua_tonumber_def)ret.unprotect((BYTE*)ScanAddress(TONUMBER));
	lua_pushcclosure _pushcclosure = (lua_pushcclosure)ret.unprotect((BYTE*)ScanAddress(PUSHCCLOSURE));
	lua_tolstring _tolstring = (lua_tolstring)ret.unprotect((BYTE*)ScanAddress(TOLSTRING));
	rbx_lua_settop_def rbx_lua_settop = (rbx_lua_settop_def)ret.unprotect(reinterpret_cast<byte*>(ScanAddress(SETTOP)));


	void WriteMemory(DWORD Address, DWORD Instruction) {
		DWORD oldProtection;
		VirtualProtect((LPVOID)Address, 1, PAGE_EXECUTE_READWRITE, &oldProtection);
		*(char*)Address = Instruction;
		VirtualProtect((LPVOID)Address, 1, oldProtection, &oldProtection);
	}

	void getfield(DWORD luaState, DWORD index, const char *k) {
		_getfield(luaState, index, k);
	}

	void settop(DWORD luaState, DWORD idx) {
		rbx_lua_settop(luaState, idx);
	}

	void pcall(DWORD luaState, DWORD nargs, DWORD nresults, DWORD errfunc) {
		_pcall(luaState, nargs, nresults, errfunc);
	}

#define call(rbx_L, nargs, nresults) pcall(rbx_L, nargs, nresults, 0)

	void pushstring(DWORD luaState, const char *s) {
		_pushstring(luaState, s);
	}

	void pushvalue(DWORD luaState, DWORD index) {
		_pushvalue(luaState, index);
	}

	DWORD newthread(DWORD luaState) {
		DWORD x = _newthread(luaState);
		return x;
	}

	void pushnumber(DWORD luaState, double n) {
		_pushnumber(luaState, n);
	}

	void setfield(DWORD luaState, DWORD index, const char *k) {
		_setfield(luaState, index, k);
	}

	double tonumber(DWORD luaState, DWORD index) {
		return _tonumber(luaState, index);
	}

	void pushcclosure(DWORD luaState, void* func, DWORD n) {
		_pushcclosure(luaState, func, n);
	}

	void pop(int State, int n) {
		return settop(State, -(n)-1);
	}

	void pushtrue(int State) {
		*(DWORD *)(*(DWORD *)(State + 16) + 8) = 1;
		*(DWORD *)(State + 16) += 16;
	}

	const char* tostring(DWORD luaState, DWORD index) {
		const char* result = _tolstring(luaState, index, 0);
		return result;
	}

	void getglobal(DWORD luaState, const char *k) {
		return getfield(luaState, -10002, k);
	}

	void pushnil(int State) {
		*(DWORD *)(*(DWORD *)(State + 16) + 8) = 0;
		*(DWORD *)(State + 16) += 16;
	}

	void pushcfunction(DWORD luaState, void* func) {
		return pushcclosure(luaState, func, 0);
	}

	void clearstack(DWORD luaState) {
		return settop(luaState, 0);
	}

	void pushboolean(int RL, bool boolean)
	{
		int v4 = *(DWORD*)(RL + 16);
		*(DWORD*)v4 = boolean;
		*(DWORD*)(v4 + 8) = 3;
		*(DWORD*)(RL + 16) += 16;
	}
}
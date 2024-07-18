#include <Windows.h>
#include "Retcheck.h"

Retcheck ret;

#define ScanAddress(address) (address - \ + (DWORD)GetModuleHandle(0))

#define SCRIPTCONTEXT 0x112E5E4 //upd8
#define GETFIELD 0x4FE3C0
#define SETTOP 0x5002A0
#define PUSHSTRING 0x4FF600
#define PUSHVALUE 0x4FF6C0
#define PCALL 0x4FF0A0
#define TOBOOLEAN 0x500410
#define PUSHNUMBER 0x4FF580
#define SETFIELD 0x4FFF10
#define NEWTHREAD 0x4FEDA0
#define PUSHCCLOSURE 0x4FF1D0
#define TOLSTRING 0x5004D0

namespace CLua {
	DWORD State;
	typedef void(__cdecl *lua_getfield)(DWORD luaState, DWORD index, const char *k);
	typedef void(__cdecl *lua_settop)(DWORD luaState, DWORD s);
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


	lua_getfield _getfield = (lua_getfield)ScanAddress(GETFIELD);
	lua_settop _settop = (lua_settop)ScanAddress(SETTOP);
	lua_pushstring _pushstring = (lua_pushstring)ScanAddress(PUSHSTRING);
	lua_pushvalue _pushvalue = (lua_pushvalue)ScanAddress(PUSHVALUE);
	lua_pcall _pcall = (lua_pcall)ScanAddress(PCALL);
	lua_toboolean toboolean = (lua_toboolean)ScanAddress(TOBOOLEAN);
	lua_pushnumber _pushnumber = (lua_pushnumber)ScanAddress(PUSHNUMBER);
	lua_setfield _setfield = (lua_setfield)ScanAddress(SETFIELD);
	lua_newthread _newthread = (lua_newthread)ScanAddress(NEWTHREAD);
	lua_pushcclosure _pushcclosure = (lua_pushcclosure)ScanAddress(PUSHCCLOSURE);
	lua_tolstring _tolstring = (lua_tolstring)ScanAddress(TOLSTRING);
	//clua_pushbool _pushbool = (clua_pushbool)ScanAddress(PUSHBOOL);


	void WriteMemory(DWORD Address, DWORD Instruction) {
		DWORD oldProtection;
		VirtualProtect((LPVOID)Address, 1, PAGE_EXECUTE_READWRITE, &oldProtection);
		*(char*)Address = Instruction;
		VirtualProtect((LPVOID)Address, 1, oldProtection, &oldProtection);
	}

	DWORD Decrypt_ptr(DWORD Pointer) {
		return (*(DWORD*)((char*)Pointer + *(DWORD*)Pointer))--;
	}

	void getfield(DWORD luaState, DWORD index, const char *k) {
		_getfield(luaState, index, k);
	}

	void settop(DWORD luaState, DWORD s) {
		_settop(luaState, s);
	}

	void pcall(DWORD luaState, DWORD nargs, DWORD nresults, DWORD errfunc) {
		_pcall(luaState, nargs, nresults, errfunc);
	}

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

	void pushcclosure(DWORD luaState, void* func, DWORD n) {
		_pushcclosure(luaState, func, n);
	}

	void pushbool(DWORD State, bool S) {
		//_pushbool(State, S);
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
}


const int scriptcontext_addr = 0xD2E5E4;
const int luaV_execute_addr = 0x90060;
const int luaV_gettable_addr = 0x92190;
const int luaV_settable_addr = 0x923D0;
const int luaM_realloc_addr = 0x8EFD0;
const int luaD_precall_addr = 0x8C8F0;
const int lua_newthread_addr = 0x938C0;
const int luaS_newlstr_addr = 0x8F6A0;
const int lua_setfield_addr = 0x94A30;
const int lua_pushcclosure_addr = 0x93CF0;
const int lua_tolstring_addr = 0x94FF0;
const int lua_index2adr_addr = 0x8B8E0;
const int luaF_close_addr = 0x8D020;
const int luaH_new_addr = 0x8DB40;
const int luaV_concat_addr = 0x8FC40;
const int xorconstant_addr = 0x1087DB0;
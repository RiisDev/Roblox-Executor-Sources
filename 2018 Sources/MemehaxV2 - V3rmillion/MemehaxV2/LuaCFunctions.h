#include <windows.h>
#include <string>
#include "BasicMemory.h"
#include "ReturnCheck.h"
#include "RawrCheckXD.h"
#include "MemoryLocations.h"

/*
	memehax ~ brought to you by tepig#3337
	9-16-2017
*/

#ifndef nigeria
#define nigeria

#define rbx_pop(L, n) LuaC::rbx_settop(L, -(n)-2)

using std::string;


class LuaC {
	typedef void(*luagetfield) (unsigned long L, int idx, const char *k, char oof);
	typedef void(*luasetfield) (unsigned long L, int idx, const char *k);
	typedef void(*luapushvalue) (unsigned long L, int idx);
	typedef void(*luapushstring) (unsigned long L, const char* s);
	typedef void(*luapushnumber) (unsigned long L, double n);
	typedef void(*luapushcclosure) (unsigned long L, int fn, int n);
	typedef int(*luapcall) (unsigned long L, int nargs, int nresults, int errfunc);
	typedef int(*luanext) (unsigned long L, int idx);
	typedef int(*luatoboolean) (unsigned long L, int idx);
	typedef int(*luatonumber) (unsigned long L, int idx);
	typedef const char* (*luatolstring) (unsigned long L, int idx, unsigned int len);
public:

	static void rbx_getfield(int L, int idx, string k) {
		luagetfield LuaGetfield = (luagetfield)location_getfield;
		BypassRet();
		LuaGetfield(L, idx, k.c_str(), 1);
		RestoreRet();
	}

	static void rbx_setfield(int L, int idx, string k)
	{
		luasetfield LuaSetfield = (luasetfield)location_setfield;
		BypassRet();
		LuaSetfield(L, idx, k.c_str());
		RestoreRet();
	}

	static void rbx_getglobal(int L, string k)
	{
		LuaC::rbx_getfield(L, -10002, k);
	}

	static void rbx_setglobal(int L, string k)
	{
		LuaC::rbx_setfield(L, -10003, k);
	}

	static void rbx_settop(int L, int idx) {
		int i;
		int a1 = L;
		int a2 = idx;
		if (a2 < 0)
		{
			*(DWORD *)(a1 + 16) += 16 * a2 + 16;
		}
		else
		{
			for (i = 16 * a2; *(DWORD *)(a1 + 16) < (unsigned int)(i + *(DWORD *)(a1 + 24)); *(DWORD *)(a1 + 16) += 16)
				*(DWORD *)(*(DWORD *)(a1 + 16) + 8) = 0;
			*(DWORD *)(a1 + 16) = i + *(DWORD *)(a1 + 24);
		}
	}

	static void rbx_pcall(int L, int nargs, int nresults, int errfunc) {
		luapcall LuaPcall = (luapcall)location_pcall;
		BypassRet();
		BypassRawr();
		LuaPcall(L, nargs, nresults, errfunc);
		RestoreRet();
		RestoreRawr();
	}

	static void rbx_pushcclosure(int L, int fn, int n) {
		luapushcclosure LuaPushcclosure = (luapushcclosure)location_pushcclosure;
		BypassRet();
		LuaPushcclosure(L, fn, n);
		RestoreRet();
	}

	static void rbx_pushnil(int L)
	{
		*(DWORD *)(*(DWORD *)(L + 16) + 8) = 0;
		*(DWORD *)(L + 16) += 16;
	}

	static void rbx_pushvalue(int L, int v) {
		luapushvalue LuaPushvalue = (luapushvalue)location_pushvalue;
		BypassRet();
		LuaPushvalue(L, v);
		RestoreRet();
	}

	static void rbx_pushnumber(int L, int n) {
		luapushnumber LuaPushnumber = (luapushnumber)location_pushnumber;
		LuaPushnumber(L, n);
	}

	static void rbx_pushstring(int L, string s)
	{
		luapushstring LuaPushstring = (luapushstring)location_pushstring;
		LuaPushstring(L, s.c_str());
	}

	static void rbx_pushboolean(int L, int a2)
	{
		int v2;
		v2 = *(DWORD *)(L + 16);
		*(DWORD *)v2 = a2 != 0;
		*(DWORD *)(v2 + 8) = 3;
		*(DWORD *)(L + 16) += 16;
	}

	static int rbx_next(int L, DWORD idx)
	{
		luanext LuaNext = (luanext)location_next;
		BypassRet();
		int nextval = LuaNext(L, idx);
		RestoreRet();
		return nextval;
	}

	static const char* rbx_tolstring(int L, int idx, int len) {
		luatolstring LuaTolstring = (luatolstring)location_tolstring;
		BypassRet();
		const char* meme = LuaTolstring(L, idx, len);
		const char* lol;
		RestoreRet();
		return lol;
	}

	static void rbx_getservice(int L, std::string russia) {
		rbx_getglobal(L, "game");
		rbx_getfield(L, -1, "GetService");
		rbx_pushvalue(L, -1);
		rbx_pushstring(L, russia);
		rbx_pcall(L, 2, 1, 1);
	}
};

#endif
#pragma once

#include "RevBase.hpp"
#include "../Lua/lobject.h"
#define rev_luai_numadd(a,b)        ((a)+(b))
#define rev_luai_numsub(a,b)        ((a)-(b))
#define rev_luai_nummul(a,b)        ((a)*(b))
#define rev_luai_numdiv(a,b)        ((a)/(b))
#define rev_luai_nummod(a,b)        ((a) - floor((a)/(b))*(b))
#define rev_luai_numpow(a,b)        (pow(a,b))
#define rev_luai_numunm(a)          (-(a))
#define rev_luai_numeq(a,b)         ((a)==(b))
#define rev_luai_numlt(a,b)         ((a)<(b))
#define rev_luai_numle(a,b)         ((a)<=(b))
#define rev_luai_numisnan(a)        (!rev_luai_numeq((a), (a)))

#define r_BASE 0x400000
#define r_MODULEBASE (DWORD)GetModuleHandle("RobloxPlayerBeta.exe")
#define rev_Offset(x) (x - r_BASE + r_MODULEBASE)
#define r_luaV_gettable_addy 0x84c5f0 
//55 8B EC 83 EC 08 53 8B  5D 0C 56 57 C7 45 FC 00
#define r_luaV_settable_addy 0x84C8F0
//55 8B EC 83 EC 0C 8B 4D  0C 56 8B 75 08 57 C7 45
#define r_lua_setfield_addy 0x50DAA0
//55 8B EC 83 EC 10 53 56  8B 75 08 57 FF 75 0C 56
#define r_luaF_close_addy 0x50B280
//55 8B EC 53 57 8B 7D 08  8B 5F 08 8D 47 08 03 D8
VOID RunScript(DWORD rL, lua_State* L, const std::string& source);

void rev_luaV_execute(DWORD rL, lua_State *L, int nexeccalls);

namespace RBX {
	//Enum TMS declare, IDS depend on order do not change
	typedef enum {
		TM_INDEX,
		TM_GC,
		TM_NEWINDEX,
		TM_MODE,
		TM_EQ,
		TM_MUL,
		TM_DIV,
		TM_ADD,
		TM_MOD,
		TM_UNM,
		TM_SUB,
		TM_POW,
		TM_CALL,
		TM_LEN,
		TM_LE,
		TM_LT,
		TM_CONCAT
	} TMS;
}
typedef int(__cdecl* rNewThread)(int);
typedef void* (__cdecl* rMRealloc)(uintptr_t, void*, int, size_t, size_t);
namespace rblx {
	typedef void(__cdecl* lua_setfield)(DWORD a, int b, const char* c);
	typedef int(__cdecl* luaV_gettable)(DWORD a, DWORD b, DWORD c, DWORD d);
	typedef int(__cdecl* luaV_settable)(DWORD a, DWORD b, DWORD c, DWORD d);
	typedef int(__cdecl* luaD_precall)(DWORD a, DWORD b, DWORD c);
	typedef void(__cdecl* luaF_close)(DWORD a, DWORD b);
}

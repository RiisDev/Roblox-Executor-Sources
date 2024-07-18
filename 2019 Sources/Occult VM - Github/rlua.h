#pragma once

#include "../stdafx.h"

#define OCCULT_DEBUG
#define AUTO auto

#ifdef OCCULT_DEBUG
#define DbgPrint OutputText
#else
#define DbgPrint __noop
#endif


#define OCCULT_IDABASE 0x400000
#define OCCULT_OFFSET(b, x) (OCCULT_MODULEBASE + ((x) - (b)))

//55 8B EC 53 57 8B 7D 08 8B 5F 08 8D 47 08 03 D8 83 7F 78 00 0F 84 ? ? ? ? 56 EB 03 8D 49 00 
#define OCCULT_RLUAF_CLOSE 0x537570  // Updated 1/10

#define OCCULT_RLUAV_XORCONST 0x1A1D3D0	// Updated 1/10

//aob broken
#define OCCULT_RLUAV_GETTABLE 0x54AB50 // Updated 1/10

//aob broken
#define OCCULT_RLUAV_SETTABLE 0x541470 // Updated 1/10

//aob broken 
#define OCCULT_RLUAD_PRECALL 0x5456D0 // Updated 1/10
#define OCCULT_RLUAD_PRECALL_RTCHECK 0x5459BD // not needed anymore
//55 8B EC 51 8B 45 10 53 56 8B F0 C1 EE 05 46 57 8B D8 8B D0 3B C6 72 26 EB 06 8D 9B 00 00 00 00
#define OCCULT_RLUAS_NEWLSTR 0x539CE0  // Updated 1/10

//55 8B EC 53 8B 5D 08 56 8B 75 14 57 8B 7B 08 8D 43 08 56 FF 75 10 03 F8 FF 75 0C 8B 47 10 FF 77 0C 
#define OCCULT_RLUAM_REALLOC 0x539610  // Updated 1/10

//55 8B EC 6A FF 68 ? ? ? ? 64 A1 ? ? ? ? 50 64 89 25 ? ? ? ? 51 56 8B 75 08 57 8B 4E 08 
#define OCCULT_RLUA_NEWTHREAD 0x53DA30			// Updated 1/10
#define OCCULT_RLUA_NEWTHREAD_RTCHECK 0x54C400	// not needed

//55 8B EC 56 8B 75 08 6A 00 6A 00 56 E8 ? ? ? ? 6A 00 6A 00 56 E8 ? ? ? ? 68 ? ? ? ?
#define OCCULT_RLUA_INITTHREAD 0x539390 // Updated 11/18

//55 8B EC 57 6A 4C 6A 00 6A 00 FF 75 08 E8 ? ? ? ? 8B F8 6A 09 57 FF 75 08 E8 ? ? ? ? 8D 4F 20 
#define OCCULT_RLUAG_NEWPROTO 0x53CA20 // not needed

//55 8B EC 57 6A 20 6A 00 6A 00 FF 75 08 E8 ? ? ? ? 8B F8 6A 0A 57 FF 75 08 E8 ? ? ? ? 8D 4F 10
#define OCCULT_RLUAG_NEWUPVALUE 0x53CAD0 // not needed??

//55 8B EC 56 8B 75 08 8B 4E 08 8B 44 31 60 3B 44 31 54 72 09 56 E8 ? ? ? ? 83 C4 04 8B 46 0C 
#define OCCULT_RLUA_PUSHCCLOSURE 0x53DE60 // not used Updated 1/10

//unable to get sig for (more than one reference)
#define OCCULT_RLUA_SETFIELD 0x53EBC0		  // Updated 1/10
#define OCCULT_RLUA_SETFIELDRETCHECK 0x54D576 // not needed

//53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 56 8B 73 08 
#define OCCULT_RLUAV_EXECUTE 0x53A630 // Updated 1/10

//55 8B EC 53 56 57 8B 7D 08 8B 77 10 85 F6 74 49 8D 5F 1C 8B C6 C1 E0 04 03 03 83 7C 18 ? ? 75 38 
#define OCCULT_RLUAH_GETN 0x537F00 // Updated 1/10

//55 8B EC 83 EC 10 FF 75 10 FF 75 0C E8 ? ? ? ? 83 C4 08 3D ? ? ? ? 75 33 66 0F 6E 4D ? 
#define OCCULT_RLUAH_SETNUM 0x5382E0 // Updated 1/10

//55 8B EC 8B 55 0C 8B 4A 14 8D 42 14 03 C8 81 F9 ? ? ? ? 75 14 33 C0 50 FF 75 10 52 FF 75 08 
#define OCCULT_RLUAH_RESIZEARRAY 0x538220 // Updated 1/10

//aob broke
#define OCCULT_RLUAH_NEW 0x5468D0 // Updated 11/18

//55 8B EC 53 56 8B 75 08 57 8B 7D 0C 8B 4E 18 8D 5F 06 89 4D 08 8D 43 01 3D ? ? ? ? 77 19 8B C3 
#define OCCULT_RLUAD_GROWSTACK 0x537270 // Updated 1/10

//aob broke
#define OCCULT_RVMCALL_BINTIM 0x542960 // Updated 11/18

#define OCCULT_RLUAO_NILOBJECT 0x1162890 // not needed

#define OCCULT_RLUA_TOUSERDATA 0x54DD90 // Updated 11/18

//havent tested this
//55 8B EC 83 EC 08 8B 55 08 53 8D 4A 08 56 57 8B 39 03 F9 89 4D FC 8B 47 5C 8D 1C 80 03 DB B8 ? ? ? ? 
#define OCCULT_RLUAC_STEP 0x536950 // Updated 1/10
#define OCCULT_RLUAC_LINK 0x81CCC0 // not used???

//havent tested this one either
//55 8B EC 53 8B 5D 0C 8B C3 57 C1 E0 04 83 C0 18 50 6A 00 6A 00 FF 75 08 E8 ? ? ? ? 8B F8 6A 06
#define OCCULT_RLUAF_NEWCCLOSURE 0x6178F8 // Updated 1/10


//C7 05 ? ? ? ? ? ? ? ? E8 ? ? ? ? 8B F0 56 E8 ? ? ? ? 83 C4 04 3D ? ? ? ? 74 2F 
//inside of
#define OCCULT_IDENTITYFlAG 0x6178F8     // Updated 1/10
#define OCCULT_DEFAULTIDENTITY 0x1A29DF0 // Updated 11/18


#define OCCULT_RETCHECK_FLAG1 0x1450ab0 // Updated 1/10
#define OCCULT_RETCHECK_FLAG2 0x1a090e4 // Updated 1/10
#define OCCULT_RETCHECK_ADDY 0x543340 // Updated 1/10

//55 8B EC 83 EC 10 56 6A 00 FF 75 0C FF 75 08 E8 ? ? ? ? 8B D0 83 C4 0C 8D 72 01 8D 64 24 00 
#define OCCULT_RLUA_TOSTRING 0x53F180 // Updated 1/10

//55 8B EC 56 57 FF 75 0C 8B 7D 08 57 E8 ? ? ? ? 8B 50 08 8B CA 83 C4 08 83 E9 07 74 19 49 74 0F
#define OCCULT_RLUA_GETMETATABLE 0x53D2F0 // Updated 1/10
#define OCCULT_RLUA_GETMETATABLERETCHECK 0x54BB9D // Updated 11/17 (should really stop using this tho)

// globalStates[vm_index] thingy
#define OCCULT_STATE_GRABBER 0x520370

#define OCCULT_RLUA_PCALL 0x53DD30 // Updated 1/10

#define RA(i)   (base+GETARG_A(i))
#define RB(i)   check_exp(getBMode(GET_OPCODE(i)) == OpArgR, base+GETARG_B(i))
#define RC(i)   check_exp(getCMode(GET_OPCODE(i)) == OpArgR, base+GETARG_C(i))
#define RKB(i)  check_exp(getBMode(GET_OPCODE(i)) == OpArgK, \
        ISK(GETARG_B(i)) ? k+INDEXK(GETARG_B(i)) : base+GETARG_B(i))
#define RKC(i)  check_exp(getCMode(GET_OPCODE(i)) == OpArgK, \
        ISK(GETARG_C(i)) ? k+INDEXK(GETARG_C(i)) : base+GETARG_C(i))
#define KBx(i)  check_exp(getBMode(GET_OPCODE(i)) == OpArgK, k+GETARG_Bx(i))

#define XRC(rL, i) 0; if (ISK(GETARG_C(i))) C = (DWORD)&(ccspool[INDEXK(GETARG_C(i))]); else C = Occult_luaV_getonstack(rL, GETARG_C(i))
#define XRB(rL, i) 0; if (ISK(GETARG_B(i))) B = (DWORD)&(ccspool[INDEXK(GETARG_B(i))]); else B = Occult_luaV_getonstack(rL, GETARG_B(i))
#define ttval(val) ((TValue*)(val))
#define dojmp(L, pc, i) (pc) += (i) // Straight outta Lua LOL
#define getnval(t) (Occult_luaX_ConvertNumber(t->value.n))
#define lisfalse(t) ((t->tt == R_LUA_TNIL) || (t->tt == R_LUA_TBOOLEAN && t->value.b == 0))
#define tonumber(o, n) (o->tt == R_LUA_TNUMBER || ((o) = Occult_luaV_tonumber(o, n)) != NULL)
#define UNREF(var) (void)(var)

#define r_luaM_malloc(rL, t)  r_luaM_realloc_(rL, NULL, 0, (t))
#define r_luaX_increasestack(rL) *(DWORD *)(rL + 16) += 16
#define r_luaX_growstack(rL, x) *(DWORD*)(rL + 16) += (16 * x)

#define Occult_luaD_savestack(rL, p) ((char *)(p) - (char *)(*(DWORD*)(rL + 20)))
#define Occult_luaD_restorestack(rL, n) (TValue*)(rL + 20) + (n);
#define Occult_rawPush(rL, v, t) { int v2 = *(DWORD*)(rL + 16); \
					*(DWORD*)(v2 + 8) = t; \
					*(DWORD*)v2 = v; \
					*(DWORD*)(rL + 16) += 16; } 

#define r_ptr_obfus(p, v) *(DWORD*)(p) = (DWORD)v - (p)
#define r_ptr_deobf(p) *(DWORD*)(p) + (p)

#define R_LUA_TNIL 0
#define R_LUA_TLIGHTUSERDATA 1
#define R_LUA_TNUMBER 2
#define R_LUA_TBOOLEAN 3
#define R_LUA_TSTRING 4
#define R_LUA_TTHREAD 5
#define R_LUA_TFUNCTION 6
#define R_LUA_TTABLE 7
#define R_LUA_TUSERDATA 8
#define R_LUA_TPROTO 9
#define R_LUA_TUPVALUE 10

typedef enum {
	R_TM_MODE,
	R_TM_NEWINDEX,
	R_TM_INDEX,
	R_TM_GC,
	R_TM_EQ,
	R_TM_MUL,
	R_TM_DIV,
	R_TM_ADD,
	R_TM_MOD,
	R_TM_UNM,
	R_TM_SUB,
	R_TM_POW,
	R_TM_CALL,
	R_TM_LEN,
	R_TM_LE,
	R_TM_LT,
	R_TM_CONCAT
} R_TMS;

struct LuaInfo {
	BYTE Identifier;
	Proto* p;
};

struct PUserdata {
	TValue* cl;
	lua_State* L;
	DWORD rL;
};

extern "C" int luaL_loadbuffer(lua_State* L, const char *buff, size_t sz, const char *name);
extern "C" Closure *luaF_newLclosure(lua_State *L, int nelems, void *e);
extern "C" int luaD_precall(lua_State *L, StkId func, int nresults);
extern "C" int luaD_poscall(lua_State *L, StkId firstResult);

typedef DWORD(__cdecl* rlua_CFunction)(DWORD rL);
typedef int(__cdecl* _r_luaV_gettable)(DWORD rL, DWORD A, DWORD B, DWORD C);
typedef int(__cdecl* _r_luaV_settable)(DWORD rL, DWORD A, DWORD B, DWORD C); // i mean its the same thing but :uuu
typedef signed int(__cdecl* _r_luaD_precall)(DWORD rL, DWORD A, DWORD B);
typedef DWORD(__cdecl* _r_luaS_newlstr)(DWORD rL, const char* str, size_t B);
typedef void*(__cdecl* _r_luaM_realloc_)(DWORD rL, void *block, size_t osize, size_t nsize);
typedef DWORD(__cdecl* _r_lua_newthread)(DWORD rL);
typedef int(__cdecl* _r_luaO_fb2int)(int x);
typedef DWORD(__cdecl* _r_luaH_new)(DWORD rL, int narray, int lnhash);
typedef void(__cdecl* _r_luaH_resizearray)(DWORD rL, void* table, int nasize);
typedef int(__cdecl* _r_call_binTM)(DWORD rL, DWORD p1, DWORD p2, DWORD res, DWORD evnt);
typedef void(__cdecl* _r_luaF_close)(DWORD rL, DWORD level);
typedef DWORD(__cdecl* _r_luaH_setnum)(DWORD rL, void* t, int key);
typedef int(__cdecl* _r_luaH_getn)(void* t);
typedef int(__cdecl* _r_luaD_growstack)(DWORD rL, int sz);
typedef DWORD(__cdecl* _r_luaG_newproto)(DWORD rL);
typedef DWORD(__cdecl* _r_luaG_newupvalue)(DWORD rL);
typedef const char*(__cdecl* _r_rLua_tostring)(DWORD rL, DWORD index);
typedef void(__cdecl* _r_lua_setfield)(DWORD rL, int idx, const char* key);
typedef void(__cdecl* _r_lua_pcall)(DWORD rL, int args, int ret, int errfunc);
typedef void(__cdecl* _r_lua_gc)(DWORD rL, int idk1, int idk2);

DWORD Occult_luaR_index2adr(DWORD rL, signed int idx);
DWORD Occult_luaR_type(DWORD rL, signed int idx);
int Occult_luaD_poscall(lua_State* L, DWORD rL, StkId firstResult);
auto Occult_luaX_backjump(DWORD jmp);
DWORD __cdecl Occult_luaX_gatecall(DWORD rL);
double Occult_luaX_ConvertNumber(double Number);
void Occult_luaX_Arith(DWORD rL, DWORD A, DWORD B, DWORD C, R_TMS op);
void Occult_luaX_dcall(lua_State* L, DWORD rL, Proto* p, LClosure* lcl, TValue* Func, int nResults);
auto Occult_luaX_backretcheck();
int Occult_luaX_pushcclosure(DWORD rL, rlua_CFunction a2, int a3);
DWORD Occult_luaX_getmetatable(DWORD rL, DWORD objindex);
int Occult_luaO_fb2int(int x);
int Occult_luaO_str2d(const char *s, double *result);
Closure* Occult_luaF_newLclosure(DWORD rL, int nelems, void *e);
void Occult_luaB_settop(DWORD rL, DWORD idx);
int Occult_luaB_gettop(DWORD rL);
DWORD Occult_luaV_getonstack(DWORD rL, DWORD idx);
BOOL Occult_luaV_convertTValue(lua_State* L, DWORD rL, TValue* From, TValue* Destination);
bool Occult_luaV_equalVal(DWORD rL, TValue* t1, TValue* t2);
TValue* Occult_luaV_tonumber(TValue* obj, TValue* n);
void Occult_r_luaV_execute(DWORD rL, int nexeccalls);
BOOL Occult_luaV_execute(lua_State* L, DWORD rL, int nexeccalls);
BOOL Occult_luaA_run(DWORD rL, const char* src);

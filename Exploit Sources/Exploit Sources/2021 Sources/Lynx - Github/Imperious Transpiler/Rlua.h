#pragma once
#include <cstdint>
#include <queue> 
#include "RetCheck.h"

extern "C" {
#include "Lua/lua.h"
#include "Lua/lauxlib.h"
#include "Lua/lualib.h"
#include "Lua/lobject.h"
#include "Lua/lstate.h"
#include "Lua/lfunc.h"
#include "Lua/lopcodes.h"
#include "Lua/lstring.h"
#include "Lua/ldo.h"
#include "Lua/llex.h"
#include "Lua/lvm.h"
}


std::queue<std::string> ScriptQueue;

DWORD EmptySpace1 = 108;
DWORD EmptySpace2 = 24;

DWORD Add(DWORD func);
typedef int (*lRua_CFunction) (DWORD rL);
#define ASLR(x)(x - 0x400000 + (DWORD)GetModuleHandleA(0))

#define r_lua_pop(rL,n)		r_lua_settop(rL, -(n)-1)
#define r_lua_getglobal(rL,s)	r_lua_getfield(rL, LUA_GLOBALSINDEX, (s))
#define r_lua_setglobal(rL,s)	r_lua_setfield(rL, LUA_GLOBALSINDEX, (s))

#define RLUA_TNIL 0
#define RLUA_TLIGHTUSERDATA 2
#define RLUA_TNUMBER 4
#define RLUA_TBOOLEAN 1
#define RLUA_TSTRING 5
#define RLUA_TTHREAD 7
#define RLUA_TFUNCTION 6
#define RLUA_TTABLE 8
#define RLUA_TUSERDATA 9
#define R_LUA_TPROTO 10
#define R_LUA_TUPVALUE 11

DWORD RL;
lua_State* L;
std::string LBI;

using t_d = int(__cdecl*)(DWORD v1, const char* v2, const char* v3, size_t v4);
t_d r_lua_deserialize = (t_d)unprotect(ASLR(0x11d75e0));//updated

using s_t = void(__cdecl*)(DWORD v1);
s_t r_lua_spawn = (s_t)unprotect(ASLR(0x726140));//updated

using rgetfield = int(__stdcall*)(DWORD rL, int idx, const char* k);
rgetfield r_lua_getfield = (rgetfield)(unprotect(ASLR(0x11CC5C0)));

using sf_t = void(__stdcall*)(DWORD v1, int v2, const char* v3);
sf_t r_lua_setfield = (sf_t)unprotect(ASLR(0x11CDA30));//updated

using ps_t = void(__cdecl*)(DWORD v1, const char* v2, int v3);
ps_t r_lua_pushlstring = (ps_t)unprotect(ASLR(0x11CD050));//updated 

using pushvalue = void(__fastcall*)(DWORD rL, DWORD idx);
pushvalue r_lua_pushvalue = (pushvalue)(unprotect(ASLR(0x11CD320)));

using rpushcclosure = void(__fastcall*)(DWORD rL, int fn, int non, int a1, int xd);
rpushcclosure r_luax_pushcclosure = (rpushcclosure)(unprotect(ASLR(0x11CCE60)));

using rnewthread = DWORD(__cdecl*)(DWORD);
rnewthread r_lua_newthread = (rnewthread)(unprotect(ASLR(0x11CCAD0)));

using rsettop = void(__cdecl*)(DWORD rL, int idx);
rsettop r_lua_settop = (rsettop)(unprotect(ASLR(0x11cdd50)));

using rLua_pcall = int(__cdecl*)(DWORD lst, int nargs, int nresults, int errfunc);
rLua_pcall r_lua_pcall = (rLua_pcall)(unprotect(ASLR(0x11CCD50)));

using rtolstring = char* (__stdcall*)(DWORD rL, int idx, size_t* size);
rtolstring r_lua_tolstring = (rtolstring)(unprotect(ASLR(0x11CDF50)));

using rtonumber = double(__cdecl*)(DWORD, int, int);
rtonumber r_lua_tonumber = (rtonumber)(ASLR(0x11CE080));

using rtype = DWORD(__cdecl*)(DWORD, int);
rtype r_lua_type = (rtype)(unprotect(ASLR(0x11CE400)));

using rpushstring = void(__cdecl*)(DWORD rL, const char*);
rpushstring r_lua_pushstring = (rpushstring)(unprotect(ASLR(0x11CD170)));

using pushnumber = double(__thiscall*)(DWORD rL, double idx);
pushnumber r_lua_pushnumber = (pushnumber)(unprotect(ASLR(0x11CD110)));

using rgetmetatable = void* (__cdecl*)(DWORD rL, int idx);
rgetmetatable r_lua_getmetatable = (rgetmetatable)(unprotect(ASLR(0x11CC670)));

using rboolean = int(__cdecl*)(unsigned int, int);
rboolean r_lua_pushboolean = (rboolean)(unprotect(ASLR(0x11cce10)));

using rxmove = void(__cdecl*)(DWORD, DWORD, DWORD);
rxmove r_lua_xmove = (rxmove)(unprotect(ASLR(0x11ce4a0)));

using checktype = void(__cdecl*)(int, int, int);
checktype rluaL_checktype = (checktype)(unprotect(ASLR(0x11ce910)));

using checknumber = lua_Number(__cdecl*)(int, int);
checknumber rluaL_checknumber = (checknumber)(unprotect(ASLR(0x11CE8A0)));

using checklstring = const char* (__cdecl*)(int, int, unsigned int*);
checklstring rluaL_checklstring = (checklstring)(unprotect(ASLR(0x11CE870)));



using rtouserdata = void* (__cdecl*)(DWORD, int);
rtouserdata r_lua_touserdata = (rtouserdata)(unprotect(ASLR(0x11CE310)));

using rsettable = void* (__cdecl*)(DWORD rL, int);
rsettable r_lua_settable = (rsettable)(unprotect(ASLR(0x11CDCD0)));

using rcreatetable = void(__cdecl*)(DWORD rL, int num, int fix);
rcreatetable r_lua_createtable = (rcreatetable)(unprotect(ASLR(0x11CC3A0)));

using rcfunc = bool(__cdecl*)(DWORD, DWORD);
rcfunc r_lua_iscfunction = (rcfunc)(unprotect(ASLR(0x11CC8D0)));

using tptr = std::uintptr_t(__cdecl*)(DWORD, int);
tptr r_lua_topointer = (tptr)(unprotect(ASLR(0x11ce100)));

using toboolean = bool(__cdecl*)(DWORD rL, bool idx);
toboolean r_lua_toboolean = (toboolean)(unprotect(ASLR(0x11CDE80)));

using rcall = void(__cdecl*)(DWORD, DWORD, DWORD);
rcall r_lua_call = (rcall)(unprotect(ASLR(0x11cc1f0)));

using rsetmt = void(__cdecl*)(DWORD, int);
rsetmt r_lua_setmetatable = (rsetmt)(unprotect(ASLR(0x11CDAE0)));

using rnewuserdata = void* (__cdecl*)(DWORD, size_t, int);
rnewuserdata r_lua_newuserdata = (rnewuserdata)(unprotect(ASLR(0x11CCB60)));

using insertt = void(__cdecl*)(DWORD, int);
insertt r_lua_insert = (insertt)(unprotect(ASLR(0x11CC850)));

using RFindFirstChildOfClass = DWORD(__thiscall*)(DWORD, DWORD, const std::string&);
RFindFirstChildOfClass FindFirstChildOfClass_Adr = (RFindFirstChildOfClass)(unprotect(ASLR(0x60AB70)));


void r_lua_pushcclosure(DWORD rL, int Func, int upvals)
{
	DWORD Func2 = Add(Func);
	r_luax_pushcclosure(rL, Func2, NULL, upvals, NULL);
}

void r_lua_setreadonly(DWORD State, int Index, bool Boolean) {
	*(BYTE*)(r_lua_topointer(State, Index) + 8) = Boolean;
}


void r_lua_pushcfunction(DWORD rL, int Func) {
	r_lua_pushcclosure(rL, Func, 0);
}

int r_lua_gettop(int rL)
{
	return *(DWORD*)(rL + 12) - *(DWORD*)(rL + 16) >> 4;
}


void r_luaL_register(DWORD rL, const char* Name, int fn(DWORD)) {
	r_lua_pushcfunction(RL, (DWORD)fn);
	r_lua_setglobal(RL, Name);
}

using getDataModel_t = void(__thiscall*)(DWORD, DWORD);
using getDataModel_tt = DWORD(__cdecl*)();

uintptr_t ReturnDataModel()
{
	DWORD DMPad[8];
	DWORD GetDataModelAdr = ASLR(0xe3c3b0);
	DWORD SomeRandomAdrIdkTheNameOf = ASLR(0xe3c260);
	reinterpret_cast<getDataModel_t>(GetDataModelAdr)(reinterpret_cast<getDataModel_tt>(SomeRandomAdrIdkTheNameOf)(), (DWORD)DMPad);
	return DMPad[0] + 0xC;
}

DWORD ReturnState(DWORD v2) {
	return *(DWORD*)(v2 + 0xA4) - (DWORD)(v2 + 0xA4);
}

const char* r_lua_getclass(int self)
{
	return (const char*)(*(int(**)(void))(*(int*)self + 16))();
}

int FindChildOfInstance(int Instance, const char* Name)
{
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);

	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		if (memcmp(r_lua_getclass(*(int*)i), Name, strlen(Name)) == 0)
		{
			return *(int*)i;
		}
	}
}

DWORD FindChildOfInstance1(DWORD Instance, const char* Child) {
	static DWORD SC;
	FindFirstChildOfClass_Adr(Instance, (DWORD)&SC, Child);
	return SC;
}

int rluaL_newmetatable(int ls, const char* name)
{
	r_lua_getfield(ls, LUA_REGISTRYINDEX, name);  /* get registry.name */
	if (r_lua_type(ls, -1) != RLUA_TNIL)  /* name already in use? */
		return 0;  /* leave previous value on top, but return 0 */
	r_lua_pop(ls, 1);
	r_lua_createtable(ls, 0, 0);  /* create metatable */
	r_lua_pushvalue(ls, -1);
	r_lua_setfield(ls, LUA_REGISTRYINDEX, name);  /* registry.name = metatable */
	return 1;
}

auto addrError = unprotect(ASLR(0x11ce970));
#define ccError __cdecl


auto addrNewUserData = unprotect(ASLR(0x11CCB60));
#define ccNewUserData __cdecl

void* rlua_newuserdata(int ls, unsigned int sz)
{
	return ((void* (ccNewUserData*)(int, unsigned int))(addrNewUserData))(ls, sz);
}


int rluaL_error(int ls, const char* reason)
{
	return ((int(ccError*)(int, const char*))(addrError))(ls, reason);
}

int rluaL_getmetatable(int ls, const char* tname)
{
	return r_lua_getfield(ls, LUA_REGISTRYINDEX, tname);
}

void rluaL_setmetatable(int ls, const char* tname)
{
	rluaL_getmetatable(ls, tname);
	r_lua_setmetatable(ls, -2);
}

auto addrCheckType = unprotect(ASLR(0x11ce910));
#define ccCheckType __cdecl


auto addrToLString = unprotect(ASLR(0x11CDF50));
#define ccToLString __stdcall   

const char* rlua_tolstring(int ls, int idx, int len)
{
	return ((const char* (ccToLString*)(int, int, int))(addrToLString))(ls, idx, len);
}

const char* rlua_tostring(int ls, int idx)
{
	return rlua_tolstring(ls, idx, 0);
}

uintptr_t  RLuaNilObject;

#define luaO_nilobject		(&luaO_nilobject_)
typedef TValue* StkId;  /* index to stack elements */

static TValue* index2adr(lua_State* L, int idx) {
	if (idx > 0) {
		TValue* o = L->base + (idx - 1);
		api_check(L, idx <= L->ci->top - L->base);
		if (o >= L->top) return cast(TValue*, luaO_nilobject);
		else return o;
	}
	else if (idx > LUA_REGISTRYINDEX) {
		api_check(L, idx != 0 && -idx <= L->top - L->base);
		return L->top + idx;
	}
	else switch (idx) {  /* pseudo-indices */
	case LUA_REGISTRYINDEX: return registry(L);
	case LUA_ENVIRONINDEX: {
		Closure* func = curr_func(L);
		sethvalue(L, &L->env, func->c.env);
		return &L->env;
	}
	case LUA_GLOBALSINDEX: return gt(L);
	default: {
		Closure* func = curr_func(L);
		idx = LUA_GLOBALSINDEX - idx;
		return (idx <= func->c.nupvalues)
			? &func->c.upvalue[idx - 1]
			: cast(TValue*, luaO_nilobject);
	}
	}
}

const char* GetStr(TString* o) { return (const char*)((uintptr_t)(o)+0x18); };
#define r_setnvalue(obj, x) { TValue *i_o = (obj); i_o->value.n = (x); i_o->tt = RLUA_TNUMBER; }

uintptr_t DXorKey;

double XorDouble(double num)
{
	uint64_t U64_Xor = *reinterpret_cast<uint64_t*>(&num) ^ *reinterpret_cast<uint64_t*>(DXorKey);
	return *reinterpret_cast<double*>(&U64_Xor);
}

const TValue* VToNumber(const TValue* obj, TValue* n)
{
	lua_Number num;
	if (ttype(obj) == RLUA_TNUMBER) return obj;
	if (ttype(obj) == RLUA_TSTRING && luaO_str2d(GetStr(rawtsvalue(obj)), &num)) {
		r_setnvalue(n, XorDouble(num));
		return n;
	}
	else
		return NULL;
}

bool IsNumber(int idx)
{
	TValue n;
	const TValue* o = index2adr(L, idx);
	return ttype(o) == LUA_TNUMBER || (((o) = VToNumber(o, &n)) != NULL);
}

DWORD instance_ptr;

#define SynException(x) (x)
#define ERR_INVALID_INST		1

#define DereferenceSmartPointerInstance(Ptr) (*(DWORD*) (Ptr))
#define BoundsCheckInstance() if (!instance_ptr) throw SynException(ERR_INVALID_INST)


std::string GetInstanceClassName(DWORD Inst)
{
	if (Inst == 0)
	{
		BoundsCheckInstance();
#ifdef _DEBUG
		return std::string(*(const char**)(*(DWORD*)(instance_ptr + 0xC) + 4));
#else
		return **(std::string**)(*(DWORD*)(instance_ptr + 0xC) + 4);
#endif
	}
#ifdef _DEBUG
	return std::string(*(const char**)(*(DWORD*)(Inst + 0xC) + 4));
#else
	return **(std::string**)(*(DWORD*)(Inst + 0xC) + 4);
#endif
}
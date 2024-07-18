#pragma once
#include <cstdint>
#include <queue> 
#include "RetCheck.h"
#include "addys.h"

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

using namespace Adresses;

std::queue<std::string> ScriptQueue;

DWORD EmptySpace1 = Identity1; //Identity1
DWORD EmptySpace2 = Identity1; //Identity2

DWORD Add(DWORD func);
typedef int (*lRua_CFunction) (DWORD rL);
#define ASLR(x)(x - 0x400000 + (DWORD)GetModuleHandleA(0))

#define r_lua_pop(rL,n)		r_lua_settop(rL, -(n)-1)
#define r_lua_getglobal(rL,s)	r_lua_getfield(rL, LUA_GLOBALSINDEX, (s))
#define r_lua_setglobal(rL,s)	r_lua_setfield(rL, LUA_GLOBALSINDEX, (s))

#define RLUA_TNIL LUA_TNIL
#define RLUA_TLIGHTUSERDATA LUA_TLIGHTUSERDATA
#define RLUA_TNUMBER LUA_TNUMBER
#define RLUA_TBOOLEAN LUA_TBOOLEAN
#define RLUA_TSTRING LUA_TSTRING
#define RLUA_TTHREAD LUA_TTHREAD
#define RLUA_TFUNCTION LUA_TFUNCTION
#define RLUA_TTABLE LUA_TTABLE
#define RLUA_TUSERDATA LUA_TUSERDATA
#define R_LUA_TPROTO LUA_TPROTO
#define R_LUA_TUPVALUE LUA_TUPVALUE

DWORD RL;
lua_State* L;
std::string LBI;

using deser = int(luau_deserialize_CCV*)(DWORD v1, const char* v2, const char* v3, size_t v4, int v5);
deser r_lua_deserialize = (deser)unprotect(ASLR(luau_deserialize_addr));//updated

using s_t = void(spawn_CCV*)(DWORD v1);
s_t r_lua_spawn = (s_t)unprotect(ASLR(spawn_addr));//updated

using rgetfield = int(lua_getfield_CCV*)(DWORD rL, int idx, const char* k);
rgetfield r_lua_getfield = (rgetfield)(unprotect(ASLR(lua_getfield_addr)));

using sf_t = void(lua_setfield_CCV*)(DWORD v1, int v2, const char* v3);
sf_t r_lua_setfield = (sf_t)unprotect(ASLR(lua_setfield_addr));//updated

using ps_t = void(lua_pushlstring_CCV*)(DWORD v1, const char* v2, int v3);
ps_t r_lua_pushlstring = (ps_t)unprotect(ASLR(lua_pushlstring_addr));//updated 

using pushvalue = void(lua_pushvalue_CCV*)(DWORD rL, DWORD idx);
pushvalue r_lua_pushvalue = (pushvalue)(unprotect(ASLR(lua_pushvalue_addr)));

using rpushcclosure = void(lua_pushcclosure_CCV*)(DWORD rL, int fn, int non, int a1, int xd);
rpushcclosure r_luax_pushcclosure = (rpushcclosure)(unprotect(ASLR(lua_pushcclosure_addr)));

using rnewthread = DWORD(lua_newthread_CCV*)(DWORD);
rnewthread r_lua_newthread = (rnewthread)(unprotect(ASLR(lua_newthread_addr)));

using rsettop = void(lua_settop_CCV*)(DWORD rL, int idx);
rsettop r_lua_settop = (rsettop)(unprotect(ASLR(lua_settop_addr)));

using rLua_pcall = int(lua_pcall_CCV*)(DWORD lst, int nargs, int nresults, int errfunc);
rLua_pcall r_lua_pcall = (rLua_pcall)(unprotect(ASLR(lua_pcall_addr)));

using rtolstring = char* (lua_tolstring_CCV*)(DWORD rL, int idx, size_t* size);
rtolstring r_lua_tolstring = (rtolstring)(unprotect(ASLR(lua_tolstring_addr)));

using rtonumber = double(lua_tonumber_CCV*)(DWORD, int, int);
rtonumber r_lua_tonumber = (rtonumber)(ASLR(lua_tonumber_addr));

using rtype = DWORD(lua_type_CCV*)(DWORD, int);
rtype r_lua_type = (rtype)(unprotect(ASLR(lua_type_addr)));

using rpushstring = void(lua_pushstring_CCV*)(DWORD rL, const char*);
rpushstring r_lua_pushstring = (rpushstring)(unprotect(ASLR(lua_pushstring_addr)));

using pushnumber = double(lua_pushnumber_CCV*)(DWORD rL, double idx);
pushnumber r_lua_pushnumber = (pushnumber)(unprotect(ASLR(lua_pushnumber_addr)));

using rgetmetatable = void* (lua_getmetatable_CCV*)(DWORD rL, int idx);
rgetmetatable r_lua_getmetatable = (rgetmetatable)(unprotect(ASLR(lua_getmetatable_addr)));

using rboolean = int(lua_pushboolean_CCV*)(unsigned int, int);
rboolean r_lua_pushboolean = (rboolean)(unprotect(ASLR(lua_pushboolean_addr)));

using rxmove = void(lua_xmove_CCV*)(DWORD, DWORD, DWORD);
rxmove r_lua_xmove = (rxmove)(unprotect(ASLR(lua_xmove_addr)));

using checktype = void(lual_checktype_CCV*)(int, int, int);
checktype rluaL_checktype = (checktype)(unprotect(ASLR(lual_checktype_addr)));

using checknumber = lua_Number(lual_checknumber_CCV*)(int, int);
checknumber rluaL_checknumber = (checknumber)(unprotect(ASLR(lual_checknumber_addr)));

using checklstring = const char* (lual_checklstring_CCV*)(int, int, unsigned int*);
checklstring rluaL_checklstring = (checklstring)(unprotect(ASLR(lual_checklstring_addr)));



using rtouserdata = void* (lua_touserdata_CCV*)(DWORD, int);
rtouserdata r_lua_touserdata = (rtouserdata)(unprotect(ASLR(lua_touserdata_addr)));

using rsettable = void* (lua_settable_CCV*)(DWORD rL, int);
rsettable r_lua_settable = (rsettable)(unprotect(ASLR(lua_settable_addr)));

using rcreatetable = void(lua_createtable_CCV*)(DWORD rL, int num, int fix);
rcreatetable r_lua_createtable = (rcreatetable)(unprotect(ASLR(lua_createtable_addr)));

using rcfunc = bool(lua_iscfunction_CCV*)(DWORD, DWORD);
rcfunc r_lua_iscfunction = (rcfunc)(unprotect(ASLR(lua_iscfunction_addr)));

using tptr = std::uintptr_t(lua_topointer_CCV*)(DWORD, int);
tptr r_lua_topointer = (tptr)(unprotect(ASLR(lua_topointer_addr)));

using toboolean = bool(lua_toboolean_CCV*)(DWORD rL, bool idx);
toboolean r_lua_toboolean = (toboolean)(unprotect(ASLR(lua_toboolean_addr)));

using rcall = void(lua_call_CCV*)(DWORD, DWORD, DWORD);
rcall r_lua_call = (rcall)(unprotect(ASLR(lua_call_addr)));

using rsetmt = void(lua_setmetatable_CCV*)(DWORD, int);
rsetmt r_lua_setmetatable = (rsetmt)(unprotect(ASLR(lua_setmetatable_addr)));

using rnewuserdata = void* (lua_newuserdata_CCV*)(DWORD, size_t, int);
rnewuserdata r_lua_newuserdata = (rnewuserdata)(unprotect(ASLR(lua_newuserdata_addr)));

using insertt = void(lua_insert_CCV*)(DWORD, int);
insertt r_lua_insert = (insertt)(unprotect(ASLR(lua_insert_addr)));

using RFindFirstChildOfClass = DWORD(findfirstchildofclass_CCV*)(DWORD, DWORD, const std::string&);
RFindFirstChildOfClass FindFirstChildOfClass_Adr = (RFindFirstChildOfClass)(unprotect(ASLR(findfirstchildofclass_addr)));


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
	return *(DWORD*)(rL + ls_top) - *(DWORD*)(rL + ls_base) >> 4;
}


void r_luaL_register(DWORD rL, const char* Name, int fn) {
	r_lua_pushcfunction(RL, (DWORD)fn);
	r_lua_setglobal(RL, Name);
}


using r_fireclickdetector_typedef = int(__thiscall*)(std::uintptr_t a1, float a2, int a3);
r_fireclickdetector_typedef r_fireclickdetector = (r_fireclickdetector_typedef)(ASLR(Adresses::fireclickdetector_addr));

using r_lua_remove_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
r_lua_remove_typedef r_lua_remove = (r_lua_remove_typedef)unprotect(ASLR(Adresses::lua_remove_addr));

using getDataModel_t = void(__thiscall*)(DWORD, DWORD);
using getDataModel_tt = DWORD(__cdecl*)();

using r_lual_error_typedef = int(__cdecl*)(int a1, const char* a2, ...);
r_lual_error_typedef r_luaL_error = (r_lual_error_typedef)(ASLR(Adresses::lual_error_addr));


using r_index2adr_typedef = TValue*(__stdcall*)(DWORD rL, int idx);
r_index2adr_typedef r_index2adr = (r_index2adr_typedef)(ASLR(Adresses::index2adr_addr));

uintptr_t ReturnDataModel()
{
	DWORD DMPad[8];
	DWORD GetDataModelAdr = ASLR(getdatamodel_addr);//getdatamodel
	DWORD SomeRandomAdrIdkTheNameOf = ASLR(getdatamodel2_addr);//getdatamodel2
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

auto addrError = unprotect(ASLR(lual_error_addr));
#define ccError lual_error_CCV


auto addrNewUserData = unprotect(ASLR(lua_newuserdata_addr));
#define ccNewUserData lua_newuserdata_CCV

void* rlua_newuserdata(int ls, unsigned int sz)
{
	return ((void* (ccNewUserData*)(int, unsigned int))(addrNewUserData))(ls, sz);
}


int rluaL_error(int ls, const char* reason)
{
	return ((int(ccError*)(int, const char*))(lua_error_addr))(ls, reason);
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

auto addrCheckType = unprotect(ASLR(lual_checktype_addr));
#define ccCheckType lual_checktype_CCV


auto addrToLString = unprotect(ASLR(lua_tolstring_addr));
#define ccToLString lua_tolstring_CCV   

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


#define r_iscfunction(o)	(ttype(o) == R_LUA_TFUNCTION && clvalue(o)->c.isC)

LUA_API lua_CFunction r_lua_tocfunction(DWORD rL, int idx) {
	StkId o = r_index2adr(rL, idx);
	return (!r_iscfunction(o)) ? NULL : clvalue(o)->c.f;
}

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

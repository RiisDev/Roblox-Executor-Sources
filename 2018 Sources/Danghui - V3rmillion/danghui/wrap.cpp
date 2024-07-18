
#include <Windows.h>
#include <type_traits>
#include "types.hpp"
#include "convert.hpp"
#include "globals.hpp"
#include "wrap.hpp"
#include "Lua/lua.hpp"
#include "Lua/lstate.h"
#include "Lua/lobject.h"
#include "Utilities/Retcheck.h"
#include "Libraries/VMP/VMProtectSDK.h"

//#define danghui_debug

#ifdef danghui_debug
#define danghui_diag printf
#else
#define danghui_diag __noop
#endif
#define danghui_top(rL) *(DWORD*)(rL + 24)
#define danghui_getstr(ts) ((const char*)(((DWORD)ts) + sizeof(TString)))
#define danghui_pushudmtmethod(L, n) { lua_pushcfunction(L, danghui_ud_##n); lua_setfield(L, -2, "__"#n); }
#define danghui_pushhmtmethod(L, n) { lua_pushcfunction(L, danghui_h_##n); lua_setfield(L, -2, "__"#n); }
#define danghui_isyield(rL) (*(BYTE*)(rL + 6) == LUA_YIELD)
#define r_protect(x) try { x; } catch (const std::exception& e) { luaL_error(L, e.what()); }
#define r_lua_pop(rL,n) r_lua_settop(rL, -(n)-1)

DWORD danghui_unprotectednewthread = 0;
DWORD danghui_unprotectedxcall = 0;

static const char danghui_threadaccesskey = 'k';
int danghui_yieldhandler(lua_State* L, unsigned int rL);
int danghui_pushtorobloxstate(lua_State* L, unsigned int rL, int idx);
int danghui_pushtouserstate(lua_State* L, unsigned rL, r_TValue* val);
int danghui_indexud(lua_State* L, unsigned int rL, void* ud, TValue* key);
int danghui_newindexud(lua_State* L, unsigned int rL, void* ud, TValue* key, TValue* val);
int danghui_indext(lua_State* L, unsigned int, void* h, TValue* key);
int danghui_newindext(lua_State* L, unsigned int rL, void* h, TValue* key, TValue* val);
int danghui_ud_index(lua_State* L);
int danghui_ud_newindex(lua_State* L);
int danghui_ud_add(lua_State* L);
int danghui_ud_sub(lua_State* L);
int danghui_ud_mul(lua_State* L);
int danghui_ud_div(lua_State* L);
int danghui_ud_mod(lua_State* L);
int danghui_ud_pow(lua_State* L);
int danghui_ud_unm(lua_State* L);
int danghui_h_index(lua_State* L);
int danghui_h_newindex(lua_State* L);
int danghui_onrobloxstateresume(unsigned int rL);
int danghui_pushcclosure(unsigned int rL, void* fn, void* udupval);

struct r_TString {
	int* next;					// +00  00
	byte marked;				// +04  04
	byte tt;					// +05  05
	byte reserved_0;			// +06  06
	byte reserved_1;			// +07	07
	unsigned int len;			// +08	08
	unsigned int hash;			// +12  0C
};

struct danghui_crossdata
{
	lua_State* L;
	LClosure* Fn;
};

/* API FUNCTION CALLS  */

static TValue *index2adr(lua_State *L, int idx) {
	if (idx > 0) {
		TValue *o = L->base + (idx - 1);
		return o;
	}
	else if (idx > LUA_REGISTRYINDEX) {
		return L->top + idx;
	}
	else switch (idx) {  /* pseudo-indices */
	case LUA_REGISTRYINDEX: return registry(L);
	case LUA_ENVIRONINDEX: {
		Closure *func = curr_func(L);
		sethvalue(L, &L->env, func->c.env);
		return &L->env;
	}
	case LUA_GLOBALSINDEX: return gt(L);
	default: {
		Closure *func = curr_func(L);
		idx = LUA_GLOBALSINDEX - idx;
		return (idx <= func->c.nupvalues)
			? &func->c.upvalue[idx - 1]
			: NULL;
	}
	}
}

int r_lua_gettop(unsigned int rL)
{
	return (*(DWORD*)(rL + 24) - *(DWORD*)(rL + 12)) >> 4;
}

void r_lua_settop(DWORD rL, signed int idx)
{
	signed int a2 = idx;
	DWORD a1 = rL;
	if (a2 < 0)
	{
		*(DWORD *)(a1 + 24) += 16 * a2 + 16;
	}
	else
	{
		int i;
		for (i = 16 * a2; *(DWORD *)(a1 + 24) < (unsigned int)(i + *(DWORD *)(a1 + 12)); *(DWORD *)(a1 + 24) += 16)
			*(DWORD *)(*(DWORD *)(a1 + 24) + 8) = 0;
		*(DWORD *)(a1 + 24) = i + *(DWORD *)(a1 + 12);
	}
}

unsigned int r_lua_newthread(unsigned int rL)
{
	typedef unsigned int(__cdecl* _newthread)(unsigned int);
	if (!danghui_unprotectednewthread)
		danghui_unprotectednewthread = Retcheck::unprotect<DWORD>((BYTE*)(DH_NEWTHRE));
	_newthread newthread = (_newthread)(danghui_unprotectednewthread);
	return newthread(rL);
}


void r_luaV_gettable(unsigned int rL, r_TValue* t, r_TValue* key, r_TValue* val)
{
	typedef void(__cdecl* _gettable)(unsigned int, r_TValue*, r_TValue*, r_TValue*);
	_gettable gettable = (_gettable)(DH_VGETTAB);
	gettable(rL, t, key, val);
}

void r_luaV_settable(unsigned int rL, r_TValue* t, r_TValue* key, r_TValue* val)
{
	typedef void(__cdecl* _settable)(unsigned int, r_TValue*, r_TValue*, r_TValue*);
	_settable settable = (_settable)(DH_VSETTAB);
	settable(rL, t, key, val);
}

void* r_luaF_newCclosure(unsigned int rL, int nelems, void* e)
{
	typedef void*(__cdecl* _newCclosure)(unsigned int, int, void*);
	_newCclosure newCclosure = (_newCclosure)(DH_NEWCCLO);
	return newCclosure(rL, nelems, e);
}

static DWORD miaujz = aslr(0x72d3f7);

int r_lua_xcall(unsigned int rL, int nargs, int nresults, int* noresults)
{
	int precallelems = r_lua_gettop(rL);
	typedef void(__cdecl* _call)(unsigned int, int, int, int); //lua_pcall
															   //typedef int(__cdecl* _call)(unsigned int, int); // lua_resume
	if (!danghui_unprotectedxcall)
		danghui_unprotectedxcall = Retcheck::unprotect<DWORD>((BYTE*)(aslr(0x735850)));
	_call call = (_call)(danghui_unprotectedxcall);
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(miaujz), "\x75", 1, 0);
	call(rL, nargs, LUA_MULTRET, NULL);
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(miaujz), "\x74", 1, 0);
	int xc = 0;
	if (noresults)
		*noresults = (r_lua_gettop(rL));
	return xc;
}


void r_Arith(unsigned int rL, r_TValue* ra, r_TValue* rb, r_TValue* rc, r_TMS op)
{
	typedef void(__cdecl* _arith)(unsigned int, r_TValue*, r_TValue*, r_TValue*, r_TMS);
	_arith arith = (_arith)(DH_LUARITH);
	arith(rL, ra, rb, rc, op);
}



/* DANGHUI FUNCTIONS */

r_TValue* danghui_index2adr(unsigned int rL, int idx)
{
	return ((r_TValue*(__cdecl*)(unsigned int, int))(aslr(0x72BC30)))(rL, idx);
}

unsigned int danghui_getrLfromuserstate(lua_State* L)
{
	lua_pushlightuserdata(L, (void*)&danghui_threadaccesskey);
	lua_gettable(L, LUA_REGISTRYINDEX);
	unsigned int rL = lua_tointeger(L, -1);
	lua_pop(L, 1);
	return rL;
}

void danghui_assignrL(lua_State* L, unsigned int rL)
{
	lua_pushlightuserdata(L, (void*)&danghui_threadaccesskey);
	lua_pushinteger(L, rL);
	lua_settable(L, LUA_REGISTRYINDEX);
	return;
}

int danghui_push(unsigned int rL, int tt, void* value)
{
	r_TValue* vl = cast(r_TValue*, danghui_top(rL));
	vl->tt = tt;
	switch (tt)
	{
	case R_LUA_TNIL:
		break;
	case R_LUA_TBOOLEAN:
		vl->value.b = *(int*)value;
		break;
	case R_LUA_TNUMBER:
		vl->value.n = r_luaA_signnumber(rL, *(lua_Number*)value);
		break;
	case R_LUA_TSTRING:
		vl->value.p = r_luaS_newlstr(rL, *(const char**)value, strlen(*(const char**)value));
		break;
	case R_LUA_TUSERDATA:		// assume its already a pointer to the userdata struct
	case R_LUA_TLIGHTUSERDATA:
	case R_LUA_TTABLE:
	case R_LUA_TTHREAD:
		vl->value.p = (void*)(value);
		break;
	default: return FALSE;
	}
	return (danghui_top(rL) += 16);
}

inline void danghui_tvpush(unsigned int rL, r_TValue* val)
{
	r_TValue* top = cast(r_TValue*, danghui_top(rL));
	top->tt = val->tt;
	top->value = val->value;
	danghui_top(rL) += 16;
}

inline void lua_pushfclosure(lua_State* L, LClosure* fc)
{
	TValue* top = L->top;
	top->tt = LUA_TFUNCTION;
	top->value.gc = cast(GCObject*, fc);
	L->top++;
}


int danghui_userstatefnhandler(lua_State* L)
{
	int elems = 0;
	int gotvalues = 0;
	r_TValue* fn = cast(r_TValue*, lua_touserdata(L, lua_upvalueindex(1)));
	DWORD rL = danghui_getrLfromuserstate(L);
	if (!fn || !rL)
		return FALSE;
	danghui_tvpush(rL, fn);
	elems = lua_gettop(L);
	for (int elemiq = 1; elemiq < elems + 1; elemiq++)
		danghui_pushtorobloxstate(L, rL, elemiq);
	r_protect({
		int xc = r_lua_xcall(rL, elems, LUA_MULTRET, &gotvalues);
	if (xc == LUA_YIELD)
		return danghui_yieldhandler(L, rL);
		});
	for (int elemiq = 1; elemiq < gotvalues + 1; elemiq++)
		danghui_pushtouserstate(L, rL, danghui_index2adr(rL, elemiq));
	r_lua_settop(rL, 0);
	return gotvalues;
}

int danghui_robloxstatefnhandler(DWORD rL)
{
	int elems = 0;
	int gotvalues = 0;
	danghui_crossdata* cd = cast(danghui_crossdata*, danghui_index2adr(rL, lua_upvalueindex(2))->value.p);
	if (!cd || !cd->L || !cd->Fn)
		return FALSE;
	lua_State* L = cd->L;
	LClosure* fn = cd->Fn;
	lua_pushfclosure(L, fn);
	elems = r_lua_gettop(rL);
	for (int elemiq = 1; elemiq < elems + 1; elemiq++)
		danghui_pushtouserstate(L, rL, danghui_index2adr(rL, elemiq));
	int xc = lua_resume(L, elems);
	if (xc == LUA_YIELD)
	{
		danghui_pushcclosure(rL, danghui_onrobloxstateresume, L);
		return -1;
	}
	return gotvalues;
}

int danghui_int3fnhandler(DWORD rL)
{
	typedef int(*_handle)(DWORD);
	_handle handle = (_handle)(danghui_index2adr(rL, lua_upvalueindex(1))->value.p);
	return handle(rL);
}

int danghui_wraplfn(lua_State* L, unsigned int rL, LClosure* fn)
{
	r_TValue* nfn = new r_TValue;
	danghui_crossdata* cd = new danghui_crossdata;
	cd->L = L;
	cd->Fn = fn;

	/* create int3fnhandler */
	void* cc = r_luaF_newCclosure(rL, 2, (void*)*(DWORD*)(rL + 64));
	*((DWORD *)cc + 4) = int3breakpoint - ((DWORD)cc + 16);
	r_TValue* upval = cast(r_TValue*, (DWORD)cc + 24);
	upval->tt = R_LUA_TLIGHTUSERDATA;
	upval->value.p = (void*)(danghui_robloxstatefnhandler);
	r_TValue* cdt = cast(r_TValue*, (DWORD)cc + 40);
	cdt->tt = R_LUA_TLIGHTUSERDATA;
	cdt->value.p = (void*)(cd);
	nfn->tt = R_LUA_TFUNCTION;
	nfn->value.p = (void*)(cc);
	danghui_tvpush(rL, nfn);
	return TRUE;
}

int danghui_pushcclosure(unsigned int rL, void* fn, void* udupval)
{
	r_TValue* nfn = new r_TValue;
	void* cc = r_luaF_newCclosure(rL, 2, (void*)*(DWORD*)(rL + 64));
	*((DWORD *)cc + 4) = int3breakpoint - ((DWORD)cc + 16);
	r_TValue* upval = cast(r_TValue*, (DWORD)cc + 24);
	upval->tt = R_LUA_TLIGHTUSERDATA;
	upval->value.p = (void*)(fn);
	r_TValue* cdt = cast(r_TValue*, (DWORD)cc + 40);
	cdt->tt = R_LUA_TLIGHTUSERDATA;
	cdt->value.p = (void*)(udupval);
	nfn->tt = R_LUA_TFUNCTION;
	nfn->value.p = (void*)(cc);
	danghui_tvpush(rL, nfn);
	return TRUE;
}

int danghui_onrobloxstateresume(unsigned int rL)
{
	lua_State* L = (lua_State*)(danghui_index2adr(rL, lua_upvalueindex(2))->value.p);
	int args = r_lua_gettop(rL);
	for (int elemiq = 1; elemiq < args + 1; elemiq++)
		danghui_pushtouserstate(L, rL, danghui_index2adr(rL, elemiq));
	return lua_resume(L, args);
}

int danghui_yieldhandler(lua_State* L, unsigned int rL)
{
	/* called after xcall noticed the method yielded */
	danghui_pushcclosure(rL, danghui_onrobloxstateresume, L);
	//*(BYTE*)(rL + 6) = 0;
	return lua_yield(L, 0);
}

int danghui_pushtorobloxstate(lua_State* L, unsigned int rL, int idx)
{
	switch (lua_type(L, idx))
	{
	case LUA_TNIL:
		danghui_push(rL, R_LUA_TNIL, nullptr);
		return TRUE;
	case LUA_TBOOLEAN:
	{
		int boolv = lua_toboolean(L, idx);
		danghui_push(rL, R_LUA_TBOOLEAN, (void*)&boolv);
		return TRUE;
	}
	case LUA_TNUMBER:
	{
		lua_Number nov = lua_tonumber(L, idx);
		danghui_push(rL, R_LUA_TNUMBER, &nov);
		return TRUE;
	}
	case LUA_TSTRING:
	{
		const char* strv = lua_tostring(L, idx);
		danghui_push(rL, R_LUA_TSTRING, &strv);
		return TRUE;
	}
	case LUA_TUSERDATA:
		danghui_tvpush(rL, (r_TValue*)(lua_touserdata(L, idx)));
		return TRUE;
	case LUA_TFUNCTION:
	{
		CClosure* cc = cast(CClosure*, lua_topointer(L, idx));
		if (cc->isC)
		{
			if (cc->upvalue->tt == LUA_TLIGHTUSERDATA)
			{
				TValue* ud = cc->upvalue;
				r_TValue* tfn = cast(r_TValue*, ud->value.p);
				danghui_tvpush(rL, tfn);
				return TRUE;
			}
		}
		else
		{
			LClosure* fn = cast(LClosure*, lua_topointer(L, idx));
			return danghui_wraplfn(L, rL, fn);
		}
		return FALSE;
	}
	case LUA_TTABLE:
		if (luaL_getmetafield(L, idx, "__value"))
		{
			danghui_tvpush(rL, (r_TValue*)(lua_touserdata(L, -1)));
			lua_pop(L, 1);
			return TRUE;
		}
		return FALSE;
	default: return FALSE;
	}
}

int danghui_wrapfn(lua_State* L, unsigned int rL, r_TValue* fn)
{
	r_TValue* cval = new r_TValue;
	cval->tt = fn->tt;
	cval->value = fn->value;
	lua_pushlightuserdata(L, cval);	// roblox c closure tvalue
	lua_pushcclosure(L, danghui_userstatefnhandler, 1);
	return TRUE;
}

int danghui_wrapud(lua_State* L, unsigned int rL, r_TValue* ud)
{
	r_TValue* stack_ud;
	stack_ud = cast(r_TValue*, lua_newuserdata(L, sizeof(r_TValue)));
	*stack_ud = *ud;

	lua_newtable(L);
	danghui_pushudmtmethod(L, add);
	danghui_pushudmtmethod(L, sub);
	danghui_pushudmtmethod(L, mul);
	danghui_pushudmtmethod(L, div);
	danghui_pushudmtmethod(L, mod);
	danghui_pushudmtmethod(L, pow);
	danghui_pushudmtmethod(L, unm);
	danghui_pushudmtmethod(L, index);
	danghui_pushudmtmethod(L, newindex);
	lua_pushliteral(L, "The metatable is locked");
	lua_setfield(L, -2, "__metatable");
	lua_setmetatable(L, -2);
	return TRUE;
}

int danghui_wraptable(lua_State* L, unsigned int rL, r_TValue* tb)
{
	r_TValue* tvb = new r_TValue;
	tvb->tt = tb->tt;
	tvb->value = tb->value;
	lua_newtable(L); // actual table
	lua_newtable(L); // metatable
	lua_pushlightuserdata(L, tvb);
	lua_setfield(L, -2, "__value");
	danghui_pushhmtmethod(L, index);
	danghui_pushhmtmethod(L, newindex);
	lua_pushliteral(L, "The metatable is locked");
	lua_setfield(L, -2, "__metatable");
	lua_setmetatable(L, -2);
	return TRUE;
}

int danghui_pushtouserstate(lua_State* L, unsigned rL, r_TValue* val)
{
	switch (val->tt)
	{
	case R_LUA_TNIL:
		lua_pushnil(L);
		return TRUE;
	case R_LUA_TBOOLEAN:
		lua_pushboolean(L, val->value.b);
		return TRUE;
	case R_LUA_TNUMBER:
		lua_pushnumber(L, r_luaA_signnumber(rL, val->value.n));
		return TRUE;
	case R_LUA_TSTRING:
		lua_pushstring(L, (const char*)((DWORD)(val->value.p) + 24));
		return TRUE;
	case R_LUA_TTABLE:
		return danghui_wraptable(L, rL, val);
	case R_LUA_TLIGHTUSERDATA:
	case R_LUA_TUSERDATA:
		return danghui_wrapud(L, rL, val);
	case R_LUA_TFUNCTION:
		return danghui_wrapfn(L, rL, val);
	default: return FALSE;
	}
}

int danghui_indext(lua_State* L, unsigned int rL, void* h, TValue* key)
{
	r_TValue indexresult;
	r_TValue convertedkey;
	r_TValue* r_h = cast(r_TValue*, h);
	if (!danghui_convert(L, rL, key, &convertedkey))
		return FALSE;
	r_protect(r_luaV_gettable(rL, r_h, &convertedkey, &indexresult));
	return danghui_pushtouserstate(L, rL, &indexresult);
}

int danghui_newindext(lua_State* L, unsigned int rL, void* h, TValue* key, TValue* val)
{
	r_TValue convertedkey;
	r_TValue convertedval;
	r_TValue* r_ud = cast(r_TValue*, h);
	if (!danghui_convert(L, rL, key, &convertedkey))
		return FALSE;
	if (!danghui_convert(L, rL, val, &convertedval))
		return FALSE;
	r_protect(r_luaV_settable(rL, r_ud, &convertedkey, &convertedval));
	return 0;
}

int danghui_indexud(lua_State* L, unsigned int rL, void* ud, TValue* key)
{
	r_TValue* indexresult = new r_TValue; //new r_TValue;
	r_TValue* convertedkey = new r_TValue; // new_rTValue;
	r_TValue* r_ud = cast(r_TValue*, ud);
	if (!danghui_convert(L, rL, key, convertedkey))
		return FALSE;
	r_protect(r_luaV_gettable(rL, r_ud, convertedkey, indexresult));
	delete convertedkey;
	return danghui_pushtouserstate(L, rL, indexresult);
}

int danghui_newindexud(lua_State* L, unsigned int rL, void* ud, TValue* key, TValue* val)
{
	r_TValue* convertedkey = new r_TValue;
	r_TValue* convertedval = new r_TValue;
	r_TValue* r_ud = cast(r_TValue*, ud);
	if (!danghui_convert(L, rL, key, convertedkey))
		return FALSE;
	if (!danghui_convert(L, rL, val, convertedval))
		return FALSE;
	r_protect(r_luaV_settable(rL, r_ud, convertedkey, convertedval));
	delete convertedkey;
	delete convertedval;
	return 0;
}

int danghui_doarith(lua_State* L, unsigned int rL, void* ud, TValue* val, r_TMS op)
{
	r_TValue* r_ud = cast(r_TValue*, ud);
	r_TValue convertedval;
	r_TValue valuereturn;
	if (!danghui_convert(L, rL, val, &convertedval))
		return FALSE;
	r_protect(r_Arith(rL, &valuereturn, r_ud, &convertedval, op));
	return danghui_pushtouserstate(L, rL, &valuereturn);
}

/* DANGHUI USERDATA OPERATORS */

int danghui_ud_index(lua_State* L)
{
	void* ud = lua_touserdata(L, -2);
	TValue* key = index2adr(L, -1);
	return danghui_indexud(L, danghui_getrLfromuserstate(L), ud, key);
}

int danghui_ud_newindex(lua_State* L)
{
	void* ud = lua_touserdata(L, -3);
	TValue* key = index2adr(L, -2);
	TValue* val = index2adr(L, -1);
	return danghui_newindexud(L, danghui_getrLfromuserstate(L), ud, key, val);
}

int danghui_ud_add(lua_State* L)
{
	void* ud = lua_touserdata(L, 1);
	TValue* val = index2adr(L, -1);
	return danghui_doarith(L, danghui_getrLfromuserstate(L), ud, val, R_TM_ADD);
}

int danghui_ud_sub(lua_State* L)
{
	void* ud = lua_touserdata(L, 1);
	TValue* val = index2adr(L, -1);
	return danghui_doarith(L, danghui_getrLfromuserstate(L), ud, val, R_TM_SUB);
}

int danghui_ud_mul(lua_State* L)
{
	void* ud = lua_touserdata(L, 1);
	TValue* val = index2adr(L, -1);
	return danghui_doarith(L, danghui_getrLfromuserstate(L), ud, val, R_TM_MUL);
}

int danghui_ud_div(lua_State* L)
{
	void* ud = lua_touserdata(L, 1);
	TValue* val = index2adr(L, -1);
	return danghui_doarith(L, danghui_getrLfromuserstate(L), ud, val, R_TM_DIV);
}

int danghui_ud_mod(lua_State* L)
{
	void* ud = lua_touserdata(L, 1);
	TValue* val = index2adr(L, -1);
	return danghui_doarith(L, danghui_getrLfromuserstate(L), ud, val, R_TM_MOD);
}

int danghui_ud_pow(lua_State* L)
{
	void* ud = lua_touserdata(L, 1);
	TValue* val = index2adr(L, -1);
	return danghui_doarith(L, danghui_getrLfromuserstate(L), ud, val, R_TM_POW);
}

int danghui_ud_unm(lua_State* L)
{
	void* ud = lua_touserdata(L, 1);
	TValue* val = index2adr(L, -1);
	return danghui_doarith(L, danghui_getrLfromuserstate(L), ud, val, R_TM_UNM);
}

/* DANGHUI TABLE METAMETHODS */

int danghui_h_index(lua_State* L)
{
	r_TValue* h;
	TValue* key = L->base + 1;
	luaL_getmetafield(L, 1, "__value");
	if (lua_type(L, -1) == LUA_TLIGHTUSERDATA)
	{
		h = cast(r_TValue*, lua_touserdata(L, -1));
		lua_pop(L, 1);
		return danghui_indext(L, danghui_getrLfromuserstate(L), h, key);
	}
	return 0;
}

int danghui_h_newindex(lua_State* L)
{
	r_TValue* h;
	TValue* key = L->base + 1;
	TValue* val = L->base + 2;
	luaL_getmetafield(L, 1, "__value");
	if (lua_type(L, -1) == LUA_TLIGHTUSERDATA)
	{
		h = cast(r_TValue*, lua_touserdata(L, -1));
		lua_pop(L, 1);
		return danghui_newindext(L, danghui_getrLfromuserstate(L), h, key, val);
	}
	return 0;
}

/* DANGHUI GLOBAL FUNCTIONS */

int danghuiB_error(lua_State* L)
{
	const char* errmsg = luaL_checkstring(L, 1);
	luaL_error(L, errmsg);
	return 0;
}

/* DANGHUI ENVIRONMENT METAMETHODS */

int danghui_env_globalindex(lua_State *L)
{
	unsigned int rL = danghui_getrLfromuserstate(L);
	r_TValue* env = cast(r_TValue*, (unsigned int*)(rL + 64));
	r_TValue* globalkey = new r_TValue;
	r_TValue* globalvar = new r_TValue;
	TValue* idxkey = index2adr(L, -1);
	if (!danghui_convert(L, rL, idxkey, globalkey))
		return FALSE;
	r_luaV_gettable(rL, env, globalkey, globalvar);
	delete globalkey;
	return danghui_pushtouserstate(L, rL, globalvar);
}

int danghui_env_globalnewindex(lua_State *L)
{
	unsigned int rL = danghui_getrLfromuserstate(L);
	r_TValue* env = cast(r_TValue*, (unsigned int*)(rL + 64));
	r_TValue* globalkey = new r_TValue;
	r_TValue* globalval = new r_TValue;
	TValue* idxkey = index2adr(L, -2);
	TValue* idxval = index2adr(L, -1);
	if (!danghui_convert(L, rL, idxkey, globalkey))
		return FALSE;
	if (!danghui_convert(L, rL, idxval, globalval))
		return FALSE;
	r_luaV_settable(rL, env, globalkey, globalval);
	delete globalkey;
	return 0;
}

DWORD BasicLuaState = 0;

#define dfn(n, fn) { lua_pushcfunction(basestate, fn); lua_setglobal(basestate, n); }
lua_State* danghui_openstate()
{
	lua_State* basestate = luaL_newstate();
	DWORD scriptcontext = danghui_grabscriptcontext();
	DWORD rL = danghui_grabstate(scriptcontext, 2);
	if (!BasicLuaState)
	{
		BasicLuaState = r_lua_newthread(rL);
		r_lua_pop(rL, 1);
	}

	DWORD tL = r_lua_newthread(BasicLuaState);
	danghui_assignrL(basestate, rL);
	r_lua_pop(BasicLuaState, 1);

	dfn("error", danghuiB_error);

	lua_pushvalue(basestate, LUA_GLOBALSINDEX); // the env
	lua_newtable(basestate);					// env's metatable
	lua_pushcfunction(basestate, danghui_env_globalindex);
	lua_setfield(basestate, -2, "__index");
	lua_pushcfunction(basestate, danghui_env_globalnewindex);
	lua_setfield(basestate, -2, "__newindex");
	lua_pushliteral(basestate, "The metatable is locked");
	lua_setfield(basestate, -2, "__metatable");
	lua_setmetatable(basestate, -2);
	return basestate;
}

void danghui_Aexecute(const char* script)
{
	std::string fscript = std::string("spawn(function() " + std::string(script) + " end)");
	lua_State* L = danghui_openstate();
	*(DWORD*)(aslr(0x15BA674)) = 6;
	if (!luaL_loadstring(L, script))//fscript.c_str()))
		lua_pcall(L, 0, 0, 0);
	lua_pop(L, 1);
}
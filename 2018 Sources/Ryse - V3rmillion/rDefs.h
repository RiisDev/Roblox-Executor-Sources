#include <xmmintrin.h>  
#include <sstream>

using namespace std;

int top = 20, base = 8; // int top = 32, base = 8;
// gettop: return (*(_DWORD *)(a1 + 20) - *(_DWORD *)(a1 + 8)) >> 4; ||  return cast_int(L->top - L->base);
struct rlua_State {
	void *spacing_0[2];
	TValue *base;
	void *spacing_1[5];
	TValue *top;
};

int rlua_gettop(int rL)
{
	rlua_State *rState = cast(rlua_State*, rL);
	return (int)(rState->top - rState->base);
	//return (int)(*(TValue**)(rL + top) - *(TValue**)(rL + base));
}

#define rlua_ttype(o) (o->tt)

#define rlua_api_check(l, e) lua_assert(e)

#define rlua_ttistable(o)    (rlua_ttype(o) == RLUA_TTABLE)
#define rlua_ttisfunction(o)    (rlua_ttype(o) == RLUA_TFUNCTION)
#define rlua_ttisthread(o)    (rlua_ttype(o) == RLUA_TTHREAD)
#define rlua_ttisuserdata(o)    (rlua_ttype(o) == RLUA_TUSERDATA)
#define rlua_ttislightuserdata(o)    (rlua_ttype(o) == RLUA_TLIGHTUSERDATA)

#define rlua_hvalue(o) &(o)->value.gc->h
#define rlua_clvalue(o) &(o)->value.gc->cl
#define rlua_thvalue(o) &(o)->value.gc->th

#define rlua_rawuvalue(o)	&(o)->value.gc->u
#define rlua_pvalue(o)	(o)->value.p

#define rlua_api_incr_top(L)   {api_check(L, L->top < L->ci->top); L->top++;}

#define RLPUSH(L, value, identifier) { int topofstack = *(DWORD *)(L + offsetof(rlua_State, top));\
                                    *(DWORD *)topofstack = value;\
                                    *(DWORD *)(topofstack + 8) = identifier;\
                                    *(DWORD *)(L + offsetof(rlua_State, top)) += 16;\
                                     }

inline void rlua_pushuserdata(int lst, void* ud) {
	RLPUSH(lst, *(DWORD*)ud, RLUA_TUSERDATA);
}

int rxor = ScanAddress(0x16EC790); // updated

/*inline void rlua_pushnumber(int lst, double *num) {
std::cout << "got number " << num << std::endl;
PVOID OldNumber = num;
double NewNumber;
__asm {
mov edx, OldNumber
mov eax, rxor
movsd xmm1, qword ptr[edx]
xorpd xmm1, [eax]
movsd NewNumber, xmm1
}
int v3 = *(DWORD *)(rState + offsetof(rlua_State, top));
*(double *)v3 = NewNumber;
*(DWORD *)(v3 + 8) = RLUA_TNUMBER;
*(DWORD *)(rState + offsetof(rlua_State, top)) += 16;
}*/

void rlua_pushnil(DWORD RL) {
	RLPUSH(RL, 0, RLUA_TNIL);
}

void rlua_pushboolean(DWORD RL, BOOL l) {
	RLPUSH(RL, l != 0, RLUA_TBOOLEAN);
}

void rlua_pushlightuserdata(DWORD RL, void *p) {
	RLPUSH(RL, (DWORD)p, RLUA_TLIGHTUSERDATA);
}

void rlua_pushstring(int RL, const char *s) {
	int v3 = *(DWORD *)(rState + offsetof(rlua_State, top));
	*(DWORD *)v3 = rluaS_newlstr(rState, s, strlen(s));
	*(DWORD *)(v3 + 8) = RLUA_TSTRING;
	*(DWORD *)(rState + offsetof(rlua_State, top)) += 16;
}

inline int rlua_type(int rState, int idx) {
	return rlua_index2adr(rState, idx)->tt;
}

inline int rlua_tonumber(int rState, int idx) {
	PVOID OldNumber = &rlua_index2adr(rState, idx)->value.n;
	double NewNumber;
	__asm {
		mov edx, OldNumber
		mov eax, rxor
		movsd xmm1, qword ptr[edx]
		xorpd xmm1, [eax]
		movsd NewNumber, xmm1
	}
	return NewNumber;
}

inline int rlua_toboolean(int rState, int idx) {
	return rlua_index2adr(rState, idx)->value.b;
}

void *rlua_touserdata(int rState, int idx) {
	TValue *o = rlua_index2adr(rState, idx);
	switch (rlua_ttype(o)) {
	case RLUA_TUSERDATA:
		return (rlua_rawuvalue(o) + 1);
		break;
	case RLUA_TLIGHTUSERDATA:
		return rlua_pvalue(o);
		break;
	default: return NULL;
	}
}

inline void *rlua_topointer(int rState, int idx) {
	TValue *o = rlua_index2adr(rState, idx);
	switch (rlua_ttype(o)) {
	case RLUA_TTABLE:
		return rlua_hvalue(o);
		break;
	case RLUA_TFUNCTION:
		return rlua_clvalue(o);
		break;
	case RLUA_TTHREAD:
		return rlua_thvalue(o);
		break;
	case RLUA_TUSERDATA:
	case RLUA_TLIGHTUSERDATA:
		return rlua_touserdata(rState, idx);
	default: return NULL;
	}
}

#define rlua_setnilvalue(obj) ((obj)->tt=RLUA_TNIL)

void rlua_settop(int rlState, int idx) {
	rlua_State *rState = cast(rlua_State*, rlState);
	if (idx >= 0) {
		while (rState->top < rState->base + idx)
			rlua_setnilvalue(rState->top++);
		rState->top = rState->base + idx;
	}
	else {
		rState->top += idx + 1;
	}
}

void rlua_pop(DWORD RL, int n) {
	rlua_settop(RL, -(n)-1);
}

void rlua_pushvalue(int rState, int idx) { // not updated?
	TValue *p = rlua_index2adr(rState, idx);
	int v3 = *(DWORD *)(rState + offsetof(rlua_State, top));
	*(DWORD *)v3 = *(DWORD *)p;
	*(DWORD *)(v3 + 4) = *(DWORD *)(p + 4);
	*(DWORD *)(v3 + 8) = p->tt;
	*(DWORD *)(rState + offsetof(rlua_State, top)) += 16;
}

inline void rlua_getfield(int lst, int idx, const char * k) {
	__try {
		rlua_getfield_real(lst, idx, k); // rlua_getfield_real(lst, idx, k, 1);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		rlua_pushnil(lst);
	}
}

const char *rlua_tostring(int rL, int idx) {
	if (rlua_index2adr(rL, idx)->tt == RLUA_TNUMBER) {
		std::stringstream str;
		str << rlua_tonumber(rL, idx);
		return str.str().c_str();
	}
	else {
		rlua_tolstring(rL, idx, NULL);
	}
}

const char *tablestring(int rState, int &counter, bool &t) {
	if (rlua_index2adr(rState, -1)->tt != RLUA_TNUMBER) {
		return rlua_tostring(rState, -1);
	}
	else {
		counter++;
		t = false;
		return "";
	}
}
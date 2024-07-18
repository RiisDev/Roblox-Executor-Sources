#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include "retcheck.h"

#define _DWORD DWORD
#define _BYTE BYTE
#define _WORD WORD

extern "C" {
	// lua core
#include "Lua\lua.h"
#include "Lua\lauxlib.h"
#include "Lua\lualib.h"
	// lua internals
#include "Lua\lobject.h"
#include "Lua\lstate.h"
#include "Lua\lgc.h"
#include "Lua\lfunc.h"
#include "Lua\lopcodes.h"
#include "Lua\ldo.h"
	// lparser.c modified for roblox's op_closure change
}

#define Format(address) (address - 0x00400000 + reinterpret_cast<uintptr_t>(GetModuleHandle(0)))

#define Unformat(a)(a + 0x400000 - (DWORD)GetModuleHandleA(0))
#define rformat(x)(x - 0x400000 + (DWORD)GetModuleHandle(NULL))

#define Declare(address, returnValue, callingConvention, ...) (returnValue(callingConvention*)(__VA_ARGS__))unprotect(Format(address))
#define RDeclare(address, returnValue, callingConvention, ...) (returnValue(callingConvention*)(__VA_ARGS__))(Format(address))
#define RLUA_TNIL 0
#define RLUA_TLIGHTUSERDATA 1
#define RLUA_TNUMBER 2
#define RLUA_TBOOLEAN 3
#define RLUA_TSTRING 4
#define RLUA_TTHREAD 5
#define RLUA_TFUNCTION 6
#define RLUA_TTABLE 7
#define RLUA_TUSERDATA 8
#define RLUA_TPROTO 9

auto rlua_pushcclosure = Declare(0x853110, TValue*, __fastcall, int, int, int);
auto _rluaV_execute = Declare(0x0084E330, void, __cdecl, int, int);
auto rlua_getfield = Declare(0x851FB0, void, __fastcall, int, int, const char*);
auto rlua_resume = Declare(0x853f60, void, __cdecl, int, int);
auto spawn = RDeclare(0x834180, void, __cdecl, int);
auto rlua_tolstring = Declare(0x854a80, const char*, __stdcall, int, int, size_t);
auto rlua_pcall = Declare(0x852fb0, int, __cdecl, int, int, int, int);
auto rlua_tonumber = RDeclare(0x854bf0, double, __cdecl, int, int, int);
auto rluaD_precall = Declare(0x84A4D0, uintptr_t, __cdecl, uintptr_t, StkId, size_t);
auto rlua_newthread = Declare(0x852c50, int, __cdecl, int);
auto rluaS_newlstr = RDeclare(0x84d750, TString*, __fastcall, int, const char*, size_t);
auto rlua_touserdata = RDeclare(0x854ea0, TValue*, __cdecl, int, int);
auto rluaV_gettable = RDeclare(0x00851240, void, __cdecl, int, TValue*, TValue*, TValue*);
auto rluaV_settable = Declare(0x8514d0, void, __cdecl, int, TValue*, TValue*, TValue*);
auto rlua_index2adr = Declare(0x848e10, TValue*, __cdecl, int, int);
auto rlua_arith = RDeclare(0x84e030, TValue*, __cdecl, int, TValue*, TValue*, TValue*, TMS);
auto rlua_h_new = RDeclare(0x84bb60, int, __cdecl, int, int, int);
auto rlua_pushlightuserdata = Declare(0x853380, void*, __cdecl, int, void*);
auto rluaM_realloc_ = RDeclare(0x0084D0C0, void*, __cdecl, int, int, int, int, int);
auto rluaD_checkstack = RDeclare(0x0084A310, int, __cdecl, int, size_t);
auto rluaH_resizeArray = RDeclare(0x0084BCE0, void*, __cdecl, uintptr_t, Table*, size_t);
auto rluaG_runerr = RDeclare(0x0084B4E0, void, __cdecl, int, const char*, ...);
auto rluaF_newcClosure = RDeclare(0x84b090, int, __cdecl, int, int, int);
auto rluaV_concat = RDeclare(0x00851370, void, _cdecl, int, int, int);
auto callorderTM = RDeclare(0x847160, int, __cdecl, int, const TValue*, const TValue*, TMS);
auto rluaH_getn = RDeclare(0x0084B9B0, int, __cdecl, Table*);
auto rluaV_lessthan = RDeclare(0x00851430, int, __cdecl, int, const TValue*, const TValue*);
auto rluaH_setnum = RDeclare(0x0084BDC0, int, __cdecl, int, Table*, int);
#include <math.h>
#define luai_numadd(a,b)	((a)+(b))
#define luai_numsub(a,b)	((a)-(b))
#define luai_nummul(a,b)	((a)*(b))
#define luai_numdiv(a,b)	((a)/(b))
#define luai_nummod(a,b)	((a) - floor((a)/(b))*(b))
#define luai_numpow(a,b)	(pow(a,b))
#define luai_numunm(a)		(-(a))
#define luai_numeq(a,b)		((a)==(b))
#define luai_numlt(a,b)		((a)<(b))
#define luai_numle(a,b)		((a)<=(b))
#define luai_numisnan(a)	(!luai_numeq((a), (a)))

#define rsetnilvalue(obj) ((obj)->tt=RLUA_TNIL)

#define rsetnvalue(obj,x) \
  { TValue *i_o=(obj); i_o->value.n=(x); i_o->tt=RLUA_TNUMBER; }

#define rsetpvalue(obj,x) \
  { TValue *i_o=(obj); i_o->value.p=(x); i_o->tt=RLUA_TLIGHTUSERDATA; }

#define rsetbvalue(obj,x) \
  { TValue *i_o=(obj); i_o->value.b=(x); i_o->tt=RLUA_TBOOLEAN; }

#define rsetsvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=RLUA_TSTRING; \
    checkliveness(G(L),i_o); }

#define rsetuvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=RLUA_TUSERDATA; \
    checkliveness(G(L),i_o); }

#define rsetthvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=RLUA_TTHREAD; \
    checkliveness(G(L),i_o); }

#define rsetclvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=RLUA_TFUNCTION; \
    }
#define rsethvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=RLUA_TTABLE; \
    checkliveness(G(L),i_o); }

#define rsetptvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=RLUA_TPROTO; \
    checkliveness(G(L),i_o); }


VOID setObject(TValue* constant_1, TValue* constant_2)
{
	constant_1->tt = constant_2->tt;
	constant_1->value = constant_2->value;

}
int decrypt_ptr(int loc) {
	return *(int*)loc - loc;
}

void r_luaC_link(int r_lua_State, int o, byte tt)
{
	printf("luac_link\n");
	int g = decrypt_ptr(r_lua_State + 20);	// global_State *g = G(L)
	printf("g: %x\n", g);
	*(int*)(o) = *(int*)(g + 44);			// o->gch.next = g->rootgc
	*(int*)(g + 44) = o;					// g->rootgc = o
	*(byte*)(o + 4) = *(byte*)(g + 24) & 3;	// o->gch.marked = luaC_white(g)
	*(byte*)(o + 5) = tt;
	printf("g: %x\n", g);
	
	// o->gch.tt = tt;
}
int rbx_pushcclosure(int a1, int a2, int a3)
{
	int v3; // ebx
	int v4; // eax
	int v5; // eax
	int v6; // esi
	int v7; // eax
	int v8; // edx
	int v9; // edx
	int v10; // ecx
	int v11; // eax
	signed int result; // eax
	int v13; // [esp+Ch] [ebp-4h]
	char* retaddr; // [esp+14h] [ebp+4h]
	int v15; // [esp+18h] [ebp+8h]

	v3 = a1;
	v13 = a2;

	v4 = *(_DWORD*)(v3 + 32);
	if (v4 == *(_DWORD*)(v3 + 40))
		v5 = *(_DWORD*)(v3 + 64);
	else
		v5 = *(_DWORD*)(**(_DWORD * *)(v4 + 4) + 16);
	v6 = a3;
	v7 = rluaF_newcClosure(v3, a3, v5);
	v8 = 16 * a3;
	v15 = v7;
	*(_DWORD*)(v7 + 20) = v7 + 20 - v13;
	*(_DWORD*)(v3 + 24) -= 16 * v6;

	if (v6)
	{
		v9 = v15 + v8 + 24;
		do
		{
			v9 -= 16;
			v10 = -24 - v15 + *(_DWORD*)(v3 + 24);
			*(_DWORD*)v9 = *(_DWORD*)(v10 + v9);
			*(_DWORD*)(v9 + 4) = *(_DWORD*)(v10 + v9 + 4);
			*(_DWORD*)(v9 + 8) = *(_DWORD*)(v10 + v9 + 8);
			--v6;
		} while (v6);
	}
	TValue _v11;
	_v11.value.gc = (GCObject*)v15;
	_v11.tt = RLUA_TFUNCTION;
	return reinterpret_cast<int>(&_v11);
}


uint32_t rluaV_execute(uintptr_t rL, lua_State* L, int nextCalls);

bool Occult_luaV_equalVal(DWORD rL, TValue* t1, TValue* t2) {
	if (t1->tt != t2->tt)
		return false;

	switch (t1->tt) {
	case RLUA_TNIL: return true;
	case RLUA_TNUMBER: return t1->value.n == t2->value.n;
	case RLUA_TBOOLEAN: return t1->value.b == t2->value.b;
	case RLUA_TLIGHTUSERDATA: return t1->value.p == t2->value.p;
	case RLUA_TTABLE:
	case RLUA_TUSERDATA: {
		if (t1->value.gc == t2->value.gc) return true;
	
		return false;
	}
	default: return t1->value.gc == t2->value.gc;
	};
}

#define equalobj(L,o1,o2) \
	(ttype(o1) == ttype(o2) && Occult_luaV_equalVal(L, o1, o2))

DWORD XorConstant = rformat(0x1D0B870);

double Occult_luaX_ConvertNumber(double Number) {
	void* oldnumber = &(Number);
	double newnumber;
	__asm
	{
		mov edx, oldnumber
		mov eax, XorConstant
		movsd xmm1, qword ptr[edx]
		xorpd xmm1, [eax]
		movsd newnumber, xmm1
	}
	return newnumber;
}


auto rluaV_tonumber = Declare(0x00851630, const TValue*, __cdecl, const TValue*, const TValue*);

#define tonumber(o,n)	(ttype(o) == RLUA_TNUMBER || \
                         (((o) = rluaV_tonumber(o,n)) != NULL))


typedef unsigned char   uint8;
#define _BYTE  uint8
#define BYTEn(x, n)   (*((_BYTE*)&(x)+n))
#define BYTE1(x)   BYTEn(x,  1)  

#define LUAU_SIZE_A 8
#define LUAU_SIZE_B 16
#define LUAU_SIZE_C 24
#define LUAU_SIZE_Bx 16

#define LUAU_GET_OPCODE(i) FindOpcodeOnLuau(i & 0xFF)
#define LUAU_GET_VM_CASE(i) (unsigned __int8)(OpCodeList[i])

#define LUAU_GETARG_A(i) BYTE1(i)
#define LUAU_GETARG_B(i) ((i >> LUAU_SIZE_B) & 0xFF)
#define LUAU_GETARG_C(i) (i >> LUAU_SIZE_C)

#define LUAU_GETARG_Bx(i) (i >> LUAU_SIZE_Bx)
#define LUAU_GETARG_sBx(i) LUAU_GETARG_Bx(i) // They don't use MAXARG_sBx for theirs, their sBx looks something like this: pc += GETARG_Bx(inst)

enum class UOpCode : uint8_t
{
	NOP = 0x90,
	MUL = 0x5,
	NTEST = 0xE8,
	TFORPREP = 0x17,
	SETGLOBAL = 0x18,
	LEN = 0xC,
	SUB = 0x6,
	TEST = 0x2a,
	SETTABLEK = 0x30,
	GETGLOBAL = 0x35,
	UNM = 0x39,
	ADD = 0x43,
	GETTABLEK = 0x4D,
	MOVE = 0x52,
	NOT = 0x56,
	NLT = 0x60,
	JMP = 0x65,
	SETTABLE = 0x6A,
	LONGJMP = 0x69,
	LOADK = 0x6F,
	CONCAT = 0x73,
	NLE = 0x7D,
	RETURN = 0x82,
	GETTABLE = 0x7,
	FORLOOP = 0xB,
	LOADNUMBER = 0xAC,
	NEQ = 0x8A,
	CALL = 0x8F,
	CLEARSTACK = 0xA3,
	FORPREP = 0xA4,
	LOADBOOL = 0x59,
	POW = 0x72,
	LT = 0x17,
	SELF = 0x4C,
	CLOSE = 0x31,
	SETLIST = 0xC3,
	LOADNIL = 0xC2,
	MOD = 0xC1,
	LE = 0xD4,
	SETUPVAL = 0xDE,
	CLOSURE = 0xAB,
	VARARG = 0xDC,
	DIV = 0xEA,
	EQ = 0xF3,
	TFORLOOP = 0xF8,
	GETUPVAL = 0xFC,
	NEWTABLE = 0xFF
};
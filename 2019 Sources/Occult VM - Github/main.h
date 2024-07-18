
#include "stdafx.h"
#include "OccultVM.h" 
#include "../Utilities/Retcheck.h"


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
_r_luaF_close = (_r_luaF_close)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAF_CLOSE));
_r_luaH_resizearray r_luaH_resizearray = (_r_luaH_resizearray)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAH_RESIZEARRAY));
_r_luaH_setnum r_luaH_setnum = (_r_luaH_setnum)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAH_SETNUM));
_r_luaH_getn r_luaH_getn = (_r_luaH_getn)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAH_GETN));
_r_luaD_growstack r_luaD_growstack = (_r_luaD_growstack)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAD_GROWSTACK));
_r_luaG_newproto r_luaG_newproto = (_r_luaG_newproto)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAG_NEWPROTO));
_r_luaG_newupvalue r_luaG_newupvalue = (_r_luaG_newupvalue)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAG_NEWUPVALUE));
_r_rLua_tostring r_rLua_tostring = (_r_rLua_tostring)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUA_TOSTRING));
_r_lua_setfield r_lua_setfield = (_r_lua_setfield)ret.unprotect<DWORD>((BYTE*)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUA_SETFIELD)));
_r_lua_pcall r_lua_pcall = (_r_lua_pcall)ret.unprotect<DWORD>((BYTE*)(OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUA_PCALL)));

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
	DbgPrint("Converting TValue located at %x to %x\n", (DWORD)From, (DWORD)Destination);
	if (From == Destination)
	{
		DbgPrint("Conversion cannot proceed: From and Destination are the same TValue\n");
		SuspendThread(GetCurrentThread());
	}
	DbgPrint("From Type: %d\n", From->tt);
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
		DbgPrint("Was String: %s\n", str);
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
		v5 = *(DWORD*)(a1 + 104);
	else
		v5 = *(DWORD*)(**(DWORD**)(v4 + 16) + 12);
	v6 = UPVALUES;
	v7 = ((int(__cdecl*)(int, int, int))Addy2)(a1, UPVALUES, v5);
	*(DWORD*)(v7 + 16) = (int)a2 - (v7 + 16);
	v8 = 16 * UPVALUES;
	*(DWORD*)(a1 + 16) -= 16 * UPVALUES;
	v14 = v7;
	v15 = 16 *UPVALUES;
	if (v6)
	{
		v9 = v8 + v14 + 24;
		do
		{
			v9 -= 16;
			v10 = -24 - v14 + *(DWORD*)(v3 + 16);
			*(DWORD*)v9 = *(DWORD*)(v10 + v9);
			*(DWORD*)(v9 + 4) = *(DWORD*)(v10 + v9 + 4);
			*(DWORD*)(v9 + 8) = *(DWORD*)(v10 + v9 + 8);
			--v6;
		} while (v6);
	}
	v11 = *(DWORD*)(v3 + 16);
	*(DWORD*)v11 = v14;
	*(DWORD*)(v11 + 8) = 6;
	*(DWORD*)(v3 + 16) += 16;
	return v11;
}

TValue* Occult_luaX_pushcclosure_singleupvalue(DWORD rL, rlua_CFunction a2, TValue* upvalueinquestion) {
	// thx ida

	int v3, v4, v5, v6, v7, v8, v9, v10, v14;

	int a1 = rL;

	DWORD Addy1 = OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAC_STEP);
	DWORD Addy2 = OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUAF_NEWCCLOSURE);

	v3 = a1;
	if (*(DWORD*)(*(DWORD*)(a1 + 8) + a1 + 96) >= *(DWORD*)(*(DWORD*)(a1 + 8) + a1 + 84))
	{
		((int(__cdecl*)(int))Addy1)(a1);
		/*((int(__cdecl*)(int))Addy1)(a1);
		((int(__cdecl*)(int))Addy1)(a1);
		((int(__cdecl*)(int))Addy1)(a1);*/
	}
	v4 = *(DWORD*)(a1 + 12);
	if (v4 == *(DWORD*)(a1 + 40))
		v5 = *(DWORD*)(a1 + 104);
	else
		v5 = *(DWORD*)(**(DWORD**)(v4 + 16) + 12);
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
	DbgPrint("\nmemtest\n");
	DbgPrint("!! In Occult_luaV_execute\n");
reentry: /* entry point??? */
	lcl = &((Closure*)L->ci->func->value.gc)->l;
	pfn = lcl->p;
	*(DWORD*)(rL + 16) += pfn->maxstacksize * 16 + 0x20; // Temporary fix for stack oddity
	ccspool = (TValue*)r_luaM_malloc(rL, pfn->sizek * sizeof(TValue));
	ostack = *(DWORD*)(rL + 16);
	cspool = (DWORD)(DWORD*)pfn->k;
	DbgPrint("!! cspool: %x\n", cspool);
	pc = (const Instruction*)L->savedpc;
	k = pfn->k;
	DbgPrint("!! Obtained pc, base and k (%x, %x, %x)\n", (DWORD)pc, (DWORD)0, (DWORD)k);
	DbgPrint("!! Now converting constants\n");
	for (int x = NULL; x < pfn->sizek; x++)
		Occult_luaV_convertTValue(L, rL, &pfn->k[x], &(ccspool[x]));
	DbgPrint("----------------------------------------------------\n");
	for (;;)
	{
		const Instruction i = *pc++;
		DbgPrint("!! Running %d\n", GET_OPCODE(i));
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
				DbgPrint("GETGLOBAL KEY: %s\n", (const char*)(((TString*)ctn->value.gc) + 1));
			}
			r_luaV_gettable(rL, (DWORD)(DWORD*)rL + 104, (DWORD)ctn, ctx);
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
			r_luaV_settable(rL, (DWORD)(DWORD*)rL + 104, (DWORD)ctn, ctx);
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
				*(DWORD*)(rL + 16) = (DWORD)(ttval(a) + b);
			}
			//r_ptr_obfus(rL + 20, pc);
			DbgPrint("nresults: %d\n", nresults);
			L->savedpc = pc;
			switch (r_luaD_precall(rL, a, nresults)) {
			case 0: {
				DbgPrint("Was a lua call?\n");
				DbgPrint("Escaping to RBX VM.\n");
				Occult_rluaV_execute(rL, 1);
				continue;
			}
			case 1: {
				DbgPrint("Was a C call?\n");
				if (nresults >= 0) {
					*(DWORD*)(rL + 16) = *(DWORD*)(*(DWORD*)(rL + 12) + 8);
				}
				continue;
			}
			default:
				return 1;
			}
			continue;
		}

		case OP_RETURN:
		{
			DbgPrint("Caught return (dbg), terminating.\n");
			DWORD A = Occult_luaV_getonstack(rL, GETARG_A(i));
			DWORD B = GETARG_B(i);
			if (B = 0){
					return;
			}
			else if ( B >= 2 ) {
					return A = B - 1;
					r_luaF_close(rL, A);
			}
			else if ( B = 0 ) {
					return A;
					r_luaF_close(rL, A);
			}
			else {
					return TRUE;
			}

			continue;
			
			
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
				DbgPrint("'for' inital value must be number\n");
				return TRUE;
			}
			else if (!tonumber(plimit, plimit))
			{
				DbgPrint("'for' limit must be a number\n");
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
			*(DWORD*)(rL + 16) = Cb + (sizeof(TValue) * 3);
			r_ptr_obfus(rL + 20, pc);
			L->savedpc = pc;
			Occult_luaX_dcall(L, rL, pfn, lcl, ttval(Cb), GETARG_C(i));
			*(DWORD*)(rL + 16) = Occult_luaV_getonstack(rL, (((DWORD)(L->ci->top) - (DWORD)(L->top)) / 16));
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
				n = cast_int(*(DWORD*)(rL + 16) - A) - 1;
				*(DWORD*)(rL + 16) = Occult_luaV_getonstack(rL, (((DWORD)(L->ci->top) - (DWORD)(L->top)) / 16));
			}
			if (c == 0) c = cast_int(*pc++);
			if (ttval(A)->tt != R_LUA_TTABLE) break;
			table = ttval(A)->value.gc;
			last = ((c - 1) * 50) + n;
			if (last > *(DWORD*)((DWORD)table + 16))
				r_luaH_resizearray(rL, table, last);
			for (; n > 0; n--) {
				TValue* v = (TValue*)(A + (16 * n));
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

			DbgPrint("Lua State: 0x%x\n", (DWORD)rL);

			TValue* cclosure = Occult_luaX_pushcclosure_singleupvalue(rL, [](DWORD State)->DWORD {
				__try {
					DbgPrint("--- FUNCTION CALLED ---\n->value %x\n", Occult_luaX_touserdata(State, lua_upvalueindex(1)));
					PUserdata tcl = *(PUserdata*)Occult_luaX_touserdata(State, lua_upvalueindex(1));
					DbgPrint("Got tcl maybe???\n");
					DbgPrint("-> got tcl, location: %x\n", &tcl);
					DbgPrint("cl: 0x%x L: 0x%x rL: 0x%x\n", (DWORD)tcl.cl, (DWORD)tcl.L, (DWORD)tcl.rL);
					if (luaD_precall(tcl.L, tcl.cl, 0) == 0) {
						DbgPrint("Executing\n");
						Occult_luaV_execute(tcl.L, tcl.rL, 1);
					}
					return LUA_MULTRET;
				}
				__except (EXCEPTION_EXECUTE_HANDLER) {
					DbgPrint("Error occured while attempting to execute function.");
					return 0;
				}
			}, &rp);

			ttval(A)->tt = R_LUA_TFUNCTION;
			ttval(A)->value.gc = cclosure->value.gc;
			L->savedpc = pc;
			DbgPrint("function pushed to a!\n");
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
			DbgPrint("ERROR: Unknown instruction! %d\n", GET_OPCODE(i));
			return TRUE;
		}

		}
	}
	//left_vm_dbg:
	Occult_luaB_settop(rL, 0);
	return FALSE;
}

extern "C" int luaL_loadbuffer(lua_State* L, const char *buff, size_t sz, const char *name);

BOOL Occult_luaA_run(DWORD rL, const char* src)
{
	DbgPrint("\nIn Occult_luaR_run\n");
	Proto* fptn;
	LClosure* lcl;
	TValue* cl;
	Closure* cla;
	DbgPrint("realloc: %x\n", (DWORD)r_luaM_realloc_);
	DbgPrint("r_luaD_precall: %x\nr_luaV_gettable: %x\nXorConstant: %x\n", (DWORD)r_luaD_precall, (DWORD)r_luaV_gettable, (DWORD)XorConstant);
	lua_State* L = luaL_newstate();
	luaL_loadstring(L, src);
	if (lua_type(L, -1) != LUA_TFUNCTION) {
		printf("[Parsetime-Error] %s\n", lua_tostring(L, -1));
		lua_pop(L, -1);
		return FALSE;
	}
	cl = (L->top - 1);
	cla = ((Closure*)((L->top - 1)->value.p));
	lcl = &(cla->l);
	fptn = lcl->p;
	DbgPrint("(o)Lua State: %x\nProto: %x", (DWORD)L, (DWORD)fptn);
	DbgPrint("\n/!\\ Launching Occult virtual machine /!\\\n");
        //here
	DWORD th = r_lua_newthread(rL);
	Occult_luaB_pop(rL, 1);

	DWORD initState = OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUA_INITTHREAD);
	((void(__cdecl*)(DWORD rL))initState)(th);

	DbgPrint("Initiated roblox thread.\n");

	Occult_luaX_pushcclosure(th, [](DWORD rL)->DWORD {
		return Occult_luaX_getmetatable(rL, 1);
	}, 0);
	DbgPrint("kek2.\n");
	r_lua_setfield(th, -10002, "getrawmetatable");
	DbgPrint("kek3.\n");
	
	Occult_luaX_pushcclosure(th, [](DWORD rL)->DWORD {
		const char* src = r_rLua_tostring(rL, 1);
		lua_State* L = luaL_newstate();
		int Status = luaL_loadbuffer(L, src, strlen(src), "@OCCULT");
		if (Status == 0) {
			Closure* newClosure = ((Closure*)((L->top - 1)->value.p));

			TValue ancl;
			ancl.tt = LUA_TFUNCTION;
			ancl.value.gc = (GCObject*)newClosure;

			DWORD state = rL;

			PUserdata test;
			test.cl = &ancl;
			test.L = L;
			test.rL = state;

			TValue rp;
			rp.tt = R_LUA_TLIGHTUSERDATA;
			rp.value.p = &test;

			DbgPrint("Lua State: 0x%x\n", (DWORD)rL);

			TValue* cclosure = Occult_luaX_pushcclosure_singleupvalue(rL, [](DWORD State)->DWORD {
				__try {
					DbgPrint("--- FUNCTION CALLED ---\n->value %x\n", Occult_luaX_touserdata(State, lua_upvalueindex(1)));
					PUserdata tcl = *(PUserdata*)Occult_luaX_touserdata(State, lua_upvalueindex(1));
					DbgPrint("Got tcl maybe???\n");
					DbgPrint("-> got tcl, location: %x\n", &tcl);
					DbgPrint("cl: 0x%x L: 0x%x rL: 0x%x\n", (DWORD)tcl.cl, (DWORD)tcl.L, (DWORD)State);
					if (luaD_precall(tcl.L, tcl.cl, 0) == 0) {
						DbgPrint("Executing\n");
						Occult_luaV_execute(tcl.L, State, 1);
					}
					return LUA_MULTRET;
				}
				__except (EXCEPTION_EXECUTE_HANDLER) {
					DbgPrint("Error occured while attempting to execute function.");
					return 0;
				}
			}, &rp);

			TValue* rbxTop = ttval(*(DWORD*)(rL + 16));
			rbxTop->tt = R_LUA_TFUNCTION;
			rbxTop->value.gc = cclosure->value.gc;
			*(DWORD*)(rL + 16) += 16;
			return 1;
		}
		else {
			TValue* rbxTop = ttval(*(DWORD*)(rL + 16));
			rbxTop->tt = R_LUA_TNIL;
			*(DWORD*)(rL + 16) += 16;

			const char* errorMsg = lua_tostring(L, -1);
			lua_pop(L, -1);

			TValue* rbxTop2 = ttval(*(DWORD*)(rL + 16));
			rbxTop2->tt = R_LUA_TSTRING;
			rbxTop2->value.gc = (GCObject*)r_luaS_newlstr(rL, errorMsg, strlen(errorMsg));
			*(DWORD*)(rL + 16) += 16;

			return 2;
		}
		return 0;
	}, 0);
	DbgPrint("kek4.\n");
	r_lua_setfield(th, -10002, "loadstring");

	DbgPrint("Set globals.\n");

	*(DWORD*)(th + 20) ^= (7 ^ *(DWORD*)(th + 20)) & 0x1F;
	*(DWORD*)OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_IDENTITYFlAG) = 1;
	*(DWORD*)OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_DEFAULTIDENTITY) = 7;

	/*TValue ancl;
	ancl.tt = LUA_TFUNCTION;
	ancl.value.gc = (GCObject*)cla;

	PUserdata test;
	test.cl = &ancl;
	test.L = L;
	test.rL = rL;

	TValue rp;
	rp.tt = R_LUA_TLIGHTUSERDATA;
	rp.value.p = &test;

	TValue* cclosure = Occult_luaX_pushcclosure_singleupvalue(rL, [](DWORD State)->DWORD {
		__try {
			DbgPrint("--- FUNCTION CALLED ---\n->value %x\n", Occult_luaX_touserdata(State, lua_upvalueindex(1)));
			PUserdata tcl = *(PUserdata*)Occult_luaX_touserdata(State, lua_upvalueindex(1));
			DbgPrint("Got tcl maybe???\n");
			DbgPrint("-> got tcl, location: %x\n", &tcl);
			DbgPrint("cl: 0x%x L: 0x%x rL: 0x%x\n", (DWORD)tcl.cl, (DWORD)tcl.L, (DWORD)tcl.rL);
			if (luaD_precall(tcl.L, tcl.cl, 0) == 0) {
				DbgPrint("Executing\n");
				Occult_luaV_execute(tcl.L, tcl.rL, 1);
			}
			return LUA_MULTRET;
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			DbgPrint("Error occured while attempting to execute function.");
			return 0;
		}
	}, &rp);
	TValue* rbxTop = ttval(*(DWORD*)(rL + 16));
	rbxTop->tt = R_LUA_TFUNCTION;
	rbxTop->value.gc = cclosure->value.gc;
	*(DWORD*)(rL + 16) += 16;
	r_lua_pcall(rL, 0, 0, 0);*/

	try {
		if (luaD_precall(L, cl, 0) == 0) {
			DbgPrint("luaD_precall return'd 0 executing.\n");
			Occult_luaV_execute(L, th, 1);
		}
	}
	catch (std::exception e) {
		printf("[Runtime-Error] %s\n", e.what());
	}

	return TRUE;
}

#pragma once
#include "MainLua.h"

using namespace std;

typedef int(__cdecl *r_lua_createttable_Def)(DWORD luaState, int object_c);
r_lua_createttable_Def r_lua_createttable = (r_lua_createttable_Def)rbxaddy(0x708DC0);

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

typedef TValue r_TValue;

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

template <typename T> class LuaVMValue
{
public:
	operator const T() const
	{
		return (T)((uintptr_t)storage + reinterpret_cast<uintptr_t>(this));
	}

	void operator=(const T& value)
	{
		storage = (T)((uintptr_t)value - reinterpret_cast<uintptr_t>(this));
	}

	const T operator->() const
	{
		return operator const T();
	}

private:
	T storage;
};

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



struct r_lua_State {
	CommonHeader;
	lu_byte status;

	LUAVM_SHUFFLE7(;,
		StkId top,  /* first free slot in the stack */
		StkId base,  /* base of current function */
		void* l_G;
	CallInfo *ci,  /* call info for current function */
		LuaVMValue<const Instruction*> savedpc,  /* `savedpc' of current function */
		StkId stack_last,  /* last free slot in the stack */
		StkId stack); /* stack base */

	LUAVM_SHUFFLE4(;,
		CallInfo *end_ci,  /* points after end of ci array*/
		CallInfo *base_ci,  /* array of CallInfo's */
		int stacksize,
		int size_ci);  /* size of array `base_ci' */

	LUAVM_SHUFFLE2(;,
		unsigned short nCcalls,  /* number of nested C calls */
		unsigned short baseCcalls);  /* nested C calls when resuming coroutine */

	LUAVM_SHUFFLE2(;,
		lu_byte hookmask,
		lu_byte allowhook);

	LUAVM_SHUFFLE9(;,
		int basehookcount,
		int hookcount,
		lua_Hook hook,
		TValue l_gt,  /* table of globals */
		TValue env,  /* temporary place for environments */
		GCObject *openupval,  /* list of open upvalues in this stack */
		GCObject *gclist,
		struct lua_longjmp *errorJmp,  /* current error recover point */
		ptrdiff_t errfunc);  /* current error handling function (stack index) */
};

void rev_luaC_link(int r_lua_State, int o, byte tt);
void* rev_luaM_realloc_(int r_lua_State, int block, int osize, int nsize);
r_Proto* rev_luaF_newproto(int r_lua_State);

void rev_luaC_link(int r_lua_State, int o, byte tt)
{
	int g = lint_deptr(r_lua_State + 8);	// global_State *g = G(L)
	*(int*)(o) = *(int*)(g + 48);			// o->gch.next = g->rootgc
	*(int*)(g + 48) = o;					// g->rootgc = o
	*(byte*)(o + 4) = *(byte*)(g + 21) & 3;	// o->gch.marked = luaC_white(g)
	*(byte*)(o + 5) = tt;					// o->gch.tt = tt;
}

void* rev_luaM_realloc_(int r_lua_State, int block, int osize, int nsize)
{
	return nullptr;
}

r_Proto* rev_luaF_newproto(int r_lua_State)
{
	r_Proto* f = rev_luaM_new(r_lua_State, r_Proto);
	rev_luaC_link(r_lua_State, (int)f, R_LUA_TPROTO);
	ZeroMemory(f, sizeof(r_Proto));
	return f;
}

#define rev_setobj(rL, a, b) *(a) = *(b)
#define rev_setobjs2s setobj
#define rev_setobj2s setobj
#define rev_setbvalue(obj, x) { r_TValue *i_o=(obj); i_o->value.b=(x); i_o->tt=R_LUA_TBOOLEAN; }
#define rev_setnilvalue(obj) ((obj)->tt=R_LUA_TNIL)
#define rev_setnvalue(obj,x) { TValue *i_o=(obj); i_o->value.n=(x); i_o->tt=R_LUA_TNUMBER; }
#define rev_sethvalue { TValue *i_o=(obj); i_o->value.gc=cast(GCObject *, (x)); i_o->tt=R_LUA_TTABLE; }
#define rev_vmtry(x) { __try{x;} __except(rev_luaV_vmcatch(GetExceptionINformation()){})}
#define runtime_check(L, c)	{ if (!(c)) break; }
#define RA(i)	(base+GETARG_A(i))
/* to be used after possible stack reallocation */
#define RB(i)	check_exp(getBMode(GET_OPCODE(i)) == OpArgR, base+GETARG_B(i))
#define RC(i)	check_exp(getCMode(GET_OPCODE(i)) == OpArgR, base+GETARG_C(i))
#define RKB(i)	check_exp(getBMode(GET_OPCODE(i)) == OpArgK, \
	ISK(GETARG_B(i)) ? k+INDEXK(GETARG_B(i)) : base+GETARG_B(i))
#define RKC(i)	check_exp(getCMode(GET_OPCODE(i)) == OpArgK, \
	ISK(GETARG_C(i)) ? k+INDEXK(GETARG_C(i)) : base+GETARG_C(i))
#define KBx(i)	check_exp(getBMode(GET_OPCODE(i)) == OpArgK, k+GETARG_Bx(i))
#define dojump(L,pc,i)	{(pc) += (i); luai_threadyield(L);}
#define Protect(x)      { L->savedpc = pc; {x;}; base = L->base; }
#define rev_luaC_barrier(rL,p,v) { if (valiswhite(v) && isblack(obj2gco(p)))  \
        luaC_barrierf(rL,obj2gco(p),gcvalue(v)); }

namespace rblx {
	typedef void(__cdecl* lua_setfield)(DWORD a, int b, const char* c);
	typedef int(__cdecl* luaV_gettable)(DWORD a, DWORD b, DWORD c, DWORD d);
	typedef int(__cdecl* luaV_settable)(DWORD a, DWORD b, DWORD c, DWORD d);
	typedef int(__cdecl* luaD_precall)(DWORD a, DWORD b, DWORD c);
}
#pragma endregion

rblx::luaV_gettable r_luaV_gettable = (rblx::luaV_gettable)(unprotect(rbxaddy(0x709550)));
rblx::luaV_settable r_luaV_settable = (rblx::luaV_settable)(unprotect(rbxaddy(0x50F5D0))); //notupdated


typedef TValue r_TValue;

DWORD rev_luaV_vmcatch(PEXCEPTION_POINTERS ExInfo)
{
	printf("[!] An error occured! Exception information:\n-> EIP: %x\n-> Error code: %d\n", ExInfo->ContextRecord->Eip, GetLastError());
	return EXCEPTION_EXECUTE_HANDLER;
}

r_Proto* rev_luaV_specproto(r_lua_State* rL, Proto* Specification)
{
	r_Proto* rp = rev_luaF_newproto((DWORD)rL);
	rp->is_vararg = Specification->is_vararg;
	rp->maxstacksize = Specification->maxstacksize;
	rp->numparams = Specification->numparams;
	return rp;
}

const TValue* rev_luaV_tonumber(const TValue *obj, TValue *n) {

	lua_Number number;
	if (obj->tt == R_LUA_TNUMBER)
		return obj;

	setnvalue(n, number);
	return n;


}

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


void rev_Arith(r_lua_State* rL, TValue*  A, TValue* B, TValue* C, TMS t_op) {
	TValue tempb, tempc;
	const TValue *b, *c;
	if ((b = rev_luaV_tonumber(B, &tempb)) != NULL &&
		(c = rev_luaV_tonumber(C, &tempc)) != NULL) {
		LUA_NUMBER nb = check_exp(ttisnumber(b), (b)->value.n);
		LUA_NUMBER nc = check_exp(ttisnumber(c), (c)->value.n);
		switch (t_op) {
		case TM_ADD:
			rev_setnvalue(A, rev_luai_numadd(nb, nc));
			break;
		case TM_SUB:
			rev_setnvalue(A, rev_luai_numsub(nb, nc));
			break;
		case TM_MUL:
			rev_setnvalue(A, rev_luai_nummul(nb, nc));
			break;
		case TM_DIV:
			rev_setnvalue(A, rev_luai_numdiv(nb, nc));
			break;
		case TM_MOD:
			rev_setnvalue(A, rev_luai_nummod(nb, nc));
			break;
		case TM_POW:
			rev_setnvalue(A, rev_luai_numpow(nb, nc));
			break;
		case TM_UNM:
			rev_setnvalue(A, rev_luai_numunm(nb));
			break;
		default:
			((void)0);
			break;
		}
	}
}

void rev_luaV_execute(DWORD rL, lua_State * L, int nexeccalls)
{
	LClosure *cl;
	StkId base;
	TValue *k;
	const Instruction *pc;
reentry:
	pc = L->savedpc;
	cl = (LClosure*)L->ci->func->value.gc;
	base = L->base;
	k = cl->p->k;
	while (1) {
		const Instruction i = *pc++;
		StkId ra;
		ra = RA(i);
		switch (GET_OPCODE(i)) {
		case OP_MOVE: {
			rev_setobjs2s(rL, ra, RB(i));
			continue;
		}
		case OP_LOADK: {
			rev_setobj2s(rL, ra, KBx(i));
			continue;
		}
		case OP_LOADBOOL: {
			rev_setbvalue(ra, GETARG_B(i));
			if (GETARG_C(i)) pc++;  /* skip next instruction (if C) */
			continue;
		}
		case OP_LOADNIL: {
			TValue *rb = RB(i);
			do {
				rev_setnilvalue(rb--);
			} while (rb >= ra);
			continue;
		}
		case OP_GETUPVAL: {
			int b = GETARG_B(i);
			rev_setobj2s(rL, ra, cl->upvals[b]->v);
			continue;
		}
		case OP_SETUPVAL: {
			UpVal *uval = cl->upvals[GETARG_B(i)];
			rev_setobj(rL, uval->v, ra);
			continue;
		}
		case OP_JMP: {
			dojump(rL, pc, GETARG_sBx(i));
			continue;
		}
		case OP_GETTABLE: {
			//we have to use rblxs func for these, dont think we can ref our own
			Protect(r_luaV_gettable(*(DWORD*)rL, (DWORD)RB(i), (DWORD)RKC(i), (DWORD)ra));
			continue;
		}
		case OP_SETTABLE: {
			Protect(r_luaV_settable((DWORD)rL, (DWORD)RB(i), (DWORD)RKC(i), (DWORD)ra));
			continue;
		}

		case OP_ADD: {
			TValue *rb = RKB(i);
			TValue *rc = RKC(i);
			if (R_LUA_TNUMBER == (DWORD)rb && R_LUA_TNUMBER == (DWORD)rc) {
				LUA_NUMBER nb = check_exp(ttisnumber(rb), (rb)->value.n);
				LUA_NUMBER nc = check_exp(ttisnumber(rc), (rc)->value.n);
				rev_setnvalue(ra, rev_luai_numadd(nb, nc));
			}
			else
				//Protect(rev_Arith((DWORD)rL, ra, rb, rc, TM_ADD));

			continue;
		}
		case OP_SUB: {
			TValue *rb = RKB(i);
			TValue *rc = RKC(i);
			if (R_LUA_TNUMBER == (DWORD)rb && R_LUA_TNUMBER == (DWORD)rc) {
				LUA_NUMBER nb = check_exp(ttisnumber(rb), (rb)->value.n);
				LUA_NUMBER nc = check_exp(ttisnumber(rc), (rc)->value.n);
				rev_setnvalue(ra, rev_luai_numsub(nb, nc));
			}
			else
//				Protect(rev_Arith(rL, ra, rb, rc, TM_SUB));

			continue;
		}
		case OP_MUL: {
			TValue *rb = RKB(i);
			TValue *rc = RKC(i);
			if (R_LUA_TNUMBER == (DWORD)rb && R_LUA_TNUMBER == (DWORD)rc) {
				LUA_NUMBER nb = check_exp(ttisnumber(rb), (rb)->value.n);
				LUA_NUMBER nc = check_exp(ttisnumber(rc), (rc)->value.n);
				rev_setnvalue(ra, rev_luai_nummul(nb, nc));
			}
			else
//				Protect(rev_Arith(rL, ra, rb, rc, TM_MUL));

			continue;
		}
		case OP_DIV: {
			TValue *rb = RKB(i);
			TValue *rc = RKC(i);
			if (R_LUA_TNUMBER == (DWORD)rb && R_LUA_TNUMBER == (DWORD)rc) {
				LUA_NUMBER nb = check_exp(ttisnumber(rb), (rb)->value.n);
				LUA_NUMBER nc = check_exp(ttisnumber(rc), (rc)->value.n);
				rev_setnvalue(ra, rev_luai_numdiv(nb, nc));
			}
			else
	//			Protect(rev_Arith(rL, ra, rb, rc, TM_DIV));

			continue;
		}
		case OP_POW: {
			TValue *rb = RKB(i);
			TValue *rc = RKC(i);
			if (R_LUA_TNUMBER == (DWORD)rb && R_LUA_TNUMBER == (DWORD)rc) {
				LUA_NUMBER nb = check_exp(ttisnumber(rb), (rb)->value.n);
				LUA_NUMBER nc = check_exp(ttisnumber(rc), (rc)->value.n);
				rev_setnvalue(ra, rev_luai_numpow(nb, nc));
			}
			else
			//	Protect(rev_Arith(rL, ra, rb, rc, TM_POW));

			continue;
		}
		case OP_UNM: {
			TValue *rb = RKB(i);
			TValue *rc = RKC(i);
			if (R_LUA_TNUMBER == (DWORD)rb && R_LUA_TNUMBER == (DWORD)rc) {
				LUA_NUMBER nb = check_exp(ttisnumber(rb), (rb)->value.n);
				LUA_NUMBER nc = check_exp(ttisnumber(rc), (rc)->value.n);
				rev_setnvalue(ra, rev_luai_numunm(nb, nc));
			}
			else
		//		Protect(rev_Arith(rL, ra, rb, rc, TM_UNM));

			continue;
		}
		}
	}
}

BOOL luaA_run(DWORD rL, const char* src)
{
	Proto* fptn;
	LClosure* lcl;
	TValue* cl;
	//DbgPrint("realloc: %x\n", (DWORD)r_luaM_realloc_);
//	DbgPrint("r_luaD_precall: %x\nr_luaV_gettable: %x\nXorConstant: %x\n", (DWORD)r_luaD_precall, (DWORD)r_luaV_gettable, (DWORD)XorConstant);
	lua_State* L = luaL_newstate();
	luaL_loadstring(L, src);
	if (lua_type(L, -1) != LUA_TFUNCTION) {
		printf("[Parsetime-Error] %s\n", lua_tostring(L, -1));
		lua_pop(L, -1);
		return FALSE;
	}
	cl = (L->top - 1);
	lcl = &(((Closure*)((L->top - 1)->value.p))->l);
	fptn = lcl->p;
	//DbgPrint("(o)Lua State: %x\nProto: %x", (DWORD)L, (DWORD)fptn);
	//DbgPrint("\n/!\\ Launching Occult virtual machine /!\\\n");
	DWORD th = rbxlua_newthread((DWORD)rL);

	//Occult_luaB_pop(th, 1);

	//DWORD initState = OCCULT_OFFSET(OCCULT_IDABASE, OCCULT_RLUA_INITTHREAD);
	//((void(__cdecl*)(DWORD rL))initState)(th);

	//DbgPrint("Initiated roblox thread.\n");
	//DbgPrint("kek2.\n");
	//r_lua_setfield(th, -10002, "getrawmetatable");
	//DbgPrint("kek3.\n");
	
}
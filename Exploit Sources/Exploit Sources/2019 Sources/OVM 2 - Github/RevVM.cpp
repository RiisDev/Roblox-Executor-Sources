
#include "RevVM.hpp"
#include "RevLAPI.hpp"
#include "RevLINT.hpp"
extern "C" {
#include "../Lua/lobject.h"
#include "../Lua/lfunc.h"
#include "../Lua/lopcodes.h"
#include "../Lua/lgc.h"
#include "..//Lua/ldo.h"
}
#include <math.h>  
VOID rSetObj(DWORD rL, TValue* obj1, TValue* obj2)
{
	obj1->value = obj2->value;
	obj1->tt = obj2->tt;
}
#pragma region RevVM Defines 
#define rev_setobj(rL, a, b) rSetObj(rL, a, b)
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
	ISK(GETARG_B(i)) ? r_realloc+INDEXK(GETARG_B(i)) : base+GETARG_B(i))
#define RKC(i)	check_exp(getCMode(GET_OPCODE(i)) == OpArgK, \
	ISK(GETARG_C(i)) ? r_realloc+INDEXK(GETARG_C(i)) : base+GETARG_C(i))
#define KBx(i)	check_exp(getBMode(GET_OPCODE(i)) == OpArgK, r_realloc+GETARG_Bx(i))
#define dojump(L,pc,i)	{(pc) += (i); luai_threadyield(L);}
#define Protect(x)      { L->savedpc = pc; {x;}; base = L->base; }
#define rev_luaC_barrier(rL,p,v) { if (valiswhite(v) && isblack(obj2gco(p)))  \
        luaC_barrierf(rL,obj2gco(p),gcvalue(v)); }
#pragma endregion

DWORD unprotect(DWORD addr)
{
	BYTE* tAddr = (BYTE*)addr;

	/*  Calcualte the size of the function.

		In theory this will run until it hits the next
		functions prolog. It assumes all calls are aligned to
		16 bytes. (grazie katie)
	*/
	do
	{
		tAddr += 16;
	} while (!(tAddr[0] == 0x55 && tAddr[1] == 0x8B && tAddr[2] == 0xEC));

	DWORD funcSz = tAddr - (BYTE*)addr;

	/* Allocate memory for the new function */
	PVOID nFunc = VirtualAlloc(NULL, funcSz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (nFunc == NULL)
		return addr;

	/* Copy the function to the newly allocated memory */
	memcpy(nFunc, (void*)addr, funcSz);

	BYTE* pos = (BYTE*)nFunc;
	BOOL valid = false;
	do
	{
		/*  Check for the return check with the sig:
			72 ?? A1 ?? ?? ?? ?? 8B

			If the sig matches replace the the jb with a jmp.
		*/
		if (pos[0] == 0x72 && pos[2] == 0xA1 && pos[7] == 0x8B) {
			*(BYTE*)pos = 0xEB;

			DWORD cByte = (DWORD)nFunc;
			do
			{
				/*  Check if the current byte is a call, if it is,
					calculate the new relative call(s).

					*(->E8 + 1) = originalFunction - nextInstruction

					oFuncPos - Position of call in original function
						= originalFunction + (->E8 - newFunction)

					oFuncAddr - Original call location
						= oFuncPos + rel32Offset + sizeof(call)

					relativeAddr - New relative address
						= oFuncAddr - ->E8 - sizeof(call)

					Since we are not using a disassembler we assume
					that if we hit a E8 byte which is properly aligned
					it is a relative call.
					For a small amount of compensation I skip the location
					of the call, since it is possible to have the byte
					E8 inside of it.
				*/
				if (*(BYTE*)cByte == 0xE8)
				{
					DWORD oFuncPos = addr + (cByte - (DWORD)nFunc);
					DWORD oFuncAddr = (oFuncPos + *(DWORD*)(oFuncPos + 1)) + 5;

					if (oFuncAddr % 16 == 0)
					{
						DWORD relativeAddr = oFuncAddr - cByte - 5;
						*(DWORD*)(cByte + 1) = relativeAddr;

						/* Don't check rel32 */
						cByte += 4;
					}
				}

				cByte += 1;
			} while (cByte - (DWORD)nFunc < funcSz);

			valid = true;
		}
		pos += 1;
	} while ((DWORD)pos < (DWORD)nFunc + funcSz);

	/* This function has no return check, let's not waste memory */
	if (!valid)
	{
		VirtualFree(nFunc, 0, MEM_RELEASE);
		return addr;
	}

	return (DWORD)nFunc;
}


rblx::luaV_gettable r_luaV_gettable = (rblx::luaV_gettable)(rev_Offset(r_luaV_gettable_addy));
rblx::luaV_settable r_luaV_settable = (rblx::luaV_settable)(rev_Offset(r_luaV_settable_addy));
rblx::lua_setfield r_lua_setfield = (rblx::lua_setfield)(rev_Offset(r_lua_setfield_addy));
rblx::luaF_close r_luaF_close = (rblx::luaF_close)(rev_Offset(r_luaF_close_addy));
rMRealloc r_luaM_realloc_ = (rMRealloc)(rev_Offset(0x848340));
typedef TString* (__cdecl* _luaS_newlstr)(uintptr_t, const char*, size_t);
_luaS_newlstr r_luaS_newlstr = (_luaS_newlstr)(rev_Offset(0x8489E0));
rNewThread r_lua_newthread = (rNewThread)(unprotect(rev_Offset(0x84E0C0)));
rblx::luaD_precall r_luaD_precall = (rblx::luaD_precall)(unprotect(rev_Offset(0x8458d0)));
typedef TValue r_TValue;

DWORD rev_luaV_vmcatch(PEXCEPTION_POINTERS ExInfo)
{
	printf("[!] An error occured! Exception information:\n-> EIP: %x\n-> Error code: %d\n", ExInfo->ContextRecord->Eip, GetLastError());
	return EXCEPTION_EXECUTE_HANDLER;
}



const TValue* rev_luaV_tonumber(const TValue *obj, TValue *n) {
	
	lua_Number number = 0;
	if (obj->tt == R_LUA_TNUMBER) 
		return obj;

	setnvalue(n, number);
	return n;
	
	
}


void rev_Arith(DWORD rL, TValue*  A, TValue* B, TValue* C, TMS t_op) {
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

void PushTValues(DWORD rL, lua_State* L, TValue* vanilla, TValue* rbx)
{
	switch (vanilla->tt)
	{
	case LUA_TNIL:
	{
		std::cout << "suprieor way of LUA_TNIL\n";
		rbx->tt = R_LUA_TNIL;
		// no need to set anything
		break;
	}

	case LUA_TBOOLEAN:
	{
		std::cout << "suprieor way of LUA_TBOOLEAN\n";
		rbx->tt = R_LUA_TBOOLEAN;
		rbx->value.b = vanilla->value.b;
		break;
	}

	case LUA_TSTRING:
	{
		std::cout << "suprieor way of LUA_TSTRING\n";
		TString* ts = (TString*)(vanilla->value.gc);
		const char* str = (const char*)(ts + 1);
		rbx->tt = R_LUA_TSTRING;
		rbx->value.gc = reinterpret_cast<GCObject*>(r_luaS_newlstr(rL, str, ts->tsv.len));
		printf_s("Was String: %s\n", str);
		break;


	}

	case LUA_TLIGHTUSERDATA:
	{
		std::cout << "suprieor way of LUA_TLIGHTUSERDATA\n";
		rbx->tt = R_LUA_TLIGHTUSERDATA;
		rbx->value.gc = reinterpret_cast<GCObject*>(vanilla->value.gc);
		break;
	}

	case LUA_TUSERDATA:
	{
		std::cout << "suprieor way of LUA_TUSERDATA\n";
		rbx->tt = R_LUA_TUSERDATA;
		rbx->value.gc = reinterpret_cast<GCObject*>(vanilla->value.gc);
		break;//woops forgot to break
	}

	}
}


void rev_luaV_execute(DWORD rL, lua_State * L, int nexeccalls)
{
	LClosure* cl;
	StkId base;
	TValue* k;
	const Instruction* pc;
reentry:  /* entry point */
	lua_assert(isLua(L->ci));
	pc = L->savedpc;
	cl = &clvalue(L->ci->func)->l;
	base = L->base;
	k = cl->p->k;
	/* main loop of interpreter */

	printf("cl: %x\n", cl);
	TValue* r_realloc = (TValue*)r_luaM_realloc_(rL, 0, 0, 16 * cl->p->sizek, 3);
	for (int x = NULL; x < cl->p->sizek; x++)
	{
		PushTValues(rL, L, &k[x], &r_realloc[x]);
	}
	while(1) {
		const Instruction i = *pc++;
		StkId ra;
		printf_s("opcodes: %s\n", luaP_opnames[GET_OPCODE(i)]);
		ra = RA(i);
		switch (GET_OPCODE(i)) {
			case OP_MOVE: {
				rev_setobjs2s(rL, ra, RKB(i));
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
			case OP_GETGLOBAL: {
				TValue* rb = KBx(i);
				Protect(r_luaV_gettable((DWORD)rL, rL + 64, (DWORD)rb, (DWORD)ra));
				continue;
			}
			case OP_GETTABLE: {
				//we have to use rblxs func for these, dont think we can ref our own
				Protect(r_luaV_gettable((DWORD)rL, (DWORD)RKB(i), (DWORD)RKC(i), (DWORD)ra));
				continue;
			}

			case OP_SETGLOBAL: {
				Protect(r_luaV_settable(rL, rL + 64, (DWORD)KBx(i), (DWORD)ra));
				continue;
			}
			case OP_SETTABLE: {
				Protect(r_luaV_settable((DWORD)rL, (DWORD)ra, (DWORD)RKB(i), (DWORD)RKC(i)));
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
					Protect(rev_Arith(rL, ra, rb, rc, TM_ADD));
				
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
					Protect(rev_Arith(rL, ra, rb, rc, TM_SUB));

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
					Protect(rev_Arith(rL, ra, rb, rc, TM_MUL));

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
					Protect(rev_Arith(rL, ra, rb, rc, TM_DIV));

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
					Protect(rev_Arith(rL, ra, rb, rc, TM_POW));

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
					Protect(rev_Arith(rL, ra, rb, rc, TM_UNM));

				continue;
			}
			case OP_CALL: {
				int b = GETARG_B(i);
				int nresults = GETARG_C(i) - 1;
				if (b != 0) *(StkId*)(rL + 20) = ra + b;  /* else previous instruction set top */
				switch (r_luaD_precall(rL, (DWORD)ra, nresults)) {
				case PCRLUA: {
					nexeccalls++;
					goto reentry;  /* restart luaV_execute over new Lua function */
				}
				case PCRC: {
					/* it was a C function (`precall' called it); adjust results */
					if (nresults >= 0)* (DWORD*)(rL + 20) = *(DWORD*)(*(DWORD*)(rL + 16) + 4);

					continue;
				}
				default: {
					return;  /* yield */
				}
				}
			}
			case OP_CLOSE:
			{
				r_luaF_close(rL, (int)ra);
				continue;
			}
			case OP_RETURN:
			{
				return;
			}
		}
	}
}

VOID RunScript(DWORD rL, lua_State* L, const std::string& source)
{
	int meme = luaL_loadstring(L, source.c_str());
	int th = r_lua_newthread(rL);
	TValue* func = L->top - 1;
	if (luaD_precall(L, func, 0) == 0)
	{
		rev_luaV_execute(th, L, 1);
	}
}
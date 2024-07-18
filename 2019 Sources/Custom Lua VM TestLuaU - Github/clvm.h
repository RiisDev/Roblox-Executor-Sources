#pragma once

/*includes*/

#include "Xor_String.h"
#include "rlua.h"
#include "CallCheck.h"
#include <thread>

/*defines and functions*/

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


#define Protect(x)	{ L->savedpc = pc; {x;}; base = L->base; }

/*arith define*/
#define rttisnumber(o)	(ttype(o) == RLUA_TNUMBER)

#define rnvalue(o)	check_exp(rttisnumber(o), (o)->value.n)

#define arith_op(rL, op,tm) { \
        TValue *rb = RKB(i); \
        TValue *rc = RKC(i); \
        if (rttisnumber(rb) && rttisnumber(rc)) { \
          lua_Number nb = rnvalue(rb), nc = rnvalue(rc); \
          rsetnvalue(ra, op(nb, nc)); \
        } \
        else \
          Protect(rlua_arith(rL, ra, rb, rc, tm)); \
      }



inline void lua_pushfclosure(lua_State* L, LClosure* fc)
{
	TValue* top = L->top;
	top->tt = LUA_TFUNCTION;
	top->value.gc = cast(GCObject*, fc);
	L->top++;
}


namespace Bridge
{
	int rbxFunctionBridge(int rL)
	{
		lua_State* nL = (lua_State*)rlua_touserdata(rL, lua_upvalueindex(1));
		Closure* ncl = (Closure*)rlua_touserdata(rL, lua_upvalueindex(2));
		lua_pushfclosure(nL, reinterpret_cast<LClosure*>(ncl));
		printf("rL: %x\n", rL);

		TValue* clval = nL->top - 1;

		if (luaD_precall(nL, clval, 0) == 0)
		{
			rluaV_execute(rL, nL, 1);
		}
		return 0;
	}

}
/*actual clvm execution*/

void PushTValues(DWORD rL, lua_State* L, TValue* vanilla, TValue* rbx)
{
	printf("[type]: Constant %d\n", vanilla->tt);
	switch (vanilla->tt)
	{
	case LUA_TNIL:
	{
		
		rbx->tt = RLUA_TNIL;
		// no need to set anything
		break;
	}

	

	case LUA_TSTRING:
	{

		TString* ts = (TString*)(vanilla->value.gc);
		const char* str = (const char*)(ts + 1);
		rbx->tt = RLUA_TSTRING;
		rbx->value.gc = reinterpret_cast<GCObject*>(rluaS_newlstr(rL, str, ts->tsv.len));
		break;


	}

	case LUA_TNUMBER:
	{

		double n = Occult_luaX_ConvertNumber(vanilla->value.n);
		rbx->tt = RLUA_TNUMBER;
		rbx->value.n = n;
		break;
	}

	case LUA_TBOOLEAN:
	{

		rbx->tt = RLUA_TBOOLEAN;
		rbx->value.b = vanilla->value.b;
		break;
	}


	case LUA_TLIGHTUSERDATA:
	{
		
		rbx->tt = RLUA_TLIGHTUSERDATA;
		rbx->value.gc = reinterpret_cast<GCObject*>(vanilla->value.gc);
		break;
	}

	case LUA_TUSERDATA:
	{
	
		rbx->tt = RLUA_TUSERDATA;
		rbx->value.gc = reinterpret_cast<GCObject*>(vanilla->value.gc);
		break;//woops forgot to break
	}

	}
}


#define rttistable(o)	(ttype(o) == RLUA_TTABLE)
#define rttisboolean(o)	(ttype(o) == RLUA_TBOOLEAN)
#define rl_isfalse(o)	(ttisnil(o) || (rttisboolean(o) && bvalue(o) == 0))
#define rhvalue(o)	check_exp(rttistable(o), &(o)->value.gc->h)
uint32_t rluaV_execute(uintptr_t rL, lua_State* L, int nextCalls)
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

	
	
	

	k = reinterpret_cast<TValue*>(rluaM_realloc_(rL, 0, 0, 16 * cl->p->sizek, 3));
	/* main loop of interpreter */


	
	for (;;) {
		const Instruction i = *pc++;

		for (auto x = NULL; x < cl->p->sizek; x++)
		{
			PushTValues(rL, L, &cl->p->k[x], &k[x]);
		}
	
		
		std::cout <<
			"Running \"" << "OP_" << luaP_opnames[GET_OPCODE(i)] << "\" " <<
			"(" << GET_OPCODE(i) << ")" << std::endl;

		
		StkId ra;

		ra = RA(i);




		switch (GET_OPCODE(i))
		{

			/*begining of exec*/


		case OP_MOVE: {
			setObject(ra, RB(i));
			continue;
		}

		case OP_LOADK:
		{
			setObject(ra, KBx(i));
			continue;
		}

		case OP_LOADBOOL: {
			rsetbvalue(ra, GETARG_B(i));
			if (GETARG_C(i)) pc++;  /* skip next instruction (if C) */
			continue;
		}
		case OP_LOADNIL: {
			TValue* rb = RB(i);
			do {
				rsetnilvalue(rb--);
			} while (rb >= ra);
			continue;
		}
		case OP_GETUPVAL: {
			int b = GETARG_B(i);
			setObject(ra, cl->upvals[b]->v);
			continue;
		}

		case OP_GETGLOBAL:
		{
		
			Protect(rluaV_gettable(rL, rlua_index2adr(rL, -10002), KBx(i), ra));
			continue;
		}



		case OP_GETTABLE: {

			Protect(rluaV_gettable(rL, RB(i), RKC(i), ra));
			continue;
		}
		case OP_SETGLOBAL: {

			Protect(rluaV_settable(rL, rlua_index2adr(rL, -10002), KBx(i), ra));
			continue;
		}
		case OP_SETUPVAL: {
			UpVal* uv = cl->upvals[GETARG_B(i)];
			setObject(uv->v, ra);
		
			continue;
		}
		case OP_SETTABLE: {
			Protect(rluaV_settable(rL, ra, RKB(i), RKC(i)));
			continue;
		}
		case OP_NEWTABLE: {
			int b = GETARG_B(i);
			int c = GETARG_C(i);
			rsethvalue(rL, ra, rlua_h_new(rL, luaO_fb2int(b), luaO_fb2int(c)));
			continue;
		}
		case OP_SELF: {
			
			StkId rb = RB(i);
			setObject(ra + 1, rb);
			Protect(rluaV_gettable(rL, rb, RKC(i), ra));
			continue;
		}
		case OP_ADD: {
			arith_op(rL, luai_numadd, TM_ADD);
			continue;
		}
		case OP_SUB: {
			arith_op(rL, luai_numsub, TM_SUB);
			continue;
		}
		case OP_MUL: {
			arith_op(rL, luai_nummul, TM_MUL);
			continue;
		}
		case OP_DIV: {
			arith_op(rL, luai_numdiv, TM_DIV);
			continue;
		}
		case OP_MOD: {
			arith_op(rL, luai_nummod, TM_MOD);
			continue;
		}
		case OP_POW: {
			arith_op(rL, luai_numpow, TM_POW);
			continue;
		}
		case OP_UNM: {
			TValue* rb = RB(i);
			if (rttisnumber(rb)) {
				lua_Number nb = rnvalue(rb);
				rsetnvalue(ra, luai_numunm(nb));
			}
			else {
				Protect(rlua_arith(rL, ra, rb, rb, TM_UNM));
			}
			continue;
		}
		case OP_NOT: {
			int res = rl_isfalse(RB(i));  /* next assignment may change this value */
			rsetbvalue(ra, res);
			continue;
		}

		case OP_LEN: {
			const TValue* rb = RB(i);
			switch (ttype(rb)) {
			case LUA_TTABLE: {
				rsetnvalue(ra, cast_num(rluaH_getn(rhvalue(rb))));
				break;
			}
			case LUA_TSTRING: {
				rsetnvalue(ra, cast_num(tsvalue(rb)->len));
				break;
			}
			default: {  /* try metamethod */
				break;
			}
			}
			continue;
		}
		case OP_CONCAT: {
			int b = GETARG_B(i);
			int c = GETARG_C(i);
			Protect(rluaV_concat(rL, c - b + 1, c); luaC_checkGC(L));
			setObject(RA(i), base + b);
			continue;
		}
		case OP_JMP: {
			dojump(L, pc, GETARG_sBx(i));
			continue;
		}

		case OP_EQ: {
			TValue* rb = RKB(i);
			TValue* rc = RKC(i);
			Protect(
				if (equalobj(rL, rb, rc) == GETARG_A(i))
					dojump(L, pc, GETARG_sBx(*pc));
			)
				pc++;
			continue;
		}
		case OP_LT: {
			Protect(
				if (rluaV_lessthan(rL, RKB(i), RKC(i)) == GETARG_A(i))
					dojump(L, pc, GETARG_sBx(*pc));
			)
				pc++;
			continue;
		}
		case OP_LE: {

			L->savedpc = pc;
			if (RKB(i)->tt == RLUA_TNUMBER && RKC(i)->tt == RLUA_TNUMBER
				&& (Occult_luaX_ConvertNumber(RKB(i)->value.n) > Occult_luaX_ConvertNumber(RKC(i)->value.n))) {
				dojump(L, pc, GETARG_sBx(*pc));
			}
			pc++;
			continue;
		}
		case OP_TEST: {
			if (rl_isfalse(ra) != GETARG_C(i))
				dojump(L, pc, GETARG_sBx(*pc));
			pc++;
			continue;
		}
		case OP_TESTSET: {
			TValue* rb = RB(i);
			if (rl_isfalse(rb) != GETARG_C(i)) {
				setObject(ra, rb);
				dojump(L, pc, GETARG_sBx(*pc));
			}
			pc++;
			continue;
		}

		case OP_CALL: {
			int b = GETARG_B(i);
			int nresults = GETARG_C(i) - 1;
			if (b != 0) *(StkId*)(rL + 24) = ra + b;  /* else previous instruction set top */
			L->savedpc = pc;
			switch (rluaD_precall(rL, ra, nresults)) {
			case PCRLUA: {
				_rluaV_execute(rL, 1);
				continue;
			}
			case PCRC: {
				/* it was a C function (`precall' called it); adjust results */
				if (nresults >= 0) {

					*(StkId*)(rL + 24) = **(StkId * *)(rL + 32);//L->ci->top;
				}
				base = L->base;
				continue;
			}
			default: {
				return 1;  /* yield */
			}
			}
		}

		case OP_TAILCALL: {
			int b = GETARG_B(i);
			if (b != 0) *(StkId*)(rL + 24) = ra + b;  /* else previous instruction set top */
			L->savedpc = pc;
			int nresults = rluaD_precall(rL, ra, LUA_MULTRET);
			if (nresults == 0)
			{
				goto reentry;
			}

			else if (nresults == 1)
			{
				base = L->base;
				continue;
			}

			continue;
		}

		case OP_RETURN:
		{
			return 0;
		}

		case OP_FORLOOP: {
			lua_Number step = rnvalue(ra + 2);
			lua_Number idx = luai_numadd(nvalue(ra), step); /* increment index */
			lua_Number limit = rnvalue(ra + 1);
			if (luai_numlt(0, step) ? luai_numle(idx, limit)
				: luai_numle(limit, idx)) {
				dojump(L, pc, GETARG_sBx(i));  /* jump back */
				rsetnvalue(ra, idx);  /* update internal index... */
				rsetnvalue(ra + 3, idx);  /* ...and external index */
			}
			continue;
		}
		case OP_FORPREP: {
			const TValue* init = ra;
			const TValue* plimit = ra + 1;
			const TValue* pstep = ra + 2;
			L->savedpc = pc;  /* next steps may throw errors */
			if (!tonumber(init, ra))
				rluaG_runerr(rL, LUA_QL("for") " initial value must be a number");
			else if (!tonumber(plimit, ra + 1))
				rluaG_runerr(rL, LUA_QL("for") " limit must be a number");
			else if (!tonumber(pstep, ra + 2))
				rluaG_runerr(rL, LUA_QL("for") " step must be a number");
			rsetnvalue(ra, luai_numsub(rnvalue(ra), rnvalue(pstep)));
			dojump(L, pc, GETARG_sBx(i));
			continue;
		}
		case OP_TFORLOOP: {
			StkId cb = ra + 3;  /* call base */
			setObject(cb + 2, ra + 2);
			setObject(cb + 1, ra + 1);
			setObject(cb, ra);
			*(StkId*)(rL + 24) = cb + 3;  /* func. + 2 args (state and index) */
			Protect(rluaD_precall(rL, cb, GETARG_C(i)));
			*(StkId*)(rL + 24) = **(StkId**)(rL + 32);
			cb = RA(i) + 3;  /* previous call may change the stack */
			if (!ttisnil(cb)) {  /* continue loop? */
				setObject(cb - 1, cb);  /* save control variable */
				dojump(L, pc, GETARG_sBx(*pc));  /* jump back */
			}
			pc++;
			continue;
		}
		case OP_SETLIST: {
			int n = GETARG_B(i);
			int c = GETARG_C(i);
			int last;
			Table* h;
			if (n == 0) {
				n = cast_int(*(StkId*)(rL + 24) - ra) - 1;
				*(StkId*)(rL + 24) = **(StkId * *)(rL + 32);
			}
			if (c == 0) c = cast_int(*pc++);
		
			h = rhvalue(ra);
			last = ((c - 1) * LFIELDS_PER_FLUSH) + n;
			if (last > h->sizearray)  /* needs more space? */
				rluaH_resizeArray(rL, h, last);  /* pre-alloc it at once */
			for (; n > 0; n--) {
				TValue* val = ra + n;
				setObject(reinterpret_cast<TValue*>(rluaH_setnum(rL, h, last--)), val);
			}
			continue;
		}

		case OP_CLOSURE: {
			Proto* p = cl->p->p[GETARG_Bx(i)];

			Closure* _cl = luaF_newLclosure(L, p->nups, cl->env);
			_cl->l.p = p;
			for (auto j = 0; j < p->nups; j++, pc++) {
				if (GET_OPCODE(*pc) == OP_GETUPVAL)
					_cl->l.upvals[j] = cl->upvals[GETARG_B(*pc)];
				else {
					lua_assert(GET_OPCODE(*pc) == OP_MOVE);
					_cl->l.upvals[j] = luaF_findupval(L, base + GETARG_B(*pc));
				}
			}
			setclvalue(L, L->top, _cl);
			incr_top(L);
			setclvalue(L, L->top, _cl);
			rlua_pushlightuserdata(rL, L);
			rlua_pushlightuserdata(rL, _cl);
			TValue* cclosure = reinterpret_cast<TValue*>(rbx_pushcclosure(rL, int3, 2));
			ra->tt = RLUA_TFUNCTION;
			ra->value.gc = reinterpret_cast<GCObject*>(cclosure->value.gc);
			continue;
		}

		case OP_VARARG: {
			int b = GETARG_B(i) - 1;
			int j;
			CallInfo* ci = L->ci;
			int n = cast_int(ci->base - ci->func) - cl->p->numparams - 1;
			if (b == LUA_MULTRET) {
				Protect(rluaD_checkstack(rL, n));
				ra = RA(i);  /* previous call may change the stack */
				b = n;
				*(StkId*)(rL + 24) = ra + n;
			}
			for (j = 0; j < b; j++) {
				if (j < n) {
					setObject(ra + j, ci->base - n + j);
				}
				else {
					setnilvalue(ra + j);
				}
			}
			continue;
		}

		}

	}

}
#define r_setidentity(rL, i) DWORD* identity  = (DWORD*)(rL - 40); *identity ^= (i ^ (unsigned __int8)*identity) & 0x1F;



typedef void(__cdecl* T_executeInNewThread)(int self,std::string script, const char* name, int a);
T_executeInNewThread executeInNewThread = (T_executeInNewThread)rformat(0x696EA0);
VOID RunScript(DWORD rL, lua_State* L, std::string source)
{
	lua_State* tH = lua_newthread(L);
	std::string Script = "spawn(function() " + std::string(source) + "end)";
	luaL_loadstring(tH, Script.c_str());
	DWORD thread = rlua_newthread(rL);
	r_setidentity(thread, 6);
	TValue* clvalue = tH->top - 1;
	if (luaD_precall(tH, clvalue, 0) == 0)
	{
		rluaV_execute(thread, tH, 1);
	}
}

extern DWORD RL = 0;

DWORD WINAPI input(PVOID lvpParameter)
{
	using namespace std;
	string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\OmithusPipe"),
		PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
		PIPE_WAIT,
		1,
		999999,
		999999,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';
				try {
					try {
						WholeScript = WholeScript + buffer;
					}
					catch (...) {
					}
				}
				catch (std::exception e) {

				}
				catch (...) {

				}
			}
			lua_State* L = luaL_newstate();
			RunScript(RL, L, WholeScript.c_str());

			WholeScript = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}

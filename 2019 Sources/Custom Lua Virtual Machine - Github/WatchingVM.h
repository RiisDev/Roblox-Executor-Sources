#pragma once
#include "RbxLua.h"

// We'll be needing to convert Constants (aka TValues)

// we'll need to look on how some VM's done tstring converting!

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
		rbx->value.gc = reinterpret_cast<GCObject*>(RLua::luaS_newlstr(rL, str, ts->tsv.len));
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

void luaV_execute(lua_State* L, DWORD rL, int nexeccalls) {
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


	// call realloc //

	TValue* realloc = reinterpret_cast<TValue*>(RLua::luaM_realloc_(rL, 0, 0, sizeof(TValue) * cl->p->sizek, 3));
	// convert constants (tvalues)
	
	/* main loop of interpreter */
	for (;;) {
		const Instruction i = *pc++;
		StkId ra = RA(i);

		for (auto x = 0; x < cl->p->sizek; x++)
		{
			PushTValues(rL, L, &k[x], (&realloc[x]));
		}

		int opcode = GET_OPCODE(i);
		switch (opcode)
		{
		case OP_MOVE: {
			/* now for op_move  */

			TValue* rb = reinterpret_cast<TValue*>(&realloc[GETARG_B(i)]);

			memcpy(reinterpret_cast<void*>(ra), reinterpret_cast<rsize_t*>(rb), sizeof(TValue));
			continue;
		}

		case OP_LOADK: {
			/*  same thing as op_move,, lol  */
			TValue* rb = reinterpret_cast<TValue*>(&realloc[GETARG_Bx(i)]);
			memcpy(reinterpret_cast<void*>(ra), reinterpret_cast<const void*>(rb), sizeof(TValue));
			continue;
		}

		case OP_GETGLOBAL: {
			/*  simple as fuck!  */
			TValue* rb = reinterpret_cast<TValue*>(&realloc[GETARG_Bx(i)]);
			Protect(RLua::luaV_gettable(rL, RLua::Index2Adr(rL, LUA_GLOBALSINDEX), rb, ra, 1));
			continue;
		}

		case OP_GETTABLE: {
			Protect(RLua::luaV_gettable(rL, RB(i), RKC(i), ra, 1));
			continue;
		}

		case OP_CALL: {
			int b = GETARG_B(i);
			int nresults = GETARG_C(i) - 1;
			if (b != 0) *(StkId*)(rL + 20) = ra + b;  /* else previous instruction set top */
			L->savedpc = pc;
			switch (RLua::luaD_precall(rL, ra, nresults)) {
			case PCRLUA: {
				nexeccalls++;
				goto reentry;  /* restart luaV_execute over new Lua function */
			}
			case PCRC: {
				/* it was a C function (`precall' called it); adjust results */
				if (nresults >= 0) *(DWORD*)(rL + 20) = *(DWORD*)(*(DWORD*)(rL + 16) + 4);
				base = L->base;
				continue;
			}
			default: {
				return;  /* yield */
			}
			}
			continue;

		}

		case OP_RETURN:
		{
			printf("op_return lol\n");
			return;

		}

		}

	}
}

void RunScript(DWORD rL, lua_State* L, const char* src)
{
	if (luaL_loadbuffer(L, src, strlen(src), src))
	{
		std::cout << "err: " << lua_tolstring(L, -1, 0) << std::endl;
		lua_pop(L, 1);
	}

	DWORD rLL = RLua::lua_newthread(rL);

	if (luaD_precall(L, L->top - 1, 0) == 0)
	{
		luaV_execute(L, rLL, 1);
	}

}
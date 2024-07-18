#include "gui.h"
#include "VMProtectSDK.h"
#include "memscan.h"
#include <Shlwapi.h>
static bool scan_finished = false;

#define R_PROTO_OBFUSCATE(p, v) *(int*)(int)(p) = (int)(v) - (int)(p)
#define RL_CLOSURE_OBFUSCATE(p, v) *(int*)(int)(p) = (int)(v) - (int)(p)



namespace lua_to_roblox
{
	INT convert_proto(int rl, Proto* p, int* rpnups) { // same order as readProto
										
														
														
		DWORD rp = rluaF_newproto(rl);
														
														/* main conversion of vanilla proto to roblox proto */

		R_PROTO_OBFUSCATE(rp + 16, RLUAS_NEW(rl, getstr(p->source)));

		*(DWORD*)(rp + 48) = p->sizep;
		auto* rpp_mem = (DWORD*)RLUAM_MALLOC(rl, sizeof(DWORD*) * p->sizep);
		R_PROTO_OBFUSCATE(rp + 20, rpp_mem);

		for (int k = 0; k < p->sizep; k++) {
			rpp_mem[k] = rluaF_newproto(rl);
			convert_proto(rl, p->p[k], rpnups);
		}

		*(DWORD*)(rp + 28) = p->sizek;
		auto* rp_k_mem = (r_tvalue*)RLUAM_MALLOC(rl, sizeof(r_tvalue) * p->sizek);
		R_PROTO_OBFUSCATE(rp + 12, rp_k_mem);

		for (int k = 0; k < p->sizek; k++) {
			TValue* o = &p->k[k];
			r_tvalue* ro = &rp_k_mem[k];
			switch (o->tt) {
			case LUA_TNIL:
				ro->tt = RLUA_TNIL;
				ro->value.n = 0;
				break;
			case LUA_TBOOLEAN:
				ro->tt = RLUA_TBOOLEAN;
				ro->value.b = o->value.b;
				break;
			case LUA_TNUMBER:
				ro->tt = RLUA_TNUMBER;
				ro->value.n = r_xor_number(&o->value.n);
				break;
			case LUA_TSTRING:
				ro->tt = RLUA_TSTRING;
				ro->value.gc = RLUAS_NEW(rl, getstr((TString*)o->value.gc));
				break;
			default:
				break;
			}
		}

		*(DWORD*)(rp + 72) = p->sizecode;
		auto* rp_code_mem = (int*)RLUAM_MALLOC(rl, sizeof(int*) * p->sizecode);
		R_PROTO_OBFUSCATE(rp + 32, rp_code_mem);

		for (int k = 0; k < p->sizecode; k++) {
			/* instruction conversion */

			Instruction inst = p->code[k]; //vanilla instruction
			int r_inst = 0;
			OpCode op = GET_OPCODE(inst);

			RSET_OPCODE(r_inst, get_roblox_opcode[op]);

			switch (getOpMode(op)) {
			case iABC:
				RSETARG_A(r_inst, GETARG_A(inst));
				RSETARG_B(r_inst, GETARG_B(inst));
				RSETARG_C(r_inst, GETARG_C(inst));
				break;
			case iABx:
				RSETARG_A(r_inst, GETARG_A(inst));
				RSETARG_Bx(r_inst, GETARG_Bx(inst));
				break;
			case iAsBx:
				RSETARG_A(r_inst, GETARG_A(inst));
				RSETARG_sBx(r_inst, GETARG_sBx(inst));
				break;
			}

			/* enc has been added this time! includes eternals really shit wrong encryption he gave everyone as an attempt to seem smarter in the eyes of others */
			switch (op) {
			case OP_JMP:
				r_inst = r_inst >> 26 << 26 | jumpEnc(r_inst, k);
				break;
			case OP_CALL:
				r_inst = r_inst >> 26 << 26 | call_enc(r_inst, k);
				break;
			case OP_TAILCALL:
			case OP_RETURN:
				r_inst = r_inst >> 26 << 26 | dax_encode_op(r_inst, k, 1470882913, k - 1577854801, 641680189) & 0x3FFFFFF;
				break;
			case OP_CLOSURE:
				r_inst = r_inst >> 26 << 26 | closure_enc(r_inst, k);
				break;
			case OP_SETUPVAL:
				r_inst = r_inst >> 26 << 26 | setupval_enc(r_inst, k);
			case OP_MOVE:
				r_inst = r_inst & 0xFFFC21FF | 0x2000;
				break;
			}

			rp_code_mem[k] = r_inst * encode_key;
		}
	
		// sizelocvars set
		*(DWORD*)(rp + 64) = p->sizelineinfo;
		auto* rp_lineinfo_mem = (int*)RLUAM_MALLOC(rl, sizeof(int*) * p->sizelineinfo);
		R_PROTO_OBFUSCATE(rp + 8, rp_lineinfo_mem);

		for (int k = 0; k < p->sizelineinfo; k++) {
			rp_lineinfo_mem[k] = p->lineinfo[k] ^ (k << 8);
		}

		// sizeupvalues set
		*(BYTE *)(rp + 79) = p->maxstacksize;
		*(BYTE *)(rp + 78) = p->is_vararg;
		*rpnups += p->nups;  *(BYTE *)(rp + 76) = p->nups;
		*(BYTE *)(rp + 77) = p->numparams;

		return rp;
	}

	VOID set_l_closure(int rl, LClosure* lcl) {
		Proto* p = lcl->p;
		//DWORD rp = rluaF_newproto(rl);
		int nelems = 0;

		int rp = convert_proto(rl, p, &nelems);
		DWORD rlcl = rluaF_newLclosure(rl, nelems, *(int*)(rl + RL_L_GT));
		*(DWORD*)(rlcl + 16) = rlcl + rp + 16;

		if (nelems)
		{
			int v304 = rlcl + 4 * nelems + 20;
			do
			{
				*(int*)(v304 - 4) = 0;
				v304 -= 4;
				--nelems;
			} while (nelems);
		}

		rlua_pushlclosure(rl, rlcl);
	}

	VOID call_l_closure(int rl, LClosure* lcl) {
		set_l_closure(rl, lcl);
		r_spawn(rl);
	}

	VOID execute_script(int rl, lua_State* l, std::string source) {
		int r_thread = r_lua_newthread(rl);
		set_encode_key(r_thread, &encode_key);
		

		int unk[] = { NULL, NULL };
		r_sandbox_thread(r_thread, 6, (int)unk);

		if (luaL_loadstring(l, source.c_str())) {
			printf("Error: %s\n", lua_tostring(l, -1));
			lua_pop(l, 1);
		}

		else {
			util::pause();
			TValue* o = (l->top) - 1;
			LClosure* lcl = &clvalue(o)->l;
			call_l_closure(r_thread, lcl);
			//R_LUA_POP(rl, 1);
			lua_pop(l, 1);
			util::resume();
		}

		printf("\nlstack: %d\n", lua_gettop(l));
		printf("rstack: %d\n", RLUA_GETTOP(rl));
	}
}

void execute_script(int rL, lua_State* L, const std::string &source) {
	lua_to_roblox::execute_script(rL, L, source);
}
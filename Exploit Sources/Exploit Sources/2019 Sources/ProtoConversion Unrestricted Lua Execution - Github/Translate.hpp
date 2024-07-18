#pragma once
#include "encrypt.h"
#include "StaticDefs.h"
#include "utilites.h"

extern "C" {
#include "Lua/lua.h"
#include "Lua/lauxlib.h"
#include "Lua/lualib.h"
#include "Lua/lopcodes.h"
#include "Lua/lobject.h"
#include "Lua/lstate.h"
}

DWORD xorCosnt = ASLR(0x1BE7EE0);

DWORD encode_key;

#define r_ptr_obfus(p, v) *(int*)(p) = *(int*)(p) ^ (int)(v)



#define RSET_OPCODE(i,o)	((i) = (((i) & MASK0(6, 26)) | (((Instruction)o << 26) & MASK1(6, 26))))
#define RGET_OPCODE(i)		(i >> 26 & MASK1(6, 0))
#define RSETARG_A(i,o)		((i) = (((i) & MASK0(8, 18)) | (((Instruction)o << 18) & MASK1(8, 18))))
#define RGETARG_A(i)		(i >> 18 & MASK1(8, 0))
#define RSETARG_B(i,o)		((i) = (((i) & MASK0(9, 0)) | (((Instruction)o << 0) & MASK1(9, 0))))
#define RGETARG_B(i)		(i >>  0 & MASK1(9, 0))
#define RSETARG_C(i,o)		((i) = (((i) & MASK0(9, 9)) | (((Instruction)o << 9) & MASK1(9, 9))))
#define RGETARG_C(i)		(i >>  9 & MASK1(9, 0))
#define RSETARG_Bx(i,b)		((i) = (((i) & MASK0(18, 0)) | (((Instruction)b << 0) & MASK1(18, 0))))
#define RGETARG_Bx(i)		(i >>  0 & MASK1(18, 0))
#define RSETARG_sBx(i,b)	RSETARG_Bx((i),cast(unsigned int, (b)+MAXARG_sBx))
#define RGETARG_sBx(i)		(RGETARG_Bx(i)-MAXARG_sBx)

const char get_roblox_opcode[NUM_OPCODES]{ // modified instruction set enum
	6, // OP_MOVE
	4, // OP_LOADK
	0, // OP_LOADBOOL
	7, // OP_LOADNIL
	2, // OP_GETUPVAL
	8, // OP_GETGLOBAL
	1, // OP_GETTABLE
	3, // OP_SETGLOBAL
	5, // OP_SETUPVAL
	15, // OP_SETTABLE
	13, // OP_NEWTABLE
	9, // OP_SELF
	16, // OP_ADD
	11, // OP_SUB
	17, // OP_MUL
	10, // OP_DIV
	11, // OP_MOD
	14, // OP_POW
	24, // OP_UNM
	22, // OP_NOT
	18, // OP_LEN
	25, // OP_CONCAT
	20, // OP_JMP
	26, // OP_EQ
	19, // OP_LT
	21, // OP_LE
	23, // OP_TEST
	33, // OP_TESTSET
	31, // OP_CALL
	27, // OP_TAILCALL
	34, // OP_RETURN
	29, // OP_FORLOOP
	35, // OP_FORPREP
	28, // OP_TFORLOOP
	30, // OP_SETLIST
	32, // OP_CLOSE
	37, // OP_CLOSURE
	36, // OP_VARARG
};


#define RLUAS_NEW(rl, str) rluaS_newlstr(rl, str, strlen(str))

int proto_hash(int a1)
{
	int key = *(DWORD *)(((a1 + 20) ^ *(DWORD *)(a1 + 20)) + 0x1C) - (((a1 + 20) ^ *(DWORD *)(a1 + 20)) + 28);
	int v12 = 476035360 << 19 >> 20 - 706264423 * 24 >> 32 - 2088292391 >> 20 << 13 + 1546808719 << 16;

	if (key > 20)
	{
		int v12 = -1395309893 << 19 >> 20 - 706264423 * (24 >> 32 - 2088292391 >> 20 << 13 + 1546808719 << 16);
	}

	return (*(DWORD *)(a1 + 68) * (v12 | 1) ^ _rotr(*(DWORD *)(a1 + 68) * (v12 | 1), 7) ^ _rotl(*(DWORD *)(a1 + 68) * (v12 | 1), 11));

}

double r_xor_number(double* number) {
	int rxor = xorCosnt;
	PVOID old_number = number;

	double new_number;

	__asm {
		mov edx, old_number
		mov eax, rxor
		movsd xmm1, qword ptr[edx]
		xorpd xmm1, [eax]
		movsd new_number, xmm1
	}

	return new_number;
}

void convert_cosntants(intptr_t rL, Proto* p, DWORD rp)
{
	TValue* rk = (TValue*)rluaM_realloc_(rL, 0, 0, sizeof(TValue) * p->sizek, 3);
	r_ptr_obfus(rp + 8, rk);
	for (int i = 0; i < p->sizek; i++)
	{
		TValue* o = &p->k[i];
		TValue* ro = &rk[i];
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
			ro->value.gc = (GCObject*)RLUAS_NEW(rL, getstr((TString*)o->value.gc));
			break;
		default:
			break;
		}
	}
}

void convert_lineinfo(intptr_t rL, Proto* p, DWORD rp)
{
	int* lineInfo = (int*)rluaM_realloc_(rL, 0, 0, sizeof(intptr_t) * p->sizelineinfo, 3);
	r_ptr_obfus(rp + 16, lineInfo);
	for (int i = 0; i < p->sizelineinfo; i++)
	{
		lineInfo[i] = p->lineinfo[i] ^ (i << 8);
	}

}


void convert_code(uintptr_t rL, Proto* p, DWORD rp)
{
	int* code = (int*)rluaM_realloc_(rL, 0, 0, sizeof(int) * p->sizecode, 3);
	r_ptr_obfus(rp + 36, code);

	for (int i = 0; i < p->sizecode; i++)
	{
		Instruction inst = p->code[i];

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

		default:
			break;
		}

		switch (op) {
		case OP_JMP:
			r_inst = r_inst >> 26 << 26 | jumpEnc(r_inst, i);
			break;
		case OP_CALL:
			printf("OP_CALL\n");
			r_inst = r_inst >> 26 << 26 | call_enc(r_inst, i);
			break;
		case OP_TAILCALL:
		case OP_RETURN:
			r_inst = r_inst >> 26 << 26 | rbxDaxEncodeOp(r_inst, i, 1470882913, i - 1577854801, 641680189) & 0x3FFFFFF;
			break;
		case OP_CLOSURE:
			r_inst = r_inst >> 26 << 26 | closure_enc(r_inst, i);
			break;
		case OP_SETUPVAL:
			r_inst = r_inst >> 26 << 26 | sub_6EC3F10(r_inst, i);
		case OP_MOVE:
			r_inst = r_inst & 0xFFFC21FF | 0x2000;
			break;
		}





		code[i] = r_inst * encode_key;


	}
}


/*
rp + 8 (rp->k)
rp + 40 (rp->sizek)
rp + 32 (rp->p)
rp + 56 (rp->sizep)
rp + 36 (rp->code)
rp + 48 (rp->sizecode)
rp + 12 (rp->locvars)
rp + 64 (rp->sizelocvars)
rp + 76 (rp->nups)
rp + 24 (rp->upvalues)
rp + 28 (rp->sizeupvalues)
rp + 60 (rp->linedefined)
rp + 16 (rp->lineinfo)
rp + 44 (rp->sizelineinfo)
rp + 72 (rp->lastlinedefined)
rp + 20 (rp->source)
rp + 78 (rp->maxstacksize)
rp + 77 (rp->is_vararg)
rp + 79 (rp->numparams)
rp + 68 (rp->hash)
*/



int TranslateProto(intptr_t rL, Proto* p)
{
	DWORD r_proto = rluaF_newproto(rL);
	*(BYTE*)(r_proto + 79) = p->numparams;
	*(BYTE*)(r_proto + 78) = p->maxstacksize;
	*(DWORD*)(r_proto + 44) = p->sizelineinfo;
	*(DWORD*)(r_proto + 48) = p->sizecode;
	*(DWORD*)(r_proto + 56) = p->sizep;
	*(DWORD*)(r_proto + 40) = p->sizek;
	printf("Roblox Proto: %X\n", r_proto);

	r_ptr_obfus(r_proto + 20, RLUAS_NEW(rL, getstr(p->source)));


	convert_code(rL, p, r_proto);
	convert_cosntants(rL, p, r_proto);
	convert_lineinfo(rL, p, r_proto);


	if (p->sizep)
	{
		DWORD* _p = (DWORD*)rluaM_realloc_(rL, 0, 0, 4 * p->sizep, 3);
		r_ptr_obfus(r_proto + 32, _p);
		for (int i = 0; i < p->sizep; i++)
		{
			_p[i] = TranslateProto(rL, p->p[i]);
		}
	}


	return r_proto;
}

#define RLPUSH(RL,v,t) { int v2 = *(DWORD*)(RL + 24); \
					*(DWORD*)(v2 + 8) = t; \
					*(DWORD*)v2 = v; \
					*(DWORD*)(RL + 24) += 16; }

int rluaF_newLclosure(unsigned int rL, signed int nelems, int e)
{
	int c = (int)rluaM_realloc_(rL, 0, 0, 4 * nelems + 20, 4);
	rluaC_link(rL, c, RLUA_TFUNCTION);
	*(int*)(c + 12) = e;
	*(byte*)(c + 7) = nelems;
	return c;
}

int ProtoCallClosure(int rL, LClosure* lcl)
{
	DWORD rp = TranslateProto(rL, lcl->p);

	DWORD closure = rluaf_newLClosure(rL, 0, *(int*)(rL + 64));
	*(DWORD*)(closure + 16) = closure + 16 - rp;
	RLPUSH(rL, closure, RLUA_TFUNCTION);
	return closure;
}

void run(lua_State* L, DWORD rL, const char* source)
{
	if (luaL_loadstring(L, source))
	{
		lua_pop(L, 1);
	}
	else
	{

		intptr_t thread = rlua_newthread(rL);
		set_encode_key(thread, &encode_key);
		printf("Passed encode!\n");

		TValue* Func = L->top - 1;
		LClosure* lcl = (LClosure*)(Func->value.p);
		ProtoCallClosure(thread, lcl);

		spawn(thread);

	}
}
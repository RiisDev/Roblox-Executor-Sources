#pragma comment(lib, "Lua/lua5.1.lib")

#include "windows.h"
#include <string>
#include <vector>
#include <sstream>
#include <istream>
#include <numeric>
#include <iterator> 
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <TlHelp32.h>
#include <fstream>
#include <iomanip>
#include <direct.h>
#include <thread>
#include <map>

extern "C"
{
#include <Lua\lfunc.h>
#include <Lua\lua.h>
#include <Lua\lualib.h>
#include <Lua\lauxlib.h>
#include <Lua\lvm.h>
#include <Lua\ldo.h>
#include <Lua\lstate.h>
#include <Lua\lstring.h>
#include <Lua\lopcodes.h>
#include <Lua\llimits.h>
#include <Lua\lobject.h>
}

uintptr_t m_rL = 0;
lua_State* m_L = 0;

std::string random_string(size_t len);

#define Log(...) printf("[%s] -> ", __FUNCTION__); printf(__VA_ARGS__)
//#define Log(...) __noop

#define R_POS_OP 26
#define R_POS_A 18
#define R_POS_B 0
#define R_POS_C 9
#define R_POS_Bx R_POS_B
#define R_SET_OPCODE(i,o) ((i) = (((i)&MASK0(SIZE_OP,R_POS_OP)) | ((cast(Instruction, o)<<R_POS_OP)&MASK1(SIZE_OP,R_POS_OP))))
#define R_SETARG_A(i,u) ((i) = (((i)&MASK0(SIZE_A,R_POS_A)) | ((cast(Instruction, u)<<R_POS_A)&MASK1(SIZE_A,R_POS_A))))
#define R_SETARG_B(i,b) ((i) = (((i)&MASK0(SIZE_B,R_POS_B)) | ((cast(Instruction, b)<<R_POS_B)&MASK1(SIZE_B,R_POS_B))))
#define R_SETARG_C(i,b) ((i) = (((i)&MASK0(SIZE_C,R_POS_C)) | ((cast(Instruction, b)<<R_POS_C)&MASK1(SIZE_C,R_POS_C))))
#define R_SETARG_Bx(i,b) ((i) = (((i)&MASK0(SIZE_Bx,R_POS_Bx)) | ((cast(Instruction, b)<<R_POS_Bx)&MASK1(SIZE_Bx,R_POS_Bx))))
#define R_SETARG_sBx(i,b) R_SETARG_Bx((i),cast(Instruction, (b)+MAXARG_sBx))
#define RSET_OPCODE(i,o) ((i) = (((i) & MASK0(6, 26)) | (((Instruction)o << 26) & MASK1(6, 26))))
#define RGET_OPCODE(i) (i >> 26 & MASK1(6, 0))
#define RSETARG_A(i,o) ((i) = (((i) & MASK0(8, 18)) | (((Instruction)o << 18) & MASK1(8, 18))))
#define RGETARG_A(i) (i >> 18 & MASK1(8, 0))
#define RSETARG_B(i,o) ((i) = (((i) & MASK0(9, 0)) | (((Instruction)o << 0) & MASK1(9, 0))))
#define RGETARG_B(i) (i >> 0 & MASK1(9, 0))
#define RSETARG_C(i,o) ((i) = (((i) & MASK0(9, 9)) | (((Instruction)o << 9) & MASK1(9, 9))))
#define RGETARG_C(i) (i >> 9 & MASK1(9, 0))
#define RSETARG_Bx(i,b) ((i) = (((i) & MASK0(18, 0)) | (((Instruction)b << 0) & MASK1(18, 0))))
#define RGETARG_Bx(i) (i >> 0 & MASK1(18, 0))
#define RSETARG_sBx(i,b) RSETARG_Bx((i),cast(unsigned int, (b)+MAXARG_sBx))
#define RGETARG_sBx(i) (RGETARG_Bx(i)-MAXARG_sBx)

typedef enum {
	R_OP_MOVE = 6,
	R_OP_LOADK = 4,
	R_OP_LOADBOOL = 0,
	R_OP_LOADNIL = 7,
	R_OP_GETUPVAL = 2,
	R_OP_GETGLOBAL = 8,
	R_OP_GETTABLE = 1,
	R_OP_SETGLOBAL = 3,
	R_OP_SETUPVAL = 5,
	R_OP_SETTABLE = 15,
	R_OP_NEWTABLE = 13,
	R_OP_SELF = 9,
	R_OP_ADD = 16,
	R_OP_SUB = 11,
	R_OP_MUL = 17,
	R_OP_DIV = 10,
	R_OP_MOD = 11,
	R_OP_POW = 14,
	R_OP_UNM = 24,
	R_OP_NOT = 22,
	R_OP_LEN = 18,
	R_OP_CONCAT = 25,
	R_OP_JMP = 20,
	R_OP_EQ = 26,
	R_OP_LT = 19,
	R_OP_LE = 21,
	R_OP_TEST = 23,
	R_OP_TESTSET = 33,
	R_OP_CALL = 31,
	R_OP_TAILCALL = 27,
	R_OP_RETURN = 34,
	R_OP_FORLOOP = 29,
	R_OP_FORLOR_OP = 29,
	R_OP_FORPREP = 35,
	R_OP_TFORLOOP = 28,
	R_OP_SETLIST = 30,
	R_OP_CLOSE = 32,
	R_OP_CLOSURE = 37,
	R_OP_VARARG = 36,
} r_OpCode;
const r_OpCode rbx_opcodes[NUM_OPCODES] = {
	R_OP_MOVE,
	R_OP_LOADK,
	R_OP_LOADBOOL,
	R_OP_LOADNIL,
	R_OP_GETUPVAL,
	R_OP_GETGLOBAL,
	R_OP_GETTABLE,
	R_OP_SETGLOBAL,
	R_OP_SETUPVAL,
	R_OP_SETTABLE,
	R_OP_NEWTABLE,
	R_OP_SELF,
	R_OP_ADD,
	R_OP_SUB,
	R_OP_MUL,
	R_OP_DIV,
	R_OP_MOD,
	R_OP_POW,
	R_OP_UNM,
	R_OP_NOT,
	R_OP_LEN,
	R_OP_CONCAT,
	R_OP_JMP,
	R_OP_EQ,
	R_OP_LT,
	R_OP_LE,
	R_OP_TEST,
	R_OP_TESTSET,
	R_OP_CALL,
	R_OP_TAILCALL,
	R_OP_RETURN,
	R_OP_FORLOOP,
	R_OP_FORPREP,
	R_OP_TFORLOOP,
	R_OP_SETLIST,
	R_OP_CLOSE,
	R_OP_CLOSURE,
	R_OP_VARARG
};

union r_Value {
	void* gc;
	void* p;
	double n;
	int b;
};
struct r_TValue {
	r_Value value;
	int tt;
};
struct r_Proto {
	int* next; // +00
	lu_byte marked; // +04
	lu_byte tt; // +05
	r_TValue* k; // +08
	int* lineinfo; // +12
	int* locvars; // +16
	int* source; // +20
	int* upvalues; // +24
	int sizeupvalues; // +28
	r_Proto* p; // +32
	Instruction* code; // +36
	int sizek; // +40
	int sizecode; // +44
	int sizelocvars; // +48
	int linedefined; // +52
	int sizep; // +56
	int sizelineinfo; // +64
	int hash; // +68
	int lastlinedefined; // +72
	lu_byte nups; // +76
	lu_byte numparams; // +77
	lu_byte maxstacksize; // +78
	lu_byte is_vararg; // +79
};
struct r_LClosure {
	int* next; // +00
	lu_byte marked; // +05
	lu_byte tt; // +06
	lu_byte nupvalues; // +07
	unsigned char unk[4]; // +11
	int env; // +12
	r_Proto* p; // +16
	int upvals[1]; // +20
};

void rluaC_link(uintptr_t rL, int o, lu_byte tt)
{
	std::uintptr_t g = *(std::uint32_t*)(rL + 20) - (rL + 20); // thread->l_G
	lu_byte currentwhite = *(lu_byte*)(g + 20); // g->currentwhite
	*(std::uint32_t*)o = *(std::uint32_t*)(g + 36); // o->gch.next
	*(std::uint32_t*)(g + 36) = o; // g->rootgc
	*(lu_byte*)(o + 4) = currentwhite & 3; // o->gch.marked
	*(lu_byte*)(o + 5) = tt; // o->gch.tt
}


//Proto Struct: FF 75 10 E8 ? ? ? ? 8D 4E 08
int rluaF_newproto(uintptr_t rL)
{
	int f = (int)rluaM_realloc_(rL, 0, 0, 80, 6);
	rluaC_link(rL, f, RLUA_TPROTO);
	*(int *)(f + 32) = f + 32;                 // p->k
	*(int *)(f + 36) = 0;                      // p->sizek
	*(int *)(f + 28) = f + 28;                 // p->p
	*(int *)(f + 56) = 0;                      // p->sizep
	*(int *)(f + 24) = f + 24;                 // p->code
	*(int *)(f + 48) = 0;                      // p->sizecode
	*(int *)(f + 64) = 0;                      // p->sizelineinfo
	*(int *)(f + 40) = 0;                      // p->sizeupvals
	*(int *)(f + 16) = f + 16;                 // p->upvalues
	*(int *)(f + 78) = 0;                       // p->numparams
	*(int *)(f + 8) = f + 8;                   // p->lineinfo
	*(int *)(f + 60) = 0;                      // p->sizelocvars
	*(int *)(f + 20) = f + 20;                 // p->locvars
	*(int *)(f + 44) = 0;                      // p->linedefined
	*(int *)(f + 72) = 0;                      // p->lastlinedefined
	*(int *)(f + 78) = 0;                      // p->maxstacksize
	*(int *)(f + 68) = 0;                     // p->hash
	*(int *)(f + 12) = 0;                       // p->source
	*(lu_byte*)(f + 78) = 0; // maxstacksize
	*(lu_byte*)(f + 76) = 0; // is_vararg
	*(lu_byte*)(f + 77) = 0; // nups
	*(lu_byte*)(f + 79) = 0; // numparams
	return f;
}


__int64 inverse(__int64 a, __int64 m)
{
	__int64 m0 = m, t, q;
	__int64 x0 = 0, x1 = 1;
	while (a > 1)
	{
		q = a / m;
		t = m;
		m = a % m, a = t;
		t = x0;
		x0 = x1 - q * x0;
		x1 = t;
	}

	if (x1 < 0)
		x1 += m0;
	return x1;
}

int rluaF_newLclosure(unsigned int rL, signed int nelems, int e)
{
	int c = (int)rluaM_realloc_(rL, 0, 0, 4 * nelems + 20, 4);
	rluaC_link(rL, c, RLUA_TFUNCTION);
	*(int*)(c + 12) = e;
	*(byte*)(c + 7) = nelems;
	if (nelems)
	{
		int v304 = c + 4 * nelems + 20;
		do
		{
			*(int*)(v304 - 4) = 0;
			v304 -= 4;
			--nelems;
		} while (nelems);
	}
	return c;
}


uint32_t dax_encode_op(uint32_t x, uint32_t mulEven, uint32_t addEven, uint32_t mulOdd, uint32_t addOdd) {
	uint32_t result = 0;
	uint32_t mask = 1;
	for (size_t i = 0; i < 8 * sizeof(uint32_t); ++i) {
		uint32_t bitDesired = mask & x;
		uint32_t bitOdd = mask & (result*mulOdd + addOdd);
		uint32_t bitEven = mask & (result*mulEven + addEven);
		if ((bitEven ^ bitOdd) != bitDesired) {
			result |= mask;
		}
		mask <<= 1;
	}
	return result;
}

int call_enc(int a1, int a2) {
	signed int v2;
	int v3;
	signed int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	int v10;

	v2 = 1;
	v10 = a2;
	v3 = 0;
	v4 = 0;
	v5 = a1;
	do {
		v6 = ((v2 & (v10 + 32757935 * v3)) != 0) ^ ((v2 & (a2 - 29799480 * v3 - 5512095)) != 0);
		v7 = -((v5 & v2) != 0);
		v2 = _rotl(v2, 1);
		v8 = (-v7 ^ v6) << v4++;
		v3 |= v8;
		a2 = v10;
	} while (v4 < 26);
	return v3 | v5 & 0xFC000000;
}

int closure_enc(int a1, int a2) {
	signed int v2; // esi@1
	int v3; // eax@1
	signed int v4; // edi@1
	int v5; // ebx@1
	int v6; // edx@2
	int v7; // ecx@2
	int v8; // edx@2
	int v10; // [sp+Ch] [bp-4h]@1

	v2 = 1;
	v10 = a2;
	v3 = 0;
	v4 = 0;
	v5 = a1;
	do {
		v6 = ((v2 & (v10 + 9837702 * v3)) != 0) ^ ((v2 & (32757935 * v3 - 29408451 + a2)) != 0);
		v7 = -((v5 & v2) != 0);
		v2 = _rotl(v2, 1);
		v8 = (-v7 ^ v6) << v4++;
		v3 |= v8;
		a2 = v10;
	} while (v4 < 26);
	return v5 ^ (v5 ^ v3) & 0x3FFFFFF;
}

#define r_ptr_obfus(p, v) *(int*)(p) = (int)(p) + (int)(v)

#define __ROL4__(x, y) _rotl(x, y)
#define __ROR4__(x, y) _rotr(x, y)


static double r_xor_number(double* Number) {
	PVOID OldNumber = Number;
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

void Convert(uintptr_t rL, TValue* from, r_TValue** to)
{
	switch (ttype(from))
	{
	case LUA_TNIL:
		(*to)->tt = RLUA_TNIL;
		(*to)->value.n = 0;
		//Log("nil\n");
		break;
	case LUA_TBOOLEAN:
		(*to)->tt = RLUA_TBOOLEAN;
		(*to)->value.b = from->value.b;
		//Log("%s\n", bvalue(from) ? "true" : "false");
		break;
	case LUA_TNUMBER:
		(*to)->tt = RLUA_TNUMBER;
		(*to)->value.n = r_xor_number(&from->value.n);
		//Log("%d\n", nvalue(from));
		break;
	case LUA_TSTRING:
		(*to)->tt = RLUA_TSTRING;
		(*to)->value.gc = rluaS_new(rL, getstr((TString*)from->value.gc));
		//Log("%s\n", svalue(from));
		break;
	default:
		break;
	}
}


static int lua_jmpbackhandler(int rL)
{
	auto call_back = (DWORD)r_lua_tonumber(rL, 1);
	return ((int(*)(int))call_back)(rL);
}

int sub_237D62F0(int a1, int a2, int a3, int a4)
{
	int result; // eax@1
	signed int v5; // ecx@1
	int v6; // edx@2
	signed int v7; // ecx@3
	int *v8; // ebx@3
	signed int v9; // edx@3
	int v10; // edi@5
	int v11; // esi@5
	int v12; // ebx@7
	int v13; // edx@10
	char v14; // cl@10
	int v15; // edi@10
	unsigned int v16; // eax@10
	signed int v17; // edi@11
	unsigned int v18; // edx@11
	signed int v19; // ebx@11
	int v20; // esi@13
	bool v21; // zf@14
	int v22[32]; // [sp+Ch] [bp-8Ch]@2
	int v23; // [sp+8Ch] [bp-Ch]@4
	int v24; // [sp+90h] [bp-8h]@6
	int *v25; // [sp+94h] [bp-4h]@3
	signed int v26; // [sp+A0h] [bp+8h]@3
	unsigned int v27; // [sp+A0h] [bp+8h]@11
	signed int v28; // [sp+A4h] [bp+Ch]@3

	result = a4 | (a1 ^ a2) & 0x3FFFFFF;
	v5 = 0;
	do
	{
		v6 = __ROL4__(a3, v5);
		v22[v5++] = v6;
	} while (v5 < 32);
	v7 = 0;
	v8 = v22;
	v9 = 1;
	v28 = 0;
	v26 = 1;
	v25 = v22;
	do
	{
		v23 = *v8;
		if (!(v23 & v9))
		{
			v10 = v7 + 1;
			v11 = __ROL4__(v9, 1);
			if (v7 + 1 < 32)
			{
				v24 = 1 << v7;
				while (1)
				{
					v12 = v22[v10];
					v7 = v28;
					if (v12 & v24)
						break;
					++v10;
					v11 = __ROL4__(v11, 1);
					if (v10 >= 32)
						goto LABEL_11;
				}
				v13 = result & ~(v9 | (1 << v10));
				*v25 = v12;
				v22[v10] = v23;
				v14 = v10 - v28;
				v15 = (result & v26) << (v10 - v28);
				v16 = (v11 & (unsigned int)result) >> v14;
				v7 = v28;
				result = v15 | v13 | v16;
			}
		}
	LABEL_11:
		v17 = 0;
		v18 = __ROL4__(1, v7);
		v27 = v18;
		v19 = 1;
		do
		{
			if (v17 != v7)
			{
				v20 = v22[v17];
				if (v20 & v18)
				{
					v22[v17] = v20 ^ *v25;
					v7 = v28;
					v21 = (result & (unsigned int)v19) >> v17 == (result & v18) >> v28;
					v18 = v27;
					if (v21)
						result &= ~(1 << v17);
					else
						result |= 1 << v17;
				}
			}
			++v17;
			v19 = __ROL4__(v19, 1);
		} while (v17 < 32);
		++v7;
		v8 = v25 + 1;
		v9 = __ROL4__(v18, 1);
		v28 = v7;
		++v25;
		v26 = v9;
	} while (v7 < 32);
	return result;
}

int jmp_enc(int a1, int a2)
{
	int v2; // ebx@1
	int v3; // esi@1
	int v4; // ecx@1
	int result; // eax@2
	int v6; // edi@2
	int v7; // edx@2
	int v8; // ecx@2
	int v9; // [sp+10h] [bp-10h]@1
	int v10; // [sp+18h] [bp-8h]@1
	int v11; // [sp+1Ch] [bp-4h]@1

	v2 = a1;
	v9 = a2;
	v3 = 0;
	v4 = -10065 * a2 & 0x3FFFF;
	v10 = -10065 * a2 & 0x3FFFF;
	v11 = -10065 * a2 & 0x3FFFF;
	while (1)
	{
		v2 ^= (v2 ^ (v3 << 18)) & 0x3FC0000;
		result = sub_237D62F0(v4, v2, 0x800041, 0x50000000);
		v6 = result & 0x3FFFFFF | 0x50000000;
		v7 = _rotl(v6, 9);
		v8 = _rotr(v6, 6);
		if ((v6 ^ v11 ^ v8 ^ v7) == v2 && (result & 0xFC000000) == 1342177280)
			break;
		if (++v3 > 255)
			break;
		v4 = v10;
	}
	return result;
}

unsigned int __cdecl setupval_enc(int a1, int a2)
{
	unsigned int v2; // ebx
	int v3; // eax
	unsigned int v4; // edx
	unsigned int v5; // ebx
	unsigned int v6; // eax
	int v7; // eax
	int *v8; // ebx
	unsigned int v9; // ecx
	signed int v10; // edx
	unsigned int v11; // edi
	int v12; // esi
	int v13; // ebx
	unsigned int v14; // esi
	int v15; // edx
	int v16; // eax
	unsigned int v17; // edi
	int v18; // edx
	signed int v19; // ebx
	int v20; // esi
	bool v21; // zf
	unsigned int result; // eax
	int v23; // esi
	int v24[32]; // [esp+0h] [ebp-A4h]
	unsigned int v25; // [esp+80h] [ebp-24h]
	unsigned int v26; // [esp+84h] [ebp-20h]
	int v27; // [esp+88h] [ebp-1Ch]
	int v28; // [esp+8Ch] [ebp-18h]
	unsigned int v29; // [esp+90h] [ebp-14h]
	int v30; // [esp+94h] [ebp-10h]
	int v31; // [esp+98h] [ebp-Ch]
	int *v32; // [esp+9Ch] [ebp-8h]
	unsigned int v33; // [esp+A0h] [ebp-4h]
	unsigned int v34; // [esp+ACh] [ebp+8h]

	v29 = 524417;
	v2 = a1 & 0xFFFC01FF;
	v3 = 0;
	v34 = a1 & 0xFFFC01FF;
	v4 = 9837702 * a2 & 0xFFFC01FF;
	v30 = 0;
	v26 = 9837702 * a2 & 0xFFFC01FF;
	while (1)
	{
		v25 = v2 | (v3 << 9);
		v5 = 0;
		v32 = (int *)((v25 ^ v4) & 0x3FFFFFF | 0x14000000);
		v6 = v29;
		do
		{
			v24[v5] = (v6 << (v5 & 0x1F)) | (v6 >> (-(v5 & 0x1F) & 0x1F));
			++v5;
		} while (v5 < 0x20);
		v7 = (int)v32;
		v8 = v24;
		v9 = 0;
		v32 = v24;
		v10 = 1;
		v33 = 0;
		v31 = 1;
		do
		{
			v27 = *v8;
			if (!(v27 & v10))
			{
				v11 = v9 + 1;
				v12 = __ROL4__(v10, 1);
				if (v9 + 1 < 0x20)
				{
					v28 = 1 << v9;
					while (1)
					{
						v13 = v24[v11];
						v9 = v33;
						if (v13 & v28)
							break;
						++v11;
						v12 = __ROL4__(v12, 1);
						if (v11 >= 0x20)
							goto LABEL_12;
					}
					v14 = v7 & v12;
					v15 = v7 & ~(v10 | (1 << v11));
					v16 = v31 & v7;
					*v32 = v13;
					v24[v11] = v27;
					v9 = v33;
					v7 = (v14 >> (v11 - v33)) | v15 | (v16 << (v11 - v33));
				}
			}
		LABEL_12:
			v17 = 0;
			v18 = __ROL4__(1, v9);
			v31 = __ROL4__(1, v9);
			v19 = 1;
			do
			{
				if (v17 != v9)
				{
					v20 = v24[v17];
					if (v20 & v18)
					{
						v24[v17] = v20 ^ *v32;
						v9 = v33;
						v21 = (v7 & (unsigned int)v19) >> v17 == (v7 & (unsigned int)v18) >> v33;
						v18 = v31;
						if (v21)
							v7 &= ~(1 << v17);
						else
							v7 |= 1 << v17;
					}
				}
				++v17;
				v19 = __ROL4__(v19, 1);
			} while (v17 < 0x20);
			++v9;
			v8 = v32 + 1;
			v10 = __ROL4__(v18, 1);
			v33 = v9;
			++v32;
			v31 = v10;
		} while (v9 < 0x20);
		result = v7 & 0x3FFFFFF | 0x14000000;
		v4 = v26;
		v23 = (result ^ v26 ^ ((result << 13) | ((unsigned __int64)result >> 19)) ^ ((result << 25) | (result >> 7))) & 0x3FFFFFF | 0x14000000;
		if (v25 == v23 && (v23 & 0xFC000000) == 335544320)
			return result;
		v3 = v30 + 1;
		v30 = v3;
		if (v3 > 511)
			return 0;
		v2 = v34;
	}
}

int Convert(uintptr_t rL, Proto* p)
{
	Log("Initializing...\n");

	int r_p = rluaF_newproto(rL);
	__int64 ckey = inverse(*(DWORD *)(rL + 20) - (rL + 20) + 28 - *(DWORD *)(*(DWORD *)(rL + 20) - (rL + 20) + 28), 4294967296);

	Log("r_p: %x\np: %x\nckey: %d\n", r_p, p, ckey);

	r_TValue* _k = (r_TValue*)rluaM_realloc_(rL, 0, 0, sizeof(r_TValue) * p->sizek, 3);
	int* _code = (int*)rluaM_realloc_(rL, 0, 0, sizeof(int) * p->sizecode, 3);
	int* _lineinfo = (int*)rluaM_realloc_(rL, 0, 0, sizeof(int) * p->sizelineinfo, 3);
	int* _p = (int*)rluaM_realloc_(rL, 0, 0, sizeof(int) * p->sizep, 3);
	int* _upvalues = (int*)rluaM_realloc_(rL, 0, 0, sizeof(int)*p->sizeupvalues, 3);

	// conversion of instructions

	for (int i = 0; i < p->sizecode; i++)
	{
		Instruction instr = (p->code[i]);
		int r_instr = 0;
		OpCode op = GET_OPCODE(instr);

		RSET_OPCODE(r_instr, rbx_opcodes[op]);

		switch (getOpMode(op))
		{
		case iABC:
			RSETARG_A(r_instr, GETARG_A(instr));
			RSETARG_B(r_instr, GETARG_B(instr));
			RSETARG_C(r_instr, GETARG_C(instr));
			break;
		case iABx:
			RSETARG_A(r_instr, GETARG_A(instr));
			RSETARG_Bx(r_instr, GETARG_Bx(instr));
			break;
		case iAsBx:
			RSETARG_A(r_instr, GETARG_A(instr));
			RSETARG_sBx(r_instr, GETARG_sBx(instr));
			break;
		}

		printf("%s\n", luaP_opnames[op]);

		switch (op)
		{
		case OP_CALL:
			r_instr = r_instr >> 26 << 26 | call_enc(r_instr, i);
			break;
		case OP_TAILCALL:
		case OP_RETURN:
			r_instr = r_instr >> 26 << 26 | dax_encode_op(r_instr, i, 1470882913, i - 1577854801, 641680189) & 0x3FFFFFF;
			break;
		case OP_CLOSURE:
			r_instr = r_instr >> 26 << 26 | closure_enc(r_instr, i);
			break;
		case OP_JMP:
			r_instr = jmp_enc(r_instr, i);
			break;
		case OP_SETUPVAL:
			printf("SETUPVAL\n");
			r_instr = setupval_enc(r_instr, i) & 0x3FFFFFF | 0x14000000;
			break;
		case OP_MOVE:
			r_instr = r_instr & 0xFFFC03FF | 0x200;
			break;
		}

		*(int*)&_code[i] = (r_instr * ckey);
	}

	for (int i = 0; i < p->sizek; i++)
	{
		r_TValue* r_obj = new r_TValue;
		Convert(rL, &p->k[i], &r_obj);
		*(r_TValue*)&_k[i] = *r_obj;
	}

	for (int i = 0; i < p->sizelineinfo; i++)
	{
		*(int*)&_lineinfo[i] = p->lineinfo[i] ^ (i << 8);
	}

	for (int i = 0; i < p->sizep; i++)
	{
		*(int*)&_p[i] = Convert(rL, p->p[i]);
	}

	*(int*)(r_p + 56) = p->sizep; // r_p->sizep
	*(int*)(r_p + 64) = p->sizelineinfo; // r_p->sizelineinfo
	*(int*)(r_p + 36) = p->sizek; // r_p->sizek
	*(int*)(r_p + 48) = p->sizecode; // r_p->sizecode

	*(byte*)(r_p + 78) = p->maxstacksize; // r_p->maxstacksize
	*(byte*)(r_p + 79) = p->numparams; // r_p->numparams
	*(byte*)(r_p + 76) = p->is_vararg; // r_p->is_vararg

	r_ptr_obfus(r_p + 12, rluaS_new(rL, getstr(p->source)));
	r_ptr_obfus(r_p + 32, _k); // r_p->k
	r_ptr_obfus(r_p + 24, _code); // r_p->code
	r_ptr_obfus(r_p + 28, _p); // r_p->p
	r_ptr_obfus(r_p + 16, _upvalues);
	r_ptr_obfus(r_p + 8, _lineinfo);

	//*(int*)(r_p + 56) = p->sizecode;
	
	return r_p;
}
void lua_pushlclosure(uintptr_t rL, r_LClosure* closure)
{
	r_TValue* top = *(r_TValue**)(rL + 32);
	top->value.gc = closure;
	top->tt = RLUA_TFUNCTION;
	*(uint32_t*)(rL + 32) += sizeof(r_TValue);
}
void call(uintptr_t rL, int p)
{
	r_LClosure* cl = (r_LClosure*)rluaF_newLclosure(rL, 0, *(int*)(rL + 64));
	*(int*)&cl->p = (int)(p)-((int)(cl)+16);

	lua_pushlclosure(rL, cl);
	

	//Log("R_LCLOSURE: %d\n", lcl);

	Log("Calling LClosure!\n");
	//rlua_pcall(rL, 0, 0, 0);
	Spawn(rL);

}

void run(lua_State* L, const char* src)
{
	Log("Source: %s\n", src);
	if (luaL_loadbuffer(L, src, strlen(src), "@Sirhurt") == 0)
	{
		const LClosure* lcl = static_cast<const LClosure*>(lua_topointer(L, -1));
		Log("Retrieved LClosure: %x\nContinuing to Convert...\n", lcl);
		uintptr_t new_thread = rlua_newthread(m_rL);
		sandboxThread(new_thread);

		call(new_thread, Convert(new_thread, lcl->p));
	}
	else {
		const char* errorMsg = lua_isstring(L, -1) ? lua_tostring(L, -1) : "An error occurred, no output from Lua.";
		Log(errorMsg);
	}
}
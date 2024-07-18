#include "LuauTranspiler.h"
#include "LuauSerializer.h"

// opcode offsets
static enum OpUCode : unsigned char
{
	OPU_LOADVARGS = 163,
	OPU_GETGLOBAL = 164, // 8
	OPU_NOP = 0,
	OPU_CALL = 159,
	OPU_LOADGLOBAL = 53, // 8g
	OPU_FORCERET = 2,
	OPU_TEST = 14,
	OPU_LOADNUM = 140,
	OPU_NEWVECTOR = 255, // 8
	OPU_CLOSURE = 217,
	OPU_MOVE = 82,
	OPU_SETLIST = 197, //8
	OPU_MOD = 33,
	OPU_CLOSE = 193,
	OPU_RETURN = 130,
	OPU_LOADARGS = 192,
	OPU_LOADK = 111,
	OPU_LOADBOOL = 169,
	OPU_LOADNIL = 198,
	OPU_GETUPVAL = 251,
	OPU_SETUPVAL = 222,
	OPU_ADD = 149,
	OPU_SETGLOBAL = 24, // 8
	OPU_GETFIELD = 77, // 8
	OPU_STACKMUL = 9,
	OPU_GETTABLE = 135,
	OPU_GETTABLESTACK = 19,
	OPU_OR = 144,
	OPU_SETFIELD = 48, // 8
	OPU_SETTABLE = 106,
	OPU_NEWTABLE = 226,
	OPU_POW = 4,
	OPU_SELF = 188, // 8
	OPU_STACKADD = 67,
	OPU_STACKSUB = 38,
	OPU_SUB = 120,
	OPU_MUL = 91,
	OPU_DIV = 62,
	OPU_STACKDIV = 236,
	OPU_STACKMOD = 207,
	OPU_STACKPOW = 178,
	OPU_UNM = 57,
	OPU_NOT = 86,
	OPU_LEN = 28,
	OPU_CONCAT = 115,
	OPU_JMP = 101,
	OPU_LONGJMP = 105,
	OPU_BACKJMP = 72,
	OPU_NOTEQ = 154,
	OPU_EQ = 241,
	OPU_LT = 183,
	OPU_IFLT = 96,
	OPU_LE = 212,
	OPU_IFLE = 125,
	OPU_TESTJMP = 43,
	OPU_AND = 173,
	OPU_VARARG = 221,
	OPU_FORPREP = 168,
	OPU_FORLOOP = 139,
	OPU_TFORLOOP = 110,
	OPU_MARKUPVAL = 18
};

// size and position of opcode arguments.
#define SIZEU_OP	8
#define SIZEU_A		8
#define SIZEU_B		8
#define SIZEU_C		8
#define SIZEU_Bx	(SIZEU_B + SIZEU_C)

#define POSU_OP		0
#define POSU_A		(POSU_OP + SIZEU_OP)
#define POSU_B		(POSU_A + SIZEU_A)
#define POSU_C		(POSU_B + SIZEU_B)
#define POSU_Bx		POSU_B

#define SIZEU_D		10
#define SIZEU_E		10
#define SIZEU_F		10
#define SIZEU_G		2
#define SIZEU_Ex	(SIZEU_E + SIZEU_F + SIZEU_G)

#define POSU_D		0
#define POSU_E		(POSU_D + SIZEU_D)
#define POSU_F		(POSU_E + SIZEU_E)
#define POSU_G		(POSU_F + SIZEU_F)

// limits for opcode arguments.
#if SIZEU_Bx < LUAI_BITSINT-1
#define MAXARGU_Bx        ((1<<SIZEU_Bx)-1)
#define MAXARGU_sBx        (MAXARGU_Bx>>1)         /* `sBx' is signed */
#else
#define MAXARGU_Bx        MAX_INT
#define MAXARGU_sBx        MAX_INT
#endif

#define MAXARGU_A        ((1<<SIZEU_A)-1)
#define MAXARGU_B        ((1<<SIZEU_B)-1)
#define MAXARGU_C        ((1<<SIZEU_C)-1)

// the following macros help to manipulate instructions
#define GETU_OPCODE(i)	(cast(OpCode, ((i)>>POSU_OP) & MASK1(SIZEU_OP,0)))
#define SETU_OPCODE(i,o)	((i) = (((i)&MASK0(SIZEU_OP,POSU_OP)) | \
		((cast(Instruction, o)<<POSU_OP)&MASK1(SIZEU_OP,POSU_OP))))

#define GETARGU_A(i)	(cast(int, ((i)>>POSU_A) & MASK1(SIZEU_A,0)))
#define SETARGU_A(i,u)	((i) = (((i)&MASK0(SIZEU_A,POSU_A)) | \
		((cast(Instruction, u)<<POSU_A)&MASK1(SIZEU_A,POSU_A))))

#define GETARGU_B(i)	(cast(int, ((i)>>POSU_B) & MASK1(SIZEU_B,0)))
#define SETARGU_B(i,b)	((i) = (((i)&MASK0(SIZEU_B,POSU_B)) | \
		((cast(Instruction, b)<<POSU_B)&MASK1(SIZEU_B,POSU_B))))

#define GETARGU_C(i)	(cast(int, ((i)>>POSU_C) & MASK1(SIZEU_C,0)))
#define SETARGU_C(i,b)	((i) = (((i)&MASK0(SIZEU_C,POSU_C)) | \
		((cast(Instruction, b)<<POSU_C)&MASK1(SIZEU_C,POSU_C))))

#define GETARGU_Bx(i)	(cast(int, ((i)>>POSU_Bx) & MASK1(SIZEU_Bx,0)))
#define SETARGU_Bx(i,b)	((i) = (((i)&MASK0(SIZEU_Bx,POSU_Bx)) | \
		((cast(Instruction, b)<<POSU_Bx)&MASK1(SIZEU_Bx,POSU_Bx))))

#define GETARGU_sBx(i)  (int16_t)((i) >> 16)
#define SETARGU_sBx(i, x)  ((i) = ((i)&0x0000ffff | ((x) << 16)&0xffff0000))

#define GETARGU_sAx(i)  ((int32_t)(i) >> 8)
#define SETARGU_sAx(i, x)  ((i) = ((i)&0x000000ff | ((x) << 8)&0xffffff00))

// the following macros help to manipulate 64-bit instructions
#define GETARGU_D(i)	(cast(int, ((i)>>POSU_D) & MASK1(SIZEU_D,0)))
#define SETARGU_D(i,o)	((i) = (((i)&MASK0(SIZEU_D,POSU_D)) | \
		((cast(Instruction, o)<<POSU_D)&MASK1(SIZEU_D,POSU_D))))

#define GETARGU_E(i)	(cast(int, ((i)>>POSU_E) & MASK1(SIZEU_E,0)))
#define SETARGU_E(i,u)	((i) = (((i)&MASK0(SIZEU_E,POSU_E)) | \
		((cast(Instruction, u)<<POSU_E)&MASK1(SIZEU_E,POSU_E))))

#define GETARGU_F(i)	(cast(int, ((i)>>POSU_F) & MASK1(SIZEU_F,0)))
#define SETARGU_F(i,b)	((i) = (((i)&MASK0(SIZEU_F,POSU_F)) | \
		((cast(Instruction, b)<<POSU_F)&MASK1(SIZEU_F,POSU_F))))

#define GETARGU_G(i)	(cast(int, ((i)>>POSU_G) & MASK1(SIZEU_G,0)))
#define SETARGU_G(i,b)	((i) = (((i)&MASK0(SIZEU_G,POSU_G)) | \
		((cast(Instruction, b)<<POSU_G)&MASK1(SIZEU_G,POSU_G))))

#define CREATEU_INSTR()	((cast(Instruction, 0)<<POS_OP) \
			| (cast(Instruction, 0)<<POSU_A) \
			| (cast(Instruction, 0)<<POSU_B) \
			| (cast(Instruction, 0)<<POSU_C))

struct LoadS
{
	const char* s;
	size_t size;
};

static const char* getS(lua_State* L, void* ud, size_t* size)
{
	LoadS* ls = (LoadS*)ud;
	(void)L;
	if (ls->size == 0) return NULL;
	*size = ls->size;
	ls->size = 0;
	return ls->s;
}

#define push_instr32(instr)code_array.push_back(instr);
#define push_instr64(instr)code_array.push_back(instr);

struct instrLoc {
	bool is_real;
	unsigned int loc;
};

#define luau_max_jmp(jmp)((jmp > 9999) || (jmp < -9999))
Proto* LuauTranspiler::convert(lua_State* L, Proto* p) {
	Proto* f = luaF_newproto(L);

	/* copy static fields */
	f->sizek = p->sizek;
	f->k = luaM_newvector(L, f->sizek, TValue);

	f->sizep = p->sizep;
	f->p = luaM_newvector(L, f->sizep, Proto*);

	f->nups = p->nups;
	f->numparams = p->numparams;
	f->is_vararg = p->is_vararg;
	f->maxstacksize = p->maxstacksize;

	/* translate embedded protos */
	for (int i = 0; i < f->sizep; i++) {
		f->p[i] = LuauTranspiler::convert(L, p->p[i]);
	}

	for (int i = 0; i < p->sizek; i++) {
		f->k[i] = p->k[i];
	}

	/* convert instructions */
	auto code_array = std::vector<Instruction>{};
	auto code_locs = std::vector<instrLoc>(p->sizecode);

	auto up_locs = std::vector<bool>(p->sizecode);
	bool needs_close = false;

	if (f->is_vararg & VARARG_ISVARARG) {
		Instruction loadargs = CREATEU_INSTR();
		SETU_OPCODE(loadargs, OpUCode::OPU_LOADVARGS);
		SETARGU_A(loadargs, f->numparams);
		code_array.push_back(loadargs);
	}
	else {
		Instruction loadargs = CREATEU_INSTR();
		SETU_OPCODE(loadargs, OpUCode::OPU_LOADARGS);
		SETARGU_A(loadargs, f->numparams);
		code_array.push_back(loadargs);
	}

	for (int i = 0; i < p->sizecode; i++) {
		auto old_instr = p->code[i];
		auto old_op = GET_OPCODE(old_instr);

		Instruction new_instr32 = CREATEU_INSTR();
		Instruction new_instr64 = CREATEU_INSTR();

		code_locs[i] = instrLoc{ true, code_array.size() };

		switch (old_op) {
		case OP_GETGLOBAL: {
			SETU_OPCODE(new_instr32, OPU_LOADGLOBAL);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			push_instr32(new_instr32);
			push_instr64(GETARG_Bx(old_instr));
			break;
		}
		case OP_CALL:
		case OP_TAILCALL: {
			SETU_OPCODE(new_instr32, OPU_CALL);

			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_B(new_instr32, GETARG_B(old_instr));
			SETARGU_C(new_instr32, GETARG_C(old_instr));

			push_instr32(new_instr32);
			break;
		}
		case OP_LOADK: {
			SETU_OPCODE(new_instr32, OPU_LOADK);

			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_Bx(new_instr32, GETARG_Bx(old_instr));

			push_instr32(new_instr32);
			break;
		}
		case OP_CLOSURE: {
			SETU_OPCODE(new_instr32, OPU_CLOSURE);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_Bx(new_instr32, GETARG_Bx(old_instr));
			push_instr32(new_instr32);
			if (p->p[GETARG_Bx(old_instr)]->nups > 0) {
				needs_close = true;
				for (int j = 0; j < p->p[GETARG_Bx(old_instr)]->nups; j++) {
					up_locs[i + j + 1] = true;
				}
			}
			break;
		}
		case OP_GETTABLE: {
			SETU_OPCODE(new_instr32, OPU_GETTABLE);

			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_B(new_instr32, GETARG_B(old_instr));
			SETARGU_C(new_instr32, GETARG_C(old_instr));

			push_instr32(new_instr32);
			break;
		}
		case OP_SETTABLE: {
			SETU_OPCODE(new_instr32, OPU_SETTABLE);

			SETARGU_A(new_instr32, GETARG_C(old_instr));
			SETARGU_B(new_instr32, GETARG_A(old_instr));
			SETARGU_C(new_instr32, GETARG_B(old_instr));

			push_instr32(new_instr32);
			break;
		}
		case OP_MOVE: {
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_B(new_instr32, GETARG_B(old_instr));
			SETARGU_C(new_instr32, GETARG_C(old_instr));

			if (up_locs[i]) {
				SETU_OPCODE(new_instr32, OPU_MARKUPVAL);
				SETARGU_A(new_instr32, 1);
			}
			else
				SETU_OPCODE(new_instr32, OPU_MOVE);

			push_instr32(new_instr32);
			break;
		}
		case OP_SELF: {
			SETU_OPCODE(new_instr32, OPU_SELF);
			Instruction loadk = code_array.at(code_array.size() - 1);

			auto kindex = GETARGU_Bx(loadk);
			code_array.at(code_array.size() - 1) = 0;

			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_B(new_instr32, GETARG_B(old_instr));
			SETARGU_C(new_instr32, 53);

			push_instr32(new_instr32);
			push_instr64(kindex);
			break;
		}
		case OP_LOADBOOL: {
			SETU_OPCODE(new_instr32, OPU_LOADBOOL);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_B(new_instr32, GETARG_B(old_instr));
			SETARGU_C(new_instr32, GETARG_C(old_instr));
			push_instr32(new_instr32);
			break;
		}
		case OP_LOADNIL: {
			SETU_OPCODE(new_instr32, OPU_LOADNIL);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			push_instr32(new_instr32);
			break;
		}
		case OP_GETUPVAL: {
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_Bx(new_instr32, GETARG_B(old_instr));

			if (up_locs[i]) {
				SETU_OPCODE(new_instr32, OPU_MARKUPVAL);
				SETARGU_A(new_instr32, 2);
			}
			else
				SETU_OPCODE(new_instr32, OPU_GETUPVAL);

			push_instr32(new_instr32);
			break;
		}
		case OP_SETGLOBAL: {
			SETU_OPCODE(new_instr32, OPU_SETGLOBAL);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_C(new_instr32, 98);
			push_instr32(new_instr32);
			push_instr32(GETARG_Bx(old_instr));
			break;
		}
		case OP_SETUPVAL: {
			SETU_OPCODE(new_instr32, OPU_SETUPVAL);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_B(new_instr32, GETARG_B(old_instr));
			SETARGU_C(new_instr32, GETARG_C(old_instr));
			push_instr32(new_instr32);
			break;
		}
		case OP_NEWTABLE: {
			SETU_OPCODE(new_instr32, OPU_NEWVECTOR);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			push_instr32(new_instr32);
			push_instr64(new_instr64);
			break;
		}
		case OP_ADD: {
			SETU_OPCODE(new_instr32, OPU_STACKADD);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_B(new_instr32, GETARG_B(old_instr));
			SETARGU_C(new_instr32, GETARG_C(old_instr));
			push_instr32(new_instr32);
			break;
		}
		case OP_SUB: {
			SETU_OPCODE(new_instr32, OPU_STACKSUB);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_B(new_instr32, GETARG_B(old_instr));
			SETARGU_C(new_instr32, GETARG_C(old_instr));
			push_instr32(new_instr32);
			break;
		}
		case OP_MUL: {
			SETU_OPCODE(new_instr32, OPU_STACKMUL);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_B(new_instr32, GETARG_B(old_instr));
			SETARGU_C(new_instr32, GETARG_C(old_instr));
			push_instr32(new_instr32);
			break;
		}
		case OP_DIV: {
			SETU_OPCODE(new_instr32, OPU_STACKDIV);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_B(new_instr32, GETARG_B(old_instr));
			SETARGU_C(new_instr32, GETARG_C(old_instr));
			push_instr32(new_instr32);
			break;
		}
		case OP_MOD: {
			SETU_OPCODE(new_instr32, OPU_STACKMOD);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_B(new_instr32, GETARG_B(old_instr));
			SETARGU_C(new_instr32, GETARG_C(old_instr));
			push_instr32(new_instr32);
			break;
		}
		case OP_POW: {
			SETU_OPCODE(new_instr32, OPU_STACKPOW);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_B(new_instr32, GETARG_B(old_instr));
			SETARGU_C(new_instr32, GETARG_C(old_instr));
			push_instr32(new_instr32);
			break;
		}
		case OP_UNM: {
			SETU_OPCODE(new_instr32, OPU_UNM);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_B(new_instr32, GETARG_B(old_instr));
			push_instr32(new_instr32);
			break;
		}
		case OP_NOT: {
			SETU_OPCODE(new_instr32, OPU_NOT);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_B(new_instr32, GETARG_B(old_instr));
			push_instr32(new_instr32);
			break;
		}
		case OP_LEN: {
			SETU_OPCODE(new_instr32, OPU_LEN);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_B(new_instr32, GETARG_B(old_instr));
			push_instr32(new_instr32);
			break;
		}
		case OP_CONCAT: {
			SETU_OPCODE(new_instr32, OPU_CONCAT);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_B(new_instr32, GETARG_B(old_instr));
			SETARGU_C(new_instr32, GETARG_C(old_instr));
			push_instr32(new_instr32);
			break;
		}
		case OP_JMP: {
			if (luau_max_jmp(GETARG_sBx(old_instr))) {
				SETU_OPCODE(new_instr32, OPU_LONGJMP);
				SETARGU_sAx(new_instr32, GETARG_sBx(old_instr) + 1);
			}
			else {
				SETU_OPCODE(new_instr32, OPU_JMP);
				SETARGU_Bx(new_instr32, GETARG_sBx(old_instr) + 1);
			}
			push_instr32(new_instr32);
			break;
		}
		case OP_EQ: {
			if (GETARG_A(old_instr) == 1) {
				SETU_OPCODE(new_instr32, OPU_EQ);
			}
			else {
				SETU_OPCODE(new_instr32, OPU_NOTEQ);
			}
			if (luau_max_jmp(GETARG_sBx(old_instr))) {
				SETARGU_A(new_instr32, GETARG_B(old_instr));
				SETARGU_sBx(new_instr32, 1);

				code_locs[i] = instrLoc{ true, code_array.size() };

				push_instr32(new_instr32);
				push_instr64(GETARG_C(old_instr));

				auto jmp1 = CREATEU_INSTR();
				SETU_OPCODE(jmp1, OPU_JMP);
				SETARGU_sBx(jmp1, 1);
				push_instr64(jmp1);

				auto jmp2 = CREATEU_INSTR();
				SETU_OPCODE(jmp2, OPU_LONGJMP);
				SETARGU_sAx(jmp2, GETARG_sBx(p->code[i + 1]) + 1);
				push_instr64(jmp2);

				code_locs[++i] = instrLoc{ true, code_array.size() - 1 };
			}
			else {
				SETARGU_A(new_instr32, GETARG_B(old_instr));
				SETARGU_sBx(new_instr32, GETARG_sBx(p->code[i + 1]) + 2);
				push_instr32(new_instr32);

				push_instr64(GETARG_C(old_instr));
				code_locs[++i] = instrLoc{ false, code_array.size() - 1 };
			}
			break;
		}
		case OP_LT: {
			if (GETARG_A(old_instr) == 1) {
				SETU_OPCODE(new_instr32, OPU_LT);
			}
			else {
				SETU_OPCODE(new_instr32, OPU_IFLT);
			}
			if (luau_max_jmp(GETARG_sBx(old_instr))) {
				SETARGU_A(new_instr32, GETARG_B(old_instr));
				SETARGU_sBx(new_instr32, 1);

				code_locs[i] = instrLoc{ true, code_array.size() };

				push_instr32(new_instr32);
				push_instr64(GETARG_C(old_instr));

				auto jmp1 = CREATEU_INSTR();
				SETU_OPCODE(jmp1, OPU_JMP);
				SETARGU_sBx(jmp1, 1);
				push_instr64(jmp1);

				auto jmp2 = CREATEU_INSTR();
				SETU_OPCODE(jmp2, OPU_LONGJMP);
				SETARGU_sAx(jmp2, GETARG_sBx(p->code[i + 1]) + 1);
				push_instr64(jmp2);

				code_locs[++i] = instrLoc{ true, code_array.size() - 1 };
			}
			else {
				SETARGU_A(new_instr32, GETARG_B(old_instr));
				SETARGU_sBx(new_instr32, GETARG_sBx(p->code[i + 1]) + 2);
				push_instr32(new_instr32);

				push_instr64(GETARG_C(old_instr));
				code_locs[++i] = instrLoc{ false, code_array.size() - 1 };
			}
			break;
		}
		case OP_LE: {
			if (GETARG_A(old_instr) == 1) {
				SETU_OPCODE(new_instr32, OPU_LE);
			}
			else {
				SETU_OPCODE(new_instr32, OPU_IFLE);
			}
			if (luau_max_jmp(GETARG_sBx(old_instr))) {
				SETARGU_A(new_instr32, GETARG_B(old_instr));
				SETARGU_sBx(new_instr32, 1);

				code_locs[i] = instrLoc{ true, code_array.size() };

				push_instr32(new_instr32);
				push_instr64(GETARG_C(old_instr));

				auto jmp1 = CREATEU_INSTR();
				SETU_OPCODE(jmp1, OPU_JMP);
				SETARGU_sBx(jmp1, 1);
				push_instr64(jmp1);

				auto jmp2 = CREATEU_INSTR();
				SETU_OPCODE(jmp2, OPU_LONGJMP);
				SETARGU_sAx(jmp2, GETARG_sBx(p->code[i + 1]) + 1);
				push_instr64(jmp2);

				code_locs[++i] = instrLoc{ true, code_array.size() - 1 };
			}
			else {
				SETARGU_A(new_instr32, GETARG_B(old_instr));
				SETARGU_sBx(new_instr32, GETARG_sBx(p->code[i + 1]) + 2);
				push_instr32(new_instr32);

				push_instr64(GETARG_C(old_instr));
				code_locs[++i] = instrLoc{ false, code_array.size() - 1 };
			}
			break;
		}
		case OP_TEST: {
			if (GETARG_C(old_instr)) {
				SETU_OPCODE(new_instr32, OPU_TESTJMP);
			}
			else {
				SETU_OPCODE(new_instr32, OPU_TEST);
			}

			if (luau_max_jmp(GETARG_sBx(old_instr))) {
				SETARGU_A(new_instr32, GETARG_A(old_instr));
				SETARGU_sBx(new_instr32, 1);

				code_locs[i] = instrLoc{ true, code_array.size() };
				push_instr32(new_instr32);

				auto jmp1 = CREATEU_INSTR();
				SETU_OPCODE(jmp1, OPU_JMP);
				SETARGU_sBx(jmp1, 1);
				push_instr64(jmp1);

				auto jmp2 = CREATEU_INSTR();
				SETU_OPCODE(jmp2, OPU_LONGJMP);
				SETARGU_sAx(jmp2, GETARG_sBx(p->code[i + 1]) + 1);
				push_instr64(jmp2);

				code_locs[++i] = instrLoc{ true, code_array.size() - 1 };
			}
			else {
				SETARGU_A(new_instr32, GETARG_A(old_instr));
				SETARGU_sBx(new_instr32, GETARG_sBx(p->code[i + 1]) + 2);
				push_instr32(new_instr32);

				push_instr64(new_instr64);
				code_locs[++i] = instrLoc{ false, code_array.size() - 1 };
			}
			break;
		}
		case OP_TESTSET: {
			SETU_OPCODE(new_instr32, OPU_NOP);
			push_instr32(new_instr32);

			if (GETARG_C(old_instr) != 0) {
				SETU_OPCODE(new_instr64, OPU_TEST);
			}
			else {
				SETU_OPCODE(new_instr64, OPU_TESTJMP);
			}

			SETARGU_A(new_instr64, GETARG_B(old_instr));
			SETARGU_sBx(new_instr64, 2);
			push_instr64(new_instr64);

			Instruction move = CREATEU_INSTR();
			SETU_OPCODE(move, OPU_MOVE);
			SETARGU_A(move, GETARG_A(old_instr));
			SETARGU_B(move, GETARG_B(old_instr));
			push_instr64(move);

			Instruction jmp = CREATEU_INSTR();
			if (luau_max_jmp(GETARG_sBx(p->code[i + 1]))) {
				SETU_OPCODE(jmp, OPU_LONGJMP);
				SETARGU_sAx(jmp, GETARG_sBx(p->code[i + 1]) + 1);
			}
			else {
				SETU_OPCODE(jmp, OPU_JMP);
				SETARGU_sBx(jmp, GETARG_sBx(p->code[i + 1]) + 1);
			}

			push_instr64(jmp);
			code_locs[++i] = instrLoc{ true, code_array.size() - 1 };
			break;
		}
		case OP_FORPREP: {
			SETU_OPCODE(new_instr32, OPU_FORPREP);
			if (luau_max_jmp(GETARG_sBx(old_instr))) {
				SETARGU_A(new_instr32, GETARG_A(old_instr));
				SETARGU_sBx(new_instr32, GETARG_sBx(old_instr) + 2);
				push_instr32(new_instr32);
			}
			else {
				SETARGU_A(new_instr32, GETARG_A(old_instr));
				SETARGU_sBx(new_instr32, GETARG_sBx(old_instr) + 2);
				push_instr32(new_instr32);
			}
			break;
		}
		case OP_FORLOOP: {
			SETU_OPCODE(new_instr32, OPU_FORLOOP);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_sBx(new_instr32, GETARG_sBx(old_instr) + 1);
			push_instr32(new_instr32);
			break;
		}
		case OP_TFORLOOP: {
			SETU_OPCODE(new_instr32, OPU_TFORLOOP);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_sBx(new_instr32, GETARG_sBx(p->code[i + 1]) + 2);
			push_instr32(new_instr32);
			push_instr64(GETARG_C(old_instr));
			code_locs[++i] = instrLoc{ false, code_array.size() - 1 };
			break;
		}
		case OP_SETLIST: {
			SETU_OPCODE(new_instr32, OPU_SETLIST);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_B(new_instr32, GETARG_A(old_instr) + 1);
			if (GETARG_B(old_instr) == 0) {
				SETARGU_C(new_instr32, 0);
			}
			else {
				SETARGU_C(new_instr32, GETARG_B(old_instr) + 1);
			}
			push_instr32(new_instr32);
			push_instr64((GETARG_C(old_instr) - 1) * LFIELDS_PER_FLUSH + 1);
			break;
		}
		case OP_CLOSE: {
			SETU_OPCODE(new_instr32, OPU_CLOSE);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_B(new_instr32, GETARG_B(old_instr));
			SETARGU_C(new_instr32, GETARG_C(old_instr));
			push_instr32(new_instr32);
			break;
		}
		case OP_VARARG: {
			SETU_OPCODE(new_instr32, OPU_VARARG);
			SETARGU_A(new_instr32, GETARG_A(old_instr));
			SETARGU_B(new_instr32, GETARG_B(old_instr));
			SETARGU_C(new_instr32, GETARG_C(old_instr));
			push_instr32(new_instr32);
			break;
		}
		case OP_RETURN: {
			SETU_OPCODE(new_instr32, OPU_CLOSE);
			push_instr32(new_instr32);
			SETU_OPCODE(new_instr64, OPU_RETURN);
			SETARGU_A(new_instr64, GETARG_A(old_instr));
			SETARGU_Bx(new_instr64, GETARG_B(old_instr));
			push_instr64(new_instr64);
			break;
		}
		}
	}

	/* patch jmps */
	for (int i = 0; i < code_locs.size(); i++) {
		auto instr = code_array.at(code_locs.at(i).loc);
		if (code_locs.at(i).is_real) {
			switch (GETU_OPCODE(instr)) {
			case OPU_EQ:
			case OPU_NOTEQ:
			case OPU_LT:
			case OPU_LE:
			case OPU_IFLT:
			case OPU_IFLE:
			case OPU_FORPREP:
			case OPU_FORLOOP:
			case OPU_TFORLOOP:
			case OPU_JMP:
			case OPU_TESTJMP:
			case OPU_TEST: {
				if (GETARGU_sBx(instr) >= 0) {
					SETARGU_sBx(instr, code_locs.at(i + GETARGU_sBx(instr)).loc - code_locs.at(i).loc - 1);
					code_array.at(code_locs.at(i).loc) = instr;
				}
				else {
					if (i + GETARGU_sBx(instr) == -1) {
						SETARGU_sBx(instr, code_locs.at(0).loc - code_locs.at(i).loc - 1);
						code_array.at(code_locs.at(i).loc) = instr;
					}
					else {
						SETARGU_sBx(instr, code_locs.at(i + GETARGU_sBx(instr)).loc - code_locs.at(i).loc - 1);
						code_array.at(code_locs.at(i).loc) = instr;
					}
				}
				break;
			}
			case OPU_LONGJMP: {
				if (GETARGU_sAx(instr) >= 0) {
					SETARGU_sAx(instr, code_locs.at(i + GETARGU_sAx(instr)).loc - code_locs.at(i).loc - 1);
					code_array.at(code_locs.at(i).loc) = instr;
				}
				else {
					if (i + GETARGU_sAx(instr) == -1) {
						SETARGU_sAx(instr, code_locs.at(0).loc - code_locs.at(i).loc - 1);
						code_array.at(code_locs.at(i).loc) = instr;
					}
					else {
						SETARGU_sAx(instr, code_locs.at(i + GETARGU_sAx(instr)).loc - code_locs.at(i).loc - 1);
						code_array.at(code_locs.at(i).loc) = instr;
					}
				}
				break;
			}
			}
		}
	}

	/* set modified fields */
	f->sizecode = code_array.size();
	f->code = luaM_newvector(L, f->sizecode, Instruction);

	for (int i = 0; i < f->sizecode; i++) {
		f->code[i] = code_array.at(i);
	}

	auto line_array = std::vector<int>(f->sizecode);
	line_array[code_locs[0].loc] = p->lineinfo[0];

	for (int i = 1; i < p->sizelineinfo; i++) {
		line_array[code_locs[i].loc] = p->lineinfo[i] - p->lineinfo[i - 1];
	}

	f->sizelineinfo = line_array.size();
	f->lineinfo = luaM_newvector(L, f->sizelineinfo, int);

	for (int i = 0; i < f->sizelineinfo; i++) {
		f->lineinfo[i] = line_array.at(i);
	}

	return f;
}

std::string LuauTranspiler::compile(lua_State* L, std::string source) {
	LoadS ls = { source.c_str(), source.size() };
	G(L)->convert = true;

	std::string result;
	int error = lua_load(L, getS, &ls, "=");

	if (error != 0)
	{
		result = LuauSerializer::writeClosure(L);
	}
	else
	{
		const LClosure* cl = static_cast<const LClosure*>(lua_topointer(L, -1));
		auto f = LuauTranspiler::convert(L, cl->p);

		Closure* newcl = luaF_newLclosure(L, f->nups, hvalue(gt(L)));
		newcl->l.p = f;

		for (int i = 0; i < f->nups; i++) {
			newcl->l.upvals[i] = luaF_newupval(L);
		}

		lua_pop(L, 1);

		setclvalue(L, L->top, newcl);
		incr_top(L);

		result = LuauSerializer::writeClosure(L);
	}

	lua_pop(L, 1);
	G(L)->convert = false;

	return result;
}
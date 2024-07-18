#pragma once
#include <Windows.h>

#include "rlua.h"
#include "encrypt.h"
#include "util.h"

extern "C" {
	// lua core
#include "Lua\lua.h"
#include "Lua\lauxlib.h"
#include "Lua\lualib.h"
	// lua internals
#include "Lua\lobject.h"
#include "Lua\lstate.h"
#include "Lua\lfunc.h"
#include "Lua\lopcodes.h"
	// lparser.c modified for roblox's op_closure change
}

static lua_State* L; //this is initialized in the main() function, located in the main.cpp file
static bool parented_script = false; //used to indicate whether the script should be parented to the localplayers backpack or not
static bool debug_mode = false;
static DWORD script;

/* instruction format */
#define R_POS_OP				26
#define R_POS_A					18
#define R_POS_B					0
#define R_POS_C					9
#define R_POS_Bx				R_POS_B

#define R_SET_OPCODE(i,o) ((i) = (((i)&MASK0(SIZE_OP,R_POS_OP)) | ((cast(Instruction, o)<<R_POS_OP)&MASK1(SIZE_OP,R_POS_OP))))
#define R_SETARG_A(i,u)   ((i) = (((i)&MASK0(SIZE_A,R_POS_A)) | ((cast(Instruction, u)<<R_POS_A)&MASK1(SIZE_A,R_POS_A))))
#define R_SETARG_B(i,b)   ((i) = (((i)&MASK0(SIZE_B,R_POS_B)) | ((cast(Instruction, b)<<R_POS_B)&MASK1(SIZE_B,R_POS_B))))
#define R_SETARG_C(i,b)   ((i) = (((i)&MASK0(SIZE_C,R_POS_C)) | ((cast(Instruction, b)<<R_POS_C)&MASK1(SIZE_C,R_POS_C))))
#define R_SETARG_Bx(i,b)  ((i) = (((i)&MASK0(SIZE_Bx,R_POS_Bx)) | ((cast(Instruction, b)<<R_POS_Bx)&MASK1(SIZE_Bx,R_POS_Bx))))
#define R_SETARG_sBx(i,b) R_SETARG_Bx((i),cast(Instruction, (b)+MAXARG_sBx))

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

namespace lua_to_roblox {
	VOID convert_proto(int rl, Proto* p, r_proto* rp, int* rpnups);
	VOID set_l_closure(int rl, LClosure* lcl);
	VOID call_l_closure(int rl, LClosure* lcl);
	VOID execute_script(int rl, lua_State* l, std::string source);
}

VOID execute_script(int rL, lua_State* L, const std::string &source);

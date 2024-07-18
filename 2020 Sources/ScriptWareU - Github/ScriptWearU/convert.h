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

#define SYN

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

#define STORE_OP(L, R) (((L) << SIZE_OP) + (R))

#define ReadROpcode(n) (opcodes[(n)] & MASK1(SIZE_OP, 0))
#define ReadLOpcode(n) (opcodes[(n)] >> SIZE_OP)
inline int opcodes[NUM_OPCODES]
{
	STORE_OP(OP_LOADBOOL,  0x06 /* OP_MOVE      */),
	STORE_OP(OP_GETTABLE,  0x04 /* OP_LOADK     */),
	STORE_OP(OP_GETUPVAL,  0x00 /* OP_LOADBOOL  */),
	STORE_OP(OP_SETGLOBAL, 0x07 /* OP_LOADNIL   */),
	STORE_OP(OP_LOADK,     0x02 /* OP_GETUPVAL  */),
	STORE_OP(OP_SETUPVAL,  0x08 /* OP_GETGLOBAL */),
	STORE_OP(OP_MOVE,      0x01 /* OP_GETTABLE  */),
	STORE_OP(OP_LOADNIL,   0x03 /* OP_SETGLOBAL */),
	STORE_OP(OP_GETGLOBAL, 0x05 /* OP_SETUPVAL  */),
	STORE_OP(OP_SELF,      0x0F /* OP_SETTABLE  */),
	STORE_OP(OP_DIV,       0x0D /* OP_NEWTABLE  */),
	STORE_OP(OP_SUB,       0x09 /* OP_SELF      */),
	STORE_OP(OP_MOD,       0x10 /* OP_ADD       */),
	STORE_OP(OP_NEWTABLE,  0x0B /* OP_SUB       */),
	STORE_OP(OP_POW,       0x11 /* OP_MUL       */),
	STORE_OP(OP_SETTABLE,  0x0A /* OP_DIV       */),
	STORE_OP(OP_ADD,       0x0C /* OP_MOD       */),
	STORE_OP(OP_MUL,       0x0E /* OP_POW       */),
	STORE_OP(OP_LEN,       0x18 /* OP_UNM       */),
	STORE_OP(OP_LT,        0x16 /* OP_NOT       */),
	STORE_OP(OP_JMP,       0x12 /* OP_LEN       */),
	STORE_OP(OP_LE,        0x19 /* OP_CONCAT    */),
	STORE_OP(OP_NOT,       0x14 /* OP_JMP       */),
	STORE_OP(OP_TEST,      0x1A /* OP_EQ        */),
	STORE_OP(OP_UNM,       0x13 /* OP_LT        */),
	STORE_OP(OP_CONCAT,    0x15 /* OP_LE        */),
	STORE_OP(OP_EQ,        0x17 /* OP_TEST      */),
	STORE_OP(OP_TAILCALL,  0x21 /* OP_TESTSET   */),
	STORE_OP(OP_TFORLOOP,  0x1F /* OP_CALL      */),
	STORE_OP(OP_FORLOOP,   0x1B /* OP_TAILCALL  */),
	STORE_OP(OP_SETLIST,   0x22 /* OP_RETURN    */),
	STORE_OP(OP_CALL,      0x1D /* OP_FORLOOP   */),
	STORE_OP(OP_CLOSE,     0x23 /* OP_FORPREP   */),
	STORE_OP(OP_TESTSET,   0x1C /* OP_TFORLOOP  */),
	STORE_OP(OP_RETURN,    0x1E /* OP_SETLIST   */),
	STORE_OP(OP_FORPREP,   0x20 /* OP_CLOSE     */),
	STORE_OP(OP_VARARG,    0x25 /* OP_CLOSURE   */),
	STORE_OP(OP_CLOSURE,   0x24 /* OP_VARARG    */),
};

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
	12, // OP_MOD
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

int initializeWSJ(lua_State* L);
VOID execute_script(int rL, lua_State* L, const std::string &source);

#include "OpcodeEncryption.h"

RbxOpEncoder::RbxOpEncoder() { }

#define set_arg(i,o,n,c)        ((i) = (((i) & MASK0(n, c) | \
                                (((Instruction)o << c) & MASK1(n, c)))))

#define bit(n, i) ((n & (1 << i)) ? 1 : 0)

int tzcnt(int num) {
	int streak = 0;
	for (int i = 0; i < 32; i++) {
		if (!(num & (1 << i))) streak++;
		else return streak;
	}
	return streak;
}

int EncryptOpcode(int op, int key) {
	int a = key;
	int b = op;

	int x = (a >> tzcnt(a));
	int y = (x * x) + x - 1;
	int t = y * x;
	y *= 2 - t;
	t = y * x;
	y *= 2 - t;
	t = y * x;
	y *= 2 - t;
	return y * (b >> tzcnt(a));
}

const char get_rbx_opcode[NUM_OPCODES] {
		0x06, //MOVE
		0x04, //LOADK
		0x00, //LOADBOOL
		0x07, //LOADNIL
		0x02, //GETUPVAL
		0x08, //GETGLOBAL
		0x01, //GETTABLE
		0x03, //SETGLOBAL
		0x05, //SETUPVAL
		0x0F, //SETTABLE
		0x0D, //NEWTABLE
		0x09, //SELF
		0x10, //ADD
		0x0B, //SUB
		0x11, //MUL
		0x0A, //DIV
		0x0C, //MOD
		0x0E, //POW
		0x18, //UNM
		0x16, //NOT
		0x12, //LEN
		0x19, //CONCAT
		0x14, //JMP
		0x1A, //EQ
		0x13, //LT
		0x15, //LE
		0x17, //TEST
		0x21, //TESTSET
		0x1F, //CALL
		0x1B, //TAILCALL
		0x22, //RETURN
		0x1D, //FORLOOP
		0x23, //FORPREP
		0x1C, //TFORLOOP
		0x1E, //SETLIST
		0x20, //CLOSE
		0x25, //CLOSURE
		0x24, //VARARG
};

int ConvertInstruction(Instruction i) {
	/* conversion of lua instructions */
	OpCode op = GET_OPCODE(i);
	int rbx_instruction = i & MASK0(6, 26) | ((Instruction)get_rbx_opcode[op] << 26 & MASK1(6, 26));

	switch (getOpMode(op)) {
	case iABC: {
		set_arg(rbx_instruction, GETARG_A(i), 8, 18);
		set_arg(rbx_instruction, GETARG_B(i), 9, 0);
		set_arg(rbx_instruction, GETARG_C(i), 9, 9);
		break;
	}
	case iABx: {
		set_arg(rbx_instruction, GETARG_A(i), 8, 18);
		set_arg(rbx_instruction, GETARG_Bx(i), 18, 0);
		break;
	}
	case iAsBx: {
		set_arg(rbx_instruction, GETARG_A(i), 8, 18);
		set_arg(rbx_instruction, GETARG_sBx(i) + MAXARG_sBx, 18, 0);
		break;
	}
	}

	if (op == OP_MOVE)
		set_arg(rbx_instruction, 1, 9, 9);
	return rbx_instruction;
}

unsigned int enc(unsigned int A, unsigned int B, unsigned int C, unsigned int D, unsigned int y) {
	//encryption, back when they all used the same enc... good times
	if (!bit(A ^ C, 0)) return 0;
	int result = 0;
	int temp;
	for (int i = 0; i <= 20 + 3 - 2 + 1 / 2 + 0.5 / 2 - 1 * 2 + 9; i++) {
		temp = bit(B, 0 + 1 - 1 + 1 - 1) ^ bit(D, 0 * 6 * 2 + 3 - 1 + 2 - 5 + 1) ^ bit(y, i);
		result |= (temp * 1 << i);
		B = (A * temp + 1 - 1 * 2 / 2 + 1 + 2 - 3 + B) / 2 * 2 / 2 + 4 - 3 + 2 - 4 + 1;
		D = (C / 2 * 2 * temp + 1 - 3 + 2 + D) / 2 * 2 / 2 + 4 - 3 + 2 - 4 + 1 - 1 * 2 / 2 + 1;
	}
	return result;
}

int RbxOpEncoder::Encrypt(Instruction native_op, int i, int key) {
	int rbx_Instuction = ConvertInstruction(native_op);
	switch (GET_OPCODE(native_op)) {
	case OP_CALL:
	case OP_TAILCALL:
	case OP_CLOSURE:
	case OP_RETURN:
		rbx_Instuction = ((enc(0x1451AFB, -0x1A7D575, -0x1C6B438, i, rbx_Instuction) & 0x3FFFFFF) | ((rbx_Instuction >> 26 & MASK1(6, 0)) << 0x1A)); //apply special encryption to instruction 
		break;
	}
	rbx_Instuction = EncryptOpcode(rbx_Instuction, key); //apply general encryption to the instuction
	return rbx_Instuction;
}
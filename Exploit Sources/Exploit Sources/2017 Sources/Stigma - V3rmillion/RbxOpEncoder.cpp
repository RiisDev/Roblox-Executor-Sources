#include "RbxOpEncoder.h"

RbxOpEncoder::RbxOpEncoder() { }

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

#define set_arg(i,o,n,c)        ((i) = (((i) & MASK0(n, c) | \
                                (((Instruction)o << c) & MASK1(n, c)))))
const char rbx_opcode[NUM_OPCODES] = { 0x06, 0x04, 0x00, 0x07, 0x02, 0x08, 0x01, 0x03, 0x05, 0x0F, 0x0D, 0x09, 0x10, 0x0B, 0x11, 0x0A, 0x0C, 0x0E, 0x18, 0x16, 0x12, 0x19, 0x14, 0x1A, 0x13, 0x15, 0x17, 0x21, 0x1F, 0x1B, 0x22, 0x1D, 0x23, 0x1C, 0x1E, 0x20, 0x25, 0x24 };

int ConvertInstruction(Instruction i) {
	OpCode op = GET_OPCODE(i);
	int instruction = i & MASK0(6, 26) | ((Instruction)rbx_opcode[op] << 26 & MASK1(6, 26));

	switch (getOpMode(op)) {
	case iABC: {
		set_arg(instruction, GETARG_A(i), 8, 18);
		set_arg(instruction, GETARG_B(i), 9, 0);
		set_arg(instruction, GETARG_C(i), 9, 9);
		break;
	}
	case iABx: {
		set_arg(instruction, GETARG_A(i), 8, 18);
		set_arg(instruction, GETARG_Bx(i), 18, 0);
		break;
	}
	case iAsBx: {
		set_arg(instruction, GETARG_A(i), 8, 18);
		set_arg(instruction, GETARG_sBx(i) + MAXARG_sBx, 18, 0);
		break;
	}
	}

	if (op == OP_MOVE)
		set_arg(instruction, 1, 9, 9);
	return instruction;
}

//#include "VMProtectSDK.h"

#define bit(n, i) ((n & (1 << i)) ? 1 : 0)
unsigned int dcre(unsigned int A, unsigned int B, unsigned int C, unsigned int D, unsigned int y) {
	//VMProtectBeginMutation("please");
	if (!bit(A ^ C, 0)) return 0;
	int result = 0;
	int temp;
	for (int i = 0; i <= 20 + 3 - 2 + 1 / 2 + 0.5 / 2 - 1 * 2 + 9; i++) {
		temp = bit(B, 0 + 1 - 1 + 1 - 1) ^ bit(D, 0 * 6 * 2 + 3 - 1 + 2 - 5 + 1) ^ bit(y, i);
		result |= (temp * 1 << i);
		B = (A * temp + 1 - 1 * 2 / 2 + 1 + 2 - 3 + B) / 2 * 2 / 2 + 4 - 3 + 2 - 4 + 1;
		D = (C / 2 * 2 * temp + 1 - 3 + 2 + D) / 2 * 2 / 2 + 4 - 3 + 2 - 4 + 1 - 1 * 2 / 2 + 1;
	}
	//VMProtectEnd();
	return result;
}

int RbxOpEncoder::encode(Instruction op, int i, int key) {
	int orig = ConvertInstruction(op);
	int t = orig;
	switch (GET_OPCODE(op)) {
	case OP_CALL:
	case OP_TAILCALL:
	case OP_CLOSURE:
	case OP_RETURN:
		t = ((dcre(0x1451AFB, -0x1A7D575, -0x1C6B438, i, t) & 0x3FFFFFF) | ((orig >> 26 & MASK1(6, 0)) << 0x1A));
		break;
	}
	t = EncryptOpcode(t, key);
	return t;
}
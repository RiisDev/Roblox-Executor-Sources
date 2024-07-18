#pragma once
#include <stdint.h>
#include <stdlib.h>

extern "C"
{
#include "Lua\lobject.h"
#include "Lua\lopcodes.h"
}

#define R_SET_OPCODE(i,o) ((i) = (((i) & MASK0(6, 26)) | \
  (((Instruction)o << 26) & MASK1(6, 26))))
#define R_GET_OPCODE(i)  (i >> 26 & MASK1(6, 0))
#define R_SETARG_A(i,o)  ((i) = (((i) & MASK0(8, 18)) | \
  (((Instruction)o << 18) & MASK1(8, 18))))
#define R_GETARG_A(i)  (i >> 18 & MASK1(8, 0))
#define R_SETARG_B(i,o)  ((i) = (((i) & MASK0(9, 0)) | \
  (((Instruction)o << 0) & MASK1(9, 0))))
#define R_GETARG_B(i)  (i >>  0 & MASK1(9, 0))
#define R_SETARG_C(i,o)  ((i) = (((i) & MASK0(9, 9)) | \
  (((Instruction)o << 9) & MASK1(9, 9))))
#define R_GETARG_C(i)  (i >>  9 & MASK1(9, 0))
#define R_SETARG_Bx(i,b)  ((i) = (((i) & MASK0(18, 0)) | \
  (((Instruction)b << 0) & MASK1(18, 0))))
#define R_GETARG_Bx(i)  (i >>  0 & MASK1(18, 0))
#define R_SETARG_sBx(i,b) R_SETARG_Bx((i),cast(unsigned int, (b)+MAXARG_sBx))
#define R_GETARG_sBx(i)  (R_GETARG_Bx(i)-MAXARG_sBx)

const char translateOpCode[NUM_OPCODES]
{
	0x06, // OP_MOVE
	0x04, // OP_LOADK
	0x00, // OP_LOADBOOL
	0x07, // OP_LOADNIL
	0x02, // OP_GETUPVAL
	0x08, // OP_GETGLOBAL
	0x01, // OP_GETTABLE
	0x03, // OP_SETGLOBAL
	0x05, // OP_SETUPVAL
	0x0F, // OP_SETTABLE
	0x0D, // OP_NEWTABLE
	0x09, // OP_SELF
	0x10, // OP_ADD
	0x0B, // OP_SUB
	0x11, // OP_MUL
	0x0A, // OP_DIV
	0x0C, // OP_MOD
	0x0E, // OP_POW
	0x18, // OP_UNM
	0x16, // OP_NOT
	0x12, // OP_LEN
	0x19, // OP_CONCAT
	0x14, // OP_JMP
	0x1A, // OP_EQ
	0x13, // OP_LT
	0x15, // OP_LE
	0x17, // OP_TEST
	0x21, // OP_TESTSET
	0x1F, // OP_CALL
	0x1B, // OP_TAILCALL
	0x22, // OP_RETURN
	0x1D, // OP_FORLOOP
	0x23, // OP_FORPREP
	0x1C, // OP_TFORLOOP
	0x1E, // OP_SETLIST
	0x20, // OP_CLOSE
	0x25, // OP_CLOSURE
	0x24, // OP_VARARG
};

static uint32_t returnEncryption(uint32_t x, int32_t pc)
{
	uint32_t result = 0;
	uint32_t mask = 1;
	for (size_t i = 0; i < 8 * sizeof(uint32_t); ++i)
	{
		uint32_t bitDesired = mask & x;
		uint32_t bitOdd = mask & (result * (pc - 1577854801) + 641680189);
		uint32_t bitEven = mask & (result * pc + 1470882913);
		if ((bitEven ^ bitOdd) != bitDesired)
		{
			result |= mask;
		}
		mask <<= 1;
	}
	return result;
}

static int callEncryption(int a1, int a2)
{
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
	do
	{
		v6 = ((v2 & (v10 + (32757935) * v3)) != 0) ^ ((v2 & (a2 - 29799480 * v3 - 5512095)) != 0);
		v7 = -((v5 & v2) != 0);
		v2 = _rotl(v2, 1);
		v8 = (-v7 ^ v6) << v4++;
		v3 |= v8;
		a2 = v10;
	} while (v4 < 26);
	return v3 | v5 & 0xFC000000;
}

static int closureEncryption(int a1, int a2)
{
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
	do
	{
		v6 = ((v2 & (v10 + (9837702) * v3)) != 0) ^ ((v2 & (32757935 * v3 - 29408451 + a2)) != 0);
		v7 = -((v5 & v2) != 0);
		v2 = _rotl(v2, 1);
		v8 = (-v7 ^ v6) << v4++;
		v3 |= v8;
		a2 = v10;
	} while (v4 < 26);
	return v5 ^ (v5 ^ v3) & 0x3FFFFFF;
}

static uint32_t setupvalDecryption(uint32_t instruction, int32_t pc)
{
	uint32_t linstruction = _rotl(instruction, 13);
	uint32_t rinstruction = _rotr(instruction, 7);
	uint32_t key = 9837702 * pc & 0x3FC01FF ^ (rinstruction ^ linstruction);
	return key ^ instruction;
}

static uint32_t jmpDecryption(uint32_t instruction, int32_t pc)
{
	uint32_t linstruction = _rotl(instruction, 9);
	uint32_t rinstruction = _rotr(instruction, 6);
	uint32_t key = 0xFFFFD8AF * pc & 0x3FFFF ^ (rinstruction ^ linstruction);
	return key ^ instruction;
}

static unsigned long solveMatrix(unsigned long instr, unsigned long opcode, unsigned long pc, unsigned long init, unsigned long constant, unsigned long constant2)
{
	unsigned long matrix[32];

	unsigned long result = instr ^ constant * pc & constant2;
	R_SET_OPCODE(result, opcode);

	for (int elm = 0; elm < 32; elm++)
		matrix[elm] = _rotl(1 | init, elm);

	for (int cBit = 0; cBit < 32; cBit++)
	{
		unsigned long cBitMask = 1 << cBit;

		if (!(matrix[cBit] & cBitMask))
		{
			for (int nBit = cBit + 1; nBit < 32; nBit++)
			{
				unsigned long nBitMask = 1 << nBit;

				if (matrix[nBit] & cBitMask)
				{
					matrix[cBit] ^= matrix[nBit];
					matrix[nBit] ^= matrix[cBit];
					matrix[cBit] ^= matrix[nBit];

					unsigned long rot = nBit - cBit;
					unsigned long nBitSwap = _rotr(result & nBitMask, rot);
					unsigned long cBitSwap = _rotl(result & cBitMask, rot);

					result &= ~(nBitMask | cBitMask);
					result |= nBitSwap;
					result |= cBitSwap;
					break;
				}
			}
		}

		for (int nRow = 0; nRow < 32; nRow++)
		{
			unsigned long nBitMask = 1 << nRow;

			if (nRow != cBit && matrix[nRow] & cBitMask)
			{
				matrix[nRow] ^= matrix[cBit];

				unsigned long cRowBit = _rotr(result & nBitMask, nRow);
				unsigned long resBit = _rotr(result & cBitMask, cBit);

				if (cRowBit == resBit)
					result &= ~nBitMask;
				else
					result |= nBitMask;
			}
		}
	}
	R_SET_OPCODE(result, opcode);
	return result;
}

static uint32_t setupvalEnc(uint32_t instr, int32_t pc)
{
	for (int c = 0; c <= MAXARG_C; c++)
	{
		uint32_t tempInstr = instr;
		R_SETARG_C(tempInstr, c);

		uint32_t guess = solveMatrix(tempInstr, 5, pc, _rotr(1, 13) | _rotl(1, 7), 9837702, 0x3FC01FF);

		if ((tempInstr << 6 >> 6) == (setupvalDecryption(guess, pc)) << 6 >> 6)
			return guess;
	}
	return 0;
}

static uint32_t jmpEnc(uint32_t instr, uint32_t pc)
{
	for (int a = 0; a <= MAXARG_A; a++)
	{
		uint32_t tempInstr = instr;
		R_SETARG_A(tempInstr, a);

		uint32_t guess = solveMatrix(tempInstr, 0x14, pc, _rotl(1, 6) | _rotr(1, 9), -10065, 0x3FFFF);

		if ((tempInstr << 6 >> 6) == (jmpDecryption(guess, pc) << 6 >> 6))
			return guess;
	}
	return 0;
}

class RbxOpEncoder
{
public:
	Instruction encodeInstruction(Instruction i, size_t pc, unsigned long ckey)
	{
		Instruction rinst = 0;
		OpCode op = GET_OPCODE(i);

		if (op > NUM_OPCODES - 1)
			return 0;

		if (op == OP_TAILCALL)
			op = OP_CALL;

		R_SETARG_A(rinst, GETARG_A(i));

		switch (getOpMode(op))
		{
		case iABC:
		{
			R_SETARG_B(rinst, GETARG_B(i));
			R_SETARG_C(rinst, GETARG_C(i));
			break;
		}
		case iABx:
			R_SETARG_Bx(rinst, GETARG_Bx(i));
			break;
		case iAsBx:
			R_SETARG_sBx(rinst, GETARG_sBx(i));
			break;
		}

		switch (op)
		{
		case OP_MOVE:
			R_SETARG_C(rinst, 1);
			break;
		case OP_SETUPVAL:
			rinst = setupvalEnc(rinst, pc);
			break;
		case OP_JMP:
			rinst = jmpEnc(rinst, pc);
			break;
		case OP_CALL:
			rinst = callEncryption(rinst, pc);
			break;
		case OP_CLOSURE:
			rinst = closureEncryption(rinst, pc);
			break;
		case OP_RETURN:
			rinst = returnEncryption(rinst, pc);
			break;
		}

		R_SET_OPCODE(rinst, translateOpCode[op]);
		return rinst * ckey;
	};
};

static unsigned int encryptFunc(unsigned long h, unsigned long y)
{
	unsigned long matrix[32];
	unsigned long m = 32;

	for (unsigned int i = 0; i < m; i++)
		matrix[i] = _rotl(h, i);

	for (unsigned int i = 0; i < m; i++)
	{
		// If diagonal element is not one
		if (!(matrix[i] & 1U << i))
		{
			// Search for row with 1
			for (unsigned int j = i + 1; j < m; j++)
			{
				if (matrix[j] & 1U << i)
				{
					// Swap row
					unsigned long tmp = matrix[i];
					matrix[i] = matrix[j];
					matrix[j] = tmp;

					// Swap bit y[i] with y[j]
					y = (y & ~(1U << j | 1U << i)) | (y & 1U << j) >> (j - i) | (y & 1U << i) << (j - i);
					break;
				}
			}
		}

		// Find other rows with non-zero pivot
		for (unsigned int j = 0; j < m; j++)
		{
			if (j != i && matrix[j] & 1U << i)
			{
				// XOR row
				matrix[j] = matrix[j] ^ matrix[i];

				// Set y[k] = y[k] xor y[i]
				if ((y & 1U << j) >> j == (y & 1U << i) >> i)
				{
					y = y & ~(1U << j);	// set y[k] = 0 if y[k]==y[i]
				}
				else
				{
					y = y | (1U << j);	// set y[k] = 1 if y[k]!=y[i]
				}
			}
		}
	}

	return y;
}


uint32_t modInverse(uint64_t a, uint64_t m)
{
	uint64_t m0 = m, t, q;
	uint64_t x0 = 0, x1 = 1;
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
	return static_cast<uint32_t>(x1);
}

static uint32_t hashEncryption(uint32_t instr, uint32_t pc)
{
	return encryptFunc(1 | _rotl(1, 7) | _rotr(1, 11), instr)* modInverse((pc | 1), 4294967296);
}

static uint32_t hashOpCodes(Proto * p, uint32_t decodeKey, uint32_t k)
{
	Instruction* code = p->code + ((k + 4) % (p->sizecode - 4));
	return R_GET_OPCODE(code[0] * decodeKey) + R_GET_OPCODE(code[1] * decodeKey)
		+ R_GET_OPCODE(code[2] * decodeKey) + R_GET_OPCODE(code[3] * decodeKey);
}

static uint32_t generateHash(Proto * p, uint32_t decodeKey)
{
	uint32_t protoHash;

	uint32_t last4ByteShuffle = p->numparams + (p->nups + (p->is_vararg + (p->maxstacksize << 8) << 8) << 8);

	uint32_t hashConstant1 = 0xACD542BB * p->sizek - 0x6D991385 * last4ByteShuffle - 0x76D62959 * p->sizep - 0x43111B53 * p->sizecode;
	uint32_t hashConstant2 = 0x1C5FB920 * p->sizek + 0x5C326D8F * p->sizep - 0x2A18BD67 * p->sizecode - 0x7C78D027 * last4ByteShuffle;

	if (p->sizecode <= 8)
	{
		protoHash = hashConstant2 ^ hashConstant1;
	}
	else
	{
		protoHash = hashOpCodes(p, decodeKey, hashConstant1) + hashConstant1;
		hashConstant2 += hashOpCodes(p, decodeKey, hashConstant2);
	}

	return hashEncryption(protoHash, hashConstant2);
}
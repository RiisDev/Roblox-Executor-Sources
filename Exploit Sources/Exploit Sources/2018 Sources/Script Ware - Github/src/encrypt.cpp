#include "encrypt.h"
#include "VMProtectSDK.h"
#include "convert.h"

/* behold! elysians encryption and eternals shit non functional enc */

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

/* Encrypt op_jmp & op_setupval */
unsigned int encrypt_func(unsigned int instr, unsigned int pc, unsigned int init, unsigned int constant, unsigned int mask)
{
	unsigned int matrix[32];

	unsigned int result = instr ^ ((constant * pc) & mask);
	RSET_OPCODE(result, RGET_OPCODE(instr));

	for (int elm = 0; elm < 32; elm++)
		matrix[elm] = _rotl(1 | init, elm);

	for (int cBit = 0; cBit < 32; cBit++)
	{
		unsigned int cBitMask = (1 << cBit);

		/* If the bit is not set */
		if (!(matrix[cBit] & cBitMask))
		{
			/* Find next bit that _does_ have that bit set */
			for (int nBit = cBit + 1; nBit < 32; nBit++)
			{
				unsigned int nBitMask = (1 << nBit);

				if (matrix[nBit] & cBitMask)
				{
					/* Swap matrix values */
					matrix[cBit] ^= matrix[nBit];
					matrix[nBit] ^= matrix[cBit];
					matrix[cBit] ^= matrix[nBit];

					/* Calcualte mask for swap */
					unsigned int rot = nBit - cBit;
					unsigned int nBitSwap = _rotr(result & nBitMask, rot);
					unsigned int cBitSwap = _rotl(result & cBitMask, rot);

					/* Clean bits and swap */
					result &= ~(nBitMask | cBitMask);
					result |= nBitSwap;
					result |= cBitSwap;
					break;
				}
			}
		}

		for (int nRow = 0; nRow < 32; nRow++)
		{
			unsigned int nBitMask = (1 << nRow);

			/* If it is not the current row and the
			correct bit is set set or clear bit
			depending on if the same bit is set */
			if (nRow != cBit && matrix[nRow] & cBitMask)
			{
				matrix[nRow] ^= matrix[cBit];

				unsigned int cRowBit = _rotr(result & nBitMask, nRow);
				unsigned int resBit = _rotr(result & cBitMask, cBit);

				/* Clear bit */
				if (cRowBit == resBit)
					result &= ~nBitMask;
				/* Set bit */
				else
					result |= nBitMask;
			}
		}
	}

	return result;
}

/* Decrypt op_jmp instruction for bruteforcer comparison */
unsigned int jumpDecryption(unsigned int instr, unsigned int pc)
{
	unsigned int sBxkey = (-10065 * pc & 0x3FFFF) ^ instr;
	unsigned int rotr = _rotr(instr, 6);
	unsigned int rotl = _rotl(instr, 9);

	unsigned int decrypted = sBxkey ^ rotr ^ rotl;
	RSET_OPCODE(decrypted, 0x14);

	return decrypted;
}

/* Encrypt op_jmp, bruteforce register A due to the lossy operations */
unsigned int jump_encryption(unsigned int instr, unsigned int pc)
{
	for (int i = 0; i <= MAXARG_A; i++)
	{
		int tempInstr = instr;
		RSETARG_A(tempInstr, i);

		int guess = encrypt_func(tempInstr, pc, _rotl(1, 13) | _rotr(1, 7), -10065, MASK1(SIZE_Bx, 0));

		if (tempInstr == jumpDecryption(guess, pc))
			return guess;
	}

	return 0;
}

/* Decrypt op_setupval instruction for bruteforcer comparison */
unsigned int setupval_decryption(unsigned int instr, unsigned int pc)
{
	unsigned int AB_key = instr ^ ((0x961C86 * pc) & (MASK1(SIZE_A, 0) | MASK1(SIZE_B, 0)));
	unsigned int rotr = _rotr(instr, 13);
	unsigned int rotl = _rotl(instr, 7);

	unsigned int decrypted = AB_key ^ rotr ^ rotl;
	RSET_OPCODE(decrypted, 0x5);

	return decrypted;
}

/* Encrypt op_setupval, bruteforce register C due to the lossy operations */
unsigned int setupval_encryption(unsigned int instr, unsigned int pc)
{
	for (int c = 0; c <= MAXARG_C; c++)
	{
		int temp_instr = instr;
		RSETARG_C(temp_instr, c);

		int guess = encrypt_func(temp_instr, pc, _rotl(1, 13) | _rotr(1, 7), 0x961C86, 0x3FC01FF);

		if (temp_instr == setupval_decryption(guess, pc))
			return guess;
	}

	return 0;
}
#pragma once
#include <Windows.h>
#include <iostream>

long long multiplicativeInverse(long long a, long long n);
uint32_t rbxDaxEncodeOp(uint32_t x, uint32_t mulEven, uint32_t addEven, uint32_t mulOdd, uint32_t addOdd);

#define __ROL4__(x, r) _rotl(x, r)
#define __ROR4__(x, r) _rotr(x, r)




static int  sub_6EC3D60(int a1, int a2, int a3, int a4, int a5, int a6)
{
	int v6; // edi
	signed int v7; // ecx
	signed int v8; // ecx
	int *v9; // ebx
	signed int v10; // edx
	int v11; // ecx
	int v12; // eax
	int v13; // eax
	signed int v14; // ebx
	signed int v15; // eax
	int v16; // esi
	int v18[32]; // [esp+Ch] [ebp-88h]
	int v19; // [esp+8Ch] [ebp-8h]
	int *v20; // [esp+90h] [ebp-4h]
	signed int v21; // [esp+9Ch] [ebp+8h]
	signed int v22; // [esp+A4h] [ebp+10h]

	v19 = a1;
	v6 = a2 ^ a6 ^ a5 ^ a3 & 0x3FFFFFF;
	v7 = 0;
	do
	{
		v18[v7] = __ROL4__(a4 | 1, v7);
		++v7;
	} while (v7 < 32);
	v8 = 0;
	v9 = v18;
	v10 = 1;
	v21 = 0;
	v22 = 1;
	v20 = v18;
	do
	{
		if (!(*v9 & v10))
		{
			v11 = v8 + 1;
			if (v11 < 32)
			{
				while (1)
				{
					v12 = v18[v11];
					if (v12 & v10)
						break;
					if (++v11 >= 32)
						goto LABEL_10;
				}
				v13 = v12 ^ *v9;
				*v9 = v13;
				v18[v11] ^= v13;
				*v9 ^= v18[v11];
				v6 = __ROR4__(v6 & (1 << v11), v11 - v21) | __ROL4__(v6 & v10, v11 - v21) | ~((1 << v11) | v22) & v6;
				v10 = v22;
			}
		LABEL_10:
			v8 = v21;
		}
		v14 = 0;
		v15 = 1;
		do
		{
			if (v14 != v8)
			{
				v16 = v18[v14];
				if (v16 & v10)
				{
					v18[v14] = v16 ^ *v20;
					v8 = v21;
					if (__ROR4__(v6 & v15, v14) == __ROR4__(v6 & v10, v21))
					{
						v6 &= ~v15;
						v8 = v21;
					}
					else
					{
						v6 |= v15;
					}
				}
			}
			v10 = v22;
			++v14;
			v15 = __ROL4__(v15, 1);
		} while (v14 < 32);
		++v8;
		v9 = v20 + 1;
		v10 = __ROL4__(v22, 1);
		v21 = v8;
		++v20;
		v22 = __ROL4__(v22, 1);
	} while (v8 < 32);
	return v6 & 0x3FFFFFF | (v19 << 26);
}

static int sub_6EC3D20(int a1, int a2)
{
	return a1 ^ -10065 * a2 & 0x3FFFF ^ __ROR4__(a1, 6) ^ __ROL4__(a1, 9);
}

static int sub_6EC3EA0(int a1, int a2)
{
	int v2; // eax
	int v3; // ebx
	int v4; // esi
	int v5; // ecx
	int v7; // [esp+Ch] [ebp-4h]

	v2 = a2;
	v3 = a1 & 0x3FFFF;
	v7 = a2;
	v4 = 0;
	while (1)
	{
		v5 = sub_6EC3D20(v4 & 0x3FFFFFF | 0x50000000, v2);
		if ((v5 & 0xFC000000) == 1342177280 && v3 == (v5 & 0x3FFFF))
			return v4 & 0x3FFFFFF | 0x50000000;
		if (++v4 >= 0x3FFFFFF)
			break;
		v2 = v7;
	}
	return 0;
}

static int __fastcall jumpEnc(int a1, int a2)
{
	int v2; // ebx
	unsigned int v3; // eax
	int v4; // edi
	int v5; // esi
	int v6; // eax
	int v7; // ecx
	int v9; // [esp+Ch] [ebp-8h]
	unsigned int v10; // [esp+10h] [ebp-4h]

	v2 = 0;
	v9 = a1;
	v3 = a1 & 0xFC03FFFF;
	v4 = a2;
	v10 = a1 & 0xFC03FFFF;
	while (1)
	{
		v5 = v3 | ((unsigned __int8)v2 << 18);
		v6 = sub_6EC3D60(20, v3 | ((unsigned __int8)v2 << 18), v4, 8388672, -10065, 0x3FFFF);
		if (v5 == sub_6EC3D20(v6, v4))
			break;
		v3 = v10;
		if (++v2 > 255)
			return sub_6EC3EA0(v9, v4);
	}
	return v7;
}

static int call_enc(int a1, int a2) {
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

static int closure_enc(int a1, int a2) {
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

static int sub_6EC3D40(int a1, int a2)
{
	return a1 ^ 9837702 * a2 & 0x3FC01FF ^ __ROR4__(a1, 7) ^ __ROL4__(a1, 13);
}

static int sub_6EC3F10(unsigned int a1, int a2)
{
	unsigned int v2; // ebx
	int v3; // esi
	unsigned int v4; // eax
	int v6; // [esp+Ch] [ebp-8h]
	int v7; // [esp+10h] [ebp-4h]

	v6 = a2;
	v2 = a1 & 0x1FF;
	v3 = 0;
	v7 = (unsigned __int8)(a1 >> 18);
	while (1)
	{
		v4 = sub_6EC3D40(v3 & 0x3FFFFFF | 0x14000000, a2);
		if ((v4 & 0xFC000000) == 335544320 && (unsigned __int8)(v4 >> 18) == v7 && (v4 & 0x1FF) == v2)
			return v3 & 0x3FFFFFF | 0x14000000;
		if (++v3 >= 0x3FFFFFF)
			break;
		a2 = v6;
	}
	return 0;
}

static int return_enc(int a1, int a2)
{
	int v11;
	int v12;
	int v23;
	int v13;
	int v14;
	int v4 = a1;
	int v15;
	int v16;
	int v17;
	v11 = a2;
	v12 = 0;
	v23 = 32;
	v13 = 1;
	v14 = a2 - 29799480;
	do
	{
		v15 = (v12 * v14 + 32757935) ^ (v11 * v12 + 1470882913);
		v16 = v12 | v13;
		v11 = a2;
		if ((v13 & v15) == (v4 & v13))
			v16 = v12;
		v13 *= 2;
		v17 = v23-- == 1;
		v12 = v16;
		v14 = a2 - 1577854801;
	} while (!v17);
	return ((v4 ^ v12) & 0x3FFFFFF ^ v4);
}
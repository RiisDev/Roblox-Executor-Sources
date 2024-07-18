/*
 * File Name: Retcheck.h
 * Author(s): Chirality
 *
 * Last modified: 15-03-2018
 *
 * Copyright © 2019, Paramake Ltd
 */

#pragma once

#include <Windows.h>
#include <map>
#include <iostream>

 /*
 * Hacker Disassembler Engine 32 C
 * Copyright (c) 2008-2009, Vyacheslav Patkov.
 * All rights reserved.
 *
 */

#include <string.h>
#include <stdint.h>
#include <stdexcept>
#define F_MODRM         0x00000001
#define F_SIB           0x00000002
#define F_IMM8          0x00000004
#define F_IMM16         0x00000008
#define F_IMM32         0x00000010
#define F_DISP8         0x00000020
#define F_DISP16        0x00000040
#define F_DISP32        0x00000080
#define F_RELATIVE      0x00000100
#define F_2IMM16        0x00000800
#define F_ERROR         0x00001000
#define F_ERROR_OPCODE  0x00002000
#define F_ERROrLENGTH  0x00004000
#define F_ERROrLOCK    0x00008000
#define F_ERROR_OPERAND 0x00010000
#define F_PREFIX_REPNZ  0x01000000
#define F_PREFIX_REPX   0x02000000
#define F_PREFIX_REP    0x03000000
#define F_PREFIX_66     0x04000000
#define F_PREFIX_67     0x08000000
#define F_PREFIX_LOCK   0x10000000
#define F_PREFIX_SEG    0x20000000
#define F_PREFIX_ANY    0x3f000000

#define PREFIX_SEGMENT_CS   0x2e
#define PREFIX_SEGMENT_SS   0x36
#define PREFIX_SEGMENT_DS   0x3e
#define PREFIX_SEGMENT_ES   0x26
#define PREFIX_SEGMENT_FS   0x64
#define PREFIX_SEGMENT_GS   0x65
#define PREFIX_LOCK         0xf0
#define PREFIX_REPNZ        0xf2
#define PREFIX_REPX         0xf3
#define PREFIX_OPERAND_SIZE 0x66
#define PREFIX_ADDRESS_SIZE 0x67

#pragma pack(push,1)

typedef struct {
	uint8_t len;
	uint8_t p_rep;
	uint8_t p_lock;
	uint8_t p_seg;
	uint8_t p_66;
	uint8_t p_67;
	uint8_t opcode;
	uint8_t opcode2;
	uint8_t modrm;
	uint8_t modrm_mod;
	uint8_t modrm_reg;
	uint8_t modrm_rm;
	uint8_t sib;
	uint8_t sib_scale;
	uint8_t sib_index;
	uint8_t sib_base;
	union {
		uint8_t imm8;
		uint16_t imm16;
		uint32_t imm32;
	} imm;
	union {
		uint8_t disp8;
		uint16_t disp16;
		uint32_t disp32;
	} disp;
	uint32_t flags;
} hde32s;

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

	/* __cdecl */
	static unsigned int hde32_disasm(const void* code, hde32s* hs);

#ifdef __cplusplus
}
#endif

#define C_NONE    0x00
#define C_MODRM   0x01
#define C_IMM8    0x02
#define C_IMM16   0x04
#define C_IMM_P66 0x10
#define C_REL8    0x20
#define C_REL32   0x40
#define C_GROUP   0x80
#define C_ERROR   0xff

#define PRE_ANY  0x00
#define PRE_NONE 0x01
#define PRE_F2   0x02
#define PRE_F3   0x04
#define PRE_66   0x08
#define PRE_67   0x10
#define PRE_LOCK 0x20
#define PRE_SEG  0x40
#define PRE_ALL  0xff

#define DELTA_OPCODES      0x4a
#define DELTA_FPU_REG      0xf1
#define DELTA_FPU_MODRM    0xf8
#define DELTA_PREFIXES     0x130
#define DELTA_OP_LOCK_OK   0x1a1
#define DELTA_OP2_LOCK_OK  0x1b9
#define DELTA_OP_ONLY_MEM  0x1cb
#define DELTA_OP2_ONLY_MEM 0x1da

static unsigned char hde32_table[] = {
	0xa3,0xa8,0xa3,0xa8,0xa3,0xa8,0xa3,0xa8,0xa3,0xa8,0xa3,0xa8,0xa3,0xa8,0xa3,
	0xa8,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xac,0xaa,0xb2,0xaa,0x9f,0x9f,
	0x9f,0x9f,0xb5,0xa3,0xa3,0xa4,0xaa,0xaa,0xba,0xaa,0x96,0xaa,0xa8,0xaa,0xc3,
	0xc3,0x96,0x96,0xb7,0xae,0xd6,0xbd,0xa3,0xc5,0xa3,0xa3,0x9f,0xc3,0x9c,0xaa,
	0xaa,0xac,0xaa,0xbf,0x03,0x7f,0x11,0x7f,0x01,0x7f,0x01,0x3f,0x01,0x01,0x90,
	0x82,0x7d,0x97,0x59,0x59,0x59,0x59,0x59,0x7f,0x59,0x59,0x60,0x7d,0x7f,0x7f,
	0x59,0x59,0x59,0x59,0x59,0x59,0x59,0x59,0x59,0x59,0x59,0x59,0x9a,0x88,0x7d,
	0x59,0x50,0x50,0x50,0x50,0x59,0x59,0x59,0x59,0x61,0x94,0x61,0x9e,0x59,0x59,
	0x85,0x59,0x92,0xa3,0x60,0x60,0x59,0x59,0x59,0x59,0x59,0x59,0x59,0x59,0x59,
	0x59,0x59,0x9f,0x01,0x03,0x01,0x04,0x03,0xd5,0x03,0xcc,0x01,0xbc,0x03,0xf0,
	0x10,0x10,0x10,0x10,0x50,0x50,0x50,0x50,0x14,0x20,0x20,0x20,0x20,0x01,0x01,
	0x01,0x01,0xc4,0x02,0x10,0x00,0x00,0x00,0x00,0x01,0x01,0xc0,0xc2,0x10,0x11,
	0x02,0x03,0x11,0x03,0x03,0x04,0x00,0x00,0x14,0x00,0x02,0x00,0x00,0xc6,0xc8,
	0x02,0x02,0x02,0x02,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0xff,0xca,
	0x01,0x01,0x01,0x00,0x06,0x00,0x04,0x00,0xc0,0xc2,0x01,0x01,0x03,0x01,0xff,
	0xff,0x01,0x00,0x03,0xc4,0xc4,0xc6,0x03,0x01,0x01,0x01,0xff,0x03,0x03,0x03,
	0xc8,0x40,0x00,0x0a,0x00,0x04,0x00,0x00,0x00,0x00,0x7f,0x00,0x33,0x01,0x00,
	0x00,0x00,0x00,0x00,0x00,0xff,0xbf,0xff,0xff,0x00,0x00,0x00,0x00,0x07,0x00,
	0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0xff,0xff,0x00,0x00,0x00,0xbf,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x7f,0x00,0x00,0xff,0x4a,0x4a,0x4a,0x4a,0x4b,0x52,0x4a,0x4a,0x4a,0x4a,0x4f,
	0x4c,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x55,0x45,0x40,0x4a,0x4a,0x4a,
	0x45,0x59,0x4d,0x46,0x4a,0x5d,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,0x4a,
	0x4a,0x4a,0x4a,0x4a,0x4a,0x61,0x63,0x67,0x4e,0x4a,0x4a,0x6b,0x6d,0x4a,0x4a,
	0x45,0x6d,0x4a,0x4a,0x44,0x45,0x4a,0x4a,0x00,0x00,0x00,0x02,0x0d,0x06,0x06,
	0x06,0x06,0x0e,0x00,0x00,0x00,0x00,0x06,0x06,0x06,0x00,0x06,0x06,0x02,0x06,
	0x00,0x0a,0x0a,0x07,0x07,0x06,0x02,0x05,0x05,0x02,0x02,0x00,0x00,0x04,0x04,
	0x04,0x04,0x00,0x00,0x00,0x0e,0x05,0x06,0x06,0x06,0x01,0x06,0x00,0x00,0x08,
	0x00,0x10,0x00,0x18,0x00,0x20,0x00,0x28,0x00,0x30,0x00,0x80,0x01,0x82,0x01,
	0x86,0x00,0xf6,0xcf,0xfe,0x3f,0xab,0x00,0xb0,0x00,0xb1,0x00,0xb3,0x00,0xba,
	0xf8,0xbb,0x00,0xc0,0x00,0xc1,0x00,0xc7,0xbf,0x62,0xff,0x00,0x8d,0xff,0x00,
	0xc4,0xff,0x00,0xc5,0xff,0x00,0xff,0xff,0xeb,0x01,0xff,0x0e,0x12,0x08,0x00,
	0x13,0x09,0x00,0x16,0x08,0x00,0x17,0x09,0x00,0x2b,0x09,0x00,0xae,0xff,0x07,
	0xb2,0xff,0x00,0xb4,0xff,0x00,0xb5,0xff,0x00,0xc3,0x01,0x00,0xc7,0xff,0xbf,
	0xe7,0x08,0x00,0xf0,0x02,0x00
};


#ifdef _MSC_VER
#pragma warning(disable:4701)
#endif

unsigned int hde32_disasm(const void* code, hde32s* hs)
{
	uint8_t x, c, * p = (uint8_t*)code, cflags, opcode, pref = 0;
	uint8_t* ht = hde32_table, m_mod, m_reg, m_rm, disp_size = 0;

	memset(hs, 0, sizeof(hde32s));

	for (x = 16; x; x--)
		switch (c = *p++) {
		case 0xf3:
			hs->p_rep = c;
			pref |= PRE_F3;
			break;
		case 0xf2:
			hs->p_rep = c;
			pref |= PRE_F2;
			break;
		case 0xf0:
			hs->p_lock = c;
			pref |= PRE_LOCK;
			break;
		case 0x26: case 0x2e: case 0x36:
		case 0x3e: case 0x64: case 0x65:
			hs->p_seg = c;
			pref |= PRE_SEG;
			break;
		case 0x66:
			hs->p_66 = c;
			pref |= PRE_66;
			break;
		case 0x67:
			hs->p_67 = c;
			pref |= PRE_67;
			break;
		default:
			goto pref_done;
		}
pref_done:

	hs->flags = (uint32_t)pref << 23;

	if (!pref)
		pref |= PRE_NONE;

	if ((hs->opcode = c) == 0x0f) {
		hs->opcode2 = c = *p++;
		ht += DELTA_OPCODES;
	}
	else if (c >= 0xa0 && c <= 0xa3) {
		if (pref & PRE_67)
			pref |= PRE_66;
		else
			pref &= ~PRE_66;
	}

	opcode = c;
	cflags = ht[ht[opcode / 4] + (opcode % 4)];

	if (cflags == C_ERROR) {
		hs->flags |= F_ERROR | F_ERROR_OPCODE;
		cflags = 0;
		if ((opcode & -3) == 0x24)
			cflags++;
	}

	x = 0;
	if (cflags & C_GROUP) {
		uint16_t t;
		t = *(uint16_t*)(ht + (cflags & 0x7f));
		cflags = (uint8_t)t;
		x = (uint8_t)(t >> 8);
	}

	if (hs->opcode2) {
		ht = hde32_table + DELTA_PREFIXES;
		if (ht[ht[opcode / 4] + (opcode % 4)] & pref)
			hs->flags |= F_ERROR | F_ERROR_OPCODE;
	}

	if (cflags & C_MODRM) {
		hs->flags |= F_MODRM;
		hs->modrm = c = *p++;
		hs->modrm_mod = m_mod = c >> 6;
		hs->modrm_rm = m_rm = c & 7;
		hs->modrm_reg = m_reg = (c & 0x3f) >> 3;

		if (x && ((x << m_reg) & 0x80))
			hs->flags |= F_ERROR | F_ERROR_OPCODE;

		if (!hs->opcode2 && opcode >= 0xd9 && opcode <= 0xdf) {
			uint8_t t = opcode - 0xd9;
			if (m_mod == 3) {
				ht = hde32_table + DELTA_FPU_MODRM + t * 8;
				t = ht[m_reg] << m_rm;
			}
			else {
				ht = hde32_table + DELTA_FPU_REG;
				t = ht[t] << m_reg;
			}
			if (t & 0x80)
				hs->flags |= F_ERROR | F_ERROR_OPCODE;
		}

		if (pref & PRE_LOCK) {
			if (m_mod == 3) {
				hs->flags |= F_ERROR | F_ERROrLOCK;
			}
			else {
				uint8_t* table_end, op = opcode;
				if (hs->opcode2) {
					ht = hde32_table + DELTA_OP2_LOCK_OK;
					table_end = ht + DELTA_OP_ONLY_MEM - DELTA_OP2_LOCK_OK;
				}
				else {
					ht = hde32_table + DELTA_OP_LOCK_OK;
					table_end = ht + DELTA_OP2_LOCK_OK - DELTA_OP_LOCK_OK;
					op &= -2;
				}
				for (; ht != table_end; ht++)
					if (*ht++ == op) {
						if (!((*ht << m_reg) & 0x80))
							goto no_lock_error;
						else
							break;
					}
				hs->flags |= F_ERROR | F_ERROrLOCK;
			no_lock_error:
				;
			}
		}

		if (hs->opcode2) {
			switch (opcode) {
			case 0x20: case 0x22:
				m_mod = 3;
				if (m_reg > 4 || m_reg == 1)
					goto error_operand;
				else
					goto no_error_operand;
			case 0x21: case 0x23:
				m_mod = 3;
				if (m_reg == 4 || m_reg == 5)
					goto error_operand;
				else
					goto no_error_operand;
			}
		}
		else {
			switch (opcode) {
			case 0x8c:
				if (m_reg > 5)
					goto error_operand;
				else
					goto no_error_operand;
			case 0x8e:
				if (m_reg == 1 || m_reg > 5)
					goto error_operand;
				else
					goto no_error_operand;
			}
		}

		if (m_mod == 3) {
			uint8_t* table_end;
			if (hs->opcode2) {
				ht = hde32_table + DELTA_OP2_ONLY_MEM;
				table_end = ht + sizeof(hde32_table) - DELTA_OP2_ONLY_MEM;
			}
			else {
				ht = hde32_table + DELTA_OP_ONLY_MEM;
				table_end = ht + DELTA_OP2_ONLY_MEM - DELTA_OP_ONLY_MEM;
			}
			for (; ht != table_end; ht += 2)
				if (*ht++ == opcode) {
					if ((*ht++ & pref) && !((*ht << m_reg) & 0x80))
						goto error_operand;
					else
						break;
				}
			goto no_error_operand;
		}
		else if (hs->opcode2) {
			switch (opcode) {
			case 0x50: case 0xd7: case 0xf7:
				if (pref & (PRE_NONE | PRE_66))
					goto error_operand;
				break;
			case 0xd6:
				if (pref & (PRE_F2 | PRE_F3))
					goto error_operand;
				break;
			case 0xc5:
				goto error_operand;
			}
			goto no_error_operand;
		}
		else
			goto no_error_operand;

	error_operand:
		hs->flags |= F_ERROR | F_ERROR_OPERAND;
	no_error_operand:

		c = *p++;
		if (m_reg <= 1) {
			if (opcode == 0xf6)
				cflags |= C_IMM8;
			else if (opcode == 0xf7)
				cflags |= C_IMM_P66;
		}

		switch (m_mod) {
		case 0:
			if (pref & PRE_67) {
				if (m_rm == 6)
					disp_size = 2;
			}
			else
				if (m_rm == 5)
					disp_size = 4;
			break;
		case 1:
			disp_size = 1;
			break;
		case 2:
			disp_size = 2;
			if (!(pref & PRE_67))
				disp_size <<= 1;
			break;
		}

		if (m_mod != 3 && m_rm == 4 && !(pref & PRE_67)) {
			hs->flags |= F_SIB;
			p++;
			hs->sib = c;
			hs->sib_scale = c >> 6;
			hs->sib_index = (c & 0x3f) >> 3;
			if ((hs->sib_base = c & 7) == 5 && !(m_mod & 1))
				disp_size = 4;
		}

		p--;
		switch (disp_size) {
		case 1:
			hs->flags |= F_DISP8;
			hs->disp.disp8 = *p;
			break;
		case 2:
			hs->flags |= F_DISP16;
			hs->disp.disp16 = *(uint16_t*)p;
			break;
		case 4:
			hs->flags |= F_DISP32;
			hs->disp.disp32 = *(uint32_t*)p;
			break;
		}
		p += disp_size;
	}
	else if (pref & PRE_LOCK)
		hs->flags |= F_ERROR | F_ERROrLOCK;

	if (cflags & C_IMM_P66) {
		if (cflags & C_REL32) {
			if (pref & PRE_66) {
				hs->flags |= F_IMM16 | F_RELATIVE;
				hs->imm.imm16 = *(uint16_t*)p;
				p += 2;
				goto disasm_done;
			}
			goto rel32_ok;
		}
		if (pref & PRE_66) {
			hs->flags |= F_IMM16;
			hs->imm.imm16 = *(uint16_t*)p;
			p += 2;
		}
		else {
			hs->flags |= F_IMM32;
			hs->imm.imm32 = *(uint32_t*)p;
			p += 4;
		}
	}

	if (cflags & C_IMM16) {
		if (hs->flags & F_IMM32) {
			hs->flags |= F_IMM16;
			hs->disp.disp16 = *(uint16_t*)p;
		}
		else if (hs->flags & F_IMM16) {
			hs->flags |= F_2IMM16;
			hs->disp.disp16 = *(uint16_t*)p;
		}
		else {
			hs->flags |= F_IMM16;
			hs->imm.imm16 = *(uint16_t*)p;
		}
		p += 2;
	}
	if (cflags & C_IMM8) {
		hs->flags |= F_IMM8;
		hs->imm.imm8 = *p++;
	}

	if (cflags & C_REL32) {
	rel32_ok:
		hs->flags |= F_IMM32 | F_RELATIVE;
		hs->imm.imm32 = *(uint32_t*)p;
		p += 4;
	}
	else if (cflags & C_REL8) {
		hs->flags |= F_IMM8 | F_RELATIVE;
		hs->imm.imm8 = *p++;
	}

disasm_done:

	if ((hs->len = (uint8_t)(p - (uint8_t*)code)) > 15) {
		hs->flags |= F_ERROR | F_ERROrLENGTH;
		hs->len = 15;
	}

	return (unsigned int)hs->len;
}


//-------------------------------------------------


class Retcheck
{
public:
	static DWORD unprotect(BYTE* funcaddr)
	{
		static int total_alloc;
		static std::map<DWORD, DWORD> cache;
		try
		{
			DWORD& cached_func = cache.at((DWORD)funcaddr);
			return (DWORD)(cached_func);
		}
		catch (std::out_of_range&)
		{
		} //cache miss, do nothing and continue

		DWORD func_size = get_func_end(funcaddr) - funcaddr;
		if (!func_size)
		{
			return (DWORD)(funcaddr);
		}

		if (total_alloc + func_size > max_alloc)
			return (DWORD)(funcaddr); //failsafe, using too much memory (over 1MB)

		void* new_func = VirtualAlloc(NULL, func_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (new_func == NULL)
			return (DWORD)(funcaddr); //alloc failed

		total_alloc += func_size;
		memcpy(new_func, funcaddr, func_size);
		if (disable_retcheck((DWORD)new_func, func_size))
		{
			cache.emplace((DWORD)funcaddr, (DWORD)new_func);
			fix_calls((DWORD)new_func, (DWORD)funcaddr, func_size);
			return (DWORD)(new_func);
		}

		//no retcheck was found, abort
		VirtualFree(new_func, 0, MEM_RELEASE);
		VirtualFree(new_func, func_size, MEM_DECOMMIT);
		return (DWORD)(funcaddr);
	}

private:
	static const int max_alloc = 1024 * 1024;

	static bool is_prolog(BYTE* addr)
	{
		return addr[0] == 0x55 && addr[1] == 0x8B && addr[2] == 0xEC;
	}

	static BYTE* get_func_end(BYTE* funcaddr) //terrible
	{
		BYTE* addr = funcaddr + 0x10;
		while (true)
		{
			if (IsBadReadPtr(addr, 4))
			{
				return funcaddr;
			}
			if (is_prolog(addr))
			{
				break;
			}
			addr += 0x10;
		}
		return addr;
	}

	static bool disable_retcheck(DWORD new_func, DWORD func_len)
	{
		bool has_retcheck = false;
		DWORD disasm_ptr = new_func;
		while (disasm_ptr - new_func < func_len)
		{
			hde32s disasm = { 0 };
			hde32_disasm((void*)disasm_ptr, &disasm);
			if (disasm.opcode == 0x3B) //CMP
			{
				DWORD ptr = disasm.disp.disp32;
				if (ptr > 0xFFFF) //filter CMP instructions, disp32 should be a pointer to the bound constant
				{
					memcpy((void*)(disasm_ptr), "\xF9\x90\x90\x90\x90\x90", 6); //setc nop nop nop nop nop
					has_retcheck = true;
				}
			}
			disasm_ptr += disasm.len;
		}
		return has_retcheck;
	}

	static BYTE test_function(DWORD func)
	{
		__try
		{
			BYTE x = *(BYTE*)func;
			return x == 0 ? 1 : x;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return 0;
		}
	}

	static void fix_calls(DWORD new_func, DWORD orig_func, DWORD func_len)
	{
		DWORD disasm_ptr = new_func;
		while (disasm_ptr - new_func < func_len)
		{
			hde32s disasm = { 0 };
			hde32_disasm((void*)disasm_ptr, &disasm);
			if (disasm.opcode == 0xE8) //CALL
			{
				DWORD rel_addr = disasm.imm.imm32;
				DWORD orig_call_instr = orig_func + (disasm_ptr - new_func);
				DWORD orig_called_func = orig_call_instr + rel_addr + 5;
				if (orig_called_func % 0x10 == 0 && test_function(orig_called_func)) //functions are aligned to 0x10 bytes
				{
					__try
					{
						DWORD new_called_func = unprotect((BYTE*)orig_called_func);
						DWORD new_rel_addr = new_called_func - disasm_ptr - 5;
						*(DWORD*)(disasm_ptr + 1) = new_rel_addr;
					}
					__except (EXCEPTION_EXECUTE_HANDLER)
					{
						//ignore it
					}
				}
			}
			disasm_ptr += disasm.len;
		}
	}
};

template <class T>
static T retcheckBypass(T func)
{
	return (T)Retcheck::unprotect((BYTE*)func);
}
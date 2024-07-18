#include <map>
#include <Windows.h>



/*
Retcheck bypass by Chirality
Code starts at around line 500
*/

//-------------------------------------------------

/*
* Hacker Disassembler Engine 32 C
* Copyright (c) 2008-2009, Vyacheslav Patkov.
* All rights reserved.
*
*/

#include <string.h>
#include <stdint.h>

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
#define F_ERROR_LENGTH  0x00004000
#define F_ERROR_LOCK    0x00008000
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

typedef struct
{
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
	union
	{
		uint8_t imm8;
		uint16_t imm16;
		uint32_t imm32;
	} imm;
	union
	{
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
	unsigned int hde32_disasm(const void *code, hde32s *hs);

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


#ifdef _MSC_VER
#pragma warning(disable:4701)
#endif

unsigned int hde32_disasm(const void *code, hde32s *hs);


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
		VirtualFree(new_func, func_size, MEM_RELEASE);
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
		BYTE* addr = funcaddr;
		do
		{
			addr += 0x10;
		} while (!is_prolog(addr));
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
				if (orig_called_func % 0x10 == 0) //functions are aligned to 0x10 bytes
				{
					DWORD new_called_func = unprotect((BYTE*)orig_called_func);
					DWORD new_rel_addr = new_called_func - disasm_ptr - 5;
					*(DWORD*)(disasm_ptr + 1) = new_rel_addr;
				}
			}
			disasm_ptr += disasm.len;
		}
	}
};

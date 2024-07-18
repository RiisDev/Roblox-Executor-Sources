#pragma once
#define HOOKER_IMPLEMENTATION
#include "HLib/hooker.h"

#include <Windows.h>
#include <iostream>
#include <map>


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
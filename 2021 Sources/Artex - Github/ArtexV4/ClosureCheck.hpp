#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include "HLib/hooker.h"

void* originalCreateEventA;
uintptr_t rebase(uintptr_t base)
{
	return base - 0x400000 + reinterpret_cast<std::uintptr_t>(GetModuleHandleA(nullptr));
}

std::uintptr_t call_checkA = rebase(0x5D2170);
std::uintptr_t caller = call_checkA + 6;

std::uintptr_t __cdecl closureCheck(std::uint32_t r_L)
{
	MessageBox(0, "another hook", "yep", 0);
	return TRUE;
}

void __declspec (naked) hookClosure()
{
	MessageBox(0, "hooked!", "yes!", 0);
	__asm
	{
		mov eax, [esp]
		cmp eax, caller
		jne normal_case
		push[esp + 8]
		call closureCheck
		add esp, 8
		retn
		normal_case :
		jmp originalCreateEventA
	}
}

void initalizeClosure(BOOL hook = true)
{
	auto hooked = hooker_hotpatch((void*)CreateEventA, (void*)hookClosure);
}

void __stdcall unHookClosure()
{
	hooker_unhook(reinterpret_cast<void*>(call_checkA), initalizeClosure);
}
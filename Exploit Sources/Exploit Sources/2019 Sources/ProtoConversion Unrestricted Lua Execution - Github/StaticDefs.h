#pragma once
#include "StaticDefines.h"

extern "C" {
#include "Lua/lua.h"
#include "Lua/lauxlib.h"
#include "Lua/lualib.h"
#include "Lua/lopcodes.h"
#include "Lua/lfunc.h"
#include "Lua/lstate.h"
#include "Lua/lobject.h"

}

#define RLUA_TNONE    -1
#define RLUA_TNIL    0
#define RLUA_TLIGHTUSERDATA  1
#define RLUA_TNUMBER   2
#define RLUA_TBOOLEAN   3
#define RLUA_TSTRING   4
#define RLUA_TTHREAD   5
#define RLUA_TFUNCTION   6
#define RLUA_TTABLE    7
#define RLUA_TUSERDATA   8
#define RLUA_TPROTO    9
#define RLUA_TUPVAL    10

#define ASLR(x)(x - 0x400000 + (int)GetModuleHandleA(0))

DWORD unprotect(DWORD addr)
{
	BYTE* tAddr = (BYTE*)addr;

	/*  Calcualte the size of the function.

		In theory this will run until it hits the next
		functions prolog. It assumes all calls are aligned to
		16 bytes. (grazie katie)
	*/
	do
	{
		tAddr += 16;
	} while (!(tAddr[0] == 0x55 && tAddr[1] == 0x8B && tAddr[2] == 0xEC));

	DWORD funcSz = tAddr - (BYTE*)addr;

	/* Allocate memory for the new function */
	PVOID nFunc = VirtualAlloc(NULL, funcSz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (nFunc == NULL)
		return addr;

	/* Copy the function to the newly allocated memory */
	memcpy(nFunc, (void*)addr, funcSz);

	BYTE* pos = (BYTE*)nFunc;
	BOOL valid = false;
	do
	{
		/*  Check for the return check with the sig:
			72 ?? A1 ?? ?? ?? ?? 8B

			If the sig matches replace the the jb with a jmp.
		*/
		if (pos[0] == 0x72 && pos[2] == 0xA1 && pos[7] == 0x8B) {
			*(BYTE*)pos = 0xEB;

			DWORD cByte = (DWORD)nFunc;
			do
			{
				/*  Check if the current byte is a call, if it is,
					calculate the new relative call(s).

					*(->E8 + 1) = originalFunction - nextInstruction

					oFuncPos - Position of call in original function
						= originalFunction + (->E8 - newFunction)

					oFuncAddr - Original call location
						= oFuncPos + rel32Offset + sizeof(call)

					relativeAddr - New relative address
						= oFuncAddr - ->E8 - sizeof(call)

					Since we are not using a disassembler we assume
					that if we hit a E8 byte which is properly aligned
					it is a relative call.
					For a small amount of compensation I skip the location
					of the call, since it is possible to have the byte
					E8 inside of it.
				*/
				if (*(BYTE*)cByte == 0xE8)
				{
					DWORD oFuncPos = addr + (cByte - (DWORD)nFunc);
					DWORD oFuncAddr = (oFuncPos + *(DWORD*)(oFuncPos + 1)) + 5;

					if (oFuncAddr % 16 == 0)
					{
						DWORD relativeAddr = oFuncAddr - cByte - 5;
						*(DWORD*)(cByte + 1) = relativeAddr;

						/* Don't check rel32 */
						cByte += 4;
					}
				}

				cByte += 1;
			} while (cByte - (DWORD)nFunc < funcSz);

			valid = true;
		}
		pos += 1;
	} while ((DWORD)pos < (DWORD)nFunc + funcSz);

	/* This function has no return check, let's not waste memory */
	if (!valid)
	{
		VirtualFree(nFunc, 0, MEM_RELEASE);
		return addr;
	}

	return (DWORD)nFunc;
}


_rluaM_realloc__Def rluaM_realloc_ = (_rluaM_realloc__Def)ASLR(0x007EB020);
_r_luaS_newlstr__Def rluaS_newlstr = (_r_luaS_newlstr__Def)ASLR(0x007EB690);
_spawn_Def spawn = (_spawn_Def)ASLR(0x007D7A40);
_r_lua_newThread__Def rlua_newthread = (_r_lua_newThread__Def)unprotect(ASLR(0x007F0750));


void rluaC_link(uintptr_t rL, int o, lu_byte tt)
{
	std::uintptr_t g = (rL + 20) ^ *(std::uint32_t*)(rL + 20); // thread->l_G
	lu_byte currentwhite = *(lu_byte*)(g + 20); // g->currentwhite
	*(std::uint32_t*)o = *(std::uint32_t*)(g + 36); // o->gch.next
	*(std::uint32_t*)(g + 36) = o; // g->rootgc
	*(lu_byte*)(o + 4) = currentwhite & 3; // o->gch.marked
	*(lu_byte*)(o + 5) = tt; // o->gch.tt
}

int rluaF_newproto(intptr_t rL)
{
	int proto = (int)rluaM_realloc_(rL, 0, 0, 80, 6);
	rluaC_link(rL, proto, 9);

	*(int*)(proto + 8) = proto + 8;
	*(int*)(proto + 40) = 0;
	*(int*)(proto + 32) = proto + 32;
	*(int*)(proto + 56) = 0;
	*(int*)(proto + 36) = proto + 36;
	*(int*)(proto + 48) = 0;
	*(int*)(proto + 12) = proto + 12;
	*(int*)(proto + 64) = 0;
	*(int*)(proto + 76) = 0;
	*(int*)(proto + 24) = proto + 24;
	*(int*)(proto + 28) = 0;
	*(int*)(proto + 60) = 0;
	*(int*)(proto + 16) = proto + 16;
	*(int*)(proto + 44) = 0;
	*(int*)(proto + 72) = 0;
	*(int*)(proto + 78) = 0;
	*(int*)(proto + 77) = 0;
	*(int*)(proto + 79) = 0;
	*(byte*)(proto + 68) = 0;
	return proto;
}

int rluaf_newLClosure(unsigned long rL, int closure, int env)
{
	int roblox = (int)rluaM_realloc_(rL, 0, 0, 4 * closure + 20, 4);
	rluaC_link(rL, roblox, 6);
	*(int*)(roblox + 5) = false;
	*(int*)(roblox + 12) = env;
	*(int*)(roblox + 7) = closure;
	return roblox;
}


VOID rlua_pushlclosure(int rl, DWORD rlcl) {
	int top = (int)(rl + 24);
	*(int*)(top + 8) = RLUA_TFUNCTION;
	*(int*)(top) = rlcl;
	*(int*)(rl + 24) += 16;
}
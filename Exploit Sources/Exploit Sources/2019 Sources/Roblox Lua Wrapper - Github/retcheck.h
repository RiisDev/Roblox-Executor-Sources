#pragma once
#include <Windows.h>

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
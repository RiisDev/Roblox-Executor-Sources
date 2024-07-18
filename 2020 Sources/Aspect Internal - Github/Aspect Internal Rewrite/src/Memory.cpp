#include "Memory.h"
#include <Psapi.h>
#include <Windows.h>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>

#pragma comment(lib, "Psapi.lib")

namespace {
const char* hex = "0123456789ABCDEF";

bool Compare(const std::vector<unsigned char>& bytes, const std::string& mask,
    const std::uintptr_t start)
{
    for (auto i = 0; i < bytes.size(); i++) {
        if (*reinterpret_cast<unsigned char*>(start + i) != bytes[i] && mask[i] == 'x') {
            return false;
        }
    }
    return true;
}

std::vector<std::string> Split(const std::string& str)
{
    std::istringstream buf(str);
    std::istream_iterator<std::string> beg(buf), end;

    return std::vector<std::string>(beg, end);
}
} // namespace

namespace Memory {
std::uintptr_t ResolveRelativeCall(const std::uintptr_t loc)
{
    return *reinterpret_cast<std::uintptr_t*>(loc + 1) + loc + 5;
}

std::uintptr_t Scan(const std::uintptr_t start, std::uintptr_t end,
    const std::string& pattern, const int flags,
    const int offset)
{
    std::vector<unsigned char> bytes;
    std::string mask;

    if (end == 0) {
        MODULEINFO mi;
        GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &mi,
            sizeof(MODULEINFO));
        end = reinterpret_cast<std::uintptr_t>(mi.lpBaseOfDll) + mi.SizeOfImage;
        if (flags & SCAN_REVERSE) {
            end -= mi.SizeOfImage;
        }
    }

    for (const auto& str : Split(pattern)) {
        if (str == "?") {
            bytes.push_back(0);
            mask += "?";
        } else if (str.length() == 2) {
            const auto low1 = std::lower_bound(hex, hex + 16, str[0]);
            const auto low2 = std::lower_bound(hex, hex + 16, str[1]);
            bytes.push_back(((low1 - hex) << 4) | (low2 - hex));
            mask += "x";
        }
    }

    std::uintptr_t result;

    if (flags & SCAN_REVERSE) {
        for (auto ptr = start; ptr > end; ptr--) {
            if (Compare(bytes, mask, ptr)) {
                result = ptr;
                break;
            }
        }
    } else {
        for (auto ptr = start; ptr < end; ptr++) {
            if (Compare(bytes, mask, ptr)) {
                result = ptr;
                break;
            }
        }
    }

    result += offset;

    if (flags & CALL_REL_32) {
        result = ResolveRelativeCall(result);
    }

    return result;
}

std::uintptr_t Scan(const std::string& pattern, const int flags,
    const int offset)
{
    MODULEINFO mi;
    GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &mi,
        sizeof(MODULEINFO));
    return Scan(reinterpret_cast<std::uintptr_t>(mi.lpBaseOfDll),
        reinterpret_cast<std::uintptr_t>(mi.lpBaseOfDll) + mi.SizeOfImage,
        pattern, flags, offset);
}

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
		if (pos[0] == 0x72 && pos[2] == 0xA1 && pos[7] == 0x8B) {
			*(BYTE*)pos = 0xEB;

			DWORD cByte = (DWORD)nFunc;
			do
			{
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
} // namespace Memory

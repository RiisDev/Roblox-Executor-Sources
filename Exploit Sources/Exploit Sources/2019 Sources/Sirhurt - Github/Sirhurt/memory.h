#include <Psapi.h>

namespace Memory
{
	// Credits to Austin

	bool compare(const char* location, const char* pattern, const char* mask)
	{
		for (; *mask; ++pattern, ++mask, ++location)
		{
			if (*mask == 'x' && *location != *pattern)
			{
				return false;
			}
		}
		return true;
	}
	uintptr_t scan(const char* pattern, const char* mask, uintptr_t start, uintptr_t end)
	{
		for (; start <= end; ++start)
		{
			if (compare((char*)start, (char*)pattern, mask))
			{
				return start;
			}
		}
		return 0;
	}
	uintptr_t scan(const char* pattern, const char* mask)
	{
		MODULEINFO moduleInfo;
		if (GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &moduleInfo, sizeof(moduleInfo)))
			return scan(pattern, mask, (uintptr_t)moduleInfo.lpBaseOfDll, moduleInfo.SizeOfImage + (uintptr_t)moduleInfo.lpBaseOfDll);
		return 0;
	}
	uintptr_t scan(char* pattern, size_t start = 0x400000, size_t end = 0xffffffff)
	{
		MEMORY_BASIC_INFORMATION mbi;
		char* buffer = reinterpret_cast<char*>(malloc(1024));
		while (start < end)
		{
			VirtualQueryEx(GetCurrentProcess(), (void*)start, &mbi, sizeof(mbi));
			if ((mbi.AllocationProtect & 238) && !(mbi.Protect & 257) && (mbi.State & 4096) && (mbi.Protect & (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)))
			{
				for (char* i = (char*)mbi.BaseAddress; i < (char*)mbi.BaseAddress + mbi.RegionSize;) {
					SIZE_T bytesRead;
					ReadProcessMemory(GetCurrentProcess(), i, buffer, 1024, &bytesRead);
					for (int pad = 0; pad < bytesRead; pad++)
					{
						if (*(uintptr_t*)(buffer + pad) == *(uintptr_t*)pattern) {
							return (uintptr_t)(i + pad);
						}
					}
					i += bytesRead;
				}
			}
			start += mbi.RegionSize;
		}
		return 0;
	}
}

namespace MemUtil
{
	void Write(void* address, byte* bytes, signed int len)
	{
		DWORD dwOldProtect;
		VirtualProtectEx(GetCurrentProcess(), address, len, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		memcpy(address, bytes, len);
		VirtualProtectEx(GetCurrentProcess(), address, len, dwOldProtect, &dwOldProtect);
	}
}
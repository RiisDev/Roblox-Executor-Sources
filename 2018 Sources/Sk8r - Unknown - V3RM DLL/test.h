namespace Memory {
	BOOL compare(const BYTE* location, const BYTE* aob, const char* mask) {
		for (; mask; ++aob, ++mask, ++location) {
			try {
				if (mask == 'x' && *location != *aob)
					return 0;
			}
			except(EXCEPTION_EXECUTE_HANDLER) {
				return 0;
			}
		}
		return 1;
	}

	DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE bMask, char szMask)
	{
		for (int i = 0; i < (int)dwLen; i++)
			if (compare((BYTE)(dwAddress + (int)i), bMask, szMask))  return (int)(dwAddress + i);
		return 0;
	}

	int Scan(char content)
	{
		DWORD PageSize;
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		PageSize = si.dwPageSize;
		MEMORY_BASIC_INFORMATION mi;
		for (DWORD lpAddr = (DWORD)GetModuleHandle(NULL); lpAddr < 0xF000000; lpAddr += PageSize)
		{
			DWORD vq = VirtualQuery((void*)lpAddr, &mi, PageSize);
			if (vq == ERROR_INVALID_PARAMETER || vq == 0) break;
			if (mi.Type == MEM_MAPPED) continue;
			if (mi.Protect == PAGE_READWRITE)
			{
				int addr = FindPattern(lpAddr, PageSize, (PBYTE)content, "xxxx");
				if (addr != 0)
				{
					return addr;
				}
			}
		}

	}
}
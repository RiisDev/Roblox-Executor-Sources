namespace Scanner {
	//This is not my scanner, I don't scan in my personal wrappers but hook gettop. But for the sake of this wrapper ill scan.
	bool Compare(const char* pData, const char* bMask, const char* szMask) {
		while (*szMask) {
			if (*szMask != '?') {
				if (*pData != *bMask) {
					return 0;
				};
			};
			++szMask, ++pData, ++bMask;
		};
		return 1;
	};


	DWORD ScanForScriptContext(const char* ScriptContextVFTableOffsetted) {
		MEMORY_BASIC_INFORMATION MemoryBasicInformation = { 0 };
		SYSTEM_INFO SystemInfo = { 0 };
		GetSystemInfo(&SystemInfo);
		DWORD Start = (DWORD)SystemInfo.lpMinimumApplicationAddress;
		DWORD End = (DWORD)SystemInfo.lpMaximumApplicationAddress;
		do {
			while (VirtualQuery((void*)Start, &MemoryBasicInformation, sizeof(MemoryBasicInformation))) {
				if ((MemoryBasicInformation.Protect & PAGE_READWRITE) && !(MemoryBasicInformation.Protect & PAGE_GUARD)) {
					for (DWORD i = (DWORD)(MemoryBasicInformation.BaseAddress); i - (DWORD)(MemoryBasicInformation.BaseAddress) < MemoryBasicInformation.RegionSize; ++i) {
						if (Compare((const char*)i, ScriptContextVFTableOffsetted, "xxxx"))
							return i;
					};
				};
				Start += MemoryBasicInformation.RegionSize;
			};
		} while (Start < End);
		return 0;
	};
}

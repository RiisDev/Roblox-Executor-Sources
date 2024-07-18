#pragma once

inline long SKID(long SkId) {
	return (SkId - 0x400000 + (DWORD)GetModuleHandle(0));
}

namespace SkId {
	bool Skidpare(const char* pskid, const char* bskid, const char* szskid)
	{
		while (*szskid) {
			if (*szskid != '?') {
				if (*pskid != *bskid) return 0;
			}
			++szskid, ++pskid, ++bskid;
		}
		return 1;
	}

	DWORD ScanforSkid(const char* vfskid)
	{
		MEMORY_BASIC_INFORMATION SKIDI = { 0 };
		SYSTEM_INFO SkId = { 0 };
		GetSystemInfo(&SkId);
		DWORD StartSkId = (DWORD)SkId.lpMinimumApplicationAddress;
		DWORD EndSkId = (DWORD)SkId.lpMaximumApplicationAddress;
		do
		{
			while (VirtualQuery((void*)StartSkId, &SKIDI, sizeof(SKIDI))) {
				if ((SKIDI.Protect & PAGE_READWRITE) && !(SKIDI.Protect & PAGE_GUARD))
				{
					for (DWORD i = (DWORD)(SKIDI.BaseAddress); i - (DWORD)(SKIDI.BaseAddress) < SKIDI.RegionSize; ++i)
					{
						if (Skidpare((const char*)i, vfskid, "xxxx"))
							return i;
					}
				}
				StartSkId += SKIDI.RegionSize;
			}
		} while (StartSkId < EndSkId);
			return 0;
	}
}
#include "PEParser32.hpp"

PEParser32::PEParser32(uint32_t uiData) : m_BaseData(uiData)
{
	auto* const pidhHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(this->m_BaseData);

	if (pidhHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		throw std::exception("Failed to parse DOS Header!");
	}

	m_NTHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(
		reinterpret_cast<uint32_t>(pidhHeader) +
		static_cast<uint32_t>(pidhHeader->e_lfanew)
		);

	if (m_NTHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		throw std::exception("Failed to parse NT Header!");
	}
}

PIMAGE_SECTION_HEADER PEParser32::GetSectionHeader(const char* cSectionName) const
{
	const PIMAGE_SECTION_HEADER pisHeader = IMAGE_FIRST_SECTION(this->m_NTHeader);
	DWORD dwOldProtection = 0;

	VirtualProtect(static_cast<LPVOID>(pisHeader), 1, PAGE_EXECUTE_READWRITE, &dwOldProtection);

	for (int i = 0; i < this->m_NTHeader->FileHeader.NumberOfSections; ++i)
	{
		if (strcmp(reinterpret_cast<char const*>(static_cast<PIMAGE_SECTION_HEADER>(pisHeader + i)->Name),
			cSectionName) == 0)
		{
			VirtualProtect(static_cast<LPVOID>(pisHeader), 1, dwOldProtection, &dwOldProtection);
			return static_cast<PIMAGE_SECTION_HEADER>(pisHeader + i);
		}
	}

	return pisHeader;
}

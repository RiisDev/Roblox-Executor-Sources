#include <Windows.h>
#include <cstdint>
#include <iostream>
#include <exception>


class PEParser32
{
public:

	PEParser32(uint32_t uiData);
	PIMAGE_SECTION_HEADER GetSectionHeader(const char* cSectionName) const;

private:
	uint32_t m_BaseData;
	PIMAGE_NT_HEADERS m_NTHeader;
};
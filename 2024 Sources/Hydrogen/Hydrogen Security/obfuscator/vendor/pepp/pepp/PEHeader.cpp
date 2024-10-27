#include "PELibrary.hpp"

using namespace pepp;

// Explicit templates.
template class PEHeader<32>;
template class PEHeader<64>;

template<unsigned int bitsize>
inline PEHeader<bitsize>::PEHeader()
	: m_image(nullptr)
{
}


template<unsigned int bitsize>
std::uint32_t PEHeader<bitsize>::calcSizeOfImage()
{
	std::uint32_t dwLowestRva{ 0 };
	std::uint32_t dwHighestRva{ 0 };

	for (std::uint16_t n = 0; n < getFileHdr().getNumberOfSections(); n++) {
		//
		// Skip sections with bad Misc.VirtualSize
		if (m_image->m_rawSectionHeaders[n].getVirtualSize() == 0)
			continue;
		//
		// Fill in high/low rvas if possible.
		if (m_image->m_rawSectionHeaders[n].getVirtualAddress() < dwLowestRva)
			dwLowestRva = 
				m_image->m_rawSectionHeaders[n].getVirtualAddress();
		if (m_image->m_rawSectionHeaders[n].getVirtualAddress() > dwHighestRva)
			dwHighestRva = 
				m_image->m_rawSectionHeaders[n].getVirtualAddress() + m_image->m_rawSectionHeaders[n].getVirtualSize();
	}

	return (dwHighestRva - dwLowestRva);
}

template<unsigned int bitsize>
std::uint32_t PEHeader<bitsize>::getStartOfCode()
{
	return m_OptionalHeader.getBaseOfCode();
}

template<unsigned int bitsize>
std::uint32_t PEHeader<bitsize>::getNextSectionOffset()
{
	std::uint16_t nlastSecIdx = getFileHdr().getNumberOfSections() - 1;
	SectionHeader const& sec = getSectionHeader(nlastSecIdx);
	std::uint32_t uNextOffset = sec.getPtrToRawData() + sec.getSizeOfRawData();

	/*
	* FileAlignment
	* The alignment of the raw data of sections in the image file, in bytes.
	*/
	return align(uNextOffset, getOptionalHdr().getFileAlignment());
}

template<unsigned int bitsize>
std::uint32_t PEHeader<bitsize>::getNextSectionRva()
{
	std::uint16_t nlastSecIdx = getFileHdr().getNumberOfSections() - 1;
	SectionHeader const& sec = getSectionHeader(nlastSecIdx);
	std::uint32_t uNextRva = sec.getVirtualAddress() + sec.getVirtualSize();

	/*
	* SectionAlignment
	* The alignment of sections loaded in memory, in bytes.
	*/
	return align(uNextRva, getOptionalHdr().getSectionAlignment());
}

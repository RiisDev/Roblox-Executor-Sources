#include "PELibrary.hpp"

using namespace pepp;

// Explicit templates.
template class OptionalHeader<32>;
template class OptionalHeader<64>;

template<unsigned int bitsize>
inline OptionalHeader<bitsize>::OptionalHeader()
{
}

template<unsigned int bitsize>
void OptionalHeader<bitsize>::setMagic(PEMagic magic)
{
	m_base->Magic = static_cast<std::uint16_t>(magic);
}

template<unsigned int bitsize>
PEMagic OptionalHeader<bitsize>::getMagic() const
{
	return static_cast<PEMagic>(m_base->Magic);
}


template<unsigned int bitsize>
void OptionalHeader<bitsize>::setImageBase(detail::Image_t<bitsize>::Address_t address)
{
	m_base->ImageBase = address;
}

template<unsigned int bitsize>
detail::Image_t<bitsize>::Address_t OptionalHeader<bitsize>::getImageBase() const
{
	return m_base->ImageBase;
}

template<unsigned int bitsize>
void OptionalHeader<bitsize>::setSizeOfImage(std::uint32_t size)
{
	m_base->SizeOfImage = size;
}

template<unsigned int bitsize>
std::uint32_t OptionalHeader<bitsize>::getSizeOfImage() const
{
	return m_base->SizeOfImage;
}

template<unsigned int bitsize>
void OptionalHeader<bitsize>::setSizeOfCode(std::uint32_t dwSize)
{
	m_base->SizeOfCode = dwSize;
}

template<unsigned int bitsize>
std::uint32_t OptionalHeader<bitsize>::getSizeOfCode() const
{
	return m_base->SizeOfCode;
}

template<unsigned int bitsize>
void OptionalHeader<bitsize>::setSizeOfInitializedData(std::uint32_t dwSize)
{
	m_base->SizeOfInitializedData = dwSize;
}

template<unsigned int bitsize>
std::uint32_t OptionalHeader<bitsize>::getSizeOfInitializedData() const
{
	return m_base->SizeOfInitializedData;
}

template<unsigned int bitsize>
void pepp::OptionalHeader<bitsize>::setSizeOfHeaders(std::uint32_t dwSize)
{
	m_base->SizeOfHeaders = dwSize;
}

template<unsigned int bitsize>
std::uint32_t pepp::OptionalHeader<bitsize>::getSizeOfHeaders() const
{
	return m_base->SizeOfHeaders;
}

template<unsigned int bitsize>
void OptionalHeader<bitsize>::setSizeOfUninitializedData(std::uint32_t dwSize)
{
	m_base->SizeOfUninitializedData = dwSize;
}

template<unsigned int bitsize>
std::uint32_t OptionalHeader<bitsize>::getSizeOfUninitializedData() const
{
	return m_base->SizeOfUninitializedData;
}

template<unsigned int bitsize>
void OptionalHeader<bitsize>::setBaseOfCode(std::uint32_t dwBase)
{
	m_base->BaseOfCode = dwBase;
}

template<unsigned int bitsize>
std::uint32_t OptionalHeader<bitsize>::getBaseOfCode() const
{
	return m_base->BaseOfCode;
}

template<unsigned int bitsize>
void OptionalHeader<bitsize>::setAddressOfEntryPoint(std::uint32_t dwBase)
{
	m_base->AddressOfEntryPoint = dwBase;
}

template<unsigned int bitsize>
std::uint32_t OptionalHeader<bitsize>::getAddressOfEntryPoint() const
{
	return m_base->AddressOfEntryPoint;
}

template<unsigned int bitsize>
std::uint32_t OptionalHeader<bitsize>::getFileAlignment() const
{
	return m_base->FileAlignment;
}

template<unsigned int bitsize>
std::uint32_t OptionalHeader<bitsize>::getSectionAlignment() const
{
	return m_base->SectionAlignment;
}

template<unsigned int bitsize>
bool OptionalHeader<bitsize>::hasRelocations() const
{
	return m_base->DataDirectory[DIRECTORY_ENTRY_BASERELOC].Size > 0;
}

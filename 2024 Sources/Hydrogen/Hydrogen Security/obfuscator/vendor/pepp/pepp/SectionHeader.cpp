#include "PELibrary.hpp"

using namespace pepp;

void SectionHeader::setName(std::string_view name)
{
	std:memcpy(m_base.Name, name.data(), name.size());
	m_base.Name[name.size()] = '\0';
}

std::string SectionHeader::getName() const
{
	char szData[9];
	std::memcpy(szData, m_base.Name, sizeof m_base.Name);
	szData[8] = '\0';
	return szData;
}

std::uint32_t SectionHeader::getFileAddress() const
{
	return m_base.Misc.PhysicalAddress;
}

void SectionHeader::setFileAddress(std::uint32_t fileAddress)
{
	m_base.Misc.PhysicalAddress = fileAddress;
}

std::uint32_t SectionHeader::getVirtualSize() const
{
	return m_base.Misc.VirtualSize;
}

void SectionHeader::setVirtualSize(std::uint32_t virtualSize)
{
	m_base.Misc.VirtualSize = virtualSize;
}

std::uint32_t SectionHeader::getVirtualAddress() const
{
	return m_base.VirtualAddress;
}

void SectionHeader::setVirtualAddress(std::uint32_t va)
{
	m_base.VirtualAddress = va;
}

std::uint32_t SectionHeader::getSizeOfRawData() const
{
	return m_base.SizeOfRawData;
}

void SectionHeader::setSizeOfRawData(std::uint32_t sz)
{
	m_base.SizeOfRawData = sz;
}

std::uint32_t SectionHeader::getPtrToRawData() const
{
	return m_base.PointerToRawData;
}

void SectionHeader::setPointerToRawData(std::uint32_t ptr)
{
	m_base.PointerToRawData = ptr;
}

std::uint32_t SectionHeader::getPtrToRelocations() const
{
	return m_base.PointerToRelocations;
}

void SectionHeader::setPtrToRelocations(std::uint32_t ptr)
{
	m_base.PointerToRelocations = ptr;
}

std::uint32_t SectionHeader::getPtrToLinenumbers() const
{
	return m_base.PointerToLinenumbers;
}

void SectionHeader::setPtrToLinenumbers(std::uint32_t ptr)
{
	m_base.PointerToLinenumbers = ptr;
}

std::uint16_t SectionHeader::getNumberOfRelocations() const
{
	return m_base.NumberOfRelocations;
}

void SectionHeader::setNumberOfRelocations(std::uint16_t num)
{
	m_base.NumberOfRelocations = num;
}

std::uint16_t SectionHeader::getNumberOfLinenumbers() const
{
	return m_base.NumberOfLinenumbers;
}

void SectionHeader::setNumberOfLinenumbers(std::uint16_t num)
{
	m_base.NumberOfLinenumbers = num;
}

std::uint32_t SectionHeader::getCharacteristics() const
{
	return m_base.Characteristics;
}

void SectionHeader::setCharacteristics(std::uint32_t chars)
{
	m_base.Characteristics = chars;
}

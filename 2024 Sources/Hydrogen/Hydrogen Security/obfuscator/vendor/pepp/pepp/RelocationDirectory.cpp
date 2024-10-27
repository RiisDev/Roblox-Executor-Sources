#include "PELibrary.hpp"

using namespace pepp;

// Explicit templates.
template class RelocationDirectory<32>;
template class RelocationDirectory<64>;

template<unsigned int bitsize>
int RelocationDirectory<bitsize>::getNumBlocks() const
{
	auto base = m_base;
	int count = 0;

	while (base->VirtualAddress)
	{
		count++;
		base = decltype(base)((char*)base + base->SizeOfBlock);
	}

	return count;
}

template<unsigned int bitsize>
int RelocationDirectory<bitsize>::getNumEntries(detail::Image_t<>::RelocationBase_t* reloc) const
{
	// MSDN: The Block Size field is then followed by any number of Type or Offset field entries.
	// Each entry is a WORD (2 bytes)
	return (reloc->SizeOfBlock - sizeof(decltype(*reloc))) / sizeof(std::uint16_t);
}

template<unsigned int bitsize>
std::uint32_t RelocationDirectory<bitsize>::getRemainingFreeBytes() const
{
	auto base = m_base;
	std::uint32_t count = 0;

	while (base->VirtualAddress)
	{
		count += base->SizeOfBlock;
		base = decltype(base)((char*)base + base->SizeOfBlock);
	}

	return std::max<std::uint32_t>(m_section->getVirtualSize() - count, 0);
}

template<unsigned int bitsize>
bool pepp::RelocationDirectory<bitsize>::changeRelocationType(std::uint32_t rva, RelocationType type)
{
	auto base = m_base;
	std::vector<BlockEntry> entries;

	while (base->VirtualAddress)
	{
		int numEntries = getNumEntries(base);
		std::uint16_t* entry = (std::uint16_t*)(base + 1);

		for (int i = 0; i != numEntries; i++, entry++)
		{
			BlockEntry block(base->VirtualAddress, *entry);
			if (block.getRva() == rva)
			{
				*entry = craftRelocationBlockEntry(type, block.getOffset());
				return true;
			}
		}

		base = decltype(base)((char*)base + base->SizeOfBlock);
	}

	return false;
}

template<unsigned int bitsize>
std::vector<BlockEntry> RelocationDirectory<bitsize>::getBlockEntries(int blockIdx)
{
	auto base = m_base;
	int count = 0;
	std::vector<BlockEntry> entries;

	while (base->VirtualAddress)
	{
		if (count == blockIdx)
		{
			int numEntries = getNumEntries(base);
			std::uint16_t* entry = (std::uint16_t*)(base + 1);

			for (int i = 0; i != numEntries; i++, entry++)
			{
				entries.emplace_back(base->VirtualAddress, *entry);
			}
		}

		base = decltype(base)((char*)base + base->SizeOfBlock);
		count++;
	}

	return entries;
}

template<unsigned int bitsize>
BlockStream RelocationDirectory<bitsize>::createBlock(std::uint32_t rva, std::uint32_t num_entries)
{
	std::uint32_t size = sizeof(detail::Image_t<>::RelocationBase_t) + (num_entries * sizeof(std::uint16_t));	
	std::uint32_t rsize = m_section->getVirtualSize();

	//std::uint32_t remainingBytesLeft = getRemainingFreeBytes();

	// Extend .reloc section if needed
	//if (remainingBytesLeft < size)
	//{
	//	if (!m_image->extendSection(m_section->getName(), size - remainingBytesLeft))
	//		return BlockStream();
	//}

	auto base = m_base;

	// Traverse to last block
	while (base->VirtualAddress)
	{
		if (base->VirtualAddress == rva)
		{
			return BlockStream(base);
		}

		base = decltype(base)((char*)base + base->SizeOfBlock);
	}

	std::uint16_t* entryPtr = (std::uint16_t*)(base + 1);

	for (int i = 0; i <= num_entries; ++i)
		entryPtr[i] = 0x0;

	//printf("block va: 0x%x\n", base->VirtualAddress);
	//printf("block sz: 0x%x\n", base->SizeOfBlock);

	//printf("new block va: 0x%x\n", rva);
	//printf("new block sz: 0x%x\n", size);

	// Set the new block's descriptor
	base->VirtualAddress = rva;
	base->SizeOfBlock = size;
	
	return BlockStream(base);
}

template<unsigned int bitsize>
BlockStream pepp::RelocationDirectory<bitsize>::getBlockStream(std::uint32_t rva)
{
	auto base = m_base;

	// Traverse to last block
	while (base->VirtualAddress)
	{
		if (base->VirtualAddress == rva)
			return BlockStream(base);

		base = decltype(base)((char*)base + base->SizeOfBlock);
	}

	return BlockStream();
}

template<unsigned int bitsize>
void pepp::RelocationDirectory<bitsize>::extend(std::uint32_t num_entries)
{
	std::uint32_t size = sizeof(detail::Image_t<>::RelocationBase_t) + num_entries * sizeof(std::uint16_t);
	std::uint32_t remaining_bytes = getRemainingFreeBytes();
	
	if (remaining_bytes < size)
	{
		m_image->extendSection(m_section->getName(), size);
		//__debugbreak();
	}
}

template<unsigned int bitsize>
void pepp::RelocationDirectory<bitsize>::forEachEntry(std::function<void(BlockEntry&)> Callback)
{
	auto base = m_base;
	std::vector<BlockEntry> entries;

	while (base->VirtualAddress)
	{
		int numEntries = getNumEntries(base);
		std::uint16_t* entry = (std::uint16_t*)(base + 1);

		for (int i = 0; i != numEntries; i++, entry++)
		{
			BlockEntry block(base->VirtualAddress, *entry);
			Callback(block);
		}

		base = decltype(base)((char*)base + base->SizeOfBlock);
	}
}

template<unsigned int bitsize>
bool pepp::RelocationDirectory<bitsize>::isRelocationPresent(std::uint32_t rva) const
{
	auto base = m_base;
	std::vector<BlockEntry> entries;

	while (base->VirtualAddress)
	{
		int numEntries = getNumEntries(base);
		std::uint16_t* entry = (std::uint16_t*)(base + 1);

		for (int i = 0; i != numEntries; i++, entry++)
		{
			BlockEntry block(base->VirtualAddress, *entry);
			if (block.getRva() == rva)
				return true;
		}

		base = decltype(base)((char*)base + base->SizeOfBlock);
	}

	return false;
}

template<unsigned int bitsize>
std::uint32_t pepp::RelocationDirectory<bitsize>::getTotalBlockSize()
{
	auto base = m_base;
	std::uint32_t count = 0;

	// Traverse to last block
	while (base->SizeOfBlock)
	{
		if (base->SizeOfBlock >= 0x1000)
			break;

		count += base->SizeOfBlock;
		base = decltype(base)((char*)base + base->SizeOfBlock);
	}

	return count;
}

template<unsigned int bitsize>
void pepp::RelocationDirectory<bitsize>::increaseBlockSize(std::uint32_t rva, std::uint32_t num_entries)
{
	auto base = m_base;

	while (base->VirtualAddress)
	{
		if (base->VirtualAddress == rva)
		{
			base->SizeOfBlock += (num_entries * sizeof(uint16_t));
			base->SizeOfBlock = (base->SizeOfBlock + 0x3) & ~0x3;
			break;
		}

		base = decltype(base)((char*)base + base->SizeOfBlock);
	}
}

template<unsigned int bitsize>
void pepp::RelocationDirectory<bitsize>::adjustBlockToFit(uint32_t delta)
{
	auto base = m_base;
	std::uint32_t count = 0;

	// Traverse to last block
	while (true)
	{
		auto next = decltype(base)((char*)base + base->SizeOfBlock);
		if (next->SizeOfBlock == 0 || next->SizeOfBlock >= 0x1000)
			break;

		base = next;
	}

	base->SizeOfBlock += delta;
}

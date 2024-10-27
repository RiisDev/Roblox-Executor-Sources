#include "PELibrary.hpp"
#include "PEUtil.hpp"
#include <algorithm>

using namespace pepp;

// Explicit templates.
template class Image<32>;
template class Image<64>;

template<unsigned int bitsize>
Image<bitsize>::Image()
	: m_isParsed(false)
{
}

template<unsigned int bitsize>
Image<bitsize>::Image(const Image& rhs)
	: m_fileName(rhs.m_fileName)
	//, m_imageBuffer(std::move(rhs.m_imageBuffer)) -- bad
	, m_imageBuffer(rhs.m_imageBuffer)
	, m_isParsed(false)
{
	// Ensure that the file was read.
	assert(m_imageBuffer.size() > 0);

	// Validate there is a valid MZ signature.
	_validate();
}

template<unsigned int bitsize>
constexpr PEMachine Image<bitsize>::getMachine() const
{
	if constexpr (bitsize == 32)
		return PEMachine::MACHINE_I386;

	return PEMachine::MACHINE_AMD64;
}


template<unsigned int bitsize>
Image<bitsize>::Image(std::string_view filepath)
	: m_fileName(filepath)
	, m_isParsed(false)
{
	io::File file(m_fileName, io::kFileInput | io::kFileBinary);

	std::vector<uint8_t> data{ file.Read() };

	m_imageBuffer.resize(data.size());
	m_imageBuffer.copy_data(0, data.data(), data.size());

	// Ensure that the file was read.
	assert(m_imageBuffer.size() > 0);

	// Validate there is a valid MZ signature.
	_validate();
}

template<unsigned int bitsize>
Image<bitsize>::Image(const void* data, std::size_t size)
	: m_isParsed(false)
{
	m_imageBuffer.resize(size);
	std::memcpy(&m_imageBuffer[0], data, size);

	// Validate there is a valid MZ signature.
	_validate();
}

template<unsigned int bitsize>
void pepp::Image<bitsize>::setFromMemory(const void* data, std::size_t size)
{
	m_imageBuffer.resize(size);
	std::memcpy(&m_imageBuffer[0], data, size);

	// Validate there is a valid MZ signature.
	_validate();
}

template<unsigned int bitsize>
bool Image<bitsize>::setFromMappedMemory(void* data, std::size_t size) noexcept
{
	m_MZHeader = reinterpret_cast<detail::Image_t<>::MZHeader_t*>(data);

	// Valid MZ tag?
	assert(magic() == IMAGE_DOS_SIGNATURE);

	// Setup the PE header data.
	m_PEHeader._setup(this);

	assert(m_PEHeader.isTaggedPE());

	m_imageBuffer.resize(size);
	std::memcpy(&m_imageBuffer[0], data, m_imageBuffer.size());

	// Okay just _validate now.
	_validate();

	// It's runtime, so map it.
	setAsMapped();
	_validate();


	return true;
}

template<unsigned int bitsize>
bool pepp::Image<bitsize>::setFromFilePath(std::string_view file_path)
{
	m_fileName = file_path;

	io::File file(m_fileName, io::kFileInput | io::kFileBinary);

	if (!file.Exists())
		return false;

	std::vector<uint8_t> data{ file.Read() };

	m_imageBuffer.resize(data.size());
	m_imageBuffer.copy_data(0, data.data(), data.size());

	// Ensure that the file was read.
	assert(m_imageBuffer.size() > 0);

	// Validate there is a valid MZ signature.
	_validate();

	return wasParsed();
}

template<unsigned int bitsize>
bool Image<bitsize>::hasDataDirectory(PEDirectoryEntry entry)
{
	return getPEHdr().getOptionalHdr().getDataDir(entry).Size > 0;
}

template<unsigned int bitsize>
void Image<bitsize>::writeToFile(std::string_view filepath)
{
	io::File file(filepath, io::kFileOutput | io::kFileBinary);
	file.Write(m_imageBuffer);
}

template<unsigned int bitsize>
void Image<bitsize>::_validate()
{
	m_MZHeader = reinterpret_cast<detail::Image_t<>::MZHeader_t*>(base());

	// Valid MZ tag?
	if (magic() != IMAGE_DOS_SIGNATURE)
		return;

	// Setup the PE header data.
	m_PEHeader._setup(this);

	if (!m_PEHeader.isTaggedPE())
		return;

	// Setup sections
	m_rawSectionHeaders = (m_PEHeader.m_PEHdr ? reinterpret_cast<SectionHeader*>(IMAGE_FIRST_SECTION(m_PEHeader.m_PEHdr)) : nullptr);

	if (m_rawSectionHeaders == nullptr)
		return;

	// Ensure the Image class was constructed with the correct bitsize.
	if constexpr (bitsize == 32)
	{
		if (m_PEHeader.getOptionalHdr().getMagic() != PEMagic::HDR_32)
			return;
	}
	else if constexpr (bitsize == 64)
	{
		if (m_PEHeader.getOptionalHdr().getMagic() != PEMagic::HDR_64)
			return;
	}

	// Setup export directory
	m_exportDirectory._setup(this);

	// Setup import directory
	m_importDirectory._setup(this);

	// Setup reloc directory
	m_relocDirectory._setup(this);

	// We hit the end, so everything should be properly parsed.
	m_isParsed = true;
}

template<unsigned int bitsize>
bool Image<bitsize>::appendExport(std::string_view exportName, std::uint32_t rva)
{
	getExportDir().add(exportName, rva);
	return false;
}

template<unsigned int bitsize>
void pepp::Image<bitsize>::scrambleVaData(uint32_t va, uint32_t size)
{
	//if (va > GetPEHeader().GetOptionalHeader().GetSizeOfImage())
	//	return;

	uint32_t offset = getPEHdr().rvaToOffset(va);

	for (uint32_t i = 0; i < size; ++i)
	{
		if ((offset + i) > buffer().size())
			break;

		buffer().deref<uint8_t>(offset + i) = rand() % 0xff;
	}
}

template<unsigned int bitsize>
bool pepp::Image<bitsize>::isDll() const
{
	return this->getPEHdr().getFileHdr().getCharacteristics() & IMAGE_FILE_DLL;
}

template<unsigned int bitsize>
bool pepp::Image<bitsize>::isSystemFile() const
{
	return this->getPEHdr().getFileHdr().getCharacteristics() & IMAGE_FILE_SYSTEM;
}

template<unsigned int bitsize>
bool pepp::Image<bitsize>::isDllOrSystemFile() const
{
	return isDll() || isSystemFile();
}

template<unsigned int bitsize>
void pepp::Image<bitsize>::relocateImage(uintptr_t imageBase)
{
	uintptr_t delta = (imageBase - getImageBase());

	m_relocDirectory.forEachEntry(
		[&](BlockEntry& entry)
		{
			uint32_t offset = getPEHdr().rvaToOffset(entry.getRva());

			switch (entry.getType())
			{
			case RelocationType::REL_BASED_ABSOLUTE:
				break;
			case RelocationType::REL_BASED_DIR64:
				if constexpr (bitsize == 32)
					DebugBreak();
				buffer().deref<uint64_t>(offset) += delta;
				break;
			case RelocationType::REL_BASED_HIGHLOW:
				buffer().deref<uint32_t>(offset) += (uint32_t)delta;
				break;
			case RelocationType::REL_BASED_HIGH:
				buffer().deref<uint16_t>(offset) += HIWORD(delta);
				break;
			case RelocationType::REL_BASED_LOW:
				buffer().deref<uint16_t>(offset) += LOWORD(delta);
				break;
			default:
				DebugBreak();
			}
		}
	);
}

template<unsigned int bitsize>
bool Image<bitsize>::extendSection(std::string_view sectionName, std::uint32_t delta)
{
	std::uint32_t fileAlignment = getPEHdr().getOptionalHdr().getFileAlignment();
	std::uint32_t sectAlignment = getPEHdr().getOptionalHdr().getSectionAlignment();
	if (fileAlignment == 0 || sectAlignment == 0 || delta == 0)
		return false;

	SectionHeader& header = getSectionHdr(sectionName);

	if (header.getName() != ".dummy")
	{
		std::unique_ptr<uint8_t> zero_buf(new uint8_t[delta]{});

		uint32_t ptr = header.getPtrToRawData() + header.getSizeOfRawData();

		header.setSizeOfRawData(align(header.getSizeOfRawData() + delta, fileAlignment));
		header.setVirtualSize(header.getVirtualSize() + delta);

		for (int i = 0; i < MAX_DIRECTORY_COUNT; i++)
		{
			auto& dir = getPEHdr().getOptionalHdr().getDataDir(i);

			if (dir.VirtualAddress == header.getVirtualAddress())
			{
				dir.Size += delta;
				break;
			}
		}

		// Update image size
		getPEHdr().getOptionalHdr().setSizeOfImage(align(getPEHdr().getOptionalHdr().getSizeOfImage() + delta, sectAlignment));

		// Fill in data
		//buffer().insert_data(header.getPtrToRawData() + header.getSizeOfRawData(), zero_buf.get(), delta);
		//buffer().insert_data(header.getPtrToRawData() + header.getSizeOfRawData() - delta, zero_buf.get(), delta);
		buffer().insert(buffer().begin() + ptr, align(delta, fileAlignment), 0);
		//buffer().resize(align(buffer().size() + delta, fileAlignment));
		
		// Re-validate the image/headers.
		_validate();

		return true;
	}

	return false;
}

template<unsigned int bitsize>
std::uint32_t Image<bitsize>::findPadding(SectionHeader* s, std::uint8_t v, std::size_t n, std::uint32_t alignment)
{
	bool bTraverseUp = s == nullptr;
	std::uint32_t startOffset{};

	n = align(n, alignment);

	if (s == nullptr)
		s = &m_rawSectionHeaders[getNumberOfSections() - 1];

	startOffset = s->getPtrToRawData();

	std::vector<uint8_t>::iterator it = buffer().end();

	// Start from bottom to top, or vice versa?
	if (bTraverseUp)
	{
		std::vector<uint8_t> tmpData(n, v);

		for (std::uint32_t i = startOffset + s->getSizeOfRawData(); i > n; i = align(i - n, alignment))
		{
			if (memcmp(&buffer()[i - n], tmpData.data(), tmpData.size()) == 0)
			{
				it = buffer().begin() + (i - n);
				break;
			}
		}
	}
	else
	{
		std::vector<uint8_t> tmpData(n, v);

		for (std::uint32_t i = startOffset; i < startOffset + (buffer().size() - startOffset); i = align(i + n, alignment))
		{
			if (memcmp(&buffer()[i], tmpData.data(), tmpData.size()) == 0)
			{
				it = buffer().begin() + (i);
				break;
			}
		}
	}


	if (it == buffer().end())
		return -1;

	return (std::uint32_t)std::distance(buffer().begin(), it);
}

template<unsigned int bitsize>
std::uint32_t Image<bitsize>::findZeroPadding(SectionHeader* s, std::size_t n, std::uint32_t alignment)
{
	return findPadding(s, 0x0, n, alignment);
}

template<unsigned int bitsize>
std::vector<std::uint32_t> Image<bitsize>::findBinarySequence(SectionHeader* s, std::string_view binary_seq) const
{
	constexpr auto ascii_to_byte = [](const char ch) [[msvc::forceinline]] {
				if (ch >= '0' && ch <= '9')
					return std::uint8_t(ch - '0');
				if (ch >= 'A' && ch <= 'F')
					return std::uint8_t(ch - 'A' + '\n');
				return std::uint8_t(ch - 'a' + '\n');
	};

	std::vector<std::uint32_t> offsets{};

	if (s == nullptr)
		s = &m_rawSectionHeaders[getNumberOfSections() - 1];

	std::uint32_t start_offset = s->getPtrToRawData();
	std::uint32_t result = 0;
	std::uint32_t match_count = 0;

	for (std::uint32_t i = start_offset; i <= start_offset + s->getSizeOfRawData(); ++i)
	{
		for (int c = 0; c < binary_seq.size();)
		{
			if (binary_seq[c] == ' ')
			{
				++c;
				continue;
			}

			if (binary_seq[c] == '?')
			{
				++c;
				++match_count;
				continue;
			}

			if (buffer()[i + match_count++] != ((ascii_to_byte(binary_seq[c]) << 4) | ascii_to_byte(binary_seq[c + 1])))
			{
				result = 0;
				break;
			}

			result = i;
			c += 2;
		}
	
		if (result)
		{
			offsets.emplace_back(i);
			i += match_count - 1;
		}

		
		match_count = 0;
		result = 0;
	}

	return offsets;
}

template<unsigned int bitsize>
std::vector<std::pair<std::int32_t, std::uint32_t>> Image<bitsize>::findBinarySequences(SectionHeader* s, std::initializer_list<std::pair<std::int32_t, std::string_view>> binary_seq) const
{
	constexpr auto ascii_to_byte = [](const char ch) [[msvc::forceinline]] {
				if (ch >= '0' && ch <= '9')
					return std::uint8_t(ch - '0');
				if (ch >= 'A' && ch <= 'F')
					return std::uint8_t(ch - 'A' + '\n');
				return std::uint8_t(ch - 'a' + '\n');
	};

	std::vector<std::pair<std::int32_t, std::uint32_t>> offsets{};

	if (s == nullptr)
		s = &m_rawSectionHeaders[getNumberOfSections() - 1];

	std::uint32_t start_offset = s->getPtrToRawData();
	std::pair<std::int32_t, std::uint32_t> result{};
	std::uint32_t match_count = 0;

	for (std::uint32_t i = start_offset; i <= start_offset + s->getSizeOfRawData(); ++i)
	{
		for (auto const& seq : binary_seq)
		{
			for (int c = 0; c < seq.second.size();)
			{
				if (seq.second[c] == ' ')
				{
					++c;
					continue;
				}

				if (seq.second[c] == '?')
				{
					++c;
					++match_count;
					continue;
				}

				std::uint8_t _byte = ((ascii_to_byte(seq.second[c]) << 4) | ascii_to_byte(seq.second[c + 1]));

				if (buffer()[i + match_count++] != _byte)
				{
					result = { 0,0 };
					break;
				}

				result = {seq.first, i};
				c += 2;
			}

			if (result.second)
			{
				offsets.emplace_back(std::move(result));
				break;
			}

			match_count = 0;
			result = { 0, 0 };
		}

		i += std::max<int>(match_count - 1, 0);
		match_count = 0;
	}

	return offsets;
}

template<unsigned int bitsize>
bool Image<bitsize>::appendSection(std::string_view section_name, std::uint32_t size, std::uint32_t chrs, SectionHeader* out)
{
	std::uint32_t fileAlignment = getPEHdr().getOptionalHdr().getFileAlignment();
	std::uint32_t sectAlignment = getPEHdr().getOptionalHdr().getSectionAlignment();
	if (fileAlignment == 0 || sectAlignment == 0)
		return false;

	std::uint32_t alignedFileSize = align(size, fileAlignment);
	std::uint32_t alignedVirtSize = size;
	std::uint32_t oldFileSize = getPEHdr().getOptionalHdr().getSizeOfImage();
	size_t oldSize = buffer().size();

	// Build a section (these should be the only necessary values to fill)
	SectionHeader sec;
	memset(&sec, 0, sizeof(sec));

	sec.setName(section_name);
	sec.setSizeOfRawData(alignedFileSize);
	sec.setVirtualSize(alignedVirtSize);
	sec.setCharacteristics(chrs);
	sec.setVirtualAddress(getPEHdr().getNextSectionRva());
	sec.setPointerToRawData(getPEHdr().getNextSectionOffset());

	// Update image size
	getPEHdr().getOptionalHdr().setSizeOfImage(align4kb(getPEHdr().getOptionalHdr().getSizeOfImage() + size));

	// Update PE header info.
	uint32_t numSections = getNumberOfSections();
	getPEHdr().getFileHdr().setNumberOfSections(numSections + 1);
	getPEHdr().getOptionalHdr().setSizeOfCode(getPEHdr().getOptionalHdr().getSizeOfCode() + alignedVirtSize);
	getPEHdr().getOptionalHdr().setSizeOfHeaders(getPEHdr().getOptionalHdr().getSizeOfHeaders() + sizeof(sec));

	// Add it in the raw section header
	SectionHeader& lastHdr = getSectionHdr(numSections);
	memcpy(&lastHdr, &sec, sizeof(sec));

	if (out)
		memcpy(out, &m_rawSectionHeaders[numSections], sizeof(SectionHeader));

	// buffer().resize(buffer().size() + alignedFileSize);
	buffer().insert(buffer().begin() + sec.getPtrToRawData(), alignedFileSize, 0);

	// Re-validate the image/headers.
	_validate();

	return true;
}


template<unsigned int bitsize>
void pepp::Image<bitsize>::setAsMapped() noexcept
{
	for (std::uint16_t i = 0; i < getNumberOfSections(); ++i)
	{
		SectionHeader& sec = getSectionHdr(i);

		sec.setPointerToRawData(sec.getVirtualAddress());
		sec.setSizeOfRawData(sec.getVirtualSize());
	}

	m_isMemMapped = true;
}

template<unsigned int bitsize>
void pepp::Image<bitsize>::mapToBuffer(pepp::Address<> basePtr, const std::vector<std::string>& ignore)
{
	for (int i = 0; i < getNumberOfSections(); ++i)
	{
		SectionHeader& sec = getSectionHdr(i);

		bool bSkip = false;

		if (!ignore.empty())
		{
			for (auto& item : ignore)
			{
				if (item == sec.getName())
				{
					bSkip = true;
					break;
				}
			}
		}

		if (bSkip)
			continue;

		memcpy((basePtr.ptr<char>() + sec.getVirtualAddress()), &base()[sec.getPtrToRawData()], sec.getSizeOfRawData());
	}
}
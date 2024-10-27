#pragma once

namespace pepp
{
	constexpr static int MAX_DIRECTORY_COUNT = 16;

	template<unsigned int>
	class Image;
	template<unsigned int>
	class OptionalHeader;
	
	class SectionHeader;
	class FileHeader;
	
	template<unsigned int bitsize = 32>
	class PEHeader : pepp::msc::NonCopyable
	{
		friend class Image<bitsize>;

		using ImageData_t = detail::Image_t<bitsize>;

		Image<bitsize>*					m_image;
		ImageData_t::Header_t*			m_PEHdr = nullptr;
		FileHeader						m_FileHeader;
		OptionalHeader<bitsize>			m_OptionalHeader;
	private:
		//! Private constructor, this should never be established outside of `class Image`
		PEHeader();
	public:

		class FileHeader& getFileHdr() {
			return m_FileHeader;
		}

		const class FileHeader& getFileHdr() const {
			return m_FileHeader;
		}

		class OptionalHeader<bitsize>& getOptionalHdr() {
			return m_OptionalHeader;
		}

		const class OptionalHeader<bitsize>& getOptionalHdr() const {
			return m_OptionalHeader;
		}

		SectionHeader& getSectionHeader(std::uint16_t dwIndex) {
			static SectionHeader dummy{};

			if (dwIndex < m_image->getNumberOfSections())
				return m_image->m_rawSectionHeaders[dwIndex];
			
			return dummy;
		}

		SectionHeader& getSectionHeader(std::string_view name) {
			static SectionHeader dummy{};

			for (std::uint16_t n = 0; n < m_image->getNumberOfSections(); n++)
			{
				if (m_image->m_rawSectionHeaders[n].getName().compare(name) == 0) {
					return m_image->m_rawSectionHeaders[n];
				}
			}

			return dummy;
		}

		SectionHeader& getSectionHeaderFromVa(std::uint32_t va) {
			static SectionHeader dummy{}; 
			
			for (std::uint16_t n = 0; n < m_image->getNumberOfSections(); n++)
			{
				if (m_image->m_rawSectionHeaders[n].hasVirtualAddress(va)) {
					return m_image->m_rawSectionHeaders[n];
				}
			}

			return dummy;
		}

		SectionHeader& getSectionHeaderFromOffset(std::uint32_t offset) {
			static SectionHeader dummy{};

			for (std::uint16_t n = 0; n < m_image->getNumberOfSections(); n++)
			{
				if (m_image->m_rawSectionHeaders[n].hasOffset(offset)) {
					return m_image->m_rawSectionHeaders[n];
				}
			}

			return dummy;
		}



		//! Calculate the number of directories present (not NumberOfRvaAndSizes)
		std::uint32_t getDirectoryCount() const {
			return getOptionalHdr().getDirectoryCount();
		}

		//! Convert a relative virtual address to a file offset
		std::uint32_t rvaToOffset(std::uint32_t rva) {
			SectionHeader const& sec { getSectionHeaderFromVa(rva) };
			//
			// Did we get one?
			if (sec.getName() != ".dummy") {
				return sec.getPtrToRawData() + rva - sec.getVirtualAddress();
			}

			return 0ul;
		}

		//! Convert a file offset back to a relative virtual address
		std::uint32_t offsetToRva(std::uint32_t offset) {
			SectionHeader const& sec{ getSectionHeaderFromOffset(offset) };
			//
			// Did we get one?
			if (sec.getName() != ".dummy") {
				return (sec.getVirtualAddress() + offset) - sec.getPtrToRawData();
			}

			return 0ul;
		}
		 
		//! Convert a rel. virtual address to a virtual address
		detail::Image_t<bitsize>::Address_t rvaToVa(std::uint32_t rva) const {
			return m_OptionalHeader.getImageBase() + rva;
		}

		//! Used to check if the NT tag is present.
		bool isTaggedPE() const {
			return m_PEHdr->Signature == IMAGE_NT_SIGNATURE;
		}

		std::uint8_t* base() const {
			return (std::uint8_t*)m_PEHdr;
		}

		constexpr std::size_t size() const {
			return sizeof(decltype(*m_PEHdr));
		}

		//! Return native pointer
		detail::Image_t<bitsize>::Header_t* native() {
			return m_PEHdr;
		}

		//! Manually calculate the size of the image
		std::uint32_t calcSizeOfImage();

		//! Manually calculate the start of the code section
		std::uint32_t getStartOfCode();

		//! Calculate next section offset
		std::uint32_t getNextSectionOffset();

		//! Calculate next section rva
		std::uint32_t getNextSectionRva();
	private:
		//! Setup the header
		void _setup(Image<bitsize>* image) {
			m_image = image;
			m_PEHdr = reinterpret_cast<decltype(m_PEHdr)>(m_image->base() + m_image->m_MZHeader->e_lfanew);
			m_FileHeader._setup(image);
			m_OptionalHeader._setup(image);
		}
	};
}
#pragma once

namespace pepp
{
	enum class PEMachine
	{
		MACHINE_I386 = 0x14c,
		MACHINE_IA64 = 0x200,
		MACHINE_AMD64 = 0x8664
	};

	class FileHeader : pepp::msc::NonCopyable
	{
		friend class PEHeader<32>;
		friend class PEHeader<64>;

		IMAGE_FILE_HEADER*	m_base;
	public:
		FileHeader() 
		{
		}

		void setMachine(PEMachine machine) {
			m_base->Machine = static_cast<std::uint16_t>(machine);
		}

		PEMachine getMachine() const {
			return static_cast<PEMachine>(m_base->Machine);
		}

		void setNumberOfSections(std::uint16_t numSections) {
			m_base->NumberOfSections = numSections;
		}

		std::uint16_t getNumberOfSections() const {
			return m_base->NumberOfSections;
		}

		void setTimeDateStamp(std::uint32_t dwTimeDateStamp) {
			m_base->TimeDateStamp = dwTimeDateStamp;
		}

		std::uint32_t getTimeDateStamp() const {
			return m_base->TimeDateStamp;
		}

		void setPointerToSymbolTable(std::uint32_t dwPointerToSymbolTable) {
			m_base->PointerToSymbolTable = dwPointerToSymbolTable;
		}

		std::uint32_t setPointerToSymbolTable() const {
			return m_base->PointerToSymbolTable;
		}

		void setNumberOfSymbols(std::uint32_t numSymbols) {
			m_base->NumberOfSymbols = numSymbols;
		}

		std::uint32_t getNumberOfSymbols() const {
			return m_base->NumberOfSymbols;
		}

		void setSizeOfOptionalHeader(std::uint16_t size) {
			m_base->SizeOfOptionalHeader = size;
		}

		std::uint16_t getSizeOfOptionalHeader() const {
			return m_base->SizeOfOptionalHeader;
		}

		void setCharacteristics(std::uint16_t chars) {
			m_base->Characteristics = chars;
		}

		std::uint16_t getCharacteristics() const {
			return m_base->Characteristics;
		}

		IMAGE_FILE_HEADER* native() const {
			return m_base;
		}
	private:
		template<unsigned int bitsize>
		void _setup(Image<bitsize>* image) {
			m_base = &image->getPEHdr().native()->FileHeader;
		}
	};
}
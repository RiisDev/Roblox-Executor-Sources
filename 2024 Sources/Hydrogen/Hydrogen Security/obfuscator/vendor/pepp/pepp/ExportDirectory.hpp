#pragma once

#include <functional>

namespace pepp
{
	struct ExportData_t
	{
		std::string name{};
		std::uint32_t rva = 0;
		std::uint32_t base_ordinal = 0xffffffff;
		std::uint32_t name_ordinal = 0xffffffff;
	};

	template<unsigned int bitsize>
	class ExportDirectory : public pepp::msc::NonCopyable
	{
		friend class Image<32>;
		friend class Image<64>;

		Image<bitsize>*							m_image;
		detail::Image_t<>::ExportDirectory_t	*m_base;
	public:
		ExportData_t getExport(std::uint32_t idx, bool demangle = true) const;
		ExportData_t getExport(std::string_view name, bool demangle = true) const;
		void add(std::string_view name, std::uint32_t rva);
		void traverseExports(const std::function<void(ExportData_t*)>& cb_func, bool demangle = true);
		bool isPresent() const noexcept;

		void setNumberOfFunctions(std::uint32_t num) {
			m_base->NumberOfFunctions = num;
		}

		std::uint32_t getNumberOfFunctions() const {
			return m_base->NumberOfFunctions;
		}

		void setNumberOfNames(std::uint32_t num) {
			m_base->NumberOfNames = num;
		}

		std::uint32_t getNumberOfNames() const {
			return m_base->NumberOfNames;
		}

		void setCharacteristics(std::uint32_t chrs) {
			m_base->Characteristics = chrs;
		}

		std::uint32_t getCharacteristics() const {
			return m_base->Characteristics;
		}

		void setTimeDateStamp(std::uint32_t TimeDateStamp) {
			m_base->TimeDateStamp = TimeDateStamp;
		}

		std::uint32_t getTimeDateStamp() const {
			return m_base->TimeDateStamp;
		}

		void setAddressOfFunctions(std::uint32_t AddressOfFunctions) {
			m_base->AddressOfFunctions = AddressOfFunctions;
		}

		std::uint32_t getAddressOfFunctions() const {
			return m_base->AddressOfFunctions;
		}

		void setAddressOfNames(std::uint32_t AddressOfNames) {
			m_base->AddressOfNames = AddressOfNames;
		}

		std::uint32_t getAddressOfNames() const {
			return m_base->AddressOfNames;
		}

		void setAddressOfNameOrdinals(std::uint32_t AddressOfNamesOrdinals) {
			m_base->AddressOfNameOrdinals = AddressOfNamesOrdinals;
		}

		std::uint32_t getAddressOfNameOrdinals() const {
			return m_base->AddressOfNameOrdinals;
		}


		constexpr std::size_t size() const {
			return sizeof(decltype(*m_base));
		}

	private:
		//! Setup the directory
		void _setup(Image<bitsize>* image) {
			m_image = image;
			m_base = reinterpret_cast<decltype(m_base)>(
				&image->base()[image->getPEHdr().rvaToOffset(
					image->getPEHdr().getOptionalHdr().getDataDir(DIRECTORY_ENTRY_EXPORT).VirtualAddress)]);
		}
	};
}
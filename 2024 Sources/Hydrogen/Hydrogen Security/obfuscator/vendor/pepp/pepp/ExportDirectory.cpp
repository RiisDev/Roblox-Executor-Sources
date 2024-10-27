#include "PELibrary.hpp"

using namespace pepp;

template class ExportDirectory<32>;
template class ExportDirectory<64>;

template<unsigned int bitsize>
ExportData_t ExportDirectory<bitsize>::getExport(std::uint32_t idx, bool demangle /*= true*/) const
{
	if (!isPresent())
		return {};

	if (idx < getNumberOfNames()) {
		std::uint8_t* base{};
		std::uint32_t funcAddresses{};
		std::uint32_t funcNames{};
		std::uint32_t funcOrdinals{};
		std::uint32_t funcNamesOffset{};
		mem::ByteVector const* buffer{};

		funcOrdinals = m_image->getPEHdr().rvaToOffset(getAddressOfNameOrdinals());
		uint16_t rlIdx = m_image->buffer().deref<uint16_t>(funcOrdinals + (idx * sizeof uint16_t));

		funcAddresses = m_image->getPEHdr().rvaToOffset(getAddressOfFunctions() + sizeof(std::uint32_t) * rlIdx);
		funcNames = m_image->getPEHdr().rvaToOffset(getAddressOfNames() + sizeof(std::uint32_t) * idx);
		funcNamesOffset = m_image->getPEHdr().rvaToOffset(m_image->buffer().deref<uint32_t>(funcNames));


		if (funcAddresses && funcNames && funcOrdinals)
		{
			return 
			{
				   demangle ? DemangleName(m_image->buffer().as<char*>(funcNamesOffset)) : m_image->buffer().as<char*>(funcNamesOffset),
				   m_image->buffer().deref<uint32_t>(funcAddresses),
				   m_base->Base + idx,
				   rlIdx
			};
		}
	}

	return {};
}

template<unsigned int bitsize>
ExportData_t pepp::ExportDirectory<bitsize>::getExport(std::string_view name, bool demangle) const
{
	for (int i = 0; i < getNumberOfNames(); i++)
	{
		ExportData_t data = getExport(i, demangle);
		if (data.name == name)
			return data;
	}

	return {};
}

template<unsigned int bitsize>
void ExportDirectory<bitsize>::traverseExports(const std::function<void(ExportData_t*)>& cb_func, bool demangle)
{
	for (int i = 0; i < getNumberOfNames(); i++)
	{
		ExportData_t data = getExport(i, demangle);
		if (data.rva != 0)
			cb_func(&data);
	}
}

template<unsigned int bitsize>
bool ExportDirectory<bitsize>::isPresent() const noexcept
{
	return m_image->getPEHdr().getOptionalHdr().getDataDir(DIRECTORY_ENTRY_EXPORT).Size > 0;
}

template<unsigned int bitsize>
void ExportDirectory<bitsize>::add(std::string_view name, std::uint32_t rva)
{
	// TODO
}

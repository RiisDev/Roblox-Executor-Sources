#include "CallingConventionDetector.hpp"
#include <functional>
#include <algorithm>
#include <thread>


CallingConventionDetector::CallingConventionDetector(uint32_t uiAddress, uint32_t uiData) : m_BaseData(uiData)
{
	this->m_Address = uiAddress;
	this->m_PEParser = new PEParser32(uiData);
	this->unmCallingConvention = this->ScanForCallingConvention();
}

CallingConventionDetector::~CallingConventionDetector()
{
	delete this->m_PEParser;
}

bool CallingConventionDetector::SetsEdxOrEcxRegister(const uint32_t& uiAddress) const
{
	for (int i = 1; i < 6; ++i)
	{
		if (*reinterpret_cast<PBYTE>(uiAddress - i) == 0x68) /* Got push, ignore it and rerun. */
		{
			return this->SetsEdxOrEcxRegister(uiAddress - i);
		}
		if (*reinterpret_cast<PBYTE>(uiAddress - (i + 1)) == 0x8B &&
			(*reinterpret_cast<PBYTE>(uiAddress - i) == 0xCE ||
				*reinterpret_cast<PBYTE>(uiAddress - i) == 0xC8 ||
				*reinterpret_cast<PBYTE>(uiAddress - i) == 0xCF))
		{
			return true;
		}
	}

	return false;
}

bool CallingConventionDetector::CallerCleansUpStack(const uint32_t& uiAddress)
{
	uint32_t uiStartAddress = uiAddress + 5; //next statement

	while (*reinterpret_cast<PBYTE>(uiStartAddress) != 0xE8 && *reinterpret_cast<PBYTE>(uiStartAddress) != 0xC3 && *
		reinterpret_cast<PBYTE>(uiStartAddress) != 0xC2 && *reinterpret_cast<PBYTE>(uiStartAddress) != 0xFF)
		//call/ret/retn
	{
		if (*reinterpret_cast<PBYTE>(uiStartAddress++) == 0x83 && *reinterpret_cast<PBYTE>(uiStartAddress) == 0xC4)
			// add esp 
			return true;
	}

	return false;
}

UnmanagedCallingConvention CallingConventionDetector::ScanForCallingConvention() const
{
	const PIMAGE_SECTION_HEADER pish = this->m_PEParser->GetSectionHeader(".text");
	const std::vector<uint32_t> references = GetXRefs(pish->VirtualAddress + this->m_BaseData, pish->Misc.VirtualSize);

	if (!references.empty())
	{
		int counter = 0;

		for (const uint32_t& reference : references)
		{
			if (this->CallerCleansUpStack(reference))
				return UnmanagedCallingConvention::UnmanagedCdecl;
			if (this->SetsEdxOrEcxRegister(reference))
				counter++;
		}

		const float percentage = static_cast<float>(counter) / static_cast<float>(references.size());
		return (percentage >= 0.2)
			? UnmanagedCallingConvention::UnmanagedFastcall
			: UnmanagedCallingConvention::UnmanagedStdcall; //you can modify this yourself.
	}
	//this is bad -> forward to next function start and then find the return statement of our function.
	//drawback is can't decipher between __stdcall and __fastcall so we will just pick fastcall
	size_t szFunctionSize = 0;

	do
	{
		szFunctionSize += 0x10;
	} 	while (memcmp(reinterpret_cast<const void*>(this->m_Address + szFunctionSize), "\x55\x8B\xEC", 3) != 0);

	uint32_t uiNextFunctionAddress = this->m_Address + szFunctionSize;

	while (true)
	{
		uiNextFunctionAddress--;

		if (*reinterpret_cast<PBYTE>(uiNextFunctionAddress) == 0xC3)
			return UnmanagedCallingConvention::UnmanagedCdecl;
		if (*reinterpret_cast<PBYTE>(uiNextFunctionAddress) == 0xC2)
			return UnmanagedCallingConvention::UnmanagedFastcall;
	}
}

UnmanagedCallingConvention CallingConventionDetector::GetCallingConvention() const
{
	return this->unmCallingConvention;
}

void CallingConventionDetector::FindNeedleInHayStack(const uint32_t& target, std::vector<uint32_t>* xrefs,
	const uint32_t& uiStartAddress, const uint32_t& uiSearchLength)
{
	for (uint32_t uiCurrentAddress = uiStartAddress; uiCurrentAddress < uiStartAddress + uiSearchLength;
		uiCurrentAddress++)
	{
		if (*reinterpret_cast<PBYTE>(uiCurrentAddress) == 0xE8)
		{
			if (*reinterpret_cast<uint32_t*>(uiCurrentAddress + 1) == (target - uiCurrentAddress - 5))
			{
				xrefs->push_back(uiCurrentAddress);
			}
		}
	}
}

std::vector<uint32_t> CallingConventionDetector::GetXRefs(const uint32_t& uiStartAddress,
	const uint32_t& uiSearchLength) const
{
	std::vector<std::thread> threads;
	std::vector<uint32_t> xrefs;
	const uint32_t final_address = uiStartAddress + uiSearchLength;
	const uint16_t page_size = 0x1000;
	const uint32_t page_amount = (uiSearchLength / page_size) + 1;
	uint32_t uiThreadScanSize = page_size * (page_amount / 5);

	for (uint32_t uiCurrentAddress = uiStartAddress; uiCurrentAddress < final_address; uiCurrentAddress +=
		uiThreadScanSize)
	{
		if (uiCurrentAddress + uiThreadScanSize > final_address)
			uiThreadScanSize = final_address - uiCurrentAddress;


		std::thread thread(FindNeedleInHayStack, this->m_Address, &xrefs, uiCurrentAddress, uiThreadScanSize);
		threads.push_back(std::move(thread));
	}

	for (std::thread& thread : threads)
		thread.join();

	return xrefs;
}
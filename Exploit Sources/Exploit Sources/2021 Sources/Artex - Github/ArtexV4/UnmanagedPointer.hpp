#include "CallingConventionDetector.hpp"
#include <cstdint>
#include <utility>

template <typename t_Function>
class UnmanagedPointer
{
public:
	template <typename... t_FunctionParameters>
	auto operator()(t_FunctionParameters ... params)
	{
		using result_type = decltype(std::declval<t_Function>()(std::declval<t_FunctionParameters>()...));
		using function_cdecl_ptr_t = result_type(__cdecl*)(t_FunctionParameters ...);
		using function_stdcall_ptr_t = result_type(__stdcall*)(t_FunctionParameters ...);
		using function_fastcall_ptr_t = result_type(_fastcall*)(t_FunctionParameters ...);

		switch (this->m_CallingConvention)
		{
		case UnmanagedCallingConvention::UnmanagedCdecl:
			return reinterpret_cast<function_cdecl_ptr_t>(this->m_Address)(params...);
		case UnmanagedCallingConvention::UnmanagedStdcall:
			return reinterpret_cast<function_stdcall_ptr_t>(this->m_Address)(params...);
		case UnmanagedCallingConvention::UnmanagedFastcall:
			return reinterpret_cast<function_fastcall_ptr_t>(this->m_Address)(params...);
		default:
			return reinterpret_cast<function_cdecl_ptr_t>(this->m_Address)(params...);
		}
	}

	UnmanagedPointer(uint32_t dwAddress, uint32_t dwBaseAddress = reinterpret_cast<uint32_t>(GetModuleHandle(nullptr)),
		bool bRetCheck = true)
	{
		this->m_Address = dwAddress;
		auto* ccDetector = new CallingConventionDetector(this->m_Address, dwBaseAddress);
		this->m_CallingConvention = ccDetector->GetCallingConvention();
		if (bRetCheck)
			this->RemoveReturnCheck();
		delete ccDetector;
	}

	UnmanagedPointer(const char* bMask, const char* szMask,
		const uint32_t& dwBaseAddress = reinterpret_cast<uint32_t>(GetModuleHandle(nullptr)),
		const uint32_t& dwLen = 0x7FFFFFF, bool bRetCheck = true)
	{
		this->m_Address = this->FindPattern(bMask, szMask, dwBaseAddress, dwLen);
		auto* ccDetector = new CallingConventionDetector(this->m_Address, dwBaseAddress);
		this->m_CallingConvention = ccDetector->GetCallingConvention();
		if (bRetCheck)
			this->RemoveReturnCheck();
		delete ccDetector;
	}

private:
	uint32_t m_Address;
	UnmanagedCallingConvention m_CallingConvention;

	static bool DataCompare(const unsigned char* pData, const unsigned char* bMask, const char* szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++bMask)
			if (*szMask == 'x' && *pData != *bMask)
				return false;
		return (*szMask) == 0;
	}

	static uint32_t FindPattern(const char* bMask, const char* szMask, const uint32_t dwBaseAddress,
		const uint32_t dwLen)
	{
		for (unsigned long i = 0; i < dwLen; i++)
			if (DataCompare(reinterpret_cast<unsigned char*>(dwBaseAddress + i), (unsigned char*)(bMask), szMask))
				return static_cast<uint32_t>(dwBaseAddress + i);
		return 0;
	}

	void RemoveReturnCheck()
	{
		LPVOID lpAllocation = nullptr;
		size_t szFunctionSize = 0;
		bool bRetcheck = false;

		do
		{
			szFunctionSize += 0x10;
		} 		while (memcmp(reinterpret_cast<const void*>(this->m_Address + szFunctionSize), "\x55\x8B\xEC", 3) != 0);


		lpAllocation = VirtualAlloc(nullptr, szFunctionSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

		memcpy(lpAllocation, reinterpret_cast<void const*>(this->m_Address), szFunctionSize);

		for (size_t i = 0; i < szFunctionSize; ++i)
		{
			const uint32_t uiRobloxAdr = this->m_Address + i;
			const uint32_t uiAllocAdr = reinterpret_cast<uint32_t>(lpAllocation) + i;

			if (*reinterpret_cast<BYTE*>(uiAllocAdr) == 0xE8)
			{
				*reinterpret_cast<uint32_t*>(uiAllocAdr + 1) = (uiRobloxAdr + *reinterpret_cast<uint32_t*>(uiRobloxAdr +
					1) + 5) - uiAllocAdr - 5;
				i += 4; //don't scan rel32
			}

			if (memcmp(reinterpret_cast<const void*>(uiAllocAdr), "\x72\x1B\xA1", 3) == 0)
			{
				bRetcheck = true;
				memcpy(reinterpret_cast<void*>(uiAllocAdr - 0x11), "\xEB\x2C\x90\x90\x90", 5);
				i += 0x1D; //retcheck is 0x1D bytes so we don't need to scan those anymore
			}
		}

		if (!bRetcheck)
			VirtualFree(lpAllocation, 0, MEM_RELEASE);
		else
			this->m_Address = reinterpret_cast<uint32_t>(lpAllocation);
	}
};
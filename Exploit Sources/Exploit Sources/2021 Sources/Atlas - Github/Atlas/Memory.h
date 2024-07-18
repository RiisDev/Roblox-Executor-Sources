#pragma once
#include "Globals.h"

class Memory {
public:
	static DWORD GetProcessID(const wchar_t* processName);

	static DWORD GetModuleBaseAddress(DWORD processID, const wchar_t* moduleName);

	static DWORD Scan(DWORD baseAddress, DWORD VFTableAddress);

	template<typename T>
	static inline T Read(LPCVOID address, SIZE_T size = sizeof(T)) { // Had to put the definition here because you can't define generic methods in the cpp file.
		T buffer;
		ReadProcessMemory(Globals::Handle, address, &buffer, size, 0);
		return buffer;
	};

	static DWORD GetPointerAddress(DWORD address);

	static DWORD GetDMAAddress(DWORD ptr, std::vector<unsigned int> offsets);

	static std::string ReadStringOfUnknownLength(DWORD address);
};
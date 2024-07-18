#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include "Memory.h"

DWORD Memory::GetProcessID(const wchar_t* processName) {
	DWORD processID = 0;

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE) {
		std::cout << "Couldn't get running processes." << std::endl;
		return 0;
	}

	PROCESSENTRY32 processEntry;
	processEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(snapshot, &processEntry)) {
		CloseHandle(snapshot);
		std::cout << "Couldn't get info of processes." << std::endl;
		return 0;
	}

	do {
		if (!_wcsicmp(processEntry.szExeFile, processName)) {
			processID = processEntry.th32ProcessID;
			break;
		}
	} while (Process32Next(snapshot, &processEntry));

	CloseHandle(snapshot);

	return processID;
}

DWORD Memory::GetModuleBaseAddress(DWORD processID, const wchar_t* moduleName) {
	DWORD moduleBaseAddress = 0;

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);
	if (snapshot == INVALID_HANDLE_VALUE) {
		std::cout << "Couldn't get running processes." << std::endl;
		return 0;
	}

	MODULEENTRY32 moduleEntry;
	moduleEntry.dwSize = sizeof(moduleEntry);

	if (!Module32First(snapshot, &moduleEntry)) {
		CloseHandle(snapshot);
		std::cout << "Couldn't get info of modules." << std::endl;
		return 0;
	}

	do {
		if (!_wcsicmp(moduleEntry.szModule, moduleName)) {
			moduleBaseAddress = (DWORD)moduleEntry.modBaseAddr;
			break;
		}
	} while (Module32Next(snapshot, &moduleEntry));

	CloseHandle(snapshot);

	return moduleBaseAddress;
}

DWORD Memory::Scan(DWORD baseAddress, DWORD VFTableAddress) {
	SYSTEM_INFO systemInfo;
	DWORD pageSize;
	DWORD pageSize4ByteSplit;
	MEMORY_BASIC_INFORMATION memoryInfo;
	GetSystemInfo(&systemInfo);
	pageSize = systemInfo.dwPageSize;
	pageSize4ByteSplit = pageSize / 4;
	DWORD* buffer = new DWORD[pageSize];

	for (DWORD addr = baseAddress; addr < 0x7FFFFFFF; addr += pageSize) {
		VirtualQueryEx(Globals::Handle, (LPCVOID)addr, &memoryInfo, pageSize);
		if (memoryInfo.Protect == PAGE_READWRITE) {
			ReadProcessMemory(Globals::Handle, (LPCVOID)addr, buffer, pageSize, 0);
			for (DWORD i = 0; i <= pageSize4ByteSplit; i++) {
				if (buffer[i] == VFTableAddress) {
					delete[] buffer;
					return (DWORD)(addr + (i * 4));
				}
			}
		}
	}

	delete[] buffer;
	return 0;
}

DWORD Memory::GetPointerAddress(DWORD address) {
	DWORD pointerAddress = GetDMAAddress(address, { 0x0 });
	return pointerAddress;
}

DWORD Memory::GetDMAAddress(DWORD pointer, std::vector<unsigned int> offsets) {
	DWORD address = pointer;
	for (unsigned int i = 0; i < offsets.size(); ++i) {
		address = Read<DWORD>((LPCVOID)address);
		address += offsets[i];
	}
	return address;
}

std::string Memory::ReadStringOfUnknownLength(DWORD address) {
	std::string string;
	char character = 0;
	int charSize = sizeof(character);
	int offset = 0;

	while (true) {
		character = Read<char>((LPCVOID)(address + offset));
		if (character == 0) break;
		offset += charSize;
		string.push_back(character);
	}

	return string;
}
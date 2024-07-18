#pragma once
#pragma once
#include <Windows.h>
#include <iostream>

namespace ManualMap
{

	struct WDLL
	{
		HANDLE hProcess;
		DWORD_PTR ImageSize;
		HMODULE Module;
		PVOID Image, Entrypoint;
		PIMAGE_DOS_HEADER DosHeader;
		PIMAGE_NT_HEADERS NtHeader;
		PIMAGE_BASE_RELOCATION BaseRelocation;
		PIMAGE_IMPORT_DESCRIPTOR ImportDes;
		PIMAGE_TLS_DIRECTORY Tls;
		DWORD Flags;
		LPVOID Parameter;
		BOOL IsDLL;
		BOOL IsWait;
		BOOL IsNtDll;
		HANDLE hThread;
	};


	enum ManualMap_FLAGS : uint32_t
	{
		CALL_EXPORT = (1 << 0), // Calls every export as DllMain
		HIDE_PE = (1 << 1), // Hides the PE header of the image by copying the header from ntdll
		ERASE_PE = (1 << 2), // Erases the PE header of the image
		CALL_NO_ENTRYPOINT = (1 << 3), // Doesn't call the entrypoint
		HIJACK_THREAD = (1 << 4), // Hijacks the thread
		WAIT_FOR_THREAD = (1 << 5), // Waits for the thread to finish
		NO_FLAGS = 0
	};

	BOOL LoadFileA(HANDLE hProcess, LPCSTR Path, DWORD Flags, WDLL*& wFile);
	BOOL LoadFileW(HANDLE hProcess, LPCWSTR Path, DWORD Flags, WDLL*& wFile);
	BOOL LoadFileFromResourcesA(HANDLE hProcess, HMODULE hDll, LPCSTR Name, LPCSTR ResourceType, DWORD Flags, WDLL*& wFile);
	BOOL LoadFileFromResourcesW(HANDLE hProcess, HMODULE hDll, LPCWSTR lpName, LPCWSTR ResourceType, DWORD Flags, WDLL*& wFile);
	BOOL LoadFileFromMemory(HANDLE hProcess, LPVOID lpDll, DWORD Flags, WDLL*& wFile);
	FARPROC GetProcAddressA(WDLL* wDll, LPCSTR lpName);
	FARPROC GetProcAddressW(WDLL* wDll, LPCWSTR lpName);
	BOOL UnloadFile(WDLL* wDll);
}
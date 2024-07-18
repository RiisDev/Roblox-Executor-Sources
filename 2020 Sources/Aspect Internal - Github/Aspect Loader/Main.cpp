#include <Windows.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <Psapi.h>
#include <urlmon.h>
#include <iomanip> 
#include <wininet.h>
#include <TlHelp32.h>
#include <winternl.h>
#include <strsafe.h>
#include <thread>
#include "CMap.hpp"
#include "Injector.hpp"

#pragma comment(lib,"ntdll")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib,"wininet.lib")

std::vector<std::uint32_t> GetProcessIds(const std::wstring& processName);
DWORD GetThreadId(DWORD dwProcessId);
uintptr_t BaseAddress;
HANDLE hProcess = INVALID_HANDLE_VALUE;
Injector inj;

void ErrorExit(const char* lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}

int main()
{
	std::cout << "[Aspect] Getting Files!" << std::endl;
	std::string folderpath = "C:\\Aspect Roblox";
	std::string dllpath = folderpath + "\\Aspect.dll";
	std::string dllurl = "https://aspectnetwork.net/aspect/internal/Aspect.dll";
	if (true)
	{
		if (CreateDirectory(folderpath.c_str(), NULL) ||
			ERROR_ALREADY_EXISTS == GetLastError())
		{
			DeleteUrlCacheEntry(dllurl.c_str());
			HRESULT hr = URLDownloadToFile(
				NULL,   // A pointer to the controlling IUnknown interface (not needed here)
				dllurl.c_str(),
				dllpath.c_str(),
				0,      // Reserved. Must be set to 0.
				NULL); // status callback interface (not needed for basic use)
			if (!SUCCEEDED(hr))
			{
				std::cout << "An error occured during file update!\nError code = 0x" << std::hex << hr << std::endl;
			}
		}
	}
	std::cout << "[Aspect] Files downloaded!" << std::endl;
	std::map<std::uint32_t, std::uint8_t> UsedProcessIds;
	bool DidWeInject = false;
	std::cout << "[Aspect] Waiting for Roblox!" << std::endl;
	while (true)
	{
		Sleep(250);
		auto ProcIds = GetProcessIds(std::wstring(L"RobloxPlayerBeta.exe"));
		for (auto Id : ProcIds)
		{
			if (DidWeInject)
			{
				Sleep(5000);
				return 1;
			}
			hProcess = 0;
			hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, Id);
			if (!hProcess)
			{
				std::cout << "[Aspect] OpenProcess failed with errorcode " << GetLastError() << std::endl;
				ErrorExit("OpenProcess");
			}

			std::cout << "[Aspect] Loading..." << std::endl;
			if (!inj.inject(Id, dllpath))
				std::cout << "[Aspect] Injection Failed!" << std::endl;
			else
				std::cout << "[Aspect] Loaded!" << std::endl;
			std::cout << "[Aspect] Loader closing in 5 seconds." << std::endl;
			DidWeInject = true;

			UsedProcessIds[Id] = 1;
			//delete Dll;
		}
	}
}

std::vector<std::uint32_t> GetProcessIds(const std::wstring& processName)
{
	std::vector<std::uint32_t> procs;
	PROCESSENTRY32W processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Snapshot == INVALID_HANDLE_VALUE) return procs;

	Process32FirstW(Snapshot, &processInfo);
	if (wcsstr(processName.c_str(), processInfo.szExeFile))
	{
		CloseHandle(Snapshot);
		procs.push_back(processInfo.th32ProcessID);
		return procs;
	}

	while (Process32NextW(Snapshot, &processInfo))
	{
		if (wcsstr(processName.c_str(), processInfo.szExeFile))
		{
			procs.push_back(processInfo.th32ProcessID);
		}
	}

	CloseHandle(Snapshot);
	return procs;
};

DWORD GetThreadId(DWORD dwProcessId)
{
	THREADENTRY32 threadinfo;
	threadinfo.dwSize = sizeof(threadinfo);

	HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessId);
	if (Snapshot == INVALID_HANDLE_VALUE) return false;

	Thread32First(Snapshot, &threadinfo);

	while (Thread32Next(Snapshot, &threadinfo))
	{
		if (threadinfo.th32ThreadID && threadinfo.th32OwnerProcessID == dwProcessId)
		{
			CloseHandle(Snapshot);
			return threadinfo.th32ThreadID;
		}
	}
	CloseHandle(Snapshot);
	return 0;
};
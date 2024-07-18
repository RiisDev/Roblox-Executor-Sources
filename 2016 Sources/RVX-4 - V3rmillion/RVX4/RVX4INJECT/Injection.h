#include <Windows.h>
#include <TlHelp32.h>
#include <Shlwapi.h>
#include <conio.h>
#include <stdio.h>
#include <iostream>
#define WIN32_LEAN_AND_MEAN

BOOL Inject(DWORD pID, const char * DLL_NAME);
DWORD GetTargetThreadIDFromProcName(const char * ProcName);
typedef LONG(NTAPI *NtSuspendProcess)(IN HANDLE ProcessHandle);
typedef LONG(NTAPI *NtResumeProcess)(IN HANDLE ProcessHandle);
std::string ws2s(const std::wstring& wstr)
{
	int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), 0, 0, 0, 0);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), &strTo[0], size_needed, 0, 0);
	return strTo;
}

bool InjectDLL(char* ProcessName)
{
	DWORD pID = GetTargetThreadIDFromProcName(ProcessName);
	char buf[MAX_PATH] = { 0 };
	GetFullPathNameA("Ryse.dll", MAX_PATH, buf, NULL);
	do
	{
		pID = GetTargetThreadIDFromProcName(ProcessName);
		Sleep(250);
	} while (pID == 0);
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
	NtSuspendProcess pfnNtSuspendProcess = (NtSuspendProcess)GetProcAddress(
		GetModuleHandle("ntdll"), "NtSuspendProcess");
	NtResumeProcess pfnNtResumeProcess = (NtSuspendProcess)GetProcAddress(
		GetModuleHandle("ntdll"), "NtResumeProcess");
	//std::cout << "ROBLOX PLAYER PROCESS HAS BEEN FOUND! WAITING FOR WINDOW TO INJECT. " << std::endl;
	while (true) {
		if (FindWindow(NULL, TEXT("ROBLOX")))  {
			LONG style = GetWindowLong(FindWindow(NULL, TEXT("ROBLOX")), GWL_STYLE);
			if (style & WS_VISIBLE)
				break;
		}
		Sleep(25);
	}
	pfnNtSuspendProcess(processHandle);
	if (!Inject(pID, buf)) {
		//printf("DLL HAS NOT INJECTED. PLEASE TRY AGAIN!");
		pfnNtResumeProcess(processHandle);
		return false;
	}
	else {
		//printf("DLL INJECTED INTO ROBLOX PLAYER.");
		pfnNtResumeProcess(processHandle);
		return true;
	}

	return false;
}

BOOL Inject(DWORD pID, const char * DLL_NAME) {
	HANDLE Proc;
	HMODULE hLib;
	char buf[50] = { 0 };
	LPVOID RemoteString, LoadLibAddy;

	if (!pID)
		return false;

	Proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
	if (!Proc)
		return false;

	LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
	RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(DLL_NAME), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(Proc, (LPVOID)RemoteString, DLL_NAME, strlen(DLL_NAME), NULL);
	CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, RemoteString, NULL, NULL);
	CloseHandle(Proc);
	return true;
}

DWORD GetTargetThreadIDFromProcName(const char * ProcName) {
	PROCESSENTRY32 pe;
	HANDLE thSnapShot;
	BOOL retval, ProcFound = false;

	thSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (thSnapShot == INVALID_HANDLE_VALUE)
		return false;

	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(thSnapShot, &pe);
	while (Process32Next(thSnapShot, &pe)) {
		WCHAR* temp = (WCHAR*)ProcName;
		char fileLoc[260];
		for (int i = 0; i<260; i++) {
			fileLoc[i] = pe.szExeFile[i];
		}
		if (_strcmpi(fileLoc, ProcName) == 0)
			return pe.th32ProcessID;
	}
	return 0; // if nothing is found I guess? :3
}
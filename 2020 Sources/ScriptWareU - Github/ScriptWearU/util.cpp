#include "util.h"
#include <iostream>

int Inverse(__int64 a, __int64 m) {
	__int64 m0 = m, t, q;
	__int64 x0 = 0, x1 = 1;
	while (a > 1)
	{
		q = a / m;
		t = m;
		m = a % m, a = t;
		t = x0;
		x0 = x1 - q * x0;
		x1 = t;
	}

	if (x1 < 0)
		x1 += m0;
	return x1;
}

VOID WriteReturn(int Address) {
	DWORD oldP;
	VirtualProtect((PVOID)Address, 1, PAGE_EXECUTE_READWRITE, &oldP);
	memset((PVOID)Address, 0xC3, 1);
	VirtualProtect((PVOID)Address, 1, oldP, &oldP);
}

BOOL WINAPI begin_console(LPCSTR console_title)
{
	/* i eat ass */
	DWORD n_old_protect;
	if (!VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, &n_old_protect))
		return FALSE;

	*(BYTE*)(FreeConsole) = 0xC3;
	if (!VirtualProtect(FreeConsole, 1, n_old_protect, &n_old_protect))
		return FALSE;

	AllocConsole();

	SetConsoleTitleA(console_title);
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
	freopen_s(&fp, "CONIN$", "r", stdin);
	std::cout.clear();
	return TRUE;
}

namespace util {
	/* credits to austin for these two functions */
	int sw_ReadFile(const std::string& path, std::string& out, unsigned char binary) {
		std::ios::openmode mode = std::ios::in;
		if (binary)
			mode |= std::ios::binary;

		std::ifstream file(path, mode);
		if (file.is_open()) {
			std::stringstream buffer;
			buffer << file.rdbuf();
			out = buffer.str();
			file.close();
			return 1;
		}

		file.close();
		return 0;
	}

	void get_file(const char* dllName, const char* fileName, char* buffer, int bfSize) {
		GetModuleFileName(GetModuleHandle(dllName), buffer, bfSize);
		if (strlen(fileName) + strlen(buffer) < MAX_PATH) {
			char* pathEnd = strrchr(buffer, '\\');
			strcpy(pathEnd + 1, fileName);
		}
		else {
			*buffer = 0;
		}
	}

	void pause() {
		THREADENTRY32 te32;
		te32.dwSize = sizeof(THREADENTRY32);
		HANDLE hThreads = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);
		if (Thread32First(hThreads, &te32)) {
			while (Thread32Next(hThreads, &te32)) {
				if (te32.th32OwnerProcessID == GetCurrentProcessId() && te32.th32ThreadID != GetCurrentThreadId()) {
					HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, false, te32.th32ThreadID);
					SuspendThread(hThread);
					CloseHandle(hThread);
				}
			}
		}
		CloseHandle(hThreads);
	}

	void resume() {
		THREADENTRY32 te32;
		te32.dwSize = sizeof(THREADENTRY32);
		HANDLE hThreads = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);
		if (Thread32First(hThreads, &te32)) {
			while (Thread32Next(hThreads, &te32)) {
				if (te32.th32OwnerProcessID == GetCurrentProcessId() && te32.th32ThreadID != GetCurrentThreadId()) {
					HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, false, te32.th32ThreadID);
					ResumeThread(hThread);
					CloseHandle(hThread);
				}
			}
		}
		CloseHandle(hThreads);
	}
}
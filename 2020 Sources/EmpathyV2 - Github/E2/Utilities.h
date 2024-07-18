#pragma once
#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
#include <locale>
#include <utility>
#include <codecvt>
#include <fstream>
#include <vector>
#include <sstream>
#include <Psapi.h>
#include <TlHelp32.h>

#include <WinInet.h>
#include <Shlwapi.h>

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "Shlwapi.lib")

#include "ConsoleCol.h"

unsigned __int64 _starting_tick;
unsigned __int64 _finish_tick;

#define ERRORMSGBOX(s, s1) MessageBox(NULL, s, s1, MB_ICONERROR | MB_OK);
#define INFOMSGBOX(s, s1) MessageBox(NULL, s, s1, MB_ICONINFORMATION | MB_OK); 

#define MSFOUND(o) throw std::exception("malicious item '" o "' found");
#define CHECKPROC(p) strcmp(entry.szExeFile, p) == 0

#define DebugPurple(...) std::cout << ipurple << "[" << __FUNCTION__ << "] -> " << __VA_ARGS__ << "\n";
#define DebugOutput(...) std::cout << white << "[" << __FUNCTION__ << "] -> " << __VA_ARGS__ << "\n";
#define DebugInfo(...) std::cout << iblue << "[" << __FUNCTION__ << "] -> " << __VA_ARGS__ << "\n";
#define DebugWarn(...) std::cout << iyellow << "[" << __FUNCTION__ << "] -> " << __VA_ARGS__ << "\n";
#define DebugError(...) std::cout << ired << "[" << __FUNCTION__ << "] -> " << __VA_ARGS__ << "\n";
#define DebugSuccess(...) std::cout << igreen << "[" << __FUNCTION__ << "] -> " << __VA_ARGS__ << "\n";



std::string CrashBoxCap;
std::string CrashBoxText;

namespace Timer {
	void Reset() {
		_starting_tick = 0;
		_finish_tick = 0;
	}

	void Start() {
		Reset();
		_starting_tick = GetTickCount64();
	}

	double GetElapsedTime() {
		if (_starting_tick & _finish_tick)
			return (_finish_tick - _starting_tick) / 1000.0;
		return 0.0;
	}

	double Stop() {
		_finish_tick = GetTickCount64();
		return GetElapsedTime();
	}
}

namespace Files {

	void GetFile(const char* dllName, const char* fileName, char* buffer, int bfSize) {
		GetModuleFileName(GetModuleHandle(dllName), buffer, bfSize);
		if (strlen(fileName) + strlen(buffer) < MAX_PATH) {
			char* pathEnd = strrchr(buffer, '\\');
			strcpy(pathEnd + 1, fileName);
		}
		else {
			*buffer = 0;
		}
	}

	long int GetFileSize(FILE* ifile) {
		long int fsize = 0;
		long int fpos = ftell(ifile);
		fseek(ifile, 0, SEEK_END);
		fsize = ftell(ifile);
		fseek(ifile, fpos, SEEK_SET);

		return fsize;
	}


	int ReadFile(const std::string& path, std::string& out, unsigned char binary) {
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

	int WriteFile(const std::string& path, std::string data, unsigned char binary) {
		std::ios::openmode mode = std::ios::out;
		if (binary)
			mode |= std::ios::binary;

		std::ofstream file(path, mode);
		if (file.is_open()) {
			file << data;
			file.close();
			return 1;
		}

		file.close();
		return 0;
	}

	std::string ReadFile(std::string path) {
		std::ios::openmode mode = std::ios::in;
		std::stringstream buffer;

		std::ifstream file(path, mode);
		if (file.is_open()) {
			buffer << file.rdbuf();
			file.close();
		}
		else return "";

		file.close();
		return buffer.str();
	}

	void WriteFile(std::string path, std::string data) {
		std::ios::openmode mode = std::ios::out;

		std::ofstream file(path, mode);
		if (file.is_open()) {
			file << data;
			file.close();
		}

		file.close();
	}

	void GetFilesInDirectory(std::vector<std::string>& out, const std::string& directory, unsigned char includePath) // thx stackoverflow
	{
		HANDLE dir;
		WIN32_FIND_DATA file_data;

		if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
			return; /* No files found */

		do {
			const std::string file_name = file_data.cFileName;
			const std::string full_file_name = directory + "/" + file_name;
			const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

			if (file_name[0] == '.')
				continue;

			if (is_directory)
				continue;

			out.push_back(includePath ? full_file_name : file_name);
		} while (FindNextFile(dir, &file_data));

		FindClose(dir);
	}

	int FileCheck(int pid, std::vector<std::string> list) {
		int count = 0;
		HANDLE proc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pid);

		if (proc) {
			char pathRaw[MAX_PATH];
			if (GetModuleFileNameEx(proc, NULL, pathRaw, MAX_PATH)) {
				std::string path = pathRaw;
				path = path.substr(0, path.find_last_of("\\/")); // remove file name

				std::vector<std::string> files;
				GetFilesInDirectory(files, path, false);

				for (std::vector<std::string>::iterator i = files.begin(); i != files.end(); i++) {
					for (std::vector<std::string>::iterator j = list.begin(); j != list.end(); j++) {
						if (*i == *j) count++;
					}
				}
			}
		}

		CloseHandle(proc);

		return count;
	}

}



namespace Processes {

	int GetProcessByImageName(const char* imageName) {
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		if (Process32First(snapshot, &entry) == TRUE)
		{
			while (Process32Next(snapshot, &entry) == TRUE)
			{
				if (strcmp(entry.szExeFile, imageName) == 0)
				{
					CloseHandle(snapshot);
					return entry.th32ProcessID;
				}
			}
		}

		CloseHandle(snapshot);
		return 0;
	}

	void Pause() {
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

	void Resume() {
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



namespace Registry {

	int ReadString(HKEY key, const char* value, std::string& out) {
		int val_sz;
		int val_type;
		char* val;

		if (RegQueryValueEx(key, value, NULL, (LPDWORD)& val_type, NULL, (LPDWORD)& val_sz) != ERROR_SUCCESS)
			return 0;

		if (val_type != REG_SZ || !val_sz)
			return 0;

		val = new (std::nothrow) char[val_sz];
		if (!val) return 0;

		if (RegQueryValueEx(key, value, NULL, NULL, (LPBYTE)val, (LPDWORD)& val_sz) != ERROR_SUCCESS) {
			delete[] val;
			return 0;
		}

		out = val;
		delete[] val;
		return 1;
	}

}



namespace Internet {

	using namespace std;

	string replaceAll(string subject, const string& search,
		const string& replace) {
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != string::npos) {
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
		return subject;
	}

	string DownloadURL(string URL) {
		HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
		HINTERNET urlFile;
		string rtn;
		if (interwebs) {
			urlFile = InternetOpenUrlA(interwebs, URL.c_str(), NULL, NULL, NULL, NULL);
			if (urlFile) {
				char buffer[2000];
				DWORD bytesRead;
				do {
					InternetReadFile(urlFile, buffer, 2000, &bytesRead);
					rtn.append(buffer, bytesRead);
					memset(buffer, 0, 2000);
				} while (bytesRead);
				InternetCloseHandle(interwebs);
				InternetCloseHandle(urlFile);
				string p = replaceAll(rtn, "|n", "\r\n");
				return p;
			}
		}
		InternetCloseHandle(interwebs);
		string p = replaceAll(rtn, "|n", "\r\n");
		return p;
	}
}




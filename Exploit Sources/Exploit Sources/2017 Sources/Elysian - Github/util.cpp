#include "util.h"
#include <WinInet.h>
#pragma comment(lib, "WinInet.lib")

#include <locale>
#include <utility>
#include <codecvt>

namespace util {
	athread::athread(void* func, void* args) {
		thread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)func, args, NULL, (LPDWORD)&threadID);
	}

	int athread::terminate() {
		if (this->running()) {
			if (TerminateThread(thread, 0)) {
				thread = NULL;
				threadID = 0;
			}
		}

		return getExitCode();
	}

	HANDLE athread::getThread() {
		return thread;
	}

	int athread::getExitCode() {
		int exitCode = 0;
		if (thread)
			GetExitCodeThread(thread, (LPDWORD)&exitCode);

		return exitCode;
	}

	int athread::getThreadId() {
		return threadID;
	}

	int athread::wait() {
		DWORD exitCode = 0;

		if (thread) {
			DWORD result;

			do {
				result = WaitForSingleObject(thread, 0);

				if (result == WAIT_OBJECT_0) {
					GetExitCodeThread(thread, &exitCode);
					break;
				}

				Sleep(100);

			} while (1);
		}

		return exitCode;
	}

	int athread::running() {
		return this->getExitCode() == STILL_ACTIVE;
	}

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

	int DownloadURL(const std::string& server, const std::string& path, const std::string& params, std::string& out, unsigned char useSSL, unsigned char dontCache, unsigned char direct) {
		HINTERNET interwebs = NULL;
		HINTERNET hConnect = NULL;
		HINTERNET hRequest = NULL;
		int rResults = 0;

		std::string path_w_params = path + (params.empty() ? "" : "?" + params);

		interwebs = InternetOpen("util/Agent", direct ? INTERNET_OPEN_TYPE_DIRECT : INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);

		if (interwebs)
			hConnect = InternetConnect(interwebs, server.c_str(), useSSL ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);

		if (hConnect)
			hRequest = HttpOpenRequest(hConnect, "GET", path_w_params.c_str(), NULL, NULL, NULL, (useSSL ? INTERNET_FLAG_SECURE : 0) | (dontCache ? INTERNET_FLAG_NO_CACHE_WRITE : 0), 0);

		if (hRequest)
			rResults = HttpSendRequest(hRequest, 0, 0, NULL, NULL);

		if (rResults) {
			char buffer[2000];
			DWORD bytesRead = 0;
			do {
				InternetReadFile(hRequest, buffer, 2000, &bytesRead);
				out.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			rResults = DLURL_SUCCESS;
		}
		else
			rResults = DLURL_FAILED_REQUEST;

		if (interwebs) InternetCloseHandle(interwebs);
		if (hConnect) InternetCloseHandle(hConnect);
		if (hRequest) InternetCloseHandle(hRequest);

		return rResults;
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

	std::vector<std::string> GetArguments(std::string input) {
		std::vector<std::string> rtn;

		if (input[0] == ' ') {
			input = input.substr(1);
		}

		BYTE size = input.size();
		DWORD pos1 = 0;

		for (int i = 0; i < size; ++i) {
			if (input[i] == ' ') {
				rtn.push_back(input.substr(pos1, i - pos1));
				pos1 = i + 1;
			}
			else if (i == size - 1) {
				rtn.push_back(input.substr(pos1, i - pos1 + 1));
				pos1 = i + 1;
			}
		}

		return rtn;
	}

	std::string GetRawStringAtDelim(std::string input, int arg, char delim) {
		char lc = 0;
		int c = 0;

		for (int i = 0; i < input.size(); ++i) {
			if (input[i] == delim && lc != delim)
				c++;

			if (c == arg)
				return input.substr(i + 1);
		}

		return "";
	}

	std::string lowercase(std::string& input) {
		std::string s;
		for (int i = 0; i < input.size(); ++i) {
			s.push_back(tolower(input[i]));
		}

		return s;
	}

	std::wstring s2ws(const std::string& str)
	{
		typedef std::codecvt_utf8<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(str);
	}

	std::string ws2s(const std::wstring& wstr)
	{
		typedef std::codecvt_utf8<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}

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

	void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory, unsigned char includePath) // thx stackoverflow
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

	/* Gets current time*/


	int __declspec(naked) GetEIP() {
		__asm pop eax
		__asm ret
	}

	void pause() {
		printf("Press enter to continue . . .");
		getchar();
	}

	namespace registry {
		int ReadString(HKEY key, const char* value, std::string& out) {
			int val_sz;
			int val_type;
			char* val;

			if (RegQueryValueEx(key, value, NULL, (LPDWORD)&val_type, NULL, (LPDWORD)&val_sz) != ERROR_SUCCESS)
				return 0;

			if (val_type != REG_SZ || !val_sz)
				return 0;

			val = new (std::nothrow) char[val_sz];
			if (!val) return 0;

			if (RegQueryValueEx(key, value, NULL, NULL, (LPBYTE)val, (LPDWORD)&val_sz) != ERROR_SUCCESS) {
				delete[] val;
				return 0;
			}

			out = val;
			delete[] val;
			return 1;
		}
	}
};
#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <fstream>
#include <sstream>
#include <vector>

int Inverse(__int64 a, __int64 m);
VOID OpenConsole(PCHAR Title);
BOOL WINAPI begin_console(LPCSTR console_title);

namespace util {
	void pause();
	void resume();

	int sw_ReadFile(const std::string& path, std::string& out, unsigned char binary);

	inline void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory, unsigned char includePath)
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

	void get_file(const char* dllName, const char* fileName, char* buffer, int bfSize);
}
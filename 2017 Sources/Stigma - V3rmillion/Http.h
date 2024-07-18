#pragma once
#include <Windows.h>
#include <stdio.h>
#include <WinInet.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

std::string DownloadURL(const char* URL) {
	HINTERNET interwebs = InternetOpen("Mozilla/5.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
	HINTERNET urlFile;
	std::string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrl(interwebs, URL, NULL, NULL, NULL, NULL);
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
			return rtn;
		}
	}
	InternetCloseHandle(interwebs);
	return rtn;
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

void writeMem(DWORD addr, char* val, int byten) {
	unsigned long oldp;
	VirtualProtect((LPVOID)addr, byten, PAGE_EXECUTE_READWRITE, &oldp);
	memcpy((LPVOID)addr, val, +byten);
	VirtualProtect((LPVOID)addr, byten, oldp, &oldp);
}
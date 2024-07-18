#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <TlHelp32.h>
#include <WinInet.h>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "wininet.lib")

using namespace std;



string ReplaceAll(string subject, const string& search,
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
			string p = ReplaceAll(rtn, "|n", "\r\n");
			return p;
		}
	}
	InternetCloseHandle(interwebs);
	string p = ReplaceAll(rtn, "|n", "\r\n");
	return p;
}

std::vector<std::string> split_string(const std::string& str, char delimiter) {
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string tok;

	while (std::getline(ss, tok, delimiter))
		tokens.push_back(tok);

	return tokens;
}


#pragma once

#include <Windows.h>
#include <WinINet.h>
#include <IOStream>
#include <String>

#pragma comment(lib, "WinINet.lib")

std::string replaceAll(std::string subject, const std::string& search,
	const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	};
	return subject;
};

std::string DownloadURL(std::string URL) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	std::string rtn;
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
			std::string p = replaceAll(rtn, "|n", "\r\n");
			return p;
		};
	};
	InternetCloseHandle(interwebs);
	std::string p = replaceAll(rtn, "|n", "\r\n");
	return p;
};

std::string hwidchecker() {
	TCHAR value[255];
	DWORD BufferSize = 255;
	HKEY key = NULL;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, ("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &key);
	int a = RegGetValue(key, "", ("ProductId"), RRF_RT_ANY, NULL, (PVOID)&value, &BufferSize);
	return(value);
};

void auth() {
	std::string hostfile = "http://xyz.potato-host.com/puddingWhitelistCheck.php?key=";
	std::string hot = hwidchecker();
	std::string result = DownloadURL(hostfile += hot);
	if (result == "1") {
		std::cout << "Success, enjoy.\n";
		ShellExecute(0, 0, "https://discord.gg/ygbrRC3", 0, 0 , SW_SHOW);
	}
	else {
		std::cout << "You're not whitelisted, send the hwid below to Pudding Mug#6560\n" << hot.c_str();
		Sleep(10000);
		exit(10);
	};
};
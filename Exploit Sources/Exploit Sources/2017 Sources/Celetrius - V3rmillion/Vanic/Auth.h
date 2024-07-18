#pragma once
#include "SDK.h"

std::string replaceAll(std::string subject, const std::string& search,
	const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}


std::string DownloadURL(const char* URL) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	std::string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrlA(interwebs, URL, NULL, NULL, NULL, NULL);
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
		}
	}
	InternetCloseHandle(interwebs);
	std::string p = replaceAll(rtn, "|n", "\r\n");
	return p;
}

void FindFiddler() {
	if (FindWindow(NULL, "Fiddler Web Debugger")) {
		printf("Detected that you are running a blacklisted program!\n");
		Sleep(5020 * 25);
		exit(-1);
	}
}

void Auth() {
	CHAR szVolumeNameBuffer[12];
	DWORD dwVolumeSerialNumber;
	DWORD dwMaximumComponentLength;
	DWORD dwFileSystemFlags;
	CHAR szFileSystemNameBuffer[10];

	GetVolumeInformationA("C:\\", szVolumeNameBuffer, 12, &dwVolumeSerialNumber, &dwMaximumComponentLength, &dwFileSystemFlags, szFileSystemNameBuffer, 10);
	std::string HDDSerial = std::to_string(dwVolumeSerialNumber);

	FindFiddler();

	std::string url = (std::string)VMProtectDecryptStringA("http://illuminatedtriangle.com/vm.php?userid=");
	url += HDDSerial;

	std::string Response = DownloadURL(url.c_str());

	if (Response == std::string(VMProtectDecryptStringA("success"))) {
		printf("Authentication successful!\n");
	}
	else {
		printf("Failed to authenticate.\r\n");
		Sleep(1000000 * 2);
		exit(-1);
	}

}
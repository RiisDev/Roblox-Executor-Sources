#include "Includes.h"
#pragma comment(lib, "wininet")

using namespace std;

DWORD dwVolumeSerialNumber;
DWORD dwMaxNameLength;
DWORD dwFileSystemFlags;
TCHAR szFileSysName[128];
TCHAR szLabel[128];
char szBuffer[MAX_PATH + 100] = "C:\\";


int GrabHWID()
{


	if (!GetVolumeInformation((LPCSTR)szBuffer, szLabel,
		sizeof(szLabel) - 1,
		&dwVolumeSerialNumber,
		&dwMaxNameLength,
		&dwFileSystemFlags,
		szFileSysName,
		sizeof(szFileSysName) - 1))
	{
		szLabel[0] = 0;
		szFileSysName[0] = 0;
	}



	cout << "HWID is : " << dwVolumeSerialNumber << endl;
	return 1;
}


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
	if (!GetVolumeInformation((LPCSTR)szBuffer, szLabel,
		sizeof(szLabel) - 1,
		&dwVolumeSerialNumber,
		&dwMaxNameLength,
		&dwFileSystemFlags,
		szFileSysName,
		sizeof(szFileSysName) - 1))
	{
		szLabel[0] = 0;
		szFileSysName[0] = 0;
	}

	std::string HDDSerial = std::to_string(dwVolumeSerialNumber);

	FindFiddler();

	std::string url = DownloadURL("https://projectangel.000webhostapp.com/auth.php?a=");
	url += HDDSerial;

	if (url == std::string("true")) {
		printf("Authentication successful!\n");
	}
	else {
		printf("Failed to authenticate.\r\n");

		exit(-1);
	}

}
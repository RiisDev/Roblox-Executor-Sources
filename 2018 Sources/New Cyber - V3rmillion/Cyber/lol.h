#pragma once
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "wininet.lib")
bool hostsok = true;



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

string DownloadURL(string URL) { // eww
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



//WAIT BRO


std::string hwidchecker() {
	TCHAR value[255];
	DWORD BufferSize = 255;
	HKEY key = NULL;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, ("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &key);
	int a = RegGetValue(key, "", ("ProductId"), RRF_RT_ANY, NULL, (PVOID)&value, &BufferSize);
	return(value);

}//this is what I mean it just dies
 // can i give you a good song to put on? in a bit



bool checkHostsFile()
{
	ifstream infile;
	infile.open("C:\\Windows\\System32\\drivers\\etc\\hosts", ifstream::in);
	if (infile.is_open())
	{
		while (infile.good())
		{
			string line = "";
			getline(infile, line);
			if (line.find("http://xyz.potato-host.com") != string::npos)
			{

				return true;
			}
		}
		infile.close();
		return false;
	}
	else
	{

		hostsok = false;
		return true;
	}
}

void auth() {
	string hostfile = "your whitelist url"; //change trial to nothing after
	string hot = hwidchecker();
	string result = DownloadURL(hostfile + hot);
	if (result == "1") {
		cout << "Success, enjoy.\n";

	}
	else {
		cout << "You're not whitelisted, send the hwid below to Pudding Mug#6560\n" << hot.c_str();
		Sleep(10000);
		exit(10);

	}
}


#include <stdio.h>
#include <windows.h>
#include <TlHelp32.h>




void yes() {
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			if (stricmp(entry.szExeFile, "Fiddler.exe") == 0)
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
				TerminateProcess(hProcess, 1);
				CloseHandle(hProcess);
			}
		}
	}
	CloseHandle(snapshot);
}
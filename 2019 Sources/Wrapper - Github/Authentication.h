#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
#include <WinInet.h>

#pragma comment(lib, "wininet.lib")

bool ispremiumuser = false;
bool firstime = false;

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

std::string getHwid() {
	TCHAR value[255];
	DWORD BufferSize = 255;
	HKEY key = NULL;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, ("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &key);
	int a = RegGetValue(key, "", ("ProductId"), RRF_RT_ANY, NULL, (PVOID)&value, &BufferSize);
	return(value);
}


std::string getdiscordid() {
	TCHAR value[255];
	DWORD BufferSize = 255;
	HKEY key = NULL;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, ("SOFTWARE\\FaggotSploit"), 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &key);
	int a = RegGetValue(key, "", ("DiscordID"), RRF_RT_ANY, NULL, (PVOID)&value, &BufferSize);
	return(value);
}

std::vector<std::string> split_string(const std::string &str, char delimiter) {
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string tok;

	while (std::getline(ss, tok, delimiter))
		tokens.push_back(tok);

	return tokens;
}


void yesnt(std::string localplayername) {
	std::string ok1 = getHwid();
	std::string DiscordID = "533971977191555072";
	std::string ok = "https://pandabrowser.000webhostapp.com/Fag/RunCommand.php?did=";
	std::string builturl = ok + DiscordID + "&hw=" + ok1 + "&pn=" + localplayername;
	std::string resok = DownloadURL(builturl);
	std::vector<string> yes = split_string(resok, '|');


	if (yes.at(0) == "BL")
	{
		ShowWindow(GetConsoleWindow(), SW_HIDE);
		std::string blres = "You Were Blacklisted For: " + yes.at(1);
		MessageBoxA(NULL, blres.c_str(), "Error!", MB_ICONERROR | MB_OK);
		exit(0);
	}

	if (resok == "HDGHASGDJHASGDHJJJKHFJKA")
	{
		ispremiumuser = true;
		std::cout << igreen << "Success!\n";
		return;
	}

	if (resok == "DJASGHDFJHASGHHJFAGS")
	{
		ispremiumuser = true;
		firstime = true;
		std::cout << igreen << "Success!\n";
		return;
	}

	if (resok == "NOTWL")
	{
		std::cout << iyellow << "Free User!\n";
		return;
	}

	std::cout << ired << "Unexpected Error!\n";
	system("pause");
	exit(0);
}
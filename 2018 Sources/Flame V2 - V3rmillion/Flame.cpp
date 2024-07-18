/* Includes */

#include "Data.h"
#include "Lua.h"
#include "RunCommand.h"
#include "LuaFunctions.h"
#include "Libary.h"

std::string Replace(std::string subject, const std::string& search,
	const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

std::string DownloadString(std::string URL) {
	VMProtectBeginMutation("a");
	HINTERNET interwebs = InternetOpen("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	std::string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrl(interwebs, URL.c_str(), NULL, NULL, NULL, NULL);
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
			std::string p = Replace(rtn, "|n", "\r\n");
			return p;
		}
	}
	InternetCloseHandle(interwebs);
	std::string p = Replace(rtn, "|n", "\r\n");
	VMProtectEnd();
	return p;
}

inline void FlameCPipe() {
	char buff[1024];
	DWORD dwRead;
	HANDLE Pipe;

	Pipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\FlameLC"),
		PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
		PIPE_WAIT,
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);


	while (Pipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(Pipe, NULL) != FALSE)
		{
			while (ReadFile(Pipe, buff, sizeof(buff) - 1, &dwRead, NULL) != FALSE)
			{
				buff[dwRead] = '\0';
				if (strlen(buff) != NULL && Whitelisted) {
					LuaC lc;
					luaC result = lc.do_string(buff);
					if (result.errors == 1) {
						char* ErrorBuffer = new char[1024]();
						sprintf(ErrorBuffer, "Lua C Error: %s.", result.error);
						EventLog::Log(MESSAGE_ERROR, ErrorBuffer);
					}
				}
			}
		}
		DisconnectNamedPipe(Pipe);
	}
}



inline void FlamePipe() {
	char buff[1024];
	DWORD dwRead;
	HANDLE Pipe;

	Pipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Flame"),
		PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
		PIPE_WAIT,
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);

	
	while (Pipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(Pipe, NULL) != FALSE)
		{
			while (ReadFile(Pipe, buff, sizeof(buff) - 1, &dwRead, NULL) != FALSE)
			{
				buff[dwRead] = '\0';
				if (strlen(buff) != NULL && Whitelisted) {
					RunCommand(buff);
				}
			}
		}
		DisconnectNamedPipe(Pipe);
	}
}


static void Elevation() {
	do {
		*(int*)(IdentityFlag) = 7;
	} while (true);
}

static std::vector<std::string> Split(std::string String) {
	std::stringstream test(String);
	std::string segment;
	std::vector<std::string> seglist;

	while (std::getline(test, segment, ' ')) {
		seglist.push_back(segment);
	}

	return seglist;
}


static int Scan() {
	try {
		ScriptContext = Scan(PAGE_READWRITE, (char*)&ScriptContextVFTable, "xxxx");

		L = *(DWORD*)(ScriptContext + 220) - (ScriptContext + 220);
		DataModel = *(int*)(ScriptContext + 0x34);
		Workspace = FindFirstClass(DataModel, "Workspace");
		Players = FindFirstClass(DataModel, "Players");

		FilteringProperty = *(int*)(Workspace + 0x25B);
		PlayerInstance = *(int*)(Players + 0x154);
		PlayerName = (std::string*)(*(int*)(PlayerInstance + 0x28));

		
		return 1;
	}
	catch (...) {
		throw std::exception();
	}

	return 0;
}


void Main()
{
	int ScanError = Scan();

	if (ScanError != 0)
	{
		std::string URL = std::string("http") + std::string("://www") + std::string(".flame-") + std::string("white") + std::string("list") + std::string(".co") + std::string("m.gri") + std::string("dhoste") + std::string("d.co.uk") + std::string("/Fla") + std::string("me/a") + std::string("uthenti") + std::string("cato") + std::string("r.php?a=");
		
		HW_PROFILE_INFO hwProfileInfo;

		if (GetCurrentHwProfile(&hwProfileInfo) != NULL) {
			URL += hwProfileInfo.szHwProfileGuid;
		}

		std::string Response = DownloadString(URL);

		Whitelisted = (Response == "1") ? true : false;

		if (Whitelisted) {
			char* MessageBuffer = new char[1024]();
			sprintf(MessageBuffer, "Welcome to Flame, %s!", (char*)PlayerName);
			EventLog::Log(MESSAGE_ERROR, MessageBuffer);
			EventLog::Log(MESSAGE_WARN, "Developed by Aero @ Vermillion.net");

			NotifyRaw(MessageBuffer);
			NotifyRaw(CheckFiltering(FilteringProperty));

			do {
				EventLog::Log(MESSAGE_ERROR, (char*)(*(int*)(PlayerInstance + 0x23C)));
				Sleep(50);
			} while (true);
		}
		else {
			TerminateProcess(GetCurrentProcess(), NULL);
		}
	}
	else {
		::MessageBox(NULL, "Addresses are outdated!", "", MB_OK);
		TerminateProcess(GetCurrentProcess(), NULL);
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Main, 0, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Elevation, 0, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)FlamePipe, 0, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ExecuteChat, 0, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)LoadChat, 0, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ClickFunctions, 0, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Hotkeys, 0, 0, 0);
		//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)FlameCPipe, 0, 0, 0);

	}

	return 1;
}
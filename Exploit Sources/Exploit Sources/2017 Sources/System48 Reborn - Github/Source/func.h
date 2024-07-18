#include "Includes/SysIncludes.h"

DWORD DataModel;
DWORD Script_Context;
DWORD Players;
DWORD LocalPlayer;
DWORD Workspace;
DWORD Lighting;

LONG RobloxWndProc;
HWND RobloxWindow;

bool accessfailed = false;
bool loadeddir = false;

bool changelogfunctionfailure = false;
bool changelogfilenotfound = false;

enum Identities {
	Anonymous = 0,
	LocalGUI_,					// Any action initiated by Roblox Studio or the mouse 
	GameScript_,				// Execution of a BaseScript object inside any DataModel
	GameScriptInRobloxPlace_,	// Execution of a BaseScript object inside any DataModel, if the place was authored by Roblox
	RobloxGameScript_,			// Execution of a BaseScript object written by Roblox inside any DataModel
	CmdLine_,					// Any script executed from the Studio command line
	COM,						// Scripts executed via the COM API (usually comes from roblox.com)
	WebService,					// Scripts executed via the Web Service API (usually comes from roblox.com)
	Replicator_,				// Receiving data via replication
	COUNT_Identities            // Not a true identity. Used for enumeration
};

enum Permissions {
	None = 0,					// Any identity can access this feature, including in-game scripts
	Plugin = 1,					// Second-lowest access level, just above in-game script
	RobloxPlace = 2,		    // A Roblox place that we own. Therefore scripts are more trusted and we allow
	LocalUser = 3,			    // non-game permission. Usually for IDE
	WritePlayer = 4,			// Permissions for changing player name, userId, etc.
	RobloxScript = 5,			// A script, such as a CoreScript, that we run inside a game
	Roblox = 6,					// Highest level of permission
	TestLocalUser = LocalUser
};

namespace Context {
	bool Is_in_role(Identities identity, Permissions p) {
		bool result;
		bool v3;

		if (p) {
			switch (identity) {
			case COM:
			case WebService:
				goto LABEL_2;
			case GameScriptInRobloxPlace_:
				return p == RobloxPlace;
			case RobloxGameScript_:
				if (p == Plugin || p == RobloxPlace)
					goto LABEL_11;
				v3 = p == LocalUser;
				goto LABEL_8;
			case LocalGUI_:
			case CmdLine_:
				if (p == Plugin || p == RobloxPlace || p == LocalUser)
					goto LABEL_11;
				result = false;
				break;
			case Replicator_:
				if (p == RobloxPlace)
					goto LABEL_11;
				v3 = p == WritePlayer;
			LABEL_8:
				if (v3)
					goto LABEL_11;
				if (p == RobloxScript)
					LABEL_11:
				result = true;
				else
					result = false;
				break;
			default:
				result = false;
				break;
			}
		}
		else
		{
		LABEL_2:
			result = true;
		}
		return result;
	}
}


namespace func {

	std::vector<std::string> SplitString(std::string str, char delimiter) {
		std::vector<std::string> tokens;
		std::stringstream ss(str);
		std::string tok;

		while (std::getline(ss, tok, delimiter))
			tokens.push_back(tok);

		return tokens;
	}

	int getParent(DWORD Instance) {
		__asm {
			mov ecx, Instance
			mov eax, [ecx + 0x34]
		}
	}

	const char* getClassName(DWORD Instance) {
		__asm {
			mov ecx, Instance
			mov eax, [ecx]
			call dword ptr[eax + 0x10]
		}
	}

	std::string getName(DWORD Instance) {
		std::string* Disgusting;
		__asm {
			mov ecx, Instance
			mov eax, [ecx + 0x28]
			mov Disgusting, eax
		}
		return Disgusting->c_str();
	}

	//int GetGlobalState(DWORD* Instance) {
		//DWORD *v2 = Instance;

//		return v2[14 * 0 + 41] - (DWORD)&v2[14 * 0 + 41];
//	}

	DWORD getInstanceParent(DWORD Instance) {
		__asm {
			mov ecx, Instance
			mov eax, [ecx + 0x34]
		}
	}

	const char* getInstanceClassName(DWORD Instance) {
		__asm {
			mov ecx, Instance
			mov eax, [ecx]
			call dword ptr[eax + 0x10]
		}
	}

	int findFirstClass(DWORD Instance, const char* Name)
	{
		DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
		DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);

		for (DWORD i = *(DWORD*)StartOfChildren; i != EndOfChildren; i += 8)
		{
			if (memcmp(getClassName(*(DWORD*)i), Name, strlen(Name)) == 0)
			{
				return *(DWORD*)i;
			}
		}
	}

	std::vector<int> getChildren(DWORD Instance) {
		DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
		DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
		std::vector<int> Children;
		for (DWORD i = *(DWORD*)StartOfChildren; i != EndOfChildren; i += 8)
		{
			Children.push_back(*(DWORD*)i);
		}
		return Children;
	}

	DWORD findFirstChild(DWORD Instance, std::string ClassName) {
		std::vector<int> InstanceChildren = getChildren(Instance);
		for (int c = 0; c < InstanceChildren.size(); c++) {
			if (getInstanceClassName(InstanceChildren[c]) == ClassName) {
				return InstanceChildren[c];
			}
		}
		return 0;
	}

	DWORD getService(const char* Service) {
		return findFirstChild(DataModel, Service);
	}

	LRESULT CALLBACK PatchedWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	void HookRobloxWndProc() {
		SetWindowLongPtr(RobloxWindow, GWLP_WNDPROC, (LONG)PatchedWndProc);
	}

	void RestoreRobloxWndProc() {
		SetWindowLongPtr(RobloxWindow, GWLP_WNDPROC, RobloxWndProc);
	}

	void StopRobloxInput() {
		RobloxWindow = FindWindowW(0, L"ROBLOX");
		RobloxWndProc = GetWindowLongPtr(RobloxWindow, GWLP_WNDPROC);
		while (1) {
			if (GetForegroundWindow() == sys->MainWindow) {
				HookRobloxWndProc();
			}
			else {
				RestoreRobloxWndProc();
			}
			Sleep(80);
		}
	}

	template<typename Out>
	void split(const std::string &s, char delim, Out result) {
		std::stringstream ss;
		ss.str(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			*(result++) = item;
		}
	}

	std::string getInstanceName(DWORD Instance) {
		std::string* Disgusting;
		__asm {
			mov ecx, Instance
			mov eax, [ecx + 0x28]
			mov Disgusting, eax
		}
		return Disgusting->c_str();
	}

	std::vector<int> GetPlayers(std::string Name) {
		std::vector<int> P;
		std::vector<std::string> Arguments = SplitString(Name, ',');

		for (DWORD i = 0; i < Arguments.size(); i++) {
			std::string Input = Arguments[i];
			transform(Input.begin(), Input.end(), Input.begin(), tolower);

			if (Input == "me" || Input == "Me") {
				P.push_back(LocalPlayer);
			}

			else if (Input == "all" || Input == "All") {
				P = getChildren(Players);
			}

			else if (Input == "others" || Input == "Others") {
				std::vector<int> P2 = getChildren(Players);
				for (DWORD i = 0; i < P2.size(); i++) {
					if (getName(P2[i]) != getName(LocalPlayer)) {
						P.push_back(P2[i]);
					}
				}
			}

			else if (Input == "guests" || Input == "Guests" || Input == "guest" || Input == "Guest") {
				std::vector<int> Plrs = getChildren(Players);
				for (int i = 0; i < Plrs.size(); i++) {
					if (getName(Plrs[i]).find("Guest ") != std::string::npos) {
						P.push_back(Plrs.at(i));
					}
				}
			}
			else if (Input == "random" || Input == "Random") {
				std::vector<int> Plrs = getChildren(Players);
				if (Plrs.size() > 0) {
					std::random_shuffle(Plrs.begin(), Plrs.end());
					P.push_back(Plrs[0]);
				}
			}

			else {
				std::vector<int> P2 = getChildren(Players);
				for (DWORD i = 0; i < P2.size(); i++) {
					std::string P2Name = getName(P2[i]);
					transform(P2Name.begin(), P2Name.end(), P2Name.begin(), tolower);
					if (P2Name.substr(0, Input.length()) == Input) {
						P.push_back(P2[i]);
					}
				}
			}
		}
		return P;
	}

	std::vector<int> GetPlayers2(std::string Name) {
		std::vector<int> P;
		std::vector<std::string> Arguments = SplitString(Name, ',');

		for (DWORD i = 0; i < Arguments.size(); i++) {
			std::string Input = Arguments[i];
			transform(Input.begin(), Input.end(), Input.begin(), tolower);

			if (Input == "me" || Input == "Me") {
				P.push_back(LocalPlayer);
			}

			else {
				std::vector<int> P2 = getChildren(Players);
				for (DWORD i = 0; i < P2.size(); i++) {
					std::string P2Name = getName(P2[i]);
					transform(P2Name.begin(), P2Name.end(), P2Name.begin(), tolower);
					if (P2Name.substr(0, Input.length()) == Input) {
						P.push_back(P2[i]);
					}
				}
			}
		}
		return P;

	}


	std::string replaceAll(std::string subject, const std::string& search, const std::string& replace) {
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos) {
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
		return subject;
	}

	std::string GetMachineID()
	{
		VMProtectBeginMutation("Sys Function: GetMachineID | Mutation");
		std::string ss;
		ss = "Err_StringIsNu";
		UCHAR szFileSys[255],
			szVolNameBuff[255];
		DWORD dwSerial;
		DWORD dwMFL;
		DWORD dwSysFlags;
		int error = 0;

		bool success = GetVolumeInformation(LPCTSTR("C:\\"), (LPTSTR)szVolNameBuff,
			255, &dwSerial,
			&dwMFL, &dwSysFlags,
			(LPTSTR)szFileSys,
			255);
		if (!success) {
			ss = "Err_Not_Elevated";
		}
		std::stringstream errorStream;
		errorStream << dwSerial;
		VMProtectEnd();
		return std::string(errorStream.str().c_str());
	}

	inline std::string DownloadURL(std::string URL) {
		VMProtectBeginVirtualization("SysFunction: DownloadURL | Virtualization (Used to stop detection of host)");
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
			}
		}
		InternetCloseHandle(interwebs);
		std::string p = replaceAll(rtn, "|n", "\r\n");
		VMProtectEnd();
		return p;
	}

	static std::string base64_decode(const std::string &in) {
		VMProtectBeginMutation("Sys Function: Base64 Encryption | Mutation");
		std::string out;

		std::vector<int> T(256, -1);
		for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

		int val = 0, valb = -8;
		for (unsigned char c : in) {
			if (T[c] == -1) break;
			val = (val << 6) + T[c];
			valb += 6;
			if (valb >= 0) {
				out.push_back(char((val >> valb) & 0xFF));
				valb -= 8;
			}
		}
		VMProtectEnd();
		return out;
	}

	int getGlobalState(DWORD* Instance) {
		DWORD *v2 = Instance;

		return v2[14 * 0 + 41] - (DWORD)&v2[14 * 0 + 41];
	}

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


	inline int FileCheck(int pid, std::vector<std::string> list) {
		int count = 0;
		HANDLE proc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pid);

		if (proc) {
			char pathRaw[MAX_PATH];
			if (GetModuleFileNameEx(proc, NULL, pathRaw, MAX_PATH)) {
				std::string path = pathRaw;
				path = path.substr(0, path.find_last_of("\\/"));

				std::vector<std::string> files;
				GetFilesInDirectory(files, path, false);

				for (std::vector<std::string>::iterator i = files.begin(); i != files.end(); i++) {
					for (std::vector<std::string>::iterator j = list.begin(); j != list.end(); j++) {
						if (*i == *j) count++;
					}
				}
			}
		}

		CloseHandle(proc);

		return count;
	}

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

	std::string currentver = ROBLOXVERSION;
	std::string site = DownloadURL("http://setup.roblox.com/version");
}
	
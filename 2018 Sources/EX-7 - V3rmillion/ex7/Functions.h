#include "Includes/Includes.h"
#include "form.h"

DWORD Script_Context;
DWORD Players;
DWORD LocalPlayer;
DWORD Lighting;
DWORD DataModel;

LONG RobloxWndProc;
HWND RobloxWindow;

bool accessfailed = false;
bool loadeddir = false;

bool changelogfunctionfailure = false;
bool changelogfilenotfound = false;

/* behold a bunch of random useless SHIT thrown into one DISGUSTING namespace for NO reason */

namespace func {
	int decrypt_ptr(int ptr) {
		return *(int*)(ptr)+(ptr);
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

	DWORD getGlobalState(DWORD* ScriptContext, DWORD Index) {
		DWORD* SC = ScriptContext;
		DWORD indx = Index;

		return SC[14 * indx + 41] ^ (DWORD)&SC[14 * indx + 41];
	}

	std::string getInstanceName(DWORD Instance) {
		std::string* Disgusting;
		__asm {
			mov ecx, Instance
			mov eax, [ecx + 0x28]
			mov Disgusting, eax
		}
		return Disgusting->c_str(); //was not lying
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
		return std::string(errorStream.str().c_str());
	}

	inline std::string DownloadURL(std::string URL) {
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
		return p;
	}

	static std::string base64_decode(const std::string &in) {
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
		return out;
	}

	void RemoveStyle(HWND hwnd, int style)
	{
		//skid elysian (once again, why are you suprised?)
		LONG lExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
		lExStyle &= ~(style);
		SetWindowLong(hwnd, GWL_EXSTYLE, lExStyle);
	}

}

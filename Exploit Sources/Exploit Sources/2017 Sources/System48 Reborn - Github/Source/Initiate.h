#include "form.h"
#include "Includes/SysIncludes.h"
#include "HandleCommands.h"

bool MaliciousItemFound = false;

#define MSFOUND(o) throw std::exception("detected '" o "'");
#define CHECKPROC(p) strcmp(entry.szExeFile, p) == 0
#define CHECKMALICIOUS

/* Initation Header | Used for the whitelist function and file scanning */

DWORD WINAPI MessageBoxA_hook(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
	MessageBoxW(hWnd, L"System48 has hit an unexpected error and has to exit!", L"EX-7 Crash", MB_ICONERROR | MB_OK);
	std::exit(1);
	return 0;
}

void catch_rbx_crash() {
	DWORD oldP;
	VirtualProtect(&MessageBoxA, 5, PAGE_EXECUTE_READWRITE, &oldP);
	*(byte*)(&MessageBoxA) = 0xE9;
	*(DWORD*)((DWORD)&MessageBoxA + 1) = ((DWORD)&MessageBoxA_hook - (DWORD)&MessageBoxA) - 5;
}

namespace Whitelist {

	void EnableTrial() {
		VMProtectBeginVirtualization("Sys Function: EnableTrial | Mutation");
        #ifndef DISABLETRIAL
		std::string key = "Vt3VoR8QYs2apTfuBG2ZOOSFwJspp2QhNtQx8AUQ9EN3AFno8T";
		std::string host = DownloadURL("https://buyex7.com/system48trail.html");

		if (key != host) {
			sys->Print(RGB(255, 0, 0), "Trial is over!\nIn order to continue you need to purchase\nSystem48 via the discord (https://discord.gg/CxdCPQB)\n");
			throw std::exception("trial");
		}

		else {
			sys->Print(RGB(0, 0, 200), "Done!\n");
			sys->SysInit();
		}
        #endif
		VMProtectEnd();
	}

	void BeginInit() {
		VMProtectBeginUltra("Sys Function: Whitelist | Ultra");
		//catch_rbx_crash();

		EnableWindow(sys->InputField, false);

		sys->Print(RGB(0, 0, 200), "Welcome to System48 v%s\n", SYSTEM48_VERSION);
		sys->Print(RGB(0, 0, 0), "\nAuthenticating User... ");

#define DISABLE_WHITELIST
        #ifndef DISABLE_WHITELIST

		/*-- Begin main Authorization sequence --*/
		sys->UpdateCheck();

		std::string hwidhere = GetMachineID();
		std::string url = DownloadURL("https://www.timberman.xyz/AzuLXHosting/Auth2.php?hwid=" + hwidhere);

		url = base64_decode(url);
		unsigned long long int url2;
		sscanf_s(url.c_str(), "%llu", &url2);
		unsigned long long int hwidhere2;
		sscanf_s(hwidhere.c_str(), "%llu", &hwidhere2);

		if (((url2 - 420) / 69) == hwidhere2) {
			sys->Print(RGB(0, 0, 200), "Done!\n");
			sys->SysInit();
		}

		else {
			sys->Print(RGB(255, 0, 0), "Error! User is not whitelisted\n");
			throw std::exception("dumb knoop");
		}

        #else
		sys->Print(RGB(0, 0, 200), "Done!\n");
        sys->SysInit();
        #endif

        #ifndef DISABLETRIAL
		EnableTrial();
        #endif

		VMProtectEnd();
	}
}

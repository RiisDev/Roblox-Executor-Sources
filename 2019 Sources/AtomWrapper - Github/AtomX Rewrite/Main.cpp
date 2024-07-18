#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <Wininet.h>
#pragma comment(lib, "wininet.lib")
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")

#include "memory.h"
#include "Wrapper.h"
#include "Functions.h"
#include "MinHook.h"
using namespace Wrapper;
using namespace std;

DWORD WINAPI input(PVOID lvpParameter)
{
	string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\AtomWrapper"),
		PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
		PIPE_WAIT,
		1,
		999999,
		999999,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';
				try {
					try {
						WholeScript = WholeScript + buffer;
					}
					catch (...) {
					}
				}
				catch (std::exception e) {

				}
				catch (...) {

				}
			}
			ExecuteScript(WholeScript);
			WholeScript = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}

#include <signal.h>
//----------------------------------------\\
//credits to louka for consolebypass
BOOL WINAPI CONSOLEBYPASS()
{
	DWORD nOldProtect;
	if (!VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, &nOldProtect))
		return FALSE;
	*(BYTE*)(FreeConsole) = 0xC3;
	if (!VirtualProtect(FreeConsole, 1, nOldProtect, &nOldProtect))
		return FALSE;	
	AllocConsole();
}


void VEH_Bypass() {
	HMODULE mod = GetModuleHandleA("ntdll.dll");
	DWORD old;
	DWORD KIException = cast(DWORD, GetProcAddress(mod, "KiUserExceptionDispatcher"));
	VirtualProtect((LPVOID)KIException, 1, PAGE_EXECUTE_READWRITE, &old);
	for (int i = 0; i < 24; i++) {
		*(char*)(KIException + i) = 0x90;
	}
	VirtualProtect((LPVOID)KIException, 1, old, &old);
}

inline std::string download_url(const std::string &url) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	std::string rtn;

	if (interwebs) {
		HINTERNET url_file = InternetOpenUrlA(interwebs, url.c_str(), NULL, NULL, NULL, NULL);

		if (url_file) {
			char buffer[2000];
			DWORD bytes_read;

			do {
				InternetReadFile(url_file, buffer, 2000, &bytes_read);
				rtn.append(buffer, bytes_read);
				memset(buffer, 0, 2000);
			} while (bytes_read);

			InternetCloseHandle(interwebs);
			InternetCloseHandle(url_file);
			std::string p = replace_all(rtn, "|n", "\r\n");
			return p;
		}
	}

	InternetCloseHandle(interwebs);
	std::string p = replace_all(rtn, "|n", "\r\n");
	return p;
}


namespace Hook {

	DWORD RLSorg = 0;
	DWORD hookaddr = 0x8113D0; //gettop addr

	int gettopd(DWORD rState)
	{
		RLSorg = rState;
		return (*(DWORD *)(rState + top) - *(DWORD *)(rState + base)) >> 4;
	}

	void CreateHook() {
		MH_Initialize();
		void* oldhook = (void*)MH_CreateHook((DWORD*)x(hookaddr), gettopd, NULL);
		MH_CreateHook((DWORD*)x(hookaddr), gettopd, (LPVOID*)oldhook);
		MH_EnableHook((DWORD*)x(hookaddr));
		MH_DisableHook((DWORD*)x(hookaddr));

		if (RLSorg == 0) {
			void* oldhook = (void*)MH_CreateHook((DWORD*)x(hookaddr), gettopd, NULL);
			MH_CreateHook((DWORD*)x(hookaddr), gettopd, (LPVOID*)oldhook);
			MH_EnableHook((DWORD*)x(hookaddr));
			while (RLS == 0) { Sleep(1); }
			MH_DisableHook((DWORD*)x(hookaddr));
		}
	}
}

BOOL WINAPI ConsoleBypass(std::string name)
{
	DWORD nOldProtect;
	if (!VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, &nOldProtect))
		return FALSE;
	*(BYTE*)(FreeConsole) = 0xC3;
	if (!VirtualProtect(FreeConsole, 1, nOldProtect, &nOldProtect))
		return FALSE;
	AllocConsole();

	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	SetWindowPos(ConsoleHandle, HWND_TOPMOST, 50, 20, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	ShowWindow(ConsoleHandle, 1);
	SetConsoleTitle(name.c_str());

}

#define SetIdentity(RLS, I) DWORD* w = (DWORD *)(RLS - 40); *w ^= (I ^ (unsigned __int8)*w) & 0x1F;


void main()
{
	ConsoleBypass("AtomWrapperX | Rewrite");
	Hook::CreateHook();
	RLS = Hook::RLSorg;
	SetIdentity(RLS, 6);
	LS = luaL_newstate();
	Wrapper::CallCheckBypass();
	luaL_openlibs(LS); 


	std::vector<std::string> rbxglobals{
	"printidentity","game","Game","workspace","Workspace",
	"Axes","BrickColor","CFrame","Color3","ColorSequence","ColorSequenceKeypoint",
	"NumberRange","NumberSequence","NumberSequenceKeypoint","PhysicalProperties","Ray",
	"Rect","Region3","Region3int16","TweenInfo","UDim","UDim2",
	"Vector2", "Vector2int16","Vector3","Vector3int16","Enum","Faces",
	"Instance","math","warn","typeof","type","spawn","Spawn","print",
	"printidentity","ypcall","Wait","wait","delay","Delay","tick","LoadLibrary","require"
	};


	for (auto func : rbxglobals) {
		std::cout << "Wrapped Global: " << func << std::endl;
		WrapGlobal(RLS, LS, func.c_str());
	}

	lua_register(LS, "CustomHttpGet", r_httpget);
	r_lua_getglobal(RLS, "print");
	r_lua_pushstring(RLS, "Atom Loaded");
	r_lua_pcall(RLS, 1, 0, 0);
	CreateThread(NULL, NULL, input, NULL, NULL,NULL);
	std::string i;
	while (true) {
		putchar('>');
		getline(cin, i);
		ExecuteScript(i);
		if (!FindWindowA(NULL, "Roblox"))
			exit(1);
	}
}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(Module);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
		break;
	case DLL_PROCESS_DETACH:
		break;
	default: break;
	}
	return TRUE;
}

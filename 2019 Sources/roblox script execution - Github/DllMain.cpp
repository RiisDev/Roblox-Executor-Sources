#include "Memory.h"
#include "Wrapper.h"
#include "AuthTrial.h"

using namespace Memory;
using namespace std;

void CreateConsole(const char *Name) {
	DWORD consoleOldProtect = 0;
	VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, &consoleOldProtect);
	*(UINT*)FreeConsole = 0xC2;
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	DWORD cons = (WS_CAPTION | DS_MODALFRAME | WS_MINIMIZEBOX | WS_SYSMENU);
	SetWindowLong(GetConsoleWindow(), GWL_STYLE, cons);
	SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	SetConsoleTitleA(Name);
}

LONG WINAPI int3cbreakpoint(PEXCEPTION_POINTERS ex)
{
	if (ex->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
	{
		if (ex->ContextRecord->Eip == int3breakpoint)
		{
			ex->ContextRecord->Eip = (DWORD)(Bridge::rbxFunctionBridge);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		return EXCEPTION_CONTINUE_SEARCH;
	}
}

#define PushGlobal(r_L, L, s) registerGlobal(r_L, L, s)

void GetFile(const char* dllName, const char* fileName, char* buffer, int bfSize) {
	GetModuleFileName(GetModuleHandle(dllName), buffer, bfSize);
	if (strlen(fileName) + strlen(buffer) < MAX_PATH) {
		char* pathEnd = strrchr(buffer, '\\');
		strcpy(pathEnd + 1, fileName);
	}
	else {
		*buffer = 0;
	}
}






int ReadFile(const std::string& path, std::string& out, unsigned char binary) {
	std::ios::openmode mode = std::ios::in;
	if (binary)
		mode |= std::ios::binary;

	std::ifstream file(path, mode);
	if (file.is_open()) {
		std::stringstream buffer;
		buffer << file.rdbuf();
		out = buffer.str();
		file.close();
		return 1;
	}

	file.close();
	return 0;
}

static int loadfile(lua_State *L) {
	if (lua_isstring(L, 1)) {
		const char * nameoffile = lua_tostring(L, 1);
		char path[MAX_PATH];
		GetFile("OmithusWrapper.dll", "", path, MAX_PATH);

		std::string lyonisameganoob;
		lyonisameganoob += path;
		lyonisameganoob += "scripts\\";
		lyonisameganoob += nameoffile;

		printf(lyonisameganoob.c_str());
		string line;
		ifstream myfile(lyonisameganoob);
		std::string data;
		if (ReadFile(lyonisameganoob, data, 0)) {
			luaL_dostring(L, data.c_str());
		}
		else {
	
		}
	}

	else {
		//luaL_error(L, "Loadfile expected a string.");
		return 1;
	}

	return 0;
}

DWORD WINAPI input(PVOID lvpParameter)
{
	string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Omithus"),
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
			std::string spawn = std::string("spawn(function() local script = Instance.new('LocalScript')" + std::string(WholeScript) + "end)");
			if (luaL_dostring(g_L, spawn.c_str()))
				printf("Error: %s\n", lua_tostring(g_L, -1));
			else
				lua_pcall(g_L, 0, 0, 0);

			WholeScript = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}

int HttpGet(lua_State *L)
{
	std::string URL = std::string(DownloadURL(lua_tolstring(L, -1, 0)));
	luaL_dostring(L, URL.c_str());
	return 1;
}



void main()
{
	CreateConsole("Omithus");



	DWORD ScriptContext_VFTable = format(0x190510C);
	int ScriptContext = Scan((char*)&ScriptContext_VFTable);//Memory::Scan(PAGE_READWRITE, (char*)&ScriptContext_VFTable, "xxxx");
	g_rL = (ScriptContext + 164) ^ *(DWORD*)(ScriptContext + 164);

	AddVectoredExceptionHandler(1, int3cbreakpoint);

	g_L = lua_open();
	luaL_openlibs(g_L);

	lua_register(g_L, "load", loadfile);

	PushGlobal(g_rL, g_L, "game");
	PushGlobal(g_rL, g_L, "Game");
	PushGlobal(g_rL, g_L, "workspace");
	PushGlobal(g_rL, g_L, "Workspace");
	PushGlobal(g_rL, g_L, "Axes");
	PushGlobal(g_rL, g_L, "BrickColor");
	PushGlobal(g_rL, g_L, "CFrame");
	PushGlobal(g_rL, g_L, "Color3");
	PushGlobal(g_rL, g_L, "ColorSequence");
	PushGlobal(g_rL, g_L, "ColorSequenceKeypoint");
	PushGlobal(g_rL, g_L, "NumberRange");
	PushGlobal(g_rL, g_L, "NumberSequence");
	PushGlobal(g_rL, g_L, "NumberSequenceKeypoint");
	PushGlobal(g_rL, g_L, "PhysicalProperties");
	PushGlobal(g_rL, g_L, "Ray");
	PushGlobal(g_rL, g_L, "Rect");
	PushGlobal(g_rL, g_L, "Region3");
	PushGlobal(g_rL, g_L, "Region3int16");
	PushGlobal(g_rL, g_L, "TweenInfo");
	PushGlobal(g_rL, g_L, "UDim");
	PushGlobal(g_rL, g_L, "UDim2");
	PushGlobal(g_rL, g_L, "Vector2");
	PushGlobal(g_rL, g_L, "Vector2int16");
	PushGlobal(g_rL, g_L, "Vector3");
	PushGlobal(g_rL, g_L, "Vector3int16");
	PushGlobal(g_rL, g_L, "Enum");
	PushGlobal(g_rL, g_L, "Faces");
	PushGlobal(g_rL, g_L, "Instance");
	PushGlobal(g_rL, g_L, "math");
	PushGlobal(g_rL, g_L, "warn");
	PushGlobal(g_rL, g_L, "typeof");
	PushGlobal(g_rL, g_L, "type");
	PushGlobal(g_rL, g_L, "spawn");
	PushGlobal(g_rL, g_L, "Spawn");
	PushGlobal(g_rL, g_L, "print");
	PushGlobal(g_rL, g_L, "printidentity");
	PushGlobal(g_rL, g_L, "ypcall");
	PushGlobal(g_rL, g_L, "Wait");
	PushGlobal(g_rL, g_L, "wait");
	PushGlobal(g_rL, g_L, "delay");
	PushGlobal(g_rL, g_L, "Delay");
	PushGlobal(g_rL, g_L, "tick");
	PushGlobal(g_rL, g_L, "LoadLibrary");

	lua_register(g_L, "HttpGet", HttpGet);

	DWORD* identity2 = (DWORD*)(g_rL - 32); *identity2 ^= (6 ^ (unsigned __int8)*identity2) & 0x1F;

	do
	{
		string x;
		getline(cin, x);
		std::string spawn = std::string("spawn(function() local script = Instance.new('LocalScript')\r\n" + std::string(x) + "\r\nend)");
		int err = luaL_dostring(g_L, x.c_str());
		if (err)
			printf("Error: %s\n", lua_tostring(g_L, -1));
	
	} while (true);

}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(main), 0, 0, 0);
	}
	return 1;
}
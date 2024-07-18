#include "Windows.h"
#include <cstdio>
#include <string>
#include "r_lua.h"
#include "bridge.h"
#include "functions.h"
#include "bitop.h"
#include "drawing.h"

using namespace std;
using Bridge::m_rL;
using Bridge::m_L;

void ConsoleBypass(const char* name)
{
	DWORD aa;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &aa);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	SetConsoleTitleA(name);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}

DWORD WINAPI luapipe(PVOID lvpParameter)
{
	string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Axon"),
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
			WholeScript = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}

int GetDatamodel()
{
	static DWORD DMPad[4]{};
	r_getdatamodel(getdatamodel2(), (DWORD)DMPad);
	DWORD DM = DMPad[0];
	return DM + 12;
}
void getdatamodeltesting()
{
	static DWORD SC;
	FindFirstChild(GetDatamodel(), (DWORD)&SC, "ScriptContext");
	m_rL = (SC + StateOffset) + *(DWORD*)(SC + StateOffset);
	SetIdentity();
}

void WrapGlobals()
{
	std::vector<const char*> Globals = {
	"printidentity","game","Game","workspace","Workspace",
	"Axes","BrickColor","CFrame","Color3","ColorSequence","ColorSequenceKeypoint",
	"NumberRange","NumberSequence","NumberSequenceKeypoint","PhysicalProperties","Ray",
	"Rect","Region3","Region3int16","TweenInfo","UDim","UDim2","Vector2",
	"Vector2int16","Vector3","Vector3int16","Enum","Faces",
	"Instance","math","warn","typeof","type","print",
	"printidentity","ypcall","Wait","wait","delay","Delay","tick", "pcall", "spawn", "Spawn"
	};
	for (int i = 0; i < Globals.size(); i++)
	{
		r_lua_getglobal(m_rL, Globals[i]);
		Bridge::push(m_rL, m_L, -1);
		lua_setglobal(m_L, Globals[i]);
		r_lua_pop(m_rL, 1);
	}
}

void Execute(std::string Script) {
	Script = "spawn(function() script=Instance.new(\"LocalScript\") " + Script + "\r\nend)";
	if (luaL_loadbuffer(m_L, Script.c_str(), Script.size(), "@Axon"))
	{

		r_lua_getglobal(m_rL, "warn");
		r_lua_pushstring(m_rL, lua_tostring(m_L, -1));
		r_lua_pcall(m_rL, 1, 0, 0);
		return;
	}
	else
	{
		lua_pcall(m_L, 0, 0, 0);
	}
	UserDataGC(m_L);
}

int main()
{
	ConsoleBypass("Lua Wrapper level 7 2020 69420 B)");
	getdatamodeltesting();
	printf("injected into client");
	m_L = luaL_newstate();
	printf("finished setting lua state");
	Bridge::VehHandlerpush();
	luaL_openlibs(m_L);
	printf("wrapping globals");
	WrapGlobals();
	printf("loading libraries");
	loadlibrary();
	luaopen_bit(m_L);
	DrawingAPI::InitiateDrawingAPI(m_L);
	printf("libraries loaded");
	lua_newtable(m_L);
	lua_setglobal(m_L, "_G");
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)luapipe, NULL, NULL, NULL);
	printf("loaded! enjoy ;)");
	Execute("game.StarterGui:SetCore(\"SendNotification\",{Title=\"Axon\",Text=\"Successfully initialized\"})");
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
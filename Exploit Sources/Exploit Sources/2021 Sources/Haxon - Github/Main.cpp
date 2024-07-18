#include "Bridge.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <iterator>
#include <fstream>
#include <intrin.h>
#include <Tlhelp32.h>
#include <CommCtrl.h>
#include <Wininet.h>
/*
* 
 __          __  _                            _                                      
 \ \        / / | |                          | |            /\                       
  \ \  /\  / /__| | ___ ___  _ __ ___   ___  | |_ ___      /  \   __  _____  _ __    
   \ \/  \/ / _ \ |/ __/ _ \| '_ ` _ \ / _ \ | __/ _ \    / /\ \  \ \/ / _ \| '_ \   
    \  /\  /  __/ | (_| (_) | | | | | |  __/ | || (_) |  / ____ \  >  < (_) | | | |_ 
     \/  \/ \___|_|\___\___/|_| |_| |_|\___|  \__\___/  /_/    \_\/_/\_\___/|_| |_(_)
                                                                                     
                                                                                     
 Hey, welcome to haxon. By meeeeeeeee.
 This tutorial will show you how to use axon.

 Also, this axon has some upgraded features, wink wink.
 Credit me if your going to use.


*/
#pragma comment(lib, "wininet.lib")
using namespace std;

DWORD ScriptContext;

using Bridge::m_rL;
using Bridge::m_L;


using namespace std;

std::string replace_all(std::string subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

static int UserDataGC(lua_State* E) {
	void* UD = lua_touserdata(E, 1);
	if (Bridge::m_rL) {

		r_lua_rawgeti(Bridge::m_rL, LUA_REGISTRYINDEX, (int)UD);
		if (r_lua_type(Bridge::m_rL, -1) <= R_LUA_TNIL) {
			lua_pushnil(E);
			lua_rawseti(E, LUA_REGISTRYINDEX, (int)UD);

		}
	}
	return 0;
}
void Executeee(std::string Script) {
	Script = "spawn(function() script=Instance.new(\"LocalScript\") " + Script + "\r\nend)";


	if (luaL_loadbuffer(m_L, Script.c_str(), Script.size(), "@Ackson"))
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
	GarbageCollector(m_L);
	
}

#define DebugPrintf printf


DWORD WINAPI input(PVOID lvpParameter)
{
	string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Ackson"), 
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
			Executeee(WholeScript);
			WholeScript = "https://raw.githubusercontent.com/Haxon-Repo/code/main/README.md";//set your watermark here! this is where the loadstring would be!
		}
		DisconnectNamedPipe(hPipe);
	}
}


int GetDatamodel()
{
	int A = 9;
	int B = 3;

	int result = A + B;

	static DWORD DMPad[16]{};
	printf("DataModel \n");
	r_getdatamodel(getdatamodel2(), (DWORD)DMPad);
	printf("DataModel Passed\n");
	DWORD DM = DMPad[0];
	printf("Returning DataModel \n");
	return DM + result;
}

int getRawMetaTable(lua_State *L) {
	Bridge::push(L, m_rL, 1);

	if (r_lua_getmetatable(m_rL, -1) == 0) {
		lua_pushnil(L);
		return 0;
	}
	Bridge::push(m_rL, L, -1);

	return 1;
}

int GetGenv(lua_State* L)
{
	if (lua_gettop(L) != 0)
		throw std::runtime_error("GetGenv function does not accept arguments");
	Bridge::push(m_rL, L, -1);
	lua_pushvalue(L, LUA_GLOBALSINDEX);
	return 1;
}

int getNamecallMethod(lua_State* L)
{
	Bridge::push(m_rL, L, -1);
	return 1;
}

void RegFuncs(lua_State* L)
{
	lua_register(L, "getgenv", GetGenv);
	lua_register(m_L, "getrawmetatable", getRawMetaTable);
	//lua_register(m_L, "getfflag", impl_getflag);
	//lua_register(m_L, "setfflag", impl_setfflag);
}

void ConsoleBypass(const char* Title) {
	DWORD aaaa;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &aaaa);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	SetConsoleTitleA(Title);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}

const char* GetClass(int self)
{
	return (const char*)(*(int(**)(void))(*(int*)self + 16))();
}

int FindFirstClass(int Instance, const char* Name)
{


	DWORD StartOfChildren = *(DWORD*)(Instance + 44);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		if (memcmp(GetClass(*(int*)i), Name, strlen(Name)) == 0)
		{
			return *(int*)i;
		}
	}
}

int IdentityNumber = 104; // This is the identity number.

DWORD SetThreadIdentity(DWORD rL, int level)
{
	return *(DWORD*)(*(DWORD*)(rL + IdentityNumber) + 24) = level;
}


void getdatamodeltesting()
{
	GDM = GetDatamodel();
	printf("GetDataModel: %08X\n", GDM);
	ScriptContext = FindFirstClass(GDM, "ScriptContext");

	DWORD oldRL = AxonWonderful::RBX_LuaState(ScriptContext); // Make sure to Update RBX_LuaState (r_lua.h)

	m_rL = r_lua_newthread(oldRL);
}

void InitGlobals()
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
		std::cout << "Haxon Global: " << Globals[i] << std::endl;
	}
}

void PushGlobal(DWORD rL, lua_State* L, const char* s)
{
	r_lua_getglobal(rL, s);
	Bridge::push(rL, L, -1);
	lua_setglobal(L, s);
	r_lua_pop(rL, 1);
	
}
void init()
{
	ConsoleBypass("Haxon Exploit| Dll source|I hate my life|free");
	printf("[INFO]: Key's do not save anymore! (Like they ever did)\n");
	getdatamodeltesting();
	SetThreadIdentity(m_rL, 6);
	m_L = luaL_newstate();
	Bridge::VehHandlerpush();
	luaL_openlibs(m_L);
	InitGlobals();
	RegFuncs(m_L);
	lua_newtable(m_L);
	lua_setglobal(m_L, "_G");
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)input, NULL, NULL, NULL);
	Executeee("https://raw.githubusercontent.com/Haxon-Repo/code/main/README.md");
	MessageBoxA(NULL, "Axon sucks, so do you,\n""don't build this cancerous source code will you?\n" "Enjoy the built Haxon, don't make any changes to my 6 day work!\n", "Skid", MB_OK);
	::ShowWindow(::GetConsoleWindow(), SW_MINIMIZE);
}

using namespace std;

void main()
{
	ConsoleBypass("Ackson API | Key System");
	init();
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
		// The Application (Roblox) has exited.
		break;
	default: break;
	}

	return TRUE;
}


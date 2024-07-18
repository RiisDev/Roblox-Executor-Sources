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

#pragma comment(lib, "wininet.lib")
using namespace std;

/*
Axon Updated by SpeedSterKawaii for April 21, 2021.
I commented out some stuff so yk what your doing.
*/

DWORD ScriptContext;

using Bridge::m_rL; // This is a DWORD.
using Bridge::m_L; // This is the Lua State.

using namespace std;

std::string replace_all(std::string subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

static int UserDataGC(lua_State* eaeae) {
	void* UD = lua_touserdata(eaeae, 1);
	if (Bridge::m_rL) {

		r_lua_rawgeti(Bridge::m_rL, LUA_REGISTRYINDEX, (int)UD);
		if (r_lua_type(Bridge::m_rL, -1) <= R_LUA_TNIL) {
			lua_pushnil(eaeae);
			lua_rawseti(eaeae, LUA_REGISTRYINDEX, (int)UD);
		}
	}
	return 0;
}

void Executeee(std::string Script) {
	Script = "spawn(function() script=Instance.new(\"LocalScript\") " + Script + "\r\nend)";

	if (luaL_loadbuffer(m_L, Script.c_str(), Script.size(), "@Axon")) // Your exploit name.
	{
		r_lua_getglobal(m_rL, "warn"); // Script Error Occurred During Execution.
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
	char buffer[999999]; // Buffer Data.
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Cryon"), // Used to connect to the pipe (you can change it to anything)
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
			Executeee(WholeScript); // Execute Data to the Pipe "Axon".
			WholeScript = "";
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
	printf("GETDATAMODEL \n");
	r_getdatamodel(getdatamodel2(), (DWORD)DMPad);
	printf("GETDATAMODEL PASSED\n"); // Passed Data-Model.
	DWORD DM = DMPad[0];
	printf("RETURNING DM \n"); // Returns Data-Model.
	return DM + result;
}


int impl_getflag(lua_State* L)
{
	Bridge::push(L, m_rL, 1);
	std::string flag = lua_tostring(L, 1);
	std::string ret;
	bool out = GetValue(flag, ret, 1);
	if (!out)
		luaL_error(L, "flag not found u coon");
	lua_pushstring(L, ret.c_str());
	return 1;
}

int impl_setfflag(lua_State* L)
{
	if (lua_type(L, 1) != LUA_TSTRING)
		return luaL_error(L, "expected a string");
	if (lua_type(L, 2) != LUA_TSTRING)
		return luaL_error(L, "expected a string");
	Bridge::push(L, m_rL, 1);
	std::string flag = lua_tostring(L, 1);
	std::string value = lua_tostring(L, 2);
	std::string ret;
	bool exist = GetValue(flag, ret, 1);
	if (!exist)
		luaL_error(L, "flag not found u coon");
	SetValue(flag, value, 63, 0);
	return 0;
}

int getRawMetaTable(lua_State* L) {
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

int GetReg(lua_State* L) { // Need to register this function.
	if (lua_gettop(L) != 0)
		throw std::runtime_error("GetReg function does not accept arguments");
	lua_pushvalue(L, LUA_REGISTRYINDEX);
	Bridge::push(m_rL, L, -1);
	return 1;
}

int GetFenv(lua_State* L) { // Need to register this function.
	if (lua_gettop(L) != 0)
		throw std::runtime_error("GetFenv function does not accept arguments");
	lua_pushvalue(L, LUA_GLOBALSINDEX);
	lua_xmove(L, L, 1);
	return 1;
}

int GetRenv(lua_State* L) { // Need to register this function.
	if (lua_gettop(L) != 0)
		throw std::runtime_error("GetRenv function does not accept arguments");
	lua_pushvalue(L, int(m_rL));
	Bridge::push(m_rL, L, -1);
	return 1;
}

int GetSenv(lua_State* L) { // Need to register this function.
	if (lua_gettop(L) != 0)
		throw std::runtime_error("GetSenv function does not accept arguments");
	lua_pushvalue(L, LUA_ENVIRONINDEX);
	Bridge::push(m_rL, L, -1);
	return 1;
}

void RegFuncs(lua_State* L) // Registers Functions in the Lua Enviroment.
{
	lua_register(L, "getgenv", GetGenv);
	lua_register(m_L, "getrawmetatable", getRawMetaTable);
	lua_register(m_L, "getfflag", impl_getflag);
	lua_register(m_L, "setfflag", impl_setfflag);
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

DWORD SetThreadIdentity(DWORD rL, int level)
{
	return *(DWORD*)(*(DWORD*)(rL + Adresses::Identity2) + 24) = level;
}

void getdatamodeltesting()
{
	GDM = GetDatamodel();
	printf("GetDataModel: %08X\n", GDM);
	ScriptContext = FindFirstClass(GDM, "ScriptContext");

	DWORD oldRL = Adresses::RBX_LuaState(ScriptContext);

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
		std::cout << "Global Wrapped: " << Globals[i] << std::endl;
	}
}

void PushGlobal(DWORD rL, lua_State* L, const char* s)
{
	r_lua_getglobal(rL, s);
	Bridge::push(rL, L, -1);
	lua_setglobal(L, s);
	r_lua_pop(rL, 1);

}

void main()
{
	ConsoleBypass("CryonGG -- Made By .Synaap"); // Console Title.
	getdatamodeltesting();
	SetThreadIdentity(m_rL, 7); // "7" defined has the Identity Number.
	m_L = luaL_newstate();
	Bridge::VehHandlerpush();
	luaL_openlibs(m_L);
	InitGlobals(); // Wrap the Globals.
	std::cout << "Globals have been Wrapped\n";
	RegFuncs(m_L); // Register Custom Functions.
	std::cout << "Custom Functions Registered\n";
	lua_newtable(m_L);
	lua_setglobal(m_L, "_G");
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)input, NULL, NULL, NULL); // Make Pipe Name.
	MessageBoxA(NULL, "Successfuly Injected!", "CryonGG", MB_ICONINFORMATION);
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
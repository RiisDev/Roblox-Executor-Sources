#include "curl/curl.h"
#include "LuaWrapper.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "ConsoleCol.h"
#include <iterator>
#include <fstream>
#include <intrin.h>
#include <Tlhelp32.h>
#include <CommCtrl.h>
#include <fstream>
#include <sstream>
#include <Psapi.h>
#include <stdint.h>
#include <memory>
#include "Utilities.h"


using namespace std;
void PushGlobal(DWORD rL, lua_State* L, const char* s)
{
	r_lua_getglobal(rL, s);
	Wrapper::Wrap(rL, L, -1);
	lua_setglobal(L, s);
	r_lua_pop(rL, 1);
}

void WrapAllGlobals(DWORD RLS, lua_State* LS) {
	r_lua_pushvalue(RLS, LUA_GLOBALSINDEX);
	r_lua_pushnil(RLS);
	while (r_lua_next(RLS, -2)) {
		const char* s = r_lua_tostring(RLS, -2);
		if (s != NULL && r_lua_type(RLS, -1) != R_LUA_TNIL) {
			std::cout << "Global: " << s << " Type: " << r_lua_type(RLS, -1) << std::endl;
			Wrapper::Wrap(RLS, LS, -1);
			lua_setglobal(LS, s);
			r_lua_pop(RLS, 1);
		}
		else {
			r_lua_pop(RLS, 1);
		}
	}
	r_lua_pop(RLS, 1);
} //This works but seems to fuck up dex

static int CopyString(lua_State* L) {
	const char* str = lua_tostring(L, -1);
	int len = strlen(str);
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len + 1);
	void* mem_ptr = GlobalLock(hMem);
	memcpy(mem_ptr, str, len + 1);
	GlobalUnlock(hMem);
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
	return 0;
}

DWORD WINAPI input(PVOID lvpParameter)
{
	string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\crashpad_5680_QEKFAKDIUDQCWAJB"),
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

static int custom_getgenv(lua_State* L) {
	r_lua_pushvalue(RLS, LUA_GLOBALSINDEX);
	Wrapper::Wrap(RLS, L, -1);
	return 1;
}

int newindex_metamethod(lua_State* L) {
	DWORD rL = r_lua_newthread(RLS);
	Wrapper::UnWrap(L, rL, lua_upvalueindex(1));
	const char* key = lua_tostring(L, 2);
	r_lua_getmetatable(rL, -1);
	DWORD top = *(DWORD*)lua_index2adr(L, -1);
	*(BYTE*)(*(DWORD*)(top + 8));
	lua_pushvalue(L, lua_upvalueindex(1));
	Wrapper::UnWrap(L, rL, 3); r_lua_setfield(rL, -2, key);
	r_lua_setmetatable(rL, -2); lua_settop(L, 0);
	r_lua_pop(rL, 2);
	return 0;
}

static int custom_getrawmetatable(lua_State* L) {
	Wrapper::UnWrap(L, RLS, 1);
	if (r_lua_getmetatable(RLS, -1)) {
		DWORD top = *(DWORD*)lua_index2adr(L, -1);
		*(BYTE*)(*(DWORD*)(top + 8) = 0);
		Wrapper::Wrap(RLS, L, -1);
		lua_createtable(L, 0, 0);
		{
			lua_pushvalue(L, 1);
			lua_pushcclosure(L, newindex_metamethod, 1);
			lua_setfield(L, -2, "__newindex"); }
	}
	lua_setmetatable(L, -2);
	return 1;
}

int setreadonly(lua_State* thread) {
	Wrapper::UnWrap(thread, RLS, -2);
	*(BYTE*)(*(DWORD*)lua_index2adr(thread, -1) + 7) = lua_toboolean(thread, -1);
	Wrapper::Wrap(RLS, thread, -1);
	return 0;
}

int custom_isreadonly(lua_State* LuaState) {
	int value = *(BYTE*)(*(DWORD*)lua_index2adr(LuaState, -1) + 8);
	if (value == 1) {
		lua_pushboolean(LuaState, true);
	}
	else if (value == 0) {
		lua_pushboolean(LuaState, false);
	}
	return 1;
}

void ConsoleBypass(const char* Name)
{
	DWORD AAAAAAAAAAAAAAA;
	VirtualProtect((PVOID)& FreeConsole, 1, PAGE_EXECUTE_READWRITE, &AAAAAAAAAAAAAAA);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	SetConsoleTitleA(Name);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}

INPUT Inputs[1] = { 0 };

int Mouse1Down(lua_State* LS) {
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
	SendInput(1, Inputs, sizeof(INPUT));
	return 0;
}

int Mouse1Up(lua_State* L) {
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
	SendInput(1, Inputs, sizeof(INPUT));
	return 0;
}

int Mouse2Down(lua_State* L) {
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN;
	SendInput(1, Inputs, sizeof(INPUT));
	return 0;
}

int Mouse2Up(lua_State* L) {
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP;
	SendInput(1, Inputs, sizeof(INPUT));
	return 0;
}

int Mouse1Click(lua_State* L) {
	float n = lua_tonumber(L, -1);
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
	SendInput(1, Inputs, sizeof(INPUT));
	Sleep(n);
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
	SendInput(1, Inputs, sizeof(INPUT));
	return 0;
}

int Mouse2Click(lua_State* L) {
	float n = lua_tonumber(L, -1);
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
	SendInput(1, Inputs, sizeof(INPUT));
	Sleep(n);
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
	SendInput(1, Inputs, sizeof(INPUT));
	return 0;
}

int MouseScroll(lua_State* L) {
	int amount = lua_tonumber(L, -1);
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_HWHEEL;
	Inputs[0].mi.mouseData = amount;
	SendInput(1, Inputs, sizeof(INPUT));
	Inputs[0].mi.mouseData = NULL;
	return 0;
}

int MouseMoveRelative(lua_State* L) {
	int x = lua_tonumber(L, -1);
	int y = lua_tonumber(L, -2);
	POINT p;
	if (GetCursorPos(&p))
	{
		SetCursorPos(p.x + x, p.y + y);
	}
	return 0;
}

void* placeHook(DWORD address, void* hook, bool revert = false) {
	DWORD oldprot;
	if (!revert) {
		void* oldmem = new void*;
		void* result = new void*;
		memcpy(oldmem, (void*)address, sizeof(void*) * 4);
		VirtualProtect((LPVOID)address, 1, PAGE_EXECUTE_READWRITE, &oldprot);
		*(char*)address = 0xE9; *(DWORD*)(address + 1) = (DWORD)hook - address - 5;
		memcpy(result, oldmem, sizeof(void*) * 4);
		VirtualProtect((LPVOID)address, 1, oldprot, &oldprot);
		return result;
	}
	else {
		VirtualProtect((LPVOID)address, 1, PAGE_EXECUTE_READWRITE, &oldprot);
		memcpy((void*)address, hook, sizeof(void*) * 4);
		VirtualProtect((LPVOID)address, 1, oldprot, &oldprot);
		return NULL;
	}
}


int gettop_d(int a1)
{
	RLS = a1;
	return (*(DWORD*)(a1 + ROBLOX_OFFSET_TOP) - *(DWORD*)(a1 + ROBLOX_OFFSET_BASE)) >> 4;
}

int gethpath(lua_State* L) {
	char path[MAX_PATH];
	Files::GetFile("OxygenWrapper.dll", "", path, MAX_PATH);
	std::cout << path << "\n";
	lua_pushstring(L, path);
	return 1;
}

int writefile(lua_State* L) {
	if (lua_gettop(L) < 2) { printf("writefile expects 2 arguments."); return 0; }
	std::string _path = (std::string)lua_tostring(L, 1);
	const char* plswrite = lua_tostring(L, 2);
	if (strlen(_path.c_str()) >= 50) {
		std::string path2backup = _path;
		_path = (std::string)plswrite;
		plswrite = path2backup.c_str();
	}
	char dog[MAX_PATH];
	Files::GetFile("OxygenWrapper.dll", "", dog, MAX_PATH);
	char pls1[100];   // array to hold the result.
	strcpy(pls1, dog); // copy string one into the result.
	strcat(pls1, "Workspace\\");
	char path[100];   // array to hold the result.
	strcpy(path, pls1); // copy string one into the result.
	strcat(path, _path.c_str());
	std::string _checkPath(path);
	/*if (_checkPath.find(".exe") || _checkPath.find(".dll")) {
		printf("Attempt to writefile to a blocked file extension.");
		return 0;
	}*/
	if (!Files::WriteFile(path, plswrite, false)) {
		return 0;
	}
	return 0;
} //i like taking bytecode funcs and using them here

int readfile(lua_State* L) {
	std::string _path = (std::string)lua_tostring(L, 1);
	gethpath(L); // TODO FIX 
	std::string ok = (std::string)lua_tostring(L, -1);
	char pls1[100];   // array to hold the result.
	strcpy(pls1, ok.c_str()); // copy string one into the result.
	strcat(pls1, "Workspace\\");
	char path[100];   // array to hold the result.
	strcpy(path, pls1); // copy string one into the result.
	strcat(path, _path.c_str());
	std::string _checkFile(path);
	std::string filecontents;
	if (!Files::ReadFile(path, filecontents, 0)) {
		r_lua_pushnil(RLS);
		//printf("Failed to read file.");
		return 1;
	}
	lua_pushstring(L, filecontents.c_str());
	return 1;
}

std::string randomstringg2(int length) {
	std::string str = "000000000123456666666666666789EFC";
	std::string ppj;
	int pos = 0;
	while (pos != length) {
		int ppk = ((rand() % (str.size() - 1)));
		ppj = ppj + str[ppk];
		pos += 1;
	}
	return ppj.c_str();
}

static int LuaU_decompile(lua_State* ring)
{
	string x;
	int i = 0;

	for (i = 1; i <= 6; i++)
	{
		x.append(randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + "\n");
	}

	lua_pushstring(ring, x.c_str());
	return 1;
}
namespace Memory {
	BOOL compare(const BYTE* location, const BYTE* aob, const char* mask) {
		for (; *mask; ++aob, ++mask, ++location) {
			__try {
				if (*mask == 'x' && *location != *aob)
					return 0;
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				return 0;
			}
		}
		return 1;
	}

	DWORD find_Pattern(DWORD size, BYTE* pattern, char* mask,
		BYTE protection = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) {
		SYSTEM_INFO SI = { 0 };
		GetSystemInfo(&SI);
		DWORD start = (DWORD)SI.lpMinimumApplicationAddress;
		DWORD end = (DWORD)SI.lpMaximumApplicationAddress;
		MEMORY_BASIC_INFORMATION mbi;
		while (start < end && VirtualQuery((void*)start, &mbi, sizeof(mbi))) {
			// Make sure the memory is committed, matches our protection, and isn't PAGE_GUARD.
			if ((mbi.State & MEM_COMMIT) && (mbi.Protect & protection) && !(mbi.Protect & PAGE_GUARD)) {
				// Scan all the memory in the region.
				for (DWORD i = (DWORD)mbi.BaseAddress; i < (DWORD)mbi.BaseAddress + mbi.RegionSize; ++i) {
					if (compare((BYTE*)i, pattern, mask)) {
						return i;
					}
				}
			}
			// Move onto the next region of memory.
			start += mbi.RegionSize;
		}
		return 0;
	}

	int Scan(DWORD mode, char* content, char* mask) {
		return find_Pattern(0x7FFFFFFF, (BYTE*)content, mask, mode);
	}
}

typedef struct LuaU_Reg {
	const char* name;
	lua_CFunction func;
} LuaU_Reg;

static const luaL_Reg CustomFunctions[] = {
  {"assert", setreadonly},
  {NULL, NULL}
};

void Main()
{
	ConsoleBypass("Debug Console");
	//CreateConsole("d");
	/*void* old = placeHook(x(0x853d30), gettop_d);
	do { Sleep(1); } while (RLS == 0);
	placeHook(x(0x853d30), old, 1);*/
	std::cout << "Scanning.." << endl;
	DWORD ScriptContextVFT_Addy = x(0x1A93A3C);
	uintptr_t v2 = Memory::Scan(PAGE_READWRITE, (char*)& ScriptContextVFT_Addy, (char*)"xxxx");
	std::cout << "Scanning Done!" << endl;
	std::cout << "LuaState.." << endl;
	RLS = v2 + 56 * 0 + 164 ^ *(DWORD*)(v2 + 56 * 0 + 164);
	std::cout << "Done!" << endl;

	std::cout << "LuaState shit and wrapping Globals!" << endl;

	LS = luaL_newstate();
	luaL_openlibs(LS);
	VehHandlerpush();
	std::vector<std::string> Globals{
"printidentity","game","Game","workspace","Workspace",
"Axes","BrickColor","CFrame","Color3","ColorSequence","ColorSequenceKeypoint",
"NumberRange","NumberSequence","NumberSequenceKeypoint","PhysicalProperties","Ray",
"Rect","Region3","Region3int16","TweenInfo","UDim","UDim2","Vector2",
"Vector2int16","Vector3","Vector3int16","Enum","Faces",
"Instance","math","warn","typeof","type","spawn","Spawn","print",
"printidentity","ypcall","Wait","wait","delay","Delay","tick","LoadLibrary",
"Path"
	};
	for (auto func : Globals) {
		PushGlobal(RLS, LS, func.c_str());
	}
	std::cout << "done!" << endl;
	std::cout << "custom funcs!!" << endl;
	//WrapAllGlobals(RLS, LS);
	//lua_newtable(LS);
	//lua_setglobal(LS, "_G");

	//lua_register(LS, "_G", (lua_CFunction)CustomFunctions);

	lua_register(LS, "setreadonly", setreadonly);
	//lua_register(LS, "isreadonly", custom_isreadonly);
	lua_register(LS, "getrawmetatable", custom_getrawmetatable);
	lua_register(LS, "copystring", CopyString);
	lua_register(LS, "setclipboard", CopyString);
	lua_register(LS, "toclipboard", CopyString);
	lua_register(LS, "getgenv", custom_getgenv);
	//lua_register(LS, "writefile", writefile);
	//lua_register(LS, "readfile", readfile);
	Inputs[0].type = INPUT_MOUSE;
	lua_register(LS, "mousemoverel", MouseMoveRelative);
	lua_register(LS, "MouseMoveRel", MouseMoveRelative);
	lua_register(LS, "mousemoverelative", MouseMoveRelative);
	lua_register(LS, "MouseMoveRelative", MouseMoveRelative);
	lua_register(LS, "MouseScroll", MouseScroll);
	lua_register(LS, "mousescroll", MouseScroll);

	lua_register(LS, "MouseButton1Click", Mouse1Click);
	lua_register(LS, "MouseButton1Press", Mouse1Down);
	lua_register(LS, "MouseButton1Release", Mouse1Up);
	lua_register(LS, "MouseButton2Click", Mouse2Click);
	lua_register(LS, "MouseButton2Press", Mouse2Down);
	lua_register(LS, "MouseButton2Release", Mouse2Up);

	lua_register(LS, "MouseButton1Down", Mouse1Down);
	lua_register(LS, "MouseButton1Up", Mouse1Up);
	lua_register(LS, "MouseButton2Down", Mouse2Down);
	lua_register(LS, "MouseButton2Up", Mouse2Up);
	lua_register(LS, "decompile", LuaU_decompile);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)input, NULL, NULL, NULL);

}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, LPVOID) {
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(Module);
		DWORD OldProtection;
		VirtualProtect(Module, 0x1000, PAGE_READWRITE, &OldProtection);
		ZeroMemory(Module, 0x1000);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Main, NULL, NULL, NULL);
		break;
	}
	return TRUE;
}
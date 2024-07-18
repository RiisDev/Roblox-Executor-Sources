

#include <iostream>
#include <fstream>
#include <string.h>
#include <Windows.h>
#include <sstream>
#include <vector>
#include <istream>
#include <iterator>
#include <string>
#include "detours.h"
#include "wincon.h"

#include <map>

#pragma comment(lib, "detours.lib")

 extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
} 

 int DataModel;
 int Workspace;
 int Players;
 int Lighting;
 int LocalPlayer;
 std::string* PlayerName;

 void Initiate();
 static int Index(lua_State* L);
 static int InstanceNew(lua_State* L);
void PushInstance(lua_State* L, DWORD addy);
 int GetParent(int Instance) {
	 return *(int*)(Instance + 0x34);
 }

 std::string* GetName(int Instance) {
	 return (std::string*)(*(int*)(Instance + 0x28));
 }

 int FindFirstChild_PartOf(int Instance, std::string Name) {
	 DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	 DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	 for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	 {
		 if (strstr(GetName(*(int*)i)->c_str(), Name.c_str()))
		 {
			 return *(int*)i;
		 }
	 }
	 return 0;
 }

 int FindFirstChild(int Instance, std::string Name) {
	 DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	 DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	 for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	 {
		 std::string* CurrentName = GetName(*(int*)i);
		 CurrentName->resize(Name.size());
		 if (*CurrentName == Name)
		 {
			 return *(int*)i;
		 }
	 }
	 return 0;
 }

 typedef int(__thiscall *getClassA)(int self);
 getClassA GetClassImpl = (getClassA)0x513150;

 const char* GetClass(int self)
 {
	 //return (const char*)GetClassImpl(self);
	 return (const char*)(*(int(**)(void))(*(int*)self + 16))() + 4;
 }

 typedef int(__thiscall *clrC)(int Instance);
 clrC ClearAllChildren = (clrC)0xE3F2C0;

 int FindFirstClass(int Instance, const char* Name)
 {
	 DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	 DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);

	 for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	 {
		 if (memcmp(GetClass(*(int*)i), Name, strlen(Name)) == 0)
		 {
			 return *(int*)i;
		 }
	 }
 }

 int GetLocalPlayer(int Plr) {
	 return *(int*)(Plr + 0x124);
 }

 int aGetCharacter(std::string Name) {
	 if (Name == "me") {
		 int Character = FindFirstChild_PartOf(Workspace, *PlayerName);
		 if (Character != 0) {
			 return Character;
		 }
		 else {
			 return 0;
		 }
	 }
	 int Character = FindFirstChild_PartOf(Workspace, Name);
	 if (Character != 0) {
		 return Character;
	 }
	 else {
		 return 0;
	 }
 }

 int aGetPlayer(std::string Name) {
	 int Player = FindFirstChild_PartOf(Players, Name);
	 if (Player != 0) {
		 return Player;
	 }
	 else {
		 return 0;
	 }
 }

 std::vector<int> GetChildren(int Instance) {
	 int StartOfChildren = *(int*)(Instance + 0x2C);
	 int EndOfChildren = *(int*)(StartOfChildren + 4);
	 std::vector<int> Children;
	 for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	 {
		 Children.push_back(*(int*)i);
	 }

	 return Children;
 }



// Vars
int AllowConsole = (int)GetProcAddress(GetModuleHandle("KERNELBASE.dll"), "FreeConsole");
int ScriptContext;
int rlua_State;
int rbxlua_State;

void Init(lua_State* L);
void executescript(std::string kekd);

void patchedconsole() {
	return;
}

HWND MainWindowHWND;
HMENU hMenu;
HMENU hMenuPopupFile;
HMENU hMenuPopupAbout;
HMODULE hInstance;
HWND ScriptTextBoxHWND;
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

#define MYMENU_EXIT                     (WM_APP + 101)
#define MYMENU_ABOUTMB                  (WM_APP + 102) 
#define MYMENU_EXECUTECODE              (WM_APP + 103)
#define MYMENU_SCRIPTTEXTBOX            (WM_APP + 104)
#define MYMENU_CLEARSCRIPT              (WM_APP + 105)
#define MYMENU_MINIMIZE                 (WM_APP + 109)

LRESULT CALLBACK DLLWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		switch (wParam)
		{
		case MYMENU_EXIT:
			if (MessageBox(0, "Are you sure you want to close stigma?", "wtf r u doin", MB_YESNO) == IDYES)
				SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case MYMENU_MINIMIZE:
			ShowWindow(hwnd, SW_MINIMIZE);
			break;
		case MYMENU_ABOUTMB:
			MessageBox(hwnd, "Original script by 9/11\nProgram by [FaZe] GabeN (aka Asymmetry)", "About", MB_OK);
			break;
		case MYMENU_CLEARSCRIPT:
			SetDlgItemText(hwnd, MYMENU_SCRIPTTEXTBOX, "");
			break;
		case MYMENU_EXECUTECODE:
			int length;
			length = SendMessage(ScriptTextBoxHWND, WM_GETTEXTLENGTH, 0, 0);
			if (length == -1)
				break;
			char buff[80896]; // = 1024 * 79
			char len[255];
			_itoa_s(length, len, 10);
			GetDlgItemText(hwnd, MYMENU_SCRIPTTEXTBOX, buff, length + 1);

			executescript(buff);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

BOOL RegisterDLLWindowClass(char *szClassName)
{
	WNDCLASSEX wc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = szClassName;
	wc.lpfnWndProc = DLLWindowProc;
	wc.style = CS_DBLCLKS;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = "Test";
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	if (!RegisterClassEx(&wc))
		return 0;
	return 1;
}

void RefreshContextMenu(HMENU hhMenu)
{
	hMenuPopupFile = CreatePopupMenu();
	AppendMenu(hMenuPopupFile, MF_STRING, MYMENU_MINIMIZE, TEXT("Minimize"));
	AppendMenu(hMenuPopupFile, MF_STRING, MYMENU_EXIT, TEXT("Exit"));
	AppendMenu(hhMenu, MF_POPUP, (UINT_PTR)hMenuPopupFile, TEXT("File"));

	hMenuPopupAbout = CreatePopupMenu();
	AppendMenu(hMenuPopupAbout, MF_STRING, MYMENU_ABOUTMB, TEXT("About"));
	AppendMenu(hhMenu, MF_POPUP, (UINT_PTR)hMenuPopupAbout, TEXT("Help"));

	HMENU scriptFiles = CreatePopupMenu();
	AppendMenu(scriptFiles, MF_STRING, MYMENU_ABOUTMB, TEXT("Scripts"));
	AppendMenu(hhMenu, MF_POPUP, (UINT_PTR)scriptFiles, TEXT("Metatables -> Execute"));

}

HMENU CreateDLLWindowMenu()
{
	HMENU heyMenu;
	heyMenu = CreateMenu();

	if (heyMenu == NULL)
		return FALSE;

	RefreshContextMenu(heyMenu);
	return heyMenu;
}

HWND AuthenciateScriptBox;

void CreateFWindows()
{
	CreateWindow("BUTTON", "EXECUTE SCRIPT", WS_CHILD | WS_VISIBLE, 407, 253, 204, 34, MainWindowHWND, (HMENU)MYMENU_EXECUTECODE, hInstance, NULL);
	CreateWindow("BUTTON", "CLEAR SCRIPT", WS_CHILD | WS_VISIBLE, 407, 293, 204, 34, MainWindowHWND, (HMENU)MYMENU_CLEARSCRIPT, hInstance, NULL);
	ScriptTextBoxHWND = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_BORDER | WS_VSCROLL, 0, 0, 622, 220,  MainWindowHWND, (HMENU)MYMENU_SCRIPTTEXTBOX, hInstance, 0);
	AuthenciateScriptBox = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_BORDER | WS_VSCROLL, 12, 253, 269, 82, MainWindowHWND, (HMENU)MYMENU_SCRIPTTEXTBOX, hInstance, 0);
	SendMessage(ScriptTextBoxHWND, EM_SETLIMITTEXT, 0x7FFFFFFE, 0);
}

void ShowForm()
{
	hInstance = GetModuleHandle(NULL);
	hMenu = CreateDLLWindowMenu();
	RegisterDLLWindowClass("DLLWindowClass");
	MainWindowHWND = CreateWindowEx(WS_EX_TOPMOST, "DLLWindowClass", "Stack!Overflow | By Donny & Ice bear", WS_EX_PALETTEWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 623, 400, NULL, hMenu, hInstance, NULL);
	CreateFWindows();
	ShowWindow(MainWindowHWND, SW_SHOWNORMAL);
	Initiate();
	MSG messages;
	while (GetMessage(&messages, NULL, 0, 0))
	{
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}
}

// magix
std::string IntToHex(int Val) {
	std::stringstream stream;
	stream << std::hex << Val;
	return stream.str();
}
//

namespace Memory {
	bool Compare(const BYTE *pData, const BYTE *bMask, const char *szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++bMask)
			if (*szMask == 'x' && *pData != *bMask) return 0;
		return (*szMask) == NULL;
	}

	DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask)
	{
		for (int i = 0; i<(int)dwLen; i++)
			if (Compare((BYTE*)(dwAddress + (int)i), bMask, szMask))  return (int)(dwAddress + i);
		return 0;
	}

	int Scan(DWORD mode, char* content, char* mask)
	{
		DWORD PageSize;
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		PageSize = si.dwPageSize;
		MEMORY_BASIC_INFORMATION mi;
		for (DWORD lpAddr = 0; lpAddr<0x7FFFFFFF; lpAddr += PageSize)
		{
			DWORD vq = VirtualQuery((void*)lpAddr, &mi, PageSize);
			if (vq == ERROR_INVALID_PARAMETER || vq == 0) break;
			if (mi.Type == MEM_MAPPED) continue;
			if (mi.Protect == mode)
			{
				int addr = FindPattern(lpAddr, PageSize, (PBYTE)content, mask);
				if (addr != 0)
				{
					return addr;
				}
			}
		}
	}
}

std::vector<std::string> split(std::string s) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	return vstrings;
}

DWORD base1 = (DWORD)GetModuleHandle(NULL);

DWORD getaddy(DWORD address) {
	return (address - 0x400000 + base1);
}


DWORD GetAdr(DWORD address) {
	DWORD base = (DWORD)GetModuleHandle("RobloxPlayerBeta.exe");
	return address - 0x400000 + base;
}

DWORD aslr(DWORD address) {
	DWORD base = (DWORD)GetModuleHandle("RobloxPlayerBeta.exe");
	return address - 0x400000 + base;
}

DWORD oldProtection;
void setJb(DWORD addr) {
	DWORD oldProtection;
	VirtualProtect((LPVOID)addr, 0x05, PAGE_EXECUTE_READWRITE, &oldProtection);
	*(char*)addr = 0xEB;
	VirtualProtect((LPVOID)addr, 0x05, oldProtection, &oldProtection);
}
void restoreJb(DWORD addr) {
	DWORD oldProtection;
	VirtualProtect((void*)addr, 5, PAGE_EXECUTE_READWRITE, &oldProtection);
	*(char*)addr = 0x72;
	VirtualProtect((void*)addr, 5, oldProtection, &oldProtection);
}

typedef int(__cdecl *Lua_newState)();
typedef void(__cdecl *Lua_getfield)(int lua_State, int idx, const char *k);
typedef int(__cdecl *Lua_pcall)(int lua_State, int nargs, int nresults, int errfunc);
typedef void(__cdecl *Lua_pushstring)(int lua_State, const char *s);
typedef const char* (__cdecl *Lua_tolstring)(int lua_State, int idx, size_t *len);
typedef void(__cdecl *Lua_pushvalue)(int lua_State, int idx);
typedef int(__cdecl *Lua_gettop)(int lua_State);
typedef void(__cdecl *Lua_settop)(int lua_State, int n);
typedef void(__cdecl *Lua_setfield)(int lua_State, int idx, const char *k);
typedef void(__cdecl *Lua_pushcclosure)(int lua_State, int(*fn)(int l), int n);
typedef void(__cdecl *Lua_remove)(int lua_State, int idx);
typedef void(__cdecl *Lua_pushnumber)(int lua_State, double number);
typedef int(__cdecl *Lua_getmetatable)(int lua_State, int objindex);
typedef void(__cdecl *Lua_gettable)(int lua_State, int idx);
typedef int(__cdecl *Lua_ref)(int lua_State, int t);
typedef void(__cdecl *Lua_pushnil)(int lua_State);
typedef int(__cdecl *Lua_next)(int lua_State, int idx);
typedef bool(__cdecl *Lua_toboolean)(int lua_State, int idx);
typedef bool(__cdecl *Lua_isstring)(int lua_State, int idx);
typedef bool(__cdecl *Lua_isuserdata)(int lua_State, int idx);
typedef bool(__cdecl *Lua_isnumber)(int lua_State, int idx);
typedef int(__cdecl *Lua_newthread)(int lua_State);
typedef bool(__cdecl *Lua_pushboolean)(int lua_State, int a1);
typedef int(__cdecl *Lua_setmetatable)(int lua_State, int a1);
typedef int(__cdecl *Lua_equal)(int lua_State, int a1, int a2);
typedef int(__cdecl *Lua_type)(int lua_State, int a1);
typedef void(__cdecl *Lua_call)(int lua_State, int nargs, int nresults);
typedef void(__cdecl *Lua_createtable)(int lua_State, int idx);
typedef void*       (__cdecl *Lua_touserdata)(int lua_State, int idx);
typedef void(__cdecl *Lua_replace)(int lua_State, int idx);
typedef double(__cdecl *Lua_tonumber)(int lua_State, int idx);

typedef int(__cdecl *Lua_resume)(int lua_State, int nArg);

typedef int(__thiscall *newRblxState)(void *thisPtr);
typedef int(__cdecl *Lua_newuserdata)(int L, size_t size);

Lua_touserdata rbxlua_touserdata = (Lua_touserdata)GetAdr(0x54DEB0);
Lua_newuserdata rbxlua_newuserdata = (Lua_newuserdata)GetAdr(0x54C480);
Lua_setmetatable rbxlua_setmetatable = (Lua_setmetatable)GetAdr(0x54D6A0);
Lua_pushnil rbxlua_pushnil = (Lua_pushnil)GetAdr(0x54CB10);
typedef int(__cdecl* InstanceNew21)(int);
InstanceNew21 InstanceNew2 = (InstanceNew21)GetAdr(0x729480);

DWORD rbxlua_State2;
DWORD GetFieldJb = GetAdr(0x54B904);
DWORD PcallJb = GetAdr(0x54C609);
DWORD SetfieldJb = GetAdr(0x54D468); //done
DWORD PushValueJb = GetAdr(0x054CBEC);
DWORD SettopJb = GetAdr(0x558521);
DWORD PushNumberJB = GetAdr(0x557F97);
DWORD PushBooleanJB = GetAdr(0x55785E);
DWORD tobooleanJB = GetAdr(0x54C0AF);
namespace lua {
	typedef void*(__cdecl *Lua_getfield)(int lua_State, int idx, const char *k);
	Lua_getfield lua_getfield = (Lua_getfield)GetAdr(0x54B890);
	typedef void*(__cdecl *pCall)(int lua_State, int naArgs, int nresults, int errfunc);
	pCall lua_pcall = (pCall)GetAdr(0x54C580);
	typedef void*(__cdecl *Lua_setfield)(int lua_State, int idx, const char *k);
	Lua_setfield lua_setfield = (Lua_setfield)GetAdr(0x54D3F0);
	typedef void*(__cdecl *Lua_pushvalue)(int lua_State, int idx);
	Lua_pushvalue lua_pushvalue = (Lua_pushvalue)GetAdr(0x54CBA0);
	typedef BOOL(__cdecl *lua_ToBoolean)(int lua_State, int idx);
	lua_ToBoolean lua_toboolean = (lua_ToBoolean)GetAdr(0x54BFE0);

	typedef const char(__cdecl *lua_ToLString)(int lua_State, int idx, size_t *len);
	lua_ToLString rbxlua_tolstring = (lua_ToLString)GetAdr(0x53B7D0);

	typedef void*(__cdecl *Lua_settop)(int lua_State, int n);
	Lua_settop rbxlua_settop = (Lua_settop)GetAdr(0x5586F0);

	typedef bool(__cdecl *Lua_pushboolean)(int lua_State, int a1);
	Lua_pushboolean rbxlua_pushboolean = (Lua_pushboolean)GetAdr(0x557830);
}
void rbxlua_getfield(int ls, int idx, const char *k) {
	setJb(GetFieldJb);
	lua::lua_getfield(ls, idx, k);
	restoreJb(GetFieldJb);
}

typedef void*(__cdecl *Lua_PushNumber)(int lua_State, double number);
Lua_PushNumber rbxlua_pushnumber = (Lua_PushNumber)GetAdr(0x54CB80);

typedef const char*(__cdecl *lua_ToLString)(int lua_State, int idx, size_t *len);
lua_ToLString rbxlua_tolstring = (lua_ToLString)GetAdr(0x53B7D0);

void rbxlua_pcall(int ls, int nar, int nre, int nfunc) {
	setJb(PcallJb);
	lua::lua_pcall(ls, nar, nre, nfunc);
	restoreJb(PcallJb);
}
void rbxlua_setfield(int ls, int idx, const char *k) {
	setJb(SetfieldJb);
	lua::lua_setfield(ls, idx, k);
	restoreJb(SetfieldJb);
}
void rbxlua_pushvalue(int ls, int v) {
	setJb(PushValueJb);
	lua::lua_pushvalue(ls, v);
	restoreJb(PushValueJb);
}
void rbxlua_pushboolean(int ls, int a) {
	setJb(PushBooleanJB);
	lua::rbxlua_pushboolean(ls, a);
	restoreJb(PushBooleanJB);
}


Lua_settop rbxlua_settop = (Lua_settop)GetAdr(0x552310);

Lua_gettop rbxlua_gettop = (Lua_gettop)GetAdr(0x54BE50);

Lua_type rbxlua_type = (Lua_type)GetAdr(0x54DE20);


Lua_pushstring rbxlua_pushstring = (Lua_pushstring)GetAdr(0x54CAE0);
typedef int*(__cdecl *lua_GetContext)();
lua_GetContext context = (lua_GetContext)GetAdr(0x665B80);
typedef bool*(__cdecl * Lua_Toboolean)(int lua_State, bool a2);


typedef int(__thiscall * LoadAssetId)(void* load, int id);
LoadAssetId LoadAssetGet = (LoadAssetId)GetAdr(0x896FE0);

void rbxlua_getglobal(int luaState, const char* k) {
	rbxlua_getfield(luaState, -10002, k);
	return;
};
const char* rbxlua_tostring(int luaState, int idx) {
	lua::rbxlua_tolstring(luaState, idx, NULL);
	return 0;
};
bool rbxlua_isnil(int luaState, int n) {
	///(lua_type(luaState, (n)) == 0);
	return 0;
};

void rbxlua_pop(int luaState, int n) {

	//lua_settop(luaState, -(n)-1);
};

void rbxlua_emptystack(int luaState)
{

	//lua_settop(luaState, 0);
};

bool rbxlua_toboolean(int luaState, int idx) {
	setJb(tobooleanJB);
	return lua::lua_toboolean(luaState, idx);
	restoreJb(tobooleanJB);
}

typedef int(__thiscall *ns)(int self);
ns openstate = (ns)GetAdr(0x052D620);


std::string Input() {
	std::string wot;
	getline(std::cin, wot);
	return wot;
}

int context7() {
	do {
		*(DWORD*)(GetAdr(0x1A21D24)) = 7; // omg context elevation is so hot!!!
	} while (true);
	return 0;
}




void Music(std::string id) {
	try {
		rbxlua_getglobal(rlua_State, "workspace");
		rbxlua_getglobal(rlua_State, "Instance");
		rbxlua_getfield(rlua_State, -1, "new");
		rbxlua_pushstring(rlua_State, "Sound");
		rbxlua_pushvalue(rlua_State, -4);
		rbxlua_pcall(rlua_State, 2, 1, 0);

		rbxlua_pushstring(rlua_State, ("rbxassetid://" + id).c_str());
		rbxlua_setfield(rlua_State, -2, "SoundId");

		rbxlua_pushstring(rlua_State, "POSITRON_AUDIO");
		rbxlua_setfield(rlua_State, -2, "Name");

		rbxlua_pushnumber(rlua_State, 1);
		rbxlua_setfield(rlua_State, -2, "Volume");

		rbxlua_getfield(rlua_State, -1, "Play");
		rbxlua_pushvalue(rlua_State, -2);
		rbxlua_pcall(rlua_State, 1, 0, 0);
	}
	catch (...) {};
}


void earrape() { // id 536554833
	try {
		rbxlua_getglobal(rlua_State, "workspace");
		rbxlua_getglobal(rlua_State, "Instance");
		rbxlua_getfield(rlua_State, -1, "new");
		rbxlua_pushstring(rlua_State, "Sound");
		rbxlua_pushvalue(rlua_State, -4);
		rbxlua_pcall(rlua_State, 2, 1, 0);

		rbxlua_pushstring(rlua_State, "rbxassetid://536554833");
		rbxlua_setfield(rlua_State, -2, "SoundId");

		rbxlua_pushstring(rlua_State, "POSITRON_AUDIO");
		rbxlua_setfield(rlua_State, -2, "Name");

		rbxlua_pushnumber(rlua_State, 1);
		rbxlua_setfield(rlua_State, -2, "Volume");

		rbxlua_getfield(rlua_State, -1, "Play");
		rbxlua_pushvalue(rlua_State, -2);
		rbxlua_pcall(rlua_State, 1, 0, 0);
	}
	catch (...) {};
}

void mello() { // id 536554833
	try {
		rbxlua_getglobal(rlua_State, "workspace");
		rbxlua_getglobal(rlua_State, "Instance");
		rbxlua_getfield(rlua_State, -1, "new");
		rbxlua_pushstring(rlua_State, "Sound");
		rbxlua_pushvalue(rlua_State, -4);
		rbxlua_pcall(rlua_State, 2, 1, 0);

		rbxlua_pushstring(rlua_State, "rbxassetid://397873706");
		rbxlua_setfield(rlua_State, -2, "SoundId");

		rbxlua_pushstring(rlua_State, "POSITRON_AUDIO");
		rbxlua_setfield(rlua_State, -2, "Name");

		rbxlua_pushnumber(rlua_State, 1);
		rbxlua_setfield(rlua_State, -2, "Volume");

		rbxlua_getfield(rlua_State, -1, "Play");
		rbxlua_pushvalue(rlua_State, -2);
		rbxlua_pcall(rlua_State, 1, 0, 0);
	}
	catch (...) {};
}

void songkek() { // id 535846635
	try {
		rbxlua_getglobal(rlua_State, "workspace");
		rbxlua_getglobal(rlua_State, "Instance");
		rbxlua_getfield(rlua_State, -1, "new");
		rbxlua_pushstring(rlua_State, "Sound");
		rbxlua_pushvalue(rlua_State, -4);
		rbxlua_pcall(rlua_State, 2, 1, 0);

		rbxlua_pushstring(rlua_State, "rbxassetid://535846635");
		rbxlua_setfield(rlua_State, -2, "SoundId");

		rbxlua_pushstring(rlua_State, "POSITRON_AUDIO");
		rbxlua_setfield(rlua_State, -2, "Name");

		rbxlua_pushnumber(rlua_State, 1);
		rbxlua_setfield(rlua_State, -2, "Volume");

		rbxlua_getfield(rlua_State, -1, "Play");
		rbxlua_pushvalue(rlua_State, -2);
		rbxlua_pcall(rlua_State, 1, 0, 0);
	}
	catch (...) {};
}

void ogodnu() { // id 535846635
	try {
		rbxlua_getglobal(rlua_State, "workspace");
		rbxlua_getglobal(rlua_State, "Instance");
		rbxlua_getfield(rlua_State, -1, "new");
		rbxlua_pushstring(rlua_State, "Sound");
		rbxlua_pushvalue(rlua_State, -4);
		rbxlua_pcall(rlua_State, 2, 1, 0);

		rbxlua_pushstring(rlua_State, "rbxassetid://315340769");
		rbxlua_setfield(rlua_State, -2, "SoundId");

		rbxlua_pushstring(rlua_State, "POSITRON_AUDIO");
		rbxlua_setfield(rlua_State, -2, "Name");

		rbxlua_pushnumber(rlua_State, 1);
		rbxlua_setfield(rlua_State, -2, "Volume");

		rbxlua_getfield(rlua_State, -1, "Play");
		rbxlua_pushvalue(rlua_State, -2);
		rbxlua_pcall(rlua_State, 1, 0, 0);
	}
	catch (...) {};
}

void ducksong() { // 153123619
	try {
		rbxlua_getglobal(rlua_State, "workspace");
		rbxlua_getglobal(rlua_State, "Instance");
		rbxlua_getfield(rlua_State, -1, "new");
		rbxlua_pushstring(rlua_State, "Sound");
		rbxlua_pushvalue(rlua_State, -4);
		rbxlua_pcall(rlua_State, 2, 1, 0);

		rbxlua_pushstring(rlua_State, "rbxassetid://153123619");
		rbxlua_setfield(rlua_State, -2, "SoundId");

		rbxlua_pushstring(rlua_State, "POSITRON_AUDIO");
		rbxlua_setfield(rlua_State, -2, "Name");

		rbxlua_pushnumber(rlua_State, 1);
		rbxlua_setfield(rlua_State, -2, "Volume");

		rbxlua_getfield(rlua_State, -1, "Play");
		rbxlua_pushvalue(rlua_State, -2);
		rbxlua_pcall(rlua_State, 1, 0, 0);
	}
	catch (...) {};
}

void fusrohduh() { // 153123619
	try {
		rbxlua_getglobal(rlua_State, "workspace");
		rbxlua_getglobal(rlua_State, "Instance");
		rbxlua_getfield(rlua_State, -1, "new");
		rbxlua_pushstring(rlua_State, "Sound");
		rbxlua_pushvalue(rlua_State, -4);
		rbxlua_pcall(rlua_State, 2, 1, 0);

		rbxlua_pushstring(rlua_State, "rbxassetid://130776150");
		rbxlua_setfield(rlua_State, -2, "SoundId");

		rbxlua_pushstring(rlua_State, "POSITRON_AUDIO");
		rbxlua_setfield(rlua_State, -2, "Name");

		rbxlua_pushnumber(rlua_State, 1);
		rbxlua_setfield(rlua_State, -2, "Volume");

		rbxlua_getfield(rlua_State, -1, "Play");
		rbxlua_pushvalue(rlua_State, -2);
		rbxlua_pcall(rlua_State, 1, 0, 0);
	}
	catch (...) {};
}

void cinemaskrillex() { // 153123619
	try {
		rbxlua_getglobal(rlua_State, "workspace");
		rbxlua_getglobal(rlua_State, "Instance");
		rbxlua_getfield(rlua_State, -1, "new");
		rbxlua_pushstring(rlua_State, "Sound");
		rbxlua_pushvalue(rlua_State, -4);
		rbxlua_pcall(rlua_State, 2, 1, 0);

		rbxlua_pushstring(rlua_State, "rbxassetid://130777688");
		rbxlua_setfield(rlua_State, -2, "SoundId");

		rbxlua_pushstring(rlua_State, "POSITRON_AUDIO");
		rbxlua_setfield(rlua_State, -2, "Name");

		rbxlua_pushnumber(rlua_State, 1);
		rbxlua_setfield(rlua_State, -2, "Volume");

		rbxlua_getfield(rlua_State, -1, "Play");
		rbxlua_pushvalue(rlua_State, -2);
		rbxlua_pcall(rlua_State, 1, 0, 0);
	}
	catch (...) {};
}

void ringing() { // 153123619
	try {
		rbxlua_getglobal(rlua_State, "workspace");
		rbxlua_getglobal(rlua_State, "Instance");
		rbxlua_getfield(rlua_State, -1, "new");
		rbxlua_pushstring(rlua_State, "Sound");
		rbxlua_pushvalue(rlua_State, -4);
		rbxlua_pcall(rlua_State, 2, 1, 0);

		rbxlua_pushstring(rlua_State, "rbxassetid://185801844");
		rbxlua_setfield(rlua_State, -2, "SoundId");

		rbxlua_pushstring(rlua_State, "POSITRON_AUDIO");
		rbxlua_setfield(rlua_State, -2, "Name");

		rbxlua_pushnumber(rlua_State, 1);
		rbxlua_setfield(rlua_State, -2, "Volume");

		rbxlua_getfield(rlua_State, -1, "Play");
		rbxlua_pushvalue(rlua_State, -2);
		rbxlua_pcall(rlua_State, 1, 0, 0);
	}
	catch (...) {};
}

void finalear() { // 153123619
	try {
		rbxlua_getglobal(rlua_State, "workspace");
		rbxlua_getglobal(rlua_State, "Instance");
		rbxlua_getfield(rlua_State, -1, "new");
		rbxlua_pushstring(rlua_State, "Sound");
		rbxlua_pushvalue(rlua_State, -4);
		rbxlua_pcall(rlua_State, 2, 1, 0);

		rbxlua_pushstring(rlua_State, "rbxassetid://390040605");
		rbxlua_setfield(rlua_State, -2, "SoundId");

		rbxlua_pushstring(rlua_State, "POSITRON_AUDIO");
		rbxlua_setfield(rlua_State, -2, "Name");

		rbxlua_pushnumber(rlua_State, 1);
		rbxlua_setfield(rlua_State, -2, "Volume");

		rbxlua_getfield(rlua_State, -1, "Play");
		rbxlua_pushvalue(rlua_State, -2);
		rbxlua_pcall(rlua_State, 1, 0, 0);
	}
	catch (...) {};
}

void anonpart() { // id 535846635
	try {
		rbxlua_getglobal(rlua_State, "workspace");
		rbxlua_getglobal(rlua_State, "Instance");
		rbxlua_getfield(rlua_State, -1, "new");
		rbxlua_pushstring(rlua_State, "ParticleEmitter");
		rbxlua_pushvalue(rlua_State, -4);
		rbxlua_pcall(rlua_State, 2, 1, 0);

		rbxlua_pushstring(rlua_State, "http://www.roblox.com/asset/?id=127476787");
		rbxlua_setfield(rlua_State, -2, "Texture");
		rbxlua_pushvalue(rlua_State, -2);
		rbxlua_pcall(rlua_State, 1, 0, 0);
	}
	catch (...) {};
}

void ChangeStat(std::string name, float val) {
	rbxlua_getglobal(rlua_State, "game");
	rbxlua_getfield(rlua_State, -1, "Players");
	rbxlua_getfield(rlua_State, -1, "LocalPlayer");
	rbxlua_getfield(rlua_State, -1, "playerstats");
	rbxlua_pushnumber(rlua_State, val);
	rbxlua_setfield(rlua_State, -2, name.c_str());
	rbxlua_pcall(rlua_State, 1, 0, 0);
	rbxlua_emptystack(rlua_State);
}

void GodPlr(const char* User) {
	rbxlua_getglobal(rlua_State, "game");
	rbxlua_getfield(rlua_State, -1, "Players");
	rbxlua_getfield(rlua_State, -1, User);

	rbxlua_getfield(rlua_State, -1, "Character");


	rbxlua_getfield(rlua_State, -1, "FindFirstChild");
	rbxlua_pushvalue(rlua_State, -2);
	rbxlua_pushstring(rlua_State, "Humanoid");
	rbxlua_pcall(rlua_State, 2, 1, 0);


	rbxlua_getglobal(rlua_State, "math");

	rbxlua_getfield(rlua_State, -1, "huge");
	rbxlua_setfield(rlua_State, -3, "MaxHealth");

	rbxlua_pop(rlua_State, 1);

	rbxlua_getfield(rlua_State, -1, "MaxHealth");
	rbxlua_setfield(rlua_State, -2, "Health");
}

void Print2(HWND hWndEdit, std::string pszText)
{
	int nLength = GetWindowTextLength(hWndEdit);
	SendMessage(hWndEdit, EM_SETSEL, (WPARAM)nLength, (LPARAM)nLength);
	SendMessage(hWndEdit, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)pszText.c_str());
}

static int printconsole(lua_State* L) {
	int nLength = GetWindowTextLength(AuthenciateScriptBox);
	std::string pszText = lua_tostring(L, -1);
	SendMessage(AuthenciateScriptBox, EM_SETSEL, (WPARAM)nLength, (LPARAM)nLength);
	SendMessage(AuthenciateScriptBox, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)pszText.c_str());
	return 0;
}

bool getFiltering() {
	rbxlua_getglobal(rbxlua_State, "game");
	rbxlua_getfield(rbxlua_State, -1, "Workspace");
	rbxlua_getfield(rbxlua_State, -1, "FilteringEnabled");
	bool isFilteringEnabledOrDisabled = rbxlua_toboolean(rlua_State, -1);
	if (!isFilteringEnabledOrDisabled) {
		Print2(AuthenciateScriptBox, "Filtering is ENABLED! - Most cmds may not work propley in-games.\n");
	}
	else {
		Print2(AuthenciateScriptBox, "Filtering is disabled! :) Enjoy.\n");
	}
	return false;
}

int GetObjects2;

int GetRawTable(lua_State* L) {
	int a = lua_getmetatable(L, -1);
	return a;
}

static int GetService(lua_State* L) {
	lua_getfield(L, -1, "__address");
	int Instance = lua_tonumber(L, -1);
	lua_pop(L, 2);
	lua_newtable(L);
	DWORD StartOfChildren = *(DWORD*)(DataModel + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		PushInstance(L, *(int*)i);
		lua_setfield(L, -2, std::to_string(i).c_str());
	}
	return 1;
}

void Initiate() {
	Print2(AuthenciateScriptBox, "Scanning... ");
	//DataModel = GetParent(ScriptContext);
	DWORD ScriptContextPtr = GetAdr(0x118A75C);
	ScriptContext = Memory::Scan(PAGE_READWRITE, (char*)&ScriptContextPtr, "xxxx");
	DataModel = GetParent(ScriptContext);
	rbxlua_State = openstate(ScriptContext);
	lua_State* L = luaL_newstate();
	Init(L);
	GetRawTable(L);
	luaopen_string(L);
	luaopen_table(L);
	luaopen_debug(L);
	luaopen_math(L);
	GetService(L);
	Print2(AuthenciateScriptBox, "OK\r\n");
	//Sleep(500);
	Print2(AuthenciateScriptBox, "Authencating... ");
	//Sleep(1000);
	Print2(AuthenciateScriptBox, "Welcome, user!\r\n");
	/* if (luaL_dofile(L, "C:\\Users\\Husky\\Desktop\\Stack!OverFlow_ EXE\\Stack!OverFlow_\\init.lua")) {
		const char* erm = lua_tostring(L, -1);
		std::string erm2 = "ERROR COMPLILING\n:";
		erm2 += erm;
		Print2(AuthenciateScriptBox, erm2.c_str());
	}
	Print2(AuthenciateScriptBox, "Loaded lua File: init.lua\n");
	//getFiltering(); */
}



void Main() {
	ShowForm();
}

int GetObjects(lua_State* L) {
	std::string erm = lua_tostring(L, -1);
	int erm2 = lua_tonumber(L, 1);
	rbxlua_getglobal(rbxlua_State, "game");
	rbxlua_getfield(rbxlua_State, -1, "GetService");
	rbxlua_getfield(rbxlua_State, -1, "InsertService");
	rbxlua_pushstring(rbxlua_State, "function loadasset(asset_name) asseturl = 'rbxasset://../../assets/' .. asset_name.. '.rbxm' local obj = game:GetObjects(asseturl)[1] if obj ~= nil then return obj else return nil end end function loadtool(toolName) local tool = loadasset(toolName) if tool ~= nil then tool.Parent = game.Players.LocalPlayer.Backpack end end function loadscript(scriptName) local src = loadasset(scriptName) if src then Spawn(function()loadstring(src.Source)()end) end end");
	rbxlua_pushnumber(rbxlua_State, lua_tonumber(L, erm2));
	rbxlua_pushvalue(rbxlua_State, -2);
	rbxlua_pcall(rbxlua_State, 2, 0, 0);
	return 0;
}

// start of env111!!

void PushInstance(lua_State *L, DWORD addy) {
	lua_newtable(L);
	lua_pushnumber(L, addy);
	lua_setfield(L, -2, "__address");

	lua_getglobal(L, "robject");
	lua_setmetatable(L, -2);
}

static int CallMethod(lua_State* L) {
	int ArgCount, fArgs = 0;
	try {
		for (int x = lua_gettop(L); x != 0; x -= 1) {
			switch (lua_type(L, x * -1)) {
			case LUA_TNIL: {
				rbxlua_pushnil(rbxlua_State);
				break;
			}
			case LUA_TNUMBER: {
				double no = lua_tonumber(L, x * -1);
				rbxlua_pushnumber(rbxlua_State, no);
				break;
			}
			case LUA_TSTRING: {
				const char* str = lua_tostring(L, x * -1);
				rbxlua_pushstring(rbxlua_State, str);
				break;
			}
			case LUA_TTABLE: {
				lua_getfield(L, -1, "__address");
				if (!lua_isnoneornil(L, -1)) {
					int* userdata = (int*)rbxlua_newuserdata(rbxlua_State, 8);
					*userdata = lua_tonumber(L, x * -1);
					*(userdata + 1) = 0;
					rbxlua_getfield(rbxlua_State, -10000, "Object");
					rbxlua_setmetatable(rbxlua_State, -2);
					lua_pop(L, 1);
				}
				else {
					lua_pop(L, 1);
					rbxlua_pushstring(rbxlua_State, "table: #NULL");
				}
				break;
			}
			case LUA_TFUNCTION: {
				break;
			}
			default:
			{
				const char* str = lua_tostring(L, x * -1);
				rbxlua_pushstring(rbxlua_State, str);
				break;
			}
			}
			ArgCount += 1;
		}
		rbxlua_pcall(rbxlua_State, ArgCount, LUA_MULTRET, 0);
		for (int x = rbxlua_gettop(rbxlua_State); x != 0; x -= 1) {
			switch (rbxlua_type(rbxlua_State, x * -1)) {
			case LUA_TNIL: {
				lua_pushnil(L);
				break;
			}
			case LUA_TNUMBER: {
				const char* str = rbxlua_tolstring(rbxlua_State, x * -1, NULL);
				lua_pushnumber(L, ::atof(str));
				break;
			}
			case LUA_TBOOLEAN: {
				bool _bool = rbxlua_toboolean(rbxlua_State, x * -1);
				lua_pushboolean(L, _bool);
				break;
			}
			case LUA_TSTRING: {
				const char* str = rbxlua_tolstring(rbxlua_State, x * -1, NULL);
				lua_pushstring(L, str);
				break;
			}
			case LUA_TUSERDATA: {
				int u = (int)rbxlua_touserdata(rbxlua_State, -1);
				PushInstance(L, u);
				break;
			}
			}
			fArgs += 1;
		}
	}
	catch (std::exception e) {
		std::string err = "C ERROR: ";
		err += e.what();
		std::cout << err.c_str() << std::endl;
	}
	return fArgs;
}

static int GetChildren(lua_State* L) {
	lua_getfield(L, -1, "__address");
	int Instance = lua_tonumber(L, -1);
	lua_pop(L, 2);
	lua_newtable(L);
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		PushInstance(L, *(int*)i);
		lua_setfield(L, -2, std::to_string(i).c_str());
	}
	return 1;
}

static int LuaFindFirstChild(lua_State* L) {
	const char* Name = lua_tostring(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, -1, "__address");
	int rInstance = lua_tonumber(L, -1);
	lua_pop(L, 2);
	int Instance = FindFirstChild(rInstance, Name);
	if (Instance != 0) {
		PushInstance(L, Instance);
		return 1;
	}
	else {
		return 0;
	}
}



static int GetObjects_LuaC(lua_State* L) {
	std::string id = lua_tostring(L, -1);
	int numberid = lua_tonumber(L, 1);
	LoadAssetGet((void*)id.c_str(), numberid);
	return 0;
}

static int Index(lua_State* L) {
	std::string Key = lua_tostring(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -1, "__address");
	int AddressLoc = lua_tonumber(L, -1);
	lua_pop(L, 2);

	int* userdata = (int*)rbxlua_newuserdata(rbxlua_State, 8);
	*userdata = AddressLoc;
	*(userdata + 1) = 0;
	rbxlua_getfield(rbxlua_State, -10000, "Object");
	rbxlua_setmetatable(rbxlua_State, -2);

	if (Key == "GetChildren" || Key == "getChildren" || Key == "children") {
		lua_pushcfunction(L, GetChildren);
		return 1;
	}
	else if (Key == "FindFirstChild" || Key == "findFirstChild") {
		lua_pushcfunction(L, LuaFindFirstChild);
		return 1;
	}
	else if (Key == "Parent") {
		PushInstance(L, GetParent(AddressLoc));
		return 1;
	}
	else {
		rbxlua_getfield(rbxlua_State, -1, Key.c_str());
		int a = rbxlua_type(rbxlua_State, -1);
		switch (rbxlua_type(rbxlua_State, -1)) {
		case LUA_TNIL: {
			return 0;
		}
		case 2:
		case LUA_TNUMBER: {
			const char* str = rbxlua_tolstring(rbxlua_State, -1, NULL);
			lua_pushnumber(L, ::atof(str));
			return 1;
		}
		case LUA_TSTRING: {
			const char* str = rbxlua_tolstring(rbxlua_State, -1, NULL);
			lua_pushstring(L, str);
			return 1;
		}
		case 6: {
			lua_pushcfunction(L, CallMethod);
			return 1;
		}
		case 8: {
			int instance = FindFirstChild(AddressLoc, Key);
			PushInstance(L, instance);
			return 1;
		}
		}
	}
}


static int NewIndex(lua_State* L) {
	int Type = lua_type(L, -1);

	std::string Key = lua_tostring(L, -2);

	lua_getfield(L, -3, "__address");
	int AddressLoc = lua_tonumber(L, -1);
	lua_pop(L, 1);

	int* userdata = (int*)rbxlua_newuserdata(rbxlua_State, 8);
	*userdata = AddressLoc;
	*(userdata + 1) = 0;
	rbxlua_getfield(rbxlua_State, -10000, "Object");
	rbxlua_setmetatable(rbxlua_State, -2);

	switch (Type) {
	case LUA_TNUMBER: {
		double no = lua_tonumber(L, -1);
		rbxlua_pushnumber(rbxlua_State, no);
		rbxlua_setfield(rbxlua_State, -2, Key.c_str());
		return 0;
	}
	case LUA_TSTRING: {
		const char* str = lua_tostring(L, -1);
		rbxlua_pushstring(rbxlua_State, str);
		rbxlua_setfield(rbxlua_State, -2, Key.c_str());
		return 0;
	}
	case LUA_TBOOLEAN: {
		bool a = lua_toboolean(L, -1);
		if (a) {
			rbxlua_pushnumber(rbxlua_State, 1);
		}
		else {
			rbxlua_pushnil(rbxlua_State);
		}
		rbxlua_setfield(rbxlua_State, -2, Key.c_str());
		return 0;
	}
	case LUA_TTABLE: {
		lua_getfield(L, -1, "__address");
		if (!lua_isnil(L, -1)) {
			int inst = lua_tonumber(L, -1);
			lua_pop(L, 1);
			int* userdata = (int*)rbxlua_newuserdata(rbxlua_State, 8);
			*userdata = inst;
			*(userdata + 1) = 0;
			rbxlua_getfield(rbxlua_State, -10000, "Object");
			rbxlua_setmetatable(rbxlua_State, -2);
			rbxlua_setfield(rbxlua_State, -2, Key.c_str());
		}
		else {
			lua_pop(L, 1);
		}
		return 0;
	}
	}
}

static int SpawnThread(lua_State *L) {
	lua_State* nL = lua_newthread(L);
}


static int Print(lua_State *L) {
	rbxlua_getfield(rbxlua_State, LUA_GLOBALSINDEX, "print");
	int ArgCount = 0;
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		switch (lua_type(L, x * -1)) {
		case LUA_TNIL: {
			rbxlua_pushnil(rbxlua_State);
			break;
		}
		case LUA_TNUMBER: {
			double no = lua_tonumber(L, x * -1);
			rbxlua_pushnumber(rbxlua_State, no);
			break;
		}
		case LUA_TSTRING: {
			const char* str = lua_tostring(L, x * -1);
			rbxlua_pushstring(rbxlua_State, str);
			break;
		}
		case LUA_TTABLE: {
			lua_getfield(L, -1, "__address");
			if (!lua_isnoneornil(L, -1)) {
				int* userdata = (int*)rbxlua_newuserdata(rbxlua_State, 8);
				*userdata = lua_tonumber(L, x * -1);
				*(userdata + 1) = 0;
				rbxlua_getfield(rbxlua_State, -10000, "Object");
				rbxlua_setmetatable(rbxlua_State, -2);
			}
			else {
				lua_pop(L, 1);
				rbxlua_pushstring(rbxlua_State, "table: #NULL");
			}
			break;
		}
		default:
		{
			const char* str = lua_tostring(L, x * -1);
			rbxlua_pushstring(rbxlua_State, str);
			break;
		}
		}
		ArgCount += 1;
	}
	rbxlua_pcall(rbxlua_State, ArgCount, 0, 0);
	return 0;
}

static int Warn(lua_State* L) {
	std::string code = lua_tostring(L, -1);
	rbxlua_getglobal(rbxlua_State, "warn");
	rbxlua_pushstring(rbxlua_State, code.c_str());
	rbxlua_pcall(rbxlua_State, 1, 0, 0);
	return 0;
}

void ExecuteChathook(std::string Src) {
	rbxlua_getglobal(rbxlua_State, "loadstring"); // Push loadstring onto the stack
	rbxlua_pushstring(rbxlua_State, Src.c_str()); // Push our code onto the stack
	rbxlua_pcall(rbxlua_State, 1, 1, 0); // Call loadstring and push return value onto stack
	rbxlua_pcall(rbxlua_State, 0, 0, 0); // Execute loadstring's return value
}




void executescript(std::string kekd) {
		lua_State* L = luaL_newstate();
		luaL_openlibs(L);
		Init(L);
		luaopen_string(L);
		luaopen_table(L);
		luaopen_debug(L);

		try {
			int err = luaL_dostring(L, kekd.c_str());
			if (err) {
				const char* errmsg = lua_tostring(L, -1);
				std::string err = "ERROR: ";
				err += errmsg;
				Print2(AuthenciateScriptBox, err.c_str());
				lua_pop(L, 1);
			}
		}
		catch (std::exception e) {
			std::string err = "C ERROR: ";
			err += e.what();
			Print2(AuthenciateScriptBox, err.c_str());
		}
		lua_close(L);
}

static int WaitForChild(lua_State* L) {
	std::string instance = lua_tostring(L, -1);
	rbxlua_getglobal(rbxlua_State, "Workspace");
	rbxlua_getfield(rbxlua_State, -1, "WaitForChild");
	rbxlua_getfield(rbxlua_State, -1, instance.c_str());
	rbxlua_pushvalue(rbxlua_State, -2);
	rbxlua_pcall(rbxlua_State, 2, 1, 0);
	return true + false;
}
static int Wait(lua_State* L) {
	int Sleep_Function = lua_tonumber(L, -1);
	Sleep(Sleep_Function);
	return 0;
}
static int ClearAllChildren2(lua_State* L) {

	rbxlua_getglobal(rbxlua_State, "Workspace");
	rbxlua_getfield(rbxlua_State, -1, "ClearAllChildren");
	rbxlua_pushvalue(rbxlua_State, -2);
	rbxlua_pcall(rbxlua_State, 2, 1, 0);
	return true + false;
}

typedef int(__thiscall * rsp)(int plr, int check);
rsp rSetParent = (rsp)GetAdr(0x421170);

static int CreateInstance(const char* className) {
	lua_State* L = luaL_newstate();
	lua_open(L);
	const char* Class = luaL_checkstring(L, 2);
	if (Class) {
		rbxlua_getglobal(rbxlua_State, "Instance");
		rbxlua_getfield(rbxlua_State, -1, "new");
		rbxlua_pushstring(rbxlua_State, Class);
	}
	else {
		if (!lua_isnil(L, -1)) {
			int Inst = lua_tonumber(L, -1);
			rSetParent(Inst, 0);
			return 0;
		}
	}
}

int RosinCreateInstance(lua_State* L) {
	int newParent = 0;
	const char* Class = luaL_checkstring(L, 1);
	if (!lua_isnil(L, 2)) {
		newParent = lua_tointeger(L, 2);
	}
	int Ins = lua_tonumber(L, -1);
	if (Ins == 0) {
		luaL_error(L, "We were unable to make that instance.");
	}
	if (Ins == -1) {
		luaL_error(L, "Uh oh! We've run into an error!");
	}
	if (newParent != 0) {
		rSetParent(Ins, newParent);
	}
	lua_pushinteger(L, Ins);
	return 1;
}

static int GetFilteringEnabled(lua_State* L) {
	Workspace = FindFirstClass(DataModel, "Workspace");
	return *(BYTE*)(Workspace + 0x1FB);
}

int InstanceNew(lua_State* L) {
	const char* erm = lua_tostring(L, -1);
	rbxlua_getglobal(rbxlua_State, "game");
	rbxlua_getfield(rbxlua_State, -1, "Workspace");
	rbxlua_getglobal(rbxlua_State, "Instance");
	rbxlua_pushstring(rbxlua_State, "new");
	rbxlua_getfield(rbxlua_State, -1, erm);
	rbxlua_getglobal(rbxlua_State, "Parent");
	rbxlua_pushvalue(rbxlua_State, -2);
	rbxlua_pcall(rbxlua_State, 2, 0, 0);
	rbxlua_pushvalue(rbxlua_State, -4);
	rbxlua_pcall(rbxlua_State, 2, 0, 0);

	return 1;
}

static int GetParentLua(lua_State* L) {
	int Instance = lua_tonumber(L, -1);
	return GetParent(Instance);
	return 0;
}

class Object
{
public:
	Object()
	{
		x = 0;
		y = 0;
	}
	Object(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	int x, y;
};


std::map<std::string, Object> objects;

static int luaGetObjects(lua_State *L)
{
	lua_newtable(L);
	int i = 0;
	for (auto it = objects.begin(); it != objects.end(); it++, i++)
	{

		lua_pushstring(L, it->first.c_str());

		luaL_getmetatable(L, "luaL_Object");
		lua_setmetatable(L, -2);

		lua_rawseti(L, -2, i + 1);
		//stackDump(L);
	}

	return 1;
}

static int luaSetPosition(lua_State* L)
{
	const char* key = luaL_checkstring(L, 1);

	int x = luaL_checknumber(L, 2);
	int y = luaL_checknumber(L, 3);

	objects[key].x = x;
	objects[key].y = y;

	return 0;
}

static int luaGetPosition(lua_State* L)
{
	const char* key = luaL_checkstring(L, 1);

	lua_pushnumber(L, objects[key].x);
	lua_pushnumber(L, objects[key].y);

	return 2;
}

void registerObject(lua_State *L)
{
	luaL_Reg regs[] =
	{
		{ "game", luaSetPosition },
		{ "GetObjects", luaGetPosition },
		{ NULL, NULL }
	};

	luaL_newmetatable(L, "luaL_Object");
	luaL_register(L, NULL, regs);
	lua_pushvalue(L, -1);
	lua_setfield(L, -1, "__index");
}

static int PrintIdentity(lua_State* l) {
	rbxlua_getfield(rbxlua_State, LUA_GLOBALSINDEX, "printidentity");
	rbxlua_pcall(rbxlua_State, 0, 0, 0);
	return 0;
}



void Init(lua_State* L) {
	lua_newtable(L);
	lua_pushcfunction(L, Index);
	lua_setfield(L, -2, "__index");

	lua_pushcfunction(L, NewIndex);
	lua_setfield(L, -2, "__newindex");

	lua_setglobal(L, "robject");
	PushInstance(L, DataModel);
	lua_setglobal(L, "game");

	PushInstance(L, DataModel);
	lua_setglobal(L, "Game");

	PushInstance(L, Workspace);
	lua_setglobal(L, "workspace");

	PushInstance(L, Workspace);
	lua_setglobal(L, "Workspace");

	lua_pushcfunction(L, GetRawTable);
	lua_setglobal(L, "getrawmetatable");

	lua_pushcfunction(L, Print);
	lua_setglobal(L, "print");

	lua_pushcfunction(L, Warn);
	lua_setglobal(L, "warn");

	lua_pushcfunction(L, Wait);
	lua_setglobal(L, "wait");



	lua_pushcfunction(L, GetService);
	lua_setglobal(L, "GetService");


	lua_pushcfunction(L, printconsole);
	lua_setglobal(L, "printconsole");

	//lua_pushcfunction(L, PrintIdentity);
	//lua_setglobal(L, "printidentity");

	lua_pushcfunction(L, WaitForChild);
	lua_setglobal(L, "WaitForChild");

	lua_pushcfunction(L, ClearAllChildren2);
	lua_setglobal(L, "ClearAllChildren");

	lua_pushcfunction(L, PrintIdentity);
	lua_setglobal(L, "printidentity");

	lua_newtable(L);
	lua_pushcfunction(L, RosinCreateInstance);
	lua_setfield(L, -2, "new");
	lua_setglobal(L, "Instance");

}




int __stdcall DllMain(HMODULE DLL, DWORD Reason, void* Useless) {
	if (Reason == 1) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Main, 0, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)context7, 0, 0, 0);
	}
	return TRUE;
}
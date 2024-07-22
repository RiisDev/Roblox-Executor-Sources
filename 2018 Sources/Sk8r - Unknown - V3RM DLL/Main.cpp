#include "globals.h"
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
#include <Psapi.h>
#include "mouseLibLua.h"
//#define DLLEXPORT __declspec(dllimport)

#pragma comment(lib, "wininet.lib")

using namespace std;
DWORD ScriptContext;
//#define log_addy_start 0x22D0B
DWORD ScriptContextVFTable = x(0x179FA80);

DWORD grabGlobalStateIndex(DWORD ScriptContext, int idx)
{
	DWORD* context = reinterpret_cast<DWORD*>(ScriptContext);
	return context[idx] + (DWORD)&context[idx];
}

using Bridge::m_rL;
using Bridge::m_L;
#define r_setidentity(rL, i) DWORD* identity  = (DWORD*)(rL - 32); *identity ^= (i ^ (unsigned __int8)*identity) & 0x1F;
#define AS_DLL "sk8r.dll"

int collectmemory(lua_State* L)
{
cout << LUA_GCCOUNT;
return 0;
}

std::string replace_all(std::string subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

void ExecuteScript(std::string Script) {
	//Bridge::CacheInstances(m_L, m_rL);
	Script = "script=Instance.new(\"LocalScript\")\n spawn(function()" + Script + "\n end)";
	if (luaL_loadstring(m_L, Script.c_str())) {
		std::string Error = lua_tostring(m_L, -1);
		printf("Error: %s\n", Error);
		r_lua_getglobal(m_rL, "warn");
		r_lua_pushstring(m_rL, Error.c_str());
		r_lua_pcall(m_rL, 1, 0, 0);
		r_lua_settop(m_rL, 0);
	}
	else {
		lua_pcall(m_L, 0, 0, 0);
	}
	Bridge::UserDataGC(m_L);
}



DWORD WINAPI input(PVOID lvpParameter)
{
	string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	LPSTR pipe = "\\\\.\\pipe\\fuckoff";
	hPipe = CreateNamedPipe(TEXT(pipe),
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

struct i_settings {
	int bufferLength = 8;
} globalSettings;

int setBuffLength(lua_State *L) {
	globalSettings.bufferLength = lua_tonumber(L, -1);
	return 1;
}

#include <windows.h>
#include <string>
#include <stdio.h>
using std::string;

#pragma comment(lib,"ws2_32.lib")

HINSTANCE hInst;
WSADATA wsaData;
void mParseUrl(char *mUrl, string &serverName, string &filepath, string &filename);
SOCKET connectToServer(char *szServerName, WORD portNum);
int getHeaderLength(char *content);
char *readUrl2(char *szUrl, long &bytesReturnedOut, char **headerOut);


int r_httpget(lua_State *L)
{
	if (!lua_isstring(L, 1))
		return 1;
	const int bufLen = 1024;
	const char *szUrl = lua_tostring(L, 1);
	long fileSize;
	char *memBuffer, *headerBuffer;
	FILE *fp;
	memBuffer = headerBuffer = NULL;
	if (WSAStartup(0x101, &wsaData) != 0)
		return -1;
	memBuffer = readUrl2((char *)szUrl, fileSize, &headerBuffer);
	lua_pushstring(L, memBuffer);
	if (fileSize != 0)
	{
		fp = fopen("downloaded.file", "wb");
		fwrite(memBuffer, 1, fileSize, fp);
		fclose(fp);
		delete(memBuffer);
		delete(headerBuffer);
	}
	WSACleanup();
	return 1;
}

void mParseUrl(char *mUrl, string &serverName, string &filepath, string &filename)
{
	string::size_type n;
	string url = mUrl;

	if (url.substr(0, 7) == "http://")
		url.erase(0, 7);

	if (url.substr(0, 8) == "https://")
		url.erase(0, 8);

	n = url.find('/');
	if (n != string::npos)
	{
		serverName = url.substr(0, n);
		filepath = url.substr(n);
		n = filepath.rfind('/');
		filename = filepath.substr(n + 1);
	}

	else
	{
		serverName = url;
		filepath = "/";
		filename = "";
	}
}

SOCKET connectToServer(char *szServerName, WORD portNum)
{
	struct hostent *hp;
	unsigned int addr;
	struct sockaddr_in server;
	SOCKET conn;

	conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (conn == INVALID_SOCKET)
		return NULL;

	if (inet_addr(szServerName) == INADDR_NONE)
	{
		hp = gethostbyname(szServerName);
	}
	else
	{
		addr = inet_addr(szServerName);
		hp = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
	}

	if (hp == NULL)
	{
		closesocket(conn);
		return NULL;
	}

	server.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(portNum);
	if (connect(conn, (struct sockaddr*)&server, sizeof(server)))
	{
		closesocket(conn);
		return NULL;
	}
	return conn;
}

int getHeaderLength(char *content)
{
	const char *srchStr1 = "\r\n\r\n", *srchStr2 = "\n\r\n\r";
	char *findPos;
	int ofset = -1;

	findPos = strstr(content, srchStr1);
	if (findPos != NULL)
	{
		ofset = findPos - content;
		ofset += strlen(srchStr1);
	}

	else
	{
		findPos = strstr(content, srchStr2);
		if (findPos != NULL)
		{
			ofset = findPos - content;
			ofset += strlen(srchStr2);
		}
	}
	return ofset;
}

char *readUrl2(char *szUrl, long &bytesReturnedOut, char **headerOut)
{
	const int bufSize = 512;
	char readBuffer[bufSize], sendBuffer[bufSize], tmpBuffer[bufSize];
	char *tmpResult = NULL, *result;
	SOCKET conn;
	string server, filepath, filename;
	long totalBytesRead, thisReadSize, headerLen;
	mParseUrl(szUrl, server, filepath, filename);
	conn = connectToServer((char*)server.c_str(), 80);
	sprintf(tmpBuffer, "GET %s HTTP/1.0", filepath.c_str());
	strcpy(sendBuffer, tmpBuffer);
	strcat(sendBuffer, "\r\n");
	sprintf(tmpBuffer, "Host: %s", server.c_str());
	strcat(sendBuffer, tmpBuffer);
	strcat(sendBuffer, "\r\n");
	strcat(sendBuffer, "\r\n");
	send(conn, sendBuffer, strlen(sendBuffer), 0);
	totalBytesRead = 0;
	while (1)
	{
		memset(readBuffer, 0, bufSize);
		thisReadSize = recv(conn, readBuffer, bufSize, 0);

		if (thisReadSize <= 0)
			break;

		tmpResult = (char*)realloc(tmpResult, thisReadSize + totalBytesRead);

		memcpy(tmpResult + totalBytesRead, readBuffer, thisReadSize);
		totalBytesRead += thisReadSize;
	}

	headerLen = getHeaderLength(tmpResult);
	long contenLen = totalBytesRead - headerLen;
	result = new char[contenLen + 1];
	memcpy(result, tmpResult + headerLen, contenLen);
	result[contenLen] = 0x0;
	char *myTmp;

	myTmp = new char[headerLen + 1];
	strncpy(myTmp, tmpResult, headerLen);
	myTmp[headerLen] = NULL;
	delete(tmpResult);
	*headerOut = myTmp;

	bytesReturnedOut = contenLen;
	closesocket(conn);
	return(result);
}

bool CompareData(const char* Data, const char* Pattern, const char* Mask) {
	while (*Mask) {
		if (*Mask != '?') {
			if (*Data != *Pattern) {
				return false;
			};
		};
		++Mask;
		++Data;
		++Pattern;
	};
	return true;
};

struct AOBType {

	string name;

	const char* first;

	const char* second;

};



/*

AOB Scanning

*/

namespace AOB {

	bool Check(const BYTE* pd, const BYTE* aob, const char* mask)
	{
		for (; *mask; ++mask, ++pd, ++aob)
			if (*mask != '?' && *pd != *aob)
				return false;
		return (*mask) == NULL;
	}



	DWORD FindPattern(const char* aob, const char* mask)
	{
		for (DWORD ind = (DWORD)GetModuleHandle(0); ind <= 0xFFFFFFF; ++ind) {
			if (Check((BYTE*)ind, (BYTE*)aob, mask))
				return ind;
		}
		return 0x00000000;
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

int getRawMetaTable(lua_State *L) {
	Bridge::push(L, m_rL, 1);

	if (r_lua_getmetatable(m_rL, -1) == 0) {
		lua_pushnil(L);
		return 0;
	}
	Bridge::push(m_rL, L, -1);
	DWORD read_only = (DWORD)r_lua_index2adr(m_rL, -1);
	*(byte*)(*(DWORD*)read_only + 8) = 0;

	return 1;
}

DWORD ScanForScriptContext(const char* SCVFT_Offsetted) {
	MEMORY_BASIC_INFORMATION BasicMemoryInformation = {};
	SYSTEM_INFO SystemInformation = {};
	GetSystemInfo(&SystemInformation);
	DWORD StartingMemorySearchPosition = (DWORD)SystemInformation.lpMinimumApplicationAddress;
	DWORD MaximumSearchBoundary = (DWORD)SystemInformation.lpMaximumApplicationAddress;
	do {
		while (VirtualQuery((void*)StartingMemorySearchPosition, &BasicMemoryInformation, sizeof(BasicMemoryInformation))) {
			if ((BasicMemoryInformation.Protect & PAGE_READWRITE) && !(BasicMemoryInformation.Protect & PAGE_GUARD)) {
				for (DWORD Key = (DWORD)(BasicMemoryInformation.BaseAddress); ((Key - (DWORD)(BasicMemoryInformation.BaseAddress) < BasicMemoryInformation.RegionSize)); ++Key) {
					if (CompareData((const char*)Key, SCVFT_Offsetted, "xxxx")) {
						return Key;
					};
				};
			};
			StartingMemorySearchPosition += BasicMemoryInformation.RegionSize;
		};
	} while (StartingMemorySearchPosition < MaximumSearchBoundary);
	return 0x0;
};

int Ryse_FetchClipboard(lua_State *L) {
	if (!OpenClipboard(nullptr))
		lua_pushstring(L, "");

	auto h = GetClipboardData(CF_TEXT);

	lua_pushstring(L, reinterpret_cast<char* >(h));
	CloseClipboard();
	return 1;
}

inline std::string getFileText(string FileName)
{
	try
	{
		ifstream _Files(FileName);
		std::string FileText;
		getline(_Files, FileText, '\a');
		return FileText;
	}
	catch (...)
	{
		return "ERROR UNABLE TO FETCH TEXT!";
	}
}

void AutoExecute(std::string FileName)
{
	string Script = getFileText(FileName);
	ExecuteScript(Script);
}

/*r_TValue* r_gettop(int r_lua_State) {
	return *(r_TValue**)(r_lua_State + 16);
}

int setrawmetatable_impl(lua_State* r_lua_State) {
	cout << "setrawmetatable_impl" << endl;
	if (r_lua_gettop(m_rL) < 2) {
		r_lua_pushboolean(m_rL, 0);
		r_lua_pushstring(m_rL, "2 or more arguments expected");
		return 2;
	}

	r_TValue* top = r_gettop(m_rL);
	r_TValue* obj1 = top - 1;
	r_TValue* obj0 = top - 2;

	if (!(obj0->tt == R_LUA_TTABLE || obj0->tt == R_LUA_TUSERDATA) || !(obj1->tt == R_LUA_TNIL || obj1->tt == R_LUA_TTABLE)) {
		r_lua_pushboolean(m_rL, 0);
		r_lua_pushstring(m_rL, "bad argument types");
		return 2;
	}

	r_lua_setmetatable(m_rL, 1);
	r_lua_pushboolean(m_rL, 1);
	return 1;
}*/

/*int WINAPI h_MessageBox(
	_In_opt_ HWND    hWnd,
	_In_opt_ LPCTSTR lpText,
	_In_opt_ LPCTSTR lpCaption,
	_In_     UINT    uType
) {
	std::string str_lp = lpText;
	std::string str_cap = lpCaption;
	if (str_cap == "Roblox Crash") {
		DWORD log_start = log_addy_start + (DWORD)GetModuleHandleA(NULL);
		DWORD old;
		for (int i = 0; i < 79; i++) {
			VirtualProtect((LPVOID)(log_start + i), 1, PAGE_EXECUTE_READWRITE, &old);
			*(char*)(log_start + i) = 0x90;
			VirtualProtect((LPVOID)(log_start + i), 1, old, &old);
		}
		str_lp = "[SK8R]: I just protected you from ban, you got crashed by ROBLOX... phew";
	}

	wchar_t* str_lp_res = new wchar_t[4096];
	wchar_t* str_cap_res = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, NULL, str_lp.c_str(), -1, str_lp_res, 4096);
	MultiByteToWideChar(CP_ACP, NULL, str_cap.c_str(), -1, str_cap_res, 4096);
	return MessageBoxW(hWnd, str_lp_res, str_cap_res, uType);
}


void InitUploadCheckHook() {
	DWORD o;
	//*(DWORD*)x(0x1B40D9C) = 6;
	VirtualProtect((LPVOID)&MessageBoxA, 1, PAGE_EXECUTE_READWRITE, &o);
	*(char*)(&MessageBoxA) = 0xE9;
	*(DWORD*)((DWORD)&MessageBoxA + 1) = ((DWORD)&h_MessageBox - (DWORD)&MessageBoxA) - 5;
	VirtualProtect((LPVOID)&MessageBoxA, 1, o, &o);
}*/

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

int getgenv(lua_State *Ls) {
	lua_pushvalue(m_L, LUA_GLOBALSINDEX);
	Bridge::push(m_rL, Ls, -1);
	return 1;
}


int getreg(lua_State *Ls) {
	lua_pushvalue(m_L, LUA_REGISTRYINDEX);
	Bridge::push(m_rL, Ls, -1);
	return 1;
}

int custom_getrenv(lua_State* L) {
	Bridge::push(m_rL, L, -10002);
	return 1;
}

int getfenv(lua_State *L) {
	lua_pushvalue(m_L, LUA_REGISTRYINDEX);
	Bridge::push(m_rL, L, -1);
	return 1;
}

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
void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory, unsigned char includePath) // thx stackoverflow
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

 int custom_writefile(lua_State *rbx_L) {
	const char* name = r_lua_tolstring(m_rL, -1, 0);
	const char* stuff = r_lua_tolstring(m_rL, -2, 0);

	char path[MAX_PATH];
	GetFile(AS_DLL, "", path, MAX_PATH);

	std::string file;
	file += path;
	file += "Workspace//";
	file += stuff;

	std::ofstream createfile(file.c_str());

	createfile << name;

	return 0; //gay help o sht
}


int custom_readfile(lua_State* r_lua_State) {
	const char * nameoffile = lua_tostring(r_lua_State, -1);

	char path[MAX_PATH];
	GetFile(AS_DLL, "", path, MAX_PATH);

	std::string file;
	file += path;
	file += "Workspace\\";
	file += nameoffile;

	printf(file.c_str());

	string line;
	ifstream myfile(file);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			lua_pushstring(r_lua_State, line.c_str());
		}
		myfile.close();
	}

	return 1;
}

	static int custom_runfile(lua_State *lua_State) {
		const char * name = r_lua_tolstring(m_rL, -1, 0);

		char path[MAX_PATH];
		GetFile(AS_DLL, "", path, MAX_PATH);

		std::string file;
		file += path;
		//file += "Workspace//";
		file += name;

		std::ifstream swfile(file);
		std::string line;

		if (swfile.is_open())
		{
			while (getline(swfile, line))
			{
				ExecuteScript(line);
			}
			swfile.close();
		}

		return 0;
	}

LPCTSTR WindowName = "Skater";
HWND Find = FindWindow(NULL, WindowName);


namespace Memory {

	void write(void* address, void* instructions, size_t size) {
		DWORD tbuf;
		VirtualProtect(address, 1, PAGE_EXECUTE_READWRITE, &tbuf);
		memcpy(address, instructions, size);
		VirtualProtect(address, 1, tbuf, &tbuf);
	}

	bool compare(BYTE* address, BYTE* pattern, BYTE* mask) {
		for (; *mask; address++, pattern++, mask++) {
			if (*mask == 'x' && *address != *pattern) {
				return false;
			}
		}
		return true;
	}

	DWORD scan(BYTE* aob, BYTE* mask, BYTE prot = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) {
		MEMORY_BASIC_INFORMATION mbi;
		DWORD j = (DWORD)GetModuleHandle(NULL);
		while (j < 0x7FFFFFFF && VirtualQuery((void*)j, &mbi, sizeof(mbi))) {
			if (!(mbi.Protect & PAGE_GUARD) && (mbi.State & MEM_COMMIT) && (mbi.Protect & prot)) {
				for (DWORD k = (DWORD)mbi.BaseAddress; k < ((DWORD)mbi.BaseAddress + mbi.RegionSize); ++k) {
					if (compare((BYTE*)k, (BYTE*)aob, (BYTE*)mask)) {
						return k;
					}
				}
			}
			j += mbi.RegionSize;
		}
		return 0;
	}
}


int custom_loadstring(lua_State* L) {
	if (lua_type(L, -1) != LUA_TSTRING) {
		luaL_error(L, "(Bad Argument) - 'loadstring' only accepts 1 argument (string).");
		return 0;
	}
	if (lua_gettop(L) == 0) {
		luaL_error(L, "'loadstring' needs 1 string argument.");
		return 0;
	}
	ExecuteScript(std::string(lua_tostring(L, -1)));
	return 0;
}

static int Panic(lua_State *Thread) {
	luaL_error(Thread, "PANIC: unprotected error in call to Lua API (%s)\n", lua_tostring(Thread, -1));
	throw;
	return 0;
}

int Dbg_Setmetatable(lua_State *L) {
	lua_pushboolean(L, lua_setmetatable(L, -1));
	return 1;
}


void main()
{
	//main2();
	/*CONSOLEBYPASS();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	SetWindowPos(ConsoleHandle, HWND_TOPMOST, 50, 20, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	ShowWindow(ConsoleHandle, 1);
	SetConsoleTitle("Sk8r Console.");*/
	ScriptContext = Memory::scan((BYTE*)&ScriptContextVFTable, (BYTE*)"xxxx", PAGE_READWRITE);
	//ScriptContext = ScanForScriptContext((char*)&ScriptContextVFTable);
	int v51 = 0;
	DWORD v39 = ScriptContext;
	m_rL = v39 + 56 * v51 + 164 ^ *(DWORD *)(v39 + 56 * v51 + 164);
	m_L = luaL_newstate();
	mouse_funcs::setup(m_L);
	keyboard_funcs::setup(m_L);
	r_setidentity(m_rL, 6);
	OSVERSIONINFO   vi;
	memset(&vi, 0, sizeof vi);
	vi.dwOSVersionInfoSize = sizeof vi;
	GetVersionEx(&vi);
	if (vi.dwMajorVersion >= 10) {
		VEH_Bypass();
	}
	Bridge::VehHandlerpush();
	luaL_openlibs(m_L);
	luaL_newmetatable(m_L, "garbagecollector"); 
	lua_pushcfunction(m_L, Bridge::UserDataGC);
	lua_setfield(m_L, -2, "__gc");

	lua_newtable(m_L);
	lua_pushcfunction(m_L, Bridge::GlobalsIndex);
	lua_setfield(m_L, -2, "__index");
	lua_pushstring(m_L, "The metatable is locked");
	lua_setfield(m_L, -2, "__metatable");
	lua_setmetatable(m_L, LUA_GLOBALSINDEX);

	Bridge::SetUpEnvironment(m_L);
	lua_register(m_L, "getrawmetatable", getRawMetaTable);
	lua_register(m_L, "loadstring", custom_loadstring);
	lua_register(m_L, "getgenv", getgenv);
	lua_register(m_L, "getreg", getreg);
	lua_register(m_L, "getfenv", getfenv);
	lua_register(m_L, "getrenv", custom_getrenv);
	lua_register(m_L, "sk8rget", r_httpget);
	//lua_register(m_L, "writefile", custom_writefile);
	//lua_register(m_L, "readfile", custom_readfile);
	//lua_register(m_L, "runfile", custom_runfile);
	//lua_register(m_L, "debug.setmetatable", Dbg_Setmetatable);
	lua_newtable(m_L);
	lua_setglobal(m_L, "_G");
	printf("ok");
	if (Find) {
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)input, NULL, NULL, NULL);
		MessageBoxA(NULL, "test", "XD", MB_OK);
		luaL_dostring(m_L, "function GetObjects(assetId) local obj = game:GetService(\"InsertService\"):LoadLocalAsset(assetId) return { obj }; end");
		luaL_dostring(m_L, "os.execute = nil os.remove = nil os.rename = nil");
		luaL_dostring(m_L, "game.StarterGui:SetCore('SendNotification', {Duration = 10; Title='Sk8r'; Text='Successfully Loaded!'})");
		//AutoExecute("AutoExecute.txt");
			//luaL_dostring(m_L, "game.ReplicatedStorage.DefaultChatSystemChatEvents.SayMessageRequest:FireServer('/me Skater loaded, visit club dark dot net!', 'All')");
	}
	else {
		MessageBoxA(NULL, "Support Original", "Do not attempt to crack sk8r!", MB_OK);
		system("start https://clubdark.net");
		Sleep(3000);
		ExitProcess(0);
	}
	}

unsigned int ProtectSections(HMODULE Module
) {

	/*

	This function is awfully similar to ROBLOX's protectVmpSections function.
	Because it's the same. This function protect's our sections.
	*/
	MODULEINFO ModuleInfo;
	GetModuleInformation(GetCurrentProcess(), Module, &ModuleInfo, sizeof(ModuleInfo));
	uintptr_t Address = reinterpret_cast<uintptr_t>(Module);
	uintptr_t TermAddress = Address + ModuleInfo.SizeOfImage;
	MEMORY_BASIC_INFORMATION MemoryInfo;


	while (Address < TermAddress) {
		VirtualQuery(reinterpret_cast<void*>(Address), &MemoryInfo, sizeof(MemoryInfo));
		if (MemoryInfo.State == MEM_COMMIT && (MemoryInfo.Protect & (PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))) {
			DWORD OldProtection;
			VirtualProtect(reinterpret_cast<void*>(Address), MemoryInfo.RegionSize, PAGE_EXECUTE_READ, &OldProtection);
		}
		Address = reinterpret_cast<uintptr_t>(MemoryInfo.BaseAddress) + MemoryInfo.RegionSize;
	}

	VirtualQuery(reinterpret_cast<void*>(MemoryInfo.AllocationBase), &MemoryInfo, sizeof(MemoryInfo));
	if (MemoryInfo.State != MEM_COMMIT || !(MemoryInfo.Protect & PAGE_EXECUTE_READ))
		return 0x400;
	return MemoryInfo.RegionSize - 0x400;
}
__forceinline void UnlinkModule(HINSTANCE Module
) {
	unsigned long PEB_DATA = 0;
	_asm {
		pushad;
		pushfd;
		mov eax, fs:[30h]
			mov eax, [eax + 0Ch]
			mov PEB_DATA, eax

			InLoadOrderModuleList :
		mov esi, [eax + 0Ch]
			mov edx, [eax + 10h]

			LoopInLoadOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 18h]
			cmp ecx, Module
			jne SkipA
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InMemoryOrderModuleList

			SkipA :
		cmp edx, esi
			jne LoopInLoadOrderModuleList

			InMemoryOrderModuleList :
		mov eax, PEB_DATA
			mov esi, [eax + 14h]
			mov edx, [eax + 18h]

			LoopInMemoryOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 10h]
			cmp ecx, Module
			jne SkipB
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InInitializationOrderModuleList

			SkipB :
		cmp edx, esi
			jne LoopInMemoryOrderModuleList

			InInitializationOrderModuleList :
		mov eax, PEB_DATA
			mov esi, [eax + 1Ch]
			mov edx, [eax + 20h]

			LoopInInitializationOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 08h]
			cmp ecx, Module
			jne SkipC
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp Finished

			SkipC :
		cmp edx, esi
			jne LoopInInitializationOrderModuleList

			Finished :
		popfd;
		popad;
	}
}


BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		char strDLLName[_MAX_PATH];
		GetModuleFileName(Module, strDLLName, _MAX_PATH);
		if (strstr(strDLLName, "sk8r.dll") <= 0)
		{
			Sleep(1500);
			MessageBoxA(NULL, "Support Original", "Do not attempt to crack sk8r!", MB_OK);
			system("start https://clubdark.net");
			Sleep(1500);
			ExitProcess(0);
		}
		else {
			DisableThreadLibraryCalls(Module);
			UnlinkModule(Module);
			DWORD OldProtection;
			VirtualProtect(Module, 4096, PAGE_READWRITE, &OldProtection);
			ZeroMemory(Module, 4096);
			ProtectSections(Module);
			HANDLE hThread = NULL;
			HANDLE hDllMainThread = OpenThread(THREAD_ALL_ACCESS, NULL, GetCurrentThreadId());
			if (Reserved == NULL) {
				if (!(hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL))) {
					CloseHandle(hDllMainThread);
					return FALSE;
				}
				CloseHandle(hThread);
			}
		}
		return TRUE;
	}
}

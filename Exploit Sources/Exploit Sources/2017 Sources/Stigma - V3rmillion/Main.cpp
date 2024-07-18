#include "Main.h"
#include "luaState.h"
//#include "VMProtectSDK.h"
#include "form.h"
#include <tchar.h>
#include <Richedit.h>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <CommCtrl.h>
#include <sstream>
#include "sigscan.h"
#include "aobscan.h"
#include "SciLexer.h"
#include "Authentication.h"
#include <queue>
#include "Update.h"
#include "LuaSerializer.h"

static rlua::Form* thisBas;
static rlua::Form* form;
static rlua::Form* thisBasS;

namespace tStringLua {
	lua_State* L;
}

void GetFile(const char* dllName, const char* fileName, char* buffer, int bfSize);
int GetParent(int object);
bool RunCommand(std::string src);
void runLua(std::string source);
int DownloadString(int r_lua_State);
void Chathook(const char* plr);
int luawritefile(int r_lua_State);
int luareadfile(int r_lua_State);
int loadfile(int r_lua_State);
int sumonexpath(int r_lua_State);
void errorlua(std::string msg);

int DataModel = GetParent(lua::ScriptContext);

#define SUMONEX_VERSION "0.0.0"

#define addfunction(s, f)			rblua_pushcclosure((int)globalState, f, 0); \
									rblua_setfield((int)globalState, LUA_GLOBALSINDEX, s);

#define console_error(msg) { rlua::Form::GetBaseInst()->RawOutputText(msg, RGB(255, 0, 0)); }
#define console_output(msg) { rlua::Form::GetBaseInst()->RawOutputText(msg, RGB(0, 235, 0)); }

#define ADDMENUITEM(hmenu, ID, string) \
 AppendMenu(hSubMenu, MF_STRING, ID, string);

#define Commands (WM_APP + 601)
#define EasyUseA (WM_APP + 602)
#define StatChangerA (WM_APP + 603)
#define openbtn (WM_APP + 604)
#define execute (WM_APP + 605)
#define clear (WM_APP + 606)
#define execcmd (WM_APP + 701)
#define SumonexGui (WM_APP + 702)
#define winApi1 (WM_APP + 705)
#define winApi2 (WM_APP + 706)

#define EUI_FILEVIEW		(WM_APP + 607)
#define EUI_LV_RCM_EXEC		(WM_APP + 700)

#define lua_error(msg) { errorlua(msg); }

#define CONSOLE

#define ADDPOPUPMENU(hmenu, string) \
 HMENU hSubMenu = CreatePopupMenu(); \
 AppendMenu(hmenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, string);




#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


TaskScheduler *scheduler;

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

void errorlua(std::string msg) {
	runLua("error'" + msg + "'");
}

/* Custom Functions */

int MouseButton1Down() {
	callcheck()
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int MouseButton2Down() {
	callcheck()
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int MouseButton1Up() {
	callcheck()
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int MouseButton2Up() {
	callcheck()
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int ConsoleOutput(int r_lua_State) {
	callcheck()
		std::string str = rblua_tostring(r_lua_State, -1);
	str += "\r\n";

	console_output(str.c_str());
	return 0;
}

int LuaConsoleError(int r_lua_State) {
	callcheck();
	std::string str = rblua_tostring(r_lua_State, -1);
	str += "\r\n";

	console_error(str.c_str());
	return 0;
}

std::string prefix = ";";

TValue* r_getobjat(int r_lua_State, int off) {
	TValue* top = *(TValue**)(r_lua_State + 16);
	TValue* base = *(TValue**)(r_lua_State + 28);

	if (off > rblua_gettop(r_lua_State)) {
		return 0;
	}

	return base + (off - 1);
}

#define RLUA_TSTRING		4

int loadstringredone(int r_lua_State) {
	callcheck()
		TValue* codeval = r_getobjat(r_lua_State, 1);

	if (codeval->tt == RLUA_TSTRING) {
		TString* tsv = *(TString**)(codeval);
		const char* code = (const char*)(tsv + 1);

		if (luaL_loadstring(tStringLua::L, code)) {
			rblua_pushnil(r_lua_State);
			rblua_pushstring(r_lua_State, lua_tostring(tStringLua::L, -1));
			lua_pop(tStringLua::L, 1);
			return 2;
		}
		return 1;
	}
	return 2;
}

int loadstring(int r_lua_State) {
	callcheck()
		const char* code = rblua_tostring(r_lua_State, 1);
	runLua(code);
	return 1;
}

int CopyString(int r_lua_State) {
	callcheck()
		const char* str = rblua_tostring(r_lua_State, -1);

	int len = strlen(str);
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len + 1);
	void *mem_ptr = GlobalLock(hMem);
	memcpy(mem_ptr, str, len + 1);
	GlobalUnlock(hMem);
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
	return 0;
}

int getrawmetatable(int r_lua_State) {
	callcheck()
		//VMProtectBeginMutation("lua_State = faule");
	rblua_getmetatable(r_lua_State, -1);
	DWORD t = rblua_indx2adr(r_lua_State, -1);
	*(BYTE*)(*(DWORD*)t + 6) = 0;
	//VMProtectEnd();
	return 1;
}

int toreadonly(int r_lua_State) {
	callcheck()
		lua_error("error");
	return 1;
}

int setreadonly(int r_lua_State) {
	callcheck()
		DWORD t = rblua_indx2adr(r_lua_State, -1);
	*(BYTE*)(*(DWORD*)t + 6) = 0;
	return 1;
}

int luawritefile(int r_lua_State) {
	callcheck()
		const char * nameoffile = rblua_tostring(r_lua_State, -1);
	const char * contentsoffile = rblua_tostring(r_lua_State, -2);

	char path[MAX_PATH];
	GetFile("summonex.dll", "", path, MAX_PATH);

	std::string lyonisanoob;
	lyonisanoob += path;
	lyonisanoob += "scripts\\";
	lyonisanoob += contentsoffile;

	std::ofstream o(lyonisanoob.c_str());

	o << nameoffile;
	return 1;
}

int sumonexpath(int r_lua_State) {
	callcheck()
		const char * nameoffile = rblua_tostring(r_lua_State, -1);

	char path[MAX_PATH];
	GetFile("summonex.dll", "", path, MAX_PATH);

	rblua_pushstring(r_lua_State, path);
	return 1;
}

int luareadfile(int r_lua_State) {
	callcheck()
		const char * nameoffile = rblua_tostring(r_lua_State, -1);

	char path[MAX_PATH];
	GetFile("summonex.dll", "", path, MAX_PATH);

	std::string lyonisameganoob;
	lyonisameganoob += path;
	lyonisameganoob += "scripts\\";
	lyonisameganoob += nameoffile;

	printf(lyonisameganoob.c_str());

	string line;
	ifstream myfile(lyonisameganoob);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			rblua_pushstring(r_lua_State, line.c_str());
		}
		myfile.close();
	}

	return 1;
}

int loadfile(int r_lua_State) {
	callcheck()
		const char * nameoffile = rblua_tostring(r_lua_State, -1);

	char path[MAX_PATH];
	GetFile("summonex.dll", "", path, MAX_PATH);

	std::string lyonisameganoob;
	lyonisameganoob += path;
	lyonisameganoob += "scripts\\";
	lyonisameganoob += nameoffile;

	printf(lyonisameganoob.c_str());

	string line;
	ifstream myfile(lyonisameganoob);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			runLua(line.c_str());
		}
		myfile.close();
	}

	return 1;
}

int DownloadString(int r_lua_State) {
	callcheck()
	const char* str = rblua_tostring(r_lua_State, 1);

	string Response = Auth::DownloadURL(str);

	runLua(Response.c_str());
	return 0;
}

//end

typedef struct ThreadRef {
	int state;
	std::string code;
	std::string source;
};

std::queue<ThreadRef>* queuesdwad;


void RunCode(int rstate, char *code) {
	LuaSerializer s;
	RbxOpEncoder e;
	printf("Test_Args.\n");
	int test_args[] = { (int)rstate };
	printf("Thread.\n");
	int thread = (int)CallFunc(newthread, 1, (int*)test_args, (int*)rets);
	printf("Key\n");
	int key = deobf_ptr((int)deobf_ptr((int)(thread + 8)) + 28);
	printf("Src\n");
	std::string src(code);
	printf("Serialize\n");
	std::string res = s.serialize(tStringLua::L, src, e, key);
	printf("Deserialize.\n");
	if (res == "error") {
		printf(res.c_str());
	}
	else {
		deserialize(thread, res, "Sumonex", 1);
	}
	printf("Restore HFlag\n");
	RESTORE_HACKFLAG();
	printf("Set Identity.\n");
	SET_IDENTITY(thread, 7);
	printf("Resume Args.\n");
	int resume_args[] = { 0, (int)thread };
	printf("Call Function.\n");
	CallFunc(resume, 2, (int*)resume_args, (int*)rets);
}

void runLua(std::string source) {
	//VMProtectBeginMutation("die");
	printf("Lua Open\n");
	tStringLua::L = lua_open();
	printf("Scheduler\n");
	scheduler = new TaskScheduler(tStringLua::L);
	SigScanner ss;
	int *SC = (int*)-1;
	printf("sc_vtable\n");
	int sc_vtable = (int)GetModuleHandle(0) + ScriptContextWAddress;
	do {
		printf("SigScan\n");
		SC = (int*)ss.ScanNoAlignW((char*)&sc_vtable, 4);
	} while ((int)SC == -1);
	printf("Locate Roblox Defs\n");
	LocateRbxDefs();
	__asm {
		push eax
		push ecx
		push 2
		mov ecx, SC
		call ggs
		mov globalState, eax
		pop ecx
		pop eax
	}

	printf("Add Functions.\n");

	addfunction("mousebutton1down", MouseButton1Down);
	addfunction("mousebutton1up", MouseButton1Up);
	addfunction("mousebutton2down", MouseButton2Down);
	addfunction("mousebutton2up", MouseButton2Up);
	addfunction("downloadstring", DownloadString);
	addfunction("copystr", CopyString);
	addfunction("console_output", ConsoleOutput);
	addfunction("console_error", LuaConsoleError);
	addfunction("loadstringa", loadstringredone);
	addfunction("loadstring", loadstring);
	addfunction("getrawmetatable", getrawmetatable);
	addfunction("writefile", luawritefile);
	addfunction("readfile", luareadfile);
	addfunction("loadfile", loadfile);
	addfunction("sumonexpath", sumonexpath);

	printf("Buff\n");

	std::string buff;
	buff = "spawn(function() local script = Instance.new('LocalScript', nil) script.Name = 'Sumonex' script.Disabled = true\r\n";
	buff += source.c_str();
	buff += "\r\nend)";

	printf("Run Code\n");

	RunCode((int)globalState, (char*)buff.c_str());

	printf("Close\n");

	lua_close(tStringLua::L);
	//VMProtectEnd();
}

/* Auto Exec */

CRITICAL_SECTION cs;

extern string autoexecpath;
string autoexecpath;

#include "Shlwapi.h"
#include <Shlwapi.h>

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

/*void luaA_runautoexec() {
	char cPath[MAX_PATH];
	GetFile("sumonex_rewrite.dll", "\\autoexecute\\", cPath, MAX_PATH);

	std::string aePath = cPath;
	std::string iPath = aePath + "init.lua";

	if (!PathFileExists(iPath.c_str()))
		form->print(RGB(255, 0, 0), "ERROR: Cannot find \"init.lua\"\r\n");


	std::string initbuffer;
	if (!ReadFile(iPath, initbuffer, 0))
		return;

	runLua(initbuffer.c_str());

	std::vector<std::string> files;
	GetFilesInDirectory(files, aePath, 0);

	for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); it++) {
		if (*it == "init.lua") continue;

		std::string fpath = aePath + *it;
		std::string buffer;
		if (ReadFile(fpath, buffer, 0)) {
			runLua(buffer.c_str());
			form->print(RGB(0, 235, 0), "Executed autoexec file '%s'\r\n", (*it).c_str());
		}
		else {
			form->print(RGB(255, 0, 0), "ERROR: Cannot execute autoexec file '%s'\r\n", (*it).c_str());
		}
	}
}*/

void loadautoexec() {
	WIN32_FIND_DATA fd;

	HANDLE hFile = FindFirstFile(autoexecpath.c_str(), &fd);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	do {
		string path = autoexecpath;
		path.resize(path.find_last_of('\\') + 1);
		path += fd.cFileName;
		if (_stricmp(fd.cFileName + strlen(fd.cFileName) - 4, ".lua") != 0)
			continue;

		std::ifstream file;
		file.open(path.c_str(), std::ifstream::binary);
		if (!file.is_open()) {
			rlua::Form::GetBaseInst()->RawOutputText("Error opening an autoexecute file\r\n", RGB(255, 0, 0));
			continue;
		}

		file.seekg(0, file.end);
		int size = (int)file.tellg();
		file.seekg(0, file.beg);
		char *filestr = new char[size + 1]();
		file.read(filestr, size);
		file.seekg(0, file.beg);
		file.close();

		runLua(filestr);

	} while (FindNextFile(hFile, &fd));
}

__declspec(naked) void FreeConsoleHook(void) {
	__asm ret
}

using namespace std;

// Add a menu item



int Ind = 0;
CHARFORMAT CharFormat;
#pragma warning(disable : 4996)

std::vector<string> str_split(string str, char delimiter)
{
	std::vector<string> tokens;
	std::stringstream ss(str);
	string tok;

	while (getline(ss, tok, delimiter))
		tokens.push_back(tok);

	return tokens;
}

HMODULE hDll;

bool AlreadyRan = false;

#pragma warning(disable : 4996)
void ConsoleBypass()
{
	DWORD nothing;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &nothing);
	*(BYTE*)(&FreeConsole) = 0xC3;
}

void Console(char* title) {
	AllocConsole();
	SetConsoleTitleA(title);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 900, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

	::ShowWindow(ConsoleHandle, SW_NORMAL);
}

void InitiateAuthenticationAndConsole() {
	ConsoleBypass();
	AllocConsole();
	Console("Test.");
	cout << "Authenticating. . .";
	Authentication();
}


void testInit() {
	ConsoleBypass();
	AllocConsole();
	SetConsoleTitleA("Sumonex Loader");
	freopen("CONOUT$", "w", stdout);
	printf("Ready!\r\n");

	//std::string mystr;
	//getline(cin, mystr);
}

void rlua::ColorFormat(std::string Text, RGB_Struct* StartColor) {
	int Find = 0;
	std::string Buffer = Text;
	std::string Buffer2 = Text;
	if (Text.find("[") != std::string::npos) {
		Buffer = Text.substr(0, Text.find("["));
		Text = Text.substr(Text.find("[") + 1);
		if (Text.find("]") != std::string::npos) {
			std::string TagInput = Text.substr(0, Text.find("]"));
			Buffer2 = Text.substr(Text.find("]") + 1);
			if (TagInput.substr(0, 1) == "#") {
				int r, g, b;
				TagInput.erase(0, 1);
				std::istringstream(TagInput.substr(0, 2)) >> std::hex >> r;
				std::istringstream(TagInput.substr(2, 2)) >> std::hex >> g;
				std::istringstream(TagInput.substr(4, 2)) >> std::hex >> b;
				if (StartColor != nullptr) thisBas->RawOutputText(Buffer, RGB(StartColor->R, StartColor->G, StartColor->B));
				else thisBas->RawOutputText(Buffer);
				ColorFormat(Buffer2, new RGB_Struct{ r, g, b });
				return;
			}
			else if (TagInput.substr(0, 1) == "/") {
				if (StartColor != nullptr) thisBas->RawOutputText(Buffer, RGB(StartColor->R, StartColor->G, StartColor->B));
				else thisBas->RawOutputText(Buffer);
				ColorFormat(Buffer2);
				return;
			}
			else {
				Buffer += "[" + TagInput + "]";
				if (StartColor != nullptr) thisBas->RawOutputText(Buffer, RGB(StartColor->R, StartColor->G, StartColor->B));
				else thisBas->RawOutputText(Buffer);
				ColorFormat(Buffer2);
				return;
			}
		}
	}
	if (StartColor != nullptr) thisBas->RawOutputText(Buffer, RGB(StartColor->R, StartColor->G, StartColor->B));
	else thisBas->RawOutputText(Buffer);
	return;
}

HWND RobloxForm = NULL;
HWND MainForm = NULL;
std::string IntToHex(int Val) {
	std::stringstream stream;
	stream << std::hex << Val;
	return stream.str();
}

HWND							_window = 0;

void FormLoop() {
	while (true) {
		if (RobloxForm == NULL || MainForm == NULL) break;
		if (IsIconic(MainForm) == 0 && GetForegroundWindow() == RobloxForm) {
			SetWindowPos(MainForm, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		else {
			SetWindowPos(MainForm, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		Sleep(1);
	}
}

#define mainTextBox (WM_APP + 380)

namespace rlua {
	Form::Form(const char* szWindowClass, const char* Title, OnLoad Func) {
		HINSTANCE hInstance = GetModuleHandle(NULL);
		HINSTANCE RichTextModule = LoadLibrary("riched32.dll");
		if (RichTextModule == 0) {
			MessageBox(NULL, TEXT("ERROR: Cannot loading 1 or more DLL's"), TEXT("ERROR"), NULL);
			std::abort();
			return;
		}

		thisBas = this;
		form = this;
		this->WindowClass = szWindowClass;

		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = &WindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = CreateSolidBrush(RGB(60, 60, 60));
		wcex.lpszClassName = szWindowClass;
		wcex.lpszMenuName = "rlua";
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

		if (!RegisterClassEx(&wcex)) {
			UnregisterClass(szWindowClass, hInstance);
			RegisterClassEx(&wcex);
			return;
		}

		this->hWindow = CreateWindowEx(WS_EX_TOPMOST, szWindowClass, Title, WS_OVERLAPPED | WS_CAPTION | WS_EX_TOPMOST | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 616, 431, 0, 0, hInstance, 0);
		if (!this->hWindow) {
			MessageBox(NULL, "ERROR: Cannot load window", "ERROR", NULL);
			return;
		}

		ShowWindow(this->hWindow, SW_SHOWDEFAULT);
		UpdateWindow(this->hWindow);
		if (Func != NULL) {
			std::thread t(*Func, this);
			t.detach();
		}
		MSG Message;
		while (GetMessage(&Message, NULL, 0, 0)) {
			if (BreakWindow) break;
			if (Message.message == WM_KEYDOWN && Message.wParam == VK_RETURN && GetFocus() == this->hWndEdit) {
				char buffer[1024];

				GetWindowText(this->hWndEdit, buffer, 1024);
				std::string str(buffer);
				//rlua::Form::GetBaseInst()->RawOutputTexts("\r\n");
				continue;
			}

			else if (Message.message == WM_KEYDOWN && Message.wParam == VK_RETURN && GetFocus() == this->hWndEdit) {
				SendMessage(this->hWndEdit, EM_SETSEL, 0, -1);
			}
			SendMessage(this->hWndList, EM_HIDESELECTION, 1, 0);

			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		if (!BreakWindow) {
			DestroyWindow(this->hWindow);
			BreakWindow = true;
			exit(0);
		}
	}

	Form::~Form() {
		if (!BreakWindow) {
			DestroyWindow(this->hWindow);
			BreakWindow = true;
		}
	}

	Form* Form::GetBaseInst() {
		return thisBas;
	}

	void Form::OutputText(std::string Text, COLORREF color) {
		Text += "\n";
		memset(&CharFormat, 0, sizeof(CharFormat));
		CharFormat.cbSize = sizeof(CharFormat);
		CharFormat.dwMask = CFM_COLOR;
		if (color != NULL) CharFormat.crTextColor = color;
		SendMessage(this->hWndList, EM_SETCHARFORMAT, (LPARAM)SCF_SELECTION, (LPARAM)&CharFormat);
		CHARRANGE CharRange;
		CharRange.cpMin = -1, CharRange.cpMax = -1;
		SendMessage(this->hWndList, EM_EXSETSEL, 0, (LPARAM)&CharRange);
		SendMessage(this->hWndList, EM_REPLACESEL, FALSE, (LPARAM)Text.c_str());
		SendMessage(this->hWndList, WM_VSCROLL, SB_BOTTOM, (LPARAM)0);
		Ind++;

	}

	void Form::CloseWindow() {
		DestroyWindow(this->hWindow);
		BreakWindow = true;
	}

#define		CONSOLE_MESSAGE_LIMIT	255

	void Form::RawOutputText(std::string Text, COLORREF color) {
		memset(&CharFormat, 0, sizeof(CharFormat));
		CharFormat.cbSize = sizeof(CharFormat);
		CharFormat.dwMask = CFM_COLOR;
		if (color != NULL) CharFormat.crTextColor = color;
		SendMessage(this->hWndList, EM_SETCHARFORMAT, (LPARAM)SCF_SELECTION, (LPARAM)&CharFormat);
		CHARRANGE CharRange;
		CharRange.cpMin = -1, CharRange.cpMax = -1;
		SendMessage(this->hWndList, EM_EXSETSEL, 0, (LPARAM)&CharRange);
		SendMessage(this->hWndList, EM_REPLACESEL, FALSE, (LPARAM)Text.c_str());
		SendMessage(this->hWndList, WM_VSCROLL, SB_BOTTOM, (LPARAM)0);
		Ind++;

	}

	void Form::print(COLORREF col, const char * format, ...) {
		char message[CONSOLE_MESSAGE_LIMIT];
		memset(message, 0, sizeof(message));
		va_list vl;
		va_start(vl, format);
		vsnprintf_s(message, CONSOLE_MESSAGE_LIMIT, format, vl);
		va_end(vl);

		int len = SendMessage(hWndList, WM_GETTEXTLENGTH, NULL, NULL);
		SendMessage(hWndList, EM_SETSEL, len, len);

		CHARFORMAT cfd; // default
		CHARFORMAT cf;
		SendMessage(hWndList, EM_GETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cfd);
		memcpy(&cf, &cfd, sizeof(CHARFORMAT));

		cf.cbSize = sizeof(CHARFORMAT);
		cf.dwMask = CFM_COLOR; // change color
		cf.crTextColor = col;
		cf.dwEffects = 0;

		SendMessage(hWndList, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
		SendMessage(hWndList, EM_REPLACESEL, FALSE, (LPARAM)message);
		SendMessage(hWndList, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cfd);
	}

	void Form::RawOutputTexts(std::string Text, COLORREF color) {
		memset(&CharFormat, 0, sizeof(CharFormat));
		CharFormat.cbSize = sizeof(CharFormat);
		CharFormat.dwMask = CFM_COLOR;
		if (color != NULL) CharFormat.crTextColor = color;
		SendMessage(this->hWndEdit, EM_SETCHARFORMAT, (LPARAM)SCF_SELECTION, (LPARAM)&CharFormat);
		CHARRANGE CharRange;
		CharRange.cpMin = -1, CharRange.cpMax = -1;
		SendMessage(this->hWndEdit, EM_EXSETSEL, 0, (LPARAM)&CharRange);
		SendMessage(this->hWndEdit, EM_REPLACESEL, FALSE, (LPARAM)Text.c_str());
		SendMessage(this->hWndEdit, WM_VSCROLL, SB_BOTTOM, (LPARAM)0);
		Ind++;

	}

	void Form::ClearOutput() {
		SETTEXTEX fat;
		fat.codepage = CP_ACP;
		fat.flags = ST_KEEPUNDO;
		SendMessage(this->hWndList, EM_SETTEXTEX, (WPARAM)&fat, 0);
	}



	void Form::SetupFonts() {
		HFONT hFont = CreateFontA(15, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Courier New"));
		HFONT hFontBtn = CreateFontA(15, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));
		SendMessage(thisBas->hWndEdit, WM_SETFONT, (WPARAM)hFontBtn, 0);
		SendMessage(thisBas->hWndEdit1, WM_SETFONT, (WPARAM)hFontBtn, 0);
		SendMessage(thisBas->hWndList, WM_SETFONT, (WPARAM)hFontBtn, 0);
		SendMessage(thisBas->hWndOutputGroup, WM_SETFONT, (WPARAM)hFontBtn, 0);

		SendMessage(thisBas->hWndExecCmd, WM_SETFONT, (WPARAM)hFontBtn, 0);
		SendMessage(thisBas->hWndExec, WM_SETFONT, (WPARAM)hFontBtn, 0);
		SendMessage(thisBas->hWndOpen, WM_SETFONT, (WPARAM)hFontBtn, 0);
		SendMessage(thisBas->hWndClear, WM_SETFONT, (WPARAM)hFontBtn, 0);
	}

	std::string IntToHex(int Val) {
		std::stringstream stream;
		stream << std::hex << Val;
		return stream.str();
	}
#define INPUT_CHAR_LIMIT            0x7FFFFFFE
	LRESULT CALLBACK Form::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		int Error;
		switch (uMsg) {
		case WM_CTLCOLORSTATIC: {
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			return (LRESULT)GetStockObject(WHITE_BRUSH);
		}
		case WM_CREATE: {
			thisBas->hWndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "RICHEDIT", 0, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | WS_BORDER, 12, 12, 393, 332, hWnd, (HMENU)mainTextBox, NULL, NULL);

			//thisBas->regTxt = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("RICHEDIT"), TEXT(""), WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE, 12, 12, 393, 332, hWnd, NULL, NULL, NULL);

			//thisBas->hWndEdit1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("RICHEDIT"), TEXT(""), WS_CHILD | ES_AUTOHSCROLL | WS_VISIBLE, 12, 261, 182, 20, hWnd, NULL, NULL, NULL);

			thisBas->hWndList = CreateWindowEx(WS_EX_CLIENTEDGE, RICHEDIT_CLASS, TEXT(""), WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY, 422, -2, 180, 395, hWnd, NULL, NULL, NULL);

			//thisBas->hWndExecCmd = CreateWindow("BUTTON", "Execute Cmd", WS_CHILD | WS_VISIBLE, 200, 261, 109, 20, hWnd, (HMENU)execcmd, NULL, NULL);

			thisBas->hWndExec = CreateWindow("BUTTON", "Execute", WS_CHILD | WS_VISIBLE, 145, 355, 127, 19, hWnd, (HMENU)execute, NULL, NULL);

			thisBas->hWndOpen = CreateWindow("BUTTON", "Open", WS_CHILD | WS_VISIBLE, 12, 355, 127, 19, hWnd, (HMENU)openbtn, NULL, NULL);

			thisBas->hWndClear = CreateWindow("BUTTON", "Clear", WS_CHILD | WS_VISIBLE, 278, 355, 127, 19, hWnd, (HMENU)clear, NULL, NULL);

			SendMessage(thisBas->hWndEdit, EM_SETLIMITTEXT, INPUT_CHAR_LIMIT, NULL);
			SendMessage(thisBas->hWndEdit, EM_SETBKGNDCOLOR, 0, RGB(240, 240, 240));
			SendMessage(thisBas->hWndExec, EM_SETBKGNDCOLOR, 0, RGB(255, 0, 0));

			thisBas->hMenuBar = CreateMenu();
			ADDPOPUPMENU(thisBas->hMenuBar, "");
			//ADDMENUITEM(thisBas->hMenuBarTwo, Commands, "");
			SetMenu(hWnd, thisBas->hMenuBar);
			thisBas->SetupFonts();


		}
						break;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case execcmd: {
				char buffer[1024];
				GetWindowText(thisBas->hWndEdit1, buffer, 1024);
				std::string a(buffer);

				RunCommand(a);
				break;
			}
			case winApi1:
			{
				MessageBoxW(NULL, L"radio1 is checked", L"radio", MB_OK | MB_ICONERROR);
			}
			break;
			case winApi2:
			{
				MessageBoxW(NULL, L"radio2 is checked", L"radio", MB_OK | MB_ICONERROR);
			}
			break;
			case SumonexGui: {
				runLua("downloadstring(\"https://hastebin.com/raw/ijedaneket.php\")");
				break;
			}
			case execute: {
				int len = SendMessage(thisBas->hWndEdit, WM_GETTEXTLENGTH, 0, 0);
				char *buf = new char[len + 1]();
				SendMessage(thisBas->hWndEdit, WM_GETTEXT, (WPARAM)len + 1, (LPARAM)(buf));

				if (buf != "") {
					runLua(buf);
				}
			}
						  break;
			case clear:
				SetWindowText(thisBas->hWndEdit, "");
				break;
			case Commands:
				/*if (!AlreadyRan) {
				if (MessageBox(0, "Are you sure you want to start the chathook?", "Sumonex", MB_YESNO) == IDYES)
				MessageBoxA(0, "Chathook started", "Sumonex", MB_OK); \
				Chathook("LocalPlayer");							  \
				AlreadyRan = true;									  \
				}
				else {
				MessageBox(0, "Chat hook was already started!", "Sumonex", MB_OK);
				}*/
				break;
			case openbtn: {
				WIN32_FIND_DATA ff;
				OPENFILENAME ofn;
				char szFile[100];

				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = NULL;
				ofn.lpstrFile = szFile;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

				GetOpenFileName(&ofn);

				ifstream inFile;
				inFile.open(ofn.lpstrFile);

				stringstream strStream;
				strStream << inFile.rdbuf();
				string str = strStream.str();

				SetWindowText(thisBas->hWndEdit, "");
				rlua::Form::GetBaseInst()->RawOutputTexts(str);

				break;
			}
			}
			break;
		case WM_CLOSE:
			ShowWindow(hWnd, SW_MINIMIZE);
			break;
		case WM_DESTROY:
			ShowWindow(hWnd, SW_MINIMIZE);
			return 0;
		case WM_SETFOCUS:
			uMsg = WM_KILLFOCUS;
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}



	void ExtOutputText(std::string s) {
		thisBas->OutputText(s);
	}
}

int LuaGetFilteringEnabled(int Workspace) {
	return *(BYTE*)(Workspace + 603);
}

int GetParent(int object)
{
	return *(int*)(object + 52);
}

std::string* GetName(int Instance) {
	return (std::string*)(*(int*)(Instance + 0x28));
}

int FindFirstChild(int Instance, std::string Name) {
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

int Workspace = FindFirstChild(DataModel, "Workspace");
int Players = FindFirstChild(DataModel, "Players");

const char* cmds{
	"kill[p] - kills [p]\r\n"
	"money - gives you money in vehicle simulator\r\n"
	"btools[p] - gives[p] btools\r\n"
	"ff[p] - gives[p] ff\r\n"
	"smoke[p] - gives[p] smoke\r\n"
	"fire[p] - gives[p] fire\r\n"
	"sp[p] - gives[p] sparkles\r\n"
	"sparkles[p] - gives[p] sparkles\r\n"
	"unff[p] - removes[p]'s ff\r\n"
	"unsmoke[p] - removes[p]'s smoke\r\n"
	"unfire[p] - removes[p]'s fire\r\n"
	"unsp[p] - removes[p]'s sparkles\r\n"
	"unsparkles[p] - removes[p]'s sparkles\r\n"
	"sink[p] - makes[p] sink into the ground\r\n"
	"bring[p] - teleports[p] to you\r\n"
	"goto[p] - teleports you to[p]\r\n"
	"snoop[p] - makes[p] have snoop particles\r\n"
	"trumpify[p] - makes[p] have trump particles\r\n"
	"ranim[p] - removes[p]'s animation\r\n"
	"invis[p] - makes[p] invisible\r\n"
	"vis[p] - makes[p] visible\r\n"
	"ws[p] [v] - changes[p]'s walkspeed to [v]\r\n"
	"jp[p] [v] - changes[p]'s jumppopwer to [v]\r\n"
	"skybox[s] - changes servers skybox to[s]\r\n"
	"music[s] - adds music with the id of[s]\r\n"
	"stopmusic - stops playing music\r\n"
	"stop - stops playing music\r\n"
	"time [v] - changes server's time to [v]\r\n"
	"mesh [p] [meshid] [meshtextureid] - makes [p] have a mesh with meshID of [meshid] and texture id of [meshtextureid]\r\n"
	"bighead [p] - gives [p] a big head\r\n"
	"char [p] [id] - makes [p] look like [id]\r\n"
	"charapp [p] [id] - makes [p] look like [id]\r\n"
	"explode [p] - explodes [p]\r\n"
	"fogend [v] - changes servers fogend to [v]\r\n"
	"changestat [p] [statname] [v] - changes [p]'s stat with name of [statname] and its value to [v]\r\n"
	"punish [p] - punishes [p]\r\n"
	"unpunish [p] - unpunishes [p]\r\n"
	"exploited - preloaded song\r\n"
	"earrape - preloaded song\r\n"
	"oui - preloaded song\r\n"
	"heathens - preloaded song\r\n"
	"curse - preloaded song\r\n"
	"troll - preloaded song\r\n"
	"earexploder - preloaded song\r\n"
	"gravity [n] - changes servers gravity to [n]\r\n"
};

vector<DWORD> GetChildren(int instance) {
	try {
		vector<DWORD> rtn;
		DWORD num4 = *(DWORD *)(*(DWORD *)(instance + 0x2C));
		DWORD num3 = *(DWORD *)(*(DWORD *)(instance + 0x2C) + 4);
		while (num4 != num3) {
			rtn.push_back(*(DWORD*)num4);
			num4 += 8;
		}
		return rtn;
	}
	catch (exception)
	{

	}
	vector<DWORD> rtnn;
	return rtnn;
}

int GetLocalPlayer(int Plr) {
	return *(int*)(Plr + 0x154);
}

std::string PlayerName = validp;

vector <DWORD> GetCharacterVector(std::string Name) {
	if (Name == "others") {
		vector<DWORD> players = GetChildren(Players);
		if (players.size() != 0) {
			string *playername = GetName(GetLocalPlayer(Players));
			for (unsigned i = 0; i < players.size(); ++i) {
				string CPLAYER = *GetName(players[i]);
				if (CPLAYER == *playername) {
					players.erase(players.begin() + i);
				}
			}
			return players;
		}
		else {
			vector <DWORD> Players;
			return Players;
		}
	}
	else if (Name == "me") {
		vector <DWORD> players = { (DWORD)FindFirstChild(Players, (PlayerName).c_str()) };
		if (players.size() != 0) {
			return players;
		}
		else {
			vector <DWORD> Players;
			return players;
		}
	}
	else if (Name == "all") {
		vector <DWORD> players = GetChildren(Players);
		if (players.size() != 0) {
			return players;
		}
		else {
			vector <DWORD> Players;
			return players;
		}
	}
	else if (Name == "guests") {
		vector <DWORD> players = GetChildren(Players);
		if (players.size() != 0) {
			for (int i = 0; i < players.size(); i++) {
				if (GetName(players[i])->find("Guest ") != std::string::npos) {
					return players;
				}

			}
		}
		else {
			vector <DWORD> Players;
			return players;
		}
	}
	else {
		vector <DWORD> players = { (DWORD)FindFirstChild(Players, (Name).c_str()) };
		if (players.size() != 0) {
			return players;
		}
		else {
			vector <DWORD> Players;
			return players;
		}
	}
}

int GetPlayer(std::string Name) {
	if (Name == "me") {
		int Character = FindFirstChild(Players, PlayerName);
		if (Character != 0) {
			return Character;
		}
		else
		{
			return 0;
		}
	}
	int Player = FindFirstChild(Players, Name);
	if (Player != 0) {
		return Player;
	}
	else
	{
		return 0;
	}
	return 0;
}

void Kill(std::string player) {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, player.c_str());
	rblua_getfield(lua::lua_State, -1, "Character");
	rblua_getfield(lua::lua_State, -1, "breakJoints");
	rblua_pushvalue(lua::lua_State, -2);
	rblua_pcall(lua::lua_State, 1, 0, 0);
	rblua_emptystack(lua::lua_State);
}

void FF(std::string player) {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, player.c_str());
	rblua_getfield(lua::lua_State, -1, "Character");
	rblua_getglobal(lua::lua_State, "Instance");
	rblua_getfield(lua::lua_State, -1, "new");
	rblua_pushstring(lua::lua_State, "ForceField");
	rblua_pushvalue(lua::lua_State, -4);
	rblua_pcall(lua::lua_State, 2, 0, 0);
	rblua_emptystack(lua::lua_State);
}

void lol() {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "StarterGui");
	rblua_getglobal(lua::lua_State, "Instance");
	rblua_getfield(lua::lua_State, -1, "new");
	rblua_pushstring(lua::lua_State, "ScreenGui");
	rblua_pushvalue(lua::lua_State, -4);
}

void UnFF(std::string player) {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, player.c_str());
	rblua_getfield(lua::lua_State, -1, "Character");
	rblua_getfield(lua::lua_State, -1, "ForceField");
	rblua_getfield(lua::lua_State, -1, "Remove");
	rblua_pushvalue(lua::lua_State, -2);
	rblua_pcall(lua::lua_State, 1, 0, 0);
	rblua_emptystack(lua::lua_State);
}

void RemoveTorsoInstance(std::string player, std::string instance) {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, player.c_str());
	rblua_getfield(lua::lua_State, -1, "Character");
	rblua_getfield(lua::lua_State, -1, "Torso");
	rblua_getfield(lua::lua_State, -1, instance.c_str());
	rblua_getfield(lua::lua_State, -1, "Remove");
	rblua_pushvalue(lua::lua_State, -2);
	rblua_pcall(lua::lua_State, 1, 0, 0);
	rblua_emptystack(lua::lua_State);
}

void Fire(std::string player) {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, player.c_str());
	rblua_getfield(lua::lua_State, -1, "Character");
	rblua_getfield(lua::lua_State, -1, "Torso");
	rblua_getglobal(lua::lua_State, "Instance");
	rblua_getfield(lua::lua_State, -1, "new");
	rblua_pushstring(lua::lua_State, "Fire");
	rblua_pushvalue(lua::lua_State, -4);
	rblua_pcall(lua::lua_State, 2, 0, 0);
	rblua_emptystack(lua::lua_State);
}

void Smoke(std::string player) {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, player.c_str());
	rblua_getfield(lua::lua_State, -1, "Character");
	rblua_getfield(lua::lua_State, -1, "Torso");
	rblua_getglobal(lua::lua_State, "Instance");
	rblua_getfield(lua::lua_State, -1, "new");
	rblua_pushstring(lua::lua_State, "Smoke");
	rblua_pushvalue(lua::lua_State, -4);
	rblua_pcall(lua::lua_State, 2, 0, 0);
	rblua_emptystack(lua::lua_State);
}

void Sparkles(std::string player) {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, player.c_str());
	rblua_getfield(lua::lua_State, -1, "Character");
	rblua_getfield(lua::lua_State, -1, "Torso");
	rblua_getglobal(lua::lua_State, "Instance");
	rblua_getfield(lua::lua_State, -1, "new");
	rblua_pushstring(lua::lua_State, "Sparkles");
	rblua_pushvalue(lua::lua_State, -4);
	rblua_pcall(lua::lua_State, 2, 0, 0);
	rblua_emptystack(lua::lua_State);
}

void Snoop(std::string player) {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, player.c_str());
	rblua_getfield(lua::lua_State, -1, "Character");
	rblua_getfield(lua::lua_State, -1, "Torso");
	rblua_getglobal(lua::lua_State, "Instance");
	rblua_getfield(lua::lua_State, -1, "new");
	rblua_pushstring(lua::lua_State, "ParticleEmitter");
	rblua_pushvalue(lua::lua_State, -4);
	rblua_pcall(lua::lua_State, 2, 1, 0);

	rblua_pushstring(lua::lua_State, "rbxassetid://387391952");
	rblua_setfield(lua::lua_State, -2, "Texture");

	rblua_pushvalue(lua::lua_State, -2);
	rblua_pcall(lua::lua_State, 1, 0, 0);
}

void Donald(std::string player) {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, player.c_str());
	rblua_getfield(lua::lua_State, -1, "Character");
	rblua_getfield(lua::lua_State, -1, "Torso");
	rblua_getglobal(lua::lua_State, "Instance");
	rblua_getfield(lua::lua_State, -1, "new");
	rblua_pushstring(lua::lua_State, "ParticleEmitter");
	rblua_pushvalue(lua::lua_State, -4);
	rblua_pcall(lua::lua_State, 2, 1, 0);

	rblua_pushstring(lua::lua_State, "rbxassetid://343377229");
	rblua_setfield(lua::lua_State, -2, "Texture");

	rblua_pushvalue(lua::lua_State, -2);
	rblua_pcall(lua::lua_State, 1, 0, 0);
}

void RemoveAnim(std::string player)
{
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, player.c_str());
	rblua_getfield(lua::lua_State, -1, "Character");
	rblua_getfield(lua::lua_State, -1, "Humanoid");
	rblua_getfield(lua::lua_State, -1, "Animator");
	rblua_getfield(lua::lua_State, -1, "Remove");
	rblua_pushvalue(lua::lua_State, -2);
	rblua_pcall(lua::lua_State, 1, 0, 0);
}

void Skybox(std::string id) {
	std::string rbx = "rbxassetid://";
	std::string lol = rbx + id;
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Lighting");
	rblua_getglobal(lua::lua_State, "Instance");
	rblua_getfield(lua::lua_State, -1, "new");
	rblua_pushstring(lua::lua_State, "Sky");
	rblua_pushvalue(lua::lua_State, -4);
	rblua_pcall(lua::lua_State, 2, 1, 0);

	rblua_pushstring(lua::lua_State, lol.c_str());
	rblua_setfield(lua::lua_State, -2, "SkyboxBk");

	rblua_pushstring(lua::lua_State, lol.c_str());
	rblua_setfield(lua::lua_State, -2, "SkyboxDn");

	rblua_pushstring(lua::lua_State, lol.c_str());
	rblua_setfield(lua::lua_State, -2, "SkyboxFt");

	rblua_pushstring(lua::lua_State, lol.c_str());
	rblua_setfield(lua::lua_State, -2, "SkyboxLf");

	rblua_pushstring(lua::lua_State, lol.c_str());
	rblua_setfield(lua::lua_State, -2, "SkyboxRt");

	rblua_pushstring(lua::lua_State, lol.c_str());
	rblua_setfield(lua::lua_State, -2, "SkyboxUp");

	rblua_pushvalue(lua::lua_State, -2);
	rblua_pcall(lua::lua_State, 1, 0, 0);
}

void ChangeSpeed(std::string player, float value) {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, player.c_str());
	rblua_getfield(lua::lua_State, -1, "Character");
	rblua_getfield(lua::lua_State, -1, "Humanoid");
	rblua_pushnumber(lua::lua_State, value);
	rblua_setfield(lua::lua_State, -2, "WalkSpeed");
	rblua_pcall(lua::lua_State, 1, 0, 0);
	rblua_emptystack(lua::lua_State);
}

void StopMusic() {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Workspace");
	rblua_getfield(lua::lua_State, -1, "Sumonex");
	rblua_getfield(lua::lua_State, -1, "Destroy");
	rblua_pushvalue(lua::lua_State, -2);
	rblua_pcall(lua::lua_State, 1, 0, 0);
	rblua_emptystack(lua::lua_State);
}

void Music(std::string id) {
	rblua_getglobal(lua::lua_State, "workspace");
	rblua_getglobal(lua::lua_State, "Instance");
	rblua_getfield(lua::lua_State, -1, "new");
	rblua_pushstring(lua::lua_State, "Sound");
	rblua_pushvalue(lua::lua_State, -4);
	rblua_pcall(lua::lua_State, 2, 1, 0);

	rblua_pushstring(lua::lua_State, ("rbxassetid://" + id).c_str());
	rblua_setfield(lua::lua_State, -2, "SoundId");

	rblua_pushstring(lua::lua_State, "Sumonex");
	rblua_setfield(lua::lua_State, -2, "Name");

	rblua_pushnumber(lua::lua_State, 1);
	rblua_setfield(lua::lua_State, -2, "Volume");

	rblua_getfield(lua::lua_State, -1, "Play");
	rblua_pushvalue(lua::lua_State, -2);
	rblua_pcall(lua::lua_State, 1, 0, 0);
}

void ChangeJump(std::string player, float value) {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, player.c_str());
	rblua_getfield(lua::lua_State, -1, "Character");
	rblua_getfield(lua::lua_State, -1, "Humanoid");
	rblua_pushnumber(lua::lua_State, value);
	rblua_setfield(lua::lua_State, -2, "JumpPower");
	rblua_pcall(lua::lua_State, 1, 0, 0);
	rblua_emptystack(lua::lua_State);
}

void Teleport(std::string player1, std::string player2) {
	rblua_getfield(lua::lua_State, -10002, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, player1.c_str());
	rblua_getfield(lua::lua_State, -1, "Character");
	rblua_getfield(lua::lua_State, -1, "HumanoidRootPart");
	rblua_getfield(lua::lua_State, -1, "CFrame");

	rblua_getfield(lua::lua_State, -10002, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, player2.c_str());
	rblua_getfield(lua::lua_State, -1, "Character");
	rblua_getfield(lua::lua_State, -1, "HumanoidRootPart");
	rblua_pushvalue(lua::lua_State, -6);
	rblua_setfield(lua::lua_State, -2, "CFrame");
}

void Sink(std::string player)
{
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, player.c_str());
	rblua_getfield(lua::lua_State, -1, "Character");
	rblua_getfield(lua::lua_State, -1, "HumanoidRootPart");
	rblua_getfield(lua::lua_State, -1, "Remove");
	rblua_pushvalue(lua::lua_State, -2);
	rblua_pcall(lua::lua_State, 1, 0, 0);
}

void SetTransp(std::string ye, std::string inst, double erm) {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, ye.c_str());
	rblua_getfield(lua::lua_State, -1, "Character");
	rblua_getfield(lua::lua_State, -1, inst.c_str());
	rblua_pushnumber(lua::lua_State, erm);
	rblua_setfield(lua::lua_State, -2, "Transparency");
	rblua_pcall(lua::lua_State, 1, 0, 0);
	rblua_emptystack(lua::lua_State);
}

void BTools(std::string player) {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, player.c_str());
	rblua_getfield(lua::lua_State, -1, "Backpack");
	rblua_getglobal(lua::lua_State, "Instance");
	rblua_getfield(lua::lua_State, -1, "new");
	rblua_pushstring(lua::lua_State, "HopperBin");
	rblua_pushvalue(lua::lua_State, -4);
	rblua_pcall(lua::lua_State, 2, 1, 0);
	rblua_pushnumber(lua::lua_State, 2);
	rblua_setfield(lua::lua_State, -2, "BinType");
	rblua_emptystack(lua::lua_State);

	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, player.c_str());
	rblua_getfield(lua::lua_State, -1, "Backpack");
	rblua_getglobal(lua::lua_State, "Instance");
	rblua_getfield(lua::lua_State, -1, "new");
	rblua_pushstring(lua::lua_State, "HopperBin");
	rblua_pushvalue(lua::lua_State, -4);
	rblua_pcall(lua::lua_State, 2, 1, 0);
	rblua_pushnumber(lua::lua_State, 3);
	rblua_setfield(lua::lua_State, -2, "BinType");
	rblua_emptystack(lua::lua_State);

	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, player.c_str());
	rblua_getfield(lua::lua_State, -1, "Backpack");
	rblua_getglobal(lua::lua_State, "Instance");
	rblua_getfield(lua::lua_State, -1, "new");
	rblua_pushstring(lua::lua_State, "HopperBin");
	rblua_pushvalue(lua::lua_State, -4);
	rblua_pcall(lua::lua_State, 2, 1, 0);
	rblua_pushnumber(lua::lua_State, 4);
	rblua_setfield(lua::lua_State, -2, "BinType");
	rblua_emptystack(lua::lua_State);
}

void MeshPlayer(std::string plr, std::string id, std::string id1) {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, plr.c_str());
	rblua_getfield(lua::lua_State, -1, "Character");
	rblua_getfield(lua::lua_State, -1, "Torso");

	rblua_getglobal(lua::lua_State, "Instance");
	rblua_getfield(lua::lua_State, -1, "new");
	rblua_pushstring(lua::lua_State, "SpecialMesh");

	rblua_pushvalue(lua::lua_State, -4);
	rblua_pcall(lua::lua_State, 2, 1, 0);

	rblua_pushstring(lua::lua_State, ("rbxassetid://" + id).c_str());
	rblua_setfield(lua::lua_State, -2, "MeshId");

	rblua_pushstring(lua::lua_State, ("rbxassetid://" + id1).c_str());
	rblua_setfield(lua::lua_State, -2, "TextureId");
	rblua_pushvalue(lua::lua_State, -4);
	rblua_pcall(lua::lua_State, 2, 1, 0);
}

void ChangeCharacter(std::string plr, std::string id) {
	std::string idxd = "http://www.roblox.com/Asset/CharacterFetch.ashx?userId=" + id;
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, plr.c_str());
	rblua_pushstring(lua::lua_State, idxd.c_str());
	rblua_setfield(lua::lua_State, -2, "CharacterAppearance");
	rblua_pcall(lua::lua_State, 1, 0, 0);
	rblua_emptystack(lua::lua_State);
	Kill(plr);
}

void BigHead(std::string plr) {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, plr.c_str());
	rblua_getfield(lua::lua_State, -1, "Character");
	rblua_getfield(lua::lua_State, -1, "GetChildren");

	rblua_getglobal(lua::lua_State, "Vector3");
	rblua_getfield(lua::lua_State, -1, "new");
	rblua_pushnumber(lua::lua_State, 5);
	rblua_pushnumber(lua::lua_State, 5);
	rblua_pushnumber(lua::lua_State, 5);
	rblua_pushnumber(lua::lua_State, 5);
	rblua_pushnumber(lua::lua_State, 5);
	rblua_pushnumber(lua::lua_State, 5);
	rblua_pcall(lua::lua_State, 6, 1, 0);
	rblua_setfield(lua::lua_State, -3, "Size");
}

void BlockHead(std::string plr) {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, plr.c_str());
	rblua_getfield(lua::lua_State, -1, "Character");
	rblua_getfield(lua::lua_State, -1, "Head");
	rblua_getfield(lua::lua_State, -1, "Mesh");
	rblua_getfield(lua::lua_State, -1, "Remove");
	rblua_pushvalue(lua::lua_State, -2);
	rblua_pcall(lua::lua_State, 1, 0, 0);
}

void GiveMoney() {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "ReplicatedStorage");
	rblua_getfield(lua::lua_State, -1, "RaceSystem");
	rblua_getfield(lua::lua_State, -1, "finish_race");
	rblua_getfield(lua::lua_State, -1, "FireServer");
	rblua_pushvalue(lua::lua_State, -2);
	rblua_pushstring(lua::lua_State, "Quarter Mile");
	rblua_pushnumber(lua::lua_State, 3.5);
	rblua_pcall(lua::lua_State, 3, 1, 0);
	rblua_emptystack(lua::lua_State);
}

std::vector<std::string> split(std::string s) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	return vstrings;
}

bool RunCommand(std::string src) {
	std::vector<std::string> In = split(src.c_str());

	if (In.at(0) == "kill") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					Kill(name.c_str());
				}
			}
		}
	}

	else if (In.at(0) == "cmds") {
		MessageBox(0, cmds, "Commands", MB_OK);
	}

	else if (In.at(0) == "money") {
		GiveMoney();
		MessageBox(0, "Gave Money", "MB_OK", MB_OK);
	}

	else if (In.at(0) == "btools") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					BTools(name.c_str());
				}
			}
		}
	}

	else if (In.at(0) == "ff") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					FF(name.c_str());
				}
			}
		}
	}

	else if (In.at(0) == "unfire") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					RemoveTorsoInstance(name.c_str(), "Fire");
				}
			}
		}
	}

	else if (In.at(0) == "unsmoke") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					RemoveTorsoInstance(name.c_str(), "Smoke");
				}
			}
		}
	}

	else if (In.at(0) == "unsp") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					RemoveTorsoInstance(name.c_str(), "Sparkles");
				}
			}
		}
	}

	else if (In.at(0) == "unsparkles") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					RemoveTorsoInstance(name.c_str(), "Sparkles");
				}
			}
		}
	}

	else if (In.at(0) == "sp") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					Sparkles(name.c_str());
				}
			}
		}
	}

	else if (In.at(0) == "prefix") {
		std::string prefixs = In.at(1);
		prefix = prefixs;
	}

	else if (In.at(0) == "bhead" || In.at(0) == "blockhead") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					BlockHead(name.c_str());
				}
			}
		}
	}

	else if (In.at(0) == "fire") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					Fire(name.c_str());
				}
			}
		}
	}

	else if (In.at(0) == "smoke") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					Smoke(name.c_str());
				}
			}
		}
	}

	else if (In.at(0) == "sparkles") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					Sparkles(name.c_str());
				}
			}
		}
	}

	else if (In.at(0) == "time") {
		std::string id = In.at(1);
		rblua_getglobal(lua::lua_State, "game");
		rblua_getfield(lua::lua_State, -1, "Lighting");
		rblua_pushstring(lua::lua_State, id.c_str());
		rblua_setfield(lua::lua_State, -2, "TimeOfDay");
		rblua_emptystack(lua::lua_State);
	}

	else if (In.at(0) == "statchange" || In.at(0) == "changestat") {
		std::string statname = In.at(1);
		float value = atof(In.at(2).c_str());
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					rblua_getglobal(lua::lua_State, "game");
					rblua_getfield(lua::lua_State, -1, "Players");
					rblua_getfield(lua::lua_State, -1, name.c_str());
					rblua_getfield(lua::lua_State, -1, "leaderstats");
					rblua_getfield(lua::lua_State, -1, statname.c_str());
					rblua_pushnumber(lua::lua_State, value);
					rblua_setfield(lua::lua_State, -2, "Value");
				}
			}
		}
	}

	else if (In.at(0) == "mesh") {
		std::string meshid = In.at(2);
		std::string meshtex = In.at(3);
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					MeshPlayer(name.c_str(), meshid, meshtex);
				}
			}
		}
	}



	else if (In.at(0) == "bighead") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					BigHead(name.c_str());
				}
			}
		}
	}

	else if (In.at(0) == "char" || In.at(0) == "charapp") {
		std::string id = In.at(2);
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					ChangeCharacter(name.c_str(), id);
				}
			}
		}
	}

	else if (In.at(0) == "fogend") {
		float number = atof(In.at(1).c_str());

		rblua_getglobal(lua::lua_State, "game");
		rblua_getfield(lua::lua_State, -1, "Lighting");
		rblua_pushnumber(lua::lua_State, number);
		rblua_setfield(lua::lua_State, -2, "FogEnd");
		rblua_pcall(lua::lua_State, 1, 0, 0);
		rblua_emptystack(lua::lua_State);
	}

	else if (In.at(0) == "sink") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					Sink(name.c_str());
				}
			}
		}
	}

	else if (In.at(0) == "bring") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					Teleport("LocalPlayer", name.c_str());
				}
			}
		}
	}

	else if (In.at(0) == "goto") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					Teleport(name.c_str(), "LocalPlayer");
				}
			}
		}
	}

	else if (In.at(0) == "snoop") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					Snoop(name.c_str());
				}
			}
		}
	}

	else if (In.at(0) == "trumpify") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					Donald(name.c_str());
				}
			}
		}
	}

	else if (In.at(0) == "ranim") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					RemoveAnim(name.c_str());
				}
			}
		}
	}

	else if (In.at(0) == "punish") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					SetTransp(name.c_str(), "Head", 1);
					SetTransp(name.c_str(), "Torso", 1);
					SetTransp(name.c_str(), "Right Arm", 1);
					SetTransp(name.c_str(), "Left Arm", 1);
					SetTransp(name.c_str(), "Left Leg", 1);
					SetTransp(name.c_str(), "Right Leg", 1);
					ChangeSpeed(name.c_str(), 0);
					ChangeJump(name.c_str(), 0);
				}
			}
		}
	}

	else if (In.at(0) == "unpunish") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					SetTransp(name.c_str(), "Head", 0);
					SetTransp(name.c_str(), "Torso", 0);
					SetTransp(name.c_str(), "Right Arm", 0);
					SetTransp(name.c_str(), "Left Arm", 0);
					SetTransp(name.c_str(), "Left Leg", 0);
					SetTransp(name.c_str(), "Right Leg", 0);
					ChangeSpeed(name.c_str(), 16);
					ChangeJump(name.c_str(), 50);
				}
			}
		}
	}

	else if (In.at(0) == "gravity") {
		rblua_getglobal(lua::lua_State, "game");
		rblua_getfield(lua::lua_State, -1, "Workspace");
		rblua_pushnumber(lua::lua_State, atoi(In.at(1).c_str()));
		rblua_setfield(lua::lua_State, -2, "Gravity");
		rblua_pcall(lua::lua_State, 1, 0, 0);
		rblua_emptystack(lua::lua_State);
	}

	else if (In.at(0) == "jump") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					rblua_getglobal(lua::lua_State, "game");
					rblua_getfield(lua::lua_State, -1, "Players");
					rblua_getfield(lua::lua_State, -1, name.c_str());
					rblua_getfield(lua::lua_State, -1, "Character");
					rblua_getfield(lua::lua_State, -1, "Humanoid");
					rblua_pushboolean(lua::lua_State, true);
					rblua_setfield(lua::lua_State, -2, "Jump");
					rblua_emptystack(lua::lua_State);
				}
			}
		}
	}

	else if (In.at(0) == "sit") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					rblua_getglobal(lua::lua_State, "game");
					rblua_getfield(lua::lua_State, -1, "Players");
					rblua_getfield(lua::lua_State, -1, name.c_str());
					rblua_getfield(lua::lua_State, -1, "Character");
					rblua_getfield(lua::lua_State, -1, "Humanoid");
					rblua_pushboolean(lua::lua_State, true);
					rblua_setfield(lua::lua_State, -2, "Sit");
					rblua_emptystack(lua::lua_State);
				}
			}
		}
	}

	else if (In.at(0) == "invis") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					SetTransp(name.c_str(), "Head", 1);
					SetTransp(name.c_str(), "Torso", 1);
					SetTransp(name.c_str(), "Right Arm", 1);
					SetTransp(name.c_str(), "Left Arm", 1);
					SetTransp(name.c_str(), "Left Leg", 1);
					SetTransp(name.c_str(), "Right Leg", 1);
				}
			}
		}
	}

	else if (In.at(0) == "unff") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					UnFF(name.c_str());
				}
			}
		}
	}

	else if (In.at(0) == "vis") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					SetTransp(name.c_str(), "Head", 0);
					SetTransp(name.c_str(), "Torso", 0);
					SetTransp(name.c_str(), "Right Arm", 0);
					SetTransp(name.c_str(), "Left Arm", 0);
					SetTransp(name.c_str(), "Left Leg", 0);
					SetTransp(name.c_str(), "Right Leg", 0);
				}
			}
		}
	}

	else if (In.at(0) == "freeze") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					ChangeSpeed(name.c_str(), 0);
				}
			}
		}
	}

	else if (In.at(0) == "thaw") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					ChangeSpeed(name.c_str(), 16);
				}
			}
		}
	}

	else if (In.at(0) == "ws") {
		float value = atof(In.at(2).c_str());
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					ChangeSpeed(name.c_str(), value);
				}
			}
		}
	}

	else if (In.at(0) == "explode") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					rblua_getglobal(lua::lua_State, "game");
					rblua_getfield(lua::lua_State, -1, "Players");
					rblua_getfield(lua::lua_State, -1, name.c_str());
					rblua_getfield(lua::lua_State, -1, "Character");
					rblua_getfield(lua::lua_State, -1, "Torso");

					rblua_getglobal(lua::lua_State, "Instance");
					rblua_getfield(lua::lua_State, -1, "new");
					rblua_pushstring(lua::lua_State, "Explosion");
					rblua_pushvalue(lua::lua_State, -4);
					rblua_pcall(lua::lua_State, 2, 0, 0);
				}
			}
		}
	}

	else if (In.at(0) == "jp") {
		float value = atof(In.at(2).c_str());
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					ChangeJump(name.c_str(), value);
				}
			}
		}
	}

	else if (In.at(0) == "skybox") {
		std::string id = In.at(1);
		Skybox(id);
	}

	else if (In.at(0) == "music" || In.at(0) == "sound") {
		std::string id = In.at(1);
		Music(id);
	}

	else if (In.at(0) == "admin") {
		vector <DWORD> Players = GetCharacterVector(In.at(1).c_str());
		if (Players.size() != 0) {
			for (unsigned i = 0; i < Players.size(); ++i) {
				int player = GetPlayer(*GetName(Players[i]));
				string name = *GetName(player);
				if (player != 0) {
					for (int i = 0; i < Players.size(); i++) {
						//ChatHookr(name.c_str());
					}
				}
			}
		}
	}

	if (In.at(0) == "exploited") {
		Music("174266954");
	}

	if (In.at(0) == "earrape") {
		Music("415903316");
	}

	if (In.at(0) == "oui") {
		Music("333339290");
	}

	if (In.at(0) == "earexploder") {
		Music("552417028");
	}

	if (In.at(0) == "heathens") {
		Music("438917260");
	}

	if (In.at(0) == "curse") {
		Music("507454162");
	}

	if (In.at(0) == "troll") {
		Music("167653905");
	}

	else if (In.at(0) == "stopmusic" || In.at(0) == "stop") {
		StopMusic();
	}

	return true;
}

static int Chathookr(lua_State* L) {
	callcheck();
	std::string msg = rblua_tostring((int)L, -2);

	RunCommand(msg.c_str());
	return 1;
}

void Chathook(const char* plr) {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, plr);
	rblua_getfield(lua::lua_State, -1, "Chatted");
	rblua_getfield(lua::lua_State, -1, "connect");
	rblua_pushvalue(lua::lua_State, -2);
	rblua_pushcfunction(lua::lua_State, Chathookr);
	rblua_pcall(lua::lua_State, 2, 0, 0);
	rblua_emptystack(lua::lua_State);
}

std::string userInfo = "";

std::string getUserInfo() {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, "LocalPlayer");
	rblua_getfield(lua::lua_State, -1, "Name");
	userInfo = rblua_tostring(lua::lua_State, -1);
	return userInfo;
	rblua_emptystack(lua::lua_State);
}

std::string Input() {
	std::string ass;
	getline(std::cin, ass);
	return ass;
}

bool toRun = true;

LONG RobloxWndProc;
HWND RobloxWindow;

LRESULT CALLBACK PatchedWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void HookRobloxWndProc() {
	SetWindowLongPtr(RobloxWindow, GWLP_WNDPROC, (LONG)PatchedWndProc);
}



void main() {
	//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StopRobloxInput, 0, 0, 0);
//	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)RobloxExit, 0, 0, 0);
	//testInit();
//	InitiateAuthenticationAndConsole();
	new rlua::Form("Summonex", "Summonex.", (rlua::OnLoad)[](rlua::Form* arg1)->void {
	});
}



BOOL APIENTRY DllMain(HMODULE hModule, DWORD reasonForCall, LPVOID lpReserved) {
	//VMProtectBeginMutation("gifmlsdfsdfiomef");
	if (reasonForCall == DLL_PROCESS_ATTACH) {
		//OldFilter = SehFunc(SEHHndler);
		DisableThreadLibraryCalls(hModule);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, 0);
	}
	//VMProtectEnd();
	return 1;
}
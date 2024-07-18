#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <string>
#include <fstream>
#include <sstream>
#include <Psapi.h>

#include "elysian.h"

#include "VMProtectSDK.h"
#include "memutil.h"
#include "mem.h"
#include "form.h"
#include "alua.h"
#include "rinstance.h"
#include "util.h"
#include "sha3.h"
#include "sha.h"
#include "base64.h"
#include "aes.h"
#include "modes.h"

void HackExit(const char* error);
void CreateConsole();
std::vector<std::string> GetArguments(const std::string& input);

RLua* rLua;

void CommandLoop() {
	while (1) {
		putchar('>');

		std::string input;
		std::getline(std::cin, input);
		std::vector<std::string> arguments = GetArguments(input);
		std::string command = arguments[0];
		arguments.erase(arguments.begin());

		if (command == "exit") {
			break;
		}
		else if (command == "listchildren") {
			printf("\n");
			char* sList[10];

			for (int i = 0; i < 10; ++i) {
				if (i == arguments.size())
					break;

				sList[i] = (char*)arguments[i].data();
			}

			int inst = rLua->GetInstanceInHierarchy(sList, arguments.size());

			if (inst) {
				std::vector<int> children = rLua->GetChildren(inst);

				for (int i = 0; i < children.size(); ++i) {
					printf("%X - %s - %s\r\n", children[i], (*rLua->GetName(children[i])).c_str(), (*rLua->GetClass(children[i])).c_str());
				}
			}
			else {
				printf("error: couldn't find instance.\n");
			}
		}
		else if (command == "runfile") {
			std::string arg_path = arguments[0];
			arg_path = "scripts\\" + arg_path;
			char path[MAX_PATH];
			util::GetFile(ELYSIAN_DLL, arg_path.c_str(), path, MAX_PATH);

			std::string data;
			if (util::ReadFile(path, data, 0))
				luaA_execute(data.c_str());
			else
				printf("unable to open path %s\n", path);
		}
		else if (!command.empty()) {
			// execute
			luaA_executeraw(input.c_str(), "ELYSIANRAW");
		}

		putchar('\n');
	}

	return;
}

/*
data format:
1 byte	unsigned char 	size of data
4 bytes	int				did error
4 bytes int				game address
4 bytes	int				r_lua_setfield
4 bytes int 			setThreadIdentity
4 bytes	int				message length (including null at end)
string	char[]			message (username or error)
*/

typedef struct ResponseData {
	int rdsz;
	int did_error;
	int game_addr;
	int setfield;
	int sti;
	int len;
};

int FileCheck(int pid, std::vector<std::string> list) {
	int count = 0;
	HANDLE proc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pid);

	if (proc) {
		char pathRaw[MAX_PATH];
		if (GetModuleFileNameEx(proc, NULL, pathRaw, MAX_PATH)) {
			std::string path = pathRaw;
			path = path.substr(0, path.find_last_of("\\/")); // remove file name

			std::vector<std::string> files;
			util::GetFilesInDirectory(files, path, false);

			for (std::vector<std::string>::iterator i = files.begin(); i != files.end(); i++) {
				for (std::vector<std::string>::iterator j = list.begin(); j != list.end(); j++) {
					if (*i == *j) count++;
				}
			}
		}
	}

	CloseHandle(proc);

	return count;
}



int GenerateChecksum(std::string& out) {
	char path[MAX_PATH];
	util::GetFile(ELYSIAN_DLL, ELYSIAN_DLL, path, MAX_PATH);

	std::string contents;
	if (!util::ReadFile(path, contents, true))
		return 0;

	__int64 buffer;
	CryptoPP::SHA3(8).CalculateDigest((byte*)&buffer, (const byte*)contents.c_str(), contents.length());

	std::stringstream ss;
	ss << std::hex << buffer;
	out = ss.str();

	return 1;
}

int GetResponseData(const std::string& content, const std::string& ticket, unsigned char* buffer, unsigned int sz) {
	VMProtectBeginMutation("data");
	if (content.size() < 1)
		throw std::exception("empty response");

	unsigned char key[32];
	CryptoPP::SHA256().CalculateDigest((byte*)key, (const byte*)ticket.c_str(), ticket.size());

	std::string response;
	CryptoPP::Base64Decoder decoder;

	decoder.Put((byte*)content.data(), content.size());
	decoder.MessageEnd();

	CryptoPP::word64 size = decoder.MaxRetrievable();
	if (size && size <= SIZE_MAX)
	{
		response.resize(size);
		decoder.Get((byte*)response.data(), response.size());
	}
	else
		throw std::exception("failed to decode");

	std::string iv = response.substr(0, 16);
	std::string data = response.substr(16);

	if (data.size() > sz)
		throw std::exception("data is too large");

	CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption cfbDecryption((const unsigned char*)key, 32, (const unsigned char*)iv.c_str());
	cfbDecryption.ProcessData(buffer, (byte*)data.c_str(), data.size());

	VMProtectEnd();
	return *buffer;
}

typedef int(__thiscall *HttpGet_Def)(int dataModel, std::string& out, std::string url, int async);
HttpGet_Def HttpGet; //= (httpGet_Def)0x579940;
int* DefaultContext; //= (int*)0x16D456C;

int FindInitDependencies() {
	int http_get_result =			sigscan::scan(0, "\x83\xEC\x20\x8B\xFC\x89\x65\x24\x6A\x00\x83\xEC\x1C", "xxxxxxxxxxxxx");	// 83 EC 20 8B FC 89 65 24 6A 00 83 EC 1C
	int default_context_result =	sigscan::scan(0, "\x83\xC4\x04\x3D\x00\x00\x00\x00\x74\x00\x8B\x06", "xxxx????x?xx");		// 83 C4 04 3D ?? ?? ?? ?? 74 ?? 8B 06

	if (!(http_get_result && default_context_result))
		return 0;

	HttpGet = (HttpGet_Def)(http_get_result - 175);
	DefaultContext = *(int**)(default_context_result + 4);
}

std::string Initiate() {
	form->print(RGB(0, 0, 0), "Authenticating... ");

	util::timer timer;
	timer.Start();

	VMProtectBeginMutation("i love you");
#ifndef DISABLE_WHITELIST

	/* check for malicious software (fiddler, cheat engine, and more) */
	int msflag = 0;
	MaliciousSoftwareCheck(&msflag);
	if (!msflag)
		throw std::exception("malicious software/item found");

	if (!FindInitDependencies())
		throw std::exception("failed to find init dependencies");

	/* change default security context so we can call httpget */
	DefaultContext[0] = 7;
	DefaultContext[1] = 1;

	/* get auth ticket */
	std::string ticket = "";
	HttpGet(0, ticket, "http://www.roblox.com/game/getauthticket", true);

	/* generate file checksum */
	std::string checksum;
	if (!GenerateChecksum(checksum))
		throw std::exception("failed to generate checksum");

	/* send auth request */
	std::string content;
	if (util::DownloadURL("www.aehst.org", "aje.php", "q=" + ticket + "&s=" + checksum, content, false, true, true) != DLURL_SUCCESS)
		throw std::exception("failed to contact server");

	/* parse and decrypt response */
	char data[300];
	GetResponseData(content, ticket, (unsigned char*)data, 300);

	ResponseData* response_data = (ResponseData*)data;
	const char* user_name = (const char*)(response_data + 1);
	if (response_data->did_error == 1)
		throw std::exception(user_name);

	if (response_data->did_error > 1) {
		/*MessageBox(NULL,	"Elysian recieved invalid or corrupted data or was unable to decrypt the response.\n"
		"Please make sure your system clock/time is correct and you have the latest version of Elysian.\n"
		"\nVisit http://time.is/ for help in synchronizing your clock.", "Error", MB_OK);*/
		throw std::exception("response is corrupted");
	}

	/* handle offsets sent from server */
	r_lua_setfield = (r_lua_setfield_Def)response_data->setfield;
	SetThreadIdentity = (SetThreadIdentity_Def)response_data->sti;
	int game_address = response_data->game_addr;
#else
	const char* user_name = "Owner";
	r_lua_setfield = (r_lua_setfield_Def)0x13DEA0;
	SetThreadIdentity = (SetThreadIdentity_Def)0x139440;
	int game_address = 0xFD7E4C;
#endif

	reg_offset(r_lua_setfield);
	reg_offset(SetThreadIdentity);
	reg_offset(game_address);

	form->print(RGB(0, 150, 0), "OK ");
	form->print(RGB(0, 0, 0), "(%fs)\r\nInitiating... \r\n", timer.Stop());

	// bypass memecheck
	BypassMemoryChecker();
	//gg
	/* safely retrieve game instance */
	int game_instance;
	try {
		game_instance = *(int*)(game_address);
		game_instance = *(int*)(game_instance + 0xD4);
		game_instance = *(int*)(game_instance + 8);
		game_instance = *(int*)(game_instance + 16);
	}
	catch (...) {
		game_instance = 0;
	}

	if (!game_instance)
		throw std::exception("failed to get game");

	rLua = new RLua(game_instance);
	printf("Game Instance: %X\n", rLua->GetGameInstance());

	int script_context = rLua->GetChildByClass(rLua->GetGameInstance(), "ScriptContext");
	if (!script_context)
		throw std::exception("failed to get script context service");

	printf("Script Context: %X\n", script_context);
	printf("Lua State: %X\n", luaA_init(script_context));

	VMProtectEnd();

	form->print(RGB(0, 0, 0), "Done!\r\n");
	return user_name;
}

void main() {
	//EnableConsole(ELYSIAN_DLL);
	//CreateConsole();

	/* initiate window */
	try {
		form->Start(ELYSIAN_TITLE, "e01043", ELYSIAN_DLL);
	}
	catch (std::exception e) {
		HackExit(e.what());
	}

	form->AssignConsoleRoutine(CommandLoop);

	/* hide console window */
#ifndef DEBUG_MODE
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	/* initiate */
	/*std::string player_name;
	try {
		player_name = Initiate();
	}
	catch (std::exception e) {
		HackExit(e.what());
	}
	catch (...) {
		HackExit("an unknown exception occured while initiating (crash)");
	}*/

	//if (!player_name.empty())
		form->print(RGB(0, 0, 255), "Welcome, ChameleonBuilding\r\n");

	luaA_setinit(true);
	
	/* wait for window to exit */
	form->Wait();

	HackExit(0);
}

void HackExit(const char* error) {
	if (error) {
		if (!form->IsRunning()) {
			ShowWindow(GetConsoleWindow(), SW_SHOW);
			printf("\nERROR: %s\n", error);
			util::pause();
		}
		else {
#ifdef DEBUG_MODE
			printf("\nERROR: %s\n", error);
			util::pause();
#endif
			form->print(RGB(255, 0, 0), "\r\nERROR: %s\r\n", error);

			// TODO: shutdown
		}
	}

	delete rLua;
	//etc

	FreeConsole();
	FreeLibraryAndExitThread(GetModuleHandle(ELYSIAN_DLL), 0);
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved) {
	DisableThreadLibraryCalls(hModule);
	if (dwReason == DLL_PROCESS_ATTACH) {
		HANDLE hThread = NULL;
		HANDLE hDllMainThread = OpenThread(THREAD_ALL_ACCESS, NULL, GetCurrentThreadId());
		if (lpReserved == NULL) {
			if (!(hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, 0))) { //start main()
				CloseHandle(hDllMainThread);
				return FALSE;
			}
			CloseHandle(hThread);
		}
		else if (dwReason == DLL_PROCESS_DETACH) {}
		return TRUE;
	}
}

void CreateConsole() {
	if (GetConsoleWindow())
		return;

	int hConHandle = 0;
	HANDLE lStdHandle = 0;
	FILE *fp = 0;

	// Allocate a console
	AllocConsole();

	// redirect unbuffered STDOUT to the console
	lStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);

	// redirect unbuffered STDIN to the console
	lStdHandle = GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	// redirect unbuffered STDERR to the console
	lStdHandle = GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);

	SetConsoleTitle(ELYSIAN_TITLE);
}

std::vector<std::string> GetArguments(const std::string& input) {
	std::vector<std::string> rtn;
	unsigned int i = 0;
	unsigned char sz = input.size();
	int pos = 0;

	if (input[0] == ' ') {
		i++;
		sz--;
	}

	for (; i < sz; ++i) {
		if (input[i] == ' ') {
			rtn.push_back(input.substr(pos, i - pos));
			pos = i + 1;
		}
		else if (i == sz - 1) {
			rtn.push_back(input.substr(pos, i - pos + 1));
			pos = i + 1;
		}
	}

	return rtn;
}
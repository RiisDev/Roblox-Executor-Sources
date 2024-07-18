#define CRT_SECURE_NO_DEPRICATE
#define CRT_SECURE_NO_WARNINGS
#include "RobloxLua.h"
#include "CommandParser.h"
#include "luaC_parser.h"

int Exploit();
int maininit();

void LuaCpipe();
void commandpipe();


std::string cmd;

int Exploit()
{
	system("CLS");
	std::cout << "Welcome to the Exploit Command Shitter" << std::endl;
	std::cout << "Type cmds for a list of commands!" << std::endl;

	/*
	Now Make a look for the commands/
	*/
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)LuaCpipe, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)commandpipe, 0, 0, 0);

	while (true)
	{
		std::getline(std::cin, cmd);
		hCmd(cmd);
	}
}



void GetRobloxState()
{
	int PointToState = 0;
	DWORD GetDM = RobloxDataModel::getDataModel();
	DWORD ScriptContext = RobloxDataModel::FindFirstClass(GetDM, "ScriptContext");
	rL = RBLX::r_findls(ScriptContext, &PointToState);
	printf("Initilised rL: (%i)\n", rL);
	printf("Initilised DataModel: (%i)\n", GetDM);
	printf("Initilised ScriptContext: (%i)\n", ScriptContext);
}

void DebugConsole(std::string beep)
{
	DWORD OldPerm;
	VirtualProtect(&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &OldPerm);
	*(BYTE*)& FreeConsole = 0xC3;
	VirtualProtect(&FreeConsole, 1, OldPerm, NULL);
	AllocConsole();
	SetConsoleTitleA(beep.c_str());
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
}

int maininit()
{
	DebugConsole("rocock");
	RBLX::Init();
	GetRobloxState();
	
	if (rL)
	{
		Exploit();
	}
	else
		printf("Failed to Initilise! \n"); // fucked this up lol
	return 1;
}


void LuaCpipe() {
	HANDLE hPipe;
	char buffer[1024];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\LuaCpipe"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				/* add terminating zero */
				buffer[dwRead] = '\0';

				LuaC(buffer);
				//runluacline(buffer);
			}
		}
		DisconnectNamedPipe(hPipe);
	}
}

void commandpipe() {
	HANDLE hPipe;
	char buffer[1024];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Commandpipe"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				/* add terminating zero */
				buffer[dwRead] = '\0';

				hCmd(buffer);
				//command(buffer);
			}
		}
		DisconnectNamedPipe(hPipe);
	}
}



BOOL __stdcall DllMain(HINSTANCE Dll, DWORD Reason, LPVOID Reserved)
{
	if (Reason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)maininit, 0, 0, 0);
	}
	if (Reason == DLL_PROCESS_DETACH)
	{
		exit(2);
	}
	return TRUE;
}

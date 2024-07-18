/*
This is essentially memehax's luaC re-written ish
in a way
this was rewritten by ElKoax and will most likely be open sourced,
id just like to say i tried to write this neat as fuck.

and i also plan on adding there parser, which is pretty cool.

------
Credits
------
Eternal (for his retcheck)
ringarang for his splitstring thingy magig
im not sure who the sigscanner is but (credits to the person who made it dunno who did)
and also tepig for giving me a base to start from (memehax's luaC) i just liked the way of the OP_GETGLOBAL etc.
*/
#include "MainInit.h"
#include "theActualluac.h"
#include "bigheckinglua.h"





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

				LuaCExecute(buffer);
			}
		}
		DisconnectNamedPipe(hPipe);
	}
}





void maininit()
{

	CBP("");
	printf("welcome to elkoax's b8 \n");
	// scanning etc will go here
    
	uint32_t ScriptContextVftable = ASLR(0x1D72A30);  // AUto update!
	uintptr_t ScriptContext = Memory2::scan((char*)&ScriptContextVftable);//UPDATE

	int _rL = ScriptContext + 56 * 0 + 164 + *(DWORD *)(ScriptContext + 56 * 0 + 164);
	Roblox = _rL;

	
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)LuaCpipe, NULL, NULL, NULL);
	
	while (true)
	{
		std::string y;
		std::getline(std::cin, y);
		LuaCExecute(y);
	}




	


}



BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		DisableThreadLibraryCalls(hModule);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)maininit, NULL, NULL, NULL);
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH: break;
	}
	return true;
}


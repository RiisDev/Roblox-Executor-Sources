#include "Core.hpp"

DWORD WINAPI LuaPipe(PVOID lvpParameter)
{
	std::string WholeScript = "";
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
					WholeScript = WholeScript + buffer;
				}
				catch (std::exception e) {
				}
				catch (...) {
				}
			}
			Execute(WholeScript);
			WholeScript = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}


void EntryPoint()
{
	CreateConsole("Axon");
	std::cout << "Scanning...";
	DWORD ScriptContext = Scanner::Scan();
	int v39 = ScriptContext;
	int v48 = 0;
	RLS = (v39 + 56 * v48 + 172) ^ *(DWORD*)(v39 + 56 * v48 + 172);
	LS = luaL_newstate();
	Level(RLS, 6);
	BreakPoints();
	luaL_openlibs(LS);
	std::cout << "Complete.\r\n";
	std::cout << "Wrapping Globals...";
	luaL_newmetatable(LS, "garbagecollector");
	lua_pushcfunction(LS, UserDataGC);
	lua_setfield(LS, -2, "__gc");
	lua_pushvalue(LS, -1);
	lua_setfield(LS, -2, "__index");
	WrapRBX(RLS, LS, "printidentity");
	WrapRBX(RLS, LS, "game");
	WrapRBX(RLS, LS, "Game");
	WrapRBX(RLS, LS, "workspace");
	WrapRBX(RLS, LS, "Workspace");
	WrapRBX(RLS, LS, "Axes");
	WrapRBX(RLS, LS, "BrickColor");
	WrapRBX(RLS, LS, "CFrame");
	WrapRBX(RLS, LS, "Color3");
	WrapRBX(RLS, LS, "ColorSequence");
	WrapRBX(RLS, LS, "ColorSequenceKeypoint");
	WrapRBX(RLS, LS, "NumberRange");
	WrapRBX(RLS, LS, "NumberSequence");
	WrapRBX(RLS, LS, "NumberSequenceKeypoint");
	WrapRBX(RLS, LS, "PhysicalProperties");
	WrapRBX(RLS, LS, "Ray");
	WrapRBX(RLS, LS, "Rect");
	WrapRBX(RLS, LS, "Region3");
	WrapRBX(RLS, LS, "Region3int16");
	WrapRBX(RLS, LS, "TweenInfo");
	WrapRBX(RLS, LS, "UDim");
	WrapRBX(RLS, LS, "UDim2");
	WrapRBX(RLS, LS, "Vector2");
	WrapRBX(RLS, LS, "Vector2int16");
	WrapRBX(RLS, LS, "Vector3");
	WrapRBX(RLS, LS, "Vector3int16");
	WrapRBX(RLS, LS, "Enum");
	WrapRBX(RLS, LS, "Faces");
	WrapRBX(RLS, LS, "Instance");
	WrapRBX(RLS, LS, "math");
	WrapRBX(RLS, LS, "warn");
	WrapRBX(RLS, LS, "typeof");
	WrapRBX(RLS, LS, "type");
	WrapRBX(RLS, LS, "spawn");
	WrapRBX(RLS, LS, "Spawn");
	WrapRBX(RLS, LS, "print");
	WrapRBX(RLS, LS, "printidentity");
	WrapRBX(RLS, LS, "ypcall");
	WrapRBX(RLS, LS, "Wait");
	WrapRBX(RLS, LS, "wait");
	WrapRBX(RLS, LS, "delay");
	WrapRBX(RLS, LS, "Delay");
	WrapRBX(RLS, LS, "tick");
	WrapRBX(RLS, LS, "LoadLibrary");
	lua_newtable(LS);
	lua_setglobal(LS, "_G");
	std::cout << "Complete.\r\n";
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)LuaPipe, NULL, NULL, NULL);
	std::cout << "Axon has injected! Updated by Communist Squid.\r\n";
}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved)
{
	if (Reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(Module);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)EntryPoint, NULL, NULL, NULL);
	}
	return TRUE;
}

#include <Windows.h>
#include <string>
#include <iostream>
#include <queue>

#include "LuauSerializer.h"
#include "LuauTranspiler.h"
#include "Retcheck.h"
#include "CallCheck Bypass.h"


#include "Rlua.h"

extern "C" {
#include "Lua\lua.h"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\llimits.h"
#include "Lua\lobject.h"
#include "Lua/llimits.h"
}


void ConsoleBypass(const char* Name)
{
	DWORD AAAAAAAAAAAAAAA;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &AAAAAAAAAAAAAAA);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	SetConsoleTitleA(Name);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}

bool ExecuteScript(DWORD RS, lua_State* L, std::string Script) {
	DWORD xD = r_lua_newthread(RS); // back
	std::string Bytecode = LuauTranspiler::compile(lua_open(), Script);


	Script = "spawn(function() script = Instance.new('LocalScript') script.Disabled = true script.Parent = nil\r\n" + Script + "\r\nend)";
	lua_getglobal(L, "string");
	lua_getfield(L, -1, "dump");
	if (luaL_loadbuffer(L, Script.c_str(), Script.size(), "@aaa"))
	{
		const std::string ERR = lua_tostring(L, -1);
		lua_close(L);
		throw std::exception(ERR.c_str());
	}
	std::cout << "[PushLString]" << std::endl;
	//r_lua_setfield(xD, LUA_GLOBALSINDEX, "SCRIPT_VALUE");
	r_lua_deserialize(xD, "tyo", LuauTranspiler::compile(lua_open(), Script).c_str(), LuauTranspiler::compile(lua_open(), Script).size());
	std::cout << "[Deserialized Bytecode!]" << std::endl;
	lua_pcall(L, 0, 0, 0);
	r_lua_spawn(xD);
	std::cout << "[Spawn]" << std::endl;


	return true;
}

bool ExecuteScript1(DWORD RS, lua_State* L, std::string Script) {
	std::cout << "[Newthread]" << std::endl;
	DWORD xD = r_lua_newthread(RS); // back
	std::cout << "[Newthread Made]" << std::endl;
	std::string Bytecode = LuauTranspiler::compile(lua_open(), Script);
	std::cout << "[LuauTranspiler]" << std::endl;

	Script = "spawn(function() script = Instance.new('LocalScript') script.Disabled = true script.Parent = nil\r\n" + Script + "\r\nend)";
	lua_getglobal(L, "string");
	lua_getfield(L, -1, "dump");
	if (luaL_loadbuffer(L, Script.c_str(), Script.size(), "@aaa"))
	{
		std::cout << "[Error]: " << lua_tostring(L, -1) << "\n";
		lua_pop(L, 1);
	}
	lua_pcall(L, 1, 1, 0);

	size_t scriptsz;
	const auto scriptb = lua_tolstring(L, -1, &scriptsz);
	std::cout << "[PushLString]" << std::endl;
	//r_lua_setfield(xD, LUA_GLOBALSINDEX, "SCRIPT_VALUE");
	r_lua_deserialize(xD, "tyo", LuauTranspiler::compile(lua_open(), Script).c_str(), LuauTranspiler::compile(lua_open(), Script).size());
	std::cout << "[Deserialized Bytecode!]" << std::endl;
	lua_pcall(L, 0, 0, 0);
	r_lua_spawn(xD);
	std::cout << "[Spawn]" << std::endl;
	return true;
}



int DumpString(DWORD rL) {

}






union r_Value
{
	void* gc;
	void* p;
	double n;
	bool b;
};

typedef struct r_TValue
{
	r_Value value;
	byte tt;
	byte shit;
};

int impl_getgc(DWORD rL)
{
	auto IncludeTables = false;
	IncludeTables = r_lua_toboolean(rL, -1);
	const auto GlobalState = rL + 8 + *(DWORD*)(rL + 8); // fucking global state ofc
	const auto DeadMask = *(BYTE*)(GlobalState + 20);
	auto Object = *(GCObject**)(GlobalState + 48); // to do: define these you can remake rluaC_link with these 
	r_lua_createtable(rL, 0, 0);
	auto n = 1;
	while (Object != nullptr)
	{
		const auto TT = *(BYTE*)((DWORD)Object + 5);

		if ((TT == RLUA_TFUNCTION || (IncludeTables ? TT == RLUA_TTABLE : TT == RLUA_TFUNCTION) || (IncludeTables ? TT == RLUA_TUSERDATA : TT == RLUA_TFUNCTION)) && (*(BYTE*)((DWORD)Object + 4) & 3) & DeadMask)
		{
			r_lua_pushnumber(rL, n++);
			r_TValue* top = *(r_TValue**)(rL + 0xC);
			top->tt = RLUA_TFUNCTION;
			top->value.gc = (GCObject*)(TT);
			*reinterpret_cast<uintptr_t*>(rL + 0xC) += sizeof(r_TValue); // modified specially for it to WORK on fucking getgc, was this that was causing the problem the whole time
			r_lua_settable(rL, -3);
		}
		Object = Object->gch.next;
	}
	return 1;
}




int fireclicka(DWORD rL)
{




	int CDectector = *(DWORD*)r_lua_touserdata(rL, 1);

	//if (CDectector.GetInstanceClassName() != "ClickDetector")
	//    return RL.ArgError(1, "userdata<ClickDetector> expected");
	//
	//if (!RL.IsNoneOrNil(2) && !RL.IsNumber(2))
	//    return RL.ArgError(2, "Variant<none, number> expected");

	float Dist = 0.0;
	if (r_lua_gettop(rL) > 1)
		Dist = (float)r_lua_tonumber(rL, 2, 0);

	//RL.GetGlobal("game");
	//RL.GetField(-1, "GetService");
	//RL.Insert(-2);
	//RL.PushString("Players");
	//RL.PCall(2, 1, 0);
	//RL.GetField(-1, "LocalPlayer");
	r_lua_getfield(rL, -10002, "game");
	r_lua_getfield(rL, -1, "GetService");
	r_lua_pushvalue(rL, -2);
	r_lua_pushstring(rL, "Players");
	r_lua_pcall(rL, 2, 1, 0);
	r_lua_getfield(rL, -1, "LocalPlayer");



	DWORD Plr = *(DWORD*)r_lua_touserdata(rL, -1);
	static DWORD FnFire = ASLR(0xBABB90);

	((void(__thiscall*)(DWORD, float, DWORD))FnFire)(CDectector, Dist, Plr);



	return 0;
}





int RenderstepHook(DWORD rL)
{
	if (!ScriptQueue.empty()) {
		auto script = ScriptQueue.front();
		ExecuteScript(rL, L, script);
		ScriptQueue.pop();
	}
	return 1;
}

void Execute(const std::string& Script) {
	if (Script != "") {
		ScriptQueue.push(Script);
	}
}

int exepipelmao() {
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;


	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Flame"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		1,
		9999999,
		9999999,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';

				if (strlen(buffer) != NULL) {
					ScriptQueue.push(buffer);
				}
			}
		}

		DisconnectNamedPipe(hPipe);
	}
}

void SetLevel(DWORD RobloxState, int Level)
{
	int v3 = *(DWORD*)(RobloxState + EmptySpace1);
	*(DWORD*)(v3 + EmptySpace2) = Level;
}


void Thread() {
	ConsoleBypass("Imperious Luau Transpiler");

	DWORD DataModel = ReturnDataModel();
	DWORD ScriptContext = FindChildOfInstance(DataModel, "ScriptContext");
	RL = r_lua_newthread(ReturnState(ScriptContext));
	DWORD Thread = r_lua_newthread(RL);
	L = luaL_newstate();
	luaL_openlibs(L);
	SetLevel(RL, 6);

	BypassCallCheck();

	printf("[System]");
	std::cout << "Hooked , Level 6 Created" << std::endl;

	r_luaL_register(RL, "fireclickdetector", fireclicka);
	r_luaL_register(RL, "getgc", impl_getgc);



	r_lua_getfield(RL, LUA_GLOBALSINDEX, "game");
	r_lua_getfield(RL, -1, "GetService");
	r_lua_pushvalue(RL, -2);
	r_lua_pushstring(RL, "RunService");
	r_lua_pcall(RL, 2, 1, 0);
	r_lua_getfield(RL, -1, "RenderStepped");
	r_lua_getfield(RL, -1, "Connect");
	r_lua_pushvalue(RL, -2);
	r_lua_pushcclosure(RL, (DWORD)RenderstepHook, 0);
	r_lua_pcall(RL, 2, 0, 0);
	r_lua_pop(RL, 2);

	std::cout << "[Thread Schel]" << std::endl;

	//Renderer::Hook();
	//Drawing::Register(RL);
	//r_lua_settop(RL, 0);

	CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(exepipelmao), NULL, NULL, NULL);


	while (true) {
		std::string Scriptt = "";
		std::getline(std::cin, Scriptt);
		ExecuteScript(RL, L, Scriptt);
	}
}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved) {
	if (Reason == DLL_PROCESS_ATTACH) {
		CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Thread), NULL, NULL, NULL);
	}
	return TRUE;
}
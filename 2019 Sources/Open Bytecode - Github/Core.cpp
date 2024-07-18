#include "RBXLua.hpp"
#include "Memory.hpp"
#include <fstream>

extern "C" {
#include "Lua/lua.h"
#include "Lua/lauxlib.h"
#include "Lua/lualib.h"
#include "Lua/lopcodes.h"
#include "Lua/lobject.h"
#include "Lua/ldo.h"
#include "Lua/lstate.h"
}

#define Log(...) printf("[%s] -> ", __FUNCTION__); printf(__VA_ARGS__)


using namespace RBX; // ew

void r_execute(std::uintptr_t rL, std::string source)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	luaL_loadbuffer(L, source.c_str(), source.length(), "=r_script");

	lua_getglobal(L, "string");
	lua_getfield(L, -1, "dump");
	lua_pushvalue(L, -3);
	lua_pcall(L, 1, 1, 0);
	lua_setglobal(L, "buffer");
	lua_pop(L, 1);

	lua_getglobal(L, "string");
	lua_getfield(L, -1, "len");
	lua_getglobal(L, "buffer");
	lua_pcall(L, 1, 1, 0);
	size_t size = lua_tointeger(L, -1);
	lua_pop(L, 1);

	lua_getglobal(L, "buffer");
	uint8_t* code = new uint8_t[size];
	memcpy(code, lua_tolstring(L, -1, &size), size);
	lua_pop(L, 1);

	// rerubi converts vanilla bytecode to roblox lua
	// instructions, and does this on their VM.
	// this means it's really sexy compared to most
	// executors! :-)/.
	r_lua_getfield(rL, -10002, "RunScript");
	r_lua_pushlstring(rL, (const char*)code, size);
	r_lua_pcall(rL, 1, 0, 0);

	free(code);
}

void ENTRY()
{

	MemUtil::Write(FreeConsole, new byte{ 0xC3 }, 1);
	AllocConsole();

	freopen("conout$", "w", stdout);
	freopen("conin$", "r", stdin);

	std::cout << "[DEBUG] Initializing and Scanning.. " << std::endl;
	std::cout << "[DEBUG] Scanning for Script 'Context' " << std::endl;

	Log("Grabbing 'Script Context'\n");
	uintptr_t ScriptContext = Memory::scan((char*)&ScriptContextVFtable);//UPDATE
	Log("Got 'Script Context', Address of the Console Output is: %x\n", ScriptContext);
	Log("Initializing Lua State\n");

	std::uintptr_t rL = ScriptContext + 56 * 0 + 164 ^ *(DWORD*)(ScriptContext + 56 * 0 + 164);

	std::ifstream ifs("bytecode.bin", std::ios::binary);
	std::string bytecode((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	ifs.close();

	int err = deserialize(rL, "@", bytecode.c_str(), bytecode.size());
	r_lua_pcall(rL, 0, -1, 0);

	Log("Read File 'bytecode.bin'\n");
	Log("Ready to execute!\n");

	std::string line;
	while (std::getline(std::cin, line))
	{
		r_execute(rL, line);
		line = "";
	}

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	//DisableThreadLibraryCalls(hModule);
	if (dwReason == DLL_PROCESS_ATTACH)
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ENTRY, 0, 0, 0);
	return 1;
}
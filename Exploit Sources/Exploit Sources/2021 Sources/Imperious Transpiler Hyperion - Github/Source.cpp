#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <string>
#include <iostream>
#include <queue>

#include "LuauSerializer.h"
#include "LuauTranspiler.h"
#include "Retcheck.h"
#include "CallCheck Bypass.h"
#include "CreativeMode.h"
#include "curl.h"
#include "BytecodeFunctions.h"
#include "GlobalCore.h"

extern "C" {
#include "Lua\lua.h"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\llimits.h"
#include "Lua\lobject.h"
#include "Lua/llimits.h"
#include "Tools.h"
}

//const char* ExploitName = "Skycloud-Exploit"; //Change this to your exploit name or the name you want.

std::string Function_Table = random_string(100);

std::string CheckForFunction(DWORD rL, std::string Script) {
	std::vector<std::string> Functions = {
		"writefile", "readfile", "loadfile", "appendfile", "getthreadidentity",
		"setclipboard", "reportkick", "getgc", "getgenv", "getrenv", "newcclosure",
		"getnamecallmethod", "islclosure", "iscclosure", "getrawmetatable", "setreadonly",
		"fireclickdetector", "is_rainfall_function", "is_rainfall_closure", "setthreadidentity",
		"make_readonly", "isreadonly", "make_writeable", "setnamecallmethod",
		"isrbxactive", "bit", "debug", "Drawing", "keypress", "keyrelease", "keytap", "leftpress",
		"leftrelease", "rightpress", "rightRelease", "leftclick", "rightclick", "mousescroll","mousemoverel",
		"hookfunction", "setrawmetatable", "checkcaller", "getsenv", "getmenv", "decompile", "LoadLibrary"
	};
	for (std::string& Ex : Functions) {
		if (Script.find(Ex) != std::string::npos) {
			std::cout << "Caught function: " << Ex << std::endl;
			Script = replaceAll(Script, Ex, Function_Table + "." + Ex);
			Script = replaceAll(Script, Function_Table + ".debug." + Function_Table + ".getregistry", Function_Table + ".debug.getregistry");
		}
	}
	return Script;
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

bool SendExecution(std::string Script) {
	Script = replaceAll(Script, "game:HttpGet", "HttpGet");
	Script = replaceAll(Script, "game:httpget", "HttpGet");
	Script = replaceAll(Script, "game:Objects", "GetObjects");
	std::string Source = CheckForFunction(RL, Script);
	if (luaL_loadbuffer(L, Source.c_str(), Source.size(), "@PandaTranspiler")) {
		std::cout << lua_tostring(L, -1) << std::endl;
	}
	else {
		DWORD scriptThread = r_lua_newthread(RL);
		const auto bytecode = pandatranspiler::script_to_bytecode(lua_open(), Source, noobs);

		auto ret = r_lua_deserialize(scriptThread, ExploitName, bytecode.c_str(), bytecode.size(), 0);
		r_lua_spawn(scriptThread);
	}
	return true;
}

//bool ExecuteScript(DWORD RS, lua_State* L, std::string Script) {
//	Script = replaceAll(Script, "game:HttpGet", Function_Table + "." + "HttpGet");
//	Script = replaceAll(Script, "game:Objects", Function_Table + "." + "GetObjects");
//	DWORD PandaBytes = r_lua_newthread(RS); // back
//	std::string Bytecode = LuauTranspiler::compile(lua_open(), Script);
//
//
//	Script = "spawn(function() script = Instance.new('LocalScript') script.Disabled = true; script.Parent = nil\r\n" + Script + "\r\nend)";
//	lua_getglobal(L, "string");
//	lua_getfield(L, -1, "dump");
//	if (luaL_loadbuffer(L, Script.c_str(), Script.size(), "@aaa"))
//	{
//		const std::string ERR = lua_tostring(L, -1);
//		lua_close(L);
//		throw std::exception(ERR.c_str());
//	}
//	//r_lua_setfield(xD, LUA_GLOBALSINDEX, "SCRIPT_VALUE");
//	r_lua_deserialize(PandaBytes, ExploitName, LuauTranspiler::compile(lua_open(), Script).c_str(), LuauTranspiler::compile(lua_open(), Script).size());
//	lua_pcall(L, 0, 0, 0);
//	r_lua_spawn(PandaBytes);
//
//
//	return true;
//}
//
//bool ExecuteScript1(DWORD RS, lua_State* L, std::string Script) {
//	//std::cout << "[Newthread]" << std::endl;
//	DWORD PandaBytes = r_lua_newthread(RS); // back
//	//std::cout << "[Newthread Made]" << std::endl;
//	std::string Bytecode = LuauTranspiler::compile(lua_open(), Script);
//	//std::cout << "[LuauTranspiler]" << std::endl;
//	Script = "spawn(function() script = Instance.new('LocalScript') script.Disabled = true script.Parent = nil\r\n" + Script + "\r\nend)";
//	lua_getglobal(L, "string");
//	lua_getfield(L, -1, "dump");
//	if (luaL_loadbuffer(L, Script.c_str(), Script.size(), "@FuckingPanda"))
//	{
//		std::cout << "[Error]: " << lua_tostring(L, -1) << "\n";
//		lua_pop(L, 1);
//	}
//	lua_pcall(L, 1, 1, 0);
//
//	size_t scriptsz;
//	const auto scriptb = lua_tolstring(L, -1, &scriptsz);
//	std::cout << iblue << "[" << red << " Panda Debug " << iblue << "]" << iblue << " " << " Pushing PushLString \r\n";
//	//r_lua_setfield(xD, LUA_GLOBALSINDEX, "SCRIPT_VALUE");
//	r_lua_deserialize(PandaBytes, "tyo", LuauTranspiler::compile(lua_open(), Script).c_str(), LuauTranspiler::compile(lua_open(), Script).size());
//	std::cout << "[]" << std::endl;
//	std::cout << iblue << "[" << red << " Panda Debug " << iblue << "]" << iblue << " " << " Deserialized Bytecode! \r\n";
//	lua_pcall(L, 0, 0, 0);
//	r_lua_spawn(PandaBytes);
//	std::cout << iblue << "[" << red << " Panda Debug " << iblue << "]" << iblue << " " << " Spawn! \r\n";
//	return true;
//}

////////////////////////////////////////////////////////////////////////////

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


int RenderstepHook(DWORD rL)
{
	if (!ScriptQueue.empty()) {
		auto script = ScriptQueue.front();
		SendExecution(script);
		ScriptQueue.pop();
	}
	return 1;
}


////////////////////////////////////////////////////////////////////////////
void Execute(const std::string& Script) {
	if (Script != "") {
		ScriptQueue.push(Script);
	}
}

void SetLevel(DWORD RobloxState, int Level)
{
	int v3 = *(DWORD*)(RobloxState + EmptySpace1);
	*(DWORD*)(v3 + EmptySpace2) = Level;
}

DWORD SetThreadIdentity(DWORD rL, int level)
{
	return *(DWORD*)(*(DWORD*)(rL + Identity2) + 24) = level;
}

DWORD WINAPI Lua(PVOID lpv_param)
{
	auto Bufferize = std::make_unique<char>(MAX_INT).get();
	std::string Script;
	DWORD ReadPipe;
	//std::string EntryPointPipe = xorstr("\\\\.\\pipe\\").crypt_get();
	//std::string RealPipeEntryPoint = xorstr(PipeName).crypt_get();
	//std::string PipeNamed = EntryPointPipe + std::string(PipeName);
	auto HandlePipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\SkycloudRelived"), PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, PIPE_WAIT, 1, 999999, 999999, NMPWAIT_USE_DEFAULT_WAIT, NULL);
	while (HandlePipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(HandlePipe, NULL) != FALSE)
		{
			while (ReadFile(HandlePipe, Bufferize, sizeof(Bufferize) - 1, &ReadPipe, NULL) != FALSE)
			{
				Bufferize[ReadPipe] = '\0';
				Script = Script + Bufferize;
			}
			Execute(Script);
			Script = "";
		}
		DisconnectNamedPipe(HandlePipe);
	}
}

void LoadFunctions()
{
	r_luaL_register(RL, "getgc", impl_getgc);
	r_luaL_register(RL, "loadstring", loadstring);
	r_luaL_register(RL, "GetObjects", getObjects_impl);
	r_luaL_register(RL, "HttpGetAsync", HttpGet_impl);
	r_luaL_register(RL, "HttpGet", HttpGet_impl);
	r_luaL_register(RL, "game:httpgetasync", HttpGet_impl);
	r_luaL_register(RL, "game:HttpGetAsync", HttpGet_impl);
	r_luaL_register(RL, "game:httpget", HttpGet_impl);
	r_luaL_register(RL, "game:HttpGet", HttpGet_impl);
	r_luaL_register(RL, "getrenv", getenv_impl);
	r_luaL_register(RL, "getgenv", getenv_impl);
	r_luaL_register(RL, "getreg", getreg_impl);
	r_luaL_register(RL, "setreadonly", setreadonly_impl);
	r_luaL_register(RL, "isreadonly", isreadonly_impl);
	r_luaL_register(RL, "checkcaller", checkcaller_impl);
	r_luaL_register(RL, "require", require);
	r_luaL_register(RL, "isluau", isluau);
	r_luaL_register(RL, "is_luau", isluau);
	//RegisterFunction(FunctionENV::getgc_impl, "getgc");
	r_luaL_register(RL, "getfflag", getfflag);
	r_luaL_register(RL, "getrawmetatable", getrawmetatable_impl);
	r_luaL_register(RL, "getnamecallmethod", getnamecallmethod_impl);
	r_luaL_register(RL, "mouse1click", mouse1click_impl);
	r_luaL_register(RL, "mouse2click", mouse2click_impl);
	r_luaL_register(RL, "mouse1press", mouse1press_impl);
	r_luaL_register(RL, "mouse2press", mouse2press_impl);
	r_luaL_register(RL, "mouse1release", mouse1release_impl);
	r_luaL_register(RL, "mouse2release", mouse2release_impl);
	r_luaL_register(RL, "mousemoverel", mousemoverel_impl);
	r_luaL_register(RL, "mousemoveabs", mousemoveabs_impl);
	r_luaL_register(RL, "keypress", keypress_impl);
	r_luaL_register(RL, "fireclickdetector", fireclickdetector);
}

void Bingo() 
{
	try
	{
	std::string meme = ExploitConsoleName;
	std::string titlez = meme.c_str();
	std::string title2 = titlez + " [ Bytecode Initialization ]";
	ConsoleBypass(title2.c_str());
	std::cout << iblue << "[" << red << titlez + " Panda Client " << iblue << "]" << igreen << " " << "__________________________________________________\n";
	std::cout << iblue << "[" << red << titlez + " Panda Client " << iblue << "]" << iblue << " " << " Initializing Bytecode Conversion!... \r\n";
	DWORD DataModel = ReturnDataModel();
	DWORD ScriptContext = FindChildOfInstance(DataModel, "ScriptContext");
	RL = r_lua_newthread(RBX_LuaState(ScriptContext));
	DWORD Thread = r_lua_newthread(RL);
	L = luaL_newstate();
	std::cout << iblue << "[" << red << titlez + " Panda Client " << iblue << "]" << iblue << " " << " Opening Libraries \r\n";
	luaL_openlibs(L);
	std::cout << iblue << "[" << red << titlez + " Panda Client " << iblue << "]" << iblue << " " << " Setting Up Roblox Level \r\n";
	SetLevel(RL, 7);
	SetThreadIdentity(RL, 7);
	std::cout << iblue << "[" << red << titlez + " Panda Client " << iblue << "]" << iblue << " " << " Bypassing CallCheck...";
	BypassCallCheck();
	std::cout << green << "Success...\r\n";
	std::cout << iblue << "[" << red << " Panda Client " << iblue << "]" << igreen << " " << "__________________________________________________\n";
	std::cout << iblue << "[" << red << " Panda Client " << iblue << "]" << iblue << " " << " Registering Functions...\n";
	//
	LoadFunctions(); //ofc registers your gay functions


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
	//CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(exepipelmao), NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Lua, NULL, NULL, NULL);
	std::cout << iblue << "[" << red << titlez + " Panda Client " << iblue << "]" << iblue << " " << " Inject Complete.. A Debug Mode has been Enable which makes this Console Always Shows.\n";
	//std::string watermark = "loadstring(game:httpget('https://raw.githubusercontent.com/SkieAdmin/PandaHub/master/PandaWelcome.lua'))()";
	//SendExecution(watermark); //Panda Official
	SendExecution("print('Working with Hyperion Transpiler')"); // No Reason to Add

	}
	catch (const std::exception& e)
	{
		std::cout << "Panda is likely not updated: " << e.what() << std::endl;
	}
}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved) {
	if (Reason == DLL_PROCESS_ATTACH) {
		IsTainedBypass();
		CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Bingo), NULL, NULL, NULL);
	}
	return TRUE;
}
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>
#include <optional>
#include <string_view>
#include <utility>
#include <TlHelp32.h>
#include <stdio.h>
#include <sstream>
#include <tchar.h>
#include "Execution.hpp"
#include "Teleport-Handler1.h"
#include "SF_dumper.cpp"
#include <fstream>
#include <ShlObj.h>
#include <KnownFolders.h>
#include <wchar.h>
#include <chrono>

void int_s()
{
	std::string Script = R"--(

		print("start Executing httpgethook...")

		spwan(function()

			local gmt = getrawmetatable(game)
			local oldNamecall = gmt.__namecall
			local oldIndex = gmt.__index

			setreadonly(gmt, false)

			gmt.__namecall = newcclosure(function(self, ...)
				if self == game and getnamecallmethod() == 'HttpGet' then
					return HttpGet(...)
				end
				return oldNamecall(self, ...)
			end)

			gmt.__index = newcclosure(function(self, index)
				if self == game and index == 'HttpGet' then
					return function(_, ...)
						return game:HttpGet(...)
					end
				end
				return oldIndex(self, index)
			end)

		end)

		setreadonly(gmt, true)
		
		loadstring(print("loadstring Function Working!"))()

		--loadstring(game:HttpGet("https://raw.githubusercontent.com/EdgeIY/infiniteyield/master/source"))()

	)--";

	Execution::run_script(exthread, (lua_State*)luaL_newstate(), Script);
} 

void print_f(int type , std::string msg)
{
	Addresses::r_lua_print(type, msg.c_str());
	Sleep(100);
}

int main()
{
	print_f(0, "YT: Start");

	exthread = (uintptr_t)LuaState::get_robloxstate();
	print_f(0, "pass: get_robloxstate");

	Lua::setlevel(exthread, 8);
	print_f(0, "Pass: Bypass LVL!");

	std::thread teleport_handler_thread(start_in);
	teleport_handler_thread.detach();
	print_f(0, "Pass: teleport_handler_thread!");

	autoexec();
	print_f(0, "Pass: AutoExec!");

	//int_s();
	//Sleep(1200);

	Execution::run_script(exthread, (lua_State*)luaL_newstate(), "printidentity()");
	Execution::run_script(exthread, (lua_State*)luaL_newstate(), "print('Ready')");

	std::thread(Execution::Execute_Function).detach();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		std::thread(main).detach();
	}
	return TRUE;
}

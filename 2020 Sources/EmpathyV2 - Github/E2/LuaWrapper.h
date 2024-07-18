#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Offsets.h"
#include <Wininet.h>
#include "ConsoleCol.h"
#pragma comment(lib, "wininet.lib")
int registry;
std::vector<int> int3breakpoints;
bool __dump = false;
namespace Wrapper {
	void Wrap(DWORD RLS, lua_State* LS, int Index);
	void UnWrap(lua_State* LS, DWORD RLS, int Index);
	static int RobloxYielding(DWORD RLS);
	int VanillaFunctionHandler(lua_State* LS);
	int RobloxFunctionHandler(DWORD RLS);
	std::string RLUA = "";
	std::string TLUA = "";
}

struct Userdata
{
	int32_t reference;
};

struct DataModelUserData {
	int ref;
};

std::string replace_all1(std::string subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

namespace Wrapper {
	void ExecuteScript(std::string Script) {

		std::string buf;
		buf += "spawn(function()\r\n";
		buf += Script;
		buf += "\r\nend)";
		if (luaL_loadstring(LS, buf.c_str())) {
			std::cout << ired << "LUA_ERROR : " << white << lua_tostring(LS, -1) << "\n";
			lua_pop(LS, 1);
			return;
		}
		else {
			lua_pcall(LS, 0, LUA_MULTRET, 0);
		}
		//UserDataGC(LS);
		////////////////////////////////////////////////////////
	}
}

LONG WINAPI vehHandler(PEXCEPTION_POINTERS ex)
{
	switch (ex->ExceptionRecord->ExceptionCode)
	{
	case (DWORD)0x80000003L:
	{
		if (ex->ContextRecord->Eip == int3breakpoints[0])
		{
			ex->ContextRecord->Eip = (DWORD)Wrapper::RobloxFunctionHandler;
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		if (ex->ContextRecord->Eip == int3breakpoints[1])
		{
			ex->ContextRecord->Eip = (DWORD)(Wrapper::RobloxYielding);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		return -1;
	}
	default: return 0;
	}
	return 0;
}

DWORD locateINT3()
{
	DWORD _s = x(0x400000);
	const char i3_8opcode[8] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
	for (int i = 0; i < MAX_INT; i++)
	{
		if (memcmp((void*)(_s + i), i3_8opcode, sizeof(i3_8opcode)) == 0)
		{
			return (_s + i);
		}
	}
	return NULL;
}

VOID VehHandlerpush()
{
	int3breakpoints.push_back(locateINT3());
	int3breakpoints.push_back(locateINT3());
	AddVectoredExceptionHandler(1, vehHandler);
}


std::string replace(std::string subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

std::string DownloadURL(const char* url) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET url_file;
	std::string rtn;

	if (interwebs) {
		url_file = InternetOpenUrlA(interwebs, url, NULL, NULL, NULL, NULL);

		if (url_file) {
			char buffer[2000];
			DWORD bytes_read;

			do {
				InternetReadFile(url_file, buffer, 2000, &bytes_read);
				rtn.append(buffer, bytes_read);
				memset(buffer, 0, 2000);
			} while (bytes_read);

			InternetCloseHandle(interwebs);
			InternetCloseHandle(url_file);
			std::string p = replace(rtn, "|n", "\r\n");
			return p;
		}
	}

	InternetCloseHandle(interwebs);
	std::string p = replace(rtn, "|n", "\r\n");
	return p;
}

int custom_httpget(lua_State* L) {
	int args = lua_gettop(L) - 1;
	std::string url;
	if (args == 2) {
		url = DownloadURL(lua_tostring(L, -2));
	}
	else {
		url = DownloadURL(lua_tostring(L, -1));
	}
	lua_pushstring(L, url.c_str());
	return 1;
}

static int Custom_GetObjects(lua_State* L) {
	auto asset = lua_tostring(L, -1);
	auto hold = std::string("return {game:GetService('InsertService'):LoadLocalAsset('") + std::string(asset) + std::string("')}");
	luaL_dostring(L, hold.c_str());

	/*
	std::string asset = lua_tostring(L, -1);
	std::cout << asset << "\n";
	r_lua_getfield(RLS, LUA_GLOBALSINDEX, "game");
	r_lua_getfield(RLS, -1, "InsertService");
	r_lua_getfield(RLS, -1, "LoadLocalAsset");
	//r_lua_pushvalue(RLS, -2);
	r_lua_pushstring(RLS, asset.c_str());
	r_lua_pcall(RLS, 2, 1, 0);*/
	return 1;
}



int RobloxGlobalIndex(lua_State* L)
{
	const char* key = lua_tostring(L, -1);
	//std::cout << key << std::endl;
	if (key == std::string("HttpGet")) {
		lua_pushcfunction(L, custom_httpget);
	}
	else if (key == std::string("GetObjects")) {
		lua_pushcfunction(L, Custom_GetObjects);
	}
	else
	{
		//DebugOutput("Index Key: ", key);
		Wrapper::UnWrap(L, RLS, 1);
		r_lua_getfield(RLS, -1, key);
		Wrapper::Wrap(RLS, L, -1);
		//r_lua_settop(RLS, -2);
	}
	return 1;
}

int RobloxGC(lua_State* L) {
		void* UserObject = lua_touserdata(L, 1);
		if (L) {
			lua_pushvalue(L, 1);
			if (!lua_isnil(L, -1)) {
				int LRef = lua_tointeger(L, -1);

				r_lua_pushnil(RLS);
				r_lua_rawseti(RLS, LUA_REGISTRYINDEX, LRef);
				r_lua_settop(RLS, 0);
			}
			lua_pop(L, 1);
		}
		r_lua_settop(RLS, 0);
		return 0;
}

namespace Wrapper {
	void Wrap(DWORD RLS, lua_State* LS, int Index) {
		/*switch (r_lua_type(RLS, Index))
		{
		case 0:
			std::cout << ired << "Wrap: " << iblue << "R_LUA_TNIL" << "\n";
			break;
		case 1:
			std::cout << ired << "Wrap: " << iblue << "R_LUA_TBOOLEAN" << "\n";
			break;
		case 3:
			std::cout << ired << "Wrap: " << iblue << "R_LUA_TNUMBER" << "\n";
			break;
		case 4:
			std::cout << ired << "Wrap: " << iblue << "R_LUA_TSTRING" << "\n";
			break;
		case 5:
			std::cout << ired << "Wrap: " << iblue << "R_LUA_TTABLE" << "\n";
			break;
		case 6:
			std::cout << ired << "Wrap: " << iblue << "R_LUA_TFUNCTION" << "\n";
			break;
		case 7:
			std::cout << ired << "Wrap: " << iblue << "R_LUA_TUSERDATA" << "\n";
			break;
		case 8:
			std::cout << ired << "Wrap: " << iblue << "R_LUA_TTHREAD" << "\n";
			break;
		}*/

		if (r_lua_type(RLS, Index) == R_LUA_TNIL) {
			lua_pushnil(LS);
		}
		else if (r_lua_type(RLS, Index) == R_LUA_TNUMBER) {
			lua_pushnumber(LS, r_lua_tonumber(RLS, Index));

		}
		else if (r_lua_type(RLS, Index) == R_LUA_TBOOLEAN) {
			lua_pushboolean(LS, r_lua_toboolean(RLS, Index));

		}
		else if (r_lua_type(RLS, Index) == R_LUA_TSTRING) {
			lua_pushstring(LS, r_lua_tostring(RLS, Index));
		}
		else if (r_lua_type(RLS, Index) == R_LUA_TTHREAD) {
			lua_newthread(LS);
		}
		else if (r_lua_type(RLS, Index) == R_LUA_TFUNCTION) {
			r_lua_pushvalue(RLS, Index);
			lua_pushnumber(LS, r_luaL_ref(RLS, LUA_REGISTRYINDEX));
			lua_pushcclosure(LS, VanillaFunctionHandler, 1);
		}
		else if (r_lua_type(RLS, Index) == R_LUA_TTABLE) {
			r_lua_pushvalue(RLS, Index);
			lua_newtable(LS);
			r_lua_pushnil(RLS);

			while (r_lua_next(RLS, -2) != R_LUA_TNIL)
			{
				Wrapper::Wrap(RLS, LS, -2);
				Wrapper::Wrap(RLS, LS, -1);
				lua_settable(LS, -3);
				r_lua_pop(RLS, 1);
			}
			r_lua_pop(RLS, 1);
		}
		else if (r_lua_type(RLS, Index) == R_LUA_TUSERDATA) {
			uintptr_t rawInstancePtr = r_lua_touserdata(RLS, Index);
			lua_rawgeti(LS, LUA_REGISTRYINDEX, rawInstancePtr);

			if (!lua_type(LS, -1)) {
				lua_settop(LS, -2);
				r_lua_pushvalue(RLS, Index);
				reinterpret_cast<Userdata*>(lua_newuserdata(LS, sizeof(Userdata)))->reference = r_luaL_ref(RLS, -10000);
				r_lua_getmetatable(RLS, Index);
				Wrapper::Wrap(RLS, LS, -1);
				lua_pushcfunction(LS, RobloxGlobalIndex);
				lua_setfield(LS, -2, "__index");
				lua_setmetatable(LS, -2);
				lua_pushvalue(LS, -1);
				lua_rawseti(LS, -10000, rawInstancePtr);
				r_lua_settop(RLS, -2);
			}
		}
	}
	void UnWrap(lua_State* LS, DWORD RLS, int Index) {
		/*switch (lua_type(LS, Index))
		{
		case 0:
			std::cout << ired << "UnWrap: " << iblue << "R_LUA_TNIL" << "\n";
			break;
		case 1:
			std::cout << ired << "UnWrap: " << iblue << "R_LUA_TBOOLEAN" << "\n";
			break;
		case 3:
			std::cout << ired << "UnWrap: " << iblue << "R_LUA_TNUMBER" << "\n";
			break;
		case 4:
			std::cout << ired << "UnWrap: " << iblue << "R_LUA_TSTRING" << "\n";
			break;
		case 5:
			std::cout << ired << "UnWrap: " << iblue << "R_LUA_TTABLE" << "\n";
			break;
		case 6:
			std::cout << ired << "UnWrap: " << iblue << "R_LUA_TFUNCTION" << "\n";
			break;
		case 7:
			std::cout << ired << "UnWrap: " << iblue << "R_LUA_TUSERDATA" << "\n";
			break;
		case 8:
			std::cout << ired << "UnWrap: " << iblue << "R_LUA_TTHREAD" << "\n";
			break;
		}*/
		if (lua_type(LS, Index) == LUA_TNIL) {
			r_lua_pushnil(RLS);
		}
		else if (lua_type(LS, Index) == LUA_TNUMBER) {
			r_lua_pushnumber(RLS, lua_tonumber(LS, Index));
		}
		else if (lua_type(LS, Index) == LUA_TBOOLEAN) {
			r_lua_pushboolean(RLS, lua_toboolean(LS, Index));
		}
		else if (lua_type(LS, Index) == LUA_TSTRING) {
			r_lua_pushstring(RLS, lua_tostring(LS, Index));
		}
		else if (lua_type(LS, Index) == LUA_TTHREAD) {
			r_lua_newthread(RLS);
		}
		else if (lua_type(LS, Index) == LUA_TFUNCTION) {
			lua_pushvalue(LS, Index);
			r_lua_pushnumber(RLS, luaL_ref(LS, LUA_REGISTRYINDEX));
			r_lua_pushcclosure(RLS, int3breakpoints[0], 1);
		}
		else if (lua_type(LS, Index) == LUA_TTABLE) {
			lua_pushvalue(LS, Index);
			r_lua_newtable(RLS);
			lua_pushnil(LS);
			while (lua_next(LS, -2) != LUA_TNIL)
			{
				Wrapper::UnWrap(LS, RLS, -2);
				Wrapper::UnWrap(LS, RLS, -1);
				r_lua_settable(RLS, -3);
				lua_pop(LS, 1);
			}
			lua_pop(LS, 1);
		}
		else if (lua_type(LS, Index) == LUA_TUSERDATA) {
			if (!r_lua_type(RLS, -1))
			{
				r_lua_settop(RLS, -2);
			}
			r_lua_rawgeti(RLS, -10000, reinterpret_cast<Userdata*>(lua_touserdata(LS, Index))->reference);
		}
	}

}

void WrapGlobal(DWORD RLS, lua_State* LS, const char* global)
{
	r_lua_getglobal(RLS, global);
	Wrapper::Wrap(RLS, LS, -1);
	lua_setglobal(LS, global);
	r_lua_pop(RLS, 1);
}

std::string randomstringg(int length) {
	std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string ppj;
	int pos = 0;
	while (pos != length) {
		int ppk = ((rand() % (str.size() - 1)));
		ppj = ppj + str[ppk];
		pos += 1;
	}
	return ppj.c_str();
}


void ExecuteScript2(std::string Script) {
	int script[] = { NULL,NULL };
	SandBoxThread(RLS, 6, (int)script);
	/////////////////////////////////////////////////////////
	std::string buf;
	buf += "spawn(function()\r\n";
	buf += Script;
	buf += "\r\nend)";
	if (luaL_loadbuffer(LS, buf.c_str(), buf.size(), "@YungCindyy")) {
		std::cout << ired << "LUA_ERROR : " << white << lua_tostring(LS, -1) << "\n";
		lua_pop(LS, 1);
		return;
	}
	else {
		lua_pcall(LS, 0, LUA_MULTRET, 0);
	}
	//UserDataGC(LS);
	////////////////////////////////////////////////////////
}

namespace Wrapper {
	static int VanillaYield(lua_State* thread)
	{
		lua_State* L = lua_tothread(thread, lua_upvalueindex(1));
		const int nargs = lua_gettop(thread);
		lua_xmove(thread, L, nargs);
		return lua_resume(L, nargs);
	}

	static int RobloxYielding(DWORD thread) {
		lua_State* L = (lua_State*)r_lua_touserdata(thread, lua_upvalueindex(1));
		const int nargs = r_lua_gettop(thread);
		for (int arg = 1; arg <= nargs; ++arg)
			Wrapper::Wrap(thread, L, arg);
		return lua_resume(L, nargs);
		lua_close(L);
	}

	int VanillaFunctionHandler(lua_State* L)
	{
		r_lua_pushstring(RLS, std::to_string(++registry).c_str());
		DWORD rL = r_lua_newthread(RLS);
		r_lua_settable(RLS, LUA_REGISTRYINDEX);
		int key = lua_tonumber(L, lua_upvalueindex(1));
		r_lua_rawgeti(rL, LUA_REGISTRYINDEX, key);

		for (int arg = 1; arg <= lua_gettop(L); ++arg)
			Wrapper::UnWrap(L, rL, arg);

		if (!r_lua_pcall(rL, lua_gettop(L), LUA_MULTRET, 0)) {
			int args = 0;
			for (int arg = 1; arg <= r_lua_gettop(rL); ++arg, ++args) {
				Wrap(rL, L, arg);
			}
			r_lua_settop(rL, 0);
			return args;
		}
		else {

			r_lua_pop(rL, 1);
			lua_pushthread(L);
			lua_pushcclosure(L, &VanillaYield, 1);
			UnWrap(L, rL, -1);
			return lua_yield(L, 0);
		}
		return 0;
	}

	int RobloxFunctionHandler(DWORD rL) {
		lua_pushstring(LS, std::to_string(++registry).c_str());
		lua_State* L = lua_newthread(LS);
		lua_settable(LS, LUA_REGISTRYINDEX);

		int key = r_lua_tonumber(rL, lua_upvalueindex(1));

		lua_rawgeti(L, LUA_REGISTRYINDEX, key);

		for (int arg = 1; arg <= r_lua_gettop(rL); ++arg)
			Wrapper::Wrap(rL, L, arg);
		int Err = lua_pcall(L, r_lua_gettop(rL), LUA_MULTRET, 0);
		if (!Err) {
			int args = 0;
			for (int arg = 1; arg <= lua_gettop(L); ++arg, ++args) {
				UnWrap(L, rL, arg);
			}
			lua_settop(L, 0);
			return args;
		}
		else if (Err == LUA_YIELD) {
			r_lua_pushlightuserdata(RLS, (void*)L);
			r_lua_pushcclosure(RLS, int3breakpoints[1], 1);
		}
		else if (Err == LUA_ERRRUN) {

			return 0;
		}
		return 0;
	}
}
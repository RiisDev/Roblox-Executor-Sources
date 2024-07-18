#pragma once

#include "HXNLua.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <Wininet.h>


#pragma comment(lib, "wininet.lib")
int registry;
std::vector<int> int3breakpoints;






namespace Wrapper {
	void Wrap(DWORD RLS, lua_State* LS, int Index);
	void UnWrap(lua_State* LS, DWORD RLS, int Index);
	static int RobloxYielding(DWORD RLS);
	int VanillaFunctionHandler(lua_State* LS);
	int RobloxFunctionHandler(DWORD RLS);
}


struct Userdata
{
	int32_t reference;
};

struct DataModelUserData {
	int ref;
};



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


void PushGlobal(DWORD rL, lua_State* L, const char* s)
{
	r_lua_getglobal(rL, s);
	Wrapper::Wrap(rL, L, -1);
	lua_setglobal(L, s);
	r_lua_pop(rL, 1);
	//std::cout << "GLOBAL : " << s << std::endl;
}


void ExecuteScript(std::string Script) {
	/* set the identity in a new thread!*/
	lua_State* lstate = lua_newthread(HXN_LSTATE);
	//int state = r_lua_newthread(HXN_STATE);
	SetIdentity(HXN_STATE, 6);
	//GetInstructions(Script);
	/////////////////////////////////////////////////////////
	std::string buf;
	buf += "spawn(function() script=Instance.new('LocalScript') script.Name = 'BeamIsGayAsFuck C:LoveElKoax' script.Parent = nil \r\n";
	buf += Script;
	buf += "\r\nend)";

	
	

	//replace(buf, "pcall(function() ", "");
	if (luaL_loadbuffer(HXN_LSTATE, buf.c_str(), buf.size(), "CyberBloxModule")) {
		return;
	}
	
	else {
		lua_pcall(HXN_LSTATE, 0, LUA_MULTRET, 0);
	}
	//CollectGarbage(LS);
	////////////////////////////////////////////////////////
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
	DWORD _s = HXN_ASLR(0x400000);
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







namespace Wrapper {
	void Wrap(DWORD RLS, lua_State* LS, int Index) {
		//printf("Wrap: %d\r\n", r_lua_type(RLS, Index));

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
			//printf("RBX TFUNCTION!! \n");
			r_lua_pushvalue(RLS, Index);
			lua_pushnumber(LS, r_luaL_ref(RLS, LUA_REGISTRYINDEX));
			lua_pushcclosure(LS, VanillaFunctionHandler, 1);
			//printf("PASSED RBX's TFUNCTION!! \n");
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
				lua_setmetatable(LS, -2);
				lua_pushvalue(LS, -1);
				lua_rawseti(LS, -10000, rawInstancePtr);
				r_lua_settop(RLS, -2);
			}
		}
	}
	void UnWrap(lua_State* LS, DWORD RLS, int Index) {
		//printf("UnWrap: %d\r\n", lua_type(LS, Index));
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
			//printf("UNWRAPPING TFUNCTION! \n");
			lua_pushvalue(LS, Index);
			r_lua_pushnumber(RLS, luaL_ref(LS, LUA_REGISTRYINDEX));
			r_lua_pushcclosure(RLS, int3breakpoints[0], 1);
			//printf("PASSED TFUNCTION!!\n");

		}
		else if (lua_type(LS, Index) == LUA_TTABLE) {
			lua_pushvalue(LS, Index);
			lua_getfield(LS, LUA_GLOBALSINDEX, "tostring");
			lua_pushvalue(LS, -2);
			lua_pcall(LS, 1, 1, 0);
			const char* a = lua_tolstring(LS, -1, NULL);
			lua_settop(LS, -2);
			r_lua_createtable(RLS, 0, 0);
			lua_pushnil(LS);
			while (lua_next(LS, -2)) {
				lua_pushvalue(LS, -1);
				lua_getfield(LS, LUA_GLOBALSINDEX, "tostring");
				lua_pushvalue(LS, -2);
				lua_pcall(LS, 1, 1, 0);
				const char* b = lua_tolstring(LS, -1, NULL);
				lua_settop(LS, -3);
				if (strcmp(a, b) == 0) {
					lua_settop(LS, -2);
					continue;
				}
				UnWrap(LS, RLS, -2);
				UnWrap(LS, RLS, -1);
				r_lua_settable(RLS, -3);
				lua_settop(LS, -2);
			}
			lua_settop(LS, -2);
		}
		else if (lua_type(LS, Index) == LUA_TUSERDATA) {
			r_lua_rawgeti(RLS, -10000, reinterpret_cast<Userdata*>(lua_touserdata(LS, Index))->reference);
			if (!r_lua_type(RLS, -1))
			{
				r_lua_settop(RLS, -2);
				r_lua_newuserdata(RLS, 0);
			}
		}
	}
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
	}


	int VanillaFunctionHandler(lua_State* L)
	{


		r_lua_pushstring(HXN_STATE, std::to_string(++registry).c_str());
		DWORD rL = r_lua_newthread(HXN_STATE);
		r_lua_settable(HXN_STATE, LUA_REGISTRYINDEX);
		int key = lua_tonumber(L, lua_upvalueindex(1));
		r_lua_rawgeti(rL, LUA_REGISTRYINDEX, key);

		for (int arg = 1; arg <= lua_gettop(L); ++arg)
			Wrapper::UnWrap(L, rL, arg);

		if (!r_lua_pcall(rL, lua_gettop(L), LUA_MULTRET, 0)) {
			int args = 0;
			for (int arg = 1; arg <= r_lua_gettop(rL); ++arg, ++args) {
				Wrapper::Wrap(rL, L, arg);
			}
			r_lua_settop(rL, 0);
			return args;
		}
		else {

			r_lua_pop(rL, 1);
			lua_pushthread(L);
			lua_pushcclosure(L, &VanillaYield, 1);
			Wrapper::UnWrap(L, rL, -1);
			return lua_yield(L, 0);
		}
		return 0;
	}

	lua_Debug ar;
	lua_State* SonaYieldwithNewThread;

	int RobloxFunctionHandler(DWORD rL) {
		lua_pushstring(HXN_LSTATE, std::to_string(++registry).c_str());
		lua_State* L = lua_newthread(HXN_LSTATE);
		lua_settable(HXN_LSTATE, LUA_REGISTRYINDEX);

		int key = r_lua_tonumber(rL, lua_upvalueindex(1));

		lua_rawgeti(L, LUA_REGISTRYINDEX, key);

		for (int arg = 1; arg <= r_lua_gettop(rL); ++arg)
			Wrapper::Wrap(rL, L, arg);
		int Err = lua_pcall(L, r_lua_gettop(rL), LUA_MULTRET, 0);
		if (!Err) {
			int args = 0;
			for (int arg = 1; arg <= lua_gettop(L); ++arg, ++args) {
				Wrapper::UnWrap(L, rL, arg);
			}
			lua_settop(L, 0);
			return args;
		}
		else if (Err == LUA_YIELD) {
			r_lua_pushlightuserdata(HXN_STATE, (void*)L);
			r_lua_pushcclosure(HXN_STATE, int3breakpoints[1], 1);
		}
		else if (Err == LUA_ERRRUN) {

		return 0;
	}
	//return 0;

}


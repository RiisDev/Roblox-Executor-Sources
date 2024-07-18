#pragma once
#include "Main.h"
#include "CustomFunctions.h"
#include "Addresses.h"
#include "lol.h"
#include "lua//lobject.h"

int rval = 5;

int functionhandler(lua_State* L);

string GetType(int idx) {
	rlua_pushvalue(rL, idx);
	rlua_getfield(rL, -10002, "type");
	rlua_pushvalue(rL, -2);
	rlua_pcall(rL, 1, 1, 0);
	string ret = rlua_tolstring(rL, -1, NULL);
	rlua_pop(rL, 1); 
	return ret;
}

typedef struct userdata {
	int key;
	int rval;
} userdata;

struct Rlua_State
{
	int loc;
};

typedef struct rUserdata {
	int* size;
} rUserdata;

int functionhandler(lua_State *L);

typedef int(*rlua_CFunction) (int L);
typedef int(*rlua_Function) (Rlua_State *L);

int PushFunctionToRegistry(lua_State *L)
{
	if (lua_type(L, -1) == LUA_TFUNCTION)
	{
		lua_getglobal(L, "MetaTableMagic");
		size_t regsize = lua_objlen(L, -1);
		lua_pushvalue(L, -2);
		lua_setfield(L, -2, to_string(regsize + 1).c_str());
		lua_pop(L, 2);
		return regsize + 1;
	}
	else
	{
		lua_pop(L, 1);
		return -1;
	}
}

bool wrap(DWORD rL, lua_State *L, int idx) {
	//cout << "Roblox type: " << rlua_type(rL, idx) << endl;
	switch (rlua_type(rL, idx)) {
	case RLUA_TBOOLEAN: {
		lua_pushboolean(L, rlua_toboolean(rL, idx));
		return true;
	}
	case RLUA_TNIL: {
		lua_pushnil(L);
		return true;
	}
	case RLUA_TNONE: {
		lua_pushnil(L);
		return true;
	}
	case RLUA_TNUMBER: {
		lua_pushnumber(L, rlua_tonumber(rL, idx));
		return true;
	}
	case RLUA_TSTRING: {
		lua_pushstring(L, rlua_tolstring(rL, idx, NULL));
		return true;
	}
	case RLUA_TFUNCTION: {
		//cout << "func\n";
		lua_pushnumber(L, rluaL_ref(rL, LUA_REGISTRYINDEX));
		lua_pushcclosure(L, functionhandler, 1);
		return true;
	}
	case RLUA_TTABLE: {
		lua_newtable(L);
		rlua_pushnil(rL);
		while (rlua_next(rL, -2) != 0) {
			rlua_pushvalue(rL, -2);
			const char *name = rlua_tolstring(rL, -1, NULL);
			rlua_pushvalue(rL, -2);
			wrap(rL, L, -1);
			lua_setfield(L, -2, name);
			rlua_pop(rL, 2);
		};
		rlua_pop(rL, 1);
		return true;
	};
	case RLUA_TUSERDATA: {
		int registry;
		rlua_pushvalue(rL, idx);

		rlua_pushstring(rL, std::to_string(++registry).c_str());
		rlua_pushvalue(rL, -2);
		rlua_settable(rL, LUA_REGISTRYINDEX);
		rlua_pop(rL, 1);

		lua_newuserdata(L, 0);

		lua_pushvalue(L, -1);
		lua_pushstring(L, std::to_string(registry).c_str());
		lua_settable(L, LUA_REGISTRYINDEX);

		luaL_getmetatable(L, "MetaTableMagic");

		lua_setmetatable(L, -2);
		return true;
	}
	}
	return false;
}

int rkey;
lua_State* global_L;

bool unwrap(lua_State *L, DWORD rL, int idx) {
	//cout << "Vanilla type: " << lua_type(L, idx) << std::endl;
	switch (lua_type(L, idx)) {
	case LUA_TNIL: {
		rlua_pushnil(rL);
		return true;
	}
	case LUA_TNONE: {
		rlua_pushnil(rL);
		return true;
	}
	case LUA_TNUMBER: {
		rlua_pushnumber(rL, lua_tonumber(L, idx));
		return true;
	}
	case LUA_TBOOLEAN: {
		rlua_pushboolean(rL, lua_toboolean(L, idx));
		return true;
	}
	case LUA_TSTRING: {
		rlua_pushstring(rL, lua_tostring(L, idx));
		return true;
	}
	case LUA_TFUNCTION:
	{
		global_L = L;
		rkey++;
		//printf("setting key\n");
		string key = std::to_string(rkey) + std::string("F");
		
		lua_pushvalue(L, idx);
		//MessageBoxA(0, std::to_string(lua_type(L, -1)).c_str(), 0, 0);
		lua_setfield(L, LUA_REGISTRYINDEX, key.c_str());
		rlua_pushstring(rL, key.c_str());
		pushcclosure((const void*)(breakpointaddys[0]), 1);
		return true;
	}
	case LUA_TUSERDATA:
	{

		lua_pushvalue(L, idx);
		lua_gettable(L, LUA_REGISTRYINDEX);
		if (!lua_isnil(L, -1)) {
			rlua_getfield(rL, LUA_REGISTRYINDEX, lua_tostring(L, -1));
		}
		else {
			rlua_newuserdata(rL, 0);
		}
		lua_pop(L, 1);
		break;
	}

	case LUA_TTABLE: {
		rlua_createtable(rL, 0, 0);
		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			lua_pushvalue(L, -2);
			const char *name = lua_tolstring(L, -1, NULL);
			lua_pushvalue(L, -2);
			unwrap(L, rL, -1);
			rlua_setfield(rL, -2, name);
			lua_pop(L, 2);
		}
		lua_pop(L, 1);
		return true;
	}
	}
	return false;
}

int functionhandler(lua_State* L)
{
	int key = lua_tonumber(L, lua_upvalueindex(1));
	int args = lua_gettop(L);
	rlua_rawgeti(rL, LUA_REGISTRYINDEX, key);
	for (int i = args; i != 0; i--) {
		unwrap(L, rL, i * -1);
	}
	rlua_pcall(rL, args, 1, 0);
	wrap(rL, L, -1);
	return 1;
}

int int3fnhandler(DWORD rL) {

	//printf("setting L to global_L\n");
	
	//printf("key\n");
	const char* key = rlua_tolstring(rL, lua_upvalueindex(1), NULL);
	//cout << key << "\n";
	int args = rlua_gettop(rL);
	//printf("getting field");
	lua_getfield(global_L, LUA_REGISTRYINDEX, key);
	//MessageBoxA(0, std::to_string(lua_type(global_L, -1)).c_str(), 0, 0);
	for (int i = 1; i <= args; i++) {
		wrap(rL, global_L, i);
		//i++;
	}
	if (!lua_pcall(global_L, args, 1, 0)) {
		unwrap(global_L, rL, -1);
		
	}
	else {
		//MessageBox(0, lua_tostring(global_L, -1), "O fak", 0);
	}
	
	return 1;
}

/*
int MT__index(lua_State *L) {
//cout << "index\n";
const char* key = lua_tolstring(L, -1, NULL);
//cout << "unwrap\n";
unwrap(L, rL, 1);
//cout << "getfield\n";
rlua_getfield(rL, -1, key);
//cout << "wrap\n";
wrap(rL, L, -1);
return 1;
}
*/

static int Custom_GetObjects(lua_State* L) {
	auto asset = lua_tostring(L, -1);
	auto hold = std::string("return {game:GetService('InsertService'):LoadLocalAsset('") + std::string(asset) + std::string("')}");
	luaL_dostring(L, hold.c_str());
	return 1;
}


static int Lua_GetSite(lua_State* L) {
	lua_pushstring(L, std::string(DownloadURL(lua_tostring(L, -1))).c_str());
	return 1;
};

static int Lua_GetObjects(lua_State* L) {
	rlua_getfield(rL, -10002, "game");
	rlua_getfield(rL, -1, "GetService");
	rlua_pushvalue(rL, -2);
	rlua_pushstring(rL, "InsertService");
	rlua_call(rL, 2, 1);
	rlua_getfield(rL, -1, "LoadLocalAsset");
	rlua_pushvalue(rL, -2);
	rlua_pushstring(rL, lua_tostring(L, -1));
	rlua_call(rL, 2, 1);
	lua_createtable(L, 0, 0);
	wrap(rL, L, -1);
	lua_insert(L, -1);
	return 1;
};

static int MT__index(lua_State *L) {
	std::string key = std::string(lua_tolstring(L, -1, NULL));
	if (key == std::string("HttpGet")) {
		lua_pushcfunction(L, Lua_GetSite);
	}
	else if (key == std::string("GetObjects")) {
		lua_pushcfunction(L, Custom_GetObjects);
	}
	else {
		unwrap(L, rL, 1);
		rlua_getfield(rL, -1, key.c_str());
		wrap(rL, L, -1);
	};
	return 1;
};



static int Wait(lua_State*L) {
	int t;
	if (lua_type(L, -1) == LUA_TNUMBER) {
		t = lua_tonumber(L, -1);
		if (t < (1 / 30)) {
			t = (1 / 30);
		};
	}
	else {
		t = (1 / 30);
	};
	Sleep(t * 1000);
	lua_pushnumber(L, t);
	return 1;
};

static int newwait(lua_State* L) {
	rlua_getglobal(rL, "wait");
	int ArgCount = 0;
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		unwrap(L, rL, x * -1);
		ArgCount += 1;
	}
	rlua_pcall(rL, ArgCount, 0, 0);
	return ArgCount;
}

static int newspawn(lua_State* L) {
	rlua_getglobal(rL, "spawn");
	int ArgCount = 0;
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		unwrap(L, rL, x * -1);
		ArgCount += 1;
	}
	rlua_pcall(rL, ArgCount, 0, 0);
	return ArgCount;
}

int MT__newindex(lua_State *L) {
	const char* key = lua_tostring(L, -2);
	unwrap(L, rL, -3);
	unwrap(L, rL, -1);
	rlua_setfield(rL, -2, key);
	rlua_pop(rL, 1);
	return 1;
}

void nopRange(DWORD start, DWORD sz) { 
	DWORD old;
	VirtualProtect((LPVOID)start, sz, PAGE_EXECUTE_READWRITE, &old); 
	memcpy((void*)start, (char*)
		90, sz);
	VirtualProtect((LPVOID)start, sz, old, &old);
}

void WrapGlobal(lua_State *L, const char* g) {
	rlua_getfield(rL, -10002, g);
	wrap(rL, L, -1);
	lua_setglobal(L, g);
}

int rPrint(lua_State *L) {
	rlua_getglobal(rL, "print");
	unwrap(L, rL, 1);
	rlua_pcall(rL, 1, 0, 0);
	return 1;
}

void RegisterMT(lua_State *L) {
	lua_newtable(L);
	lua_pushcfunction(L, MT__index);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, MT__newindex);
	lua_setfield(L, -2, "__newindex");
	lua_pushstring(L, "The metatable is locked");
	lua_setfield(L, -2, "__metatable");
	lua_setfield(L, LUA_REGISTRYINDEX, "MetaTableMagic");
}

int custom_getrawmetatable(lua_State *L) {
	unwrap(L, rL, 1);
	if (!rlua_getmetatable(rL, -1))
		lua_pushnil(L);
	wrap(rL, L, -1);
	return 1;
}

void RegisterRF(lua_State *L) {
	WrapGlobal(L, "print");
	WrapGlobal(L, "Instance");
	WrapGlobal(L, "Enum");
	WrapGlobal(L, "plugin");
	WrapGlobal(L, "Enum");
	WrapGlobal(L, "Axes");
	WrapGlobal(L, "BrickColor");
	WrapGlobal(L, "CFrame");
	WrapGlobal(L, "Color3");
	WrapGlobal(L, "ColorSequence");
	WrapGlobal(L, "ColorSequenceKeypoint");
	WrapGlobal(L, "Faces");
	WrapGlobal(L, "NumberRange");
	WrapGlobal(L, "NumberSequence");
	WrapGlobal(L, "NumberSequenceKeypoint");
	WrapGlobal(L, "PhysicalProperties");
	WrapGlobal(L, "Random");
	WrapGlobal(L, "Ray");
	WrapGlobal(L, "Rect");
	WrapGlobal(L, "Region3");
	WrapGlobal(L, "Region3int16");
	WrapGlobal(L, "TweenInfo");
	WrapGlobal(L, "UDim");
	WrapGlobal(L, "UDim2");
	WrapGlobal(L, "Vector2");
	WrapGlobal(L, "Vector3");
	WrapGlobal(L, "Vector3int16");
	WrapGlobal(L, "delay");
	WrapGlobal(L, "elapsedTime");
	WrapGlobal(L, "printidentity");
	WrapGlobal(L, "LoadLibrary");
	WrapGlobal(L, "PlayerGui");
	WrapGlobal(L, "PluginManager");
	WrapGlobal(L, "require");
	WrapGlobal(L, "settings");
	WrapGlobal(L, "spawn");
	WrapGlobal(L, "tick");
	WrapGlobal(L, "time");
	WrapGlobal(L, "typeof");
	WrapGlobal(L, "UserSettings");
	WrapGlobal(L, "version");
	WrapGlobal(L, "wait");
	WrapGlobal(L, "warn");
	WrapGlobal(L, "error");
}

void printconsole(lua_State *L) {
	std::string toCons = lua_tostring(L, -1);
	std::cout << toCons << "\n";
}

void wrapper_fn_getglobal(lua_State *L) {
	std::string global = lua_tostring(L, -1);
	rlua_getfield(rL, LUA_GLOBALSINDEX, global.c_str());
}
void wrapper_fn_getfield(lua_State *L) {
	std::string field = lua_tostring(L, -1);
	int idx = lua_tointeger(L, -2);
	rlua_getfield(rL, idx, field.c_str());
}

void wrapper_fn_pushstring(lua_State *L) {
	std::string strk = lua_tostring(L, -1);
	rlua_pushstring(rL, strk.c_str());
}

void wrapper_fn_pushnumber(lua_State *L) {
	double num = lua_tonumber(L, -1);
	rlua_pushnumber(rL, num);
}

void toClipboard(const std::string &s) {
	OpenClipboard(0);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size());
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size());
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}

void fn_toclipboard(lua_State *L) {
	std::string tc = lua_tostring(L, -1);
	toClipboard(tc);
}

int MouseButton1Down(lua_State* L) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int MouseButton2Down(lua_State* L) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int MouseButton1Up(lua_State* L) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int MouseButton2Up(lua_State* L) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

void AddFunction(lua_State* L, std::string Name, lua_CFunction function) {
	lua_newtable(L);
	lua_pushcfunction(L, function);
	lua_setglobal(L, Name.c_str());
	lua_settop(L, 0);
}

void RegisterCF(lua_State *L) {
	lua_register(L, "getrawmetatable", custom_getrawmetatable);
	lua_register(L, "wait", reinterpret_cast<lua_CFunction>(newwait));
	lua_register(L, "spawn", reinterpret_cast<lua_CFunction>(newspawn));
	lua_register(L, "printconsole", (lua_CFunction)printconsole);
	lua_register(L, "getglobal", (lua_CFunction)wrapper_fn_getglobal);
	lua_register(L, "getfield", (lua_CFunction)wrapper_fn_getfield);
	lua_register(L, "pushstring", (lua_CFunction)wrapper_fn_pushstring);
	lua_register(L, "pushnumber", (lua_CFunction)wrapper_fn_pushnumber);
	lua_register(L, "toclipboard", (lua_CFunction)fn_toclipboard);
	lua_register(L, "loadfile", (lua_CFunction)loadfile);
	     
	
	//lua_register(L, "getgenv", (lua_CFunction)fn_getgenv);
}

void SetUpEnvironment(lua_State *L) {

	WrapGlobal(L, "game");
	WrapGlobal(L, "workspace");
	RegisterMT(L);
	RegisterRF(L);
	RegisterCF(L);	
	AddFunction(L, "MouseButton1Up", MouseButton1Up);
	AddFunction(L, "MouseButton2Up", MouseButton2Up);
	AddFunction(L, "MouseButton1Down", MouseButton1Down);
	AddFunction(L, "MouseButton2Down", MouseButton2Down);
}


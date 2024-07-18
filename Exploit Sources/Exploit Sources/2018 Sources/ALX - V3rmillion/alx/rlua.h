#ifndef RLUA_H
#define RLUA_H 1
#include <windef.h>
#include <iostream>
#include <vector>

/*
typedef void(*Lua_pushstring)(DWORD luaState, const char *s);
typedef void(*Lua_setfield)(DWORD luaState, int index, const char *k);
typedef void(*Lua_createtable)(DWORD luaState, int narr, int nrec);
typedef int(*Lua_error)(DWORD luaState);
typedef void(*Lua_pushcclosure)(DWORD luaState, void* fn, int n);
typedef int(*Lua_pcall)(DWORD luaState, int nargs, int nresults, int errfunc);
typedef void(*Lua_createtable)(DWORD luaState, int narr, int nrec);
typedef void(*Lua_pushlightuserdata)(DWORD luaState, void *p);
typedef void(*Lua_getfield)(DWORD luaState, int index, const char *k);
typedef void*(*Lua_newuserdata)(DWORD luaState, size_t size);
typedef int(*Lua_setmetatable)(DWORD luaState, int index);
typedef void(*Lua_pushnumber)(DWORD luaState, double n);
typedef void(*Lua_settop)(DWORD luaState, int index);
typedef void(*Lua_pushboolean)(DWORD luaState, int b);
typedef void(*Lua_pushinteger)(DWORD luaState, ptrdiff_t n);*/

typedef int(*__thiscall setProp)(void* self, void* val);

typedef int(*LSetProperty)(DWORD luaState);
//typedef int(*LGetProperty)(DWORD luaState);

typedef struct Vector3 { float x, y, z; };

typedef struct CFrame
{
	float rotationmatrix[9];
	Vector3 position;
};

typedef struct InstanceTable {
	DWORD begin;
	DWORD stop;
	BYTE valid;
};

typedef struct ScanResults {
	DWORD failList[13];
	DWORD failCount;
};

typedef struct PropertyInfo {
	int propInfo;
	int instOffset;
	int luaState;
	int loc;
	int instS;
};

typedef int(__thiscall *SetMinutesAfterMidnight)(void* self, double minutes);
typedef int(__thiscall *SetCFrame)(void* self, CFrame* cfStruct);
typedef CFrame*(__thiscall *GetCFrame)(void* self);
typedef int(__thiscall *PlaySoundInstance)(void* self);
typedef int(__thiscall *StopSoundInstance)(void* self);

typedef int(__thiscall *SetProperty)(PropertyInfo* propInfo, void* value);

class RLua {
	DWORD L;
	LSetProperty lSetProperty;
	//LGetProperty lGetProperty;
	ScanResults results;
	InstanceTable getInstanceTable(DWORD instance, DWORD offset);
	SetCFrame setCFrame;
	GetCFrame getCFrame;

public:
	//RLua(const DWORD lcvPtr);
	void initiate(DWORD gameInstance);
	DWORD getLuaState();
	ScanResults getInitResults();
	void setProperty(DWORD instance, const char* propName, const char* propType, void* value);

	SetProperty setProperty_float;
	SetProperty setProperty_double;
	SetProperty setProperty_integer;
	SetProperty setProperty_bool;
	SetProperty setProperty_string;

	/*
	Lua_pushstring pushstring;
	Lua_setfield setfield;
	Lua_createtable createtable;
	Lua_error error;
	Lua_pushcclosure pushcclosure;
	Lua_pcall pcall;
	Lua_pushlightuserdata pushlightuserdata;
	Lua_pushnumber pushnumber;
	Lua_getfield getfield;
	Lua_newuserdata newuserdata;
	Lua_setmetatable setmetatable;
	Lua_settop settop;
	Lua_pushboolean pushboolean;
	Lua_pushinteger pushinteger;*/

	std::vector<DWORD> getChildren(DWORD instance);
	std::vector<DWORD> getRecursiveChildren(DWORD instance);
	DWORD getChildByName(DWORD instance, const char* name);
	DWORD getInstanceInHierarchy(DWORD gameInstance, char** names, DWORD size);
	std::vector<DWORD> getPlayers(DWORD gameInst);
	DWORD getPlayerByName(DWORD gameInstance, const char* name);
	DWORD getCharacterFromPlayer(DWORD player);
	//DWORD createInstance(char *instanceName, DWORD parent);
	void destroyInstance(DWORD instance); // naked
	void setObjectCFrame(DWORD instance, float x, float y, float z);
	void setObjectCFrame(DWORD instance, CFrame* cfStruct);
	CFrame* getObjectCFrame(DWORD instance);
	DWORD getLocalPlayer(DWORD gameInstance);
	void unanchorChildren(DWORD instance);
	void unlockChildren(DWORD instance);
	DWORD clearInstanceByName(DWORD instance, const char* name);
	DWORD clearInstanceByClass(DWORD instance, const char* cname);
	std::string* getName(DWORD instance);
	std::string getClassName(DWORD instance);

	SetMinutesAfterMidnight setMinutesAfterMidnight;
	PlaySoundInstance playSoundInstance;
	StopSoundInstance stopSoundInstance;

	DWORD setInstanceParent;
	DWORD getInstanceIDFromName;
};

DWORD createInstance(RLua& rLua, char *instanceName, DWORD parent);
#endif
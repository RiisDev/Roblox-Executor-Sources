#include "Roblox.hpp"
#include "Utils.hpp"
#include "Bypasses.hpp"

#include <windows.h>
#include <iostream>
#include <vector>
#include <functional>

int datamodel = 0;
int scriptcontext = 0;
int luastate = 0;

#define identityoffset1 0x6C
#define identityoffset2  0x18

#define flag0GetDatamodel 0x0
#define offset0GetDatamodel 0x2C
auto addrType = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7db370));
#define ccType __cdecl

auto addrSetTop = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7daf30));
#define ccSetTop __stdcall  

auto addrGetTop = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7d9460));
#define ccGetTop __cdecl

auto addrSetField = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7dabd0));
#define ccSetField __stdcall  

auto addrSetTable = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7daed0)); // ;)
#define ccSetTable __cdecl //ty bb

auto addrGetField = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7d8f20));
#define ccGetField __fastcall   

auto addrIsCFunction = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7db2f0));
#define ccIsCFunction __cdecl //Ya forgot some 0x0

auto addrGetTable = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7d9400));
#define ccGetTable __cdecl 

auto addrPushLString = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7da080));
#define ccPushLString __cdecl

auto addrPushCClosure = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7d9e00));
#define ccPushCClosure __fastcall 

auto addrCreateTable = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7d8b70));
#define ccCreateTable __cdecl

auto addrPushValue = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7da2f0));
#define ccPushValue __stdcall  

auto addrPushNil = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7da100));
#define ccPushNil __cdecl 

auto addrPushBoolean = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7d9db0));
#define ccPushBoolean __cdecl 

auto addrPushNumber = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7da150));
#define ccPushNumber __thiscall

auto addrGetMetatable = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7d92d0));
#define ccGetMetatable __fastcall  

auto addrSetMetatable = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7dad30));
#define ccSetMetatable __cdecl

auto addrSetReadOnly = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7dae80));
#define ccSetReadOnly __cdecl

auto addrToLString = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7db0f0));
#define ccToLString __cdecl  

auto addrToBoolean = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7db060));
#define ccToBoolean __cdecl 

auto addrToUserData = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7db310));
#define ccToUserData __cdecl

auto addrToThread = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7DB2F0));
#define ccToThread __cdecl

auto addrCheckInteger = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7d8770)); //last updated 22.04
#define ccCheckInteger __cdecl

auto addrCheckLString = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7d64d0));
#define ccCheckLString __cdecl

auto addrCheckNumber = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7D6500));
#define ccCheckNumber __cdecl

auto addrCheckOption = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7d89a0));//This is checkstack not checkoption
#define ccCheckOption __cdecl 

auto addrCheckType = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7d65a0));
#define ccCheckType __cdecl

auto addrCall = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7d8930));
#define ccCall __cdecl

auto addrPCall = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7d9d00)); //updated
#define ccPCall __cdecl 

auto addrError = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7d65e0));
#define ccError __cdecl

auto addrNext = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7d9ac0)); //updated
#define ccNext __cdecl 

auto addrRemove = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7da770));
#define ccRemove __cdecl

auto addrRawSetI = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7da640));
#define ccRawSetI __cdecl 

auto addrDeserializer = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x8e0140)); //: bytecode version mismatch
#define ccDeserializer __cdecl

auto addrSpawn = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x72c400));
#define ccSpawn __cdecl

auto addrNewThread = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7d9870)); //updated
#define ccNewThread __cdecl

auto addrNewUserData = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7d99b0)); //updated
#define ccNewUserData __cdecl

auto addrIndex2Adr = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x7d3ab0)); //updated
#define ccIndex2Adr __cdecl

auto addrSandboxThread = Bypasses::Retcheck::remove(Utils::RebaseAddress(0x72c0a0)); //updated
#define ccSandboxThread __cdecl

template<class Ret, class... Args>
std::function<Ret(Args...)> Call(DWORD Address, std::string Convention)
{
	if (Convention == "Stdcall")
	{
		typedef Ret(__stdcall* Fn_t)(Args...);
		return (Fn_t)Address;
	}
	else if (Convention == "Cdecl")
	{
		typedef Ret(__cdecl* Fn_t)(Args...);
		return (Fn_t)Address;
	}
	else if (Convention == "Fastcall")
	{
		typedef Ret(__fastcall* Fn_t)(Args...);
		return (Fn_t)Address;
	}
	else if (Convention == "Thiscall")
	{
		typedef Ret(__thiscall* Fn_t)(Args...);
		return (Fn_t)Address;
	}
}


int Roblox::Functions::GetDatamodel()
{
	volatile DWORD StackPad[16]{};
	static DWORD DMPad[16]{};
	auto Function = Call<DWORD, DWORD, DWORD>(Utils::RebaseAddress(0xe51ac0), "Thiscall"); //right
	auto Function2 = Call<DWORD>(Utils::RebaseAddress(0xe51970), "Cdecl"); //right -> "SetupGame" 1xref; the first address after the IF statement
	Function(Function2(), (DWORD)DMPad);
	DWORD DM = DMPad[0];
	return DM + 0x44;
}

int Roblox::Functions::rlua_pop(int ls, int a)
{
	return rlua_settop(ls, -(a)-1);
}

int Roblox::Functions::rlua_type(int ls, int idx)
{
	return ((int(ccType*)(int, int))(addrType))(ls, idx);
}

int Roblox::Functions::rlua_settop(int ls, int a)
{
	return ((int(ccSetTop*)(int, int))(addrSetTop))(ls, a);
}

int Roblox::Functions::rlua_gettop(int ls)
{
	return ((int(ccGetTop*)(int))(addrGetTop))(ls);
}

int Roblox::Functions::rlua_getglobal(int ls, const char* key)
{
	return rlua_getfield(ls, -10002, key);
}

int Roblox::Functions::rlua_setglobal(int ls, const char* key)
{
	return rlua_setfield(ls, -10002, key);
}

int Roblox::Functions::rlua_getfield(int ls, int idx, const char* key)
{
	return ((int(ccGetField*)(int, int, const char*))(addrGetField))(ls, idx, key);
}

int Roblox::Functions::rlua_iscfunction(int ls, int idx)
{
	return ((int(ccIsCFunction*)(int, int))(addrIsCFunction))(ls, idx);
}

void Roblox::Functions::rlua_gettable(int ls, int idx)
{
	return ((void(ccGetTable*)(int, int))(addrGetTable))(ls, idx);
}

int Roblox::Functions::rlua_setfield(int ls, int idx, const char* key)
{
	return ((int(ccSetField*)(int, int, const char*))(addrSetField))(ls, idx, key);
}

int Roblox::Functions::rlua_settable(int ls, int idx)
{
	return ((int(ccSetTable*)(int, int))(addrSetTable))(ls, idx);
}

int Roblox::Functions::rlua_setreadonly(int ls, int a, int b)
{
	return ((int(ccSetReadOnly*)(int, int, int))(addrSetReadOnly))(ls, a, b);
}

int Roblox::Functions::rlua_setmetatable(int ls, int a)
{
	return ((int(ccSetMetatable*)(int, int))(addrSetMetatable))(ls, a);
}

int Roblox::Functions::rlua_getmetatable(int ls, int a)
{
	return ((int(ccGetMetatable*)(int, int))(addrGetMetatable))(ls, a);
}

int Roblox::Functions::rluaL_getmetatable(int ls, const char* tname)
{
	return rlua_getfield(ls, LUA_REGISTRYINDEX, tname);
}

void Roblox::Functions::rluaL_setmetatable(int ls, const char* tname)
{
	rluaL_getmetatable(ls, tname);
	rlua_setmetatable(ls, -2);
}

int Roblox::Functions::rlua_pushlstring(int ls, const char* val, int len)
{
	return ((int(ccPushLString*)(int, const char*, int))(addrPushLString))(ls, val, len);
}

int Roblox::Functions::rlua_pushcfunction(int ls, rbx_CFunction fn)
{
	return Functions::rlua_pushcclosure(ls, fn, 0);
}

int Roblox::Functions::rlua_pushcclosure(int ls, rbx_CFunction fn, int n)
{
	DWORD addr = Bypasses::Callcheck::Add(fn);
	return ((int(ccPushCClosure*)(int, rbx_CFunction, int, int, int))addrPushCClosure)(ls, (rbx_CFunction)addr, NULL, n, NULL);
}

int Roblox::Functions::rlua_pushnumber(int ls, double num)
{
	return ((int(ccPushNumber*)(int, double))(addrPushNumber))(ls, num);
}

int Roblox::Functions::rlua_createtable(int ls, int a, int b)
{
	return ((int(ccCreateTable*)(int, int, int))(addrCreateTable))(ls, a, b);
}

int Roblox::Functions::rlua_pushvalue(int ls, int a)
{
	return ((int(ccPushValue*)(int, int))(addrPushValue))(ls, a);
}

int Roblox::Functions::rlua_pushnil(int ls)
{
	return ((int(ccPushNil*)(int))(addrPushNil))(ls);
}

int Roblox::Functions::rlua_pushboolean(int ls, bool a)
{
	return ((int(ccPushBoolean*)(int, bool))(addrPushBoolean))(ls, a);
}

void Roblox::Functions::rlua_register(int ls, rbx_CFunction fn, const char* name)
{
	Functions::rlua_pushcfunction(ls, fn);
	Functions::rlua_setglobal(ls, name);
}

const char* Roblox::Functions::rlua_tolstring(int ls, int idx, int len)
{
	return ((const char* (ccToLString*)(int, int, int))(addrToLString))(ls, idx, len);
}

const char* Roblox::Functions::rlua_tostring(int ls, int idx)
{
	return rlua_tolstring(ls, idx, 0);
}

bool Roblox::Functions::rlua_toboolean(int ls, int idx)
{
	return ((bool (ccToBoolean*)(int, int))(addrToBoolean))(ls, idx);
}

void* Roblox::Functions::rlua_touserdata(int ls, int idx)
{
	return ((void* (ccToUserData*)(int, int))(addrToUserData))(ls, idx);
}

int Roblox::Functions::rlua_tothread(int ls, int idx)
{
	return ((int(ccToThread*)(int, int))(addrToThread))(ls, idx);
}

lua_Integer Roblox::Functions::rluaL_checkinteger(int ls, int numarg)
{
	return ((lua_Integer(ccCheckInteger*)(int, int))(addrCheckInteger))(ls, numarg);
}

const char* Roblox::Functions::rluaL_checklstring(int ls, int a, unsigned int* b)
{
	return ((const char* (ccCheckLString*)(int, int, unsigned int*))(addrCheckLString))(ls, a, b);
}

lua_Number Roblox::Functions::rluaL_checknumber(int ls, int numarg)
{
	return ((lua_Number(ccCheckNumber*)(int, int))(addrCheckNumber))(ls, numarg);
}

int Roblox::Functions::rluaL_checkoption(int ls, int numarg, const char* def, const char* const* lst)
{
	return ((int(ccCheckOption*)(int, int, const char*, const char* const*))(addrCheckOption))(ls, numarg, def, lst);
}

void Roblox::Functions::rluaL_checktype(int ls, int numarg, int t)
{
	return; ((void(ccCheckType*)(int, int, int))(addrCheckType))(ls, numarg, t);
}

void Roblox::Functions::rlua_rawseti(int ls, int a, int b)
{
	((void(ccRawSetI*)(int, int, int))(addrRawSetI))(ls, a, b);
}

int Roblox::Functions::rlua_call(int ls, int a, int b)
{
	return ((int(ccCall*)(int, int, int))(addrCall))(ls, a, b);
}

int Roblox::Functions::rlua_pcall(int ls, int a, int b, int c)
{
	return ((int(ccPCall*)(int, int, int, int))(addrPCall))(ls, a, b, c);
}

int Roblox::Functions::rlua_next(int ls, int a)
{
	return ((int(ccNext*)(int, int))(addrNext))(ls, a);
}

void Roblox::Functions::rlua_remove(int ls, int a)
{
	((void(ccRemove*)(int, int))(addrRemove))(ls, a);
}

int Roblox::Functions::rluaL_error(int ls, const char* reason)
{
	return ((int(ccError*)(int, const char*))(addrError))(ls, reason);
}

int Roblox::Functions::rlua_pushstring(int ls, const char* val)
{
	return Functions::rlua_pushlstring(ls, val, strlen(val));
}

int Roblox::Functions::rlua_newthread(int ls)
{
	return ((int(ccNewThread*)(int))(addrNewThread))(ls);
}

void* Roblox::Functions::rlua_newuserdata(int ls, unsigned int sz)
{
	return ((void* (ccNewUserData*)(int, unsigned int))(addrNewUserData))(ls, sz);
}

int Roblox::Functions::rluaL_newmetatable(int ls, const char* name)
{
	rlua_getfield(ls, LUA_REGISTRYINDEX, name);  /* get registry.name */
	if (rlua_type(ls, -1) != RLUA_TNIL)  /* name already in use? */
		return 0;  /* leave previous value on top, but return 0 */
	rlua_pop(ls, 1);
	rlua_createtable(ls, 0, 0);  /* create metatable */
	rlua_pushvalue(ls, -1);
	rlua_setfield(ls, LUA_REGISTRYINDEX, name);  /* registry.name = metatable */
	return 1;
}

TValue* Roblox::Functions::rlua_index2adr(int a, int b)
{
	return ((TValue * (ccIndex2Adr*)(int, int))(addrIndex2Adr))(a, b);
}

int Roblox::Functions::deserializer(int ls, const char* name, const char* source, int unk)
{
	return ((int(ccDeserializer*)(int, const char*, const char*, int))(addrDeserializer))(ls, name, source, unk);
}

void Roblox::Functions::spawn(int ls)
{
	((void(ccSpawn*)(int))(addrSpawn))(ls);
}

DWORD Datamodel;

void Roblox::Initialize()
{
	printf("a\n");
	datamodel = Functions::GetDatamodel();
	printf("b\n");
	scriptcontext = Instance::FindFirstClass(datamodel, "ScriptContext");
	printf("c\n");
	luastate = scriptcontext + 56 * 0 + 164 ^ *(DWORD*)(scriptcontext + 56 * 0 + 164);
}

int Roblox::GetDatamodel()
{
	return datamodel;
}

int Roblox::GetScriptContext()
{
	return scriptcontext;
}

int Roblox::GetLuaState()
{
	return luastate;
}


void Roblox::Functions::SetContext(int ls, int context)
{
	*(DWORD*)(*(DWORD*)(ls + identityoffset1) + identityoffset2) = context;
}

int Roblox::Functions::GetContext(int ls)
{
	DWORD a1 = ls;
	int v3 = *(DWORD*)(a1 + identityoffset1);
	int final = *(DWORD*)(v3 + identityoffset2);
	return final;
}

const char* Roblox::Instance::GetClass(int self)
{
	return (const char*)(*(int(**)(void))(*(int*)self + 16))();
}

int Roblox::Instance::FindFirstClass(int Instance, const char* Name)
{
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);

	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		if (memcmp(GetClass(*(int*)i), Name, strlen(Name)) == 0)
		{
			return *(int*)i;
		}
	}
}
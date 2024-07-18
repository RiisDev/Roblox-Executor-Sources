
#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iosfwd>

#pragma region Roblox Offsets
#define OVM_XRExchange 0x000000
#pragma endregion

#pragma region Roblox Lua Types
#define R_LUA_TNIL 0
#define R_LUA_TLIGHTUSERDATA 1
#define R_LUA_TNUMBER 2
#define R_LUA_TBOOLEAN 3
#define R_LUA_TSTRING 4
#define R_LUA_TTHREAD 5
#define R_LUA_TFUNCTION 6
#define R_LUA_TTABLE 7
#define R_LUA_TUSERDATA 8
#define R_LUA_TPROTO 9
#define R_LUA_TUPVALUE 10
#pragma endregion

#define OVM_Offset(a, b) (a - b + OVM_GetModuleBase())
#define OVM_GetXRExchangeConst() (OVM_ReadAt<DWORD, OVM_XRExchange>())

inline DWORD OVM_GetModuleBase()
{
	static DWORD ModuleBase;
	if (!ModuleBase)
		ModuleBase = reinterpret_cast<DWORD>(GetModuleHandleA(NULL));
	return ModuleBase;
}

// Use definitive offset when value is known at compile time
template <typename _xT, DWORD _DefinitiveOffset = 0>
inline _xT* OVM_ReadAt(DWORD GivenOffset = 0)
{
	if (_DefinitiveOffset)
		return reinterpret_cast<_xT*>(OVM_GetModuleBase() + _DefinitiveOffset);
	else if (GivenOffset)
		return reinterpret_cast<_xT*>(OVM_GetModuleBase() + GivenOffset);
	else
		return nullptr;
}
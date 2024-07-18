#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>

std::vector<DWORD> pushVectorT;

namespace Script_Context
{
	/*globals*/
	uintptr_t scriptContextVFtable = 0;
	uintptr_t scriptContext = 0;

	/*luastate define*/
	namespace LuaState
	{
		uintptr_t* v39 = reinterpret_cast<uintptr_t*>(scriptContext);
		int v48 = 0;

		uintptr_t grabLuaState(uintptr_t rL)
		{
			v48 = rL;
			uint32_t lstate = (unsigned int)& v39[14 * v48 + 43] ^ v39[14 * v48 + 43];
			return lstate;
		
		}

	}
	



}
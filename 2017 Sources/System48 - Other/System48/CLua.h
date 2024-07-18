#pragma once
#include <Windows.h>

#define ScanAddress(address) (address - 0x400000 + (DWORD)GetModuleHandle(0))

#define RETCHECK 0x542EF0
#define RETCHECK_FLAG1 0x143EA80
#define RETCHECK_FLAG2 0x19F6144

#define RLUA_PRECALL_FLAG 0x15A3AC8

#define GETFIELD 0x7219d0
#define SETTOP 0x723ad0
#define PUSHSTRING 0x722d50
#define PUSHVALUE 0x722e10
#define PCALL 0x722790
#define TOBOOLEAN 0x723c50
#define PUSHNUMBER 0x722cc0
#define SETFIELD 0x7236f0
#define NEWTHREAD 0x722460
#define PUSHCCLOSURE 0x7228e0
#define TOLSTRING 0x723d10

DWORD unprotect(DWORD addr)
{
	BYTE* tAddr = (BYTE*)addr;

	/* Calcualte the size of the function
	In theory this will run until it hits the next
	functions prolog. It assumes all calls are aligned to
	16 bytes. (grazie katie)
	*/
	do
	{
		tAddr += 16;
	} while (!(tAddr[0] == 0x55 && tAddr[1] == 0x8B && tAddr[2] == 0xEC));

	DWORD funcSz = tAddr - (BYTE*)addr;

	/* Allocate memory for the new function */
	PVOID nFunc = VirtualAlloc(NULL, funcSz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (nFunc == NULL)
		return addr;

	/* Copy the function to the newly allocated memory */
	memcpy(nFunc, (void*)addr, funcSz);

	BYTE* pos = (BYTE*)nFunc;
	BOOL valid = false;
	do
	{
		/* Check for the return check with the sig:
		72 ?? A1 ?? ?? ?? ?? 8B
		If the sig matches replace the the jb with a jmp.
		*/
		if (pos[0] == 0x72 && pos[2] == 0xA1 && pos[7] == 0x8B) {
			*(BYTE*)pos = 0xEB;

			DWORD cByte = (DWORD)nFunc;
			do
			{
				/* Check if the current byte is a call if it is,
				calculate the new relative call(s).
				*(->E8 + 1) = originalFunction - nextInstruction

				oFuncPos - Position of call in original function
				= originalFunction + (->E8 - newFunction)

				oFuncAddr - Original call location
				= oFuncPos + rel32Offset + sizeof(call)

				relativeAddr - New relative address
				= oFuncAddr - ->E8 - sizeof(call)

				Since we are not using a disassembler we assume
				that if we hit a E8 byte which is properly aligned
				it is a relative call.
				For a small amount of compensation I skip location
				of the call, since it is possible to have the byte
				E8 inside of it.
				*/
				if (*(BYTE*)cByte == 0xE8)
				{
					DWORD oFuncPos = addr + (cByte - (DWORD)nFunc);
					DWORD oFuncAddr = (oFuncPos + *(DWORD*)(oFuncPos + 1)) + 5;

					if (oFuncAddr % 16 == 0)
					{
						DWORD relativeAddr = oFuncAddr - cByte - 5;
						*(DWORD*)(cByte + 1) = relativeAddr;

						cByte += 4;
					}
				}

				cByte += 1;
			} while (cByte - (DWORD)nFunc < funcSz);

			valid = true;
		}
		pos += 1;
	} while ((DWORD)pos < (DWORD)nFunc + funcSz);

	/* This function has no return check, let's not waste memory */
	if (!valid)
	{
		VirtualFree(nFunc, funcSz, MEM_RELEASE);
		return addr;
	}

	return (DWORD)nFunc;
}

namespace CLua {
	DWORD State;
	typedef void(__cdecl *lua_getfield)(DWORD luaState, DWORD index, const char *k);
	typedef void(__cdecl *lua_settop)(DWORD luaState, DWORD s);
	typedef void(__cdecl *lua_pushstring)(DWORD luaState, const char *s);
	typedef void(__cdecl *lua_pushvalue)(DWORD luaState, DWORD index);
	typedef DWORD(__cdecl *lua_pcall)(DWORD luaState, DWORD nargs, DWORD nresults, DWORD errfunc);
	typedef DWORD(__cdecl *lua_toboolean)(DWORD luaState, DWORD index);
	typedef void(__cdecl *lua_pushnumber)(DWORD luaState, double n);
	typedef void(__cdecl *lua_setfield)(DWORD luaState, DWORD index, const char *k);
	typedef DWORD(__cdecl *lua_newthread)(DWORD luaState);
	typedef void(__cdecl *lua_pushcclosure)(DWORD luaState, LPVOID hi, DWORD n);
	typedef const char*(__cdecl *lua_tolstring)(DWORD lua_State, DWORD index, size_t *len);
	typedef void(__cdecl *clua_pushbool)(DWORD, bool);


	lua_getfield _getfield = (lua_getfield)unprotect(ScanAddress(GETFIELD));
	lua_settop _settop = (lua_settop)unprotect(ScanAddress(SETTOP));
	lua_pushstring _pushstring = (lua_pushstring)unprotect(ScanAddress(PUSHSTRING));
	lua_pushvalue _pushvalue = (lua_pushvalue)unprotect(ScanAddress(PUSHVALUE));
	lua_pcall _pcall = (lua_pcall)unprotect(ScanAddress(PCALL));
	lua_toboolean toboolean = (lua_toboolean)unprotect(ScanAddress(TOBOOLEAN));
	lua_pushnumber _pushnumber = (lua_pushnumber)unprotect(ScanAddress(PUSHNUMBER));
	lua_setfield _setfield = (lua_setfield)unprotect(ScanAddress(SETFIELD));
	lua_newthread _newthread = (lua_newthread)unprotect(ScanAddress(NEWTHREAD));
	lua_pushcclosure _pushcclosure = (lua_pushcclosure)unprotect(ScanAddress(PUSHCCLOSURE));
	lua_tolstring _tolstring = (lua_tolstring)unprotect(ScanAddress(TOLSTRING));
	//clua_pushbool _pushbool = (clua_pushbool)ScanAddress(PUSHBOOL);

	typedef int(__cdecl *Lua_getmetatable)(DWORD lua_State, int idx);
	Lua_getmetatable getmetatable = (Lua_getmetatable)unprotect(ScanAddress(0x721ca0));


	void WriteMemory(DWORD Address, DWORD Instruction) {
		DWORD oldProtection;
		VirtualProtect((LPVOID)Address, 1, PAGE_EXECUTE_READWRITE, &oldProtection);
		*(char*)Address = Instruction;
		VirtualProtect((LPVOID)Address, 1, oldProtection, &oldProtection);
	}

	DWORD RetcheckFlag1, RetcheckFlag2;
	DWORD Retcheck = ScanAddress(RETCHECK);

	void Retcheck_Disable() {
		RetcheckFlag1 = *(DWORD*)ScanAddress(RETCHECK_FLAG1);
		RetcheckFlag2 = *(DWORD*)ScanAddress(RETCHECK_FLAG2);
		WriteMemory(Retcheck, 0xC3);
	}

	void Retcheck_Enable() {
		*(DWORD*)ScanAddress(RETCHECK_FLAG1) = RetcheckFlag1;
		*(DWORD*)ScanAddress(RETCHECK_FLAG2) = RetcheckFlag2;
		WriteMemory(Retcheck, 0x55);
	}

	DWORD Decrypt_ptr(DWORD Pointer) {
		return (*(DWORD*)((char*)Pointer + *(DWORD*)Pointer))--;
	}

	void Disable_Callcheck() {
		DWORD Flag = ScanAddress(RLUA_PRECALL_FLAG);
		*(DWORD *)(Flag - (DWORD)&Flag) |= *(DWORD *)(Flag - (DWORD)&Flag) + 1;
		//Decrypt_ptr(Flag);
	}

	void getfield(DWORD luaState, DWORD index, const char *k) {
		//Retcheck_Disable();
		_getfield(luaState, index, k);
		//Retcheck_Enable();
	}

	void settop(DWORD luaState, DWORD s) {
		//Retcheck_Disable();
		_settop(luaState, s);
		//Retcheck_Enable();
	}

	void pcall(DWORD luaState, DWORD nargs, DWORD nresults, DWORD errfunc) {
		//Retcheck_Disable();
		//_pcall(luaState, nargs, nresults, errfunc);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(ScanAddress(0x71b007)), "\xEB", 1, 0);
		_pcall(luaState, nargs, nresults, errfunc);
		WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(ScanAddress(0x71b007)), "\x74", 1, 0);
	}

	void pushstring(DWORD luaState, const char *s) {
		//Retcheck_Disable();
		_pushstring(luaState, s);
		//Retcheck_Enable();
	}

	void pushvalue(DWORD luaState, DWORD index) {
		//Retcheck_Disable();
		_pushvalue(luaState, index);
		//Retcheck_Enable();
	}

	DWORD newthread(DWORD luaState) {
		//Retcheck_Disable();
		DWORD x = _newthread(luaState);
		//Retcheck_Enable();
		return x;
	}

	void pushnumber(DWORD luaState, double n) {
		//Retcheck_Disable();
		_pushnumber(luaState, n);
		//Retcheck_Enable();
	}

	void pushcclosure(DWORD luaState, void* func, DWORD n) {
		//Retcheck_Disable();
		_pushcclosure(luaState, func, n);
		//Retcheck_Enable();
	}

	void pushbool(DWORD State, bool S) {
		//Retcheck_Disable();
		//_pushbool(State, S);
		//Retcheck_Enable();
	}

	void pop(int State, int n) {
		return settop(State, -(n)-1);
	}

	void pushtrue(int State) {
		*(DWORD *)(*(DWORD *)(State + 16) + 8) = 1;
		*(DWORD *)(State + 16) += 16;
	}

	const char* tostring(DWORD luaState, DWORD index) {
		//Retcheck_Disable();
		const char* result = _tolstring(luaState, index, 0);
		//Retcheck_Enable();
		return result;
	}

	void getglobal(DWORD luaState, const char *k) {
		return getfield(luaState, -10002, k);
	}

	void pushnil(int State) {
		*(DWORD *)(*(DWORD *)(State + 16) + 8) = 0;
		*(DWORD *)(State + 16) += 16;
	}

	void pushcfunction(DWORD luaState, void* func) {
		return pushcclosure(luaState, func, 0);
	}

	void clearstack(DWORD luaState) {
		return settop(luaState, 0);
	}

	void setfield(DWORD luaState, DWORD index, const char *k) {
		//Retcheck_Disable();
		//_setfield(luaState, index, k);
		//Retcheck_Enable();
		pushvalue(luaState, index);
		if (getmetatable(luaState, -1)) {
			getfield(luaState, -1, "__newindex");
			pushvalue(luaState, -3);
			pushstring(luaState, k);
			pushvalue(luaState, -6);
			pcall(luaState, 3, 0, 0);
			pop(luaState, 3);
		}
		else {
			pop(luaState, 1);
			_setfield(luaState, index, k);
		}
	}
}
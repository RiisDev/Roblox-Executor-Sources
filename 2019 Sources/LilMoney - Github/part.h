#include "Console.hpp"
#include <Windows.h>
#include <iostream>
#include "RetCheck.hpp"

extern "C" {
#include "Lua\lua.h"
#include "Lua\lua.hpp"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\llimits.h"
#include "Lua/lvm.h"
#include "Lua/lopcodes.h"
}

DWORD RobloxState;
lua_State* LuaState;

#define TOP 12
#define BASE 28

#define DeclareAddress(x) (x - 0x400000 + (DWORD)GetModuleHandleA(0))

class RobloxModKeys {
public:
	int UserData = 0,
		TString = 2,
		TValue = 3,
		LClosure = 4,
		UpValue = 5,
		Proto = 6,
		Thread = 7,
		Table = 8;
};
RobloxModKeys ModKeys;

#define ASLR(x) (x - 0x400000 + (DWORD)GetModuleHandleA(0))
#define Declare(address, returnValue, callingConvention, ...) (returnValue(callingConvention*)(__VA_ARGS__))(unprotect(ASLR(address)))
#define ttval(val) ((TValue*)(val))

DWORD XorConstant = (DWORD)(DWORD*)(DeclareAddress(0x20C7720));

auto r_luaV_gettable = (void(__cdecl*)(DWORD, DWORD*, DWORD*, DWORD, int))(unprotect(ASLR(0x82BD70)));

typedef DWORD(__fastcall* _r_luaS_newlstr)(DWORD rL, const char* str, size_t B);
_r_luaS_newlstr r_luaS_newlstr = (_r_luaS_newlstr)(DeclareAddress(0x828290));

typedef void*(__cdecl* _r_luaM_realloc_)(DWORD rL, void *block, size_t osize, size_t nsize);
_r_luaM_realloc_ r_luaM_realloc_ = (_r_luaM_realloc_)(DeclareAddress(0x827c10));
#define r_luaM_malloc(rL, t, m)  r_luaM_realloc_(rL, NULL, 0, (t))

typedef int(__cdecl* PreCallTypedef)(DWORD rL, DWORD a1, DWORD a2);
PreCallTypedef PreCallOffsetUnprotected = (PreCallTypedef)(DeclareAddress(0x825190));

int r_luaD_precall(DWORD a, DWORD b, DWORD c) {
	signed int ReturnValue = PreCallOffsetUnprotected(a, b, c);
	return ReturnValue;
}

auto r_lua_newthread = Declare(0x82D740, int, __cdecl, DWORD a1);
auto r_lua_index2adr = Declare(0x823ae0, TValue*, __stdcall, DWORD a1, int idx);
auto r_lua_getfield = Declare(0x82caf0, void, __stdcall, int a1, int idx, const char *key);
auto r_lua_settop = Declare(0x82F280, void, __fastcall, DWORD a1, int idx);

//auto r_luaD_precall = Declare(0x825190, signed int, __cdecl, DWORD rL, DWORD A, DWORD B);

static int SandboxThread = ASLR(0x813b40);
typedef int(__cdecl* rSandboxThread_def)(int, int, int);
static rSandboxThread_def r_sandbox_thread = (rSandboxThread_def)SandboxThread;

//StkId GetRobloxStack(DWORD Roblox, DWORD IndexKey) {
//	StkId RobloxBase = (StkId)(Roblox + BASE);
//	StkId ValueFromStack = (RobloxBase + IndexKey);
//	return ValueFromStack;
//}

DWORD GetRobloxStack(DWORD rL, DWORD idx)
{
	return *(DWORD*)(rL + BASE) + TOP * idx;
}

#define OFF_CI 8
#define OFF_CI_TOP 16

#define TSTRING_SIZE 24
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

bool ConvertOpCodes = true;

BOOL LuaToRobloxConstantConversion(lua_State* L, DWORD rL, TValue* From, TValue* Destination)
{
	if (From == Destination)
	{
		SuspendThread(GetCurrentThread());
	}
	switch (From->tt)
	{
	case LUA_TNIL:
	{
		Destination->tt = R_LUA_TNIL;
		Destination->value.p = NULL;
		break;
	}

	case LUA_TBOOLEAN:
	{
		Destination->tt = R_LUA_TBOOLEAN;
		Destination->value.b = From->value.b;
		break;
	}

	case LUA_TNUMBER:
	{
		double newnumber;
		void* oldnumber = &(From->value.n);
		Destination->tt = R_LUA_TNUMBER;
		__asm
		{
			mov edx, oldnumber
			mov eax, XorConstant
			movsd xmm1, qword ptr[edx]
			xorpd xmm1, [eax]
			movsd newnumber, xmm1
		}
		Destination->value.n = newnumber;
		break;
	}

	case LUA_TSTRING:
	{
		TString* ts = (TString*)(From->value.gc);
		const char* str = (const char*)(ts + 1);
		Destination->tt = R_LUA_TSTRING;
		Destination->value.gc = (GCObject*)r_luaS_newlstr(rL, str, ts->tsv.len);
		break;
	}

	case LUA_TLIGHTUSERDATA: {
		Destination->tt = R_LUA_TLIGHTUSERDATA;
		Destination->value.p = From->value.p;
		break;
	}

	case LUA_TUSERDATA: {
		Destination->tt = R_LUA_TUSERDATA;
		Destination->value.gc = From->value.gc;
		break;
	}

	}
	return TRUE;
}

void VMExecute(lua_State* Lua, DWORD Roblox, int NumberOfCalls) {
	LClosure* VMLuaClosure;
	StkId VMLuaBase;
	TValue* VMLuaConstants;
	TValue* ConstantPool;
	const Instruction *VMLuaInstruction;
	Proto* ProtoFromClosure;

	std::cout << ired << "Creating VM" << std::endl;

VMEntryPoint:
	lua_assert(isLua(Lua->ci));
	VMLuaInstruction = Lua->savedpc;
	VMLuaClosure = &clvalue(Lua->ci->func)->l;
	VMLuaBase = Lua->base;
	ProtoFromClosure = VMLuaClosure->p;
	ConstantPool = ProtoFromClosure->k;

	std::cout << ired << "Set values" << std::endl;

	DWORD RobloxBaseOffset = *(DWORD*)(Roblox + BASE);
	DWORD RobloxTopOffset = *(DWORD*)(Roblox + TOP);
	*(DWORD*)(Roblox + TOP) += ProtoFromClosure->maxstacksize * 16 + 32;

	VMLuaConstants = (TValue*)r_luaM_malloc(Roblox, ProtoFromClosure->sizek * sizeof(TValue));
	for (int Iterator = 0; Iterator < ProtoFromClosure->sizek; Iterator++) {
		LuaToRobloxConstantConversion(Lua, Roblox, &ProtoFromClosure->k[Iterator], &(VMLuaConstants[Iterator]));
	}

	std::cout << ired << "Converted TValues" << std::endl;

	while(ConvertOpCodes) {
		const Instruction CurrentInstruction = *VMLuaInstruction++;
		std::cout << iblue << "[" << iyellow << "Running instruction" << iblue << "] " << ired << luaP_opnames[GET_OPCODE(CurrentInstruction)] << std::endl;

		switch (GET_OPCODE(CurrentInstruction)) {
			case OP_GETGLOBAL: {
				TValue* LuaArgument = &(VMLuaConstants)[GETARG_Bx(CurrentInstruction)];
				DWORD RobloxArgument = (DWORD)GetRobloxStack(Roblox, GETARG_A(CurrentInstruction));

				const char* GetGlobalKey = (const char*)(((uintptr_t)LuaArgument->value.gc) + TSTRING_SIZE);
				std::cout << iblue << "GetGlobal Key: " << iyellow << GetGlobalKey << std::endl;

				r_luaV_gettable(Roblox, (DWORD*)r_lua_index2adr(Roblox, LUA_GLOBALSINDEX), (DWORD*)LuaArgument, RobloxArgument, 1);
				continue;
			}
			case OP_LOADK: {
				DWORD RobloxArgument = (DWORD)GetRobloxStack(Roblox, GETARG_A(CurrentInstruction));
				TValue* Constant = &(VMLuaConstants[GETARG_Bx(CurrentInstruction)]);
				memcpy((void*)RobloxArgument, (void*)Constant, sizeof(TValue));
				//setobj2s(Lua, (TValue*)RobloxState, Constant);
				continue;
			}
			case OP_CALL: {
				DWORD RobloxArgument = (DWORD)GetRobloxStack(Roblox, GETARG_A(CurrentInstruction));
				int LuaArgument = GETARG_B(CurrentInstruction);
				int NumberOfResults = GETARG_C(CurrentInstruction) - 1;
				if (LuaArgument != 0) {
					RobloxTopOffset = (DWORD)(ttval(RobloxArgument) + LuaArgument);
				}
				Lua->savedpc = VMLuaInstruction;
				int PreCallReply = PreCallOffsetUnprotected(Roblox, RobloxArgument, NumberOfResults);
				switch (PreCallReply) {
					case 0: {
						NumberOfCalls++;
						goto VMEntryPoint;
					}
					case 1: {
						if (NumberOfResults >= 0) {
							*(DWORD*)(Roblox + TOP) = *(DWORD*)(*(DWORD*)(Roblox + OFF_CI) + OFF_CI_TOP);
						}
						continue;
					}
					default: {
						return;
					}
				}
				continue;
			}
			//case OP_CLOSURE: {
			//	r_Proto *p;
			//	r_Closure *ncl;
			//	int nup, j;
			//	p = cl->p->p[GETARG_Bx(i)];
			//	nup = p->nups;
			//	ncl = r_luaF_newLclosure(L, nup, cl->env);
			//	ncl->l.p = p;
			//	for (j = 0; j < nup; j++, pc++) {
			//		if (GET_OPCODE(*pc) == OP_GETUPVAL)
			//			ncl->l.upvals[j] = cl->upvals[GETARG_B(*pc)];
			//		else {
			//			lua_assert(GET_OPCODE(*pc) == OP_MOVE);
			//			ncl->l.upvals[j] = r_luaF_findupval(L, base + GETARG_B(*pc));
			//		}
			//	}
			//	setclvalue(L, ra, ncl);
			//	Protect(luaC_checkGC(L));
			//	continue;
			//}

			case OP_RETURN: {
				return;
			}
		}
	}
}

void RunScriptOnVM(DWORD Roblox, lua_State* Lua, std::string ScriptToRun) {
	luaL_loadstring(Lua, ScriptToRun.c_str());
	if (!luaD_precall(Lua, Lua->top - 1, 0)) {
		VMExecute(Lua, Roblox, 1);
	}
}

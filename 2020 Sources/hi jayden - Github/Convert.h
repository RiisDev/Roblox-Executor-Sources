#pragma once
#include "RBX.hpp"

/*  Include Lua Files  */

#include "LuauInstConvert.h"


using luamReallocFn = void* (__cdecl*)(uintptr_t rL, void*, void*, size_t, size_t);
luamReallocFn r_luaM_realloc_ = reinterpret_cast<luamReallocFn>(SignatureFind::Addresses::RobloxBase(0x7c3fe0));

#define R_LUA_TNIL 0
#define R_LUA_TLIGHTUSERDATA 2
#define R_LUA_TNUMBER 3
#define R_LUA_TBOOLEAN 1
#define R_LUA_TSTRING 4
#define R_LUA_TTHREAD 8
#define R_LUA_TFUNCTION 7
#define R_LUA_TTABLE 6
#define R_LUA_TUSERDATA 5
#define R_LUA_TPROTO 9
#define R_LUA_TUPVALUE 10

DWORD DXorKey = SignatureFind::Addresses::RobloxBase(0x2065CB0);

zx3::RbxLua RLua;


double XorDouble(double num)
{
	uint64_t U64_Xor = *reinterpret_cast<uint64_t*>(&num) ^ *reinterpret_cast<uint64_t*>(DXorKey);
	return *reinterpret_cast<double*>(&U64_Xor);
}

using _BYTE = BYTE;

namespace zx3
{
	namespace Conversion
	{
		/* Convert start off proto to vanilla, vanilla to proto  */
		DWORD ConvertProto(DWORD rL, Proto* p)
		{
			DWORD rp = RLua.luaF_newproto(rL);
			zx3::Obfuscation::ObfuscateProto(rp + 12, reinterpret_cast<uintptr_t>(RLua.luaS_newlstr(rL, getstr(p->source), strlen(getstr(p->source)))));

			auto instrs = StartConvert.ConvertStart(p->code, p->sizecode);

			*reinterpret_cast<uintptr_t*>(rp + 48) = p->sizecode;
			uintptr_t* code = reinterpret_cast<uintptr_t*>(r_luaM_realloc_(rL, 0, 0, 4 * instrs.size(), 4));
			zx3::Obfuscation::ObfuscateProto(rp + 32, reinterpret_cast<uintptr_t>(code));

			*reinterpret_cast<uintptr_t*>(rp + 8) = p->sizek;
			TValue* r_k = reinterpret_cast<TValue*>(r_luaM_realloc_(rL, 0, 0, 16 * p->sizek, 4));
			zx3::Obfuscation::ObfuscateProto(rp + 40, reinterpret_cast<std::uintptr_t>(r_k));

			*reinterpret_cast<uintptr_t*>(rp + 76) = p->sizep;
			DWORD* r_p = reinterpret_cast<DWORD*>(r_luaM_realloc_(rL, 0, 0, 4 * p->sizep, 4));
			zx3::Obfuscation::ObfuscateProto(rp + 20, reinterpret_cast<std::uintptr_t>(r_p));

			*reinterpret_cast<uintptr_t*>(rp + 64) = p->sizelineinfo;
			DWORD* lineinfo = reinterpret_cast<DWORD*>(r_luaM_realloc_(rL, 0, 0, 4 * p->sizelineinfo, 4));
			zx3::Obfuscation::ObfuscateProto(rp + 36, reinterpret_cast<std::uintptr_t>(lineinfo));

			for (int i = 0; i < instrs.size(); i++)
			{
				code[i] = (uintptr_t)instrs[i];
			}

			for (int i = 0; i < p->sizek; i++)
			{
				TValue* vk = &p->k[i];
				TValue* k = &r_k[i];

				switch (vk->tt)
				{
				case LUA_TNIL:
					break;
				case LUA_TNUMBER:
					k->tt = R_LUA_TNUMBER;
					k->value.n = XorDouble(vk->value.n);
					break;
				case LUA_TBOOLEAN:
					k->tt = R_LUA_TBOOLEAN;
					k->value.b = vk->value.b;
					break;
				case LUA_TSTRING:
				{
					TString* ts = (TString*)(vk->value.gc);
					const char* str = (const char*)(ts + 1);
					k->tt = R_LUA_TSTRING;
					k->value.gc = (GCObject*)(RLua.luaS_newlstr(rL, str, ts->tsv.len));
					break;
				}

				}
			}

			for (int i = 0; i < p->sizep; i++)
			{
				r_p[i] = ConvertProto(rL, p->p[i]);
			}

			for (int i = 0; i < p->sizelineinfo; i++)
			{
				lineinfo[i] = p->lineinfo[i];
			}
			*(_BYTE*)(rp + 83) = p->maxstacksize;
			*(_BYTE*)(rp + 82) = p->is_vararg;
			*(_BYTE*)(rp + 81) = p->nups;
			*(_BYTE*)(rp + 80) = p->numparams;

			return rp;
		}
		VOID PushAsync(uintptr_t rL, DWORD Function)
		{

			DWORD top = *(DWORD*)(rL + 12);
			*(DWORD*)(top + 8) = R_LUA_TFUNCTION;
			*(DWORD*)(top) = Function;
			*(DWORD*)(rL + 12) += 16;
			r_spawn(rL);
		}

		void SetClosure(uintptr_t rL, Proto* p)
		{
			DWORD rp = ConvertProto(rL, p);
			DWORD rlcl = (DWORD)RLua.luaF_newLClosure(rL, 0, *(int*)(rL + 72), rp);
			PushAsync(rL, rlcl);
		}

		void ExecuteScript(lua_State* L, DWORD rL, const std::string& Source)
		{
			std::string Script;
			Script = "spawn(function() local script = Instance.new('LocalScript', nil) script.Name = '666h' script.Disabled = true\r\n";
			Script += Source;
			Script += "\r\nend)"; //add creation of localscript to the start of the script



			if (luaL_loadstring(L, Source.c_str()))
			{
				std::cout << "--> Error; " << lua_tostring(L, -1) << std::endl;
				lua_pop(L, 1);
			}
			else
			{
				TValue* top = L->top - 1;
				const LClosure* lcl = reinterpret_cast<const LClosure*>(top->value.gc);
				Proto* p = lcl->p;
				SetClosure(rL, p);
			}
		}
	}
}
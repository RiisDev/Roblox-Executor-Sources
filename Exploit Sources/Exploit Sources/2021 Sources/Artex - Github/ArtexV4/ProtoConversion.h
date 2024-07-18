#pragma once
#include "RbxConversion.hpp"




 #define r_sl_obfusticate(p, v) *(DWORD *)(p) = (DWORD)v - (p)

constexpr void obfusticate_proto_name(DWORD p, DWORD* name)
{
	r_sl_obfusticate(p, name); 
}
void set_name(DWORD rL, const char* name)
{
	obfusticate_proto_name(rL + 40, (DWORD*)luaS_newlstr(rL, name, strlen(name)));
}

namespace RBX
{
	namespace Conversion
	{
		r_TValue ConvertConstant(std::uintptr_t r_L, TValue lv)
		{
			r_TValue rv = r_TValue();
			switch (lv.tt)
			{
			case LUA_TNIL:
			{
				rv.tt = R_LUA_TNIL;
				break;
			}
			case LUA_TNUMBER:
			{
				rv.tt = R_LUA_TNUMBER;
				rv.value.n = r_xor_number(&lv.value.n);
				break;
			}
			case LUA_TBOOLEAN:
			{
				rv.tt = R_LUA_TBOOLEAN;
				rv.value.b = lv.value.b;
				break;
			}
			case LUA_TSTRING:
			{
				const char* str = getstr(&lv.value.gc->ts.tsv);
				rv.tt = R_LUA_TSTRING;
				rv.value.p = luaS_newlstr(r_L, str, lv.value.gc->ts.tsv.len);
				break;
			}
			}
			return rv;
		}


	    uintptr_t ConvertProto(lua_State* L, std::uintptr_t r_L, Proto* p)
		{
			
			std::unique_ptr<ServerProto> P = std::make_unique<ServerProto>(r_luaF_newProto(r_L));

			P->source = reinterpret_cast<TString*>(luaS_newlstr(r_L, "@", 1));

			

			P->maxstacksize = p->maxstacksize;
			P->numparams = p->numparams;
			P->nups = p->nups;
			P->is_vararg = p->is_vararg;

			std::vector<int> new_lineinfo;
			auto translate_ = push_instrs(p, new_lineinfo);


			P->sizecode = translate_.size();
			P->sizek = p->sizek;
			P->sizep = p->sizep;
			P->sizelineinfo = translate_.size();
			P->sizelinecomp_info = P->sizelineinfo;

			P->p = reinterpret_cast<DWORD*>(r_luaM_realloc_(r_L, 4 * P->sizep));
			P->code = reinterpret_cast<Instruction*>(r_luaM_realloc_(r_L, 4 * translate_.size()));
			P->k = reinterpret_cast<r_TValue*>(r_luaM_realloc_(r_L, 16 * P->sizek));
			P->lineinfo = reinterpret_cast<int*>(r_luaM_realloc_(r_L, sizeof(int) * P->sizelineinfo));//this omes second//
			P->lineinfo_notcomp = reinterpret_cast<int*>(r_luaM_realloc_(r_L, sizeof(int) * translate_.size())); //this comes first//

			for (auto i = 0; i < P->sizecode; ++i)
			{
				P->code[i] = static_cast<Instruction>(translate_[i]);
			}

			for (auto i = 0; i < P->sizek; ++i)
			{
				P->k[i] = ConvertConstant(r_L, p->k[i]);
			}

			for (auto i = 0; i < P->sizep; ++i)
			{
				P->p[i] = Conversion::ConvertProto(L, r_L, p->p[i]);
			}

			int j = 0;
			for (int i = 0; i < translate_.size(); ++i)
			{
				P->lineinfo_notcomp[i] = new_lineinfo[i];
				j = P->lineinfo[i] - 1;//P->lineinfo[i];
			}

			return P->R_Proto;
		}
	}
}
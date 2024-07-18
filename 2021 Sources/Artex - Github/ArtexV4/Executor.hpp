#pragma once
#include "ProtoConversion.h"

using namespace RBX; // Less work.

namespace ProtoExecutor
{
	void Convert(std::uintptr_t r_L, std::string source, std::string chunkName)
	{
		const auto L = luaL_newstate();
		if (luaL_loadbuffer(L, source.c_str(), source.length(), chunkName.c_str()))
		{
			const char* err = lua_tostring(L, -1);
			lua_pop(L, 1);
			throw std::exception(err);
		}
		const auto cl = reinterpret_cast<LClosure*>((L->top - 1)->value.p);
		size_t nups = 0;
		const auto rp = RBX::Conversion::ConvertProto(L, r_L, cl->p);
		const auto rcl = RBX::r_luaF_newLClosure(r_L, nups, *reinterpret_cast<std::uintptr_t*>(r_L + RBX::Offsets::l_gt), rp);
		auto top = *reinterpret_cast<RBX::r_TValue**>(r_L + RBX::Offsets::l_top);
		top->tt = R_LUA_TFUNCTION;
		top->value.p = reinterpret_cast<void*>(rcl);
		(*reinterpret_cast<RBX::r_TValue**>(r_L + RBX::Offsets::l_top))++;
	}

	void ExecuteScript(std::uintptr_t r_State, std::string source, bool internal, std::string chunkName)
	{
		//source = "spawn(function() script = Instance.new('LocalScript') " + std::string(source) + "\r\nend) \r\n";
		const auto thread = RBX::r_lua_newthread(r_State);
		Convert(thread, source, chunkName.c_str());
		RBX::spawn(thread);
	}
}
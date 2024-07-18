#include "instructionConversion.hpp"
#include "Utilities.h"
#include <iomanip>
#include <string>
#include <iostream>
#include <sstream>

script_ware::Instructions StartInstructions;

std::vector<UInstruction> LuaUConvert(lua_State* L, Proto* P)
{
	const auto Conv = StartInstructions.ConvertStart(P->code, P->sizecode);

	P->sizecode = Conv.size();
	P->code = luaM_newvector(L, Conv.size(), Instruction);

	for (auto i = 0; i < Conv.size(); i++)
		P->code[i] = (Instruction)Conv[i];

	for (auto i = 0; i < P->sizep; i++)
		LuaUConvert(L, P->p[i]);

	return Conv;
}

namespace script_ware
{
	namespace Serializer
	{

		void write_table(std::stringstream& ss, Proto* p)
		{
			std::vector<lua_TValue*> kConstant;
			kConstant.clear();

			for (int i = 0; i < p->sizek; i++)
			{
				TValue* k = &p->k[i];
				if (k->tt == LUA_TSTRING)
				{
					kConstant.push_back(k);
				}
			}

			stream_write.writeCompressedInt(ss, kConstant.size()); /* Write string table size */

			for (auto& str : kConstant)
			{
				std::string Value = getstr(&str->value.gc->ts);
				std::cout << "STRING: " << Value << std::endl;
				stream_write.writeCompressedInt(ss, Value.length());
				for (auto& x : Value)
					stream_write.writeByte(ss, x);
			}

		}

	}
}

void main()
{
	SetConsoleTitle("Script-Ware Beta // Serializer");

	auto* L = lua_open();


	std::string Source;
	while (std::getline(std::cin, Source))
	{
		const auto error = luaL_loadbuffer(L, Source.c_str(), Source.length(), "@");
		if (error == 0)
		{
			const auto* cl = static_cast<const LClosure*>(lua_topointer(L, -1));
			auto* P = cl->p;
			auto instrs = LuaUConvert(L, P);

			std::stringstream ss;

			stream_write.writeByte(ss, 0x1); //luau-bit
			script_ware::Serializer::write_table(ss, P);
			stream_write.writeByte(ss, instrs.size());

			for (auto i = 0; i < instrs.size(); i++)
			{
				stream_write.writeInt(ss, (Instruction)instrs[i]);
			}

			std::string out = ss.str();
			for (byte x : out)
			{
				std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)x << " ";
			}
		}
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH) {
		DWORD c = 0;
		VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, &c);
		*reinterpret_cast<UINT*>(FreeConsole) = 0xC3;
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)main, nullptr, 0, nullptr);
	}
	return 1;
}
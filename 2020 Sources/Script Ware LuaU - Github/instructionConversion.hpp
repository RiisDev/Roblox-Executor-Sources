#pragma once
#include <vector>
#include "utils_to_insert.hpp"

namespace script_ware
{
	class Instructions
	{
	public:
		static __forceinline std::vector<UInstruction> ConvertStart(Instruction* Instructions, size_t Size)
		{
			auto VanillaInstrs = std::vector<Instruction>(Instructions, Instructions + Size);
			auto LuaUInstrs = std::vector<UInstruction>();



			for (auto inster = 0; inster < VanillaInstrs.size(); inster++)
			{
				const auto Instr = VanillaInstrs[inster];

				const auto Opc = GET_OPCODE(Instr);

				LuaUInstrs.emplace_back(insertInst->generateABC(UOpCode::CLEARSTACK, GETARG_A(Instr), GETARG_B(Instr), GETARG_C(Instr)));

				printf("LUA_OP: %s\n", luaP_opnames[Opc]);

				switch (Opc)
				{
				case OP_MOVE:
					LuaUInstrs.emplace_back(insertInst->generateABx(UOpCode::MOVE, GETARG_A(Instr), GETARG_Bx(Instr)));
					break;
				case OP_LOADK:
					LuaUInstrs.emplace_back(insertInst->generateABx(UOpCode::LOADK, GETARG_A(Instr), GETARG_Bx(Instr)));
					break;
				case OP_GETGLOBAL:
					LuaUInstrs.emplace_back(insertInst->generateABx(UOpCode::GETGLOBAL, GETARG_A(Instr), GETARG_Bx(Instr)));
					break;
				case OP_CALL:
					LuaUInstrs.emplace_back(insertInst->generateABC(UOpCode::CALL, GETARG_A(Instr), GETARG_B(Instr), GETARG_C(Instr)));
					break;
				case OP_RETURN:
					LuaUInstrs.emplace_back(insertInst->generateABC(UOpCode::RETURN, GETARG_A(Instr), GETARG_B(Instr), 0));
					break;
				}

			}

			return LuaUInstrs;
		}
	};
}


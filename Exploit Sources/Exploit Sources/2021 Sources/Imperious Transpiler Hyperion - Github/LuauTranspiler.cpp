
#include "LuauTranspiler.h"
#include <iostream>

/*Dummy this is an Hyperion Transpiler, Well idc, i Open-Source this to everyone
if you see this code like these, yea thats hyperion.
*/


void pandatranspiler::adjust_lines(std::vector<int>& line_info, int lineinfo, std::size_t new_instructions_amount)
{
	for (int i = 0; i < new_instructions_amount; ++i)
		line_info.push_back(lineinfo);
}

void pandatranspiler::convert_instructions(std::vector<r_instruction>& instruction_vector, std::vector<int>& line_info, Proto* proto)
{
	std::vector<relocation_index> relocation_vector;

	std::unordered_map<int, int> index_map;

	instruction_vector.emplace_back(r_opcode::op_initvararg, proto->numparams, 0, 0);
	adjust_lines(line_info, 0, 1);

	bool needs_close = false;

	for (auto i = 0; i < proto->sizecode; ++i)
	{
		int old_size = instruction_vector.size();

		index_map.insert({ i, instruction_vector.size() });

		const auto instruction = proto->code[i];

		const auto opcode = GET_OPCODE(instruction);

		switch (opcode)
		{
			// Table stuff
		case OP_GETTABLE:
		{
			instruction_vector.emplace_back(r_opcode::op_gettable, GETARG_A(instruction), GETARG_B(instruction), GETARG_C(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}

		case OP_SETTABLE:
		{
			instruction_vector.emplace_back(r_opcode::op_settable, GETARG_C(instruction), GETARG_A(instruction), GETARG_B(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}

		// Local/Upvalue/Closure stuff
		case OP_CLOSURE:
		{
			instruction_vector.emplace_back(r_opcode::op_closure, GETARG_A(instruction), static_cast<std::uint16_t>(GETARG_Bx(instruction)));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_MOVE:
		{
			instruction_vector.emplace_back(r_opcode::op_move, GETARG_A(instruction), GETARG_B(instruction), GETARG_C(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_CLOSE:
		{
			instruction_vector.emplace_back(r_opcode::op_close, GETARG_A(instruction), GETARG_B(instruction), GETARG_C(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_VARARG:
		{
			instruction_vector.emplace_back(r_opcode::op_vararg, GETARG_A(instruction), GETARG_B(instruction), GETARG_C(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_CLUPVAL:
		{
			instruction_vector.emplace_back(r_opcode::op_clupval, GETARG_A(instruction), GETARG_B(instruction), 0);
			adjust_lines(line_info, proto->lineinfo[i], 1);
			needs_close = true;
			break;
		}
		case OP_GETUPVAL:
		{
			instruction_vector.emplace_back(r_opcode::op_getupval, GETARG_A(instruction), GETARG_B(instruction), 0);
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_SETUPVAL:
		{
			instruction_vector.emplace_back(r_opcode::op_setupval, GETARG_A(instruction), GETARG_B(instruction), 0);
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}

		// Global Stuff
		case OP_GETGLOBAL:
		{
			instruction_vector.emplace_back(r_opcode::op_ugetglobal, GETARG_A(instruction), 0, 0);
			instruction_vector.emplace_back(GETARG_Bx(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 2);
			break;
		}
		case OP_SETGLOBAL:
		{
			instruction_vector.emplace_back(r_opcode::op_setglobal, GETARG_A(instruction), 0, 0);
			instruction_vector.emplace_back(GETARG_Bx(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 2);
			break;
		}

		// Stack Stuff
		case OP_LOADK:
		{
			instruction_vector.emplace_back(r_opcode::op_loadk, GETARG_A(instruction), static_cast<std::uint16_t>(GETARG_Bx(instruction)));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}

		case OP_LOADBOOL:
		{
			instruction_vector.emplace_back(r_opcode::op_loadbool, GETARG_A(instruction), GETARG_B(instruction), GETARG_C(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_RETURN:
		{
			if (needs_close)
				instruction_vector.emplace_back(r_opcode::op_close, 0, 0, 0);
			instruction_vector.emplace_back(r_opcode::op_return, GETARG_A(instruction), GETARG_B(instruction), 0);
			adjust_lines(line_info, proto->lineinfo[i], needs_close ? 2 : 1);
			break;
		}
		case OP_TAILCALL:
		case OP_CALL:
		{
			instruction_vector.emplace_back(r_opcode::op_call, GETARG_A(instruction), GETARG_B(instruction), GETARG_C(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_SETLIST: {
			instruction_vector.emplace_back(r_opcode::op_setlist, GETARG_A(instruction), GETARG_A(instruction) + 1, GETARG_B(instruction) == 0 ? 0 : GETARG_B(instruction) + 1);
			instruction_vector.emplace_back(static_cast<uint16_t>((GETARG_C(instruction) - 1) * LFIELDS_PER_FLUSH + 1)); // LFIELDS_PER_FLUSH modified to 16
			adjust_lines(line_info, proto->lineinfo[i], 2);

			break;
		}
		case OP_NEWTABLE:
		{
			instruction_vector.emplace_back(r_opcode::op_newtable, GETARG_A(instruction), 0, 0);
			instruction_vector.emplace_back(0);
			adjust_lines(line_info, proto->lineinfo[i], 2);
			break;
		}
		case OP_LOADNIL:
		{
			auto b = GETARG_B(instruction);

			int count = 0;

			while (b >= GETARG_A(instruction)) {
				++count;
				instruction_vector.emplace_back(r_opcode::op_loadnil, b--, 0, 0);
			}

			adjust_lines(line_info, proto->lineinfo[i], count);

			break;
		}
		case OP_SELF:
		{
			instruction_vector.emplace_back(r_opcode::op_self, GETARG_A(instruction), GETARG_B(instruction), 0);
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_PSUEDOSELF:
		{
			instruction_vector.emplace_back(GETARG_Bx(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}

		// Unary/Binary stuff
		case OP_NOT:
		{
			instruction_vector.emplace_back(r_opcode::op_not, GETARG_A(instruction), GETARG_B(instruction), 0);
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_UNM:
		{
			instruction_vector.emplace_back(r_opcode::op_unm, GETARG_A(instruction), GETARG_B(instruction), 0);
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_LEN:
		{
			instruction_vector.emplace_back(r_opcode::op_len, GETARG_A(instruction), GETARG_B(instruction), GETARG_C(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_CONCAT:
		{
			instruction_vector.emplace_back(r_opcode::op_concat, GETARG_A(instruction), GETARG_B(instruction), GETARG_C(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}

		// Math stuff
		case OP_MUL:
		{
			instruction_vector.emplace_back(r_opcode::op_mul, GETARG_A(instruction), GETARG_B(instruction), GETARG_C(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_ADD:
		{
			instruction_vector.emplace_back(r_opcode::op_add, GETARG_A(instruction), GETARG_B(instruction), GETARG_C(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_SUB:
		{
			instruction_vector.emplace_back(r_opcode::op_sub, GETARG_A(instruction), GETARG_B(instruction), GETARG_C(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_DIV:
		{
			instruction_vector.emplace_back(r_opcode::op_div, GETARG_A(instruction), GETARG_B(instruction), GETARG_C(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_POW:
		{
			instruction_vector.emplace_back(r_opcode::op_pow, GETARG_A(instruction), GETARG_B(instruction), GETARG_C(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_MOD:
		{
			instruction_vector.emplace_back(r_opcode::op_mod, GETARG_A(instruction), GETARG_B(instruction), GETARG_C(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_ADD_EQ:
		{
			instruction_vector.emplace_back(r_opcode::op_add, GETARG_A(instruction), GETARG_A(instruction), GETARG_B(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_SUB_EQ:
		{
			instruction_vector.emplace_back(r_opcode::op_sub, GETARG_A(instruction), GETARG_A(instruction), GETARG_B(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_MUL_EQ:
		{
			instruction_vector.emplace_back(r_opcode::op_mul, GETARG_A(instruction), GETARG_A(instruction), GETARG_B(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_MOD_EQ:
		{
			instruction_vector.emplace_back(r_opcode::op_mod, GETARG_A(instruction), GETARG_A(instruction), GETARG_B(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_POW_EQ:
		{
			instruction_vector.emplace_back(r_opcode::op_pow, GETARG_A(instruction), GETARG_A(instruction), GETARG_B(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_DIV_EQ:
		{
			instruction_vector.emplace_back(r_opcode::op_div, GETARG_A(instruction), GETARG_A(instruction), GETARG_B(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}

		// Things needing relocated
		case OP_JMP:
		{
			instruction_vector.emplace_back(r_opcode::op_jmp, GETARG_A(instruction), static_cast<std::int16_t>(GETARG_sBx(instruction)));
			relocation_vector.emplace_back(i, instruction_vector.size() - 1, instruction, -1);
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}

		// Conditionals
		case OP_EQ:
		{
			instruction_vector.emplace_back(!GETARG_A(instruction) ? r_opcode::op_eq : r_opcode::op_neq, GETARG_B(instruction), static_cast<std::int16_t>(2));
			instruction_vector.emplace_back(GETARG_C(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 2);
			break;
		}
		case OP_LT:
		{
			instruction_vector.emplace_back(!GETARG_A(instruction) ? r_opcode::op_lt : r_opcode::op_gt, GETARG_B(instruction), static_cast<std::int16_t>(2));
			instruction_vector.emplace_back(GETARG_C(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 2);
			break;
		}
		case OP_LE:
		{
			instruction_vector.emplace_back(!GETARG_A(instruction) ? r_opcode::op_le : r_opcode::op_ge, GETARG_B(instruction), static_cast<std::int16_t>(2));
			instruction_vector.emplace_back(GETARG_C(instruction));
			adjust_lines(line_info, proto->lineinfo[i], 2);
			break;
		}
		case OP_TEST:
		{
			instruction_vector.emplace_back(GETARG_C(instruction) ? r_opcode::op_test : r_opcode::op_ntest, GETARG_A(instruction), static_cast<std::int16_t>(1)); //maybe need fix idk
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_TESTSET:
		{
			instruction_vector.emplace_back(GETARG_C(instruction) ? r_opcode::op_test : r_opcode::op_ntest, GETARG_B(instruction), static_cast<std::int16_t>(2));
			instruction_vector.emplace_back(r_opcode::op_move, GETARG_A(instruction), GETARG_B(instruction), 0);
			adjust_lines(line_info, proto->lineinfo[i], 2);
			break;
		}
		case OP_FORPREP: // Properly fixes limit step index order in compiler and retains external index so for i = 0,10 do i = 0 print(i) end doesnt freeze (not inf loop)
		{
			auto A = GETARG_A(instruction);

			instruction_vector.emplace_back(r_opcode::op_move, A + 3, A + 0, 0);
			instruction_vector.emplace_back(r_opcode::op_move, A + 0, A + 1, 0);
			instruction_vector.emplace_back(r_opcode::op_move, A + 1, A + 2, 0);
			instruction_vector.emplace_back(r_opcode::op_move, A + 2, A + 3, 0);

			instruction_vector.emplace_back(r_opcode::op_forprep, A, static_cast<int16_t>(0));
			relocation_vector.emplace_back(i, instruction_vector.size() - 1, instruction, 0);
			instruction_vector.emplace_back(r_opcode::op_move, A + 3, A + 2, 0);

			adjust_lines(line_info, proto->lineinfo[i], 6);
			break;
		}
		case OP_FORLOOP:
		{
			instruction_vector.emplace_back(r_opcode::op_forloop, GETARG_A(instruction), static_cast<int16_t>(0));
			relocation_vector.emplace_back(i, instruction_vector.size() - 1, instruction, -2);
			adjust_lines(line_info, proto->lineinfo[i], 1);
			break;
		}
		case OP_TFORLOOP:
		{
			instruction_vector.emplace_back(r_opcode::op_tforloop, GETARG_A(instruction), static_cast<int16_t>(2)); // fix to work with farjump later
			instruction_vector.emplace_back(GETARG_C(instruction));
			instruction_vector.emplace_back(r_opcode::op_jmp, GETARG_A(instruction), static_cast<std::int16_t>(1));
			adjust_lines(line_info, proto->lineinfo[i], 3);
			break;
		}

		default:
		{
			std::cout << "UNSUPPORTED OPCODE: " << luaP_opnames[opcode] << '\n';
			break;
		}
		}
	}

	for (auto& rel : relocation_vector) {
		const auto vanilla_jump = GETARG_sBx(rel.original_instruction) + rel.original_index + 1;

		auto index = index_map.find(vanilla_jump);

		if (index != index_map.end())
		{
			auto jump_difference = index->second - rel.luau_index;

			auto test = jump_difference + rel.shift;

			if (test > SHRT_MAX || test < SHRT_MIN)
			{
				instruction_vector[rel.luau_index] = r_instruction(r_opcode::op_longjmp, jump_difference + rel.shift);
			}
			else
			{
				instruction_vector[rel.luau_index].sBx = jump_difference + rel.shift;
			}
		}
		else
		{
			std::cout << "COULDNT RESOLVE JUMP\n\n\n\n\n\n";
		}
	}
}

#include "LuauSerializer.h"
#include "writer.h"

std::string pandatranspiler::script_to_bytecode(lua_State* ls, const std::string& script, bool& compiled)
{
	if (!luaL_loadbuffer(ls, script.c_str(), script.size(), "=hyperion"))
	{
		compiled = true;

		std::unordered_map<TString*, int> string_map;
		std::unordered_map<Proto*, int> proto_map;
		std::vector<Proto*> proto_vector;
		std::vector<std::string> string_vector;

		pandawriter::writer stream;

		const auto main_proto = clvalue(ls->top - 1)->l.p;

		stream.write<bool>(true);

		std::function<void(Proto*)> get_protos = [&](Proto* p) -> void {
			for (int i = 0; i < p->sizep; ++i) {
				get_protos(p->p[i]);
			}
			proto_map[p] = proto_vector.size();
			proto_vector.push_back(p);
		};

		get_protos(main_proto);

		std::function<void(Proto*)> get_strings = [&](Proto* p) -> void {
			for (auto i = 0; i < p->sizek; ++i) {
				if (p->k[i].tt == LUA_TSTRING) {
					const auto tstring = &p->k[i].value.gc->ts;
					if (string_map.find(tstring) == string_map.end()) {
						string_map[tstring] = string_map.size() + 1;
						string_vector.emplace_back(getstr(tstring), tstring->tsv.len);
					}
				}
			}

			for (int i = 0; i < p->sizep; ++i) {
				get_strings(p->p[i]);
			}
		};

		get_strings(main_proto);

		stream.write_compressed_int(string_vector.size());

		for (const auto& str : string_vector)
		{
			stream.write_string(str);
		}

		stream.write_compressed_int(proto_vector.size());

		for (const auto& proto : proto_vector)
		{
			stream.write<std::uint8_t>(proto->maxstacksize);

			stream.write<std::uint8_t>(proto->numparams);

			stream.write<std::uint8_t>(proto->nups);

			stream.write<std::uint8_t>(proto->is_vararg);

			std::vector<r_instruction> instructions;

			std::vector<int> line_info;

			convert_instructions(instructions, line_info, proto);

			stream.write_compressed_int(instructions.size());

			for (const auto& instruction : instructions)
				stream.write<int>(instruction.Value);

			stream.write_compressed_int(proto->sizek);

			for (auto i = 0; i < proto->sizek; ++i)
			{
				const auto t_val = &proto->k[i];
				switch (t_val->tt) {
				case LUA_TNIL: {
					stream.write<std::uint8_t>(0);
					break;
				}
				case LUA_TBOOLEAN: {
					stream.write<std::uint8_t>(1);
					stream.write<std::uint8_t>(bvalue(t_val));
					break;
				}
				case LUA_TNUMBER: {
					stream.write<std::uint8_t>(2);
					stream.write<double>(nvalue(t_val));
					break;
				}
				case LUA_TSTRING: {
					stream.write<std::uint8_t>(3);
					stream.write_compressed_int(string_map[rawtsvalue(t_val)]);
					break;
				}
				}
			}

			stream.write_compressed_int(proto->sizep);

			for (auto i = 0; i < proto->sizep; ++i)
				stream.write_compressed_int(proto_map[proto->p[i]]);

			stream.write<bool>(false);
			stream.write<bool>(true);
			stream.write<bool>(false);

			auto lline = 0;

			for (const auto& line : line_info)
				stream.write<std::uint8_t>(0);

			for (const auto& line : line_info)
			{
				stream.write(line - lline);
				lline = line;
			}

			stream.write<bool>(false);

		}

		stream.write_compressed_int(proto_vector.size() - 1);

		auto bytecode = stream.to_string();

		//std::vector<uint8_t> bytes;
		//for (auto x : bytecode)
		//	bytes.push_back(x);

		//for (auto& x : bytes)
		//	std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(x) << " ";

		return bytecode;
	}
	else
	{
		std::cout << lua_tostring(ls, -1);
		compiled = false;

		return lua_tostring(ls, -1);
	}
}

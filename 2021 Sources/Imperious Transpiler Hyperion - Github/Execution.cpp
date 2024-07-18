//#include "execution.h"
//#include "LuauSerializer.h"
//#include "../globals/globals.hpp"
//#include "../../transpiler/serializer/bytecode_serializer.hpp" 
//
//void pandatranspiler::execute_script(int rstate, const std::string& script, bool spawn, bool compiler)
//{
//	std::string buff;
//	buff += script;
//
//	if (compiler)
//	{
//		using namespace compiler;
//
//		std::unique_ptr<ir::ast::function> stmt = nullptr;
//		parser::parser p(script);
//
//		try
//		{
//			stmt = p.parse();
//			compiler::codegen::chunk::chunk ch(stmt.get());
//			compiler::codegen::codegen gen(ch);
//			gen.generate();
//
//			compiler::serializer::serializer s(ch);
//			s.serialize();
//
//			auto bytecode = s.return_str().to_string();
//
//			state rbstate(rstate);
//			if (spawn) //dont create new thread of spawn is true, maybe ill make seperate func later this looks bad
//				rbstate.new_thread();
//			rbstate.luau_load("=hyperion", bytecode);
//			if (spawn) //spawn the function or no, used for loadstring
//				rbstate.spawn();
//
//		}
//		catch (const compiler::error& e)
//		{
//			state rbstate(rstate);
//			hyperion::globals::lasterror = std::to_string(e.get_location().begin.line) + ":" + e.what();
//			rbstate.settop(0);
//			rbstate.getglobal("compilererror");
//			rbstate.spawn();
//
//			std::cout << "compiler_exception: " << e.what() << "\t" << e.get_location().begin.line + 1 << ':' << e.get_location().begin.column + 1 << '\n';
//		}
//	}
//	else
//	{
//		auto ls = lua_open();
//		bool compilation_status = false;
//		auto bytecode = hyperion::conversion::script_to_bytecode(ls, buff, compilation_status);
//
//		if (compilation_status)
//		{
//			state rbstate(rstate);
//
//			if (spawn) //dont create new thread of spawn is true, maybe ill make seperate func later this looks bad
//				rbstate.new_thread();
//			rbstate.luau_load("=hyperion", bytecode);
//			if (spawn) //spawn the function or no, used for loadstring
//				rbstate.spawn();
//		}
//		else
//		{
//			state rbstate(rstate);
//			hyperion::globals::lasterror = bytecode;
//			rbstate.settop(0);
//			rbstate.getglobal("compilererror");
//			rbstate.spawn();
//		}
//	}
//}

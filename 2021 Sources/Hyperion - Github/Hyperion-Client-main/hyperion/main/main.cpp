#include "main.hpp"
#include <string>
#include <thread>
#include <websocket.h>
#include "..\roblox\bypasses\console\console.hpp"
#include "..\roblox\bypasses\callcheck\callcheck.hpp"
#include "../compiler/Parser/parser.hpp"
#include "../compiler/CodeGen/codegen.hpp"
#include "../utils/Error/error.hpp"
#include "../compiler/Serializer/serializer.hpp"
#include "../roblox/environment/environment.hpp"
#include "../roblox/globals/globals.hpp"
#include "../roblox/datamodel/datamodel.hpp"
#include "../renderer/renderer.hpp"
#include "../utils/request.hpp"
#include "../roblox/lualibs/objectmapper.hpp"

int islegal(int rstate)
{
	hyperion::lsm::luaStruct* t = reinterpret_cast<hyperion::lsm::luaStruct*>(hyperion::functions::touserdata(rstate, -1));
	std::cout << "b4\n";
	if (t == nullptr) {
		//luaL_error(ls, "Expected self (:) call.");
		return 0;
	}
	std::cout << "pushing\n";
	hyperion::functions::push_bool(rstate, t->getValue("age")->value.n >= 18 ? 1 : 0);
	return 1;
}

void hyperion::main::main_thread()
{
	auto rstate = initialize();
	hyperion::globals::rstate = rstate.return_state();

	std::string script;

	lsm::luaStruct* human = new lsm::luaStruct("human");

	human->create("sumthin", hyperion::types::r_lua_tstring, false);
	rstate.push_string("idfk"); //this gets gced
	human->setValue("sumthin", rstate, -1);

	human->create("age", hyperion::types::r_lua_tnumber, false);
	hyperion::functions::pushnumber(rstate.return_state(), 10);
	human->setValue("age", rstate, -1);

	human->create("gender", hyperion::types::r_lua_tstring, false);
	rstate.push_string("Male"); //this gets gced
	human->setValue("gender", rstate, -1);

	human->create("color", hyperion::types::r_lua_tstring, false);
	rstate.push_string("Black"); 
	human->setValue("color", rstate, -1);

	human->push(rstate);

	rstate.setglobal("human");

	human->create("isLegal", hyperion::types::r_lua_tfunction, true);

	hyperion::bypasses::callcheck::push_function(rstate, reinterpret_cast<uintptr_t>(islegal), 0);

	human->setValue("isLegal", rstate, -1);

	while (std::getline(std::cin, script))
	{
		if (!script.empty())
			srcs.push(script);
	}
}

DWORD input()
{
	std::string script_buffer;
	char buffer[999999];
	DWORD readWords;
	HANDLE pipeHandle = CreateNamedPipe(TEXT("\\\\.\\pipe\\HyperionPipe"), PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, PIPE_WAIT, 1, 999999, 999999, NMPWAIT_USE_DEFAULT_WAIT, 0);
	while (pipeHandle != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(pipeHandle, 0))
		{
			while (ReadFile(pipeHandle, buffer, sizeof(buffer) - 1, &readWords, 0))
			{
				buffer[readWords] = '\0';
				script_buffer.append(buffer);
				
			}
			srcs.push(script_buffer);
			script_buffer.clear();
		}

		DisconnectNamedPipe(pipeHandle);
	}
}

hyperion::state hyperion::main::initialize()
{
	try
	{
		init_http_lib();

		hyperion::state rstate;

		rstate.set_identity(8);

		hyperion::globals::gstate = hyperion::functions::get_globalstate(rstate.return_state());

		hyperion::bypasses::callcheck::start_callcheck_bypass(rstate);

		hyperion::globals::datamodel = hyperion::datamodel::get_datamodel();

		std::cout << "Datamodel: " << std::hex << hyperion::globals::datamodel << std::endl;

		*reinterpret_cast<uintptr_t*>((hyperion::globals::datamodel - hyperion::offsets::inst) + hyperion::offsets::dm_net) = 3;

		hyperion::renderer::dx_hook::hook_init();

		rstate.new_thread();

		rstate.sandboxthread();

		hyperion::functions::replace(rstate.return_state(), -10002);

		hyperion::functions::create_table(rstate.return_state(), 0, 0);
		rstate.setglobal("_G");

		hyperion::environment::init_functions(rstate);

		std::cout << "Environment pushed\n";

		hyperion::taskscheduler::init_script_scheduler(rstate.return_state());

		std::cout << "Script scheduler done\n";

		std::thread(input).detach();

		return rstate;
	}
	catch (const std::exception& e)
	{
		std::cout << "Hyperion is likely not updated: " << e.what() << std::endl;
	}
}

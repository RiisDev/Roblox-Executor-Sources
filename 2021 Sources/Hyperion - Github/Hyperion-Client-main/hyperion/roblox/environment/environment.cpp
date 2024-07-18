#include "environment.hpp"
#include <thread>
#include "../datamodel/datamodel.hpp"
#include "../globals/globals.hpp"
#include "../taskscheduler/taskscheduler.hpp"
#include "../execution/execution.hpp"
#include "../lualibs/drawlib.hpp"
#include "../../utils/request.hpp"

std::unordered_map<int, int> hyperion::bypasses::callcheck::closure_map;

std::unordered_map<int, StkId> hyperion::bypasses::callcheck::newcclosure_map;

int oldnamecall_addr;

void teleporting()
{
	hyperion::datamodel::wait_for_gameloaded();

	*reinterpret_cast<uintptr_t*>((hyperion::globals::datamodel - hyperion::offsets::inst) + hyperion::offsets::dm_net) = 3;

	hyperion::bypasses::callcheck::closure_map.clear();

	auto tempstate = hyperion::state();
	tempstate.set_identity(8);

	hyperion::globals::gstate = hyperion::functions::get_globalstate(tempstate.return_state());
	tempstate.new_thread();

	tempstate.sandboxthread();
	hyperion::functions::replace(tempstate.return_state(), -10002);

	hyperion::functions::create_table(tempstate.return_state(), 0, 0);
	tempstate.setglobal("_G");

	hyperion::globals::rstate = tempstate.return_state();

	hyperion::environment::init_functions(hyperion::globals::rstate);

	hyperion::lualibs::drawlib::init_functions(hyperion::globals::rstate);

	hyperion::taskscheduler::init_script_scheduler(hyperion::globals::rstate);
}

int tp_handler(int ls)
{
	hyperion::state rstate(ls);

	uintptr_t teleport_enum = *reinterpret_cast<uintptr_t*>(rstate.touserdata(1));
	std::string teleport_type = *reinterpret_cast<std::string*>(teleport_enum + 0x4);

	if (strcmp(teleport_type.c_str(), "InProgress") == 0)
	{
		std::cout << "Teleport in progress!" << std::endl;
		std::thread(teleporting).detach();
	}
	return 0;
}

int hyperion::environment::getconstant(uintptr_t ls) //TODO XOR NUM FOR NUMBERS
{
	state rstate(ls);

	if (!hyperion::functions::is_cfunction(ls, 1))
	{
		auto closure = rstate.topointer(1);

		const auto index = static_cast<int>(rstate.to_number(2));

		const auto proto = hyperion::obfuscation::get_proto(closure);

		if (*reinterpret_cast<std::uint8_t*>(proto + 68) < index - 1 || index <= 0) // Check if index fall within sizek bounds
		{
			rstate.push_string("Invalid index");

			return 1;
		}

		const auto constant_array = reinterpret_cast<TValue*>(hyperion::obfuscation::get_proto_member(closure, 16)); //p->k

		auto tval = &(constant_array[index - 1]);

		TValue* top = *reinterpret_cast<TValue**>(ls + 16);
		top->value = tval->value;
		top->tt = tval->tt;
		*reinterpret_cast<uintptr_t*>(ls + 16) += 16;
	}
	else
	{
		rstate.push_string("um this is a cclosure retard");
	}

	return 1;
}

int hyperion::environment::setconstant(uintptr_t ls)
{
	state rstate(ls);

	if (!hyperion::functions::is_cfunction(ls, 1))
	{
		auto closure = rstate.topointer(1);

		const auto index = static_cast<int>(rstate.to_number(2));

		const auto proto = hyperion::obfuscation::get_proto(closure);

		auto obj = reinterpret_cast<TValue*>(rstate.index2addr(-1));

		if (*reinterpret_cast<std::uint8_t*>(proto + 68) < index - 1 || index <= 0) // Check if index fall within sizek bounds
		{
			rstate.push_string("Invalid index");

			return 1;
		}

		auto constant_array = reinterpret_cast<TValue*>(hyperion::obfuscation::get_proto_member(closure, 16)); //p->k

		auto tval = &(constant_array[index - 1]);

		tval->value = obj->value;
		tval->tt = obj->tt;

		return 0;
	}
	else
	{
		rstate.push_string("um this is a cclosure retard");
	}

	return 1;
}

int hyperion::environment::getconstants(uintptr_t ls)
{
	state rstate(ls);

	hyperion::functions::create_table(ls, 0, 0);

	if (!hyperion::functions::is_cfunction(ls, 1))
	{
		auto closure = rstate.topointer(1);

		const auto proto = hyperion::obfuscation::get_proto(closure);

		const auto array_size = *reinterpret_cast<std::uint8_t*>(proto + 68);

		const auto constant_array = reinterpret_cast<TValue*>(hyperion::obfuscation::get_proto_member(closure, 16)); //p->k

		for (int i = 0; i < array_size; ++i)
		{
			auto tval = &(constant_array[i]);

			TValue* top = *reinterpret_cast<TValue**>(ls + 16);
			top->value = tval->value;
			top->tt = tval->tt;
			*reinterpret_cast<uintptr_t*>(ls + 16) += 16; //USE MY PUSH_OBJECT U LOW IQ RETARD

			hyperion::functions::rawseti(ls, -2, i + 1);
		}
	}
	else
	{
		rstate.push_string("um this is a cclosure retard");
	}

	return 1;
}

struct upvalue
{
	uint64_t pad;
	TValue* tval;
};

int hyperion::environment::getupvalues(uintptr_t ls)
{
	state rstate(ls);

	hyperion::functions::create_table(ls, 0, 0);

	if (!hyperion::functions::is_cfunction(ls, 1))
	{
		auto closure = rstate.topointer(1);

		auto array_size = *reinterpret_cast<byte*>(closure + 7);

		for (int i = 1; i <= array_size; ++i)
		{
			auto upval = *reinterpret_cast<TValue*>(closure + 16 * (i + 1));

			auto top = *reinterpret_cast<TValue**>(ls + 16);

			if (upval.tt == 11)
				upval = *reinterpret_cast<TValue*>(reinterpret_cast<TValue*>(upval.value.p)->unk);

			top->value = upval.value;
			top->tt = upval.tt;
			*reinterpret_cast<uintptr_t*>(ls + 16) += 16;

			hyperion::functions::rawseti(ls, -2, i);
		}
	}

	return 1;
}

int hyperion::environment::getupvalue(uintptr_t ls)
{
	state rstate(ls);

	if (!hyperion::functions::is_cfunction(ls, 1))
	{
		auto closure = rstate.topointer(1);

		auto array_size = *reinterpret_cast<byte*>(closure + 7);

		const auto index = static_cast<int>(rstate.to_number(2));

		if (array_size < index || index <= 0) // Check if index fall within sizek bounds
		{
			rstate.push_string("Invalid index");

			return 1;
		}

		auto upval = *reinterpret_cast<TValue*>(closure + 16 * (index + 1));

		auto top = *reinterpret_cast<TValue**>(ls + 16);

		if (upval.tt == 11)
			upval = *reinterpret_cast<TValue*>(reinterpret_cast<TValue*>(upval.value.p)->unk);

		top->value = upval.value;
		top->tt = upval.tt;
		*reinterpret_cast<uintptr_t*>(ls + 16) += 16;
	}

	return 1;
}

int hyperion::environment::setupvalue(uintptr_t ls)
{
	state rstate(ls);

	if (!hyperion::functions::is_cfunction(ls, 1))
	{
		auto closure = rstate.topointer(1);

		auto array_size = *reinterpret_cast<byte*>(closure + 7);

		const auto index = static_cast<int>(rstate.to_number(2));

		auto obj = reinterpret_cast<TValue*>(rstate.index2addr(-1));

		if (array_size < index - 1 || index <= 0) // Check if index fall within sizek bounds
		{
			rstate.push_string("Invalid index");

			return 1;
		}

		auto upval = reinterpret_cast<TValue*>(closure + 16 * (index + 1));

		auto top = *reinterpret_cast<TValue**>(ls + 16);

		if (upval->tt == 11)
			upval = reinterpret_cast<TValue*>(reinterpret_cast<TValue*>(upval->value.p)->unk);

		upval->value = obj->value;
		upval->tt = obj->tt;

	}

	return 0;
}

void hyperion::environment::init_functions(state rstate)
{
	hyperion::functions::create_table(rstate.return_state(), 0, 0);
	rstate.setglobal("debug");

	rstate.getglobal("debug");
	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(getconstant), 0);
	rstate.setfield(-2, "getconstant");

	rstate.getglobal("debug");
	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(setupvalue), 0);
	rstate.setfield(-2, "setupvalue");

	rstate.getglobal("debug");
	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(setconstant), 0);
	rstate.setfield(-2, "setconstant");

	rstate.getglobal("debug");
	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(getconstants), 0);
	rstate.setfield(-2, "getconstants");

	rstate.getglobal("debug");
	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(getupvalues), 0);
	rstate.setfield(-2, "getupvalues");

	rstate.getglobal("debug");
	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(getupvalue), 0);
	rstate.setfield(-2, "getupvalue");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(getgenv), 0);
	rstate.setglobal("getgenv");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(getrenv), 0);
	rstate.setglobal("getrenv");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(getreg), 0);
	rstate.setglobal("getreg");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(checkcaller), 0);
	rstate.setglobal("checkcaller");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(hookfunction), 0);
	rstate.setglobal("hookfunction");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(stroketon), 0);
	rstate.setglobal("stroketon");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(getminors), 0);
	rstate.setglobal("getminors");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(setreadonly), 0);
	rstate.setglobal("setreadonly");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(isreadonly), 0);
	rstate.setglobal("isreadonly");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(compilererror), 0);
	rstate.setglobal("compilererror");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(getrawmetatable), 0);
	rstate.setglobal("getrawmetatable");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(runbytecode), 0);
	rstate.setglobal("runbytecode");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(setsimradiusflag), 0); //sets flag only, require init script to set actual value
	rstate.setglobal("setsimulationradiusflag");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(loadstring), 0);
	rstate.setglobal("loadstring");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(getnamecallmethod), 0);
	rstate.setglobal("getnamecallmethod");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(newcclosure), 0);
	rstate.setglobal("newcclosure");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(getgc), 0);
	rstate.setglobal("getgc");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(test), 0);
	rstate.setglobal("test");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(httpget), 0);
	rstate.setglobal("HttpGet");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(getconnections), 0);
	rstate.setglobal("getothersignals");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(enableconnection), 0);
	rstate.setglobal("enableconnection");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(disableconnection), 0);
	rstate.setglobal("disableconnection");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(firesignal), 0);
	rstate.setglobal("firesignalhelper");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(fireonesignal), 0);
	rstate.setglobal("fireonesignal");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(fireproximityprompt), 0);
	rstate.setglobal("fireproximityprompt");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(fireclickdetector), 0);
	rstate.setglobal("fireclickdetector");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(firetouchtransmitter), 0);
	rstate.setglobal("firetouchinterest");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(setproximityprompttime), 0);
	rstate.setglobal("setproximitypromptduration");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(islclosure), 0);
	rstate.setglobal("islclosure");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(iscclosure), 0);
	rstate.setglobal("iscclosure");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(setpropvisibility), 0);
	rstate.setglobal("setpropvisibility");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(setthreadidentity), 0);
	rstate.setglobal("setthreadidentity");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(setfpscap), 0);
	rstate.setglobal("setfpscap");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(getfpscap), 0);
	rstate.setglobal("getfpscap");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(getfflag), 0);
	rstate.setglobal("getfflag");

	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(setfflag), 0);
	rstate.setglobal("setfflag");

	rstate.settop(0);

	printf("Custom Funcs Done\n");

	std::thread(hyperion::environment::init_tphandler, rstate).detach(); //will wait for gameloaded then create the connection, the above code can run before game loads fully
}

void hyperion::environment::init_tphandler(state rstate)
{
	while (hyperion::datamodel::is_gameloaded() == false)
		Sleep(100);

	//TP Handler connection
	rstate.getglobal("game");
	rstate.getfield(-1, "GetService");
	rstate.push_value(-2);
	rstate.push_string("Players");
	rstate.pcall(2, 1, 0);
	rstate.getfield(-1, "LocalPlayer");
	rstate.getfield(-1, "OnTeleport");
	rstate.getfield(-1, "Connect");
	rstate.push_value(-2);
	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(tp_handler), 0);
	rstate.pcall(2, 1, 0);

	printf("TP Handler Done\n");
}

int hooked_index(uintptr_t ls)
{
	hyperion::state rstate(ls);
	if (strcmp(rstate.tostring(2), "HttpGet") == 0)
	{
		rstate.getglobal("HttpGet");
		return 1;
	}

	rstate.push_value(-10003);
	rstate.push_value(-3);
	rstate.push_value(-3);
	rstate.call(2, 1);

	return 1;
}
//ffaded was here
int hooked_namecall(uintptr_t ls)
{
	const char* namecall = reinterpret_cast<const char*>(*reinterpret_cast<uintptr_t*>(ls + hyperion::offsets::namecall) + 20);

	if (std::strcmp(namecall, "HttpGet") == 0)
	{
		hyperion::state rstate(ls);

		auto args = hyperion::functions::gettop(ls);

		bool async = false;

		if (args > 2)
			async = rstate.to_bool(3);

		if (args < 2) //self is always an arg
			rstate.error("HttpGet takes 1 arg");

		if (async)
		{
			*reinterpret_cast<const char**>(hyperion::offsets::rebase(0x265C240)) = rstate.tostring(2); // "about:blank"
			*reinterpret_cast<const char**>(hyperion::offsets::rebase(0x265C244)) = rstate.tostring(2);

			rstate.getglobal("game");
			rstate.getfield(-1, "HttpGetAsync");
			rstate.push_value(1);
			rstate.push_value(2);
			if (rstate.pcall(2, 1, 0))
			{
				if (std::strcmp("attempt to yield across metamethod/C-call boundary", rstate.tostring(-1)) == 0)
				{
					return hyperion::functions::yield(rstate.return_state(), 0);
				}
			}
		}
		else
		{
			return hyperion::environment::httpget(ls);
		}
	}
	if (std::strcmp(namecall, "GetObjects") == 0)
	{
		return hyperion::environment::getobjects(ls);
	}

	return reinterpret_cast<int(__cdecl*)(int a1)>(oldnamecall_addr)(ls);
}

void hyperion::environment::init_index_hook(state rstate)
{
	rstate.getglobal("game");
	rstate.getmetatable(-1);
	rstate.setreadonly(-1, false);

	rstate.getfield(-1, "__index");
	hyperion::functions::pushcclosure(rstate.return_state(), hyperion::offsets::blankfunction2_offset, 0, 1, 0);
	rstate.setfield(-2, "__index");

	rstate.getglobal("game");
	rstate.getmetatable(-1);
	rstate.setreadonly(-1, true);
	rstate.settop(0);
}

void hyperion::environment::init_namecall_hook(state rstate)
{
	rstate.getglobal("game");
	rstate.getmetatable(-1);
	rstate.setreadonly(-1, false);

	rstate.getfield(-1, "__namecall");
	auto addr = rstate.topointer(-1);
	oldnamecall_addr = hyperion::obfuscation::get_proto(addr);
	hyperion::functions::pushcclosure(rstate.return_state(), hyperion::offsets::blankfunction3_offset, 0, 1, 0);
	rstate.setfield(-2, "__namecall");

	rstate.getglobal("game");
	rstate.getmetatable(-1);
	rstate.setreadonly(-1, true);
	rstate.settop(0);
}

int hyperion::environment::setfpscap(uintptr_t ls)
{
	state rstate(ls);

	const auto fps = rstate.to_number(1);
	hyperion::taskscheduler::unlock_fps(fps);

	return 0;
}

int hyperion::environment::getfpscap(uintptr_t ls)
{
	state rstate(ls);

	const auto fps = hyperion::taskscheduler::get_fps_cap();
	rstate.push_number(fps);

	return 1;
}

int hyperion::environment::getfflag(uintptr_t ls)
{
	state rstate(ls);

	const auto flag = rstate.tostring(1);

	std::string ret;

	auto out = hyperion::functions::getfflag(flag, ret, 1);

	if (!out)
		rstate.error("Flag not found!");

	rstate.push_string(ret);

	return 1;
}

int hyperion::environment::setfflag(uintptr_t ls)
{
	state rstate(ls);

	const auto flag = rstate.tostring(1);
	const auto value = rstate.tostring(2);

	std::string ret;

	auto out = hyperion::functions::getfflag(flag, ret, 1);

	if (!out)
		rstate.error("Flag not found!");

	hyperion::functions::setfflag(flag, value, 63, 0);

	return 0;
}

int hyperion::environment::httpget(uintptr_t ls)
{
	state rstate(ls);

	if (hyperion::functions::gettop(ls) < 2)
		rstate.error("HttpGet takes 1 arg");

	const char* link = rstate.tostring(2);
	rstate.push_string(get(link));

	return 1;
}

int hyperion::environment::islclosure(uintptr_t ls)
{
	state rstate(ls);

	const auto isc1 = hyperion::functions::is_cfunction(ls, 1);

	rstate.push_bool(!isc1);
	return 1;
}

int hyperion::environment::iscclosure(uintptr_t ls)
{
	state rstate(ls);

	const auto isc1 = hyperion::functions::is_cfunction(ls, 1);

	rstate.push_bool(isc1);
	return 1;
}

int hyperion::environment::getobjects(uintptr_t ls)
{
	state rstate(ls);

	rstate.push_value(1); // game
	rstate.getfield(-1, "GetService"); // func
	rstate.push_value(1); // game
	rstate.push_string("InsertService");
	rstate.pcall(2, 1, 0); // stack after call: arg1, arg2, game, ret value
	rstate.getfield(-1, "LoadLocalAsset");
	rstate.push_value(-2); // ret value (InsertService)
	rstate.push_value(2); // arg2 should be string
	rstate.pcall(2, 1, 0); // stack after call: arg1, arg2, game, insertservice, asset
	hyperion::functions::create_table(rstate.return_state(), 0, 0);
	rstate.push_value(-2);
	hyperion::functions::rawseti(ls, -2, 1);

	return 1;
}

int hyperion::environment::getconnections(uintptr_t ls)
{
	state rstate(ls);

	auto signal = *reinterpret_cast<uintptr_t*>(rstate.touserdata(1));

	signal = *reinterpret_cast<uintptr_t*>(signal + 0x10);

	hyperion::functions::create_table(rstate.return_state(), 0, 0);

	int index = 1;
	while (signal)
	{
		hyperion::functions::create_table(rstate.return_state(), 0, 0);

		rstate.push_string("__OBJECT");
		functions::push_light_userdata(rstate.return_state(), reinterpret_cast<void*>(signal));
		functions::settable(rstate.return_state(), -3);

		rstate.push_string("__ENABLED");
		if (*reinterpret_cast<uintptr_t*>(signal + 0x14) > 0)
			rstate.push_bool(true);
		else
			rstate.push_bool(false);
		functions::settable(rstate.return_state(), -3);

		functions::rawseti(rstate.return_state(), -2, index++);

		signal = *reinterpret_cast<uintptr_t*>(signal + 0x10);
	}

	return 1;
}

int hyperion::environment::disableconnection(uintptr_t ls)
{
	state rstate(ls);
	auto signal = reinterpret_cast<int>(rstate.touserdata(1));
	*reinterpret_cast<uintptr_t*>(signal + 0x14) = 0;

	return 0;
}

int hyperion::environment::enableconnection(uintptr_t ls)
{
	state rstate(ls);
	auto signal = reinterpret_cast<int>(rstate.touserdata(1));
	*reinterpret_cast<uintptr_t*>(signal + 0x14) = 1;

	return 0;
}

bool validate_connection(uintptr_t connection)
{
	if (*reinterpret_cast<uintptr_t*>(connection + 0x4) == 1 || *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(connection + 0x18) + 0x40) == 0)
		return 0;
	else
		return 1;
}

int hyperion::environment::firesignal(uintptr_t ls)
{
	state rstate(ls);
	uintptr_t signal = *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(rstate.touserdata(1)) + 0x10);
	rstate.remove(1);

	int args = functions::gettop(ls);

	while (signal)
	{
		uintptr_t ref = *reinterpret_cast<uintptr_t*>(signal + 0x1C);

		if (!validate_connection(signal))
		{
			signal = *reinterpret_cast<uintptr_t*>(signal + 0x10);
			continue;
		}

		signal = *reinterpret_cast<uintptr_t*>(signal + 0x10);
		uintptr_t state_ref = *reinterpret_cast<uintptr_t*>(ref + 0x44);
		uintptr_t state_obf = *reinterpret_cast<uintptr_t*>(state_ref + 0x8);
		uintptr_t state = hyperion::functions::get_globalstate(state_obf);
		if (state != hyperion::globals::gstate)
			continue;

		int idx = *reinterpret_cast<int*>(ref + 0x4C);
		uintptr_t new_state = functions::newthread(ls);

		functions::rawgeti(new_state, LUA_REGISTRYINDEX, idx);
		functions::xmove(new_state, ls, 1);

		for (int i = 1; i <= args; i++)
			rstate.push_value(i);

		rstate.pcall(args, 0, 0);
	}

	return 0;
}

int hyperion::environment::fireproximityprompt(uintptr_t ls)
{
	state rstate(ls);

	uintptr_t instance = *reinterpret_cast<uintptr_t*>(rstate.touserdata(1));
	hyperion::functions::fireproximityprompt(instance);
	return 0;
}

int hyperion::environment::setproximityprompttime(uintptr_t ls)
{
	state rstate(ls);

	uintptr_t Instance = *reinterpret_cast<uintptr_t*>(rstate.touserdata(1));
	float time = rstate.to_number(2);
	const float old_val = *reinterpret_cast<float*>(Instance + 0x58); //not used rn
	*reinterpret_cast<float*>(Instance + 0x58) = time;
	return 0;
}

int hyperion::environment::fireclickdetector(uintptr_t ls)
{
	state rstate(ls);

	uintptr_t click_detector = *reinterpret_cast<uintptr_t*>(rstate.touserdata(1));
	float distance = rstate.to_number(2);
	rstate.settop(0);

	rstate.getglobal("game");
	rstate.getfield(-1, "GetService");
	rstate.push_value(-2);
	rstate.push_string("Players");
	rstate.pcall(2, 1, 0);
	rstate.getfield(-1, "LocalPlayer");

	uintptr_t localplayer = *reinterpret_cast<uintptr_t*>(rstate.touserdata(-1));

	hyperion::functions::fireclickdetector(click_detector, distance, localplayer);
	return 0;
}

int hyperion::environment::firetouchtransmitter(uintptr_t ls)
{
	state rstate(ls);

	uintptr_t transmitter = *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(rstate.touserdata(1)) + 0xA8);
	uintptr_t to_touch = *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(rstate.touserdata(2)) + 0xA8);
	uintptr_t world = *reinterpret_cast<uintptr_t*>(transmitter + 0x1A0);
	int touch_type = rstate.to_number(3);

	hyperion::functions::firetouchinterest(world, transmitter, to_touch, touch_type);
	return 0;
}

/* INIT SCRIPT FOR :Fire
local MT = {
  __index = function(a, b)
	if b == "Fire" then
	  return function(self, ...) fireonesignal(self.__OBJECT, ...) end
	end
	return nil
  end
}

function attachMT(tbl)
  setmetatable(tbl, MT)
  return tbl
end

getgenv().firesignal = function(a, ...)
  temp=a:Connect(function()end)
  temp:Disconnect()
  return firesignalhelper(temp, ...)
end

getgenv().getconnections = function(a)
  temp = a:Connect(function() end)
  signals = getothersignals(temp)
  for i,v in pairs(signals) do
	signals[i] = attachMT(v)
  end
  temp:Disconnect()
  return signals
end
*/

/* TEST SCRIPT
local a = game.Players.LocalPlayer.Character.Humanoid.Changed

a:Connect(function(...) print("3", ...) end)

a:Connect(function(...) print("1", ...) end)

a:Connect(function(...) print("2", ...) end)

local connections = getconnections(a)

connections[1]:Fire('hi', 1, 4, 5)

connections[2]:Fire('hi', 2)

connections[3]:Fire('hi', 3)


*/

int hyperion::environment::fireonesignal(uintptr_t ls)
{
	state rstate(ls);
	uintptr_t signal = reinterpret_cast<uintptr_t>(rstate.touserdata(1));

	int args = functions::gettop(ls);

	uintptr_t ref = *reinterpret_cast<uintptr_t*>(signal + 0x1C);

	if (!validate_connection(signal))
	{
		return 0;
	}

	uintptr_t state_ref = *reinterpret_cast<uintptr_t*>(ref + 0x44);
	uintptr_t state_obf = *reinterpret_cast<uintptr_t*>(state_ref + 0x8);
	uintptr_t state = hyperion::functions::get_globalstate(state_obf);
	if (state != hyperion::globals::gstate)
		return 0;

	int idx = *reinterpret_cast<int*>(ref + 0x4C);
	uintptr_t new_state = functions::newthread(ls);

	functions::rawgeti(new_state, LUA_REGISTRYINDEX, idx);
	functions::xmove(new_state, ls, 1);

	for (int i = 2; i <= args; i++)
		rstate.push_value(i);

	rstate.pcall(args - 1, 0, 0);

	return 0;
}

int hyperion::environment::gethui(uintptr_t ls)
{
	state rstate(ls);
	rstate.getglobal("game");
	rstate.getfield(-1, "Players");
	rstate.getfield(-1, "LocalPlayer");
	rstate.getfield(-1, "PlayerGui");
	functions::push_light_userdata(rstate.return_state(), rstate.touserdata(-1));
	return 1;
}

int hyperion::environment::getgenv(uintptr_t ls)
{
	state rstate(ls);
	rstate.push_value(-10002);
	return 1;
}

int hyperion::environment::getrenv(uintptr_t ls)
{
	state rstate(ls);
	state gstate(hyperion::globals::gstate);
	gstate.push_value(-10002);
	gstate.xmove(hyperion::globals::gstate, ls, 1);

	return 1;
}

int hyperion::environment::getreg(uintptr_t ls)
{
	state rstate(ls);
	rstate.push_value(-10000);
	return 1;
}

int hyperion::environment::getgc(uintptr_t ls)
{
	state rstate(ls);
	auto tables = rstate.to_bool(1);
	auto global_state = hyperion::obfuscation::get_global_state(rstate.return_state());
	auto gc_object = *reinterpret_cast<uintptr_t**>(global_state + hyperion::offsets::root_gc);
	hyperion::functions::create_table(ls, 0, 0);
	for (int i = 1; gc_object != nullptr; ++i)
	{
		auto type = *reinterpret_cast<BYTE*>(reinterpret_cast<uintptr_t>(gc_object) + hyperion::offsets::tt);
		if (type == hyperion::types::r_lua_tuserdata || type == hyperion::types::r_lua_tfunction || (type == hyperion::types::r_lua_ttable && tables))
		{
			rstate.push_object(reinterpret_cast<uintptr_t>(gc_object), type);
			hyperion::functions::rawseti(ls, -2, i);
		}
		gc_object = *reinterpret_cast<uintptr_t**>(gc_object);
	}
	return 1;
}

int hyperion::environment::newcclosurehandler(uintptr_t ls)
{
	state rstate(ls);
	auto args = hyperion::functions::gettop(ls);
	rstate.push_value(-10003);

	for (auto i = 1; i <= args; ++i)
	{
		rstate.push_value(i);
	}

	auto error = rstate.pcall(args, -1, 0);

	if (error)
	{
		hyperion::globals::lasterror = rstate.tostring(1);
		rstate.getglobal("compilererror");
		rstate.spawn();
		return 0;
	}
	auto newa = hyperion::functions::gettop(ls);

	return newa - args;
}

int hyperion::environment::newcclosure(uintptr_t ls)
{
	state rstate(ls);

	rstate.push_value(-1);
	hyperion::bypasses::callcheck::newcclosure_map.insert({ rstate.topointer(1), reinterpret_cast<TValue*>(rstate.index2addr(-1)) });
	hyperion::bypasses::callcheck::push_function(rstate.return_state(), reinterpret_cast<std::uintptr_t>(newcclosurehandler), 1);
	return 1;
}


int hyperion::environment::setpropvisibility(uintptr_t ls)
{
	state rstate(ls);

	auto instance = *reinterpret_cast<uintptr_t*>(rstate.touserdata(1));

	auto property_name = hyperion::functions::tostr(ls, 2);

	auto index = *reinterpret_cast<uint16_t*>(property_name + 6);

	bool visibility = rstate.to_bool(3);

	auto propdescriptor = hyperion::offsets::prop_descriptor_vector[index];

	auto class_descriptor = *reinterpret_cast<uintptr_t*>(instance + offsets::class_descriptor);

	auto property = *reinterpret_cast<uintptr_t*>(hyperion::functions::getproperty(class_descriptor, propdescriptor));

	if (visibility)
		*reinterpret_cast<int*>(property + 0x24) = 35; //readwrite
	else
		*reinterpret_cast<int*>(property + 0x24) = 5; //readonly 

	return 0;
}

int hyperion::environment::setreadonly(uintptr_t ls)
{
	state rstate(ls);
	rstate.setreadonly(1, rstate.to_bool(2));
	return 0;
}

int hyperion::environment::isreadonly(uintptr_t ls)
{
	state rstate(ls);
	int table = *reinterpret_cast<int*>(rstate.index2addr(-1));
	rstate.push_bool(*reinterpret_cast<BYTE*>(table + hyperion::offsets::readonly)); //readonly offset, require updates
	return 1;
}

int hyperion::environment::compilererror(uintptr_t ls) //runs when compiler errors
{
	state rbstate(ls);
	if (hyperion::globals::lasterror != "")
	{
		rbstate.getglobal("warn");
		rbstate.push_string(hyperion::globals::lasterror);
		rbstate.pcall(1, 0, 0);
		hyperion::globals::lasterror = "";
	}
	return 0;
}

int hyperion::environment::getrawmetatable(uintptr_t ls)
{
	state rbstate(ls);
	if (rbstate.getmetatable(-1) == 0) {
		rbstate.push_nil();
		return 1;
	}
	return 1;
}

int hyperion::environment::runbytecode(uintptr_t ls)
{
	state rbstate(ls);

	auto script = static_cast<std::string>(rbstate.tostring(1));

	std::string bytecode;

	for (auto i = 0; i < script.length(); i += 2)
	{
		std::string uhhhh = script.substr(i, 2);
		bytecode.push_back(static_cast<char>(static_cast<int>(strtol(uhhhh.c_str(), 0, 16))));
	}

	rbstate.settop(0);
	rbstate.luau_load("=hyperion", bytecode);
	rbstate.spawn();
}

int hyperion::environment::setsimradiusflag(uintptr_t ls)
{
	state rbstate(ls);
	uintptr_t localplayer = *reinterpret_cast<uintptr_t*>(rbstate.touserdata(1));
	bool flag = rbstate.to_bool(2);

	uintptr_t class_descriptor = *reinterpret_cast<uintptr_t*>(localplayer + 0xC);
	uintptr_t prop_vector = *reinterpret_cast<uintptr_t*>(class_descriptor + 0x24);
	uintptr_t simulation_radius = *reinterpret_cast<uintptr_t*>(prop_vector + 0x20C);

	if (flag)
		*reinterpret_cast<int*>(simulation_radius + 0x24) = 35; //readwrite
	else
		*reinterpret_cast<int*>(simulation_radius + 0x24) = 5; //readonly 

	return 0;
}

int hyperion::environment::loadstring(uintptr_t ls)
{
	state rbstate(ls);
	auto script = static_cast<std::string>(rbstate.tostring(1));
	rbstate.settop(0);

	hyperion::execution::execute_script(rbstate.return_state(), script, false, false); //TODO: make seperate func for loadstring, also on compilation error its supposed to return the error itself, also need chunkname optimal arg
	return 1;
}

int hyperion::environment::setthreadidentity(uintptr_t ls)
{
	state rstate(ls);

	auto identity = rstate.to_number(1);
	rstate.set_identity(identity);

	uintptr_t stc[9];
	hyperion::functions::impersonator(stc, identity);

	return 0;
}

int hyperion::environment::test(uintptr_t ls)
{
	state rstate(ls);
	uintptr_t scr = *reinterpret_cast<uintptr_t*>(rstate.touserdata(1));
	rstate.settop(0);

	std::cout << scr << std::endl;

	return 1;
}

int hyperion::environment::stroketon(uintptr_t ls)
{
	state rstate(ls);
	rstate.push_string("skid");
	return 1;
}

int hyperion::environment::getminors(uintptr_t ls)
{
	/*std::vector<std::string> Final;
	state rstate(ls);
	rstate.getglobal("game");
	rstate.getfield(-1,"Players");
	rstate.getfield(-1,"GetChildren");
	rstate.push_value(-2);
	rstate.pcall(1, 1, 0);*/
	//shit nvm yall don't have pushnil
	return 0;
}

int hyperion::environment::checkcaller(uintptr_t ls)
{
	state rstate(ls);
	int flag = *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(ls + hyperion::offsets::extraspace) + 0x20); //flag, doesnt require updates
	rstate.push_bool(flag);
	return 1;
}

int hyperion::environment::hookfunction(uintptr_t ls)
{
	state rstate(ls);
	auto toreplace = rstate.topointer(1);
	auto toreplacewith = rstate.topointer(2);

	bool isc1 = hyperion::functions::is_cfunction(ls, 1);
	bool isc2 = hyperion::functions::is_cfunction(ls, 2);

	if (isc1 && isc2)
	{
		auto key1 = hyperion::bypasses::callcheck::closure_map[hyperion::obfuscation::get_closure_name(toreplace)];
		auto key2 = hyperion::bypasses::callcheck::closure_map[hyperion::obfuscation::get_closure_name(toreplacewith)];

		if (key1 && key2)
		{
			hyperion::bypasses::callcheck::closure_map[hyperion::obfuscation::get_closure_name(toreplace)] = key2;
		}
		else if (key1 && !key2)
		{
			auto cl = hyperion::obfuscation::get_proto(toreplacewith);
			hyperion::bypasses::callcheck::closure_map[hyperion::obfuscation::get_closure_name(toreplace)] = cl;
		}
		else if (!key1 && key2)
		{
			obfuscation::set_closure_name(toreplace, hyperion::obfuscation::get_closure_name(toreplacewith));
			auto a = hyperion::obfuscation::get_proto(toreplacewith);

			auto f = (toreplace + offsets::closure_deobf);
			*reinterpret_cast<int*>(f) = f ^ a;
		}
		else if (!key1 && !key2)
		{
			auto a = hyperion::obfuscation::get_proto(toreplacewith);

			auto f = (toreplace + offsets::closure_deobf);
			*reinterpret_cast<int*>(f) = f ^ a;
		}
	}
	else if (isc1 && !isc2)
	{
		auto key1 = hyperion::bypasses::callcheck::closure_map[hyperion::obfuscation::get_closure_name(toreplace)];

		if (key1)
		{




			//hyperion::bypasses::callcheck::closure_map[hyperion::obfuscation::get_closure_name(toreplace)] = a;


		}
	}

	return 0;
}

int hyperion::environment::getnamecallmethod(uintptr_t ls)
{
	state rstate(ls);
	uintptr_t namecall = *reinterpret_cast<uintptr_t*>(ls + hyperion::offsets::namecall); //__namecall, require updates
	if (!namecall)
	{
		rstate.push_nil();
		return 1;
	}

	rstate.push_object(namecall, hyperion::types::r_lua_tstring);
	return 1;
}

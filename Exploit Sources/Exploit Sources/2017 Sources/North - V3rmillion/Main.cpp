#include "SDK.h"
#include "VMProtectSDK.h"

void gay() {
	Console* console = new Console("North - v1.1.2");
	*console << "hi";
	/*
	welcomeshit(console);
	
	*console << WHITE << "Getting Script Context...";
	rLua::sc = Addys::SC::ptr();
	*console << GREEN;
	printf("%08x\n", rLua::sc);

	*console << WHITE << "Getting Lua State...";
	rLua::state = rLua::getGlobalState(rLua::sc, 2);
	*console << GREEN;
	printf("%08x\n", rLua::state);
	*console << WHITE << "Started! Say 'help' for a list of commands.\n";

	bool running = true;
	cmds["kill"] = &kill;
	cmds["ws"] = &ws;
	cmds["airwalk"] = &airwalk;
	cmds["noairwalk"] = &noairwalk;
	cmds["btools"] = &btools;
	cmds["notools"] = &notools;
	cmds["watch"] = &watch;
	cmds["unwatch"] = &unwatch;
	cmds["punish"] = &punish;
	cmds["unpunish"] = &unpunish;
	cmds["jp"] = &jp;
	cmds["moonwalk"] = &moonwalk;
	cmds["nomoon"] = &nomoon;
	cmds["god"] = &god;
	cmds["ungod"] = &ungod;
	cmds["name"] = &name;
	cmds["char"] = &charr;
	cmds["rchar"] = &rchar;
	cmds["noob"] = &noob;
	cmds["ff"] = &ff;
	cmds["unff"] = &unff;
	cmds["smoke"] = &smoke;
	cmds["unsmoke"] = &unsmoke;
	cmds["sparkles"] = &sparkles;
	cmds["unsparkles"] = &unsparkles;
	cmds["fire"] = &fire;
	cmds["unfire"] = &unfire;
	cmds["fogend"] = &fogend;
	cmds["freeze"] = &freeze;
	cmds["thaw"] = &unfreeze;
	cmds["admin"] = &fogend;
	cmds["noclip"] = &noclip;
	cmds["statchange"] = &statchange;
	cmds["clip"] = &clip;
	cmds["tp"] = &tp;
	cmds["help"] = &help;
	cmds["getglobal"] = &getglobal;
	cmds["pcall"] = &pcall;
	cmds["getfield"] = &getfield;
	cmds["pushnumber"] = &pushnumber;
	cmds["setfield"] = &setfield;
	cmds["pushvalue"] = &pushvalue;
	cmds["pushstring"] = &pushstring;
	cmds["pushnil"] = &pushnil;
	cmds["pushbool"] = &pushbool;
	cmds["tostring"] = &tostring;
	cmds["giraffe"] = &giraffe;
	cmds["head"] = &head;

	Chathook::hook_chat("LocalPlayer");

	while (running) {
		std::cout << "> ";
		std::vector<std::string> In = split(Input());
		if (cmds.size() > 0 && cmds.find(In.at(0)) != cmds.end()) {
			if (In.at(0) == "admin") {
				std::vector<const char*> plrs = rLua::Utils::parsePlayers(In.at(1));
				for (int i = 0; i < plrs.size(); i++) {
					Chathook::hook_chat(plrs.at(0));
				}
			} else {
				cmds[In.at(0)](In);
			}
		}
		else {
			*console << RED << "Command not found! Type 'help' to see a list of commands.\n" << WHITE;
		}
	}
	*/
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)gay, 0, 0, 0);
	}
	return 1;
}
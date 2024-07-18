#pragma once

#include "SDK.h"

#define CALLCHECK_DEC *(int*)((char*)Offset(0x158F224) + *(DWORD*)Offset(0x158F224)) = 0;

namespace Chathook {
	void hook_chat(const char* plr);

	void chat_handler(int state) {
		VMProtectBeginMutation("youmamamasdarfasdfasrstosptsptsptg");
		CALLCHECK_DEC;
		std::string str = std::string(rLua::tolstring(state, -2, NULL));
		std::string c = str.substr(0, 1);
		if (c == "!") {
			std::vector<std::string> cmd = split(str.substr(1));
			if(cmds.size() > 0 && cmds.find(cmd.at(0)) != cmds.end()) {
				if (cmd.at(0) == "admin") {
					std::vector<const char*> plrs = rLua::Utils::parsePlayers(cmd.at(1));
					for (int i = 0; i < plrs.size(); i++) {
						hook_chat(plrs.at(0));
					}
				} else {
					cmds[cmd[0]](cmd);	
				}
			} else {
				// DISGUSTING
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
				printf("\n[Chat Hook]: Command not found! Type 'help' to see a list of commands.\n");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				printf("> ");
			}
		}
		VMProtectEnd();
	}


	void hook_chat(const char* plr) {
		VMProtectBeginMutation("youmamamasdarfasdfasrg");
		std::vector<const char*> inst = { "Players", plr, "Chatted", "connect" };
		rLua::Utils::getInstance(inst);
		rLua::pushvalue(rLua::state, -2);
		rLua::pushcclosure(rLua::state, &chat_handler, 0);
		rLua::pcall(rLua::state, 2, 0, 0);
		VMProtectEnd();
	}
}
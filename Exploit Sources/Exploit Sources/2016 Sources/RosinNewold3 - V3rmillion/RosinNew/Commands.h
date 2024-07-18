#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <functional>

namespace Rosin {
	//typedef void(*CommandFunction)(std::vector<std::string>);
	typedef std::function<void(std::vector<std::string>)> CommandFunction;
	struct CommandItem {
		std::string Name;
		std::string Desc;
		CommandFunction Func;
		std::vector<std::string> Calls;
	};
	class Command {
	public:
		std::map<std::string, CommandItem> Commands;
		char Suffix = ' ';
		char Prefix = '\0';

		void NewCommand(std::string Name, std::vector<std::string> Calls, std::string Desc, CommandFunction Call);
		bool RemoveCommand(std::string Name);
		bool ParseCommand(std::string Input);
		CommandItem* GetCommandByCall(std::string Call);
	};
}
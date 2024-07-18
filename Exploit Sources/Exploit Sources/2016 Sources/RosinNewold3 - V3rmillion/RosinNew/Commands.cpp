#include "Commands.h"
#include <sstream>
#include <thread>

std::vector<std::string> split(std::string s, char splitchar) {
	std::string Val;
	std::vector<std::string> Ret;
	std::istringstream iis;
	iis.clear();
	iis.str(s);
	while (std::getline(iis, Val, splitchar)) {
		Ret.push_back(Val);
	}
	return Ret;
}

void CallCommand(Rosin::CommandFunction Call, std::vector<std::string> Args) {
	(Call)(Args);
}

namespace Rosin {
	void Command::NewCommand(std::string Name, std::vector<std::string> Calls, std::string Desc, CommandFunction Call) {
		if (this->Commands.find(Name) == this->Commands.end()) {
			CommandItem Item{ Name, Desc, Call, Calls };
			this->Commands.insert(std::pair<std::string, CommandItem>(Name, Item));
			printf("Added command %s\n", Name.c_str());
		}
	}
	bool Command::RemoveCommand(std::string Name) {
		if (this->Commands.find(Name) != this->Commands.end()) {
			this->Commands.erase(Name);
			return true;
		}
		return false;
	}
	CommandItem* Command::GetCommandByCall(std::string Call) {
		for (auto i : this->Commands) {
			for (size_t c = 0; c < i.second.Calls.size(); c++) {
				std::string cur = i.second.Calls[c];
				if (cur == Call)
					return &this->Commands[i.first];
			}
		}
		return nullptr;
	}
	bool Command::ParseCommand(std::string Input) {
		std::string Buffer;
		if (this->Prefix != '\0') {
			Buffer += this->Prefix;
			if (Input.substr(0, 1).compare(Buffer.c_str()) != 0) {
				return false;
			}
			Input = Input.substr(1);
		}
		std::vector<std::string> vec = split(Input, this->Suffix);
		CommandItem* Command = this->GetCommandByCall(vec[0]);
		if (Command != nullptr) {
			vec.erase(vec.begin());
			try {
				std::thread t(&CallCommand, Command->Func, vec);
				t.detach();
				return true;
			}
			catch (...) {
				return true;
			}
		}
		return false;
	}
}
/* Includes */

#include "Data.h"
#include "Lua.h"

struct luaC {
	int errors;
	string error;
};

class LuaC {

private:
	bool luaC_isnumber(const std::string& s);
	bool luaC_compare(string a, string b);
	bool luaC_compare(int a, int b);
	luaC luaC_err(string error);
	vector<string> split(const string& str, const string& delim);
public:
	luaC do_string(string input);
};

vector<string> LuaC::split(const string& str, const string& delim)
{
	vector<string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == string::npos) pos = str.length();
		string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

luaC LuaC::luaC_err(string error) {
	return luaC{ 1, error };
}

bool LuaC::luaC_compare(int a, int b) {
	return a == b;
}

bool LuaC::luaC_compare(string a, string b) {
	return a == b;
}

bool LuaC::luaC_isnumber(const std::string& s) {
	try {
		double n = stod(s);
		return true;
	}
	catch (invalid_argument &ex) {
		return false;
	}
}

int luaCState = 0;

luaC LuaC::do_string(string input) {
	istringstream isis(input);
	string line;
	while (getline(isis, line))
	{

		std::vector<string> arg = split(line, " ");

		if (arg.at(0) == "getglobal") {
			if (!(arg.size() == 2)) {
				return luaC_err("invalid arguments for `getglobal`");
			}

			GetGlobal(L, arg.at(1).c_str());
		}

		else if (arg.at(0) == "getfield") {
			if (!(arg.size() == 3)) {
				return luaC_err("invalid arguments for `getfield`");
			}

			if (!(luaC_isnumber(arg.at(1)))) {
				return luaC_err("invalid integer for argument 2 at `getfield`");
			}

			GetField(L, stod(arg.at(1).c_str()), arg.at(2).c_str());
		}

		else if (arg.at(0) == "pushstring") {
			if (!(arg.size() == 2)) {
				return luaC_err("invalid arguments for `pushstring`");
			}

			PushString(L, arg.at(1).c_str());
		}

		else if (arg.at(0) == "pcall") {
			if (!(arg.size() == 4)) {
				return luaC_err("invalid arguments for `pcall`");
			}

			for (int i = 1; i <4; i++) {
				if (!(luaC_isnumber(arg.at(i)))) {
					return luaC_err("invalid integer for argument " + to_string(i) + " at `pcall`");
				}
			}

			Pcall(L, stod(arg.at(1)), stod(arg.at(2)), stod(arg.at(3)));
		}
		else if (arg.at(0) == "tonumber") {
			if (!(arg.size() == 2)) {
				return luaC_err("invalid arguments for `tonumber`");
			}

			if (!(luaC_isnumber(arg.at(1)))) {
				return luaC_err("invalid integer for argument 2 at `tonumber`");
			}

			ToNumber(L, stod(arg.at(1)));
		}

		else if (arg.at(0) == "pushboolean") {
			if (!(arg.size() == 2)) {
				return luaC_err("invalid arguments for `pushboolean`");
			}


			if (arg.at(1) != "true" || arg.at(1) != "false" || arg.at(1) != "1" || arg.at(1) != "0") {
				return luaC_err("invalid boolean for argument 2 at `pushboolean`");
			}


			PushBoolean(L, stod(arg.at(1)));
		}

		else if (arg.at(0) == "pushnil") {
			if (!(arg.size() == 1)) {
				return luaC_err("invalid arguments for `pushnil`");
			}

			PushNil(L);
		}

		else if (arg.at(0) == "settop") {
			if (!(arg.size() == 2)) {
				return luaC_err("invalid arguments for `settop`");
			}

			if (!(luaC_isnumber(arg.at(1)))) {
				return luaC_err("invalid integer for argument 2 at `settop`");
			}

			SetTop(L, stod(arg.at(1)));
		}

		else if (arg.at(0) == "pushvalue") {
			if (!(arg.size() == 2)) {
				return luaC_err("invalid arguments for `pushvalue`");
			}

			if (!(luaC_isnumber(arg.at(1)))) {
				return luaC_err("invalid integer for argument 2 at `pushvalue`");
			}

			PushValue(L, stod(arg.at(1)));
		}

		else if (arg.at(0) == "setfield") {
			if (!(arg.size() == 3)) {
				return luaC_err("invalid arguments for `setfield`");
			}

			if (!(luaC_isnumber(arg.at(1)))) {
				return luaC_err("invalid integer for argument 2 at `setfield`");
			}

			SetField(L, stod(arg.at(1)), arg.at(2).c_str());
		}

		else if (arg.at(0) == "pushnumber") {
			if (!(arg.size() == 2)) {
				return luaC_err("invalid arguments for `pushnumber`");
			}

			if (!(luaC_isnumber(arg.at(1)))) {
				return luaC_err("invalid integer for argument 2 at `pushnumber`");
			}

			PushNumber(L, stod(arg.at(1)));
		}

		else if (arg.at(0) == "tostring") {
			if (!(arg.size() == 2)) {
				return luaC_err("invalid arguments for `tostring`");
			}

			if (!(luaC_isnumber(arg.at(1)))) {
				return luaC_err("invalid integer for argument 2 at `tostring`");
			}

			ToString(L, stod(arg.at(1)));
		}
	}
}
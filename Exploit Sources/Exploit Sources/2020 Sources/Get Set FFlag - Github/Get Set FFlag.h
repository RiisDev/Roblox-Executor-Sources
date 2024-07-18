//To find them in IDA u can search for "_DataCenterFilter" in strings. Its used in Setfflag and the actual name is FLog::SetValue
//GetValue is close in memory to SetValue so you should be able to find it
//All addresses in this release are rebased to 0x400000 to get the address in memory simply do "Address - 0x400000 + RobloxPlayerBeta.exe"

typedef bool(__cdecl* FLog)(const std::string& name, std::string& value, bool alsoCheckUnknown);
FLog GetValue = reinterpret_cast<FLog>(Rebase(0x15A5860));

typedef bool(__cdecl* SLog)(const std::string& name, const std::string& value, int Type, bool loadedFromServer);
SLog SetValue = reinterpret_cast<SLog>(Rebase(0x15A6030));

int GetFFlag(int ls)
{
	std::string flag = Roblox::r_lua_tostring(ls, 1);
	std::string ret;

	bool out = GetValue(flag, ret, 1);
	if (!out)
		Roblox::r_luaL_error("Flag not found!");

	Roblox::r_lua_pushstring(ls, ret.c_str());
	return 1;
}

int SetFFlag(int ls)
{
	std::string flag = Roblox::r_lua_tostring(ls, 1);
	std::string value = Roblox::r_lua_tostring(ls, 2);
	std::string ret;

	bool exist = GetValue(flag, ret, 1);

	if (!exist)
		Roblox::r_luaL_error("Flag not found!");

	SetValue(flag, value, 63, 0);
	return 0;
}

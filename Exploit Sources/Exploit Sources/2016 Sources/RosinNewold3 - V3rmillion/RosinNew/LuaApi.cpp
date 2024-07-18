#include "lua.hpp"
#include <iostream>
#include "MainForm.h"
#include <vector>
#include "Commands.h"
#include <map>
#include "Addresses.h"

extern bool ProcessCommand(std::string str);

namespace Rosin {
	extern struct CFrame;
	extern Command* GetCmdManager();
	extern std::string DownloadURL(const char* URL, const char* headers = NULL, bool ReplaceN = false);
	namespace Roblox {
		extern int GetLocalPlayer(int);
		extern int GetPlayers();
		extern int FindFirstChild(int, std::string);
		extern int FindFirstChild_PartOf(int, std::string);
		extern const char* GetClass(int);
		extern std::string* GetName(int);
		extern void SetIntValue(int, int);
		extern std::vector<DWORD> GetChildren(int);
		extern int GetDataModel();
		extern int GetParent(int);
		extern void SetStringValue(int, std::string);
		extern void SetBoolValue(int BoolValue, bool Val);
		extern void SetNumberValue(int NumberValue, __int64 Val);
		extern int GetService(std::string Name);
		extern void RemoveInstance(int);
		extern std::string GetFullName(int);
		extern double GetDistributedGameTime(int);
		extern std::vector<DWORD> GetCharacter(std::string);
		extern int setPartCFrame(int Ins, float x, float y, float z);
		extern void SetFogEnd(int l, float f);
		extern void SetFogStart(int l, float f);
		extern void SetFogColor(int Lighting, float R, float G, float B);
		extern void SetRobloxLocked(int Ins, int RbxLocked);
		extern int GetRobloxLocked(int Inst);
		extern int GetPlrCharacter(int);
		extern int GetFilteringEnabled(int);
		extern int GetPlayerUserId(int Player);
		extern int GetAccountAge(int Player);
		extern int CreateInstance(const char*);
	}
}

lua_State* RosinState = nullptr;

/*std::map < std::string, std::map<std::string, lua_CFunction> classProps = {
	{"Part", {
		{"Transparency", [](lua_State* L)->int{
		}
	}}
}*/

class Instance {
protected:
	static int lua_ToString(lua_State*);
	static int lua_Index(lua_State*);
	static int lua_NewIndex(lua_State*);

	std::map<std::string, lua_CFunction> luaGetters;
	std::map<std::string, lua_CFunction> luaSetters;

	std::string Name;
	std::string ClassName;

	template <typename T> static inline T *CheckInstance(lua_State *L, int idx, bool selfCheck = true)
	{
		static_assert(std::is_base_of<Instance, T>::value, "T must inherit from Instance");
		void *udata = lua_touserdata(L, idx);
		if (udata == NULL)
		{
			if (selfCheck)
			{
				const char *msg = lua_pushfstring(L, "Instance expected, got %s", lua_typename(L, lua_type(L, idx)));
				lua_pop(L, 1);
				luaL_argerror(L, idx, msg);
			}
			return NULL;
		}
		Instance *inst = *((Instance**)udata);
		T *requested_instance = dynamic_cast<T*>(inst);
		if (requested_instance == NULL)
		{
			if (selfCheck)
			{
				const char *msg = lua_pushfstring(L, "Instance expected, got userdata");
				lua_pop(L, 1);
				luaL_argerror(L, idx, msg);
			}
		}
		return requested_instance;
	}
public:
	void WrapToLua(lua_State*);
};

int Instance::lua_Index(lua_State* L) {
	Instance* inst = CheckInstance<Instance>(L, 1);
	if (inst != nullptr) {
		//lua_pop(L, 1);
		const char* Index = luaL_checkstring(L, 2);
		lua_pop(L, 1);
		if (inst->luaGetters.find(std::string(Index)) != inst->luaGetters.end()) {
			lua_CFunction func = inst->luaGetters[std::string(Index)];
			lua_pop(L, 1);
			return func(L);
		}
		return luaL_error(L, "%s is not a valid member of %s", Index, inst->ClassName.c_str());
	}
}

int Instance::lua_NewIndex(lua_State* L) {
	Instance* inst = CheckInstance<Instance>(L, 1);
	if (inst != nullptr) {
		const char* Index = luaL_checkstring(L, 2);
		lua_pop(L, 1);
		if (inst->luaGetters.find(std::string(Index)) != inst->luaGetters.end()) {
			lua_CFunction func = inst->luaGetters[std::string(Index)];
			lua_pop(L, 1);
			lua_pushvalue(L, 1);
			return func(L);
		}
		return luaL_error(L, "%s is not a valid member of %s", Index, inst->ClassName.c_str());
	}
}

int Instance::lua_ToString(lua_State *L) {
	Instance* inst = CheckInstance<Instance>(L, 1);
	if (inst != nullptr) {
		lua_pushstring(L, inst->Name.c_str());
		return 1;
	}
}

void Instance::WrapToLua(lua_State* L) {
	lua_newtable(L);
	lua_newtable(L);
	lua_pushstring(L, "This metatable is locked.");
	lua_setfield(L, -2, "__metatable");
	lua_pushcfunction(L, Instance::lua_ToString);
	lua_setfield(L, -2, "__tostring");
	lua_pushcfunction(L, Instance::lua_NewIndex);
	lua_setfield(L, -2, "__newindex");
	lua_pushcfunction(L, Instance::lua_Index);
	lua_setfield(L, -2, "__index");
	lua_setmetatable(L, -2);
	Instance** userdata = (Instance**)lua_newuserdata(L, sizeof(this));
	*userdata = this;
	lua_insert(L, 1);
	lua_setmetatable(L, -2);
}


class DataModel : public Instance {
public:
	DataModel() {
		ClassName = "DataModel";
		Name = "Game";

		luaGetters["Shutdown"] = (lua_CFunction)[](lua_State* L)->int {
			exit(0);
		};
	}
};

int RosinSetProperty(lua_State* L) {
	int Ins = luaL_checkinteger(L, 1);
	const char* PropCC = luaL_checkstring(L, 2);

	std::string Prop(PropCC);

	const char* ClassCC = Rosin::Roblox::GetClass(Ins);

	std::string Class(ClassCC);

	//class specific props

	bool setProp = false;

	if (Class == "Part") {
		if (Prop == "Transparency") {
			double Trans = luaL_checknumber(L, 3);
			Rosin::Roblox::SetPartTrans(Ins, Trans);
			setProp = true;
		}
		else if (Prop == "Reflectance") {
			double Reflec = luaL_checknumber(L, 3);
			Rosin::Roblox::SetRelfectance(Ins, Reflec);
			setProp = true;
		}
		else if (Prop == "Locked") {
			if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
			bool val = lua_toboolean(L, 3);
			Rosin::Roblox::SetPartLocked(Ins, val ? 1 : 0);
			setProp = true;
		}
		else if (Prop == "CanCollide") {
			if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
			bool val = lua_toboolean(L, 3);
			Rosin::Roblox::CanCollide(Ins, val ? 1 : 0);
			setProp = true;
		}
	}
	else if (Class == "Lighting") {
		if (Prop == "FogEnd") {
			int fog = luaL_checkinteger(L, 3);
			Rosin::Roblox::SetFogEnd(Ins, fog);
			setProp = true;
		}
		else if (Prop == "FogStart") {
			int fog = luaL_checkinteger(L, 3);
			Rosin::Roblox::SetFogStart(Ins, fog);
			setProp = true;
		}
		else if (Prop == "FogColor") {
			if (lua_istable(L, 3)) {
				lua_getfield(L, 3, "r");
				double r = lua_tonumber(L, -1);
				lua_getfield(L, 3, "g");
				double g = lua_tonumber(L, -1);
				lua_getfield(L, 3, "b");
				double b = lua_tonumber(L, -1);
				Rosin::Roblox::SetFogColor(Ins, r, g, b);
				setProp = true;
			}
			else {
				luaL_argerror(L, 3, "table expected");
			}
		}
	}
	else if (Class == "IntValue") {
		if (Prop == "Value") {
			int val = luaL_checkinteger(L, 3);
			Rosin::Roblox::SetIntValue(Ins, val);
			setProp = true;
		}
	}
	else if (Class == "StringValue") {
		if (Prop == "Value") {
			const char* val = luaL_checkstring(L, 3);
			Rosin::Roblox::SetStringValue(Ins, std::string(val));
			setProp = true;
		}
	}
	else if (Class == "BoolValue") {
		if (Prop == "Value") {
			if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
			bool val = lua_toboolean(L, 3);
			Rosin::Roblox::SetBoolValue(Ins, val);
			setProp = true;
		}
	}
	else if (Class == "Player") {
		if (Prop == "CharacterAppearance") {
			const char* CharApp = luaL_checkstring(L, 3);
			Rosin::Roblox::SetCharApp(Ins, &std::string(CharApp));
			setProp = true;
		}
		else if (Prop == "CanLoadCharacterAppearance") {
			if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
			bool val = lua_toboolean(L, 3);
			Rosin::Roblox::CanLoadCharacterAppearance(&Ins, val ? 1 : 0);
			setProp = true;
		}
		else if (Prop == "Character") {
			int newChar = luaL_checkinteger(L, 3);
			Rosin::Roblox::SetPlrChar(Ins, newChar);
			setProp = true;
		}
		else if (Prop == "PersonalServerRank") {
			int newRank = luaL_checkinteger(L, 3);
			Rosin::Roblox::PSRRR(Ins, newRank);
			setProp = true;
		}
	}
	else if (Class == "Sky") {
		if (Prop == "SkyboxBk") {
			const char* Id = luaL_checkstring(L, 3);
			Rosin::Roblox::SkyboxBk(Ins, &std::string(Id));
			setProp = true;
		}
		else if (Prop == "SkyboxFt") {
			const char* Id = luaL_checkstring(L, 3);
			Rosin::Roblox::SkyboxFt(Ins, &std::string(Id));
			setProp = true;
		}
		else if (Prop == "SkyboxUp") {
			const char* Id = luaL_checkstring(L, 3);
			Rosin::Roblox::SkyboxUp(Ins, &std::string(Id));
			setProp = true;
		}
		else if (Prop == "SkyboxDn") {
			const char* Id = luaL_checkstring(L, 3);
			Rosin::Roblox::SkyboxDn(Ins, &std::string(Id));
			setProp = true;
		}
		else if (Prop == "SkyboxLf") {
			const char* Id = luaL_checkstring(L, 3);
			Rosin::Roblox::SkyboxLf(Ins, &std::string(Id));
			setProp = true;
		}
		else if (Prop == "SkyboxRt") {
			const char* Id = luaL_checkstring(L, 3);
			Rosin::Roblox::SkyboxRt(Ins, &std::string(Id));
			setProp = true;
		}
		else if (Prop == "StarCount") {
			int starCount = luaL_checkinteger(L, 3);
			Rosin::Roblox::SetStarCount(Ins, starCount);
			setProp = true;
		}
	}
	else if (Class == "Sound") {
		if (Prop == "SoundId") {
			const char* Id = luaL_checkstring(L, 3);
			Rosin::Roblox::SoundId(Ins, std::string(Id));
			setProp = true;
		}
		else if (Prop == "Volume") {
			float Volume = luaL_checknumber(L, 3);
			Rosin::Roblox::SetVolume(Ins, Volume);
			setProp = true;
		}
		else if (Prop == "Pitch") {
			float Pitch = luaL_checknumber(L, 3);
			Rosin::Roblox::SetPitch(Ins, Pitch);
			setProp = true;
		}
	}
	else if (Class == "TextLabel") {
		if (Prop == "Text") {
			const char* newText = luaL_checkstring(L, 3);
			Rosin::Roblox::SetTLText(Ins, newText);
			setProp = true;
		}
		else if (Prop == "Visible") {
			if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
			bool val = lua_toboolean(L, 3);
			Rosin::Roblox::SetGuiVisible(Ins, val ? 1 : 0);
			setProp = true;
		}
		else if (Prop == "Size") {
			if (lua_istable(L, 3)) {
				lua_getfield(L, 3, "x");
				int a1 = lua_tointeger(L, -1);
				lua_getfield(L, 3, "xx");
				int a2 = lua_tointeger(L, -1);
				lua_getfield(L, 3, "y");
				int a3 = lua_tointeger(L, -1);
				lua_getfield(L, 3, "yy");
				int a4 = lua_tointeger(L, -1);
				Rosin::Roblox::SetGuiSize(Ins, a1, a2, a3, a4);
				setProp = true;
			}
			else
			{
				luaL_argerror(L, 3, "table expected");
			}
		}
		else if (Prop == "Position") {
			if (lua_istable(L, 3)) {
				lua_getfield(L, 3, "x");
				int a1 = lua_tointeger(L, -1);
				lua_getfield(L, 3, "xx");
				int a2 = lua_tointeger(L, -1);
				lua_getfield(L, 3, "y");
				int a3 = lua_tointeger(L, -1);
				lua_getfield(L, 3, "yy");
				int a4 = lua_tointeger(L, -1);
				lua_pop(L, 0);
				Rosin::Roblox::SetGuiPos(Ins, a1, a2, a3, a4);
				setProp = true;
			}
			else
			{
				luaL_argerror(L, 3, "table expected");
			}
		}
	}
	else if (Class == "HopperBin") {
		if (Prop == "BinType") {
			int Type = luaL_checkinteger(L, 3);
			Rosin::Roblox::SetBinType(Ins, Type);
			setProp = true;
		}
	}

	//props that work on any class

	if (Prop == "Name") {
		const char* newName = luaL_checkstring(L, 3);
		Rosin::Roblox::SetName(Ins, &std::string(newName));
		setProp = true;
	}
	else if (Prop == "Parent") {
		int newParent = luaL_checkinteger(L, 3);
		Rosin::Roblox::rSetParent(Ins, newParent);
		setProp = true;
	}
	else if (Prop == "Archivable") {
		if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
		bool val = lua_toboolean(L, 3);
		Rosin::Roblox::SetArchivable(Ins, val ? 1 : 0);
		setProp = true;
	}

	if (Prop == "RobloxLocked") {
		if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
		bool val = lua_toboolean(L, 3);
		Rosin::Roblox::SetRobloxLocked(val, val ? 1 : 0);
		setProp = true;
	}


	lua_pushboolean(L, setProp ? 1 : 0);
	return 1;
}

int RosinGetProperty(lua_State* L) {
	int Ins = luaL_checkinteger(L, 1);
	const char* PropCC = luaL_checkstring(L, 2);

	std::string Prop(PropCC);

	const char* ClassCC = Rosin::Roblox::GetClass(Ins);

	std::string Class(ClassCC);

	if (Class == "Player") {
		if (Prop == "Character") {
			lua_pushinteger(L, Rosin::Roblox::GetPlrCharacter(Ins));
			return 1;
		}
		else if (Prop == "userId") {
			lua_pushinteger(L, Rosin::Roblox::GetPlayerUserId(Ins));
			return 1;
		}
		else if (Prop == "AccountAge") {
			lua_pushinteger(L, Rosin::Roblox::GetAccountAge(Ins));
			return 1;
		}
	}
	else if (Class == "Workspace") {
		if (Prop == "FilteringEnabled") {
			lua_pushboolean(L, Rosin::Roblox::GetFilteringEnabled(Ins));
			return 1;
		}
	}
	else if (Class == "Players") {
		if (Prop == "LocalPlayer") {
			lua_pushinteger(L, Rosin::Roblox::GetLocalPlayer(Ins));
			return 1;
		}
		else if (Prop == "localPlayer") {
			lua_pushinteger(L, Rosin::Roblox::GetLocalPlayer(Ins));
			return 1;
		}
	}

	if (Prop == "Name") {
		lua_pushstring(L, Rosin::Roblox::GetName(Ins)->c_str());
		return 1;
	}
	else if (Prop == "ClassName") {
		lua_pushstring(L, Rosin::Roblox::GetClass(Ins));
		return 1;
	}
	else if (Prop == "Parent") {
		lua_pushinteger(L, Rosin::Roblox::GetParent(Ins));
		return 1;
	}
	else if (Prop == "RobloxLocked") {
		lua_pushboolean(L, Rosin::Roblox::GetRobloxLocked(Ins));
		return 1;
	}

	return 0;
}

int RosinCallFunction(lua_State* L) {
	int Ins = luaL_checkinteger(L, 1);
	const char* FuncCC = luaL_checkstring(L, 2);

	std::string Func(FuncCC);

	const char* ClassCC = Rosin::Roblox::GetClass(Ins);

	std::string Class(ClassCC);

	if (Class == "Sound") {
		if (Func == "Play") {
			Rosin::Roblox::playSound(Ins);
		}
		else if (Func == "Stop") {
			Rosin::Roblox::stopMusic(Ins);
		}
		else if (Func == "play") {
			Rosin::Roblox::playSound(Ins);
		}
		else if (Func == "stop") {
			Rosin::Roblox::stopMusic(Ins);
		}
	}
	else if (Class == "Part") {
		if (Func == "BreakJoints") {
			Rosin::Roblox::BreakJoints(Ins);
		}
		else if (Func == "breakJoints") {
			Rosin::Roblox::BreakJoints(Ins);
		}
	}
	else if (Class == "Model") {
		if (Func == "BreakJoints") {
			Rosin::Roblox::BreakJoints(Ins);
		}
		else if (Func == "breakJoints") {
			Rosin::Roblox::BreakJoints(Ins);
		}
	}
	else if (Class == "Workspace") {
		if (Func == "BreakJoints") {
			Rosin::Roblox::BreakJoints(Ins);
		}
		else if (Func == "breakJoints") {
			Rosin::Roblox::BreakJoints(Ins);
		}
	}
	else if (Class == "Terrain") {
		if (Func == "Clear") {
			Rosin::Roblox::ClearTerrain(Ins);
		}
		else if (Func == "clear") {
			Rosin::Roblox::ClearTerrain(Ins);
		}
	}
	else if (Class == "Players") {
		if (Func == "Chat") {
			const char* toChat = luaL_checkstring(L, 3);
			Rosin::Roblox::SendChat(Ins, toChat);
		}
		else if (Func == "chat") {
			const char* toChat = luaL_checkstring(L, 3);
			Rosin::Roblox::SendChat(Ins, toChat);
		}
	}

	if (Func == "Remove") {
		Rosin::Roblox::RemoveInstance(Ins);
	}
	else if (Func == "remove") {
		Rosin::Roblox::RemoveInstance(Ins);
	}
}

int RosinGetService(lua_State* L) {
	const char* ServName = luaL_checkstring(L, 1);
	int Serv = Rosin::Roblox::GetService(std::string(ServName));
	if (Serv == 0) luaL_error(L, "'%s' is not a valid service name.", ServName);
	lua_pushinteger(L, Serv);
	return 1;
}

int RosinGetChildren(lua_State* L) {
	int Ins = luaL_checkinteger(L, 1);
	std::vector<DWORD> Children = Rosin::Roblox::GetChildren(Ins);
	lua_newtable(L);
	for (int c = 0; c < Children.size(); c++) {
		lua_pushinteger(L, c + 1);
		lua_pushinteger(L, Children[c]);
		lua_settable(L, -3);
	}
	lua_pushvalue(L, -1);
	return 1;
}

int RosinFindFirstChild(lua_State* L) {
	int Ins = luaL_checkinteger(L, 1);
	const char* Name = luaL_checkstring(L, 2);
	int ret = Rosin::Roblox::FindFirstChild(Ins, std::string(Name));
	lua_pushinteger(L, ret);
	return 1;
}

int LuaExecuteCommand(lua_State* L) {
	const char* Str = luaL_checkstring(L, 1);
	bool Return = ProcessCommand(std::string(Str));
	lua_pushboolean(L, Return ? 1 : 0);
	return 1;
}

DWORD SingletonClass = 0x568980;
DWORD CloseSingleton = 0x541F70;
DWORD Printf = 0x568910;
void inline Output(int Type, const char* Message, ...) {
	unsigned __int64 arg_1;
	__asm {
		sub esp, 4
			lea eax, arg_1
			push eax
			mov eax, SingletonClass
			call eax
			lea ecx, dword ptr[ebp + 16]
			push ecx
			push dword ptr[ebp + 12]
			push dword ptr[ebp + 8]
			push dword ptr[eax]
			mov eax, Printf
			call eax
			add esp, 20
			lea ecx, dword ptr arg_1
			mov eax, CloseSingleton
			call eax
	}
}

static int LuaRosinPrint(lua_State* L) {
	int n = lua_gettop(L);  /* number of arguments */
	int i;
	std::string out;
	lua_getglobal(L, "tostring");
	for (i = 1; i <= n; i++) {
		const char *s;
		lua_pushvalue(L, -1);  /* function to be called */
		lua_pushvalue(L, i);   /* value to print */
		lua_call(L, 1, 1);
		s = lua_tostring(L, -1);  /* get result */
		if (s == NULL)
			return luaL_error(L, LUA_QL("tostring") " must return a string to "
				LUA_QL("print"));
		if (i > 1) out += "\t";
		out += s;
		lua_pop(L, 1);  /* pop result */
	}
	Rosin::Form::GetBaseInst()->OutputText(out);
	return 0;
}

static int LuaRosinWarn(lua_State* L) {
	int n = lua_gettop(L);  /* number of arguments */
	int i;
	std::string out;
	lua_getglobal(L, "tostring");
	for (i = 1; i <= n; i++) {
		const char *s;
		lua_pushvalue(L, -1);  /* function to be called */
		lua_pushvalue(L, i);   /* value to print */
		lua_call(L, 1, 1);
		s = lua_tostring(L, -1);  /* get result */
		if (s == NULL)
			return luaL_error(L, LUA_QL("tostring") " must return a string to "
				LUA_QL("print"));
		if (i > 1) out += "\t";
		out += s;
		lua_pop(L, 1);  /* pop result */
	}
	Output(2, out.c_str());
	return 0;
}

int RosinPrintIdentity(lua_State* L) {
	Output(0, "Current identity is 7");
	return 0;
}

int RosinGetPlayerPos(lua_State* L) {
	const char* plr = luaL_checkstring(L, 1);
	std::vector<DWORD> plrs = Rosin::Roblox::GetCharacter(std::string(plr));
	lua_newtable(L);
	for (int c = 0; c < plrs.size(); c++) {
		DWORD Char = plrs[c];
		std::string PlayerName = *Rosin::Roblox::GetName(Char);
		int Torso = Rosin::Roblox::FindFirstChild_PartOf(Char, "Torso");
		if (Torso) {
			Rosin::CFrame* cf = Rosin::Roblox::GetCFrame((void*)Torso);
			
			lua_newtable(L);
			lua_pushnumber(L, cf->position.x);
			lua_setfield(L, -2, "x");
			lua_pushnumber(L, cf->position.y);
			lua_setfield(L, -2, "y");
			lua_pushnumber(L, cf->position.z);
			lua_setfield(L, -2, "z");
			lua_setfield(L, -2, PlayerName.c_str());
		}
	}
	return 1;
}

int RosinSetPlayerPos(lua_State* L) {
	const char* plr = luaL_checkstring(L, 1);
	double x = luaL_checknumber(L, 2);
	double y = luaL_checknumber(L, 3);
	double z = luaL_checknumber(L, 4);
	std::vector<DWORD> plrs = Rosin::Roblox::GetCharacter(std::string(plr));
	for (int c = 0; c < plrs.size(); c++) {
		DWORD Char = plrs[c];
		int Torso = Rosin::Roblox::FindFirstChild_PartOf(Char, "HumanoidRootPart");
		if (Torso) {
			Rosin::Roblox::setPartCFrame(Torso, x, y, z);
		}
	}
	return 0;
}

int LuaWait(lua_State* L) {
	int time = luaL_checknumber(L, 1);
	Sleep(time * 1000);
	return 0;
}

int LuaRbxTime(lua_State* L) {
	int Workspace = Rosin::Roblox::GetService("Workspace");
	if (Workspace) {
		lua_pushnumber(L, Rosin::Roblox::GetDistributedGameTime(Workspace));
		return 1;
	}
	return 0;
}

std::map<const char*, int> refs;

int RosinNewCommand(lua_State* L) {
	const char* cmd = luaL_checkstring(L, 1);
	if (lua_isfunction(L, 2)) {
		lua_pushvalue(L, 2);
		int ref = luaL_ref(L, LUA_REGISTRYINDEX);
		refs[cmd] = ref;
		Rosin::Command* CmdManager = Rosin::GetCmdManager();
		CmdManager->NewCommand(cmd, { cmd }, "", (Rosin::CommandFunction)[cmd](std::vector<std::string> In)->void {
			lua_newtable(RosinState);
			for (int c = 0; c < In.size(); c++) {
				std::string val = In[c];
				lua_pushinteger(RosinState, c + 1);
				lua_pushstring(RosinState, val.c_str());
				lua_settable(RosinState, -3);
			}
			lua_rawgeti(RosinState, LUA_REGISTRYINDEX, refs[cmd]);
			lua_pushvalue(RosinState, -2);
			int ret = lua_pcall(RosinState, 1, 0, 0);
			if (ret != 0) {
				const char* error = lua_tostring(RosinState, -1);
				lua_pop(RosinState, 1);
				std::string err = "Command (";
				err += cmd;
				err += ") error:\n";
				err += error;
				Rosin::Form::GetBaseInst()->OutputText(err, RGB(255, 0, 0));
			}
		});
	}
	else {
		luaL_error(L, "Something went wrong!");
	}
	return 0;
}

int RosinRemoveCommand(lua_State* L) {
	const char* cmd = luaL_checkstring(L, 1);
	Rosin::Command* CmdManager = Rosin::GetCmdManager();
	bool ret = CmdManager->RemoveCommand(cmd);
	if (!ret) {
		return luaL_error(L, "Command '%s' not found.", cmd);
	}
	lua_pushboolean(L, 1);
	return 1;
}

int RosinHttpGet(lua_State* L) {
	const char* URL = luaL_checkstring(L, 1);
	std::string Ret = Rosin::DownloadURL(URL);
	lua_pushstring(L, Ret.c_str());
	return 1;
}

int RosinCreateInstance(lua_State* L) {
	int newParent = 0;
	const char* Class = luaL_checkstring(L, 1);
	if (!lua_isnil(L, 2)) {
		newParent = lua_tointeger(L, 2);
	}
	int Ins = Rosin::Roblox::CreateInstance(Class);
	if(Ins == 0){
		luaL_error(L, "We were unable to make that instance.");
	}
	if (Ins == -1) {
		luaL_error(L, "Uh oh! We've run into an error!");
	}
	if (newParent != 0) {
		Rosin::Roblox::rSetParent(Ins, newParent);
	}
	lua_pushinteger(L, Ins);
	return 1;
}

lua_State* SetupLuaState(lua_State* L) {

	luaL_openlibs(L);

	lua_pushcfunction(L, LuaRosinPrint);
	lua_setglobal(L, "print");

	lua_pushcfunction(L, LuaExecuteCommand);
	lua_setglobal(L, "execute");

	lua_pushcfunction(L, LuaWait);
	lua_setglobal(L, "wait");

	lua_pushcfunction(L, LuaRbxTime);
	lua_setglobal(L, "time");

	lua_pushcfunction(L, LuaRosinWarn);
	lua_setglobal(L, "warn");

	lua_pushcfunction(L, RosinPrintIdentity);
	lua_setglobal(L, "printidentity");

	lua_pushcfunction(L, RosinGetPlayerPos);
	lua_setglobal(L, "GetPlayerPos");

	lua_pushcfunction(L, RosinSetPlayerPos);
	lua_setglobal(L, "SetPlayerPos");

	//

	lua_createtable(L, 0, 0);

	lua_pushcfunction(L, RosinNewCommand);
	lua_setfield(L, -2, "new");

	lua_pushcfunction(L, RosinRemoveCommand);
	lua_setfield(L, -2, "remove");

	lua_setglobal(L, "Commands");

	//

	lua_pushinteger(L, Rosin::Roblox::GetDataModel());
	lua_setglobal(L, "game");

	lua_pushinteger(L, Rosin::Roblox::GetDataModel());
	lua_setglobal(L, "Game");

	lua_pushinteger(L, Rosin::Roblox::GetService("Workspace"));
	lua_setglobal(L, "workspace");

	lua_pushinteger(L, Rosin::Roblox::GetService("Workspace"));
	lua_setglobal(L, "Workspace");

	lua_pushcfunction(L, RosinGetChildren);
	lua_setglobal(L, "GetChildren");

	lua_pushcfunction(L, RosinSetProperty);
	lua_setglobal(L, "SetProperty");

	lua_pushcfunction(L, RosinGetProperty);
	lua_setglobal(L, "GetProperty");

	lua_pushcfunction(L, RosinFindFirstChild);
	lua_setglobal(L, "FindFirstChild");

	lua_pushcfunction(L, RosinGetService);
	lua_setglobal(L, "GetService");

	lua_pushcfunction(L, RosinCallFunction);
	lua_setglobal(L, "CallMethod");

	//

	lua_newtable(L);

	lua_pushcfunction(L, RosinHttpGet);
	lua_setfield(L, -2, "get");

	lua_setglobal(L, "Http");

	//

	lua_newtable(L);

	lua_pushcfunction(L, RosinCreateInstance);
	lua_setfield(L, -2, "new");

	lua_setglobal(L, "Instance");

	//

	lua_pushnil(L);
	lua_setglobal(L, "debug");

	DataModel* DM = new DataModel();
	DM->WrapToLua(L);
	lua_setglobal(L, "DataModel");

	return L;
}

void LuaExecuteString(std::string Source) {
	if (RosinState == nullptr) {
		RosinState = luaL_newstate();
		SetupLuaState(RosinState);
	}

	int ret = luaL_dostring(RosinState, Source.c_str());
	if (ret) {
		const char* err = lua_tostring(RosinState, 1);
		Rosin::Form::GetBaseInst()->OutputText(std::string("Lua Error:\n" + std::string(err)), RGB(255, 0, 0));
	}
}

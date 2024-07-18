#pragma once
#include "SDK.h"
#include "DevTools.h"
#include "VMProtectSDK.h"

typedef int(*lua_CFunction) (int *L);

std::vector<const char*> getChildren_noLP(int userdata);

namespace rLua {
	int state;
	int sc;

	auto getGlobalState = (int(__thiscall*)(int, int))Addys::state;

	auto getfield = (void(__cdecl*)(int, int, const char*))Retcheck::unprotect((BYTE*)Addys::getfield);
	auto pushvalue = (int(__cdecl*)(int, int))Retcheck::unprotect((BYTE*)Addys::pushvalue);
	auto pcall = (int(__cdecl*)(int, int, int, int))Retcheck::unprotect((BYTE*)Addys::pcall);
	auto pushstring = (void(__cdecl*)(int, const char*))Addys::pushstring;
	auto settop = (void(__cdecl*)(int, int))Retcheck::unprotect((BYTE*)Addys::settop);
	auto pushnumber = (void(__cdecl*)(int, double))Retcheck::unprotect((BYTE*)Addys::pushnumber);
	auto touserdata = (void*(__cdecl*)(int, int))Retcheck::unprotect((BYTE*)Addys::touserdata);
	auto setfield = (void(__cdecl*)(int, int, const char*))Retcheck::unprotect((BYTE*)Addys::setfield);
	auto pushbool = (void(__cdecl*)(int, int))Retcheck::unprotect((BYTE*)Addys::pushbool);
	auto pushlstring = (void(__cdecl*)(int, const char*, size_t))Retcheck::unprotect((BYTE*)Addys::pushlit);
	//auto gettable = (void(__cdecl*)(int, int))Retcheck::unprotect((BYTE*)Addys::gettable);
	//auto settable = (void(__cdecl*)(int, int))Retcheck::unprotect((BYTE*)Addys::settable);
	auto insert = (void(__cdecl*)(int, int))Retcheck::unprotect((BYTE*)Addys::insert);
	auto remove = (void(__cdecl*)(int, int))Retcheck::unprotect((BYTE*)Addys::remove);
	auto pushcclosure = (void(__cdecl*)(int, void(int state), int))Retcheck::unprotect((BYTE*)Addys::pushclosure);
	auto tolstring = (const char*(__cdecl*)(int, int, int))Retcheck::unprotect((BYTE*)Addys::tolstring);
	auto newuserdata = (void*(__cdecl*)(int, size_t))Retcheck::unprotect((BYTE*)Addys::pushuserdata);
	auto setmetatable = (int(__cdecl*)(int, int))Retcheck::unprotect((BYTE*)Addys::setmetatable);
	auto toboolean = (int(__cdecl*)(int, int))Retcheck::unprotect((BYTE*)Addys::toboolean);

	inline void pushnil(DWORD rL)
	{
		*(DWORD*)(rL + 16) += 16;
	}

	inline void getglobal(int state, const char* field) {
		getfield(state, -10002, field);
	}

	inline void pop(int state, int idx) {
		settop(state, -(idx)-1);
	}

	inline void pushliteral(int state, const char* lit) {
		pushlstring(state, lit, sizeof(lit));
	}

	namespace Utils {
		// ends up at -1 on the stack
		void getInstance(std::vector<const char*> path) {
			getglobal(state, "game");
			for (int i = 0; i < path.size(); i++) {
				getfield(state, -1, path.at(i));
			}
		}

		// TODO: PROPS
		//create an instance and returns the position on the stack
		int newInstance(std::vector<const char*> parent, const char* type, const char* name) {
			VMProtectBeginMutation("lo12l-thisismyassasssssssdluacfuckoff");
			//create instance
			getglobal(state, "Instance");
			getfield(state, -1, "new");
			pushstring(state, type);
			pcall(state, 1, 1, 0);
			//set parent
			getglobal(state, "game");
			for (int i = 0; i < parent.size(); i++) {
				getfield(state, -1, parent.at(i));
			}
			int inst_idx = (-((signed int)parent.size() - 1)) - 3;
			setfield(state, inst_idx, "Parent");
			//set name
			pushstring(state, name);
			setfield(state, inst_idx, "Name");
			VMProtectEnd();
			return inst_idx;
		}

		// deletes instance
		void deleteInstance(std::vector<const char*> path) {
			VMProtectBeginMutation("lo23l-thisisadsadsmyluacfucvvvdsskoff");
			getglobal(state, "game");
			for (int i = 0; i < path.size(); i++) {
				getfield(state, -1, path.at(i));
			}
			getfield(state, -1, "Destroy");
			pushvalue(state, -2); 
			pcall(state, 1, 0, 0);
			settop(state, 0);
			VMProtectEnd();
		}

		//create a new vector3 at -1 on the stack
		void newVector3(float x, float y, float z) {
			VMProtectBeginMutation("locl-cthisismyluacasdkas33fdvvvfuckoff");
			Vector3 new_cf;
			new_cf.x = x;
			new_cf.y = y;
			new_cf.z = z;
			Vector3* cf_ptr = (Vector3*)newuserdata(state, sizeof(new_cf) + 0x4);
			*(Vector3*)(cf_ptr + 1) = new_cf;
			getfield(state, -10000, "Vector3");
			setmetatable(state, -2);
			VMProtectEnd();
		}

		//create a new cframe at -1 on the stack
		void newCFrame(float x, float y, float z) {
			VMProtectBeginMutation("locl-cthisismyluazxczxcgtcasdasfdvvvfuckoff");
			CFrame new_cf;
			new_cf.position.x = x;
			new_cf.position.y = y;
			new_cf.position.z = z;
			CFrame* cf_ptr = (CFrame*)newuserdata(state, sizeof(new_cf) + 0x4);
			*(CFrame*)(cf_ptr + 1) = new_cf;
			getfield(state, -10000, "CFrame");
			setmetatable(state, -2);
			VMProtectEnd();
		}

		//create a new cframe at -1 on the stack
		void newUDim2(float sx, int ox, float sy, int oy) {
			VMProtectBeginMutation("locl-cthisismyluacasdaszxcfdvvvfuckoff");
			UDim2 new_udim;
			new_udim.X.Scale = sx;
			new_udim.X.Offset = ox;
			new_udim.Y.Scale = sy;
			new_udim.Y.Scale = oy;
			int* udm_ptr = (int*)newuserdata(state, sizeof(new_udim) + 0x4);
			*(UDim2*)(udm_ptr + 1) = new_udim;
			getfield(state, -10000, "UDim2");
			setmetatable(state, -2);
			VMProtectEnd();
		}

		// TODO: implement 'short names'
		std::vector<const char*> parsePlayers(std::string &tok) {
			VMProtectBeginMutation("lol-cxthisismydasdaslasduacfuckoff");
			std::vector<const char*> ret;
			if (tok == "me") {
				ret.push_back("LocalPlayer");
			} else if(tok == "all") {
				getglobal(state, "game");
				getfield(state, -1, "Players");
				int plrs_obj = (int)touserdata(state, -1);
				std::vector<const char*> plrs = getChildren(plrs_obj);
				return plrs;
			} else  if(tok == "others") {
				getglobal(state, "game");
				getfield(state, -1, "Players");
				int plrs_obj = (int)touserdata(state, -1);
				std::vector<const char*> plrs = getChildren_noLP(plrs_obj);
				return plrs;
			} else {
				// plrs
				getglobal(state, "game");
				getfield(state, -1, "Players");
				int plrs_obj = (int)touserdata(state, -1);
				std::vector<const char*> plrs = getChildren(plrs_obj);
				std::vector<const char*> ret_t;
				// search
				for (int i = 0; i < plrs.size(); i++) {
					std::string name = std::string(plrs[i]);
					if (name.substr(0, tok.length()) == tok) {
						ret_t.push_back(plrs[i]);
					}
				}
				return ret_t;
			}
			VMProtectEnd();
			return ret;
		}
	}
};

std::vector<const char*> getChildren_noLP(int userdata) {
	VMProtectBeginMutation("lol-thisismyluacadsdsaasdfuc34asdkoff");
	// localplayer name
	rLua::getglobal(rLua::state, "game");
	rLua::getfield(rLua::state, -1, "Players");
	rLua::getfield(rLua::state, -1, "LocalPlayer");
	rLua::getfield(rLua::state, -1, "Name");
	const char* lpname = rLua::tolstring(rLua::state, -1, NULL);
	int Instance = *(int*)(userdata + 4);
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	std::vector<const char*> ret;
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8) {
		int ptr = *(int*)i;

		const char* name = (const char*)(*(int*)(ptr + 0x28));
		if (name != lpname) {
			ret.push_back(name);
		}
	}
	VMProtectEnd();
	return ret;
}
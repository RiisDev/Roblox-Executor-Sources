#include "Rlua.h"
#include "CoreFunctions.h"
namespace GlobalsData {

	static int UserDataGC(lua_State* L) {
		void* UD = lua_touserdata(L, 1);
		lua_pushvalue(L, 1);
		lua_gettable(L, LUA_REGISTRYINDEX);
		if (!lua_isnil(L, -1)) {
			int Reference = lua_tointeger(L, -1);
			rlua_pushnil(luaState);
			rlua_rawseti(luaState, LUA_REGISTRYINDEX, Reference);
			rlua_settop(luaState, 0);
			writerf(1, "CheatSquad -> Garbage Collected!");
			return 1;
		}
		lua_pop(L, 1);
		return 0;
	}


	int GlobalsIndexMetamethod(lua_State* L) {
		std::string GlobalKey = lua_tostring(L, -1); 
		std::cout << GlobalKey << std::endl; 
		lua_pop(L, 1); 

		rlua_getfield(luaState, LUA_GLOBALSINDEX, GlobalKey.c_str()); 
		wrap(L, rL, -1); 
		r_lua_pop(luaState, 1); 

		return 1;
	}

	int GlobalsNewIndexMetamethod(lua_State* L) {

		std::string GlobalKey = lua_tostring(L, -2);
		std::cout << GlobalKey << std::endl; 
		unwrap(L, rL, -1);
		rlua_setfield(luaState, LUA_GLOBALSINDEX, GlobalKey.c_str());
		r_lua_pop(luaState, 1);
		return 1;
	}

	void Register(lua_State* L) {
		lua_createtable(L, 0, 0); //main table
		lua_createtable(L, 0, 0); //metatable

		lua_pushcclosure(L, GlobalsIndexMetamethod, 0);
		lua_setfield(L, -2, "__index");

		lua_pushcclosure(L, GlobalsNewIndexMetamethod, 0);
		lua_setfield(L, -2, "__newindex");

		lua_pushcclosure(L, UserDataGC, 0);
		lua_setfield(L, -2, "__gc");

		lua_setmetatable(L, -2);
		lua_setglobal(L, "_G");
	}
}
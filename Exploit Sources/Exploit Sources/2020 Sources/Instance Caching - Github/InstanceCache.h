void cacheObject(uintptr_t rL, lua_State* L, int index)
	{

	
		const auto robloxPtr = (void*)r_lua_touserdata(rL, index);/*grabs a userdata from it's index*/
		const auto iterator = vanillaInstanceMap.find((void*)robloxPtr);/*find's the roblox's userdata from it's current mapped*/

		if (iterator == vanillaInstanceMap.cend()) { /*uses cend to cache all the used stuff*/
			r_lua_pushvalue(rL, index);
			reinterpret_cast<Userdata*>(lua_newuserdata(L, sizeof(Userdata)))->reference = r_luaL_ref(rL, LUA_REGISTRYINDEX);/*calls roblox's ref by using newuserdata*/
		
			r_lua_getmetatable(rL, index);/*grabs the metatable to it's index*/

			Bridge::push(rL, L, -1);/*push to vanilla*/
		
			lua_setmetatable(L, -2);/*set the metatable*/
		
			lua_pushvalue(L, -1);/*push the value and set it to -1 (index)*/
			lua_rawseti(L, LUA_REGISTRYINDEX, ++registryIndex);/*rawseti the registryindex aka LUA_REGISTRYINDEX (-10000) but it's set as a var*/

			vanillaInstanceMap[robloxPtr] = (void*)registryIndex;/*used vanilla cache map*/

			r_lua_pop(rL, 1);/*pop the stack lol*/

		}
		else
		{

			lua_rawgeti(L, LUA_REGISTRYINDEX, (int)iterator->second);
		}
	}

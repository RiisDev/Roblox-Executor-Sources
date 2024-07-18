/* if your retarded as fuck dont try using this, if your not a retard then go ahead. this is mainly for Bytecode/Proto Conversion. shade "Made" this so i fixed it for the skiddo's have fun*/

int FireClickDetector(DWORD rL)
	{
		r_lua_getglobal(rL, "game");
		r_lua_getfield(rL, -1, "GetService");
		r_lua_insert (rL, -2);
		r_lua_pushstring(rL, "Players");
		r_lua_pcall(rL, 2, 1, 0);
		r_lua_getfield(rL, -1, "LocalPlayer");
		uintptr_t LocalPtr = *reinterpret_cast<uintptr_t*>(r_lua_touserdata(rL, -1));

		

		float Distance = 0.0;
		if (r_lua_gettop(rL) == 2)
		{
			Distance = (float)r_lua_tonumber(rL, 2, 0);
			r_lua_remove(rL, 2);
		}

		uintptr_t clickDetectorPointer = *reinterpret_cast<uintptr_t*>(r_lua_touserdata(rL, 1));
		
		r_fireclickdetector(clickDetectorPointer, Distance, LocalPtr);
		
		return 0;
	}

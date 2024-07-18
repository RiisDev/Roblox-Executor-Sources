/* hi axon skids */

int FireClickDetector(lua_State* L)
	{ 
	        DWORD rL = m_rL;
		r_lua_getglobal(rL, "game");
		r_lua_getfield(rL, -1, "GetService");
		r_lua_insert (rL, -2);
		r_lua_pushstring(rL, "Players");
		r_lua_pcall(rL, 2, 1, 0);
		r_lua_getfield(rL, -1, "LocalPlayer");
		uintptr_t LocalPtr = *reinterpret_cast<uintptr_t*>(r_lua_touserdata(rL, -1));

		

		float Distance = 0.0;
		if (lua_gettop(L) == 2)
		{
			Distance = (float)lua_tonumber(L, 2);
			r_lua_remove(rL, 2);
		}

    Bridge::push(m_L, m_rL, -1);
  
		uintptr_t clickDetectorPointer = *reinterpret_cast<uintptr_t*>(r_lua_touserdata(rL, 1));
		
		r_fireclickdetector(clickDetectorPointer, Distance, LocalPtr);
		
		return 0;
	}

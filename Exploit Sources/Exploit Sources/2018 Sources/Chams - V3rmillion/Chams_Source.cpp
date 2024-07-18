void newCham(DWORD rL) {
	const char* localName;

	Chams::RLua::getglobal(rL, "game"); 
	Chams::RLua::getfield(rL, -1, "Players"); 

	Chams::RLua::getfield(rL, -1, "LocalPlayer"); 
	Chams::RLua::getfield(rL, -1, "Name"); 
	localName = Chams::RLua::tostring(rL, -1);
	Chams::RLua::pop(rL, 2); 

	Chams::RLua::getfield(rL, -1, "GetPlayers"); 
	Chams::RLua::pushvalue(rL, -2); 
	Chams::RLua::pcall(rL, 1, 1, 0); 

	Chams::RLua::pushnil(rL); 
	while (Chams::RLua::next(rL, -2) != R_LUA_TNIL) { 

		bool validChar = false;
		bool validName = false;

		Chams::RLua::getfield(rL, -1, "Character"); 
		validChar = Chams::RLua::type(rL, -1) != R_LUA_TNIL;

		Chams::RLua::getfield(rL, -2, "Name"); 

		if (strcmp(Chams::RLua::tostring(rL, -1), localName) != 0)
			validName = true;

		Chams::RLua::pop(rL, 1); 

		if (validChar && validName) {
			Chams::RLua::getfield(rL, -1, "GetChildren"); 
			Chams::RLua::pushvalue(rL, -2); 
			Chams::RLua::pcall(rL, 1, 1, 0); 
			Chams::RLua::remove(rL, -2); 

			Chams::RLua::pushnil(rL); 
			while (Chams::RLua::next(rL, -2) != R_LUA_TNIL) { 
				bool isABasePart;

				Chams::RLua::getfield(rL, -1, "IsA"); 
				Chams::RLua::pushvalue(rL, -2); 
				Chams::RLua::pushstring(rL, "BasePart"); 
				Chams::RLua::pcall(rL, 2, 1, 0); 
				isABasePart = Chams::RLua::toboolean(rL, -1);
				Chams::RLua::pop(rL, 1); 

				if (isABasePart) {
					Chams::RLua::getglobal(rL, "Instance"); 
					Chams::RLua::getfield(rL, -1, "new"); 
					Chams::RLua::pushstring(rL, "BoxHandleAdornment"); 
					Chams::RLua::getglobal(rL, "game"); 
					Chams::RLua::getfield(rL, -1, "CoreGui"); 
					Chams::RLua::remove(rL, -2); 
					Chams::RLua::pcall(rL, 2, 1, 0); 
					Chams::RLua::remove(rL, -2); 

					Chams::RLua::pushstring(rL, "ChamAdornment");  
					Chams::RLua::setfield(rL, -2, "Name"); 

					Chams::RLua::pushvalue(rL, -2); 
					Chams::RLua::setfield(rL, -2, "Adornee"); 

					Chams::RLua::pushboolean(rL, 1); 
					Chams::RLua::setfield(rL, -2, "AlwaysOnTop"); 

					Chams::RLua::pushnumber(rL, 0); 
					Chams::RLua::setfield(rL, -2, "ZIndex"); 

					Chams::RLua::getfield(rL, -2, "Size"); 
					Chams::RLua::setfield(rL, -2, "Size"); 

					Chams::RLua::getfield(rL, -5, "TeamColor"); 
					Chams::RLua::getfield(rL, -1, "Name"); 

					const char* plrTeam;
					const char* localTeam;

					plrTeam = Chams::RLua::tostring(rL, -1);

					Chams::RLua::pop(rL, 2); 

					Chams::RLua::getfield(rL, -8, "LocalPlayer"); 
					Chams::RLua::getfield(rL, -1, "TeamColor"); 
					Chams::RLua::getfield(rL, -1, "Name"); 
					localTeam = Chams::RLua::tostring(rL, -1);

					Chams::RLua::pop(rL, 3); 

					if (strcmp(plrTeam, localTeam) == 0) {
						Chams::RLua::getglobal(rL, "BrickColor"); 
						Chams::RLua::getfield(rL, -1, "new"); 
						Chams::RLua::pushstring(rL, "Lime green"); 
						Chams::RLua::pcall(rL, 1, 1, 0); 
						Chams::RLua::setfield(rL, -3, "Color"); 
					}
					else {
						Chams::RLua::getglobal(rL, "BrickColor"); 
						Chams::RLua::getfield(rL, -1, "new"); 
						Chams::RLua::pushstring(rL, "Really red"); 
						Chams::RLua::pcall(rL, 1, 1, 0); 
						Chams::RLua::setfield(rL, -3, "Color"); 
					}

					Chams::RLua::pop(rL, 3); 
				}
				else
					Chams::RLua::pop(rL, 1); 
			}
		}

		Chams::RLua::pop(rL, 2); 
	}
	Chams::RLua::settop(rL, 0);
}

void deleteCham(DWORD rL) {
	Chams::RLua::getglobal(rL, "game"); 
	Chams::RLua::getfield(rL, -1, "CoreGui"); 
	Chams::RLua::getfield(rL, -1, "GetChildren"); 
	Chams::RLua::pushvalue(rL, -2); 
	Chams::RLua::pcall(rL, 1, 1, 0); 

	Chams::RLua::pushnil(rL); 
	while (Chams::RLua::next(rL, -2) != R_LUA_TNIL) { 
		const char* name;

		Chams::RLua::getfield(rL, -1, "Name"); 
		name = Chams::RLua::tostring(rL, -1);
		Chams::RLua::pop(rL, 1); 

		if (strcmp(name, "ChamAdornment") == 0) {
			Chams::RLua::getfield(rL, -1, "Destroy"); 
			Chams::RLua::pushvalue(rL, -2); 
			Chams::RLua::pcall(rL, 1, 0, 0); 
		}
		Chams::RLua::pop(rL, 1); 
	}
	Chams::RLua::settop(rL, 0);
}

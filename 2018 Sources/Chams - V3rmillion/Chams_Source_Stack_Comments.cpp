void newCham(DWORD rL) {
	const char* localName;

	Chams::RLua::getglobal(rL, "game"); // [game]
	Chams::RLua::getfield(rL, -1, "Players"); // [game, players]

	Chams::RLua::getfield(rL, -1, "LocalPlayer"); // [game, players, localplayer]
	Chams::RLua::getfield(rL, -1, "Name"); // [game, players, localplayer, 'lpname']
	localName = Chams::RLua::tostring(rL, -1);
	Chams::RLua::pop(rL, 2); // [game, players]

	Chams::RLua::getfield(rL, -1, "GetPlayers"); // [game, players, function: getplayers]
	Chams::RLua::pushvalue(rL, -2); // [game, players, function: getplayers, players]
	Chams::RLua::pcall(rL, 1, 1, 0); // [game, players, {plr}]

	Chams::RLua::pushnil(rL); // [game, players, {plr}, nil]
	while (Chams::RLua::next(rL, -2) != R_LUA_TNIL) { // [game, players, {plr}, key, plr]

		bool validChar = false;
		bool validName = false;

		Chams::RLua::getfield(rL, -1, "Character"); // [game, players, {plr}, key, plr, character]
		validChar = Chams::RLua::type(rL, -1) != R_LUA_TNIL;

		Chams::RLua::getfield(rL, -2, "Name"); // [game, players, {plr}, key, plr, character, 'plrname']

		if (strcmp(Chams::RLua::tostring(rL, -1), localName) != 0)
			validName = true;

		Chams::RLua::pop(rL, 1); // [game, players, {plr}, key, plr, character]

		if (validChar && validName) {
			Chams::RLua::getfield(rL, -1, "GetChildren"); // [game, players, {plr}, key, plr, character, function: getchildren]
			Chams::RLua::pushvalue(rL, -2); // [game, players, {plr}, key, plr, character, function: getchildren, character]
			Chams::RLua::pcall(rL, 1, 1, 0); // [game, players, {plr}, key, plr, character, {child}]
			Chams::RLua::remove(rL, -2); // [game, players, {plr}, key, plr, {child}]

			Chams::RLua::pushnil(rL); // [game, players, {plr}, key, plr, {child}, nil]
			while (Chams::RLua::next(rL, -2) != R_LUA_TNIL) { // [game, players, {plr}, key, {child}, key, child]
				bool isABasePart;

				Chams::RLua::getfield(rL, -1, "IsA"); // [game, players, {plr}, key, plr, {child}, key, child, function: isa]
				Chams::RLua::pushvalue(rL, -2); // [game, players, {plr}, key, plr, {child}, key, child, function: isa, child]
				Chams::RLua::pushstring(rL, "BasePart"); // [game, players, {plr}, key, plr, {child}, key, child, function: isa, child, 'basepart']
				Chams::RLua::pcall(rL, 2, 1, 0); // [game, players, {plr}, key, plr, {child}, key, child, true]
				isABasePart = Chams::RLua::toboolean(rL, -1);
				Chams::RLua::pop(rL, 1); // [game, players, {plr}, key, plr, {child}, key, child]

				if (isABasePart) {
					Chams::RLua::getglobal(rL, "Instance"); // [game, players, {plr}, key, plr, {child}, key, child, instance]
					Chams::RLua::getfield(rL, -1, "new"); // [game, players, {plr}, key, plr, {child}, key, child, instance, new]
					Chams::RLua::pushstring(rL, "BoxHandleAdornment"); // [game, players, {plr}, key, plr, {child}, key, child, instance, new, 'boxhandleadornment']
					Chams::RLua::getglobal(rL, "game"); // [game, players, {plr}, key, plr, {child}, key, child, instance, new, 'boxhandleadornment', game]
					Chams::RLua::getfield(rL, -1, "CoreGui"); // [game, players, {plr}, key, plr, {child}, key, child, instance, new, 'boxhandleadornment', game, coregui]
					Chams::RLua::remove(rL, -2); // [game, players, {plr}, key, plr, {child}, key, child, instance, new, 'boxhandleadornment', coregui]
					Chams::RLua::pcall(rL, 2, 1, 0); // [game, players, {plr}, key, plr, {child}, key, child, instance, boxhandleadornment]
					Chams::RLua::remove(rL, -2); // [game, players, {plr}, key,  plr, {child}, key,child, boxhandleadornment]

					Chams::RLua::pushstring(rL, "ChamAdornment");  // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment, chamadornment]
					Chams::RLua::setfield(rL, -2, "Name"); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment]

					Chams::RLua::pushvalue(rL, -2); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment, child]
					Chams::RLua::setfield(rL, -2, "Adornee"); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment]

					Chams::RLua::pushboolean(rL, 1); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment, 1]
					Chams::RLua::setfield(rL, -2, "AlwaysOnTop"); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment]

					Chams::RLua::pushnumber(rL, 0); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment, 0]
					Chams::RLua::setfield(rL, -2, "ZIndex"); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment]

					Chams::RLua::getfield(rL, -2, "Size"); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment, size]
					Chams::RLua::setfield(rL, -2, "Size"); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment]

					Chams::RLua::getfield(rL, -5, "TeamColor"); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment, teamcolor]
					Chams::RLua::getfield(rL, -1, "Name"); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment, teamcolor, teamcolorname]

					const char* plrTeam;
					const char* localTeam;

					plrTeam = Chams::RLua::tostring(rL, -1);

					Chams::RLua::pop(rL, 2); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment]

					Chams::RLua::getfield(rL, -8, "LocalPlayer"); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment, localplayer]
					Chams::RLua::getfield(rL, -1, "TeamColor"); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment, localplayer, teamcolor]
					Chams::RLua::getfield(rL, -1, "Name"); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment, localplayer, teamcolor, teamcolorname]
					localTeam = Chams::RLua::tostring(rL, -1);

					Chams::RLua::pop(rL, 3); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment]

					if (strcmp(plrTeam, localTeam) == 0) {
						Chams::RLua::getglobal(rL, "BrickColor"); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment, brickcolor]
						Chams::RLua::getfield(rL, -1, "new"); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment, brickcolor, new]
						Chams::RLua::pushstring(rL, "Lime green"); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment, brickcolor, new, 'lime green']
						Chams::RLua::pcall(rL, 1, 1, 0); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment, brickcolor, lime green]
						Chams::RLua::setfield(rL, -3, "Color"); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment, brickcolor]
					}
					else {
						Chams::RLua::getglobal(rL, "BrickColor"); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment, brickcolor]
						Chams::RLua::getfield(rL, -1, "new"); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment, brickcolor, new]
						Chams::RLua::pushstring(rL, "Really red"); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment, brickcolor, new, 'really red']
						Chams::RLua::pcall(rL, 1, 1, 0); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment, brickcolor, really red]
						Chams::RLua::setfield(rL, -3, "Color"); // [game, players, {plr}, key, plr, {child}, key, child, boxhandleadornment, brickcolor]
					}

					Chams::RLua::pop(rL, 3); // [game, players, {plr}, key, plr, {child}, key]
				}
				else
					Chams::RLua::pop(rL, 1); // [game, players, {plr}, key, plr, {child}, key]
			}
		}

		Chams::RLua::pop(rL, 2); // [game, players, {plr}, key]
	}
	Chams::RLua::settop(rL, 0);
}

void deleteCham(DWORD rL) {
	Chams::RLua::getglobal(rL, "game"); // [game]
	Chams::RLua::getfield(rL, -1, "CoreGui"); // [game, coregui]
	Chams::RLua::getfield(rL, -1, "GetChildren"); // [game, coregui, getchildren]
	Chams::RLua::pushvalue(rL, -2); // [game, function: getchildren, coregui]
	Chams::RLua::pcall(rL, 1, 1, 0); // [game, {children}]

	Chams::RLua::pushnil(rL); // [game, {children}, nil]
	while (Chams::RLua::next(rL, -2) != R_LUA_TNIL) { // [game, {children}, key, child]
		const char* name;

		Chams::RLua::getfield(rL, -1, "Name"); // [game, {children}, key, child, 'childname']
		name = Chams::RLua::tostring(rL, -1);
		Chams::RLua::pop(rL, 1); // [game, {children}, key, child]

		if (strcmp(name, "ChamAdornment") == 0) {
			Chams::RLua::getfield(rL, -1, "Destroy"); // [game, {children}, key, child, function: destroy]
			Chams::RLua::pushvalue(rL, -2); // [game, {children}, key, child, function: destroy, child]
			Chams::RLua::pcall(rL, 1, 0, 0); // [game, {children}, key, child]
		}
		Chams::RLua::pop(rL, 1); // [game, {children}, key]
	}
	Chams::RLua::settop(rL, 0);
}

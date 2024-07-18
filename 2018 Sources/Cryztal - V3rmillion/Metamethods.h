int print(lua_State *Ls) {
	string args = lua_tolstring(Ls, -2);
	getglobal(L, "print");
	pushstring(args.c_str());
	pcall(L, 1, 0, 0);
}

int warn(lua_State *Ls) {
	string args = lua_tolstring(Ls, -2);
	getglobal(L, "warn");
	pushstring(args.c_str());
	pcall(L, 1, 0, 0);
}

int error(lua_State *Ls) {
	string args = lua_tolstring(Ls, -2);
	getglobal(L, "error");
	pushstring(args.c_str());
	pcall(L, 1, 0, 0);
}

int pi(lua_State *Ls) {
	string args = lua_tolstring(Ls, -2);
	getglobal(L, "printidentity");
	pcall(L, 0, 0, 0);
}

int newinstance(lua_State *Ls) {
	string args = lua_tolstring(Ls, -2);
	getglobal(L, "Instance");
	getfield(L, -1, "new");
	pushstring(args.c_str());
	pcall(L, 4, 1, 0);
	getglobal(L, "workspace");
	pushvalue(L, -2);
	setfield(L, -2, "Parent");
}
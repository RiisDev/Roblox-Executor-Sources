#pragma once
#include "Main.h"
#include "Addresses.h"
#include "CustomFunctions.h"
#include "Wrapper.h"



int Vector3new(lua_State *L) {
	int args = 0;
	std::cout << "getglobal Vector3\n";
	rlua_getglobal(rL, "Vector3");
	std::cout << "getfield -1 new\n";
	rlua_getfield(rL, -1, "new");
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		unwrap(L, rL, x);
		args += 1;
	}
	std::cout << "call " << args << " 1\n";
	rlua_pcall(rL, args, 1, 0);
	return 1;
}

int CFramenew(lua_State *L) {
	int args = 0;
	std::cout << "getglobal CFrame\n";
	rlua_getglobal(rL, "CFrame");
	std::cout << "getfield -1 new\n";
	rlua_getfield(rL, -1, "new");
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		unwrap(L, rL, x);
		args += 1;
	}
	std::cout << "call " << args << " 1\n";
	rlua_pcall(rL, args, 1, 0);
	return 1;
}

int BrickColorNew(lua_State *L) {
	int args = 0;
	std::cout << "getglobal BrickColor\n";
	rlua_getglobal(rL, "BrickColor");
	std::cout << "getfield -1 new\n";
	rlua_getfield(rL, -1, "new");
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		unwrap(L, rL, x);
		args += 1;
	}
	rlua_pcall(rL, args, 1, 0);
	return 1;
}

int BrickColorPalette(lua_State *L) {
	int args = 0;
	std::cout << "getglobal BrickColor\n";
	rlua_getglobal(rL, "BrickColor");
	std::cout << "getfield -1 palette\n";
	rlua_getfield(rL, -1, "palette");
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		unwrap(L, rL, x);
		args += 1;
	}
	rlua_pcall(rL, args, 1, 0);
	return 1;
}

int BrickColorRandom(lua_State *L) {
	int args = 0;
	std::cout << "getglobal BrickColor\n";
	rlua_getglobal(rL, "BrickColor");
	std::cout << "getfield -1 random\n";
	rlua_getfield(rL, -1, "random");
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		unwrap(L, rL, x);
		args += 1;
	}
	rlua_pcall(rL, args, 1, 0);
	return 1;
}

int BrickColorWhite(lua_State *L) {
	int args = 0;
	std::cout << "getglobal BrickColor\n";
	rlua_getglobal(rL, "BrickColor");
	std::cout << "getfield -1 White\n";
	rlua_getfield(rL, -1, "White");
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		unwrap(L, rL, x);
		args += 1;
	}
	rlua_pcall(rL, args, 1, 0);
	return 1;
}

int BrickColorGray(lua_State *L) {
	int args = 0;
	std::cout << "getglobal BrickColor\n";
	rlua_getglobal(rL, "BrickColor");
	std::cout << "getfield -1 Gray\n";
	rlua_getfield(rL, -1, "Gray");
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		unwrap(L, rL, x);
		args += 1;
	}
	rlua_pcall(rL, args, 1, 0);
	return 1;
}

int BrickColorDarkGray(lua_State *L) {
	int args = 0;
	std::cout << "getglobal BrickColor\n";
	rlua_getglobal(rL, "BrickColor");
	std::cout << "getfield -1 DarkGray\n";
	rlua_getfield(rL, -1, "DarkGray");
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		unwrap(L, rL, x);
		args += 1;
	}
	rlua_pcall(rL, args, 1, 0);
	return 1;
}

int BrickColorBlack(lua_State *L) {
	int args = 0;
	std::cout << "getglobal BrickColor\n";
	rlua_getglobal(rL, "BrickColor");
	std::cout << "getfield -1 Black\n";
	rlua_getfield(rL, -1, "Black");
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		unwrap(L, rL, x);
		args += 1;
	}
	rlua_pcall(rL, args, 1, 0);
	return 1;
}

int BrickColorRed(lua_State *L) {
	int args = 0;
	std::cout << "getglobal BrickColor\n";
	rlua_getglobal(rL, "BrickColor");
	std::cout << "getfield -1 Red\n";
	rlua_getfield(rL, -1, "Red");
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		unwrap(L, rL, x);
		args += 1;
	}
	rlua_pcall(rL, args, 1, 0);
	return 1;
}

int BrickColorYellow(lua_State *L) {
	int args = 0;
	std::cout << "getglobal BrickColor\n";
	rlua_getglobal(rL, "BrickColor");
	std::cout << "getfield -1 Yellow\n";
	rlua_getfield(rL, -1, "Yellow");
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		unwrap(L, rL, x);
		args += 1;
	}
	rlua_pcall(rL, args, 1, 0);
	return 1;
}

int BrickColorGreen(lua_State *L) {
	int args = 0;
	std::cout << "getglobal BrickColor\n";
	rlua_getglobal(rL, "BrickColor");
	std::cout << "getfield -1 Green\n";
	rlua_getfield(rL, -1, "Green");
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		unwrap(L, rL, x);
		args += 1;
	}
	rlua_pcall(rL, args, 1, 0);
	return 1;
}

int BrickColorBlue(lua_State *L) {
	int args = 0;
	std::cout << "getglobal BrickColor\n";
	rlua_getglobal(rL, "BrickColor");
	std::cout << "getfield -1 Blue\n";
	rlua_getfield(rL, -1, "Blue");
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		unwrap(L, rL, x);
		args += 1;
	}
	rlua_pcall(rL, args, 1, 0);
	lua_pushcfunction(L, BrickColorBlue);
	return 1;
}

int Vector3FromNormalId(lua_State *L) {
	int args = 0;
	std::cout << "getglobal Vector3\n";
	rlua_getglobal(rL, "Vector3");
	std::cout << "getfield -1 FromNormalId\n";
	rlua_getfield(rL, -1, "FromNormalId");
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		unwrap(L, rL, x);
		args += 1;
	}
	rlua_pcall(rL, args, 1, 0);
	return 1;
}

int Vector3FromAxis(lua_State *L) {
	int args = 0;
	std::cout << "getglobal Vector3\n";
	rlua_getglobal(rL, "Vector3");
	std::cout << "getfield -1 FromAxis\n";
	rlua_getfield(rL, -1, "FromAxis");
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		unwrap(L, rL, x);
		args += 1;
	}
	rlua_pcall(rL, args, 1, 0);
	wrap(rL, L, -1);
	return 1;
}

static const luaL_Reg BrickColor[] = {
	{ "new", BrickColorNew },
	{ "palette", BrickColorPalette },
	{ "random", BrickColorRandom },
	{ "White", BrickColorWhite },
	{ "Gray", BrickColorGray },
	{ "DarkGray", BrickColorDarkGray },
	{ "Black", BrickColorBlack },
	{ "Red", BrickColorRed },
	{ "Yellow", BrickColorYellow },
	{ "Green", BrickColorGreen },
	{ "Blue", BrickColorBlue },
	{ NULL, NULL }
};

static const luaL_Reg Vector3[] = {
	{ "new", Vector3new },
	{ "FromNormalId", Vector3FromNormalId },
	{ "FromAxis", Vector3FromAxis },
	{ NULL, NULL }
};

static const luaL_Reg CFrame[] = {
	{ "new", CFramenew },
	{ NULL, NULL }
};
#pragma once
#include "Definitions.h"
#include "Functions.h"
//Custom functions.

void CreateCustomTable() {
	lua_createtable(L, 0, 0);
}
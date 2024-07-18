#pragma once

#include <Windows.h>

void Loop();
void TeleportedToAnotherGame();

static lua_State* XYZ_State;
inline lua_State *GetGlobalState() { return XYZ_State; }
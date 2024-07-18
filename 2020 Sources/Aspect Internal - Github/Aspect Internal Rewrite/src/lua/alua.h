#pragma once
#include "../sdk/classes.h"
#include "hook_manager.h"
extern class RBXInstance;

class alua_t
{
public:
	int screenh;
	int screenw;
	ViewMatrix_t matrix;
	void initialize(RBXInstance* dm, ViewMatrix_t* matrix, int screenw, int screenh);
	vec2 w2s(vec3 origin, ViewMatrix_t matrix, int screenw, int screenh);
	hookmanager_t* hookmanager = new hookmanager_t();
	sol::state state;
};

extern alua_t alua;
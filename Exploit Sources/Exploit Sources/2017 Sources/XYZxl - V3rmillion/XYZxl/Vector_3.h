/*
* File Name: Vector3.hpp
* Author: Pkamara
* Description: 3D Vector
*/
/*
#pragma once

#include <conio.h>


#pragma comment(lib,"lua5.1.lib")
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

//#include "../lua-5.1/src/lua.hpp"
#include <math.h>
#include <iostream>
#include <string>
#include "TypeDefs.h"

#define lua_vec3_classname "Vector3"

namespace Types
{
	class Vector3
	{
	public:
		Vector3();
		Vector3(double X, double Y, double Z);
		virtual ~Vector3();

		static void RegisterLua(lua_State* L);

		double X;
		double Y;
		double Z;
		double Magnitude;

		Vector3* GetNormalised();

		bool Equals(Vector3* Value);

		Vector3* Clone();

		Vector3* Add(double Value);
		Vector3* Add(Vector3* Value);

		Vector3* Sub(double Value);
		Vector3* Sub(Vector3* Value);

		Vector3* Mul(double Value);
		Vector3* Mul(Vector3* Value);

		Vector3* Div(double Value);
		Vector3* Div(Vector3* Value);

		Vector3* Neg();
		Vector3* Lerp(Vector3* Goal, double Alpha);

		double DOT(Vector3* Value);

		Vector3* Cross(Vector3* Value);
		Vector3* Sirp(Vector3* Value, double Alpha);

		//Rosin::Vector3* ConvertToRbx();

		bool IsClose(Vector3* Value, double epsilon = 1e-6);

		static int lua_index(lua_State* L);
		static int lua_newindex(lua_State* L);

		static int lua_ToString(lua_State* L);
		static int lua_GetUnit(lua_State* L);
		static int lua_GetMagnitude(lua_State* L);
		static int lua_GetX(lua_State* L);
		static int lua_GetY(lua_State* L);
		static int lua_GetZ(lua_State* L);
		static int lua_Lerp(lua_State* L);
		static int lua_Sirp(lua_State* L);
		static int lua_DOT(lua_State* L);
		static int lua_Cross(lua_State* L);
		static int lua_IsClose(lua_State* L);

		static int lua_Unm(lua_State* L);
		static int lua_Add(lua_State* L);
		static int lua_Sub(lua_State* L);
		static int lua_Mul(lua_State* L);
		static int lua_Div(lua_State* L);
		static int lua_Eq(lua_State* L);

		int WrapLua(lua_State* L);
	};

	Vector3* CheckVector3(lua_State* L, int N);
}*/
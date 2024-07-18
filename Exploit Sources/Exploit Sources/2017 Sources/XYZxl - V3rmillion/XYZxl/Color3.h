/*
* File Name: Color3.hpp
* Author: Pkamara
* Description: 3D Vector
*/

#pragma once

#include "lua.hpp"
#include <math.h>
#include <iostream>
#include <string>

#define lua_col3_classname "Color3"

namespace XYZ_Color3 {
	struct Color3 { float x, y, z; };
}

namespace Types
{
	class Color3
	{
	public:
		Color3();
		Color3(double X, double Y, double Z);
		virtual ~Color3();

		static void RegisterLua(lua_State* L);

		double X;
		double Y;
		double Z;
		double Magnitude;

		Color3* GetNormalised();

		bool Equals(Color3* Value);

		Color3* Clone();

		Color3* Add(double Value);
		Color3* Add(Color3* Value);

		Color3* Sub(double Value);
		Color3* Sub(Color3* Value);

		Color3* Mul(double Value);
		Color3* Mul(Color3* Value);

		Color3* Div(double Value);
		Color3* Div(Color3* Value);

		Color3* Neg();
		Color3* Lerp(Color3* Goal, double Alpha);

		double DOT(Color3* Value);

		Color3* Cross(Color3* Value);
		Color3* Sirp(Color3* Value, double Alpha);

		XYZ_Color3::Color3* ConvertToRbx();

		bool IsClose(Color3* Value, double epsilon = 1e-6);

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

	Color3* CheckColor3(lua_State* L, int N);
}
#pragma once

#include "lua.hpp"
#include <math.h>
#include <iostream>
#include <string>

#define lua_vec2_classname "Vector2"

namespace XYZ_Vector2 {
	struct Vector2 { float a, b; };
}

namespace Types
{
	class Vector2
	{
	public:
		Vector2();
		Vector2(double A, double B);
		virtual ~Vector2();

		static void RegisterLua(lua_State* L);

		double A;
		double B;
		double Magnitude;

		Vector2* GetNormalised();

		bool Equals(Vector2* Value);

		Vector2* Clone();

		Vector2* Add(double Value);
		Vector2* Add(Vector2* Value);

		Vector2* Sub(double Value);
		Vector2* Sub(Vector2* Value);

		Vector2* Mul(double Value);
		Vector2* Mul(Vector2* Value);

		Vector2* Div(double Value);
		Vector2* Div(Vector2* Value);

		Vector2* Neg();
		Vector2* Lerp(Vector2* Goal, double Alpha);

		double DOT(Vector2* Value);

		//Vector2* Cross(Vector2* Value);
		Vector2* Sirp(Vector2* Value, double Alpha);

		XYZ_Vector2::Vector2* ConvertToRbx();

		bool IsClose(Vector2* Value, double epsilon = 1e-6);

		static int lua_index(lua_State* L);
		static int lua_newindex(lua_State* L);

		static int lua_ToString(lua_State* L);
		static int lua_GetUnit(lua_State* L);
		static int lua_GetMagnitude(lua_State* L);

		static int lua_GetA(lua_State* L);
		static int lua_GetB(lua_State* L);

		static int lua_Lerp(lua_State* L);
		static int lua_Sirp(lua_State* L);
		static int lua_DOT(lua_State* L);
		//static int lua_Cross(lua_State* L);
		static int lua_IsClose(lua_State* L);

		static int lua_Unm(lua_State* L);
		static int lua_Add(lua_State* L);
		static int lua_Sub(lua_State* L);
		static int lua_Mul(lua_State* L);
		static int lua_Div(lua_State* L);
		static int lua_Eq(lua_State* L);

		int WrapLua(lua_State* L);
	};

	Vector2* CheckVector2(lua_State* L, int N);
}
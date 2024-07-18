/*
* File Name: NumberRange.hpp
* Author: Pkamara
* Description: 3D Vector
*/

#pragma once

#include "lua.hpp"
#include <math.h>
#include <iostream>
#include <string>

#define lua_col3_classname "NumberRange"

namespace XYZ_NumberRange {
	struct NumberRange { float a, b; };
}

namespace Types
{
	class NumberRange
	{
	public:
		NumberRange();
		NumberRange(double A, double B);
		virtual ~NumberRange();

		static void RegisterLua(lua_State* L);

		double A;
		double B;
		double Magnitude;

		NumberRange* GetNormalised();

		bool Equals(NumberRange* Value);

		NumberRange* Clone();

		NumberRange* Add(double Value);
		NumberRange* Add(NumberRange* Value);

		NumberRange* Sub(double Value);
		NumberRange* Sub(NumberRange* Value);

		NumberRange* Mul(double Value);
		NumberRange* Mul(NumberRange* Value);

		NumberRange* Div(double Value);
		NumberRange* Div(NumberRange* Value);

		NumberRange* Neg();
		NumberRange* Lerp(NumberRange* Goal, double Alpha);

		double DOT(NumberRange* Value);

		//NumberRange* Cross(NumberRange* Value);
		NumberRange* Sirp(NumberRange* Value, double Alpha);

		XYZ_NumberRange::NumberRange* ConvertToRbx();

		bool IsClose(NumberRange* Value, double epsilon = 1e-6);

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

	NumberRange* CheckNumberRange(lua_State* L, int N);
}
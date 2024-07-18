/*
* File Name: UDim2.hpp
* Author: Pkamara
* Description: 3D Vector
*/

#pragma once

#include "lua.hpp"
#include <math.h>
#include <iostream>
#include <string>

#define lua_col3_classname "UDim2"

namespace XYZ_UDim2 {
	struct UDim2 { float a, b, c, d; };
}

namespace Types
{
	class UDim2
	{
	public:
		UDim2();
		UDim2(double A, double B, double C, double D);
		virtual ~UDim2();

		static void RegisterLua(lua_State* L);

		double A;
		double B;
		double C;
		double D;
		double Magnitude;

		UDim2* GetNormalised();

		bool Equals(UDim2* Value);

		UDim2* Clone();

		UDim2* Add(double Value);
		UDim2* Add(UDim2* Value);

		UDim2* Sub(double Value);
		UDim2* Sub(UDim2* Value);

		UDim2* Mul(double Value);
		UDim2* Mul(UDim2* Value);

		UDim2* Div(double Value);
		UDim2* Div(UDim2* Value);

		UDim2* Neg();
		UDim2* Lerp(UDim2* Goal, double Alpha);

		double DOT(UDim2* Value);

		//UDim2* Cross(UDim2* Value);
		UDim2* Sirp(UDim2* Value, double Alpha);

		XYZ_UDim2::UDim2* ConvertToRbx();

		bool IsClose(UDim2* Value, double epsilon = 1e-6);

		static int lua_index(lua_State* L);
		static int lua_newindex(lua_State* L);

		static int lua_ToString(lua_State* L);
		static int lua_GetUnit(lua_State* L);
		static int lua_GetMagnitude(lua_State* L);

		static int lua_GetA(lua_State* L);
		static int lua_GetB(lua_State* L);
		static int lua_GetC(lua_State* L);
		static int lua_GetD(lua_State* L);

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

	UDim2* CheckUDim2(lua_State* L, int N);
}
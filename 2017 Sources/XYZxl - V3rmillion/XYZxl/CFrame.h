/*
* File Name: CFrame.hpp
* Author: Pkamara
* Description: 3D Vector
*/

#pragma once

#include "lua.hpp"
#include <math.h>
#include <iostream>
#include <string>

#define lua_col3_classname "CFrame"

namespace XYZ_CFrame {
	struct CFrame { float x, y, z; };
}

namespace Types
{
	class CFrame
	{
	public:
		CFrame();
		CFrame(double X, double Y, double Z);
		virtual ~CFrame();

		static void RegisterLua(lua_State* L);

		double X;
		double Y;
		double Z;
		double Magnitude;

		CFrame* GetNormalised();

		bool Equals(CFrame* Value);

		CFrame* Clone();

		CFrame* Add(double Value);
		CFrame* Add(CFrame* Value);

		CFrame* Sub(double Value);
		CFrame* Sub(CFrame* Value);

		CFrame* Mul(double Value);
		CFrame* Mul(CFrame* Value);

		CFrame* Div(double Value);
		CFrame* Div(CFrame* Value);

		CFrame* Neg();
		CFrame* Lerp(CFrame* Goal, double Alpha);

		double DOT(CFrame* Value);

		CFrame* Cross(CFrame* Value);
		CFrame* Sirp(CFrame* Value, double Alpha);

		XYZ_CFrame::CFrame* ConvertToRbx();

		bool IsClose(CFrame* Value, double epsilon = 1e-6);

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

	CFrame* CheckCFrame(lua_State* L, int N);
}
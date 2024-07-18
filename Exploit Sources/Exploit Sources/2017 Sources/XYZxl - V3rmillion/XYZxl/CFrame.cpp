/*
* File Name: CFrame.hpp
* Author: Pkamara
* Description: 3D Vector
*/

#include "CFrame.h"
#define M_PI 3.14159265358979323846

void _____luaL_setfuncs(lua_State *L, const luaL_Reg *l, int nup) {
	luaL_checkstack(L, nup, "too many upvalues");
	for (; l->name != NULL; l++) {
		int i;
		for (i = 0; i < nup; i++)
			lua_pushvalue(L, -nup);
		lua_pushcclosure(L, l->func, nup);
		lua_setfield(L, -(nup + 2), l->name);
	}
	lua_pop(L, nup);
}

namespace Types
{
	CFrame::CFrame()
	{
		X = 0;
		Y = 0;
		Z = 0;
		Magnitude = 1;
	}

	CFrame::CFrame(double X, double Y, double Z)
	{
		this->X = X;
		this->Y = Y;
		this->Z = Z;
		Magnitude = sqrt(DOT(this));
	}

	CFrame::~CFrame() {}

	void CFrame::RegisterLua(lua_State* L)
	{

		luaL_newmetatable(L, lua_col3_classname);

		lua_pushstring(L, "__metatable");
		lua_pushstring(L, "This metatable is locked");
		lua_rawset(L, -3);

		lua_pushstring(L, "__methods");
		lua_newtable(L);
		luaL_Reg methods[]{
			{ "lerp", lua_Lerp },
			{ "Lerp", lua_Lerp },
			{ "sirp", lua_Sirp },
			{ "Sirp", lua_Sirp },
			{ "dot", lua_DOT },
			{ "Dot", lua_DOT },
			{ "cross", lua_Cross },
			{ "Cross", lua_Cross },
			{ "isClose", lua_IsClose },
			{ "IsClose", lua_IsClose },
			{ NULL, NULL }
		};
		_____luaL_setfuncs(L, methods, 0);
		lua_rawset(L, -3);

		lua_pushstring(L, "__properties");
		lua_newtable(L);
		luaL_Reg props[]{
			{ "unit", lua_GetUnit },
			{ "Unit", lua_GetUnit },
			{ "magnitude", lua_GetMagnitude },
			{ "Magnitude", lua_GetMagnitude },
			{ "x", lua_GetX },
			{ "X", lua_GetX },
			{ "y", lua_GetY },
			{ "Y", lua_GetY },
			{ "z", lua_GetZ },
			{ "Z", lua_GetZ },
			{ NULL, NULL }
		};
		_____luaL_setfuncs(L, props, 0);
		lua_rawset(L, -3);

		lua_pushstring(L, "__tostring");
		lua_pushcfunction(L, lua_ToString);
		lua_rawset(L, -3);

		lua_pushstring(L, "__unm");
		lua_pushcfunction(L, lua_Unm);
		lua_rawset(L, -3);

		lua_pushstring(L, "__add");
		lua_pushcfunction(L, lua_Add);
		lua_rawset(L, -3);

		lua_pushstring(L, "__sub");
		lua_pushcfunction(L, lua_Sub);
		lua_rawset(L, -3);

		lua_pushstring(L, "__mul");
		lua_pushcfunction(L, lua_Mul);
		lua_rawset(L, -3);

		lua_pushstring(L, "__div");
		lua_pushcfunction(L, lua_Div);
		lua_rawset(L, -3);

		lua_pushstring(L, "__eq");
		lua_pushcfunction(L, lua_Eq);
		lua_rawset(L, -3);

		lua_pushstring(L, "__index");
		lua_pushcfunction(L, lua_index);
		lua_rawset(L, -3);

		lua_pushstring(L, "__newindex");
		lua_pushcfunction(L, lua_newindex);
		lua_rawset(L, -3);

		lua_pop(L, 1);
	}

	CFrame* CFrame::GetNormalised()
	{
		CFrame* NewVector = new CFrame();
		NewVector->X = X / Magnitude;
		NewVector->Y = Y / Magnitude;
		NewVector->Z = Z / Magnitude;

		return NewVector;
	}

	bool CFrame::Equals(CFrame* Value)
	{
		if (Value)
		{
			return (X == Value->X && Y == Value->Y && Value->Z == Z);
		}
		return false;
	}

	CFrame* CFrame::Clone()
	{
		CFrame* NewVector = new CFrame();
		NewVector->X = X;
		NewVector->Y = Y;
		NewVector->Z = Z;
		NewVector->Magnitude = Magnitude;

		return NewVector;
	}

	CFrame* CFrame::Add(double Value)
	{
		return new CFrame(X + Value, Y + Value, Z + Value);
	}

	CFrame* CFrame::Add(CFrame* Value)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return new CFrame(X + Value->X, Y + Value->Y, Z + Value->Z);
	}

	CFrame* CFrame::Sub(double Value)
	{
		return new CFrame(X - Value, Y - Value, Z - Value);
	}

	CFrame* CFrame::Sub(CFrame* Value)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return new CFrame(X - Value->X, Y - Value->Y, Z - Value->Z);
	}

	CFrame* CFrame::Mul(double Value)
	{
		return new CFrame(X * Value, Y * Value, Z * Value);
	}

	CFrame* CFrame::Mul(CFrame* Value)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return new CFrame(X * Value->X, Y * Value->Y, Z * Value->Z);
	}

	CFrame* CFrame::Div(double Value)
	{
		return new CFrame(X / Value, Y / Value, Z / Value);
	}

	CFrame* CFrame::Div(CFrame* Value)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return new CFrame(X / Value->X, Y / Value->Y, Z / Value->Z);
	}

	CFrame* CFrame::Neg()
	{
		return new CFrame(-X, -Y, -Z);
	}

	CFrame* CFrame::Lerp(CFrame* Goal, double Alpha)
	{
		if (Goal == NULL)
		{
			return NULL;
		}
		return Add(Alpha)->Mul(Goal->Sub(this));
	}

	CFrame* CFrame::Sirp(CFrame* Goal, double Alpha)
	{
		if (Goal == NULL)
		{
			return NULL;
		}
		return Add(Goal - this)->Mul(1 - cos(Alpha*M_PI / 2));
	}

	double CFrame::DOT(CFrame* Value)
	{
		if (Value == NULL)
		{
			return 0;
		}
		return X*Value->X + Y*Value->Y + Z*Value->Z;
	}

	CFrame* CFrame::Cross(CFrame* Value)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return new CFrame(Y*Value->Z - Z*Value->Y, X*Value->Z - Value->Z*X, X*Value->Y - Value->Y*X);
	}

	bool CFrame::IsClose(CFrame* Value, double epsilon)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return Sub(Value)->Magnitude <= epsilon;
	}

	XYZ_CFrame::CFrame* CFrame::ConvertToRbx() {
		XYZ_CFrame::CFrame asd{
			X,
			Y,
			Z
		};
		return &asd;
	}

	int CFrame::lua_index(lua_State* L)
	{
		CFrame* LuaVec3 = CheckCFrame(L, 1);
		if (LuaVec3 != NULL)
		{
			const char* Name = luaL_checkstring(L, 2);

			lua_getmetatable(L, 1);
			lua_getfield(L, -1, "__properties");
			lua_getfield(L, -1, Name);
			if (lua_iscfunction(L, -1))
			{
				lua_remove(L, -2);
				lua_remove(L, -2);

				lua_pushvalue(L, 1);
				lua_call(L, 1, 1);
				return 1;
			}
			else
			{
				lua_pop(L, 2);

				lua_getfield(L, -1, "__methods");
				lua_getfield(L, -1, Name);
				if (lua_iscfunction(L, -1))
				{
					lua_remove(L, -2);
					lua_remove(L, -3);

					return 1;
				}
				else
				{
					lua_pop(L, 3);
					return luaL_error(L, "attempt to index '%s' (a nil value)", Name);
				}
			}
		}
		return 0;
	}

	int CFrame::lua_newindex(lua_State* L)
	{
		const char* Name = luaL_checkstring(L, 2);
		return luaL_error(L, "%s cannot be assigned to", Name);
	}

	int CFrame::lua_ToString(lua_State* L)
	{
		CFrame* LuaVec3 = CheckCFrame(L, 1);
		std::string Return = "";
		Return += std::to_string(LuaVec3->X) + ", " + std::to_string(LuaVec3->Y) + ", " + std::to_string(LuaVec3->Z);
		lua_pushstring(L, Return.c_str());
		return 1;
	}

	int CFrame::lua_GetUnit(lua_State* L)
	{
		CFrame* LuaVec3 = CheckCFrame(L, 1);
		return LuaVec3->GetNormalised()->WrapLua(L);
	}

	int CFrame::lua_GetMagnitude(lua_State* L)
	{
		CFrame* LuaVec3 = CheckCFrame(L, 1);
		lua_pushnumber(L, LuaVec3->Magnitude);
		return 1;
	}

	int CFrame::lua_GetX(lua_State* L)
	{
		CFrame* LuaVec3 = CheckCFrame(L, 1);
		lua_pushnumber(L, LuaVec3->X);
		return 1;
	}

	int CFrame::lua_GetY(lua_State* L)
	{
		CFrame* LuaVec3 = CheckCFrame(L, 1);
		lua_pushnumber(L, LuaVec3->Y);
		return 1;
	}

	int CFrame::lua_GetZ(lua_State* L)
	{
		CFrame* LuaVec3 = CheckCFrame(L, 1);
		lua_pushnumber(L, LuaVec3->Z);
		return 1;
	}

	int CFrame::lua_Lerp(lua_State* L)
	{
		CFrame* LuaVec3 = CheckCFrame(L, 1);
		CFrame* GoalVec3 = CheckCFrame(L, 2);
		double alpha = luaL_checknumber(L, 3);
		return LuaVec3->Lerp(GoalVec3, alpha)->WrapLua(L);
	}

	int CFrame::lua_Sirp(lua_State* L)
	{
		CFrame* LuaVec3 = CheckCFrame(L, 1);
		CFrame* GoalVec3 = CheckCFrame(L, 2);
		double alpha = luaL_checknumber(L, 3);
		return LuaVec3->Sirp(GoalVec3, alpha)->WrapLua(L);
	}

	int CFrame::lua_DOT(lua_State* L)
	{
		CFrame* LuaVec3 = CheckCFrame(L, 1);
		CFrame* LuaVec3_2 = CheckCFrame(L, 2);
		lua_pushnumber(L, LuaVec3->DOT(LuaVec3_2));
		return 1;
	}

	int CFrame::lua_Cross(lua_State* L)
	{
		CFrame* LuaVec3 = CheckCFrame(L, 1);
		CFrame* LuaVec3_2 = CheckCFrame(L, 2);
		return LuaVec3->Cross(LuaVec3_2)->WrapLua(L);
	}

	int CFrame::lua_IsClose(lua_State* L)
	{
		CFrame* LuaVec3 = CheckCFrame(L, 1);
		CFrame* LuaVec3_2 = CheckCFrame(L, 2);
		double epsilon = 1e-6;
		if (lua_isnumber(L, 3)) {
			epsilon = lua_tonumber(L, 3);
		}
		lua_pushboolean(L, LuaVec3->IsClose(LuaVec3_2, epsilon));
		return 1;
	}

	int CFrame::lua_Unm(lua_State* L)
	{
		CFrame* LuaVec3 = CheckCFrame(L, 1);
		return LuaVec3->Neg()->WrapLua(L);
	}

	int CFrame::lua_Add(lua_State* L)
	{
		CFrame* LuaVec3 = CheckCFrame(L, 1);
		if (lua_isnumber(L, 2))
		{
			double NumAdd = lua_tonumber(L, 2);
			return LuaVec3->Add(NumAdd)->WrapLua(L);
		}
		else
		{
			CFrame* LuaVec3_2 = CheckCFrame(L, 2);
			return LuaVec3->Add(LuaVec3_2)->WrapLua(L);
		}
		return 0;
	}

	int CFrame::lua_Sub(lua_State* L)
	{
		CFrame* LuaVec3 = CheckCFrame(L, 1);
		if (lua_isnumber(L, 2))
		{
			double NumSub = lua_tonumber(L, 2);
			return LuaVec3->Sub(NumSub)->WrapLua(L);
		}
		else
		{
			CFrame* LuaVec3_2 = CheckCFrame(L, 2);
			return LuaVec3->Sub(LuaVec3_2)->WrapLua(L);
		}
		return 0;
	}

	int CFrame::lua_Div(lua_State* L)
	{
		CFrame* LuaVec3 = CheckCFrame(L, 1);
		if (lua_isnumber(L, 2))
		{
			double NumDiv = lua_tonumber(L, 2);
			return LuaVec3->Div(NumDiv)->WrapLua(L);
		}
		else
		{
			CFrame* LuaVec3_2 = CheckCFrame(L, 2);
			return LuaVec3->Div(LuaVec3_2)->WrapLua(L);
		}
		return 0;
	}

	int CFrame::lua_Mul(lua_State* L)
	{
		CFrame* LuaVec3 = CheckCFrame(L, 1);
		if (lua_isnumber(L, 2))
		{
			double NumMul = lua_tonumber(L, 2);
			return LuaVec3->Mul(NumMul)->WrapLua(L);
		}
		else
		{
			CFrame* LuaVec3_2 = CheckCFrame(L, 2);
			return LuaVec3->Mul(LuaVec3_2)->WrapLua(L);
		}
		return 0;
	}

	int CFrame::lua_Eq(lua_State* L) {
		CFrame* LuaVec3 = CheckCFrame(L, 1);
		if (lua_isuserdata(L, 2)) {
			CFrame* LuaVec3_2 = CheckCFrame(L, 2);
			lua_pushboolean(L, LuaVec3->Equals(LuaVec3_2));
		}
		else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	int CFrame::WrapLua(lua_State* L)
	{
		CFrame** LuaVec3 = (CFrame**)lua_newuserdata(L, sizeof(*this));
		*LuaVec3 = this;

		luaL_getmetatable(L, lua_col3_classname);
		lua_setmetatable(L, -2);
		return 1;
	}

	CFrame* CheckCFrame(lua_State* L, int N)
	{
		return *(CFrame**)luaL_checkudata(L, N, lua_col3_classname);
	}
}
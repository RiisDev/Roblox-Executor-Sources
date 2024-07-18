/*
* File Name: UDim2.hpp
* Author: Pkamara
* Description: 3D Vector
*/

#include "UDim2.h"
#define M_PI 3.14159265358979323846

void __luaL_setfuncs(lua_State *L, const luaL_Reg *l, int nup) {
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
	UDim2::UDim2()
	{
		A = 0;
		B = 0;
		C = 0;
		D = 0;
		Magnitude = 1;
	}

	UDim2::UDim2(double A, double B, double C, double D)
	{
		this->A = A;
		this->B = B;
		this->C = C;
		this->D = D;
		Magnitude = sqrt(DOT(this));
	}

	UDim2::~UDim2() {}

	void UDim2::RegisterLua(lua_State* L)
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
			//{ "cross", lua_Cross },
			//{ "Cross", lua_Cross },
			{ "isClose", lua_IsClose },
			{ "IsClose", lua_IsClose },
			{ NULL, NULL }
		};
		__luaL_setfuncs(L, methods, 0);
		lua_rawset(L, -3);

		lua_pushstring(L, "__properties");
		lua_newtable(L);
		luaL_Reg props[]{
			{ "unit", lua_GetUnit },
			{ "Unit", lua_GetUnit },
			{ "magnitude", lua_GetMagnitude },
			{ "Magnitude", lua_GetMagnitude },
			{ "a", lua_GetA },
			{ "A", lua_GetA },
			{ "b", lua_GetB },
			{ "B", lua_GetB },
			{ "c", lua_GetC },
			{ "C", lua_GetC },
			{ "d", lua_GetD },
			{ "D", lua_GetD },
			{ NULL, NULL }
		};
		__luaL_setfuncs(L, props, 0);
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

	UDim2* UDim2::GetNormalised()
	{
		UDim2* NewVector = new UDim2();
		NewVector->A = A / Magnitude;
		NewVector->B = B / Magnitude;
		NewVector->C = C / Magnitude;
		NewVector->D = D / Magnitude;

		return NewVector;
	}

	bool UDim2::Equals(UDim2* Value)
	{
		if (Value)
		{
			return (A == Value->A && B == Value->B && Value->C == C && Value->D == D);
		}
		return false;
	}

	UDim2* UDim2::Clone()
	{
		UDim2* NewVector = new UDim2();
		NewVector->A = A;
		NewVector->B = B;
		NewVector->C = C;
		NewVector->D = D;
		NewVector->Magnitude = Magnitude;

		return NewVector;
	}

	UDim2* UDim2::Add(double Value)
	{
		return new UDim2(A + Value, B + Value, C + Value, D + Value);
	}

	UDim2* UDim2::Add(UDim2* Value)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return new UDim2(A + Value->A, B + Value->B, C + Value->C, D + Value->D);
	}

	UDim2* UDim2::Sub(double Value)
	{
		return new UDim2(A - Value, B - Value, C - Value, D - Value);
	}

	UDim2* UDim2::Sub(UDim2* Value)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return new UDim2(A - Value->A, B - Value->B, C - Value->C, D - Value->D);
	}

	UDim2* UDim2::Mul(double Value)
	{
		return new UDim2(A * Value, B * Value, C * Value, D * Value);
	}

	UDim2* UDim2::Mul(UDim2* Value)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return new UDim2(A * Value->A, B * Value->B, C * Value->C, D * Value->D);
	}

	UDim2* UDim2::Div(double Value)
	{
		return new UDim2(A / Value, B / Value, C / Value, D / Value);
	}

	UDim2* UDim2::Div(UDim2* Value)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return new UDim2(A / Value->A, B / Value->B, C / Value->C, D / Value->D);
	}

	UDim2* UDim2::Neg()
	{
		return new UDim2(-A, -B, -C, -D);
	}

	UDim2* UDim2::Lerp(UDim2* Goal, double Alpha)
	{
		if (Goal == NULL)
		{
			return NULL;
		}
		return Add(Alpha)->Mul(Goal->Sub(this));
	}

	UDim2* UDim2::Sirp(UDim2* Goal, double Alpha)
	{
		if (Goal == NULL)
		{
			return NULL;
		}
		return Add(Goal - this)->Mul(1 - cos(Alpha*M_PI / 2));
	}

	double UDim2::DOT(UDim2* Value)
	{
		if (Value == NULL)
		{
			return 0;
		}
		return A*Value->A + B*Value->B + C*Value->C + D*Value->D;
	}

	/*UDim2* UDim2::Cross(UDim2* Value)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return new UDim2(Y*Value->Z - Z*Value->Y, X*Value->Z - Value->Z*X, X*Value->Y - Value->Y*X);
	}*/

	bool UDim2::IsClose(UDim2* Value, double epsilon)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return Sub(Value)->Magnitude <= epsilon;
	}

	XYZ_UDim2::UDim2* UDim2::ConvertToRbx() {
		XYZ_UDim2::UDim2 asd {
			A,
			B,
			C,
			D
		};
		return &asd;
	}

	int UDim2::lua_index(lua_State* L)
	{
		UDim2* LuaVec3 = CheckUDim2(L, 1);
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

	int UDim2::lua_newindex(lua_State* L)
	{
		const char* Name = luaL_checkstring(L, 2);
		return luaL_error(L, "%s cannot be assigned to", Name);
	}

	int UDim2::lua_ToString(lua_State* L)
	{
		UDim2* LuaVec3 = CheckUDim2(L, 1);
		std::string Return = "";
		Return += std::to_string(LuaVec3->A) + ", " + std::to_string(LuaVec3->B) + ", " + std::to_string(LuaVec3->C) + ", " + std::to_string(LuaVec3->D);
		lua_pushstring(L, Return.c_str());
		return 1;
	}

	int UDim2::lua_GetUnit(lua_State* L)
	{
		UDim2* LuaVec3 = CheckUDim2(L, 1);
		return LuaVec3->GetNormalised()->WrapLua(L);
	}

	int UDim2::lua_GetMagnitude(lua_State* L)
	{
		UDim2* LuaVec3 = CheckUDim2(L, 1);
		lua_pushnumber(L, LuaVec3->Magnitude);
		return 1;
	}

	int UDim2::lua_GetA(lua_State* L)
	{
		UDim2* LuaVec3 = CheckUDim2(L, 1);
		lua_pushnumber(L, LuaVec3->A);
		return 1;
	}

	int UDim2::lua_GetB(lua_State* L)
	{
		UDim2* LuaVec3 = CheckUDim2(L, 1);
		lua_pushnumber(L, LuaVec3->B);
		return 1;
	}

	int UDim2::lua_GetC(lua_State* L)
	{
		UDim2* LuaVec3 = CheckUDim2(L, 1);
		lua_pushnumber(L, LuaVec3->C);
		return 1;
	}

	int UDim2::lua_GetD(lua_State* L)
	{
		UDim2* LuaVec3 = CheckUDim2(L, 1);
		lua_pushnumber(L, LuaVec3->D);
		return 1;
	}

	int UDim2::lua_Lerp(lua_State* L)
	{
		UDim2* LuaVec3 = CheckUDim2(L, 1);
		UDim2* GoalVec3 = CheckUDim2(L, 2);
		double alpha = luaL_checknumber(L, 3);
		return LuaVec3->Lerp(GoalVec3, alpha)->WrapLua(L);
	}

	int UDim2::lua_Sirp(lua_State* L)
	{
		UDim2* LuaVec3 = CheckUDim2(L, 1);
		UDim2* GoalVec3 = CheckUDim2(L, 2);
		double alpha = luaL_checknumber(L, 3);
		return LuaVec3->Sirp(GoalVec3, alpha)->WrapLua(L);
	}

	int UDim2::lua_DOT(lua_State* L)
	{
		UDim2* LuaVec3 = CheckUDim2(L, 1);
		UDim2* LuaVec3_2 = CheckUDim2(L, 2);
		lua_pushnumber(L, LuaVec3->DOT(LuaVec3_2));
		return 1;
	}

	/*int UDim2::lua_Cross(lua_State* L)
	{
		UDim2* LuaVec3 = CheckUDim2(L, 1);
		UDim2* LuaVec3_2 = CheckUDim2(L, 2);
		return LuaVec3->Cross(LuaVec3_2)->WrapLua(L);
	}*/

	int UDim2::lua_IsClose(lua_State* L)
	{
		UDim2* LuaVec3 = CheckUDim2(L, 1);
		UDim2* LuaVec3_2 = CheckUDim2(L, 2);
		double epsilon = 1e-6;
		if (lua_isnumber(L, 3)) {
			epsilon = lua_tonumber(L, 3);
		}
		lua_pushboolean(L, LuaVec3->IsClose(LuaVec3_2, epsilon));
		return 1;
	}

	int UDim2::lua_Unm(lua_State* L)
	{
		UDim2* LuaVec3 = CheckUDim2(L, 1);
		return LuaVec3->Neg()->WrapLua(L);
	}

	int UDim2::lua_Add(lua_State* L)
	{
		UDim2* LuaVec3 = CheckUDim2(L, 1);
		if (lua_isnumber(L, 2))
		{
			double NumAdd = lua_tonumber(L, 2);
			return LuaVec3->Add(NumAdd)->WrapLua(L);
		}
		else
		{
			UDim2* LuaVec3_2 = CheckUDim2(L, 2);
			return LuaVec3->Add(LuaVec3_2)->WrapLua(L);
		}
		return 0;
	}

	int UDim2::lua_Sub(lua_State* L)
	{
		UDim2* LuaVec3 = CheckUDim2(L, 1);
		if (lua_isnumber(L, 2))
		{
			double NumSub = lua_tonumber(L, 2);
			return LuaVec3->Sub(NumSub)->WrapLua(L);
		}
		else
		{
			UDim2* LuaVec3_2 = CheckUDim2(L, 2);
			return LuaVec3->Sub(LuaVec3_2)->WrapLua(L);
		}
		return 0;
	}

	int UDim2::lua_Div(lua_State* L)
	{
		UDim2* LuaVec3 = CheckUDim2(L, 1);
		if (lua_isnumber(L, 2))
		{
			double NumDiv = lua_tonumber(L, 2);
			return LuaVec3->Div(NumDiv)->WrapLua(L);
		}
		else
		{
			UDim2* LuaVec3_2 = CheckUDim2(L, 2);
			return LuaVec3->Div(LuaVec3_2)->WrapLua(L);
		}
		return 0;
	}

	int UDim2::lua_Mul(lua_State* L)
	{
		UDim2* LuaVec3 = CheckUDim2(L, 1);
		if (lua_isnumber(L, 2))
		{
			double NumMul = lua_tonumber(L, 2);
			return LuaVec3->Mul(NumMul)->WrapLua(L);
		}
		else
		{
			UDim2* LuaVec3_2 = CheckUDim2(L, 2);
			return LuaVec3->Mul(LuaVec3_2)->WrapLua(L);
		}
		return 0;
	}

	int UDim2::lua_Eq(lua_State* L) {
		UDim2* LuaVec3 = CheckUDim2(L, 1);
		if (lua_isuserdata(L, 2)) {
			UDim2* LuaVec3_2 = CheckUDim2(L, 2);
			lua_pushboolean(L, LuaVec3->Equals(LuaVec3_2));
		}
		else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	int UDim2::WrapLua(lua_State* L)
	{
		UDim2** LuaVec3 = (UDim2**)lua_newuserdata(L, sizeof(*this));
		*LuaVec3 = this;

		luaL_getmetatable(L, lua_col3_classname);
		lua_setmetatable(L, -2);
		return 1;
	}

	UDim2* CheckUDim2(lua_State* L, int N)
	{
		return *(UDim2**)luaL_checkudata(L, N, lua_col3_classname);
	}
}
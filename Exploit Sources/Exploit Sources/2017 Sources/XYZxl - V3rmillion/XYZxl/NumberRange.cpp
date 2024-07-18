/*
* File Name: NumberRange.hpp
* Author: Pkamara
* Description: 3D Vector
*/

#include "NumberRange.h"
#define M_PI 3.14159265358979323846

void ___luaL_setfuncs(lua_State *L, const luaL_Reg *l, int nup) {
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
	NumberRange::NumberRange()
	{
		A = 0;
		B = 0;
		Magnitude = 1;
	}

	NumberRange::NumberRange(double A, double B)
	{
		this->A = A;
		this->B = B;
		Magnitude = sqrt(DOT(this));
	}

	NumberRange::~NumberRange() {}

	void NumberRange::RegisterLua(lua_State* L)
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
		___luaL_setfuncs(L, methods, 0);
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
			{ NULL, NULL }
		};
		___luaL_setfuncs(L, props, 0);
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

	NumberRange* NumberRange::GetNormalised()
	{
		NumberRange* NewVector = new NumberRange();
		NewVector->A = A / Magnitude;
		NewVector->B = B / Magnitude;

		return NewVector;
	}

	bool NumberRange::Equals(NumberRange* Value)
	{
		if (Value)
		{
			return (A == Value->A && B == Value->B);
		}
		return false;
	}

	NumberRange* NumberRange::Clone()
	{
		NumberRange* NewVector = new NumberRange();
		NewVector->A = A;
		NewVector->B = B;
		NewVector->Magnitude = Magnitude;

		return NewVector;
	}

	NumberRange* NumberRange::Add(double Value)
	{
		return new NumberRange(A + Value, B + Value);
	}

	NumberRange* NumberRange::Add(NumberRange* Value)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return new NumberRange(A + Value->A, B + Value->B);
	}

	NumberRange* NumberRange::Sub(double Value)
	{
		return new NumberRange(A - Value, B - Value);
	}

	NumberRange* NumberRange::Sub(NumberRange* Value)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return new NumberRange(A - Value->A, B - Value->B);
	}

	NumberRange* NumberRange::Mul(double Value)
	{
		return new NumberRange(A * Value, B * Value);
	}

	NumberRange* NumberRange::Mul(NumberRange* Value)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return new NumberRange(A * Value->A, B * Value->B);
	}

	NumberRange* NumberRange::Div(double Value)
	{
		return new NumberRange(A / Value, B / Value);
	}

	NumberRange* NumberRange::Div(NumberRange* Value)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return new NumberRange(A / Value->A, B / Value->B);
	}

	NumberRange* NumberRange::Neg()
	{
		return new NumberRange(-A, -B);
	}

	NumberRange* NumberRange::Lerp(NumberRange* Goal, double Alpha)
	{
		if (Goal == NULL)
		{
			return NULL;
		}
		return Add(Alpha)->Mul(Goal->Sub(this));
	}

	NumberRange* NumberRange::Sirp(NumberRange* Goal, double Alpha)
	{
		if (Goal == NULL)
		{
			return NULL;
		}
		return Add(Goal - this)->Mul(1 - cos(Alpha*M_PI / 2));
	}

	double NumberRange::DOT(NumberRange* Value)
	{
		if (Value == NULL)
		{
			return 0;
		}
		return A*Value->A + B*Value->B;
	}

	/*NumberRange* NumberRange::Cross(NumberRange* Value)
	{
	if (Value == NULL)
	{
	return NULL;
	}
	return new NumberRange(Y*Value->Z - Z*Value->Y, X*Value->Z - Value->Z*X, X*Value->Y - Value->Y*X);
	}*/

	bool NumberRange::IsClose(NumberRange* Value, double epsilon)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return Sub(Value)->Magnitude <= epsilon;
	}

	XYZ_NumberRange::NumberRange* NumberRange::ConvertToRbx() {
		XYZ_NumberRange::NumberRange asd{
			A,
			B,
		};
		return &asd;
	}

	int NumberRange::lua_index(lua_State* L)
	{
		NumberRange* LuaVec3 = CheckNumberRange(L, 1);
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

	int NumberRange::lua_newindex(lua_State* L)
	{
		const char* Name = luaL_checkstring(L, 2);
		return luaL_error(L, "%s cannot be assigned to", Name);
	}

	int NumberRange::lua_ToString(lua_State* L)
	{
		NumberRange* LuaVec3 = CheckNumberRange(L, 1);
		std::string Return = "";
		Return += std::to_string(LuaVec3->A) + ", " + std::to_string(LuaVec3->B);
		lua_pushstring(L, Return.c_str());
		return 1;
	}

	int NumberRange::lua_GetUnit(lua_State* L)
	{
		NumberRange* LuaVec3 = CheckNumberRange(L, 1);
		return LuaVec3->GetNormalised()->WrapLua(L);
	}

	int NumberRange::lua_GetMagnitude(lua_State* L)
	{
		NumberRange* LuaVec3 = CheckNumberRange(L, 1);
		lua_pushnumber(L, LuaVec3->Magnitude);
		return 1;
	}

	int NumberRange::lua_GetA(lua_State* L)
	{
		NumberRange* LuaVec3 = CheckNumberRange(L, 1);
		lua_pushnumber(L, LuaVec3->A);
		return 1;
	}

	int NumberRange::lua_GetB(lua_State* L)
	{
		NumberRange* LuaVec3 = CheckNumberRange(L, 1);
		lua_pushnumber(L, LuaVec3->B);
		return 1;
	}

	int NumberRange::lua_Lerp(lua_State* L)
	{
		NumberRange* LuaVec3 = CheckNumberRange(L, 1);
		NumberRange* GoalVec3 = CheckNumberRange(L, 2);
		double alpha = luaL_checknumber(L, 3);
		return LuaVec3->Lerp(GoalVec3, alpha)->WrapLua(L);
	}

	int NumberRange::lua_Sirp(lua_State* L)
	{
		NumberRange* LuaVec3 = CheckNumberRange(L, 1);
		NumberRange* GoalVec3 = CheckNumberRange(L, 2);
		double alpha = luaL_checknumber(L, 3);
		return LuaVec3->Sirp(GoalVec3, alpha)->WrapLua(L);
	}

	int NumberRange::lua_DOT(lua_State* L)
	{
		NumberRange* LuaVec3 = CheckNumberRange(L, 1);
		NumberRange* LuaVec3_2 = CheckNumberRange(L, 2);
		lua_pushnumber(L, LuaVec3->DOT(LuaVec3_2));
		return 1;
	}

	/*int NumberRange::lua_Cross(lua_State* L)
	{
	NumberRange* LuaVec3 = CheckNumberRange(L, 1);
	NumberRange* LuaVec3_2 = CheckNumberRange(L, 2);
	return LuaVec3->Cross(LuaVec3_2)->WrapLua(L);
	}*/

	int NumberRange::lua_IsClose(lua_State* L)
	{
		NumberRange* LuaVec3 = CheckNumberRange(L, 1);
		NumberRange* LuaVec3_2 = CheckNumberRange(L, 2);
		double epsilon = 1e-6;
		if (lua_isnumber(L, 3)) {
			epsilon = lua_tonumber(L, 3);
		}
		lua_pushboolean(L, LuaVec3->IsClose(LuaVec3_2, epsilon));
		return 1;
	}

	int NumberRange::lua_Unm(lua_State* L)
	{
		NumberRange* LuaVec3 = CheckNumberRange(L, 1);
		return LuaVec3->Neg()->WrapLua(L);
	}

	int NumberRange::lua_Add(lua_State* L)
	{
		NumberRange* LuaVec3 = CheckNumberRange(L, 1);
		if (lua_isnumber(L, 2))
		{
			double NumAdd = lua_tonumber(L, 2);
			return LuaVec3->Add(NumAdd)->WrapLua(L);
		}
		else
		{
			NumberRange* LuaVec3_2 = CheckNumberRange(L, 2);
			return LuaVec3->Add(LuaVec3_2)->WrapLua(L);
		}
		return 0;
	}

	int NumberRange::lua_Sub(lua_State* L)
	{
		NumberRange* LuaVec3 = CheckNumberRange(L, 1);
		if (lua_isnumber(L, 2))
		{
			double NumSub = lua_tonumber(L, 2);
			return LuaVec3->Sub(NumSub)->WrapLua(L);
		}
		else
		{
			NumberRange* LuaVec3_2 = CheckNumberRange(L, 2);
			return LuaVec3->Sub(LuaVec3_2)->WrapLua(L);
		}
		return 0;
	}

	int NumberRange::lua_Div(lua_State* L)
	{
		NumberRange* LuaVec3 = CheckNumberRange(L, 1);
		if (lua_isnumber(L, 2))
		{
			double NumDiv = lua_tonumber(L, 2);
			return LuaVec3->Div(NumDiv)->WrapLua(L);
		}
		else
		{
			NumberRange* LuaVec3_2 = CheckNumberRange(L, 2);
			return LuaVec3->Div(LuaVec3_2)->WrapLua(L);
		}
		return 0;
	}

	int NumberRange::lua_Mul(lua_State* L)
	{
		NumberRange* LuaVec3 = CheckNumberRange(L, 1);
		if (lua_isnumber(L, 2))
		{
			double NumMul = lua_tonumber(L, 2);
			return LuaVec3->Mul(NumMul)->WrapLua(L);
		}
		else
		{
			NumberRange* LuaVec3_2 = CheckNumberRange(L, 2);
			return LuaVec3->Mul(LuaVec3_2)->WrapLua(L);
		}
		return 0;
	}

	int NumberRange::lua_Eq(lua_State* L) {
		NumberRange* LuaVec3 = CheckNumberRange(L, 1);
		if (lua_isuserdata(L, 2)) {
			NumberRange* LuaVec3_2 = CheckNumberRange(L, 2);
			lua_pushboolean(L, LuaVec3->Equals(LuaVec3_2));
		}
		else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	int NumberRange::WrapLua(lua_State* L)
	{
		NumberRange** LuaVec3 = (NumberRange**)lua_newuserdata(L, sizeof(*this));
		*LuaVec3 = this;

		luaL_getmetatable(L, lua_col3_classname);
		lua_setmetatable(L, -2);
		return 1;
	}

	NumberRange* CheckNumberRange(lua_State* L, int N)
	{
		return *(NumberRange**)luaL_checkudata(L, N, lua_col3_classname);
	}
}
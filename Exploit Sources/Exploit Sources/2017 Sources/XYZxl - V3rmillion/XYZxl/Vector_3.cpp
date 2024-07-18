/*
* File Name: Vector3.hpp
* Author: Pkamara
* Description: 3D Vector
*/

/*

#include <Windows.h>
#include "Vector3.h"
#include <conio.h>

#define M_PI 3.14159265358979323846

void luaL_setfuncs(lua_State *L, const luaL_Reg *l, int nup) {
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
	Vector3::Vector3()
	{
		X = 0;
		Y = 0;
		Z = 0;
		Magnitude = 1;
	}

	Vector3::Vector3(double X, double Y, double Z)
	{
		this->X = X;
		this->Y = Y;
		this->Z = Z;
		Magnitude = sqrt(DOT(this));
	}

	Vector3::~Vector3() {}

	void Vector3::RegisterLua(lua_State* L)
	{

		luaL_newmetatable(L, lua_vec3_classname);

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
		luaL_setfuncs(L, methods, 0);
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
		luaL_setfuncs(L, props, 0);
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

	Vector3* Vector3::GetNormalised()
	{
		Vector3* NewVector = new Vector3();
		NewVector->X = X / Magnitude;
		NewVector->Y = Y / Magnitude;
		NewVector->Z = Z / Magnitude;

		return NewVector;
	}

	bool Vector3::Equals(Vector3* Value)
	{
		if (Value)
		{
			return (X == Value->X && Y == Value->Y && Value->Z == Z);
		}
		return false;
	}

	Vector3* Vector3::Clone()
	{
		Vector3* NewVector = new Vector3();
		NewVector->X = X;
		NewVector->Y = Y;
		NewVector->Z = Z;
		NewVector->Magnitude = Magnitude;

		return NewVector;
	}

	Vector3* Vector3::Add(double Value)
	{
		return new Vector3(X + Value, Y + Value, Z + Value);
	}

	Vector3* Vector3::Add(Vector3* Value)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return new Vector3(X + Value->X, Y + Value->Y, Z + Value->Z);
	}

	Vector3* Vector3::Sub(double Value)
	{
		return new Vector3(X - Value, Y - Value, Z - Value);
	}

	Vector3* Vector3::Sub(Vector3* Value)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return new Vector3(X - Value->X, Y - Value->Y, Z - Value->Z);
	}

	Vector3* Vector3::Mul(double Value)
	{
		return new Vector3(X * Value, Y * Value, Z * Value);
	}

	Vector3* Vector3::Mul(Vector3* Value)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return new Vector3(X * Value->X, Y * Value->Y, Z * Value->Z);
	}

	Vector3* Vector3::Div(double Value)
	{
		return new Vector3(X / Value, Y / Value, Z / Value);
	}

	Vector3* Vector3::Div(Vector3* Value)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return new Vector3(X / Value->X, Y / Value->Y, Z / Value->Z);
	}

	Vector3* Vector3::Neg()
	{
		return new Vector3(-X, -Y, -Z);
	}

	Vector3* Vector3::Lerp(Vector3* Goal, double Alpha)
	{
		if (Goal == NULL)
		{
			return NULL;
		}
		return Add(Alpha)->Mul(Goal->Sub(this));
	}

	Vector3* Vector3::Sirp(Vector3* Goal, double Alpha)
	{
		if (Goal == NULL)
		{
			return NULL;
		}
		return Add(Goal - this)->Mul(1 - cos(Alpha*M_PI / 2));
	}

	double Vector3::DOT(Vector3* Value)
	{
		if (Value == NULL)
		{
			return 0;
		}
		return X*Value->X + Y*Value->Y + Z*Value->Z;
	}

	Vector3* Vector3::Cross(Vector3* Value)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return new Vector3(Y*Value->Z - Z*Value->Y, X*Value->Z - Value->Z*X, X*Value->Y - Value->Y*X);
	}

	bool Vector3::IsClose(Vector3* Value, double epsilon)
	{
		if (Value == NULL)
		{
			return NULL;
		}
		return Sub(Value)->Magnitude <= epsilon;
	}

	Rosin::Vector3* Vector3::ConvertToRbx() {
		Rosin::Vector3 asd{
			X,
			Y,
			Z
		};
		return &asd;
	}

	int Vector3::lua_index(lua_State* L)
	{
		Vector3* LuaVec3 = CheckVector3(L, 1);
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

	int Vector3::lua_newindex(lua_State* L)
	{
		const char* Name = luaL_checkstring(L, 2);
		return luaL_error(L, "%s cannot be assigned to", Name);
	}

	int Vector3::lua_ToString(lua_State* L)
	{
		Vector3* LuaVec3 = CheckVector3(L, 1);
		std::string Return = "";
		Return += std::to_string(LuaVec3->X) + ", " + std::to_string(LuaVec3->Y) + ", " + std::to_string(LuaVec3->Z);
		lua_pushstring(L, Return.c_str());
		return 1;
	}

	int Vector3::lua_GetUnit(lua_State* L)
	{
		Vector3* LuaVec3 = CheckVector3(L, 1);
		return LuaVec3->GetNormalised()->WrapLua(L);
	}

	int Vector3::lua_GetMagnitude(lua_State* L)
	{
		Vector3* LuaVec3 = CheckVector3(L, 1);
		lua_pushnumber(L, LuaVec3->Magnitude);
		return 1;
	}

	int Vector3::lua_GetX(lua_State* L)
	{
		Vector3* LuaVec3 = CheckVector3(L, 1);
		lua_pushnumber(L, LuaVec3->X);
		return 1;
	}

	int Vector3::lua_GetY(lua_State* L)
	{
		Vector3* LuaVec3 = CheckVector3(L, 1);
		lua_pushnumber(L, LuaVec3->Y);
		return 1;
	}

	int Vector3::lua_GetZ(lua_State* L)
	{
		Vector3* LuaVec3 = CheckVector3(L, 1);
		lua_pushnumber(L, LuaVec3->Z);
		return 1;
	}

	int Vector3::lua_Lerp(lua_State* L)
	{
		Vector3* LuaVec3 = CheckVector3(L, 1);
		Vector3* GoalVec3 = CheckVector3(L, 2);
		double alpha = luaL_checknumber(L, 3);
		return LuaVec3->Lerp(GoalVec3, alpha)->WrapLua(L);
	}

	int Vector3::lua_Sirp(lua_State* L)
	{
		Vector3* LuaVec3 = CheckVector3(L, 1);
		Vector3* GoalVec3 = CheckVector3(L, 2);
		double alpha = luaL_checknumber(L, 3);
		return LuaVec3->Sirp(GoalVec3, alpha)->WrapLua(L);
	}

	int Vector3::lua_DOT(lua_State* L)
	{
		Vector3* LuaVec3 = CheckVector3(L, 1);
		Vector3* LuaVec3_2 = CheckVector3(L, 2);
		lua_pushnumber(L, LuaVec3->DOT(LuaVec3_2));
		return 1;
	}

	int Vector3::lua_Cross(lua_State* L)
	{
		Vector3* LuaVec3 = CheckVector3(L, 1);
		Vector3* LuaVec3_2 = CheckVector3(L, 2);
		return LuaVec3->Cross(LuaVec3_2)->WrapLua(L);
	}

	int Vector3::lua_IsClose(lua_State* L)
	{
		Vector3* LuaVec3 = CheckVector3(L, 1);
		Vector3* LuaVec3_2 = CheckVector3(L, 2);
		double epsilon = 1e-6;
		if (lua_isnumber(L, 3)) {
			epsilon = lua_tonumber(L, 3);
		}
		lua_pushboolean(L, LuaVec3->IsClose(LuaVec3_2, epsilon));
		return 1;
	}

	int Vector3::lua_Unm(lua_State* L)
	{
		Vector3* LuaVec3 = CheckVector3(L, 1);
		return LuaVec3->Neg()->WrapLua(L);
	}

	int Vector3::lua_Add(lua_State* L)
	{
		Vector3* LuaVec3 = CheckVector3(L, 1);
		if (lua_isnumber(L, 2))
		{
			double NumAdd = lua_tonumber(L, 2);
			return LuaVec3->Add(NumAdd)->WrapLua(L);
		}
		else
		{
			Vector3* LuaVec3_2 = CheckVector3(L, 2);
			return LuaVec3->Add(LuaVec3_2)->WrapLua(L);
		}
		return 0;
	}

	int Vector3::lua_Sub(lua_State* L)
	{
		Vector3* LuaVec3 = CheckVector3(L, 1);
		if (lua_isnumber(L, 2))
		{
			double NumSub = lua_tonumber(L, 2);
			return LuaVec3->Sub(NumSub)->WrapLua(L);
		}
		else
		{
			Vector3* LuaVec3_2 = CheckVector3(L, 2);
			return LuaVec3->Sub(LuaVec3_2)->WrapLua(L);
		}
		return 0;
	}

	int Vector3::lua_Div(lua_State* L)
	{
		Vector3* LuaVec3 = CheckVector3(L, 1);
		if (lua_isnumber(L, 2))
		{
			double NumDiv = lua_tonumber(L, 2);
			return LuaVec3->Div(NumDiv)->WrapLua(L);
		}
		else
		{
			Vector3* LuaVec3_2 = CheckVector3(L, 2);
			return LuaVec3->Div(LuaVec3_2)->WrapLua(L);
		}
		return 0;
	}

	int Vector3::lua_Mul(lua_State* L)
	{
		Vector3* LuaVec3 = CheckVector3(L, 1);
		if (lua_isnumber(L, 2))
		{
			double NumMul = lua_tonumber(L, 2);
			return LuaVec3->Mul(NumMul)->WrapLua(L);
		}
		else
		{
			Vector3* LuaVec3_2 = CheckVector3(L, 2);
			return LuaVec3->Mul(LuaVec3_2)->WrapLua(L);
		}
		return 0;
	}

	int Vector3::lua_Eq(lua_State* L) {
		Vector3* LuaVec3 = CheckVector3(L, 1);
		if (lua_isuserdata(L, 2)) {
			Vector3* LuaVec3_2 = CheckVector3(L, 2);
			lua_pushboolean(L, LuaVec3->Equals(LuaVec3_2));
		}
		else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	int Vector3::WrapLua(lua_State* L)
	{
		Vector3** LuaVec3 = (Vector3**)lua_newuserdata(L, sizeof(*this));
		*LuaVec3 = this;

		luaL_getmetatable(L, lua_vec3_classname);
		lua_setmetatable(L, -2);
		return 1;
	}

	Vector3* CheckVector3(lua_State* L, int N)
	{
		return *(Vector3**)luaL_checkudata(L, N, lua_vec3_classname);
	}
}

*/
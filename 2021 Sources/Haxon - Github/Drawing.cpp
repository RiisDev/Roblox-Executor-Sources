#include "Drawing.hpp"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

#include <d3d11.h>
#include <D3Dcompiler.h>
#include <cstdint>
#include <vector>
#pragma comment(lib, "D3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")

//stuff for d3d hooking

std::uintptr_t VirtualTable;
using PresentFunctionHook = HRESULT(__stdcall*)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
PresentFunctionHook OldPresent; //the hook for the old present func

LONG WNDProc;
HWND RBXHWND;

//our d3d stuff

ID3D11Device* Drawing_Device;
ID3D11DeviceContext* Drawing_Context;
ID3D11Texture2D* Drawing_Render_Target_Texture;
ID3D11RenderTargetView* Drawing_Render_Target_View;

LONG SW;
LONG SH;

//vectors for drawings

std::vector<DrawingAPI_Shape_Text*> DRAWINGS_TEXT;
std::vector<DrawingAPI_Shape_Line*> DRAWINGS_LINE;
std::vector<DrawingAPI_Shape_Square*> DRAWINGS_SQUARE;
std::vector<DrawingAPI_Shape_Circle*> DRAWINGS_CIRCLE;
std::vector<DrawingAPI_Shape_Triangle*> DRAWINGS_TRIANGLE;


bool Ready = false;

namespace DrawingAPILuaAPI {

	namespace Lua {

		/*

		TEXT

		*/

		namespace Text {

			static int __gc(lua_State* LS) {
				luaL_checktype(LS, 1, LUA_TUSERDATA);
				DrawingAPI_Shape_Text& Object = *static_cast<DrawingAPI_Shape_Text*>(lua_touserdata(LS, 1));
				for (size_t Iteration = 0; Iteration < DRAWINGS_TEXT.size(); Iteration++)
				{
					if (DRAWINGS_TEXT[Iteration] == &Object) {
						DRAWINGS_TEXT.erase(DRAWINGS_TEXT.begin() + Iteration);
					}
				}
				return 0;
			}

			static int __index(lua_State* LS) {
				luaL_checktype(LS, 1, LUA_TUSERDATA);
				DrawingAPI_Shape_Text& Object = *static_cast<DrawingAPI_Shape_Text*>(lua_touserdata(LS, 1));
				const char* Key = luaL_checklstring(LS, 2, 0);
				if (!std::strcmp(Key, "Remove")) {
					lua_pushcfunction(LS, __gc);
					return 1;
				}
				else if (!std::strcmp(Key, "Visible")) {
					lua_pushboolean(LS, Object.IsShapeVisible);
					return 1;
				}
				else if (!std::strcmp(Key, "ZIndex")) {
					lua_pushnumber(LS, Object.ZIndex);
					return 1;
				}
				else if (!std::strcmp(Key, "Text")) {
					lua_pushstring(LS, Object.Text);
					return 1;
				}
				else if (!std::strcmp(Key, "Position")) {
					lua_getglobal(LS, "Vector2");
					lua_getfield(LS, -1, "new");
					lua_pushnumber(LS, Object.Position.Position_X);
					lua_pushnumber(LS, Object.Position.Position_Y);
					lua_call(LS, 2, 1);
					return 1;
				}
				else if (!std::strcmp(Key, "Color")) {
					lua_getglobal(LS, "Color3");
					lua_getfield(LS, -1, "fromRGB");
					lua_pushnumber(LS, Object.Colour.Colour_R);
					lua_pushnumber(LS, Object.Colour.Colour_G);
					lua_pushnumber(LS, Object.Colour.Colour_B);
					lua_call(LS, 3, 1);
					return 1;
				}
				else if (!std::strcmp(Key, "Outline")) {
					lua_pushboolean(LS, Object.Outline);
					return 1;
				}
				else if (!std::strcmp(Key, "Transparency")) {
					lua_pushnumber(LS, Object.Transparency);
					return 1;
				}
				else if (!std::strcmp(Key, "TextBounds")) {
					lua_getglobal(LS, "Vector2");
					lua_getfield(LS, -1, "new");
					lua_pushnumber(LS, Object.TextBounds.Position_X);
					lua_pushnumber(LS, Object.TextBounds.Position_Y);
					lua_call(LS, 2, 1);
					return 1;
				}
				return 0;
			}

			static int __newindex(lua_State* LS) {
				luaL_checktype(LS, 1, LUA_TUSERDATA);
				DrawingAPI_Shape_Text& Object = *static_cast<DrawingAPI_Shape_Text*>(lua_touserdata(LS, 1));
				const char* Key = luaL_checklstring(LS, 2, 0);
				if (!std::strcmp(Key, "Visible")) {
					luaL_checktype(LS, 3, LUA_TBOOLEAN);
					Object.IsShapeVisible = lua_toboolean(LS, 3);
					return 0;
				}
				else if (!std::strcmp(Key, "ZIndex")) {
					Object.ZIndex = luaL_checknumber(LS, 3);
					return 0;
				}
				else if (!std::strcmp(Key, "Text")) {
					unsigned int Length;
					const char* Text = luaL_checklstring(LS, 3, &Length);
					Object.Text = Text;
					Object.TextEnd = Text + Length;
					return 0;
				}
				else if (!std::strcmp(Key, "Position")) {
					luaL_checktype(LS, 3, LUA_TUSERDATA);
					lua_pushvalue(LS, 3);
					lua_getfield(LS, -1, "X");
					Object.Position.Position_X = luaL_checknumber(LS, -1);
					lua_getfield(LS, -2, "Y");
					Object.Position.Position_Y = luaL_checknumber(LS, -1);
					return 0;
				}
				else if (!std::strcmp(Key, "Color")) {
					luaL_checktype(LS, 3, LUA_TUSERDATA);
					lua_pushvalue(LS, 3);
					lua_getfield(LS, -1, "r");
					Object.Colour.Colour_R = luaL_checknumber(LS, -1);
					lua_getfield(LS, -2, "g");
					Object.Colour.Colour_G = luaL_checknumber(LS, -1);
					lua_getfield(LS, -3, "b");
					Object.Colour.Colour_B = luaL_checknumber(LS, -1);
					return 0;
				}
				else if (!std::strcmp(Key, "Center")) {
					luaL_checktype(LS, 3, LUA_TBOOLEAN);
					Object.Center = lua_toboolean(LS, 3);
					return 0;
				}
				else if (!std::strcmp(Key, "Outline")) {
					luaL_checktype(LS, 3, LUA_TBOOLEAN);
					Object.Outline = lua_toboolean(LS, 3);
					return 0;
				}
				else if (!std::strcmp(Key, "Transparency")) {
					Object.Transparency = luaL_checknumber(LS, 3);
					return 0;
				}
				return 0;
			}

		}

		/*

		LINE

		*/

		namespace Line {
			static int __gc(lua_State* LS) {
				luaL_checktype(LS, 1, LUA_TUSERDATA);
				DrawingAPI_Shape_Line& Object = *static_cast<DrawingAPI_Shape_Line*>(lua_touserdata(LS, 1));
				for (size_t Iteration = 0; Iteration < DRAWINGS_LINE.size(); Iteration++)
				{
					if (DRAWINGS_LINE[Iteration] == &Object) {
						DRAWINGS_LINE.erase(DRAWINGS_LINE.begin() + Iteration);
					}
				}
				return 0;
			}

			static int __index(lua_State* LS) {
				luaL_checktype(LS, 1, LUA_TUSERDATA);
				DrawingAPI_Shape_Line& Object = *static_cast<DrawingAPI_Shape_Line*>(lua_touserdata(LS, 1));
				const char* Key = luaL_checklstring(LS, 2, 0);
				if (!std::strcmp(Key, "Remove")) {
					lua_pushcfunction(LS, __gc);
					return 1;
				}
				else if (!std::strcmp(Key, "Visible")) {
					lua_pushboolean(LS, Object.IsShapeVisible);
					return 1;
				}
				else if (!std::strcmp(Key, "ZIndex")) {
					lua_pushnumber(LS, Object.ZIndex);
					return 1;
				}
				else if (!std::strcmp(Key, "From")) {
					lua_getglobal(LS, "Vector2");
					lua_getfield(LS, -1, "new");
					lua_pushnumber(LS, Object.From.Position_X);
					lua_pushnumber(LS, Object.From.Position_Y);
					lua_call(LS, 2, 1);
					return 1;
				}
				else if (!std::strcmp(Key, "To")) {
					lua_getglobal(LS, "Vector2");
					lua_getfield(LS, -1, "new");
					lua_pushnumber(LS, Object.To.Position_X);
					lua_pushnumber(LS, Object.To.Position_Y);
					lua_call(LS, 2, 1);
					return 1;
				}
				else if (!std::strcmp(Key, "Color")) {
					lua_getglobal(LS, "Color3");
					lua_getfield(LS, -1, "fromRGB");
					lua_pushnumber(LS, Object.Colour.Colour_R);
					lua_pushnumber(LS, Object.Colour.Colour_G);
					lua_pushnumber(LS, Object.Colour.Colour_B);
					lua_call(LS, 3, 1);
					return 1;
				}
				else if (!std::strcmp(Key, "Thickness")) {
					lua_pushnumber(LS, Object.Thickness);
					return 1;
				}
				else if (!std::strcmp(Key, "Transparency")) {
					lua_pushnumber(LS, Object.Transparency);
					return 1;
				}
			}

			static int __newindex(lua_State* LS) {
				luaL_checktype(LS, 1, LUA_TUSERDATA);
				DrawingAPI_Shape_Line& Object = *static_cast<DrawingAPI_Shape_Line*>(lua_touserdata(LS, 1));
				const char* Key = luaL_checklstring(LS, 2, 0);
				if (!std::strcmp(Key, "Visible")) {
					luaL_checktype(LS, 3, LUA_TBOOLEAN);
					Object.IsShapeVisible = lua_toboolean(LS, 3);
					return 0;
				}
				if (!std::strcmp(Key, "Visible")) {
					Object.IsShapeVisible = lua_tonumber(LS, 3);
					return 0;
				}
				if (!std::strcmp(Key, "From")) {
					luaL_checktype(LS, 3, LUA_TUSERDATA);
					lua_pushvalue(LS, 3);
					lua_getfield(LS, -1, "X");
					Object.From.Position_X = luaL_checknumber(LS, -1);
					lua_getfield(LS, -2, "Y");
					Object.From.Position_Y = luaL_checknumber(LS, -1);
					return 0;
				}
				if (!std::strcmp(Key, "To")) {
					luaL_checktype(LS, 3, LUA_TUSERDATA);
					lua_pushvalue(LS, 3);
					lua_getfield(LS, -1, "X");
					Object.To.Position_X = luaL_checknumber(LS, -1);
					lua_getfield(LS, -2, "Y");
					Object.To.Position_Y = luaL_checknumber(LS, -1);
					return 0;
				}
				else if (!std::strcmp(Key, "Color")) {
					luaL_checktype(LS, 3, LUA_TUSERDATA);
					lua_pushvalue(LS, 3);
					lua_getfield(LS, -1, "r");
					Object.Colour.Colour_R = luaL_checknumber(LS, -1);
					lua_getfield(LS, -2, "g");
					Object.Colour.Colour_G = luaL_checknumber(LS, -1);
					lua_getfield(LS, -3, "b");
					Object.Colour.Colour_B = luaL_checknumber(LS, -1);
					return 0;
				}
				else if (!std::strcmp(Key, "Thickness")) {
					Object.Thickness = luaL_checknumber(LS, 3);
					return 0;
				}
				else if (!std::strcmp(Key, "Transparency")) {
					Object.Transparency = luaL_checknumber(LS, 3);
					return 0;
				}
			}
		}

		/*

		SQUARE

		*/

		namespace Square {

			static int __gc(lua_State* LS) {
				luaL_checktype(LS, 1, LUA_TUSERDATA);
				DrawingAPI_Shape_Square& Object = *static_cast<DrawingAPI_Shape_Square*>(lua_touserdata(LS, 1));
				for (size_t Iteration = 0; Iteration < DRAWINGS_SQUARE.size(); Iteration++)
				{
					if (DRAWINGS_SQUARE[Iteration] == &Object) {
						DRAWINGS_SQUARE.erase(DRAWINGS_SQUARE.begin() + Iteration);
					}
				}
				return 0;
			}

			static int __index(lua_State* LS) {
				luaL_checktype(LS, 1, LUA_TUSERDATA);
				DrawingAPI_Shape_Square& Object = *static_cast<DrawingAPI_Shape_Square*>(lua_touserdata(LS, 1));
				const char* Key = luaL_checklstring(LS, 2, 0);
				if (!std::strcmp(Key, "Remove")) {
					lua_pushcfunction(LS, __gc);
					return 1;
				}
				else if (!std::strcmp(Key, "Visible")) {
					lua_pushboolean(LS, Object.IsShapeVisible);
					return 1;
				}
				else if (!std::strcmp(Key, "ZIndex")) {
					lua_pushnumber(LS, Object.ZIndex);
					return 1;
				}
				else if (!std::strcmp(Key, "Position")) {
					lua_getglobal(LS, "Vector2");
					lua_getfield(LS, -1, "new");
					lua_pushnumber(LS, Object.Position.Position_X);
					lua_pushnumber(LS, Object.Position.Position_Y);
					lua_call(LS, 2, 1);
					return 1;
				}
				else if (!std::strcmp(Key, "Size")) {
					lua_getglobal(LS, "Vector2");
					lua_getfield(LS, -1, "new");
					lua_pushnumber(LS, Object.Size.Position_X);
					lua_pushnumber(LS, Object.Size.Position_Y);
					lua_call(LS, 2, 1);
					return 1;
				}
				else if (!std::strcmp(Key, "Color")) {
					lua_getglobal(LS, "Color3");
					lua_getfield(LS, -1, "fromRGB");
					lua_pushnumber(LS, Object.Colour.Colour_R);
					lua_pushnumber(LS, Object.Colour.Colour_G);
					lua_pushnumber(LS, Object.Colour.Colour_B);
					lua_call(LS, 3, 1);
					return 1;
				}
				else if (!std::strcmp(Key, "Thickness")) {
					lua_pushnumber(LS, Object.Thickness);
					return 1;
				}
				else if (!std::strcmp(Key, "Filled")) {
					lua_pushboolean(LS, Object.Filled);
					return 1;
				}
				else if (!std::strcmp(Key, "Transparency")) {
					lua_pushnumber(LS, Object.Transparency);
					return 1;
				}
			}

			static int __newindex(lua_State* LS) {
				luaL_checktype(LS, 1, LUA_TUSERDATA);
				DrawingAPI_Shape_Square& Object = *static_cast<DrawingAPI_Shape_Square*>(lua_touserdata(LS, 1));
				const char* Key = luaL_checklstring(LS, 2, 0);
				if (!std::strcmp(Key, "Visible")) {
					luaL_checktype(LS, 3, LUA_TBOOLEAN);
					Object.IsShapeVisible = lua_toboolean(LS, 3);
				}
				else if (!std::strcmp(Key, "ZIndex")) {
					Object.ZIndex = luaL_checknumber(LS, 3);
				}
				else if (!std::strcmp(Key, "Position")) {
					luaL_checktype(LS, 3, LUA_TUSERDATA);
					lua_pushvalue(LS, 3);
					lua_getfield(LS, -1, "X");
					Object.Position.Position_X = luaL_checknumber(LS, -1);
					lua_getfield(LS, -2, "Y");
					Object.Position.Position_Y = luaL_checknumber(LS, -1);
					return 0;
				}
				else if (!std::strcmp(Key, "Size")) {
					luaL_checktype(LS, 3, LUA_TUSERDATA);
					lua_pushvalue(LS, 3);
					lua_getfield(LS, -1, "X");
					Object.Size.Position_X = luaL_checknumber(LS, -1);
					lua_getfield(LS, -2, "Y");
					Object.Size.Position_Y = luaL_checknumber(LS, -1);
					return 0;
				}
				else if (!std::strcmp(Key, "Color")) {
					luaL_checktype(LS, 3, LUA_TUSERDATA);
					lua_pushvalue(LS, 3);
					lua_getfield(LS, -1, "r");
					Object.Colour.Colour_R = luaL_checknumber(LS, -1);
					lua_getfield(LS, -2, "g");
					Object.Colour.Colour_G = luaL_checknumber(LS, -1);
					lua_getfield(LS, -3, "b");
					Object.Colour.Colour_B = luaL_checknumber(LS, -1);
					return 0;
				}
				else if (!std::strcmp(Key, "Thickness")) {
					Object.Thickness = luaL_checknumber(LS, 3);
				}
				else if (!std::strcmp(Key, "Filled")) {
					luaL_checktype(LS, 3, LUA_TBOOLEAN);
					Object.Filled = lua_toboolean(LS, 3);
				}
				else if (!std::strcmp(Key, "Transparency")) {
					Object.Transparency = luaL_checknumber(LS, 3);
				}
				return 0;
			}
		}

		/*

		CIRCLE

		*/

		namespace Circle {

			static int __gc(lua_State* LS) {
				luaL_checktype(LS, 1, LUA_TUSERDATA);
				DrawingAPI_Shape_Circle& Object = *static_cast<DrawingAPI_Shape_Circle*>(lua_touserdata(LS, 1));
				for (size_t Iteration = 0; Iteration < DRAWINGS_CIRCLE.size(); Iteration++)
				{
					if (DRAWINGS_CIRCLE[Iteration] == &Object) {
						DRAWINGS_CIRCLE.erase(DRAWINGS_CIRCLE.begin() + Iteration);
					}
				}
				return 0;
			}

			static int __index(lua_State* LS) {
				luaL_checktype(LS, 1, LUA_TUSERDATA);
				DrawingAPI_Shape_Circle& Object = *static_cast<DrawingAPI_Shape_Circle*>(lua_touserdata(LS, 1));
				const char* Key = luaL_checklstring(LS, 2, 0);
				if (!std::strcmp(Key, "Remove")) {
					lua_pushcfunction(LS, __gc);
					return 1;
				}
				else if (!std::strcmp(Key, "Visible")) {
					lua_pushboolean(LS, Object.IsShapeVisible);
					return 1;
				}
				else if (!std::strcmp(Key, "ZIndex")) {
					lua_pushnumber(LS, Object.ZIndex);
					return 1;
				}
				else if (!std::strcmp(Key, "Position")) {
					lua_getglobal(LS, "Vector2");
					lua_getfield(LS, -1, "new");
					lua_pushnumber(LS, Object.Position.Position_X);
					lua_pushnumber(LS, Object.Position.Position_Y);
					lua_call(LS, 2, 1);
					return 1;
				}
				else if (!std::strcmp(Key, "Radius")) {
					lua_pushnumber(LS, Object.Radius);
					return 1;
				}
				else if (!std::strcmp(Key, "Color")) {
					lua_getglobal(LS, "Color3");
					lua_getfield(LS, -1, "fromRGB");
					lua_pushnumber(LS, Object.Colour.Colour_R);
					lua_pushnumber(LS, Object.Colour.Colour_G);
					lua_pushnumber(LS, Object.Colour.Colour_B);
					lua_call(LS, 3, 1);
					return 1;
				}
				else if (!std::strcmp(Key, "Thickness")) {
					lua_pushnumber(LS, Object.Thickness);
					return 1;
				}
				else if (!std::strcmp(Key, "Filled")) {
					lua_pushboolean(LS, Object.Filled);
					return 1;
				}
				else if (!std::strcmp(Key, "Transparency")) {
					lua_pushnumber(LS, Object.Transparency);
					return 1;
				}
				else if (!std::strcmp(Key, "NumSides")) {
					lua_pushnumber(LS, Object.NumberOfSides);
					return 1;
				}
				return 0;
			}

			static int __newindex(lua_State* LS) {
				luaL_checktype(LS, 1, LUA_TUSERDATA);
				DrawingAPI_Shape_Circle& Object = *static_cast<DrawingAPI_Shape_Circle*>(lua_touserdata(LS, 1));
				const char* Key = luaL_checklstring(LS, 2, 0);
				if (!std::strcmp(Key, "Visible")) {
					luaL_checktype(LS, 3, LUA_TBOOLEAN);
					Object.IsShapeVisible = lua_toboolean(LS, 3);
				}
				else if (!std::strcmp(Key, "ZIndex")) {
					Object.ZIndex = luaL_checknumber(LS, 3);
				}
				else if (!std::strcmp(Key, "Position")) {
					luaL_checktype(LS, 3, LUA_TUSERDATA);
					lua_pushvalue(LS, 3);
					lua_getfield(LS, -1, "X");
					Object.Position.Position_X = luaL_checknumber(LS, -1);
					lua_getfield(LS, -2, "Y");
					Object.Position.Position_Y = luaL_checknumber(LS, -1);
					return 0;
				}
				else if (!std::strcmp(Key, "Radius")) {
					Object.Radius = luaL_checknumber(LS, 3);
				}
				else if (!std::strcmp(Key, "Color")) {
					luaL_checktype(LS, 3, LUA_TUSERDATA);
					lua_pushvalue(LS, 3);
					lua_getfield(LS, -1, "r");
					Object.Colour.Colour_R = luaL_checknumber(LS, -1);
					lua_getfield(LS, -2, "g");
					Object.Colour.Colour_G = luaL_checknumber(LS, -1);
					lua_getfield(LS, -3, "b");
					Object.Colour.Colour_B = luaL_checknumber(LS, -1);
					return 0;
				}
				else if (!std::strcmp(Key, "Thickness")) {
					Object.Thickness = luaL_checknumber(LS, 3);
				}
				else if (!std::strcmp(Key, "Filled")) {
					luaL_checktype(LS, 3, LUA_TBOOLEAN);
					Object.Filled = lua_toboolean(LS, 3);
				}
				else if (!std::strcmp(Key, "Transparency")) {
					Object.Transparency = luaL_checknumber(LS, 3);
				}
				else if (!std::strcmp(Key, "NumSides")) {
					Object.NumberOfSides = luaL_checknumber(LS, 3);
				}
				return 0;
			}
		}

		/*

		TRIANGLE

		*/

		namespace Triangle {

			static int __gc(lua_State* LS) {
				luaL_checktype(LS, 1, LUA_TUSERDATA);
				DrawingAPI_Shape_Triangle& Object = *static_cast<DrawingAPI_Shape_Triangle*>(lua_touserdata(LS, 1));
				for (size_t Iteration = 0; Iteration < DRAWINGS_TRIANGLE.size(); Iteration++)
				{
					if (DRAWINGS_TRIANGLE[Iteration] == &Object) {
						DRAWINGS_TRIANGLE.erase(DRAWINGS_TRIANGLE.begin() + Iteration);
					}
				}
				return 0;
			}

			static int __index(lua_State* LS) {
				luaL_checktype(LS, 1, LUA_TUSERDATA);
				DrawingAPI_Shape_Triangle& Object = *static_cast<DrawingAPI_Shape_Triangle*>(lua_touserdata(LS, 1));
				const char* Key = luaL_checklstring(LS, 2, 0);
				if (!std::strcmp(Key, "Remove")) {
					lua_pushcfunction(LS, __gc);
					return 1;
				}
				else if (!std::strcmp(Key, "Visible")) {
					lua_pushboolean(LS, Object.IsShapeVisible);
					return 1;
				}
				else if (!std::strcmp(Key, "ZIndex")) {
					lua_pushnumber(LS, Object.ZIndex);
					return 1;
				}
				else if (!std::strcmp(Key, "PointA")) {
					lua_getglobal(LS, "Vector2");
					lua_getfield(LS, -1, "new");
					lua_pushnumber(LS, Object.Point1.Position_X);
					lua_pushnumber(LS, Object.Point1.Position_Y);
					lua_call(LS, 2, 1);
					return 1;
				}
				else if (!std::strcmp(Key, "PointB")) {
					lua_getglobal(LS, "Vector2");
					lua_getfield(LS, -1, "new");
					lua_pushnumber(LS, Object.Point2.Position_X);
					lua_pushnumber(LS, Object.Point2.Position_Y);
					lua_call(LS, 2, 1);
					return 1;
				}
				else if (!std::strcmp(Key, "PointC")) {
					lua_getglobal(LS, "Vector2");
					lua_getfield(LS, -1, "new");
					lua_pushnumber(LS, Object.Point3.Position_X);
					lua_pushnumber(LS, Object.Point3.Position_Y);
					lua_call(LS, 2, 1);
					return 1;
				}
				else if (!std::strcmp(Key, "Color")) {
					lua_getglobal(LS, "Color3");
					lua_getfield(LS, -1, "fromRGB");
					lua_pushnumber(LS, Object.Colour.Colour_R);
					lua_pushnumber(LS, Object.Colour.Colour_G);
					lua_pushnumber(LS, Object.Colour.Colour_B);
					lua_call(LS, 3, 1);
					return 1;
				}
				else if (!std::strcmp(Key, "Thickness")) {
					lua_pushnumber(LS, Object.Thickness);
					return 1;
				}
				else if (!std::strcmp(Key, "Filled")) {
					lua_pushboolean(LS, Object.Filled);
					return 1;
				}
				else if (!std::strcmp(Key, "Transparency")) {
					lua_pushnumber(LS, Object.Transparency);
					return 1;
				}
			}

			static int __newindex(lua_State* LS) {
				luaL_checktype(LS, 1, LUA_TUSERDATA);
				DrawingAPI_Shape_Triangle& Object = *static_cast<DrawingAPI_Shape_Triangle*>(lua_touserdata(LS, 1));
				const char* Key = luaL_checklstring(LS, 2, 0);
				if (!std::strcmp(Key, "Visible")) {
					luaL_checktype(LS, 3, LUA_TBOOLEAN);
					Object.IsShapeVisible = lua_toboolean(LS, 3);
				}
				else if (!std::strcmp(Key, "ZIndex")) {
					Object.ZIndex = luaL_checknumber(LS, 3);
				}
				else if (!std::strcmp(Key, "PointA")) {
					luaL_checktype(LS, 3, LUA_TUSERDATA);
					lua_pushvalue(LS, 3);
					lua_getfield(LS, -1, "X");
					Object.Point1.Position_X = luaL_checknumber(LS, -1);
					lua_getfield(LS, -2, "Y");
					Object.Point1.Position_Y = luaL_checknumber(LS, -1);
					return 0;
				}
				else if (!std::strcmp(Key, "PointB")) {
					luaL_checktype(LS, 3, LUA_TUSERDATA);
					lua_pushvalue(LS, 3);
					lua_getfield(LS, -1, "X");
					Object.Point2.Position_X = luaL_checknumber(LS, -1);
					lua_getfield(LS, -2, "Y");
					Object.Point2.Position_Y = luaL_checknumber(LS, -1);
					return 0;
				}
				else if (!std::strcmp(Key, "PointC")) {
					luaL_checktype(LS, 3, LUA_TUSERDATA);
					lua_pushvalue(LS, 3);
					lua_getfield(LS, -1, "X");
					Object.Point3.Position_X = luaL_checknumber(LS, -1);
					lua_getfield(LS, -2, "Y");
					Object.Point3.Position_Y = luaL_checknumber(LS, -1);
					return 0;
				}
				else if (!std::strcmp(Key, "Color")) {
					luaL_checktype(LS, 3, LUA_TUSERDATA);
					lua_pushvalue(LS, 3);
					lua_getfield(LS, -1, "r");
					Object.Colour.Colour_R = luaL_checknumber(LS, -1);
					lua_getfield(LS, -2, "g");
					Object.Colour.Colour_G = luaL_checknumber(LS, -1);
					lua_getfield(LS, -3, "b");
					Object.Colour.Colour_B = luaL_checknumber(LS, -1);
					return 0;
				}
				else if (!std::strcmp(Key, "Thickness")) {
					Object.Thickness = luaL_checknumber(LS, 3);
				}
				else if (!std::strcmp(Key, "Filled")) {
					luaL_checktype(LS, 3, LUA_TBOOLEAN);
					Object.Filled = lua_toboolean(LS, 3);
				}
				else if (!std::strcmp(Key, "Transparency")) {
					Object.Transparency = luaL_checknumber(LS, 3);
				}
				return 0;
			}
		}
	}


	/*Lua Functions*/

	static int New(lua_State* LS) {
		const char* Object = luaL_checklstring(LS, 1, 0);
		void* ObjectUD;
		if (!std::strcmp(Object, "Line")) {
			ObjectUD = lua_newuserdata(LS, sizeof(DrawingAPI_Shape_Line));
			lua_getfield(LS, LUA_REGISTRYINDEX, "DrawingObject_Line");
			lua_setmetatable(LS, -2);
			new (ObjectUD)DrawingAPI_Shape_Line;
			DRAWINGS_LINE.push_back((DrawingAPI_Shape_Line*)ObjectUD);
		}
		else if (!std::strcmp(Object, "Circle")) {
			ObjectUD = lua_newuserdata(LS, sizeof(DrawingAPI_Shape_Circle));
			lua_getfield(LS, LUA_REGISTRYINDEX, "DrawingObject_Circle");
			lua_setmetatable(LS, -2);
			new (ObjectUD)DrawingAPI_Shape_Circle;
			DRAWINGS_CIRCLE.push_back((DrawingAPI_Shape_Circle*)ObjectUD);
			return 1;
		}
		else if (!std::strcmp(Object, "Square")) {
			ObjectUD = lua_newuserdata(LS, sizeof(DrawingAPI_Shape_Square));
			lua_getfield(LS, LUA_REGISTRYINDEX, "DrawingObject_Square");
			lua_setmetatable(LS, -2);
			new (ObjectUD)DrawingAPI_Shape_Square;
			DRAWINGS_SQUARE.push_back((DrawingAPI_Shape_Square*)ObjectUD);
			return 1;
		}
		else if (!std::strcmp(Object, "Triangle")) {
			ObjectUD = lua_newuserdata(LS, sizeof(DrawingAPI_Shape_Triangle));
			lua_getfield(LS, LUA_REGISTRYINDEX, "DrawingObject_Triangle");
			lua_setmetatable(LS, -2);
			new (ObjectUD)DrawingAPI_Shape_Triangle;
			DRAWINGS_TRIANGLE.push_back((DrawingAPI_Shape_Triangle*)ObjectUD);
			return 1;
		}
		else if (!std::strcmp(Object, "Text")) {
			ObjectUD = lua_newuserdata(LS, sizeof(DrawingAPI_Shape_Text));
			lua_getfield(LS, LUA_REGISTRYINDEX, "DrawingObject_Text");
			lua_setmetatable(LS, -2);
			new (ObjectUD)DrawingAPI_Shape_Text;
			DRAWINGS_TEXT.push_back((DrawingAPI_Shape_Text*)ObjectUD);
			return 1;
		}
		else {
			luaL_error(LS, "DrawingObject determined as NULL or isn't found.");
			return 0;
		}
		return 0;
	}

	static int Clear(lua_State* LS) {
		DRAWINGS_TEXT.clear();
		DRAWINGS_LINE.clear();
		DRAWINGS_SQUARE.clear();
		DRAWINGS_CIRCLE.clear();
		DRAWINGS_TRIANGLE.clear();
		return 0;
	}

	void InitiateLuaAPI(lua_State* LS) {

		/*LineObject Table*/

		luaL_newmetatable(LS, "DrawingObject_Line");
		lua_pushcfunction(LS, Lua::Line::__gc);
		lua_setfield(LS, -2, "__gc");
		lua_pushcfunction(LS, Lua::Line::__index);
		lua_setfield(LS, -2, "__index");
		lua_pushcfunction(LS, Lua::Line::__newindex);
		lua_setfield(LS, -2, "__newindex");
		lua_pop(LS, 1);

		/*TextObject Table*/

		luaL_newmetatable(LS, "DrawingObject_Text");
		lua_pushcfunction(LS, Lua::Text::__gc);
		lua_setfield(LS, -2, "__gc");
		lua_pushcfunction(LS, Lua::Text::__index);
		lua_setfield(LS, -2, "__index");
		lua_pushcfunction(LS, Lua::Text::__newindex);
		lua_setfield(LS, -2, "__newindex");
		lua_pop(LS, 1);

		/*SquareObject Table*/

		luaL_newmetatable(LS, "DrawingObject_Square");
		lua_pushcfunction(LS, Lua::Square::__gc);
		lua_setfield(LS, -2, "__gc");
		lua_pushcfunction(LS, Lua::Square::__index);
		lua_setfield(LS, -2, "__index");
		lua_pushcfunction(LS, Lua::Square::__newindex);
		lua_setfield(LS, -2, "__newindex");
		lua_pop(LS, 1);

		/*CircleObject Table*/

		luaL_newmetatable(LS, "DrawingObject_Circle");
		lua_pushcfunction(LS, Lua::Circle::__gc);
		lua_setfield(LS, -2, "__gc");
		lua_pushcfunction(LS, Lua::Circle::__index);
		lua_setfield(LS, -2, "__index");
		lua_pushcfunction(LS, Lua::Circle::__newindex);
		lua_setfield(LS, -2, "__newindex");
		lua_pop(LS, 1);

		/*TriangleObject Table*/

		luaL_newmetatable(LS, "DrawingObject_Triangle");
		lua_pushcfunction(LS, Lua::Triangle::__gc);
		lua_setfield(LS, -2, "__gc");
		lua_pushcfunction(LS, Lua::Triangle::__index);
		lua_setfield(LS, -2, "__index");
		lua_pushcfunction(LS, Lua::Triangle::__newindex);
		lua_setfield(LS, -2, "__newindex");
		lua_pop(LS, 1);


		/*Drawing.* Table*/

		lua_createtable(LS, 0, 0);
		{
			lua_pushcfunction(LS, New);
			lua_setfield(LS, -2, "new");

			lua_pushcfunction(LS, Clear);
			lua_setfield(LS, -2, "clear");
		}
		lua_setglobal(LS, "Drawing");

	}




}


/*Drawing API Rendering*/


HRESULT WINAPI DrawingPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {

	if (!Ready) {
		Ready = true;
		pSwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&Drawing_Device));
		Drawing_Device->GetImmediateContext(&Drawing_Context);

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(RBXHWND);
		ImGui_ImplDX11_Init(Drawing_Device, Drawing_Context);
		ImGui_ImplDX11_CreateDeviceObjects();

		ImGui::StyleColorsDark();
	}

	if (!Drawing_Render_Target_View) {
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&Drawing_Render_Target_Texture));
		Drawing_Device->CreateRenderTargetView(Drawing_Render_Target_Texture, nullptr, &Drawing_Render_Target_View);
		Drawing_Render_Target_Texture->Release();

		ImGui::CreateContext();
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	auto RenderList = ImGui::GetBackgroundDrawList();

	//Lines
	for (DrawingAPI_Shape_Line* Line : DRAWINGS_LINE) {
		if (Line->IsShapeVisible) {
			RenderList->AddLine({ Line->From.Position_X, Line->From.Position_Y }, { Line->To.Position_X, Line->To.Position_Y }, ImColor(Line->Colour.Colour_R, Line->Colour.Colour_G, Line->Colour.Colour_B, Line->Thickness), Line->Thickness);
		}
	}

	//Circles

	for (DrawingAPI_Shape_Circle* Circle : DRAWINGS_CIRCLE) {
		if (Circle->IsShapeVisible) {
			if (Circle->Filled) {
				RenderList->AddCircleFilled({ Circle->Position.Position_X, Circle->Position.Position_Y }, Circle->Radius, ImColor(Circle->Colour.Colour_R, Circle->Colour.Colour_G, Circle->Colour.Colour_B, Circle->Transparency), Circle->NumberOfSides);
			}
			else {
				RenderList->AddCircle({ Circle->Position.Position_X, Circle->Position.Position_Y }, Circle->Radius, ImColor(Circle->Colour.Colour_R, Circle->Colour.Colour_G, Circle->Colour.Colour_B, Circle->Transparency), Circle->NumberOfSides, Circle->Thickness);
			}
		}
	}

	//Squares

	for (DrawingAPI_Shape_Square* Square : DRAWINGS_SQUARE) {
		if (Square->IsShapeVisible) {
			if (Square->Filled) {
				RenderList->AddRectFilled({ Square->Position.Position_X, Square->Position.Position_Y }, { Square->Position.Position_X + Square->Size.Position_X, Square->Position.Position_Y + Square->Size.Position_Y }, ImColor(Square->Colour.Colour_R, Square->Colour.Colour_G, Square->Colour.Colour_B, Square->Transparency), Square->Rounding);
			}
			else {
				RenderList->AddRect({ Square->Position.Position_X, Square->Position.Position_Y }, { Square->Position.Position_X + Square->Size.Position_X, Square->Position.Position_Y + Square->Size.Position_Y }, ImColor(Square->Colour.Colour_R, Square->Colour.Colour_G, Square->Colour.Colour_B, Square->Transparency), Square->Rounding, 15, Square->Thickness);
			}
		}
	}

	//Triangles

	for (DrawingAPI_Shape_Triangle* Triangle : DRAWINGS_TRIANGLE) {
		if (Triangle->IsShapeVisible) {
			if (Triangle->Filled) {
				RenderList->AddTriangleFilled({ Triangle->Point1.Position_X, Triangle->Point1.Position_Y }, { Triangle->Point2.Position_X, Triangle->Point2.Position_Y }, { Triangle->Point3.Position_X, Triangle->Point3.Position_Y }, ImColor(Triangle->Colour.Colour_R, Triangle->Colour.Colour_G, Triangle->Colour.Colour_B, Triangle->Transparency));
			}
			else {
				RenderList->AddTriangle({ Triangle->Point1.Position_X, Triangle->Point1.Position_Y }, { Triangle->Point2.Position_X, Triangle->Point2.Position_Y }, { Triangle->Point3.Position_X, Triangle->Point3.Position_Y }, ImColor(Triangle->Colour.Colour_R, Triangle->Colour.Colour_G, Triangle->Colour.Colour_B, Triangle->Transparency), Triangle->Thickness);
			}
		}
	}

	//Text

	for (DrawingAPI_Shape_Text* Text : DRAWINGS_TEXT) {
		if (Text->IsShapeVisible) {
			RenderList->AddText({ Text->Position.Position_X, Text->Position.Position_Y }, ImColor(Text->Colour.Colour_R, Text->Colour.Colour_G, Text->Colour.Colour_B, Text->Transparency), Text->Text, Text->TextEnd);
		}
	}



	ImGui::Render();
	Drawing_Context->OMSetRenderTargets(1, &Drawing_Render_Target_View, nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return OldPresent(pSwapChain, SyncInterval, Flags);
}

LRESULT IMGUI_IMPL_API NewWNDProc(HWND hWnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	if (Ready) {
		ImGui_ImplWin32_WndProcHandler(hWnd, msg, w_param, l_param);
		switch (msg) {
		case WM_SIZE: {
			if (Drawing_Render_Target_View) {
				Drawing_Render_Target_View->Release();
				Drawing_Render_Target_View = nullptr;
			}
			RECT RCT;
			GetWindowRect(hWnd, &RCT);
			SW = RCT.right - RCT.left;
			SH = RCT.bottom - RCT.top;
			break;
		}
		default: {
			break;
		}
		}
	}
	return CallWindowProc(reinterpret_cast<WNDPROC>(WNDProc), hWnd, msg, w_param, l_param);
}

bool DrawingAPI::InitiateDrawingAPI(lua_State* LS) {
	RBXHWND = FindWindowW(NULL, L"Roblox");


	DXGI_SWAP_CHAIN_DESC SwapChainDesc = { 0 };
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	SwapChainDesc.OutputWindow = RBXHWND;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Windowed = TRUE;

	D3D_FEATURE_LEVEL RequestedLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	IDXGISwapChain* SwapChain = nullptr;
	ID3D11Device* Device;
	ID3D11DeviceContext* Context;

	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, RequestedLevels, 1, D3D11_SDK_VERSION, &SwapChainDesc, &SwapChain, &Device, nullptr, &Context))) {
		return false;
	}

	VirtualTable = *reinterpret_cast<uintptr_t*>(SwapChain);
	OldPresent = *reinterpret_cast<PresentFunctionHook*>(VirtualTable + 32);

	unsigned long VProtect;
	if (!VirtualProtect(reinterpret_cast<LPVOID>(VirtualTable), 64, PAGE_READWRITE, &VProtect)) {
		return false;
	}

	*reinterpret_cast<void**>(VirtualTable + 32) = &DrawingPresent;

	if (!VirtualProtect(reinterpret_cast<LPVOID>(VirtualTable), 64, VProtect, &VProtect)) {
		return false;
	}

	WNDProc = GetWindowLong(RBXHWND, GWLP_WNDPROC);
	SetWindowLong(RBXHWND, GWLP_WNDPROC, reinterpret_cast<LONG>(NewWNDProc));

	RECT RCT;
	GetWindowRect(RBXHWND, &RCT);
	SW = RCT.right - RCT.left;
	SH = RCT.bottom - RCT.top;


	DrawingAPILuaAPI::InitiateLuaAPI(LS);
	return true;
}

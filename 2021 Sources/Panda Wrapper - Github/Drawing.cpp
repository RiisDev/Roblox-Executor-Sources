///*
//	Drawing lib for Moonware, made by Moonware Dev Team.
//    Copyright (C) 2020  Cursed Torch#1468
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License as
//    published by the Free Software Foundation, either version 3 of the
//    License, or (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.
//*/
//
//#include "Drawing.hpp"
//
//#include "Roblox.hpp"
//
//#include <Windows.h>
//#include <iostream>
//#include <string>
//#include "Rlua.h"
//
//std::vector<DrawingLine*> linedrawings = {};
//std::vector<DrawingCircle*> circledrawings = {};
//std::vector<DrawingSquare*> squaredrawings = {};
//std::vector<DrawingTriangle*> triangledrawings = {};
//std::vector<DrawingText*> textdrawings = {};
//
//DrawingVec2::DrawingVec2(float _x, float _y)
//{
//	x = _x;
//	y = _y;
//}
//
//DrawingColor::DrawingColor(float _r, float _g, float _b)
//{
//	r = _r;
//	g = _g;
//	b = _b;
//}
//
//void vector2new(lua_State* ls, int x, int y) {
//	lua_getglobal(ls, "Vector2");
//	lua_getfield(ls, -1, "new");
//	lua_pushnumber(ls, x);
//	lua_pushnumber(ls,y);
//	lua_call(ls, 2, 1);
//}
//
//void vector2new(lua_State* ls, DrawingVec2 dv2) {
//	lua_getglobal(ls, "Vector2");
//	lua_getfield(ls, -1, "new");
//	lua_pushnumber(ls, dv2.x);
//	lua_pushnumber(ls, dv2.y);
//	lua_call(ls, 2, 1);
//}
//
//void color3fromrgb(lua_State* ls, int r, int g, int b) {
//	lua_getglobal(ls, "Color3");
//	lua_getfield(ls, -1, "fromRGB");
//	lua_pushnumber(ls, r);
//	lua_pushnumber(ls, g);
//	lua_pushnumber(ls, b);
//	lua_call(ls, 3, 1);
//}
//
//void color3fromrgb(lua_State* ls, DrawingColor dc) {
//	lua_getglobal(ls, "Color3");
//	lua_getfield(ls, -1, "new");
//	lua_pushnumber(ls, dc.r);
//	lua_pushnumber(ls, dc.g);
//	lua_pushnumber(ls, dc.b);
//	lua_call(ls, 3, 1);
//}
//
//DrawingVec2 getdwfromvector2(lua_State* ls, int idx) {
//	luaL_checktype(ls, idx, RLUA_TUSERDATA);
//	lua_pushvalue(ls, idx);
//	lua_getfield(ls, -1, "X");
//	float x = luaL_checknumber(ls, -1);
//	lua_getfield(ls, -2, "Y");
//	float y = luaL_checknumber(ls, -1);
//	return { x, y };
//}
//
//DrawingColor getdcfromcolor3(lua_State* ls, int idx) {
//	luaL_checktype(ls, idx, RLUA_TUSERDATA);
//	lua_pushvalue(ls, idx);
//	lua_getfield(ls, -1, "r");
//	float r = luaL_checknumber(ls, -1);
//	lua_getfield(ls, -2, "g");
//	float g = luaL_checknumber(ls, -1);
//	lua_getfield(ls, -3, "b");
//	float b = luaL_checknumber(ls, -1);
//	return { r, g, b };
//}
//
///* DRAWING : LINE */
//static int drawingline_remove(lua_State* ls) {	
//	luaL_checktype(ls, 1, RLUA_TUSERDATA);
//	DrawingLine& db = *static_cast<DrawingLine*>(lua_touserdata(ls, 1));
//	//printf("bye bye %p\n", &db);
//	for (int idx = 0; idx < linedrawings.size(); idx++) {
//		if (linedrawings[idx] == &db) {
//			linedrawings.erase(linedrawings.begin() + idx);
//		}
//	}
//	return 0;
//}
//
//static int drawingline_index(lua_State* ls) {
//	luaL_checktype(ls, 1, RLUA_TUSERDATA);
//	DrawingLine& dl = *static_cast<DrawingLine*>(lua_touserdata(ls, 1));
//	const char* key = luaL_checklstring(ls, 2, 0);
//	//printf("index start: %p (%s)\n", &dl, key);
//	if (0 == std::strcmp(key, "Remove")) {
//		lua_pushcfunction(ls, drawingline_remove);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Visible")) {
//		lua_pushboolean(ls, dl.visible);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "ZIndex")) {
//		lua_pushboolean(ls, dl.zindex);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "From")) {
//		vector2new(ls, dl.from);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "To")) {
//		vector2new(ls, dl.to);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Color")) {
//		color3fromrgb(ls, dl.color);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Thickness")) {
//		lua_pushnumber(ls, dl.thickness);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Transparency")) {
//		lua_pushnumber(ls, dl.transparency);
//		return 1;
//	}
//	return 0;
//}
//
//static int drawingline_newindex(lua_State* ls) {
//	luaL_checktype(ls, 1, RLUA_TUSERDATA);	
//	DrawingLine& dl = *static_cast<DrawingLine*>(lua_touserdata(ls, 1));
//	const char* key = luaL_checklstring(ls, 2, 0);
//	//printf("newindex start: %p (%s)\n", &dl, key);
//	if (0 == std::strcmp(key, "Visible")) {
//		luaL_checktype(ls, 3, RLUA_TBOOLEAN);
//		dl.visible = lua_toboolean(ls, 3);
//		return 0;
//	}
//	if (0 == std::strcmp(key, "ZIndex")) {
//		dl.zindex = luaL_checknumber(ls, 3);
//		return 0;
//	}
//	else if (0 == std::strcmp(key, "From")) {
//		dl.from = getdwfromvector2(ls, 3);
//		return 0;
//	}
//	else if (0 == std::strcmp(key, "To")) {
//		dl.to = getdwfromvector2(ls, 3);
//		return 0;
//	}
//	else if (0 == std::strcmp(key, "Color")) {
//		dl.color = getdcfromcolor3(ls, 3);
//		return 0;
//	}
//	else if (0 == std::strcmp(key, "Thickness")) {
//		dl.thickness = luaL_checknumber(ls, 3);
//		return 0;
//	}
//	else if (0 == std::strcmp(key, "Transparency")) {
//		dl.transparency = luaL_checknumber(ls, 3);
//		return 0;
//	}
//	return 0;
//}
//
///* DRAWING : CIRCLE */
//
//static int drawingcircle_remove(lua_State* ls) {
//	luaL_checktype(ls, 1, RLUA_TUSERDATA);
//	DrawingCircle& db = *static_cast<DrawingCircle*>(lua_touserdata(ls, 1));
//	//printf("bye bye %p\n", &db);
//	for (int idx = 0; idx < circledrawings.size(); idx++) {
//		if (circledrawings[idx] == &db) {
//			circledrawings.erase(circledrawings.begin() + idx);
//		}
//	}
//	return 0;
//}
//
//static int drawingcircle_index(lua_State* ls) {
//	luaL_checktype(ls, 1, RLUA_TUSERDATA);
//	DrawingCircle& dc = *static_cast<DrawingCircle*>(lua_touserdata(ls, 1));
//	const char* key = luaL_checklstring(ls, 2, 0);
//	//printf("index start: %p (%s)\n", &dc, key);
//	if (0 == std::strcmp(key, "Remove")) {
//		lua_pushcfunction(ls, drawingcircle_remove);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Visible")) {
//		lua_pushboolean(ls, dc.visible);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "ZIndex")) {
//		lua_pushboolean(ls, dc.zindex);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Position")) {
//		vector2new(ls, dc.position);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Radius")) {
//		lua_pushnumber(ls, dc.radius);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Color")) {
//		color3fromrgb(ls, dc.color);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Thickness")) {
//		lua_pushnumber(ls, dc.thickness);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Filled")) {
//		lua_pushboolean(ls, dc.filled);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Transparency")) {
//		lua_pushnumber(ls, dc.transparency);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "NumSides")) {
//		lua_pushnumber(ls, dc.numsides);
//		return 1;
//	}
//	return 0;
//}
//
//static int drawingcircle_newindex(lua_State* ls) {
//	luaL_checktype(ls, 1, RLUA_TUSERDATA);
//	DrawingCircle& dc = *static_cast<DrawingCircle*>(lua_touserdata(ls, 1));
//	const char* key = luaL_checklstring(ls, 2, 0);
//	//printf("newindex start: %p (%s)\n", &dc, key);
//	if (0 == std::strcmp(key, "Visible")) {
//		luaL_checktype(ls, 3, RLUA_TBOOLEAN);
//		dc.visible = lua_toboolean(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "ZIndex")) {
//		dc.zindex = luaL_checknumber(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Position")) {
//		dc.position = getdwfromvector2(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Radius")) {
//		dc.radius = luaL_checknumber(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Color")) {
//		dc.color = getdcfromcolor3(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Thickness")) {
//		dc.thickness = luaL_checknumber(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Filled")) {
//		luaL_checktype(ls, 3, RLUA_TBOOLEAN);
//		dc.filled = lua_toboolean(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Transparency")) {
//		dc.transparency = luaL_checknumber(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "NumSides")) {
//		dc.numsides = luaL_checknumber(ls, 3);
//	}
//	return 0;
//}
//
///* DRAWING : SQUARE */
//
//static int drawingsquare_remove(lua_State* ls) {
//	luaL_checktype(ls, 1, RLUA_TUSERDATA);
//	DrawingSquare& db = *static_cast<DrawingSquare*>(lua_touserdata(ls, 1));
//	//printf("bye bye %p\n", &db);
//	for (int idx = 0; idx < squaredrawings.size(); idx++) {
//		if (squaredrawings[idx] == &db) {
//			squaredrawings.erase(squaredrawings.begin() + idx);
//		}
//	}
//	return 0;
//}
//
//static int drawingsquare_index(lua_State* ls) {
//	luaL_checktype(ls, 1, RLUA_TUSERDATA);
//	DrawingSquare& dc = *static_cast<DrawingSquare*>(lua_touserdata(ls, 1));
//	const char* key = luaL_checklstring(ls, 2, 0);
//	//printf("index start: %p (%s)\n", &dc, key);
//	if (0 == std::strcmp(key, "Remove")) {
//		lua_pushcfunction(ls, drawingsquare_remove);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Visible")) {
//		lua_pushboolean(ls, dc.visible);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "ZIndex")) {
//		lua_pushboolean(ls, dc.zindex);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Position")) {
//		vector2new(ls, dc.position);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Size")) {
//		vector2new(ls, dc.size);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Color")) {
//		color3fromrgb(ls, dc.color);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Thickness")) {
//		lua_pushnumber(ls, dc.thickness);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Filled")) {
//		lua_pushboolean(ls, dc.filled);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Transparency")) {
//		lua_pushnumber(ls, dc.transparency);
//		return 1;
//	}
//	return 0;
//}
//
//static int drawingsquare_newindex(lua_State* ls) {
//	luaL_checktype(ls, 1, RLUA_TUSERDATA);
//	DrawingSquare& dc = *static_cast<DrawingSquare*>(lua_touserdata(ls, 1));
//	const char* key = luaL_checklstring(ls, 2, 0);
//	//printf("newindex start: %p (%s)\n", &dc, key);
//	if (0 == std::strcmp(key, "Visible")) {
//		luaL_checktype(ls, 3, RLUA_TBOOLEAN);
//		dc.visible = lua_toboolean(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "ZIndex")) {
//		dc.zindex = luaL_checknumber(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Position")) {
//		dc.position = getdwfromvector2(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Size")) {
//		dc.size = getdwfromvector2(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Color")) {
//		dc.color = getdcfromcolor3(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Thickness")) {
//		dc.thickness = luaL_checknumber(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Filled")) {
//		luaL_checktype(ls, 3, RLUA_TBOOLEAN);
//		dc.filled = lua_toboolean(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Transparency")) {
//		dc.transparency = luaL_checknumber(ls, 3);
//	}
//	return 0;
//}
//
///* DRAWING : TRIANGLE */
//
//static int drawingtriangle_remove(lua_State* ls) {
//	luaL_checktype(ls, 1, RLUA_TUSERDATA);
//	DrawingTriangle& db = *static_cast<DrawingTriangle*>(lua_touserdata(ls, 1));
//	//printf("bye bye %p\n", &db);
//	for (int idx = 0; idx < triangledrawings.size(); idx++) {
//		if (triangledrawings[idx] == &db) {
//			triangledrawings.erase(triangledrawings.begin() + idx);
//		}
//	}
//	return 0;
//}
//
//static int drawingtriangle_index(lua_State* ls) {
//	luaL_checktype(ls, 1, RLUA_TUSERDATA);
//	DrawingTriangle& dc = *static_cast<DrawingTriangle*>(lua_touserdata(ls, 1));
//	const char* key = luaL_checklstring(ls, 2, 0);
//	//printf("index start: %p (%s)\n", &dc, key);
//	if (0 == std::strcmp(key, "Remove")) {
//		lua_pushcfunction(ls, drawingtriangle_remove);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Visible")) {
//		lua_pushboolean(ls, dc.visible);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "ZIndex")) {
//		lua_pushboolean(ls, dc.zindex);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "PointA")) {
//		vector2new(ls, dc.pointa);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "PointB")) {
//		vector2new(ls, dc.pointb);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "PointC")) {
//		vector2new(ls, dc.pointc);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Color")) {
//		color3fromrgb(ls, dc.color);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Thickness")) {
//		lua_pushnumber(ls, dc.thickness);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Filled")) {
//		lua_pushboolean(ls, dc.filled);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Transparency")) {
//		lua_pushnumber(ls, dc.transparency);
//		return 1;
//	}
//	return 0;
//}
//
//static int drawingtriangle_newindex(lua_State* ls) {
//	luaL_checktype(ls, 1, RLUA_TUSERDATA);
//	DrawingTriangle& dc = *static_cast<DrawingTriangle*>(lua_touserdata(ls, 1));
//	const char* key = luaL_checklstring(ls, 2, 0);
//	//printf("newindex start: %p (%s)\n", &dc, key);
//	if (0 == std::strcmp(key, "Visible")) {
//		luaL_checktype(ls, 3, RLUA_TBOOLEAN);
//		dc.visible = lua_toboolean(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "ZIndex")) {
//		dc.zindex = luaL_checknumber(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "PointA")) {
//		dc.pointa = getdwfromvector2(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "PointB")) {
//		dc.pointb = getdwfromvector2(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "PointC")) {
//		dc.pointc = getdwfromvector2(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Color")) {
//		dc.color = getdcfromcolor3(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Thickness")) {
//		dc.thickness = luaL_checknumber(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Filled")) {
//		luaL_checktype(ls, 3, RLUA_TBOOLEAN);
//		dc.filled = lua_toboolean(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Transparency")) {
//		dc.transparency = luaL_checknumber(ls, 3);
//	}
//	return 0;
//}
//
///* DRAWING : TEXT */
//
//static int drawingtext_remove(lua_State* ls) {
//	luaL_checktype(ls, 1, RLUA_TUSERDATA);
//	DrawingText& db = *static_cast<DrawingText*>(lua_touserdata(ls, 1));
//	//printf("bye bye %p\n", &db);
//	for (int idx = 0; idx < textdrawings.size(); idx++) {
//		if (textdrawings[idx] == &db) {
//			textdrawings.erase(textdrawings.begin() + idx);
//		}
//	}
//	return 0;
//}
//
//static int drawingtext_index(lua_State* ls) {
//	luaL_checktype(ls, 1, RLUA_TUSERDATA);
//	DrawingText& dc = *static_cast<DrawingText*>(lua_touserdata(ls, 1));
//	const char* key = luaL_checklstring(ls, 2, 0);
//	//printf("index start: %p (%s)\n", &dc, key);
//	if (0 == std::strcmp(key, "Remove")) {
//		lua_pushcfunction(ls, drawingtext_remove);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Visible")) {
//		lua_pushboolean(ls, dc.visible);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "ZIndex")) {
//		lua_pushboolean(ls, dc.zindex);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Text")) {
//		lua_pushlstring(ls, dc.text, dc.textlen);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Position")) {
//		vector2new(ls, dc.position);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Color")) {
//		color3fromrgb(ls, dc.color);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Center")) {
//		lua_pushboolean(ls, dc.center);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Outline")) {
//		lua_pushboolean(ls, dc.outline);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Transparency")) {
//		lua_pushnumber(ls, dc.transparency);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "Size")) {
//		lua_pushnumber(ls, dc.size);
//		return 1;
//	}
//	else if (0 == std::strcmp(key, "TextBounds")) {
//		vector2new(ls, dc.textbounds);
//		return 1;
//	}
//	return 0;
//}
//
//static int drawingtext_newindex(lua_State* ls) {
//	luaL_checktype(ls, 1, RLUA_TUSERDATA);
//	DrawingText& dc = *static_cast<DrawingText*>(lua_touserdata(ls, 1));
//	const char* key = luaL_checklstring(ls, 2, 0);
//	//printf("newindex start: %p (%s)\n", &dc, key);
//	if (0 == std::strcmp(key, "Visible")) {
//		luaL_checktype(ls, 3, RLUA_TBOOLEAN);
//		dc.visible = lua_toboolean(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "ZIndex")) {
//		dc.zindex = luaL_checknumber(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Text")) {
//		unsigned int len;
//		const char* text = luaL_checklstring(ls, 3, &len);
//		dc.text = text;
//		dc.textend = text + len;
//		dc.textlen = len;
//	}
//	else if (0 == std::strcmp(key, "Position")) {
//		dc.position = getdwfromvector2(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Color")) {
//		dc.color = getdcfromcolor3(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Center")) {
//		luaL_checktype(ls, 3, RLUA_TBOOLEAN);
//		dc.center = lua_toboolean(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Outline")) {
//		luaL_checktype(ls, 3, RLUA_TBOOLEAN);
//		dc.outline = lua_toboolean(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Transparency")) {
//		dc.transparency = luaL_checknumber(ls, 3);
//	}
//	else if (0 == std::strcmp(key, "Size")) {
//		dc.size = luaL_checknumber(ls, 3);
//	}
//	return 0;
//}
//
//void initmetatables(lua_State* ls) {
//	/* DRAWING : LINE */
//	luaL_newmetatable(ls, "drawingline");
//
//	lua_pushcfunction(ls, drawingline_index);
//	lua_setfield(ls, -2, "__index");
//
//	lua_pushcfunction(ls, drawingline_newindex);
//	lua_setfield(ls, -2, "__newindex");
//
//	lua_pushcfunction(ls, drawingline_remove);
//	lua_setfield(ls, -2, "__gc");
//
//	lua_pop(ls, 1);
//
//	/* DRAWING : CIRCLE */
//	luaL_newmetatable(ls, "drawingcircle");
//
//	lua_pushcfunction(ls, drawingcircle_index);
//	lua_setfield(ls, -2, "__index");
//
//	lua_pushcfunction(ls, drawingcircle_newindex);
//	lua_setfield(ls, -2, "__newindex");
//
//	lua_pushcfunction(ls, drawingcircle_remove);
//	lua_setfield(ls, -2, "__gc");
//
//	lua_pop(ls, 1);
//
//	/* DRAWING : SQUARE */
//	luaL_newmetatable(ls, "drawingsquare");
//
//	lua_pushcfunction(ls, drawingsquare_index);
//	lua_setfield(ls, -2, "__index");
//
//	lua_pushcfunction(ls, drawingsquare_newindex);
//	lua_setfield(ls, -2, "__newindex");
//
//	lua_pushcfunction(ls, drawingsquare_remove);
//	lua_setfield(ls, -2, "__gc");
//
//	lua_pop(ls, 1);
//
//	/* DRAWING : TRIANGLE */
//	luaL_newmetatable(ls, "drawingtriangle");
//
//	lua_pushcfunction(ls, drawingtriangle_index);
//	lua_setfield(ls, -2, "__index");
//
//	lua_pushcfunction(ls, drawingtriangle_newindex);
//	lua_setfield(ls, -2, "__newindex");
//
//	lua_pushcfunction(ls, drawingtriangle_remove);
//	lua_setfield(ls, -2, "__gc");
//
//	lua_pop(ls, 1);
//
//	/* DRAWING : TEXT */
//	luaL_newmetatable(ls, "drawingtext");
//
//	lua_pushcfunction(ls, drawingtext_index);
//	lua_setfield(ls, -2, "__index");
//
//	lua_pushcfunction(ls, drawingtext_newindex);
//	lua_setfield(ls, -2, "__newindex");
//
//	lua_pushcfunction(ls, drawingtext_remove);
//	lua_setfield(ls, -2, "__gc");
//
//	lua_pop(ls, 1);
//}
//
//static int drawing_new(lua_State* ls) {
//	const char* type = luaL_checklstring(ls, 1, 0);
//	//printf("start: %s\n", type);
//	void* udata;
//	if (std::string(type) == "Line") {
//		udata = lua_newuserdata(ls, sizeof(DrawingLine));
//		Roblox::Functions::rluaL_setmetatable(rL, "drawingline");
//		new (udata) DrawingLine;
//		linedrawings.push_back((DrawingLine*)udata);
//	}
//	else if (std::string(type) == "Circle") {
//		udata = lua_newuserdata(ls, sizeof(DrawingCircle));
//		Roblox::Functions::rluaL_setmetatable(rL, "drawingcircle");
//		new (udata) DrawingCircle;
//		circledrawings.push_back((DrawingCircle*)udata);
//	}
//	else if (std::string(type) == "Square") {
//		udata = lua_newuserdata(ls, sizeof(DrawingSquare));
//		Roblox::Functions::rluaL_setmetatable(rL, "drawingsquare");
//		new (udata) DrawingSquare;
//		squaredrawings.push_back((DrawingSquare*)udata);
//	}
//	else if (std::string(type) == "Triangle") {
//		udata = lua_newuserdata(ls, sizeof(DrawingTriangle));
//		Roblox::Functions::rluaL_setmetatable(rL, "drawingtriangle");
//		new (udata) DrawingTriangle;
//		triangledrawings.push_back((DrawingTriangle*)udata);
//	}
//	else if (std::string(type) == "Text") {
//		udata = lua_newuserdata(ls, sizeof(DrawingText));
//		Roblox::Functions::rluaL_setmetatable(rL, "drawingtext");
//		new (udata) DrawingText;
//		textdrawings.push_back((DrawingText*)udata);
//	}
//	else {
//		//printf("oops: %s\n", type);
//		luaL_error(ls, "Unsupported Drawing class.");
//		return 0;
//	}
//	//printf("end: %s\n", type);
//	return 1;
//}
//
//static int drawing_clear(lua_State* ls) {
//	linedrawings.clear();
//	circledrawings.clear();
//	squaredrawings.clear();
//	triangledrawings.clear();
//	textdrawings.clear();
//	return 0;
//}
//
//void newfunc(DWORD* ls, const char* name, void* func) {
//	rlua_pushstring(ls, name);
//	rlua_pushccfunction(ls, func);
//	rlua_settable(ls, -3);
//}
//
//void Drawing::Register(lua_State* ls)
//{
//	initmetatables(ls);
//	lua_createtable(ls, 0, 0);
//	newfunc(ls, "new", drawing_new); 
//	newfunc(ls, "clear", drawing_clear);
//	lua_setglobal(ls, "Drawing");
//	lua_pop(ls, 1);
//	Roblox::Functions::rlua_settop(GetLuaState(), 0);
//}
//
//std::vector<DrawingLine*> Drawing::GetLineDrawings()
//{
//	return linedrawings;
//}
//
//std::vector<DrawingCircle*> Drawing::GetCircleDrawings()
//{
//	return circledrawings;
//}
//
//std::vector<DrawingSquare*> Drawing::GetSquareDrawings()
//{
//	return squaredrawings;
//}
//
//std::vector<DrawingTriangle*> Drawing::GetTriangleDrawings()
//{
//	return triangledrawings;
//}
//
//std::vector<DrawingText*> Drawing::GetTextDrawings()
//{
//	return textdrawings;
//}
//
//DrawingBase::~DrawingBase()
//{
//}

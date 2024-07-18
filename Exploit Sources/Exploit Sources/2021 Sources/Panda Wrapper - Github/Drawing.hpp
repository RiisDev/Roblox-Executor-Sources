/*
	Drawing lib for Moonware, made by Moonware Dev Team.
    Copyright (C) 2020  Cursed Torch#1468

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <vector>

struct DrawingVec2 {
	float x = 0;
	float y = 0;
	DrawingVec2(float _x = 0, float _y = 0);
};

struct DrawingColor {
	float r = 0;
	float g = 0;
	float b = 0;
	DrawingColor(float _r = 0, float _g = 0, float _b = 0);
};

struct DrawingBase {
	bool visible = false;
	int zindex = 1;
	virtual ~DrawingBase();
};

struct DrawingLine : public DrawingBase {
	DrawingVec2 from;
	DrawingVec2 to;
	DrawingColor color;
	float thickness = 16;
	float transparency = 1;
};

struct DrawingSquare : public DrawingBase {
	DrawingVec2 position;
	DrawingVec2 size;
	DrawingColor color;
	float thickness = 16;
	bool filled = false;
	float transparency = 1;
};

struct DrawingCircle : public DrawingBase {
	DrawingVec2 position;
	float radius = 0;
	DrawingColor color;
	float thickness = 15;
	bool filled = false;
	float transparency = 1;
	float numsides = 250;
};

struct DrawingTriangle : public DrawingBase {
	DrawingVec2 pointa;
	DrawingVec2 pointb;
	DrawingVec2 pointc;
	DrawingColor color;
	float thickness = 15;
	bool filled = false;
	float transparency = 1;
};

struct DrawingText : public DrawingBase {
	const char* text = "Text";
	const char* textend = text+strlen(text);
	int textlen = strlen(text);
	DrawingVec2 position;
	DrawingColor color;
	bool center = false;
	bool outline = false;	
	float transparency = 1;
	float size = 0;
	DrawingVec2 textbounds = {0, 16};
};

class Drawing
{
public:
	static void Register(lua_State* ls);
	static std::vector<DrawingLine*> GetLineDrawings();
	static std::vector<DrawingCircle*> GetCircleDrawings();
	static std::vector<DrawingSquare*> GetSquareDrawings();
	static std::vector<DrawingTriangle*> GetTriangleDrawings();
	static std::vector<DrawingText*> GetTextDrawings();
};


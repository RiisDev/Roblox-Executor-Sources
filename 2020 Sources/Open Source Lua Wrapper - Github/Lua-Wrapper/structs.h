#pragma once
#include <string>

/*Position & Colour*/

struct DrawingAPI_Vector2 {
	float Position_X = 0;
	float Position_Y = 0;
	DrawingAPI_Vector2(float NewX = 0, float NewY = 0) {
		Position_X = NewX;
		Position_Y = NewY;
	};
};

struct DrawingAPI_Colour3 {
	float Colour_R = 0;
	float Colour_G = 0;
	float Colour_B = 0;
	DrawingAPI_Colour3(float R = 0, float G = 0, float B = 0) {
		Colour_R = R;
		Colour_G = G;
		Colour_B = B;
	}
};

/*Shapes & Text*/

struct DrawingAPI_Shape {
	bool IsShapeVisible = true;
	int ZIndex = 1;
	virtual ~DrawingAPI_Shape() { }
};


struct DrawingAPI_Shape_Line : public DrawingAPI_Shape {
	DrawingAPI_Vector2 From;
	DrawingAPI_Vector2 To;
	DrawingAPI_Colour3 Colour;
	float Thickness = 16; //16 by default
	float Transparency = 1;
};

struct DrawingAPI_Shape_Square : public DrawingAPI_Shape {
	DrawingAPI_Vector2 Position;
	DrawingAPI_Vector2 Size;
	DrawingAPI_Colour3 Colour;
	float Thickness = 16; //16 by default
	float Rounding = 0;
	bool Filled = false;
	float Transparency = 1;
};

struct DrawingAPI_Shape_Circle : public DrawingAPI_Shape {
	DrawingAPI_Vector2 Position;
	DrawingAPI_Colour3 Colour;
	float Radius = 0;
	float Thickness = 16; //16 by default
	bool Filled = false;
	float NumberOfSides = 250;
	float Transparency = 1;
};

struct DrawingAPI_Shape_Triangle : public DrawingAPI_Shape {
	DrawingAPI_Vector2 Point1;
	DrawingAPI_Vector2 Point2;
	DrawingAPI_Vector2 Point3;
	DrawingAPI_Colour3 Colour;
	float Thickness = 16; //16 by default
	bool Filled = false;
	float Transparency = 1;
};

struct DrawingAPI_Shape_Text : public DrawingAPI_Shape {
	const char* Text;
	const char* TextEnd;
	DrawingAPI_Vector2 Position;
	DrawingAPI_Colour3 Colour;
	bool Center = false;
	bool Outline = false;
	float Transparency = 1;
	DrawingAPI_Vector2 TextBounds = { 0, 16 };
};

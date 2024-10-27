#include <Windows.h>
#include <vector>
#include <string>
#include <functional>
#include <winnt.h>
#include <iostream>
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string> 
#include <thread>
#include <fstream>
#include <filesystem>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <chrono>
#include <string>
#include <regex>
#include <cmath>
#include <d3d11.h>
#include <D3Dcompiler.h>
#include <d3d9.h>
#include "..//..//RBX.hpp"
#include "..//ImGui/imgui.h"
#include "..//ImGUI/imgui_impl_dx11.h"
#include "..//ImGUI/imgui_impl_win32.h"
#include "..//ImGUI/imgui_impl_dx9.h"
#include "..//MinHook/MinHook.hpp"
#pragma comment(lib, "D3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "libMinHook.x86.lib")

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
    const char* textend = text + strlen(text);
    int textlen = strlen(text);
    DrawingVec2 position;
    DrawingColor color;
    bool center = false;
    bool outline = false;
    float transparency = 1;
    float size = 0;
    DrawingVec2 textbounds = { 0, 16 };
};

class Drawing
{
public:
    static void Register(DWORD ls);
    static std::vector<DrawingLine*> GetLineDrawings();
    static std::vector<DrawingCircle*> GetCircleDrawings();
    static std::vector<DrawingSquare*> GetSquareDrawings();
    static std::vector<DrawingTriangle*> GetTriangleDrawings();
    static std::vector<DrawingText*> GetTextDrawings();
};

std::vector<DrawingLine*> DrawLine;
std::vector<DrawingCircle*> DrawCircle;
std::vector<DrawingSquare*> DrawSquare;
std::vector<DrawingTriangle*> DrawTriangle;
std::vector<DrawingText*> DrawTexts;

DrawingVec2::DrawingVec2(float _x, float _y)
{
	x = _x;
	y = _y;
}

DrawingColor::DrawingColor(float _r, float _g, float _b)
{
	r = _r;
	g = _g;
	b = _b;
}

void Vec2New(DWORD ls, int x, int y) {
	Fluxus::RBX RBX(ls);

	RBX.GetGlobal("Vector2");
	RBX.Getfield(-1, "new");
	RBX.PushNumber(x);
	RBX.PushNumber(y);
	RBX.ECall(2, 1);
}

void Vec2New(DWORD ls, DrawingVec2 dv2) {
	Fluxus::RBX RBX(ls);

	RBX.GetGlobal("Vector2");
	RBX.Getfield(-1, "new");
	RBX.PushNumber(dv2.x);
	RBX.PushNumber(dv2.y);
	RBX.ECall(2, 1);
}

void Color3FromRGB(DWORD ls, int r, int g, int b) {
	Fluxus::RBX RBX(ls);

	RBX.GetGlobal("Color3");
	RBX.Getfield(-1, "fromRGB");
	RBX.PushNumber(r);
	RBX.PushNumber(g);
	RBX.PushNumber(b);
	RBX.ECall(3, 1);
}

void Color3FromRGB(DWORD ls, DrawingColor dc) {
	Fluxus::RBX RBX(ls);

	RBX.GetGlobal("Color3");
	RBX.Getfield(-1, "new");
	RBX.PushNumber(dc.r);
	RBX.PushNumber(dc.g);
	RBX.PushNumber(dc.b);
	RBX.ECall(3, 1);
}

DrawingVec2 GetDrawingVector(DWORD ls, int idx) {
	Fluxus::RBX RBX(ls);

	//(ls, idx, LUA_TUSERDATA);
	RBX.PushValue(idx);
	RBX.Getfield(-1, "X");
	float x = RBX.ToNumber(-1,0);
	RBX.Getfield(-2, "Y");
	float y = RBX.ToNumber(-1,0);
	return { x, y };
}



DrawingColor getdcfromcolor3(DWORD ls, int idx) {
	Fluxus::RBX RBX(ls);

	//(ls, idx, LUA_TUSERDATA);
	RBX.PushValue(idx);
	RBX.Getfield(-1, "r");
	float r = RBX.ToNumber(-1,0);
	RBX.Getfield(-2, "g");
	float g = RBX.ToNumber(-1,0);
	RBX.Getfield(-3, "b");
	float b = RBX.ToNumber(-1,0);
	return { r, g, b };
}

/* DRAWING : LINE */
static int LineRemove(DWORD ls) {
	Fluxus::RBX RBX(ls);

	//(ls, 1, LUA_TUSERDATA);
	DrawingLine& db = *static_cast<DrawingLine*>((void*)RBX.ToUserData(1));
	for (size_t idx = 0; idx < DrawLine.size(); idx++) {
		if (DrawLine[idx] == &db) {
			DrawLine.erase(DrawLine.begin() + idx);
		}
	}
	return 0;
}

static int LineIndex(DWORD ls) {
	Fluxus::RBX RBX(ls);

	//(ls, 1, LUA_TUSERDATA);
	DrawingLine& dl = *static_cast<DrawingLine*>((void*)RBX.ToUserData(1));
	const char* key = RBX.Tolstring(2, 0);
	if (!std::strcmp(key, "Remove")) {
		RBX.PushCFunction(LineRemove);
		return 1;
	}
	else if (!std::strcmp(key, "Visible")) {
		RBX.PushBoolean(dl.visible);
		return 1;
	}
	else if (!std::strcmp(key, "ZIndex")) {
		RBX.PushBoolean(dl.zindex);
		return 1;
	}
	else if (!std::strcmp(key, "From")) {
		Vec2New(ls, dl.from);
		return 1;
	}
	else if (!std::strcmp(key, "To")) {
		Vec2New(ls, dl.to);
		return 1;
	}
	else if (!std::strcmp(key, "Color")) {
		Color3FromRGB(ls, dl.color);
		return 1;
	}
	else if (!std::strcmp(key, "Thickness")) {
		RBX.PushNumber(dl.thickness);
		return 1;
	}
	else if (!std::strcmp(key, "Transparency")) {
		RBX.PushNumber(dl.transparency);
		return 1;
	}
	return 0;
}

static int LineNewIndex(DWORD ls) {
	Fluxus::RBX RBX(ls);

	//(ls, 1, LUA_TUSERDATA);
	DrawingLine& dl = *static_cast<DrawingLine*>((void*)RBX.ToUserData(1));
	const char* key = RBX.Tolstring(2, 0);
	if (!std::strcmp(key, "Visible")) {
		//(ls, 3, LUA_TBOOLEAN);
		dl.visible = RBX.ToBoolean(3);
		return 0;
	}
	if (!std::strcmp(key, "ZIndex")) {
		dl.zindex = RBX.ToNumber(3,0);
		return 0;
	}
	else if (!std::strcmp(key, "From")) {
		dl.from = GetDrawingVector(ls, 3);
		return 0;
	}
	else if (!std::strcmp(key, "To")) {
		dl.to = GetDrawingVector(ls, 3);
		return 0;
	}
	else if (!std::strcmp(key, "Color")) {
		dl.color = getdcfromcolor3(ls, 3);
		return 0;
	}
	else if (!std::strcmp(key, "Thickness")) {
		dl.thickness = RBX.ToNumber(3,0);
		return 0;
	}
	else if (!std::strcmp(key, "Transparency")) {
		dl.transparency = RBX.ToNumber(3,0);
		return 0;
	}
	return 0;
}

static int CircleRemove(DWORD ls) {
	Fluxus::RBX RBX(ls);

	DrawingCircle& db = *static_cast<DrawingCircle*>((void*)RBX.ToUserData(1));
	for (size_t idx = 0; idx < DrawCircle.size(); idx++) {
		if (DrawCircle[idx] == &db) {
			DrawCircle.erase(DrawCircle.begin() + idx);
		}
	}
	return 0;
}

static int CircleIndex(DWORD ls) {
	Fluxus::RBX RBX(ls);

	DrawingCircle& dc = *static_cast<DrawingCircle*>((void*)RBX.ToUserData(1));
	const char* key = RBX.Tolstring(2, 0);
	if (!std::strcmp(key, "Remove")) {
		RBX.PushCFunction(CircleRemove);
		return 1;
	}
	else if (!std::strcmp(key, "Visible")) {
		RBX.PushBoolean(dc.visible);
		return 1;
	}
	else if (!std::strcmp(key, "ZIndex")) {
		RBX.PushBoolean(dc.zindex);
		return 1;
	}
	else if (!std::strcmp(key, "Position")) {
		Vec2New(ls, dc.position);
		return 1;
	}
	else if (!std::strcmp(key, "Radius")) {
		RBX.PushNumber(dc.radius);
		return 1;
	}
	else if (!std::strcmp(key, "Color")) {
		Color3FromRGB(ls, dc.color);
		return 1;
	}
	else if (!std::strcmp(key, "Thickness")) {
		RBX.PushNumber(dc.thickness);
		return 1;
	}
	else if (!std::strcmp(key, "Filled")) {
		RBX.PushBoolean(dc.filled);
		return 1;
	}
	else if (!std::strcmp(key, "Transparency")) {
		RBX.PushNumber(dc.transparency);
		return 1;
	}
	else if (!std::strcmp(key, "NumSides")) {
		RBX.PushNumber(dc.numsides);
		return 1;
	}
	return 0;
}

static int CircleNewIndex(DWORD ls) {
	Fluxus::RBX RBX(ls);

	DrawingCircle& dc = *static_cast<DrawingCircle*>((void*)RBX.ToUserData(1));
	const char* key = RBX.Tolstring(2, 0);
	if (!std::strcmp(key, "Visible")) {
		dc.visible = RBX.ToBoolean(3);
	}
	else if (!std::strcmp(key, "ZIndex")) {
		dc.zindex = RBX.ToNumber(3,0);
	}
	else if (!std::strcmp(key, "Position")) {
		dc.position = GetDrawingVector(ls, 3);
	}
	else if (!std::strcmp(key, "Radius")) {
		dc.radius = RBX.ToNumber(3,0);
	}
	else if (!std::strcmp(key, "Color")) {
		dc.color = getdcfromcolor3(ls, 3);
	}
	else if (!std::strcmp(key, "Thickness")) {
		dc.thickness = RBX.ToNumber(3,0);
	}
	else if (!std::strcmp(key, "Filled")) {
		dc.filled = RBX.ToBoolean(3);
	}
	else if (!std::strcmp(key, "Transparency")) {
		dc.transparency = RBX.ToNumber(3,0);
	}
	else if (!std::strcmp(key, "NumSides")) {
		dc.numsides = RBX.ToNumber(3,0);
	}
	return 0;
}

/* DRAWING : SQUARE */

static int SquareRemove(DWORD ls) {
	Fluxus::RBX RBX(ls);

	DrawingSquare& db = *static_cast<DrawingSquare*>((void*)RBX.ToUserData(1));
	for (size_t idx = 0; idx < DrawSquare.size(); idx++) {
		if (DrawSquare[idx] == &db) {
			DrawSquare.erase(DrawSquare.begin() + idx);
		}
	}
	return 0;
}

static int SquareIndex(DWORD ls) {
	Fluxus::RBX RBX(ls);

	DrawingSquare& dc = *static_cast<DrawingSquare*>((void*)RBX.ToUserData(1));
	const char* key = RBX.Tolstring(2, 0);
	if (!std::strcmp(key, "Remove")) {
		RBX.PushCFunction(SquareRemove);
		return 1;
	}
	else if (!std::strcmp(key, "Visible")) {
		RBX.PushBoolean(dc.visible);
		return 1;
	}
	else if (!std::strcmp(key, "ZIndex")) {
		RBX.PushBoolean(dc.zindex);
		return 1;
	}
	else if (!std::strcmp(key, "Position")) {
		Vec2New(ls, dc.position);
		return 1;
	}
	else if (!std::strcmp(key, "Size")) {
		Vec2New(ls, dc.size);
		return 1;
	}
	else if (!std::strcmp(key, "Color")) {
		Color3FromRGB(ls, dc.color);
		return 1;
	}
	else if (!std::strcmp(key, "Thickness")) {
		RBX.PushNumber(dc.thickness);
		return 1;
	}
	else if (!std::strcmp(key, "Filled")) {
		RBX.PushBoolean(dc.filled);
		return 1;
	}
	else if (!std::strcmp(key, "Transparency")) {
		RBX.PushNumber(dc.transparency);
		return 1;
	}
	return 0;
}

static int SquareNewIndex(DWORD ls) {
	Fluxus::RBX RBX(ls);

	DrawingSquare& dc = *static_cast<DrawingSquare*>((void*)RBX.ToUserData(1));
	const char* key = RBX.Tolstring(2, 0);
	if (!std::strcmp(key, "Visible")) {
		dc.visible = RBX.ToBoolean(3);
	}
	else if (!std::strcmp(key, "ZIndex")) {
		dc.zindex = RBX.ToNumber(3,0);
	}
	else if (!std::strcmp(key, "Position")) {
		dc.position = GetDrawingVector(ls, 3);
	}
	else if (!std::strcmp(key, "Size")) {
		dc.size = GetDrawingVector(ls, 3);
	}
	else if (!std::strcmp(key, "Color")) {
		dc.color = getdcfromcolor3(ls, 3);
	}
	else if (!std::strcmp(key, "Thickness")) {
		dc.thickness = RBX.ToNumber(3,0);
	}
	else if (!std::strcmp(key, "Filled")) {
		dc.filled = RBX.ToBoolean(3);
	}
	else if (!std::strcmp(key, "Transparency")) {
		dc.transparency = RBX.ToNumber(3,0);
	}
	return 0;
}

static int TriangleRemove(DWORD ls) {
	Fluxus::RBX RBX(ls);

	DrawingTriangle& db = *static_cast<DrawingTriangle*>((void*)RBX.ToUserData(1));
	for (size_t idx = 0; idx < DrawTriangle.size(); idx++) {
		if (DrawTriangle[idx] == &db) {
			DrawTriangle.erase(DrawTriangle.begin() + idx);
		}
	}
	return 0;
}

static int TriangleIndex(DWORD ls) {
	Fluxus::RBX RBX(ls);

	DrawingTriangle& dc = *static_cast<DrawingTriangle*>((void*)RBX.ToUserData(1));
	const char* key = RBX.Tolstring(2, 0);
	if (!std::strcmp(key, "Remove")) {
		RBX.PushCFunction(TriangleRemove);
		return 1;
	}
	else if (!std::strcmp(key, "Visible")) {
		RBX.PushBoolean(dc.visible);
		return 1;
	}
	else if (!std::strcmp(key, "ZIndex")) {
		RBX.PushBoolean(dc.zindex);
		return 1;
	}
	else if (!std::strcmp(key, "PointA")) {
		Vec2New(ls, dc.pointa);
		return 1;
	}
	else if (!std::strcmp(key, "PointB")) {
		Vec2New(ls, dc.pointb);
		return 1;
	}
	else if (!std::strcmp(key, "PointC")) {
		Vec2New(ls, dc.pointc);
		return 1;
	}
	else if (!std::strcmp(key, "Color")) {
		Color3FromRGB(ls, dc.color);
		return 1;
	}
	else if (!std::strcmp(key, "Thickness")) {
		RBX.PushNumber(dc.thickness);
		return 1;
	}
	else if (!std::strcmp(key, "Filled")) {
		RBX.PushBoolean(dc.filled);
		return 1;
	}
	else if (!std::strcmp(key, "Transparency")) {
		RBX.PushNumber(dc.transparency);
		return 1;
	}
	return 0;
}

static int TriangleNewIndex(DWORD ls) {
	Fluxus::RBX RBX(ls);

	DrawingTriangle& dc = *static_cast<DrawingTriangle*>((void*)RBX.ToUserData(1));
	const char* key = RBX.Tolstring(2, 0);
	if (!std::strcmp(key, "Visible")) {
		dc.visible = RBX.ToBoolean(3);
	}
	else if (!std::strcmp(key, "ZIndex")) {
		dc.zindex = RBX.ToNumber(3,0);
	}
	else if (!std::strcmp(key, "PointA")) {
		dc.pointa = GetDrawingVector(ls, 3);
	}
	else if (!std::strcmp(key, "PointB")) {
		dc.pointb = GetDrawingVector(ls, 3);
	}
	else if (!std::strcmp(key, "PointC")) {
		dc.pointc = GetDrawingVector(ls, 3);
	}
	else if (!std::strcmp(key, "Color")) {
		dc.color = getdcfromcolor3(ls, 3);
	}
	else if (!std::strcmp(key, "Thickness")) {
		dc.thickness = RBX.ToNumber(3,0);
	}
	else if (!std::strcmp(key, "Filled")) {
		dc.filled = RBX.ToBoolean(3);
	}
	else if (!std::strcmp(key, "Transparency")) {
		dc.transparency = RBX.ToNumber(3,0);
	}
	return 0;
}


static int TextRemove(DWORD ls) {
	Fluxus::RBX RBX(ls);

	DrawingText& db = *static_cast<DrawingText*>((void*)RBX.ToUserData(1));
	for (size_t idx = 0; idx < DrawTexts.size(); idx++) {
		if (DrawTexts[idx] == &db) {
			DrawTexts.erase(DrawTexts.begin() + idx);
		}
	}
	return 0;
}

static int TextIndex(DWORD ls) {
	Fluxus::RBX RBX(ls);

	DrawingText& dc = *static_cast<DrawingText*>((void*)RBX.ToUserData(1));
	const char* key = RBX.Tolstring(2, 0);
	if (!std::strcmp(key, "Remove")) {
		RBX.PushCFunction(TextRemove);
		return 1;
	}
	else if (!std::strcmp(key, "Visible")) {
		RBX.PushBoolean(dc.visible);
		return 1;
	}
	else if (!std::strcmp(key, "ZIndex")) {
		RBX.PushBoolean(dc.zindex);
		return 1;
	}
	else if (!std::strcmp(key, "Text")) {
		RBX.Pushstring(dc.text);
		return 1;
	}
	else if (!std::strcmp(key, "Position")) {
		Vec2New(ls, dc.position.x);
		Vec2New(ls, dc.position.y);
		return 1;
	}
	else if (!std::strcmp(key, "Color")) {
		Color3FromRGB(ls, dc.color);
		return 1;
	}
	else if (!std::strcmp(key, "Center")) {
		RBX.PushBoolean(dc.center);
		return 1;
	}
	else if (!std::strcmp(key, "Outline")) {
		RBX.PushBoolean(dc.outline);
		return 1;
	}
	else if (!std::strcmp(key, "Transparency")) {
		RBX.PushNumber(dc.transparency);
		return 1;
	}
	else if (!std::strcmp(key, "TextBounds")) {
		Vec2New(ls, dc.textbounds.x);
		Vec2New(ls, dc.textbounds.y);
		return 1;
	}
	return 0;
}

static int TextNewIndex(DWORD ls) {
	Fluxus::RBX RBX(ls);

	DrawingText& dc = *static_cast<DrawingText*>((void*)RBX.ToUserData(1));
	const char* key = RBX.Tolstring(2, 0);
	if (!std::strcmp(key, "Visible")) {
		dc.visible = RBX.ToBoolean(3);
	}
	else if (!std::strcmp(key, "ZIndex")) {
		dc.zindex = RBX.ToNumber(3,0);
	}
	else if (!std::strcmp(key, "Text")) {
		unsigned int len;
		const char* text = RBX.Tolstring(3, &len);
		dc.text = text;
		dc.textend = text + len;
		dc.textlen = len;
	}
	else if (!std::strcmp(key, "Position")) {
		dc.position = GetDrawingVector(ls, 3);
	}
	else if (!std::strcmp(key, "Color")) {
		dc.color = getdcfromcolor3(ls, 3);
	}
	else if (!std::strcmp(key, "Center")) {
		dc.center = RBX.ToBoolean(3);
	}
	else if (!std::strcmp(key, "Outline")) {
		dc.outline = RBX.ToBoolean(3);
	}
	else if (!std::strcmp(key, "Transparency")) {
		dc.transparency = RBX.ToNumber(3,0);
	}
	else if (!std::strcmp(key, "Size")) {
		dc.size = RBX.ToNumber(3,0);
	}
	return 0;
}

int NewDrawing(DWORD ls) {
	Fluxus::RBX RBX(ls);

	const char* type = RBX.Tolstring(1, 0);
	void* Userdata = nullptr;
	if (std::string(type) == "Line") {
		Userdata = (void*)RBX.Idkwtf("drawingline", sizeof(DrawingLine), 0);
		new (Userdata) DrawingLine;
		DrawLine.push_back((DrawingLine*)Userdata);
	}
	else if (std::string(type) == "Circle") {
		Userdata = (void*)RBX.Idkwtf("drawingcircle", sizeof(DrawingCircle), 0);
		new (Userdata) DrawingCircle;
		DrawCircle.push_back((DrawingCircle*)Userdata);
	}
	else if (std::string(type) == "Square") {
		Userdata = (void*)RBX.Idkwtf("drawingsquare", sizeof(DrawingSquare), 0);
		new (Userdata) DrawingSquare;
		DrawSquare.push_back((DrawingSquare*)Userdata);
	}
	else if (std::string(type) == "Triangle") {
		Userdata = (void*)RBX.Idkwtf("drawingtriangle", sizeof(DrawingTriangle), 0);
		new (Userdata) DrawingTriangle;
		DrawTriangle.push_back((DrawingTriangle*)Userdata);
	}
	else if (std::string(type) == "Text") {
		Userdata = (void*)RBX.Idkwtf("drawingtext", sizeof(DrawingText), 0);
		new (Userdata) DrawingText;
		DrawTexts.push_back((DrawingText*)Userdata);
	}
	else {
		return 0;
	}
	return 1;
}

static int ClearDrawing(DWORD ls) {
	DrawLine.clear();
	DrawCircle.clear();
	DrawSquare.clear();
	DrawTriangle.clear();
	DrawTexts.clear();
	return 0;
}

std::vector<DrawingLine*> Drawing::GetLineDrawings()
{
	return DrawLine;
}

std::vector<DrawingCircle*> Drawing::GetCircleDrawings()
{
	return DrawCircle;
}

std::vector<DrawingSquare*> Drawing::GetSquareDrawings()
{
	return DrawSquare;
}

std::vector<DrawingTriangle*> Drawing::GetTriangleDrawings()
{
	return DrawTriangle;
}

std::vector<DrawingText*> Drawing::GetTextDrawings()
{
	return DrawTexts;
}

DrawingBase::~DrawingBase()
{
}

char pressedKeys[256] = {};

struct config {
	uint8_t openkey = VK_INSERT;
} Config;


ID3D11Device* p_device;
ID3D11DeviceContext* p_context;
ID3D11Texture2D* p_render_target_texture;
ID3D11RenderTargetView* p_render_target_view;

uintptr_t vt;
using present_fn = HRESULT(__stdcall*)(IDXGISwapChain* pSwapChain,
	UINT SyncInterval, UINT Flags);
present_fn o_present;
LONG o_wndproc;
HWND hWnd;



unsigned int s_w, s_h;
int logo_w, logo_h;
ID3D11ShaderResourceView* logo_texture;

bool guiactive = false;
bool ready = false;
bool imguienabled = false;

void handle_input(UINT msg, WPARAM w_param, LPARAM l_param);
LRESULT IMGUI_IMPL_API wndproc(HWND hWnd, UINT msg, WPARAM w_param, LPARAM l_param);
HRESULT WINAPI present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void render() {

	auto imguilist = ImGui::GetBackgroundDrawList();

	for (DrawingLine* dl : Drawing::GetLineDrawings()) {
		if (!dl->visible) continue;

		imguilist->AddLine({ dl->from.x, dl->from.y }, { dl->to.x, dl->to.y }, ImColor(dl->color.r, dl->color.g, dl->color.b, dl->transparency), dl->thickness);
	}
	for (DrawingCircle* dc : Drawing::GetCircleDrawings()) {
		if (!dc->visible) continue;

		if (dc->filled) {
			imguilist->AddCircleFilled({ dc->position.x, dc->position.y }, dc->radius, ImColor(dc->color.r, dc->color.g, dc->color.b, dc->transparency), dc->numsides);
		}
		else {
			imguilist->AddCircle({ dc->position.x, dc->position.y }, dc->radius, ImColor(dc->color.r, dc->color.g, dc->color.b, dc->transparency), dc->numsides, dc->thickness);
		}
	}
	for (DrawingSquare* ds : Drawing::GetSquareDrawings()) {
		if (!ds->visible) continue;

		if (ds->filled) {
			imguilist->AddRectFilled({ ds->position.x, ds->position.y }, { ds->position.x + ds->size.x, ds->position.y + ds->size.y }, ImColor(ds->color.r, ds->color.g, ds->color.b, ds->transparency));
		}
		else {
			imguilist->AddRect({ ds->position.x, ds->position.y }, { ds->position.x + ds->size.x, ds->position.y + ds->size.y }, ImColor(ds->color.r, ds->color.g, ds->color.b, ds->transparency), 0, 15, ds->thickness);
		}
	}
	for (DrawingTriangle* dt : Drawing::GetTriangleDrawings()) {
		if (!dt->visible) continue;

		if (dt->filled) {
			imguilist->AddTriangleFilled({ dt->pointa.x, dt->pointa.y }, { dt->pointb.x, dt->pointb.y }, { dt->pointc.x, dt->pointc.y }, ImColor(dt->color.r, dt->color.g, dt->color.b, dt->transparency));
		}
		else {
			imguilist->AddTriangle({ dt->pointa.x, dt->pointa.y }, { dt->pointb.x, dt->pointb.y }, { dt->pointc.x, dt->pointc.y }, ImColor(dt->color.r, dt->color.g, dt->color.b, dt->transparency), dt->thickness);
		}
	}
	for (DrawingText* dt : Drawing::GetTextDrawings()) {
		if (!dt->visible) continue;


		ImVec2 TextSize = ImGui::CalcTextSize(dt->text, dt->textend);
		dt->textbounds.x = TextSize.x;
		dt->textbounds.y = TextSize.y;

		if (!dt->center)
			imguilist->AddText(ImGui::GetFont(), ImGui::GetFontSize(), { dt->position.x , dt->position.y }, ImColor(dt->color.r, dt->color.g, dt->color.b, dt->transparency), dt->text, dt->textend);
		else
			imguilist->AddText(ImGui::GetFont(), ImGui::GetFontSize(), { dt->position.x - (TextSize.x / 2), dt->position.y - (TextSize.y / 2) }, ImColor(dt->color.r, dt->color.g, dt->color.b, dt->transparency), dt->text, dt->textend);
	}
}

bool DrawCursor = false;

HRESULT WINAPI present(IDXGISwapChain* pSwapChain, UINT SyncInterval,
	UINT Flags)
{
	if (!ready) {
		ready = true;
		pSwapChain->GetDevice(__uuidof(ID3D11Device),
			(void**)(&p_device));
		p_device->GetImmediateContext(&p_context);
		ImGui::CreateContext();
		ImGui::GetIO().Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\corbel.ttf"), 16.0f);

		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX11_Init(p_device, p_context);
		ImGui_ImplDX11_CreateDeviceObjects();
		ImGui::GetIO().MouseDrawCursor = DrawCursor;

		ImGui::StyleColorsDark();

		ImGui::GetIO().IniFilename = 0;
	}

	ID3D11Texture2D* pBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	p_device->CreateRenderTargetView(pBackBuffer, NULL, &p_render_target_view);
	pBackBuffer->Release();

	ImGui_ImplDX11_Init(p_device, p_context);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	render();

	ImGui::Render();

	p_context->OMSetRenderTargets(1, &p_render_target_view, nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (p_render_target_view) { p_render_target_view->Release(); p_render_target_view = nullptr; }

	return o_present(pSwapChain, SyncInterval, Flags);
}

bool robloxhasfocus = true;

LRESULT IMGUI_IMPL_API wndproc(HWND hWnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
	if (ready) {
		ImGui_ImplWin32_WndProcHandler(hWnd, msg, w_param, l_param);

		switch (msg) {
		case WM_ACTIVATE:
			if (w_param == WA_ACTIVE || w_param == WA_CLICKACTIVE) {
				robloxhasfocus = true;
			}
			else if (w_param == WA_INACTIVE) {
				robloxhasfocus = false;
			}
			break;
		case WM_SIZE:
			if (p_render_target_view) { p_render_target_view->Release(); p_render_target_view = nullptr; }
			RECT rc;
			GetWindowRect(hWnd, &rc);
			s_w = rc.right - rc.left;
			s_h = rc.bottom - rc.top;
			break;
		case WM_LBUTTONDOWN:
			pressedKeys[VK_LBUTTON] = true;
			break;
		case WM_LBUTTONUP:
			pressedKeys[VK_LBUTTON] = false;
			break;
		case WM_RBUTTONDOWN:
			pressedKeys[VK_RBUTTON] = true;
			break;
		case WM_RBUTTONUP:
			pressedKeys[VK_RBUTTON] = false;
			break;
		case WM_MBUTTONDOWN:
			pressedKeys[VK_MBUTTON] = true;
			break;
		case WM_MBUTTONUP:
			pressedKeys[VK_MBUTTON] = false;
			break;
		case WM_XBUTTONDOWN:
		{
			UINT button = GET_XBUTTON_WPARAM(w_param);
			if (button == XBUTTON1)
			{
				pressedKeys[VK_XBUTTON1] = true;
			}
			else if (button == XBUTTON2)
			{
				pressedKeys[VK_XBUTTON2] = true;
			}
			break;
		}
		case WM_XBUTTONUP:
		{
			UINT button = GET_XBUTTON_WPARAM(w_param);
			if (button == XBUTTON1)
			{
				pressedKeys[VK_XBUTTON1] = false;
			}
			else if (button == XBUTTON2)
			{
				pressedKeys[VK_XBUTTON2] = false;
			}
			break;
		}
		case WM_KEYDOWN:
			pressedKeys[w_param] = true;
			break;
		case WM_KEYUP:
			pressedKeys[w_param] = false;
			break;
		default: break;
		}

		handle_input(msg, w_param, l_param);
	}

	if (msg == WM_CLOSE) exit(0);

	if (guiactive) {
		return true;
	}

	return CallWindowProc(reinterpret_cast<WNDPROC>(o_wndproc), hWnd,
		msg, w_param, l_param);
}

void handle_input(UINT msg, WPARAM w_param, LPARAM l_param)
{
	if (!imguienabled) return;

	if (msg == WM_KEYUP && w_param == Config.openkey)
		guiactive = !guiactive;
	ImGui::GetIO().MouseDrawCursor = DrawCursor;
}

bool RobloxHasFocus()
{
	return robloxhasfocus;
}

namespace Fluxus {
	namespace Drawing {
		void RegisterDrawing(DWORD ls) {
			Fluxus::RBX RBX(ls);

			/* DRAWING : LINE */
			RBX.NewMetaTable("drawingline");

			RBX.PushCFunction(LineIndex);
			RBX.Setfield(-2, "__index");

			RBX.PushCFunction(LineNewIndex);
			RBX.Setfield(-2, "__newindex");

			RBX.PushCFunction(LineRemove);
			RBX.Setfield(-2, "__gc");

			RBX.Pop(1);

			/* DRAWING : CIRCLE */
			RBX.NewMetaTable("drawingcircle");

			RBX.PushCFunction(CircleIndex);
			RBX.Setfield(-2, "__index");

			RBX.PushCFunction(CircleNewIndex);
			RBX.Setfield(-2, "__newindex");

			RBX.PushCFunction(CircleRemove);
			RBX.Setfield(-2, "__gc");

			RBX.Pop(1);

			/* DRAWING : SQUARE */
			RBX.NewMetaTable("drawingsquare");

			RBX.PushCFunction(SquareIndex);
			RBX.Setfield(-2, "__index");

			RBX.PushCFunction(SquareNewIndex);
			RBX.Setfield(-2, "__newindex");

			RBX.PushCFunction(SquareRemove);
			RBX.Setfield(-2, "__gc");

			RBX.Pop(1);

			/* DRAWING : TRIANGLE */
			RBX.NewMetaTable("drawingtriangle");

			RBX.PushCFunction(TriangleIndex);
			RBX.Setfield(-2, "__index");

			RBX.PushCFunction(TriangleNewIndex);
			RBX.Setfield(-2, "__newindex");

			RBX.PushCFunction(TriangleRemove);
			RBX.Setfield(-2, "__gc");

			RBX.Pop(1);

			/* DRAWING : TEXT */
			RBX.NewMetaTable("drawingtext");

			RBX.PushCFunction(TextIndex);
			RBX.Setfield(-2, "__index");

			RBX.PushCFunction(TextNewIndex);
			RBX.Setfield(-2, "__newindex");

			RBX.PushCFunction(TextRemove);
			RBX.Setfield(-2, "__gc");

			RBX.Pop(1);

			RBX.CreateTable(0, 0);
			{
				RBX.Pushstring("new");
				RBX.PushCFunction(NewDrawing);
				RBX.Settable(-3);

				RBX.Pushstring("clear");
				RBX.PushCFunction(ClearDrawing);
				RBX.Settable(-3);

				RBX.Pushstring("Clear");
				RBX.PushCFunction(ClearDrawing);
				RBX.Settable(-3);
			}
			RBX.SetGlobal("Drawing");
		}

		HWND GetRobloxHWND()
		{
			return hWnd;
		}

		void HookRenderer() {
			hWnd = FindWindowW(NULL, L"ROBLOX");

			DXGI_SWAP_CHAIN_DESC scd = { 0 };
			ZeroMemory(&scd, sizeof scd);
			scd.BufferCount = 1;
			scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			scd.OutputWindow = hWnd;
			scd.SampleDesc.Count = 1;
			scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			scd.Windowed = TRUE;
			D3D_FEATURE_LEVEL requestedLevels[] = { D3D_FEATURE_LEVEL_11_0 };

			IDXGISwapChain* swap = nullptr;
			ID3D11Device* device;
			ID3D11DeviceContext* context;
			HRESULT result = D3D11CreateDeviceAndSwapChain(
				nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, requestedLevels, 1,
				D3D11_SDK_VERSION, &scd, &swap, &device, nullptr, &context);

			if (FAILED(result))
				return;

			vt = *reinterpret_cast<uintptr_t*>(swap);
			o_present = *reinterpret_cast<present_fn*>(vt + 32);

			unsigned long o_proc;
			if (!VirtualProtect((LPVOID)vt, 64, PAGE_READWRITE, &o_proc))
				return;
			*reinterpret_cast<void**>(vt + 32) = &present;

			VirtualProtect((LPVOID)vt, 64, o_proc, &o_proc);

			o_wndproc = GetWindowLong(hWnd, GWLP_WNDPROC);
			SetWindowLong(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG>(wndproc));

			RECT rc;
			GetWindowRect(hWnd, &rc);
			s_w = rc.right - rc.left;
			s_h = rc.bottom - rc.top;

			device->Release();
			context->Release();
			swap->Release();
		}
	}
}
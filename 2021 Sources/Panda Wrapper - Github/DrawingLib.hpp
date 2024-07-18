//#pragma once
//
//#include "ImGUI/imgui.h"
//#include "ImGUI/imgui_internal.h"
//#include "ImGUI/imgui_impl_dx11.h"
//#include <functional>
//#include "Rlua.h"
//
//
//enum D3DTypes
//{
//	D3_LINE,
//	D3_TEXT,
//	D3_SQUARE,
//	D3_CIRCLE,
//};
//
//struct D3DHeader
//{
//	BYTE Visible;
//	D3DTypes Type;
//};
//
//struct D3DLine
//{
//	ImVec2 From;
//	ImVec2 To;
//	ImU32 Color{};
//	float Thickness{};
//};
//
//struct D3DSquare
//{
//	ImVec2 Pos;
//	ImVec2 Size;
//	ImU32 Color{};
//	float Thickness{};
//	BYTE Filled{};
//};
//
//struct D3DCircle
//{
//	ImVec2 Pos;
//	float Radius{};
//	ImU32 Color{};
//	float Thickness{};
//	BYTE Filled{};
//	DWORD Sides{};
//};
//
//struct D3DText
//{
//	std::string* Text{};
//	ImVec2 Pos;
//	float Size{};
//	ImU32 Font{};
//	ImU32 Color{};
//	BYTE Center{};
//	BYTE Outline{};
//	ImU32 OutlineColor{};
//};
//
//struct D3DObject
//{
//	D3DHeader Header;
//	DWORD* Ptr;
//};
//std::vector<ImFont*> Fonts;
//std::vector<D3DObject*> RenderChain;
//ImFont* GetFont(int idx)
//{
//	return Fonts.at(idx);
//}
//void AddToRenderList(D3DObject* Object)
//{
//	RenderChain.push_back(Object);
//}
//void RemoveFromRenderList(D3DObject* Object)
//{
//	RenderChain.erase(std::remove(RenderChain.begin(), RenderChain.end(), Object), RenderChain.end());
//}
//namespace Drawing {
//
//	int createrenderobject(lua_State* L)
//	{
//
//
//		const std::string Type = luaL_checkstring(L, 1);
//
//		/* syn::D3D::Initialize checks if its already init'd, we don't need to add a check here */
//
//
//		if (Type == "Line")
//		{
//			auto Line = new D3DLine();
//			Line->Color = ImGui::GetColorU32(ImVec4(0, 0, 0, 255));
//			Line->From = ImVec2(0, 0);
//			Line->To = ImVec2(0, 0);
//			Line->Thickness = 0;
//
//			auto Obj = new D3DObject();
//			Obj->Header.Type = D3_LINE;
//			Obj->Header.Visible = FALSE;
//			Obj->Ptr = (DWORD*)Line;
//
//			AddToRenderList(Obj);
//
//			rlua_pushlightuserdata(luaState, Obj);
//
//			return 1;
//		}
//
//		if (Type == "Text")
//		{
//			auto Text = new D3DText();
//			Text->Color = ImGui::GetColorU32(ImVec4(0, 0, 0, 255));
//			Text->OutlineColor = ImGui::GetColorU32(ImVec4(0, 0, 0, 255));
//			Text->Center = FALSE;
//			Text->Outline = FALSE;
//			Text->Pos = ImVec2(0, 0);
//			Text->Size = 16;
//			Text->Font = 0;
//			Text->Text = new std::string("");
//
//			auto Obj = new D3DObject();
//			Obj->Header.Type = D3_TEXT;
//			Obj->Header.Visible = FALSE;
//			Obj->Ptr = (DWORD*)Text;
//
//			AddToRenderList(Obj);
//
//			rlua_pushlightuserdata(luaState, Obj);
//
//			return 1;
//		}
//
//		if (Type == "Square")
//		{
//			auto Square = new D3DSquare();
//			Square->Color = ImGui::GetColorU32(ImVec4(0, 0, 0, 255));
//			Square->Filled = FALSE;
//			Square->Thickness = 16;
//			Square->Pos = ImVec2(0, 0);
//			Square->Size = ImVec2(16, 16);
//
//			auto Obj = new D3DObject();
//			Obj->Header.Type = D3_SQUARE;
//			Obj->Header.Visible = FALSE;
//			Obj->Ptr = (DWORD*)Square;
//
//			AddToRenderList(Obj);
//
//			rlua_pushlightuserdata(luaState, Obj);
//
//			return 1;
//		}
//
//		if (Type == "Circle")
//		{
//			auto Circle = new D3DCircle();
//			Circle->Color = ImGui::GetColorU32(ImVec4(0, 0, 0, 255));
//			Circle->Filled = FALSE;
//			Circle->Thickness = 16;
//			Circle->Pos = ImVec2(0, 0);
//			Circle->Radius = 1;
//			Circle->Sides = 100;
//
//			auto Obj = new D3DObject();
//			Obj->Header.Type = D3_CIRCLE;
//			Obj->Header.Visible = FALSE;
//			Obj->Ptr = (DWORD*)Circle;
//
//			AddToRenderList(Obj);
//
//			rlua_pushlightuserdata(luaState, Obj);
//
//			return 1;
//		}
//
//		return 1;
//	}
//
//	int setrenderproperty(lua_State* L)
//	{
//
//		const std::string Property = luaL_checkstring(L, 2);
//		luaL_checkany(L, 3);
//
//		const auto Obj = (D3DObject*)lua_touserdata(L, 1);
//
//		const std::function<ImVec2()> GetVec2 = [=]
//		{
//			rlua_getfield(luaState, 3, "X");
//			const float X = (float)rlua_tonumber(luaState, -1);
//			rlua_settop(luaState, -(1) - 1);
//			rlua_getfield(luaState, 3, "Y");
//			const float Y = (float)rlua_tonumber(luaState, -1);
//			rlua_settop(luaState, -(1) - 1);
//
//			return ImVec2(X, Y);
//		};
//
//		const std::function<ImU32(ImU32)> GetColor = [=](ImU32 Old)
//		{
//			rlua_getfield(luaState, 3, "r");
//			const float R = (float)rlua_tonumber(luaState, -1) * 255;
//			rlua_settop(luaState, -(1) - 1);
//			rlua_getfield(luaState, 3, "g");
//			const float G = (float)rlua_tonumber(luaState, -1) * 255;
//			rlua_settop(luaState, -(1) - 1);
//			rlua_getfield(luaState, 3, "b");
//			const float B = (float)rlua_tonumber(luaState, -1) * 255;
//			rlua_settop(luaState, -(1) - 1);
//
//			return ImGui::GetColorU32(ImVec4(R / 255, G / 255, B / 255, ImGui::ColorConvertU32ToFloat4(Old).w));
//		};
//
//		if (Property == "Visible")
//		{
//			Obj->Header.Visible = rlua_toboolean(luaState, 3);
//
//			return 0;
//		}
//
//		switch (Obj->Header.Type)
//		{
//		case D3_LINE:
//		{
//			auto Line = (D3DLine*)Obj->Ptr;
//
//			if (Property == "From")
//			{
//				Line->From = GetVec2();
//				return 0;
//			}
//
//			if (Property == "To")
//			{
//				Line->To = GetVec2();
//				return 0;
//			}
//
//			if (Property == "Color")
//			{
//				Line->Color = GetColor(Line->Color);
//				return 0;
//			}
//
//			if (Property == "Thickness")
//			{
//				Line->Thickness = (float)rlua_tonumber(luaState, 3);
//				return 0;
//			}
//
//			if (Property == "Transparency")
//			{
//				auto Col = ImGui::ColorConvertU32ToFloat4(Line->Color);
//				Col.w = (float)rlua_tonumber(luaState, 3);
//				Line->Color = ImGui::GetColorU32(Col);
//				return 0;
//			}
//
//			return 1;
//		}
//
//		case D3_TEXT:
//		{
//			auto Text = (D3DText*)Obj->Ptr;
//
//			if (Property == "Text")
//			{
//				size_t TextSize;
//				const auto TextCStr = rlua_tolstring(luaState, 3, &TextSize);
//
//				delete Text->Text;
//				Text->Text = new std::string(TextCStr, TextSize);
//				return 0;
//			}
//
//			if (Property == "Position")
//			{
//				Text->Pos = GetVec2();
//				return 0;
//			}
//
//			if (Property == "Size")
//			{
//				Text->Size = (float)rlua_tonumber(luaState, 3);
//				return 0;
//			}
//
//			if (Property == "Font")
//			{
//				Text->Font = rlua_tonumber(luaState, 3);
//				return 0;
//			}
//
//			if (Property == "Color")
//			{
//				Text->Color = GetColor(Text->Color);
//				return 0;
//			}
//
//			if (Property == "Center")
//			{
//				Text->Center = rlua_toboolean(luaState, 3);
//				return 0;
//			}
//
//			if (Property == "Outline")
//			{
//				Text->Outline = rlua_toboolean(luaState, 3);
//				return 0;
//			}
//
//			if (Property == "OutlineColor")
//			{
//				Text->OutlineColor = GetColor(Text->OutlineColor);
//				return 0;
//			}
//
//			if (Property == "Transparency")
//			{
//				ImVec4 Col = ImGui::ColorConvertU32ToFloat4(Text->Color);
//				ImVec4 ColOutline = ImGui::ColorConvertU32ToFloat4(Text->OutlineColor);
//				float Amt = (float)rlua_tonumber(luaState, 3);
//
//				Col.w = Amt;
//				ColOutline.w = Amt;
//				Text->Color = ImGui::GetColorU32(Col);
//				Text->OutlineColor = ImGui::GetColorU32(ColOutline);
//
//				return 0;
//			}
//
//			if (Property == "TextBounds")
//			{
//				return 1;
//			}
//
//			return 1;
//		}
//
//		case D3_SQUARE:
//		{
//			auto Square = (D3DSquare*)Obj->Ptr;
//
//			if (Property == "Position")
//			{
//				Square->Pos = GetVec2();
//				return 0;
//			}
//
//			if (Property == "Size")
//			{
//				Square->Size = GetVec2();
//				return 0;
//			}
//
//			if (Property == "Color")
//			{
//				Square->Color = GetColor(Square->Color);
//				return 0;
//			}
//
//			if (Property == "Thickness")
//			{
//				Square->Thickness = (float)rlua_tonumber(luaState, 3);
//				return 0;
//			}
//
//			if (Property == "Filled")
//			{
//				Square->Filled = rlua_toboolean(luaState, 3);
//				return 0;
//			}
//
//			if (Property == "Transparency")
//			{
//				auto Col = ImGui::ColorConvertU32ToFloat4(Square->Color);
//				Col.w = (float)rlua_tonumber(luaState, 3);
//				Square->Color = ImGui::GetColorU32(Col);
//				return 0;
//			}
//
//			return 1;
//		}
//
//		case D3_CIRCLE:
//		{
//			auto Circle = (D3DCircle*)Obj->Ptr;
//
//			if (Property == "Position")
//			{
//				Circle->Pos = GetVec2();
//				return 0;
//			}
//
//			if (Property == "Radius")
//			{
//				Circle->Radius = (float)rlua_tonumber(luaState, 3);
//				return 0;
//			}
//
//			if (Property == "Color")
//			{
//				Circle->Color = GetColor(Circle->Color);
//				return 0;
//			}
//
//			if (Property == "Thickness")
//			{
//				Circle->Thickness = (float)rlua_tonumber(luaState, 3);
//				return 0;
//			}
//
//			if (Property == "Filled")
//			{
//				Circle->Filled = rlua_toboolean(luaState, 3);
//				return 0;
//			}
//
//			if (Property == "Transparency")
//			{
//				auto Col = ImGui::ColorConvertU32ToFloat4(Circle->Color);
//				Col.w = (float)rlua_tonumber(luaState, 3);
//				Circle->Color = ImGui::GetColorU32(Col);
//				return 0;
//			}
//
//			if (Property == "NumSides")
//			{
//				Circle->Sides = rlua_tonumber(luaState, 3);
//				return 0;
//			}
//
//			return 1;
//		}
//		}
//
//		return 1;
//	}
//
//	int getrenderproperty(lua_State* L)
//	{
//
//
//		const std::string Property = luaL_checkstring(L, 2);
//
//		const auto Obj = (D3DObject*)lua_touserdata(L, 1);
//
//		const std::function<int(ImVec2)> PushVec2 = [=](ImVec2 Vec)
//		{
//			rlua_getfield(luaState, LUA_GLOBALSINDEX, "Vector2");
//			rlua_getfield(luaState, -1, "new");
//			rlua_pushnumber(luaState, Vec.x);
//			rlua_pushnumber(luaState, Vec.y);
//			rlua_pcall(luaState, 2, 1, 0);
//
//			return 1;
//		};
//
//		const std::function<int(ImU32)> PushColor = [=](ImU32 Col)
//		{
//			const auto ColObj = ImGui::ColorConvertU32ToFloat4(Col);
//
//			rlua_getfield(luaState, LUA_GLOBALSINDEX, "Color3");
//			rlua_getfield(luaState, -1, "fromRGB");
//			rlua_pushnumber(luaState, ColObj.x);
//			rlua_pushnumber(luaState, ColObj.y);
//			rlua_pushnumber(luaState, ColObj.z);
//			rlua_pcall(luaState, 3, 1, 0);
//
//			return 1;
//		};
//
//		if (Property == "Visible")
//		{
//			rlua_pushboolean(luaState, Obj->Header.Visible);
//			return 1;
//		}
//
//		switch (Obj->Header.Type)
//		{
//		case D3_LINE:
//		{
//			const auto Line = (D3DLine*)Obj->Ptr;
//
//			if (Property == "From")
//			{
//				return PushVec2(Line->From);
//			}
//
//			if (Property == "To")
//			{
//				return PushVec2(Line->To);
//			}
//
//			if (Property == "Color")
//			{
//				return PushColor(Line->Color);
//			}
//
//			if (Property == "Thickness")
//			{
//				rlua_pushnumber(luaState, Line->Thickness);
//				return 1;
//			}
//
//			if (Property == "Transparency")
//			{
//				const auto Col = ImGui::ColorConvertU32ToFloat4(Line->Color);
//				rlua_pushnumber(luaState, Col.w);
//				return 1;
//			}
//
//			throw std::exception("invalid property for line");
//		}
//
//		case D3_TEXT:
//		{
//			const auto Text = (D3DText*)Obj->Ptr;
//
//			if (Property == "Text")
//			{
//				rlua_pushstring(luaState, Text->Text->c_str());
//				return 1;
//			}
//
//			if (Property == "Position")
//			{
//				return PushVec2(Text->Pos);
//			}
//
//			if (Property == "Size")
//			{
//				rlua_pushnumber(luaState, Text->Size);
//				return 1;
//			}
//
//			if (Property == "Font")
//			{
//				rlua_pushnumber(luaState, Text->Font);
//				return 1;
//			}
//
//			if (Property == "Color")
//			{
//				return PushColor(Text->Color);
//			}
//
//			if (Property == "Center")
//			{
//				rlua_pushboolean(luaState, Text->Center);
//				return 1;
//			}
//
//			if (Property == "Outline")
//			{
//				rlua_pushboolean(luaState, Text->Outline);
//				return 1;
//			}
//
//			if (Property == "OutlineColor")
//			{
//				return PushColor(Text->OutlineColor);
//			}
//
//			if (Property == "Transparency")
//			{
//				const auto Col = ImGui::ColorConvertU32ToFloat4(Text->Color);
//				rlua_pushnumber(luaState, Col.w);
//				return 1;
//			}
//
//			if (Property == "TextBounds")
//			{
//				const auto TextSize = GetFont(Text->Font)->CalcTextSizeA(
//					Text->Size, FLT_MAX, 0.0f, Text->Text->c_str());
//				return PushVec2(TextSize);
//			}
//
//			return 1;
//		}
//
//		case D3_SQUARE:
//		{
//			const auto Square = (D3DSquare*)Obj->Ptr;
//
//			if (Property == "Position")
//			{
//				return PushVec2(Square->Pos);
//			}
//
//			if (Property == "Size")
//			{
//				return PushVec2(Square->Size);
//			}
//
//			if (Property == "Color")
//			{
//				return PushColor(Square->Color);
//			}
//
//			if (Property == "Thickness")
//			{
//				rlua_pushnumber(luaState, Square->Thickness);
//				return 1;
//			}
//
//			if (Property == "Filled")
//			{
//				rlua_pushboolean(luaState, Square->Filled);
//				return 1;
//			}
//
//			if (Property == "Transparency")
//			{
//				const auto Col = ImGui::ColorConvertU32ToFloat4(Square->Color);
//				rlua_pushnumber(luaState, Col.w);
//				return 1;
//			}
//
//			return 1;
//		}
//
//		case D3_CIRCLE:
//		{
//			const auto Circle = (D3DCircle*)Obj->Ptr;
//
//			if (Property == "Position")
//			{
//				return PushVec2(Circle->Pos);
//			}
//
//			if (Property == "Radius")
//			{
//				rlua_pushnumber(luaState, Circle->Radius);
//				return 1;
//			}
//
//			if (Property == "Color")
//			{
//				return PushColor(Circle->Color);
//			}
//
//			if (Property == "Thickness")
//			{
//				rlua_pushnumber(luaState, Circle->Thickness);
//				return 1;
//			}
//
//			if (Property == "Filled")
//			{
//				rlua_pushboolean(luaState, Circle->Filled);
//				return 1;
//			}
//
//			if (Property == "Transparency")
//			{
//				const auto Col = ImGui::ColorConvertU32ToFloat4(Circle->Color);
//				rlua_pushnumber(luaState, Col.w);
//				return 1;
//			}
//
//			if (Property == "NumSides")
//			{
//				rlua_pushnumber(luaState, Circle->Sides);
//				return 1;
//			}
//
//			return 1;
//		}
//		}
//
//		return 1;
//	}
//
//	int destroyrenderobject(lua_State* L)
//	{
//
//
//		/* TODO: Check if its an actual render object */
//
//
//		const auto Obj = (D3DObject*)lua_touserdata(L, 1);
//
//
//
//		RemoveFromRenderList(Obj);
//
//		switch (Obj->Header.Type)
//		{
//		case D3_LINE:
//		{
//			delete (D3DLine*)Obj->Ptr;
//			break;
//		}
//		case D3_TEXT:
//		{
//			const auto Text = (D3DText*)Obj->Ptr;
//			delete Text->Text;
//			delete Text;
//			break;
//		}
//		case D3_CIRCLE:
//		{
//			delete (D3DCircle*)Obj->Ptr;
//			break;
//		}
//		case D3_SQUARE:
//		{
//			delete (D3DSquare*)Obj->Ptr;
//			break;
//		}
//		}
//
//		delete Obj;
//
//		return 0;
//	}
//
//
//	void InitDrawing(lua_State* L) {
//		lua_createtable(L, 0, 0);
//		{
//			lua_pushcfunction(L, createrenderobject);
//			lua_setfield(L, -2, "new");
//
//			lua_pushcfunction(L, setrenderproperty);
//			lua_setfield(L, -2, "new");
//
//			lua_pushcfunction(L, getrenderproperty);
//			lua_setfield(L, -2, "new");
//
//			lua_pushcfunction(L, destroyrenderobject);
//			lua_setfield(L, -2, "clear");
//		}
//		lua_register(L, "createrenderobject", createrenderobject);
//		lua_register(L, "setrenderproperty", setrenderproperty);
//		lua_register(L, "getrenderproperty", getrenderproperty);
//		lua_register(L, "destroyrenderobject", destroyrenderobject);
//	}
//}
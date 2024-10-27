#include "drawing.hpp"

#include <Luau/lstate.h>
#include <Luau/lua.h>
#include <Luau/lapi.h>
#include <Luau/lualib.h>
#include <Luau/lgc.h>

#include <managers/managers.hpp>

static std::unordered_map< roblox::drawing::base_t*, int > key_map;

inline void make_vector2(lua_State* state, ImVec2 vec)
{
	lua_getglobal(state, "Vector2");
	lua_getfield(state, -1, "new");

	lua_pushnumber(state, vec.x);
	lua_pushnumber(state, vec.y);

	lua_pcall(state, 2, 1, 0);
}

inline void make_color3(lua_State* state, ImColor col)
{
	lua_getglobal(state, "Color3");
	lua_getfield(state, -1, "new");

	const auto [r, g, b, a] = col.Value;

	lua_pushnumber(state, r);
	lua_pushnumber(state, g);
	lua_pushnumber(state, b);

	lua_pcall(state, 3, 1, 0);
}

void roblox::drawing::triangle_t::draw()
{
	auto dl = ImGui::GetBackgroundDrawList();

	auto dd = managers::renderer_mgr.get_display_density();
	const auto point_a = this->point_a * dd;
	const auto point_b = this->point_b * dd;
	const auto point_c = this->point_c * dd;

	if (!this->visible) return;

	if (this->filled) {
		dl->AddTriangleFilled(point_a, point_b, point_c,
			ImColor(this->color.Value.x, this->color.Value.y, this->color.Value.z, this->transparency));
	}
	else {
		dl->AddTriangle(point_a, point_b, point_c,
			ImColor(this->color.Value.x, this->color.Value.y, this->color.Value.z, this->transparency), this->thickness);
	}
}

void roblox::drawing::quad_t::draw()
{
	auto dl = ImGui::GetBackgroundDrawList();

	auto dd = managers::renderer_mgr.get_display_density();
	const auto point_a = this->point_a * dd;
	const auto point_b = this->point_b * dd;
	const auto point_c = this->point_c * dd;
	const auto point_d = this->point_d * dd;

	if (!this->visible) return;

	if (this->filled) {
		dl->AddQuadFilled(point_a, point_b, point_c, point_d,
			ImColor(this->color.Value.x, this->color.Value.y, this->color.Value.z, this->transparency));
	}
	else {
		dl->AddQuad(point_a, point_b, point_c, point_d,
			ImColor(this->color.Value.x, this->color.Value.y, this->color.Value.z, this->transparency), this->thickness);
	}
}

void roblox::drawing::square_t::draw()
{
	auto dl = ImGui::GetBackgroundDrawList();

	auto dd = managers::renderer_mgr.get_display_density();

	const auto pos = this->position * dd;

	if (!this->visible) return;

	if (this->filled) {
		dl->AddRectFilled(
			pos, pos + size * dd, ImColor(this->color.Value.x, this->color.Value.y, this->color.Value.z, this->transparency)
		);
	}
	else {
		dl->AddRect(
			pos, pos + size * dd, ImColor(this->color.Value.x, this->color.Value.y, this->color.Value.z, this->transparency)
			, this->thickness);
	}
}

void roblox::drawing::circle_t::draw()
{
	auto dl = ImGui::GetBackgroundDrawList();

	auto dd = managers::renderer_mgr.get_display_density();

	const auto pos = this->position * dd;

	if (!this->visible) return;

	if (this->filled) {
		dl->AddCircleFilled(
			{ pos.x, pos.y }, this->radius, ImColor(this->color.Value.x, this->color.Value.y, this->color.Value.z, this->transparency)
			, this->numsides);
	}
	else {
		dl->AddCircle(
			{ pos.x, pos.y }, this->radius, ImColor(this->color.Value.x, this->color.Value.y, this->color.Value.z, this->transparency)
			, this->numsides, this->thickness);
	}
}

void roblox::drawing::image_t::draw()
{
	auto dl = ImGui::GetBackgroundDrawList( );
	auto dd = managers::renderer_mgr.get_display_density( );

	if ( !this->visible ) return;

	dl->AddImageRounded( reinterpret_cast< void* >( texture ), { this->position.x * dd, this->position.y * dd },
		{ ( this->position.x + this->size.x ) * dd, ( this->position.y + this->size.y ) * dd }, { 0, 0 }, { 1, 1 },
		ImColor( this->color.Value.x, this->color.Value.y, this->color.Value.z, this->transparency ), this->rounding );
}

ImVec2 custom_calc_text_sz(const char* text, float sz, const char* text_end = NULL, bool hide_text_after_double_hash = false, float wrap_width = -1.0f)
{
	ImGuiContext& g = *GImGui;

	const char* text_display_end;
	if (hide_text_after_double_hash)
		text_display_end = ImGui::FindRenderedTextEnd(text, text_end);
	else
		text_display_end = text_end;

	ImFont* font = g.Font;
	const float font_size = sz ? sz : g.FontSize;
	if (text == text_display_end)
		return ImVec2(0.0f, font_size);
	ImVec2 text_size = font->CalcTextSizeA(font_size, FLT_MAX, wrap_width, text, text_display_end, NULL);

	text_size.x = IM_FLOOR(text_size.x + 0.99999f);

	return text_size;
}

void roblox::drawing::text_t::draw()
{
	auto dl = ImGui::GetBackgroundDrawList();

	if (!this->visible) return;

	auto text_sz = custom_calc_text_sz(this->str.c_str(), this->size, nullptr, false, 0.0f);

	auto dd = managers::renderer_mgr.get_display_density();

	auto pos_x = this->position.x * dd;
	auto pos_y = this->position.y * dd;
	if (this->center) {
		pos_x -= text_sz.x / 2.f;
	}

	if (this->outline) {
		ImGuiContext& g = *GImGui;


		ImFont* font = g.Font;

		const auto col = ImColor(this->outline_color.Value.x, this->outline_color.Value.y, this->outline_color.Value.z, this->transparency);

		ImVec2 pos1({ pos_x, pos_y });
		ImVec2 pos2(pos1);
		ImVec2 pos3(pos1);
		ImVec2 pos4(pos1);

		pos1.x -= 1;
		pos1.y -= 1;

		dl->AddText(nullptr, this->size, pos1, col, this->str.c_str(), nullptr);
		pos2.x -= 1;
		pos2.y += 1;
		dl->AddText(nullptr, this->size, pos2, col, this->str.c_str(), nullptr);
		pos3.x += 1;
		pos3.y -= 1;
		dl->AddText(nullptr, this->size, pos3, col, this->str.c_str(), nullptr);
		pos4.x += 1;
		pos4.y += 1;
		dl->AddText(nullptr, this->size, pos4, col, this->str.c_str(), nullptr);
	}

	dl->AddText(nullptr, this->size, { pos_x, pos_y }, ImColor(this->color.Value.x, this->color.Value.y, this->color.Value.z, this->transparency), this->str.c_str(), nullptr);
}

void roblox::drawing::line_t::draw()
{
	auto dl = ImGui::GetBackgroundDrawList();

	auto dd = managers::renderer_mgr.get_display_density();
	const auto from = this->from * dd;
	const auto to = this->to * dd;

	if (!this->visible) return;
	dl->AddLine(
		{ from.x, from.y }, { to.x, to.y },
		ImColor(this->color.Value.x, this->color.Value.y, this->color.Value.z, this->transparency), this->thickness
	);
}

std::int32_t roblox::drawing::remove(lua_State* state)
{
	auto& drawing_vec = managers::drawing_mgr.drawing_objects;

	const auto item = reinterpret_cast<base_t*>(lua_touserdata(state, 1));
	if (const auto iter = std::find(drawing_vec.begin(), drawing_vec.end(), item); iter != drawing_vec.end())
	{
		drawing_vec.erase(iter);

		lua_unref(state, key_map.at(item));

		key_map.erase(item);
	}

	return 0;
}

std::int32_t roblox::drawing::clear(lua_State* state)
{
	auto& drawing_vec = managers::drawing_mgr.drawing_objects;

	for (auto iter = drawing_vec.begin(); iter != drawing_vec.end(); iter++)
	{
		lua_unref(state, key_map.at(*iter));

		key_map.erase(*iter);
	}

	drawing_vec.clear();

	return 0;
}

std::int32_t roblox::drawing::drawing_new(lua_State* state)
{
	const auto type = luaL_checkstring(state, 1);

	base_t* obj = nullptr;

	if (!std::strcmp(type, "Text"))
	{
		obj = new (lua_newuserdata(state, sizeof(text_t))) text_t;
	}
	else if (!std::strcmp(type, "Circle"))
	{
		obj = new (lua_newuserdata(state, sizeof(circle_t))) circle_t;
	}
	else if (!std::strcmp(type, "Square"))
	{
		obj = new (lua_newuserdata(state, sizeof(square_t))) square_t;
	}
	else if (!std::strcmp(type, "Line"))
	{
		obj = new (lua_newuserdata(state, sizeof(line_t))) line_t;
	}
	else if (!std::strcmp(type, "Image"))
	{
		obj = new (lua_newuserdata(state, sizeof(image_t))) image_t;
	}
	else if (!std::strcmp(type, "Quad"))
	{
		obj = new (lua_newuserdata(state, sizeof(quad_t))) quad_t;
	}
	else if (!std::strcmp(type, "Triangle"))
	{
		obj = new (lua_newuserdata(state, sizeof(triangle_t))) triangle_t;
	}
	else
	{
		luaL_error(state, "Invalid drawing type");
	}

	luaL_getmetatable(state, "drawing_obj_mt");
	lua_setmetatable(state, -2);

	//push to container
	key_map.insert({ obj, lua_ref(state, -1) });
	managers::drawing_mgr.drawing_objects.push_back(obj);

	return 1;
}

std::int32_t roblox::drawing::handle_index(lua_State* state)
{
	return static_cast<base_t*>(lua_touserdata(state, 1))->handle_index(state);
}

std::int32_t roblox::drawing::handle_newindex(lua_State* state)
{
	return static_cast<base_t*>(lua_touserdata(state, 1))->handle_newindex(state);
}

std::int32_t roblox::drawing::isrenderobj(lua_State* state)
{
	luaL_checktype(state, 1, LUA_TUSERDATA);

	const auto ud = static_cast<base_t*>(lua_touserdata(state, 1));

	lua_pushboolean(state, key_map.find(ud) != key_map.end());

	return 1;
}

std::int32_t roblox::drawing::getrenderproperty(lua_State* state)
{
	luaL_checktype(state, 1, LUA_TUSERDATA);

	lua_getfield(state, 1, luaL_checkstring(state, 2));

	return lua_gettop(state) - 2;
}

std::int32_t roblox::drawing::setrenderproperty(lua_State* state)
{
	luaL_checktype(state, 1, LUA_TUSERDATA);

	lua_pushvalue(state, 3);
	lua_setfield(state, 1, luaL_checkstring(state, 2));

	return 0;
}

std::int32_t roblox::drawing::circle_t::handle_index(lua_State* state)
{
	const auto drawing_object = static_cast<circle_t*>(lua_touserdata(state, 1));

	const auto type = luaL_checkstring(state, 2);

	if (!std::strcmp(type, "Position"))
	{
		make_vector2(state, drawing_object->position);
		return 1;
	}
	else if (!std::strcmp(type, "Radius"))
	{
		lua_pushnumber(state, drawing_object->radius);
		return 1;
	}
	else if (!std::strcmp(type, "NumSides"))
	{
		lua_pushnumber(state, drawing_object->numsides);
		return 1;
	}
	else if (!std::strcmp(type, "Thickness"))
	{
		lua_pushnumber(state, drawing_object->thickness);
		return 1;
	}
	else if (!std::strcmp(type, "Filled"))
	{
		lua_pushboolean(state, drawing_object->filled);
		return 1;
	}

	return base_t::handle_index(state);
}

std::int32_t roblox::drawing::circle_t::handle_newindex(lua_State* state)
{
	const auto drawing_object = static_cast<circle_t*>(lua_touserdata(state, 1));

	const auto type = luaL_checkstring(state, 2);

	if (!std::strcmp(type, "Position"))
	{
		drawing_object->position = *reinterpret_cast<const ImVec2*>(lua_topointer(state, 3));
		return 0;
	}
	else if (!std::strcmp(type, "Radius"))
	{
		drawing_object->radius = luaL_checknumber(state, 3);
		return 0;
	}
	else if (!std::strcmp(type, "Thickness"))
	{
		drawing_object->thickness = luaL_checknumber(state, 3);

		return 0;
	}
	else if (!std::strcmp(type, "NumSides"))
	{
		drawing_object->numsides = luaL_checknumber(state, 3);
		return 0;
	}
	else if (!std::strcmp(type, "Filled"))
	{
		drawing_object->filled = luaL_checkboolean(state, 3);
		return 0;
	}

	return base_t::handle_newindex(state);
}

std::int32_t roblox::drawing::square_t::handle_index(lua_State* state)
{
	const auto drawing_object = static_cast<square_t*>(lua_touserdata(state, 1));

	const auto type = luaL_checkstring(state, 2);

	if (!std::strcmp(type, "Position"))
	{
		make_vector2(state, drawing_object->position);
		return 1;
	}
	else if (!std::strcmp(type, "Size"))
	{
		make_vector2(state, drawing_object->size);
		return 1;
	}
	else if (!std::strcmp(type, "Thickness"))
	{
		lua_pushnumber(state, drawing_object->thickness);
		return 1;
	}
	else if (!std::strcmp(type, "Filled"))
	{
		lua_pushboolean(state, drawing_object->filled);
		return 1;
	}

	return base_t::handle_index(state);
}

std::int32_t roblox::drawing::square_t::handle_newindex(lua_State* state)
{
	const auto drawing_object = static_cast<square_t*>(lua_touserdata(state, 1));

	const auto type = luaL_checkstring(state, 2);

	if (!std::strcmp(type, "Position"))
	{
		drawing_object->position = *reinterpret_cast<const ImVec2*>(lua_topointer(state, 3));

		return 0;
	}
	else if (!std::strcmp(type, "Size"))
	{
		drawing_object->size = *reinterpret_cast<const ImVec2*>(lua_topointer(state, 3));

		return 0;
	}
	else if (!std::strcmp(type, "Thickness"))
	{
		drawing_object->thickness = luaL_checknumber(state, 3);

		return 0;
	}
	else if (!std::strcmp(type, "Filled"))
	{
		drawing_object->filled = luaL_checkboolean(state, 3);

		return 0;
	}

	return base_t::handle_newindex(state);
}

std::int32_t roblox::drawing::image_t::handle_index( lua_State* state )
{
	const auto drawing_object = static_cast< image_t* >( lua_touserdata( state, 1 ) );

	const auto type = luaL_checkstring( state, 2 );

	if ( !std::strcmp( type, "Position" ) )
	{
		make_vector2( state, drawing_object->position );
		return 1;
	}
	else if ( !std::strcmp( type, "Size" ) )
	{
		make_vector2( state, drawing_object->size );
		return 1;
	}
	else if ( !std::strcmp( type, "Rounding" ) )
	{
		lua_pushnumber( state, drawing_object->rounding );
		return 1;
	}

	return base_t::handle_index( state );
}

std::int32_t roblox::drawing::image_t::handle_newindex( lua_State* state )
{
	const auto drawing_object = static_cast< image_t* >( lua_touserdata( state, 1 ) );

	const auto type = luaL_checkstring( state, 2 );

	if ( !std::strcmp( type, "Position" ) )
	{
		drawing_object->position = *reinterpret_cast< const ImVec2* >( lua_topointer( state, 3 ) );

		return 0;
	}
	else if ( !std::strcmp( type, "Size" ) )
	{
		drawing_object->size = *reinterpret_cast< const ImVec2* >( lua_topointer( state, 3 ) );

		return 0;
	}
	else if ( !std::strcmp( type, "Rounding" ) )
	{
		drawing_object->rounding = luaL_checknumber( state, 3 );

		return 0;
	}
	else if ( !std::strcmp( type, "Data" ) )
	{
		std::size_t nbytes = 0;
		auto data = luaL_checklstring( state, 3, &nbytes );
		if ( !drawing_object->texture )
			glDeleteTextures( 1, &drawing_object->texture );
		
		int ph0, ph1;
		auto success = managers::renderer_mgr.load_texture( data, nbytes, &drawing_object->texture, &ph0, &ph1 );
		if ( !success ) luaL_errorL( state, "Failed to load texture from data." );

		return 0;
	}

	return base_t::handle_newindex( state );
}

std::int32_t roblox::drawing::quad_t::handle_index(lua_State* state)
{
	const auto drawing_object = static_cast<quad_t*>(lua_touserdata(state, 1));

	const auto type = luaL_checkstring(state, 2);

	if (!std::strcmp(type, "PointA"))
	{
		make_vector2(state, drawing_object->point_a);
		return 1;
	}
	else if (!std::strcmp(type, "PointB"))
	{
		make_vector2(state, drawing_object->point_b);
		return 1;
	}
	else if (!std::strcmp(type, "PointC"))
	{
		make_vector2(state, drawing_object->point_c);
		return 1;
	}
	else if (!std::strcmp(type, "PointD"))
	{
		make_vector2(state, drawing_object->point_d);

		return 1;
	}
	else if (!std::strcmp(type, "Thickness"))
	{
		lua_pushnumber(state, drawing_object->thickness);

		return 1;
	}
	else if (!std::strcmp(type, "Filled"))
	{
		lua_pushboolean(state, drawing_object->filled);

		return 1;
	}

	return base_t::handle_index(state);
}

std::int32_t roblox::drawing::quad_t::handle_newindex(lua_State* state)
{
	const auto drawing_object = static_cast<quad_t*>(lua_touserdata(state, 1));

	const auto type = luaL_checkstring(state, 2);

	if (!std::strcmp(type, "Filled"))
	{
		drawing_object->filled = luaL_checkboolean(state, 3);

		return 0;
	}
	else if (!std::strcmp(type, "PointA"))
	{
		drawing_object->point_a = *reinterpret_cast<const ImVec2*>(lua_topointer(state, 3));

		return 0;
	}
	else if (!std::strcmp(type, "PointB"))
	{
		drawing_object->point_b = *reinterpret_cast<const ImVec2*>(lua_topointer(state, 3));

		return 0;
	}
	else if (!std::strcmp(type, "PointC"))
	{
		drawing_object->point_c = *reinterpret_cast<const ImVec2*>(lua_topointer(state, 3));

		return 0;
	}
	else if (!std::strcmp(type, "PointD"))
	{
		drawing_object->point_d = *reinterpret_cast<const ImVec2*>(lua_topointer(state, 3));

		return 0;
	}
	else if (!std::strcmp(type, "Thickness"))
	{
		drawing_object->thickness = luaL_checknumber(state, 3);

		return 0;
	}

	return base_t::handle_newindex(state);
}

std::int32_t roblox::drawing::triangle_t::handle_index(lua_State* state)
{
	const auto drawing_object = static_cast<triangle_t*>(lua_touserdata(state, 1));

	const auto type = luaL_checkstring(state, 2);

	if (!std::strcmp(type, "PointA"))
	{
		make_vector2(state, drawing_object->point_a);

		return 1;
	}
	else if (!std::strcmp(type, "PointB"))
	{
		make_vector2(state, drawing_object->point_b);

		return 1;
	}
	else if (!std::strcmp(type, "PointC"))
	{
		make_vector2(state, drawing_object->point_c);

		return 1;
	}
	else if (!std::strcmp(type, "Thickness"))
	{
		lua_pushnumber(state, drawing_object->thickness);

		return 1;
	}
	else if (!std::strcmp(type, "Filled"))
	{
		lua_pushboolean(state, drawing_object->filled);

		return 1;
	}

	return base_t::handle_index(state);
}

std::int32_t roblox::drawing::triangle_t::handle_newindex(lua_State* state)
{
	const auto drawing_object = static_cast<triangle_t*>(lua_touserdata(state, 1));

	const auto type = luaL_checkstring(state, 2);

	if (!std::strcmp(type, "Filled"))
	{
		drawing_object->filled = luaL_checkboolean(state, 3);

		return 0;
	}
	else if (!std::strcmp(type, "PointA"))
	{
		drawing_object->point_a = *reinterpret_cast<const ImVec2*>(lua_topointer(state, 3));

		return 0;
	}
	else if (!std::strcmp(type, "PointB"))
	{
		drawing_object->point_b = *reinterpret_cast<const ImVec2*>(lua_topointer(state, 3));

		return 0;
	}
	else if (!std::strcmp(type, "PointC"))
	{
		drawing_object->point_c = *reinterpret_cast<const ImVec2*>(lua_topointer(state, 3));

		return 0;
	}
	else if (!std::strcmp(type, "Thickness"))
	{
		drawing_object->thickness = luaL_checknumber(state, 3);

		return 0;
	}

	return base_t::handle_newindex(state);
}

std::int32_t roblox::drawing::text_t::handle_index(lua_State* state)
{
	const auto drawing_object = static_cast<text_t*>(lua_touserdata(state, 1));

	const auto type = luaL_checkstring(state, 2);

	if (!std::strcmp(type, "Text"))
	{
		lua_pushstring(state, drawing_object->str.c_str());

		return 1;
	}
	else if (!std::strcmp(type, "OutlineColor"))
	{
		make_color3(state, drawing_object->outline_color);

		return 1;
	}
	else if (!std::strcmp(type, "Center"))
	{
		lua_pushboolean(state, drawing_object->center);

		return 1;
	}
	else if (!std::strcmp(type, "Outline"))
	{
		lua_pushboolean(state, drawing_object->outline);

		return 1;
	}
	else if (!std::strcmp(type, "Position"))
	{
		make_vector2(state, drawing_object->position);

		return 1;
	}
	else if (!std::strcmp(type, "Size"))
	{
		lua_pushnumber(state, drawing_object->size);

		return 1;
	}
	else if (!std::strcmp(type, "TextBounds"))
	{
		make_vector2(state, custom_calc_text_sz(this->str.c_str(), this->size, nullptr, false, 0.0f));

		return 1;
	}
	else if (!std::strcmp(type, "OutlineOpacity"))
	{
		lua_pushnumber(state, drawing_object->outline_opacity);

		return 1;
	}
	else if (!std::strcmp(type, "Font"))
	{
		// Todo
		//lua_pushnumber( state, drawing_object->font );

		return 1;
	}

	return base_t::handle_index(state);
}

std::int32_t roblox::drawing::text_t::handle_newindex(lua_State* state)
{
	const auto drawing_object = static_cast<text_t*>(lua_touserdata(state, 1));

	const auto type = luaL_checkstring(state, 2);

	if (!std::strcmp(type, "Text"))
	{
		drawing_object->str = luaL_checkstring(state, 3);

		return 0;
	}
	else if (!std::strcmp(type, "OutlineColor"))
	{
		drawing_object->outline_color = *reinterpret_cast<const color_t*>(lua_topointer(state, 3));

		return 0;
	}
	else if (!std::strcmp(type, "Center"))
	{
		drawing_object->center = luaL_checkboolean(state, 3);

		return 0;
	}
	else if (!std::strcmp(type, "Outline"))
	{
		drawing_object->outline = luaL_checkboolean(state, 3);

		return 0;
	}
	else if (!std::strcmp(type, "Position"))
	{
		drawing_object->position = *reinterpret_cast<const ImVec2*>(lua_topointer(state, 3));

		return 0;
	}
	else if (!std::strcmp(type, "Size"))
	{
		drawing_object->size = luaL_checknumber(state, 3);

		return 0;
	}
	else if (!std::strcmp(type, "OutlineOpacity"))
	{
		drawing_object->outline_opacity = luaL_checknumber(state, 3);

		return 0;
	}
	else if (!std::strcmp(type, "Font"))
	{
		// Todo
		//lua_pushnumber( state, drawing_object->font );



		return 0;
	}

	return base_t::handle_newindex(state);
}

std::int32_t roblox::drawing::line_t::handle_index(lua_State* state)
{
	const auto drawing_object = static_cast<line_t*>(lua_touserdata(state, 1));

	const auto type = luaL_checkstring(state, 2);

	if (!std::strcmp(type, "From"))
	{
		make_vector2(state, drawing_object->from);

		return 1;
	}
	else if (!std::strcmp(type, "To"))
	{
		make_vector2(state, drawing_object->to);

		return 1;
	}
	else if (!std::strcmp(type, "Thickness"))
	{
		lua_pushnumber(state, drawing_object->thickness);

		return 1;
	}

	return base_t::handle_index(state);
}

std::int32_t roblox::drawing::line_t::handle_newindex(lua_State* state)
{
	const auto drawing_object = static_cast<line_t*>(lua_touserdata(state, 1));

	const auto type = luaL_checkstring(state, 2);

	if (!std::strcmp(type, "From"))
	{
		drawing_object->from = *reinterpret_cast<const ImVec2*>(lua_topointer(state, 3));

		return 0;
	}
	else if (!std::strcmp(type, "To"))
	{
		drawing_object->to = *reinterpret_cast<const ImVec2*>(lua_topointer(state, 3));

		return 0;
	}
	else if (!std::strcmp(type, "Thickness"))
	{
		drawing_object->thickness = luaL_checknumber(state, 3);

		return 0;
	}

	return base_t::handle_newindex(state);
}

std::int32_t roblox::drawing::base_t::handle_index(lua_State* state)
{
	const auto drawing_object = static_cast<base_t*>(lua_touserdata(state, 1));

	const auto type = luaL_checkstring(state, 2);

	if (!std::strcmp(type, "ZIndex"))
	{
		lua_pushinteger(state, drawing_object->zindex);
	}
	else if (!std::strcmp(type, "Visible"))
	{
		lua_pushboolean(state, drawing_object->visible);
	}
	else if (!std::strcmp(type, "Transparency"))
	{
		lua_pushnumber(state, drawing_object->transparency);
	}
	else if (!std::strcmp(type, "Color"))
	{
		make_color3(state, drawing_object->color);
	}
	else if (!std::strcmp(type, "Remove"))
	{
		managers::function_mgr.lua_pushcclosurek(state, remove, nullptr, 0, nullptr);
	}
	else if (!std::strcmp(type, "Destroy"))
	{
		managers::function_mgr.lua_pushcclosurek(state, remove, nullptr, 0, nullptr);
	}
	else
	{
		std::printf("invalid index field %s\n", type);

		return 0;
	}

	return 1;
}

std::int32_t roblox::drawing::base_t::handle_newindex(lua_State* state)
{
	const auto drawing_object = static_cast<base_t*>(lua_touserdata(state, 1));

	const auto type = luaL_checkstring(state, 2);

	if (!std::strcmp(type, "ZIndex"))
	{
		drawing_object->zindex = luaL_checkinteger(state, 3);
	}
	else if (!std::strcmp(type, "Visible"))
	{
		drawing_object->visible = luaL_checkboolean(state, 3);
	}
	else if (!std::strcmp(type, "Transparency"))
	{
		drawing_object->transparency = luaL_checknumber(state, 3);
	}
	else if (!std::strcmp(type, "Color"))
	{
		drawing_object->color = *reinterpret_cast<const color_t*>(lua_topointer(state, 3));
	}
	else
	{
		std::printf("invalid newindex field %s\n", type);
	}

	return 0;
}

void roblox::drawing::initiate()
{
	lua_newtable(managers::roblox_mgr.thread);
	managers::function_mgr.lua_pushcclosurek(managers::roblox_mgr.thread, drawing_new, nullptr, 0, nullptr);
	lua_setfield(managers::roblox_mgr.thread, -2, "new");
	managers::function_mgr.lua_pushcclosurek(managers::roblox_mgr.thread, clear, nullptr, 0, nullptr);

	lua_setfield(managers::roblox_mgr.thread, -2, "clear");

	lua_newtable( managers::roblox_mgr.thread );

	lua_pushinteger( managers::roblox_mgr.thread, 0 );
	lua_setfield( managers::roblox_mgr.thread, -2, "UI" );

	lua_pushinteger( managers::roblox_mgr.thread, 1 );
	lua_setfield( managers::roblox_mgr.thread, -2, "System" );

	lua_pushinteger( managers::roblox_mgr.thread, 2 );
	lua_setfield( managers::roblox_mgr.thread, -2, "Plex" );

	lua_pushinteger( managers::roblox_mgr.thread, 3 );
	lua_setfield( managers::roblox_mgr.thread, -2, "Monospace" );

	lua_setfield( managers::roblox_mgr.thread, -2, "Fonts" );

	managers::function_mgr.lua_pushcclosurek(managers::roblox_mgr.thread, clear, nullptr, 0, nullptr);
	lua_setglobal(managers::roblox_mgr.thread, "cleardrawcache");

	lua_setglobal(managers::roblox_mgr.thread, "Drawing");

	luaL_newmetatable(managers::roblox_mgr.thread, "drawing_obj_mt");
	managers::function_mgr.lua_pushcclosurek(managers::roblox_mgr.thread, handle_index, nullptr, 0, nullptr);
	lua_setfield(managers::roblox_mgr.thread, -2, "__index");
	managers::function_mgr.lua_pushcclosurek(managers::roblox_mgr.thread, handle_newindex, nullptr, 0, nullptr);
	lua_setfield(managers::roblox_mgr.thread, -2, "__newindex");

	managers::function_mgr.lua_pushcclosurek(managers::roblox_mgr.thread, isrenderobj, nullptr, 0, nullptr);
	lua_setglobal(managers::roblox_mgr.thread, "isrenderobj");

	managers::function_mgr.lua_pushcclosurek(managers::roblox_mgr.thread, getrenderproperty, nullptr, 0, nullptr);
	lua_setglobal(managers::roblox_mgr.thread, "getrenderproperty");

	managers::function_mgr.lua_pushcclosurek(managers::roblox_mgr.thread, setrenderproperty, nullptr, 0, nullptr);
	lua_setglobal(managers::roblox_mgr.thread, "setrenderproperty");
}
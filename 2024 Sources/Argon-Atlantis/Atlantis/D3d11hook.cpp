#include "D3D11Hook.hpp"

#include <winternl.h>
#include <xcall_once.h>
#include <string>
#include <memory>
#include "imgui/TextEditor.h"
#include "XorStr.hpp"

#include "Roblox.hpp"
#include "imgui/imgui_internal.h"

namespace HelperFuncs
{

    //using print_t = void(__fastcall*)(int type, const char* source, ...);
   // static print_t sigma_print = reinterpret_cast<print_t>((uintptr_t)GetModuleHandleA(0) + 0x12DB520);

}

namespace Logger {
    void printf(const char* real, ...) {
        //HelperFuncs::sigma_print(1, real);
    }
}


namespace TaskSchedulerV2
{

    constexpr uintptr_t JobsStart = 0x198;
    constexpr uintptr_t JobsEnd = 0x1A0;
    constexpr uintptr_t JobName = 0x90;

    uintptr_t GetJobByName(std::string JobName) {
        uintptr_t base = (uintptr_t)GetModuleHandleA("RobloxPlayerBeta.exe");

        uintptr_t Scheduler = *reinterpret_cast<uintptr_t*>(base + 0x5BD1AF8);

        //Logger::printf("TaskScheduler: %p\n", Scheduler);

        uintptr_t JobRet = 0;
        uintptr_t JobStart = *(uintptr_t*)(Scheduler + JobsStart);

        while (JobStart != *(uintptr_t*)(Scheduler + JobsEnd))
        {
            /*       std::string erm = *(std::string*)(*(uintptr_t*)(JobStart)+0x90);
                   Logger::printf("Job: %s\n", erm.c_str());*/

            if (*(std::string*)(*(uintptr_t*)(JobStart)+0x90) == JobName) {
                JobRet = *(uintptr_t*)JobStart;
                //Logger::printf("found job");
            }
            JobStart += 0x10;
        }

        return JobRet;
    }

}

namespace Drawing {


	struct color_t
	{
		float r, g, b;

		operator ImVec4() { return { r, g, b, 1.f }; }

		operator ImVec4() const { return { r, g, b, 1.f }; }

		operator ImColor() const { return { r, g, b, 1.f }; }
	};

	enum class font_t {
		ui,
		system,
		plex,
		monospace
	};

	struct base_t {
		int zindex{ 1 };
		bool visible{ false };
		double transparency{ 1.0 };
		ImColor color{ 0.f, 0.f, 0.f, 0.f };

		virtual std::int32_t handle_index(lua_State* state);
		virtual std::int32_t handle_newindex(lua_State* state);

		virtual ~base_t() = default;
		virtual void draw() = 0;
	};

	struct line_t : public base_t {
		double thickness{ 1.0 };
		ImVec2 from{ 0.0, 0.0 };
		ImVec2 to{ 0.0, 0.0 };

		std::int32_t handle_index(lua_State* state) override;
		std::int32_t handle_newindex(lua_State* state) override;

		virtual ~line_t() = default;
		void draw() override;
	};

	struct text_t : public base_t {
		std::string str{ "Text" };
		double size{ 8 };
		bool center{ false };
		bool outline{ false };
		ImColor outline_color{ 0.f, 0.f, 0.f, 0.f };
		double outline_opacity{ 0.0 };
		ImVec2 position{ 0.0, 0.0 };
		ImVec2 text_bounds{ 0, 0 }; // will be read-only
		font_t font{ font_t::system };

		std::int32_t handle_index(lua_State* state) override;
		std::int32_t handle_newindex(lua_State* state) override;

		virtual ~text_t() = default;
		void draw() override;
	};

	struct image_t : public base_t {
		ImVec2 size{ 0.0, 0.0 };
		ImVec2 position{ 0.0, 0.0 };
		double rounding{ 0 };

		std::int32_t handle_index(lua_State* state) override;
		std::int32_t handle_newindex(lua_State* state) override;

		virtual ~image_t() = default;
		void draw() override;
	};

	struct circle_t : public base_t {
		double thickness{ 1.0 };
		double numsides{ 360.0 };
		double radius{ 2.0 };
		ImVec2 position{ 0.0, 0.0 };
		bool filled{ false };

		std::int32_t handle_index(lua_State* state) override;
		std::int32_t handle_newindex(lua_State* state) override;

		virtual ~circle_t() = default;
		void draw() override;
	};

	struct square_t : public base_t {
		double thickness{ 1.0 };
		ImVec2 size{ 100,100 };
		ImVec2 position{ 0.0, 0.0 };
		bool filled{ false };

		std::int32_t handle_index(lua_State* state) override;
		std::int32_t handle_newindex(lua_State* state) override;

		virtual ~square_t() = default;
		void draw() override;
	};

	struct quad_t : public base_t {
		double thickness{ 1.0 };
		ImVec2 point_a{ 0.0, 0.0 };
		ImVec2 point_b{ 0.0, 0.0 };
		ImVec2 point_c{ 0.0, 0.0 };
		ImVec2 point_d{ 0.0, 0.0 };
		bool filled{ false };

		std::int32_t handle_index(lua_State* state) override;
		std::int32_t handle_newindex(lua_State* state) override;

		virtual ~quad_t() = default;
		void draw() override;
	};

	struct triangle_t : public base_t {
		double thickness{ 1.0 };
		ImVec2 point_a{ 0.0, 0.0 };
		ImVec2 point_b{ 0.0, 0.0 };
		ImVec2 point_c{ 0.0, 0.0 };
		bool filled{ false };

		std::int32_t handle_index(lua_State* state) override;
		std::int32_t handle_newindex(lua_State* state) override;

		virtual ~triangle_t() = default;
		void draw() override;
	};


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

	std::vector< base_t*> drawing_objects;
	std::unordered_map< base_t*, int > key_map;

	void triangle_t::draw()
	{
		auto dl = ImGui::GetBackgroundDrawList();


		const auto point_a = this->point_a;
		const auto point_b = this->point_b;
		const auto point_c = this->point_c;

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

	void quad_t::draw()
	{
		auto dl = ImGui::GetBackgroundDrawList();


		const auto point_a = this->point_a;
		const auto point_b = this->point_b;
		const auto point_c = this->point_c;
		const auto point_d = this->point_d;

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

	void square_t::draw()
	{
		auto dl = ImGui::GetBackgroundDrawList();

		if (!this->visible) return;

		ImVec2 top_left = { this->position.x, this->position.y };
		ImVec2 bottom_right = { this->position.x + this->size.x, this->position.y + this->size.y };

		ImU32 color = ImColor(this->color.Value.x, this->color.Value.y, this->color.Value.z, this->transparency);

		if (this->filled) {
			dl->AddRectFilled(top_left, bottom_right, color);
		}
		else {
			dl->AddRect(top_left, bottom_right, color, 0.0f, ImDrawFlags_None, this->thickness);
		}

	}

	void circle_t::draw()
	{
		auto dl = ImGui::GetBackgroundDrawList();



		const auto pos = this->position;

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

	void image_t::draw()
	{
		auto dl = ImGui::GetBackgroundDrawList();


		if (!this->visible) return;

		//dl->AddImageRounded(reinterpret_cast<void*>(texture), { this->position.x, this->position.y },
		//	{ (this->position.x + this->size.x), (this->position.y + this->size.y) }, { 0, 0 }, { 1, 1 },
		//	ImColor(this->color.Value.x, this->color.Value.y, this->color.Value.z, this->transparency), this->rounding);
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

	void text_t::draw()
	{
		auto dl = ImGui::GetBackgroundDrawList();

		if (!this->visible) return;

		auto text_sz = custom_calc_text_sz(this->str.c_str(), this->size, nullptr, false, 0.0f);



		auto pos_x = this->position.x;
		auto pos_y = this->position.y;
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

	void line_t::draw()
	{
		auto dl = ImGui::GetBackgroundDrawList();


		const auto from = this->from;
		const auto to = this->to;

		if (!this->visible) return;
		dl->AddLine(
			{ from.x, from.y }, { to.x, to.y },
			ImColor(this->color.Value.x, this->color.Value.y, this->color.Value.z, this->transparency), this->thickness
		);
	}

	std::int32_t remove(lua_State* state)
	{
		auto& drawing_vec = drawing_objects;

		const auto item = reinterpret_cast<base_t*>(lua_touserdata(state, 1));
		if (const auto iter = std::find(drawing_vec.begin(), drawing_vec.end(), item); iter != drawing_vec.end())
		{
			drawing_vec.erase(iter);

			lua_unref(state, key_map.at(item));

			key_map.erase(item);
		}

		return 0;
	}

	std::int32_t clear(lua_State* state)
	{
		auto& drawing_vec = drawing_objects;

		for (auto iter = drawing_vec.begin(); iter != drawing_vec.end(); iter++)
		{
			lua_unref(state, key_map.at(*iter));

			key_map.erase(*iter);
		}

		drawing_vec.clear();

		return 0;
	}

	std::int32_t drawing_new(lua_State* state)
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

		luaL_getmetatable(state, "AtlantisDrawing");
		lua_setmetatable(state, -2);

		//push to container
		key_map.insert({ obj, lua_ref(state, -1) });
		drawing_objects.push_back(obj);

		return 1;
	}

	std::int32_t handle_index(lua_State* state)
	{
		return static_cast<base_t*>(lua_touserdata(state, 1))->handle_index(state);
	}

	std::int32_t handle_newindex(lua_State* state)
	{
		return static_cast<base_t*>(lua_touserdata(state, 1))->handle_newindex(state);
	}

	std::int32_t isrenderobj(lua_State* state)
	{
		luaL_checktype(state, 1, LUA_TUSERDATA);

		const auto ud = static_cast<base_t*>(lua_touserdata(state, 1));

		lua_pushboolean(state, key_map.find(ud) != key_map.end());

		return 1;
	}

	std::int32_t getrenderproperty(lua_State* state)
	{
		luaL_checktype(state, 1, LUA_TUSERDATA);

		lua_getfield(state, 1, luaL_checkstring(state, 2));

		return lua_gettop(state) - 2;
	}

	std::int32_t setrenderproperty(lua_State* state)
	{
		luaL_checktype(state, 1, LUA_TUSERDATA);

		lua_pushvalue(state, 3);
		lua_setfield(state, 1, luaL_checkstring(state, 2));

		return 0;
	}

	std::int32_t circle_t::handle_index(lua_State* state)
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

	std::int32_t circle_t::handle_newindex(lua_State* state)
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

	std::int32_t square_t::handle_index(lua_State* state)
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

	std::int32_t square_t::handle_newindex(lua_State* state)
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

	std::int32_t image_t::handle_index(lua_State* state)
	{
		const auto drawing_object = static_cast<image_t*>(lua_touserdata(state, 1));

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
		else if (!std::strcmp(type, "Rounding"))
		{
			lua_pushnumber(state, drawing_object->rounding);
			return 1;
		}

		return base_t::handle_index(state);
	}

	std::int32_t image_t::handle_newindex(lua_State* state)
	{
		const auto drawing_object = static_cast<image_t*>(lua_touserdata(state, 1));

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
		else if (!std::strcmp(type, "Rounding"))
		{
			drawing_object->rounding = luaL_checknumber(state, 3);

			return 0;
		}

		return base_t::handle_newindex(state);
	}

	std::int32_t quad_t::handle_index(lua_State* state)
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

	std::int32_t quad_t::handle_newindex(lua_State* state)
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

	std::int32_t triangle_t::handle_index(lua_State* state)
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

	std::int32_t triangle_t::handle_newindex(lua_State* state)
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

	std::int32_t text_t::handle_index(lua_State* state)
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

	std::int32_t text_t::handle_newindex(lua_State* state)
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

	std::int32_t line_t::handle_index(lua_State* state)
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

	std::int32_t line_t::handle_newindex(lua_State* state)
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

	std::int32_t base_t::handle_index(lua_State* state)
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
			lua_pushcclosurek(state, remove, nullptr, 0, nullptr);
		}
		else if (!std::strcmp(type, "__OBJECT_EXISTS"))
		{
			if (std::find(drawing_objects.begin(), drawing_objects.end(), drawing_object) != drawing_objects.end()) {
				lua_pushboolean(state, true);
			}
			else {
				lua_pushboolean(state, false);
			}
		}
		else if (!std::strcmp(type, "Destroy"))
		{
			lua_pushcclosurek(state, remove, nullptr, 0, nullptr);
		}
		else
		{
			std::printf("invalid index field %s\n", type);

			return 0;
		}

		return 1;
	}

	std::int32_t base_t::handle_newindex(lua_State* state)
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
}

typedef NTSTATUS(NTAPI* NtProtectVirtualMemory_t)(
    HANDLE ProcessHandle,
    PVOID* BaseAddress,
    PSIZE_T RegionSize,
    ULONG NewProtect,
    PULONG OldProtect
    );

NtProtectVirtualMemory_t NtProtectVirtualMemory = (NtProtectVirtualMemory_t)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtProtectVirtualMemory");

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

/*extern "C" NTSTATUS NTAPI NtProtectVirtualMemory(
    HANDLE ProcessHandle,
    PVOID* BaseAddress,
    PSIZE_T RegionSize,
    ULONG NewProtect,
    PULONG OldProtect
);*/

HWND D3D11Hook::window = nullptr;
DXGI_SWAP_CHAIN_DESC D3D11Hook::swapchain_desc = {};

D3D11Hook::Present D3D11Hook::o_present = nullptr;
D3D11Hook::ResizeBuffers D3D11Hook::o_resize_buffers = nullptr;
WNDPROC D3D11Hook::o_window_callback = nullptr;

IDXGISwapChain* D3D11Hook::swap_chain = nullptr;
ID3D11Device* D3D11Hook::d3d_device = nullptr;
ID3D11DeviceContext* D3D11Hook::device_context = nullptr;
ID3D11RenderTargetView* D3D11Hook::render_target_view = nullptr;
ID3D11Texture2D* D3D11Hook::back_buffer = nullptr;

std::int32_t D3D11Hook::width = NULL;
std::int32_t D3D11Hook::height = NULL;

bool D3D11Hook::render_scene = false;
bool D3D11Hook::render_interface = false;
bool D3D11Hook::render_initialized = true;

float D3D11Hook::dpi_scale = 1.0f;
std::vector < ImFont* > D3D11Hook::rendering_fonts;

ImFont* D3D11Hook::ui;
ImFont* D3D11Hook::system;
ImFont* D3D11Hook::plex;
ImFont* D3D11Hook::monospace;
ImFont* D3D11Hook::consolas;

D3D11Hook::render_vf_function_t D3D11Hook::original_render_step = nullptr;

bool D3D11Hook::is_render_hooked = false;
void on_resize() {
    if (D3D11Hook::render_target_view) {
        D3D11Hook::render_target_view->Release();
        D3D11Hook::render_target_view = nullptr;
    }
    return;
}

HRESULT __stdcall swapchain_resize_buffers(
    IDXGISwapChain* swap_chain,
    UINT buffer_count,
    UINT width,
    UINT height,
    DXGI_FORMAT new_format,
    UINT flags
) {
    on_resize();
    return D3D11Hook::o_resize_buffers(swap_chain, buffer_count, width, height, new_format, flags);
}


static std::uint32_t window_width = 0, window_height = 0;
LRESULT __stdcall window_callback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;


    if (msg == WM_DPICHANGED) {
		D3D11Hook::dpi_scale = ((WORD)(((DWORD_PTR)(wParam)) & 0xffff)) / 96.0f;
    } else if (msg == WM_SIZE) {
        std::uint32_t width = LOWORD(lParam), height = HIWORD(lParam);
        if (!D3D11Hook::is_render_hooked && (window_width != width || window_height != height)) {
            window_width = width, window_height = height;
            //window_size_changes.push(true);
        }
    }
    


    switch (msg) {
    case 522:
    case 533:
    case 534:
    case 535:
    case 536:
    case 514:
    case 517:
    case 516:
    case 257:
    case 256:
    case 132:
    case 523:
    case 524:
    case 793:
        if (D3D11Hook::render_interface) {
            return true;
        }
        break;
    }

    return CallWindowProc(D3D11Hook::o_window_callback, hWnd, msg, wParam, lParam);
}



enum tabs {
    Editor = 0,
    Options = 1
};

tabs currenttab = Editor;

static int g_Width = 800;
static int g_Height = 600;

static bool render = false;

void on_step() {
    static std::once_flag step_flag;
    std::call_once(step_flag, []() {
        if (D3D11Hook::swap_chain->GetDesc(&D3D11Hook::swapchain_desc) != S_OK)
            Logger::printf("Failed to get SpawChain desc");

        if (D3D11Hook::swap_chain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&D3D11Hook::d3d_device)) != S_OK)
            Logger::printf("Failed to get d3d device");

        if (D3D11Hook::swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&D3D11Hook::back_buffer)) != S_OK)
            Logger::printf("Failed to get back buffer");

        D3D11Hook::window = D3D11Hook::swapchain_desc.OutputWindow;
        //D3D11Hook::g_storage.store("roblox_window", reinterpret_cast<std::uintptr_t>(D3D11Hook::window));
        D3D11Hook::dpi_scale = GetDpiForWindow(D3D11Hook::window) / 96.f;

        D3D11Hook::o_window_callback = reinterpret_cast<WNDPROC>(
            SetWindowLongPtr(D3D11Hook::window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(window_callback))
            );

        D3D11Hook::d3d_device->GetImmediateContext(&D3D11Hook::device_context);
        if (D3D11Hook::d3d_device->CreateRenderTargetView(D3D11Hook::back_buffer, nullptr, &D3D11Hook::render_target_view) != S_OK)
            Logger::printf("Failed to create target view");
        //g_Log.error(xorstr("Failed to create target view"));

        D3D11Hook::back_buffer->Release();
        D3D11Hook::back_buffer = nullptr;

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;;
        io.IniFilename = NULL;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

        ImGui_ImplWin32_Init(D3D11Hook::window);
        ImGui_ImplDX11_Init(D3D11Hook::d3d_device, D3D11Hook::device_context);

        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);


        style.WindowRounding = 8.0f;
        style.FrameRounding = 4.0f;

        });

    if (D3D11Hook::render_target_view == nullptr) {
        D3D11Hook::swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&D3D11Hook::back_buffer));
        D3D11Hook::d3d_device->CreateRenderTargetView(D3D11Hook::back_buffer, nullptr, &D3D11Hook::render_target_view);
        D3D11Hook::back_buffer->Release();
    }

    TextEditor editor;
    auto lang = TextEditor::LanguageDefinition::Lua();
    editor.SetLanguageDefinition(lang);
    TextEditor::Palette palette = TextEditor::GetDarkPalette();
    palette[(int)TextEditor::PaletteIndex::Background] = 0xFF101010;
    palette[(int)TextEditor::PaletteIndex::Cursor] = 0xFFFFFFFF;
    palette[(int)TextEditor::PaletteIndex::Selection] = 0xFF404040;
    editor.SetPalette(palette);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGuiIO& io = ImGui::GetIO();


    io.MousePos.x /= D3D11Hook::dpi_scale;
    io.MousePos.y /= D3D11Hook::dpi_scale;

	if (0 == 1)
		D3D11Hook::render_interface = !D3D11Hook::render_interface;

    ImGui::NewFrame();

    /*const auto copy = g_drawing_cache.copy();
    for (std::size_t i = 0; i < copy.size(); i++)
        environment::utilities::render_object(copy[i]);

    if (render_interface)
        user_interface::render_interface();*/

        //g_alert.render()
    if (D3D11Hook::render_interface) {
        {
			ImGui::SetNextWindowPos(ImVec2(340, 10), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(540, 400), ImGuiCond_FirstUseEver);
            ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);

			for (const auto& log : console_logs) {
				ImGui::TextUnformatted(log.c_str());
			}

            ImGui::SetNextWindowSize(ImVec2(440, 300), ImGuiCond_FirstUseEver);

            ImGui::SetCursorPos(ImVec2(5, ImGui::GetWindowHeight() - 30));
            ImGui::BeginChild("Input", ImVec2(ImGui::GetWindowWidth() - 10, 25));
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

            static char inputBuf[256] = "";
            float buttonWidth = 50.0f;
            ImGui::PushItemWidth(ImGui::GetWindowWidth() - buttonWidth - 8);
            if (ImGui::InputText("##ConsoleInput", inputBuf, IM_ARRAYSIZE(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                memset(inputBuf, 0, sizeof(inputBuf));
            }
            ImGui::PopItemWidth();
            ImGui::PopStyleColor();

            ImGui::SameLine();
            ImGui::Button("Enter", ImVec2(buttonWidth, 19));
            ImGui::EndChild();
            ImGui::End();
        }

        {
            ImGui::SetNextWindowPos(ImVec2(340, 10), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(540, 400), ImGuiCond_FirstUseEver);

            ImGui::SetWindowPos(ImVec2(10, 10));
            ImGui::SetWindowSize(ImVec2(g_Width / 2 - 20, g_Height - 20));
            ImGui::Begin("Pluto", nullptr,
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoResize);

            if (ImGui::Button("Editor")) {
                currenttab = Editor;
            }
            ImGui::SameLine();
            if (ImGui::Button("Options"))
            {
                currenttab = Options;
            }

            if (currenttab == Editor) {
                editor.Render("TextEditor" /*, ImVec2(0, ImGui::GetContentRegionAvail().y - 30)*/);

                ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 28);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
                if (ImGui::Button("Execute")) {
					coderun_queue.push(editor.GetText());
				}
                ImGui::SameLine();
                if (ImGui::Button("Clear")) {}
                ImGui::SameLine();
                if (ImGui::Button("Execute from clipboard")) {}
                ImGui::SameLine();
                if (ImGui::Button("Scripts"))
                {
                    ImGui::OpenPopup("ScriptsMenu");
                }
                if (ImGui::BeginPopup("ScriptsMenu"))
                {
                    if (ImGui::MenuItem("Secure dex")) {
                    }
                    if (ImGui::MenuItem("Infinity yield")) {
                        Logger::printf("pressed");
                        coderun_queue.push("loadstring(game:HttpGet('https://raw.githubusercontent.com/edgeiy/infiniteyield/master/source'))()");
                    }
                    if (ImGui::MenuItem("Infinity yield reborn")) {}
                    if (ImGui::MenuItem("UNC")) {
                        coderun_queue.push("loadstring(game:HttpGet('https://raw.githubusercontent.com/unified-naming-convention/NamingStandard/refs/heads/main/UNCCheckEnv.lua'))()");
                    }
                    if (ImGui::MenuItem("sUNC")) {}
                    if (ImGui::MenuItem("Bad for your health V4")) {}
                    if (ImGui::MenuItem("Simple spy")) {}
                    if (ImGui::MenuItem("Hydroxide")) {}
                    ImGui::EndPopup();
                }
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Injected");
                ImGui::PopStyleColor();
            }
            else if (currenttab == Options) {
                ImGui::BeginChild("OptionsChild", ImVec2(0, 0), true);

                ImGui::Columns(2, "OptionsColumns", false);

                ImGui::Text("Editor");
                static bool console = true;
                static bool disablePurchases = true;
                ImGui::Checkbox("Console", &console);
                ImGui::Checkbox("Disable purchases", &disablePurchases);

                ImGui::NextColumn();

                ImGui::Text("UI settings");
                static bool openKey = false;
                if (ImGui::ArrowButton("##openkey", openKey ? ImGuiDir_Down : ImGuiDir_Right)) {
                    openKey = !openKey;
                }
                ImGui::SameLine();
                ImGui::Text("Open key");

                if (openKey) {
                    ImGui::Indent(20);
                    static int keyBinding = 0;
                    ImGui::InputInt("Key binding", &keyBinding);
                    ImGui::Unindent(20);
                }

                ImGui::Columns(1);
                ImGui::EndChild();
            }


            ImGui::End();
        }
    }
	if (!TaskScheduler::is_loaded()) { Drawing::drawing_objects.clear(); return; };

	for (auto obj : Drawing::drawing_objects) if (obj) obj->draw();

    ImGui::Render();
    ImDrawData* data = ImGui::GetDrawData();

    for (std::int32_t i = 0; i < data->CmdListsCount; i++) {
        ImDrawList* cmd_list = data->CmdLists[i];
        for (std::int32_t cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
            ImDrawCmd* cmd = &cmd_list->CmdBuffer[cmd_i];
            cmd->ClipRect.x *= D3D11Hook::dpi_scale;
            cmd->ClipRect.y *= D3D11Hook::dpi_scale;
            cmd->ClipRect.z *= D3D11Hook::dpi_scale;
            cmd->ClipRect.w *= D3D11Hook::dpi_scale;
        }
    }

    D3D11Hook::device_context->OMSetRenderTargets(1, &D3D11Hook::render_target_view, NULL);
    ImGui_ImplDX11_RenderDrawData(data);

    return;
}

HRESULT __stdcall swapchain_present(
    IDXGISwapChain* swap_chain,
    UINT sync_interval,
    UINT flags
) {
    on_step();
    return D3D11Hook::o_present(swap_chain, sync_interval, flags);
}

std::uintptr_t __fastcall renderjob_step(std::uintptr_t a1, std::uintptr_t a2) {
    on_step();
    on_resize();

    return D3D11Hook::original_render_step != nullptr ? D3D11Hook::original_render_step(a1, a2) : NULL;
}

void D3D11Hook::Reinitialize() {
	Drawing::drawing_objects.clear();

	D3D11Hook::is_render_hooked = false;
	D3D11Hook::window = nullptr;
	D3D11Hook::swapchain_desc = {};

	D3D11Hook::o_present = nullptr;
	D3D11Hook::o_resize_buffers = nullptr;
	D3D11Hook::o_window_callback = nullptr;

	D3D11Hook::swap_chain = nullptr;
	D3D11Hook::d3d_device = nullptr;
	D3D11Hook::device_context = nullptr;
	D3D11Hook::render_target_view = nullptr;
	D3D11Hook::back_buffer = nullptr;

	D3D11Hook::render_scene = false;
	D3D11Hook::render_interface = false;
	D3D11Hook::render_initialized = true;

	D3D11Hook::original_render_step = nullptr;

	D3D11Hook::is_render_hooked = false;

	D3D11Hook::Initialize();
}

void D3D11Hook::Initialize() {

    Logger::printf("called");
    std::uintptr_t base_address = (std::uintptr_t)(GetModuleHandle(nullptr));

    std::uintptr_t renderjob = TaskSchedulerV2::GetJobByName("RenderJob");
    Logger::printf("Renderjob: %p\n", renderjob);

    std::uintptr_t viewbase = *reinterpret_cast<std::uintptr_t*>(renderjob + 0x218);
    Logger::printf("Viewbase: %p\n", viewbase);

    std::uintptr_t device_d3d11 = *reinterpret_cast<std::uintptr_t*>(viewbase + 0x8);
    Logger::printf("Device D3d11: %p\n", device_d3d11);

    swap_chain = *reinterpret_cast<IDXGISwapChain**>(device_d3d11 + 0xA8);
    Logger::printf("Device SwapChain: %p\n", device_d3d11);

    std::uintptr_t* sc_vftable = *reinterpret_cast<std::uintptr_t**>(swap_chain);
    std::uintptr_t* method_table = static_cast<std::uintptr_t*>(
        calloc(18, sizeof(std::uintptr_t))
        );
    std::memcpy(method_table, sc_vftable, 18 * sizeof(std::uintptr_t));

    o_present = reinterpret_cast<decltype(o_present)>(method_table[8]);
    o_resize_buffers = reinterpret_cast<decltype(o_resize_buffers)>(method_table[13]);

    LPVOID address = (LPVOID)(sc_vftable);
    std::size_t size = sizeof(std::uintptr_t) * 18;

    DWORD old_protection = NULL;
    NTSTATUS nt_status = NtProtectVirtualMemory(
        GetCurrentProcess(),
        &address,
        &size,
        PAGE_READWRITE,
        &old_protection
    );

    if (NT_SUCCESS(nt_status)) {
        sc_vftable[8] = reinterpret_cast<std::uintptr_t>(swapchain_present);
        sc_vftable[13] = reinterpret_cast<std::uintptr_t>(swapchain_resize_buffers);

        NtProtectVirtualMemory(
            GetCurrentProcess(),
            &address,
            &size,
            old_protection,
            &old_protection
        );
    }
    else {
       Logger::printf("NTStatus: %p", nt_status);
        //g_log.debugf(XORSTR("NTStatus: %p, falling back to backup drawing"), nt_status);

#define RENDER_STEP_INDEX 8
#define VFTABLE_HOOK_SIZE 55

        void** _vtable = new void* [VFTABLE_HOOK_SIZE + 1];
        std::memset(_vtable, NULL, sizeof(std::uintptr_t) * (VFTABLE_HOOK_SIZE + 1));
        std::memcpy(_vtable, *reinterpret_cast<void***>(device_d3d11), sizeof(std::uintptr_t) * VFTABLE_HOOK_SIZE);

        original_render_step = reinterpret_cast<render_vf_function_t>(_vtable[RENDER_STEP_INDEX]);

        _vtable[RENDER_STEP_INDEX] = renderjob_step;

        *reinterpret_cast<void***>(device_d3d11) = _vtable;

        is_render_hooked = NT_SUCCESS(nt_status);
        //g_storage.store(XORSTR("render_hooked"), is_render_hooked); /* This just determines if we vftable hooking */
    }

}

void D3D11Hook::InitializeEnvironment(lua_State* L) {
	lua_newtable(L);
	lua_pushcclosurek(L, Drawing::drawing_new, nullptr, 0, nullptr);
	lua_setfield(L, -2, "new");
    lua_pushcclosurek(L, Drawing::clear, nullptr, 0, nullptr);

	lua_setfield(L, -2, "clear");

	lua_newtable(L);

	lua_pushinteger(L, 0);
	lua_setfield(L, -2, "UI");

	lua_pushinteger(L, 1);
	lua_setfield(L, -2, "System");

	lua_pushinteger(L, 2);
	lua_setfield(L, -2, "Plex");

	lua_pushinteger(L, 3);
	lua_setfield(L, -2, "Monospace");

	lua_setfield(L, -2, "Fonts");

	lua_pushcclosurek(L, Drawing::clear, nullptr, 0, nullptr);
	lua_setglobal(L, "cleardrawcache");

	lua_setglobal(L, "Drawing");

	luaL_newmetatable(L, "AtlantisDrawing");
	lua_pushcclosurek(L, Drawing::handle_index, nullptr, 0, nullptr);
	lua_setfield(L, -2, "__index");
	lua_pushcclosurek(L, Drawing::handle_newindex, nullptr, 0, nullptr);
	lua_setfield(L, -2, "__newindex");

	lua_pushcclosurek(L, Drawing::isrenderobj, nullptr, 0, nullptr);
	lua_setglobal(L, "isrenderobj");

	lua_pushcclosurek(L, Drawing::getrenderproperty, nullptr, 0, nullptr);
	lua_setglobal(L, "getrenderproperty");

	lua_pushcclosurek(L, Drawing::setrenderproperty, nullptr, 0, nullptr);
	lua_setglobal(L, "setrenderproperty");
}

void D3D11Hook::ClearDrawings() {
	Drawing::drawing_objects.clear();
}

void D3D11Hook::Unhook() {
	Logger::printf("Unhook called");

	if (swap_chain) {
		std::uintptr_t* sc_vftable = *reinterpret_cast<std::uintptr_t**>(swap_chain);


		if (sc_vftable[8] == reinterpret_cast<std::uintptr_t>(swapchain_present)) {
			sc_vftable[8] = reinterpret_cast<std::uintptr_t>(o_present);
		}
		if (sc_vftable[13] == reinterpret_cast<std::uintptr_t>(swapchain_resize_buffers)) {
			sc_vftable[13] = reinterpret_cast<std::uintptr_t>(o_resize_buffers);
		}

		LPVOID address = (LPVOID)(sc_vftable);
		std::size_t size = sizeof(std::uintptr_t) * 18;

		DWORD old_protection = NULL;
		NTSTATUS nt_status = NtProtectVirtualMemory(
			GetCurrentProcess(),
			&address,
			&size,
			PAGE_READWRITE,
			&old_protection
		);

		if (NT_SUCCESS(nt_status)) {
			NtProtectVirtualMemory(
				GetCurrentProcess(),
				&address,
				&size,
				old_protection,
				&old_protection
			);
			Logger::printf("Vftable restored successfully");
		}
		else {
			Logger::printf("NTStatus: %p - Failed to restore vftable", nt_status);
		}
	}


	if (is_render_hooked) {
		std::uintptr_t base_address = (std::uintptr_t)(GetModuleHandle(nullptr));

		std::uintptr_t renderjob = TaskSchedulerV2::GetJobByName("RenderJob");
		Logger::printf("Renderjob: %p\n", renderjob);

		std::uintptr_t viewbase = *reinterpret_cast<std::uintptr_t*>(renderjob + 0x218);
		Logger::printf("Viewbase: %p\n", viewbase);

		std::uintptr_t device_d3d11 = *reinterpret_cast<std::uintptr_t*>(viewbase + 0x8);
		Logger::printf("Device D3d11: %p\n", device_d3d11);

		swap_chain = *reinterpret_cast<IDXGISwapChain**>(device_d3d11 + 0xA8);
		Logger::printf("Device SwapChain: %p\n", device_d3d11);

		void** _vtable = *reinterpret_cast<void***>(device_d3d11);

		_vtable[RENDER_STEP_INDEX] = reinterpret_cast<void*>(original_render_step);

		*reinterpret_cast<void***>(device_d3d11) = _vtable;


		is_render_hooked = false;
	}
}

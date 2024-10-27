#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <Luau/lstate.h>
#include <Luau/lua.h>
#include <Luau/lapi.h>
#include <Luau/lualib.h>
#include <Luau/lgc.h>

#include <GLES3/gl3.h>

#include <cstdint>
#include <string>

namespace roblox::drawing {
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
        font_t font{ font_t::ui };

        std::int32_t handle_index(lua_State* state) override;
        std::int32_t handle_newindex(lua_State* state) override;

        virtual ~text_t() = default;
        void draw() override;
    };

    struct image_t : public base_t {
        ImVec2 size{ 0.0, 0.0 };
        ImVec2 position{ 0.0, 0.0 };
        double rounding{ 0 };

		GLuint texture{ 0 }; // not reflected

        std::int32_t handle_index( lua_State* state ) override;
        std::int32_t handle_newindex( lua_State* state ) override;

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

    std::int32_t remove(lua_State* state);

    std::int32_t clear(lua_State* state);

    std::int32_t drawing_new(lua_State* state);

    std::int32_t handle_index(lua_State* state);

    std::int32_t handle_newindex(lua_State* state);

    std::int32_t isrenderobj(lua_State* state);

    std::int32_t getrenderproperty(lua_State* state);

    std::int32_t setrenderproperty(lua_State* state);

    void initiate();
}
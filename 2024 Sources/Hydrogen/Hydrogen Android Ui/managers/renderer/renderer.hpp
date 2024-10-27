#pragma once

#include <mutex>

#include <dependencies/imgui/imgui.h>
#include <dependencies/imgui/imgui_impl_opengl3.h>
#include <dependencies/imgui/imgui_impl_glfw.h>
#include <dependencies/glew/glew.h>
#include <dependencies/glfw/glfw3.h>

namespace managers
{
    class renderer_manager_t
    {
    public:
        std::atomic<float> width, height;

        renderer_manager_t( ) = default;

        void initiate( );

        bool load_texture( const char* data, std::size_t sz, GLuint* out_tex, int* out_width, int* out_height );

        float get_display_density( );

        float epoch( );
    };

    extern renderer_manager_t renderer_mgr;
}


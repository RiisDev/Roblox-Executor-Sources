#pragma once

#include <dependencies/common_includes.hpp>

#include <dependencies/imgui/imgui.h>
#include <android/native_window.h>
#include <dependencies/imgui/imgui_impl_android.h>
#include <dependencies/imgui/imgui_impl_opengl3.h>

#include <GLES3/gl3.h>

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


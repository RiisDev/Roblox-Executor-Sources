#pragma once

#include <string>
#include <functional>

#include <managers/renderer/renderer.hpp>
#include <managers/menu/image.hpp>

namespace menu {
    struct tab_t {
        menu::image_t* icon;
        std::string name; // prolly unused
        std::function<void( ImVec2, ImVec2, float )> render;
    };

    namespace tabs {
        void wip( ImVec2 pos, ImVec2 size, float opacity );

        void code( ImVec2 pos, ImVec2 size, float opacity );
    }
}
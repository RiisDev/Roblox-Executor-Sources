#pragma once

#include <mutex>
#include <atomic>
#include <functional>

#include "../../dependencies/imgui/imgui.h"
#include "../../dependencies/imgui/TextEditor.h"
#include "tabs.hpp"
#include "image.hpp"

namespace managers
{
    class menu_manager_t
    {
    public:
        enum class notification_type_t {
            success,
            error,
            warn,
            info
        };

        struct notification_t {
            notification_type_t tt;
			std::string text;
            notification_t( notification_type_t tt, std::string text );
        };

        std::atomic<bool> authed = false;
        std::atomic<bool> menu_toggled = false;
        std::atomic<bool> game_joined = false;
		
        std::atomic<bool> should_show;
        std::atomic<std::size_t> selected_tab;
        std::vector<menu::tab_t> tabs;
		std::vector<notification_t> notifications;

        std::mutex show_mutex;

        TextEditor editor;
		
        // images
        menu::image_t* run_img = nullptr;
        menu::image_t* trash_img = nullptr;
        menu::image_t* upload_img = nullptr;
        menu::image_t* appico_img = nullptr;
        menu::image_t* cross_img = nullptr;
        menu::image_t* key_img = nullptr;
        menu::image_t* files_img = nullptr;
        menu::image_t* check_icn = nullptr;
        menu::image_t* cross_icn = nullptr;
        menu::image_t* warn_icn = nullptr;
        menu::image_t* info_icn = nullptr;

        void initiate( );
        void dispose( );
        void render( );    
    };

    extern menu_manager_t menu_mgr;
}


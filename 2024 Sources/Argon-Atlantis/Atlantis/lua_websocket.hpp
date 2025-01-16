#pragma once

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include "Luau/lua.h"



namespace atlantis::lua_objects {
    class lua_websocket
    {
    private:
        ix::WebSocket socket = ix::WebSocket(); // Websocket object 

    public:
        lua_State* L; // socket lua thread

        std::string mUrl;
        int L_refid; // socket lua thread Registry Reference ID ( lua_ref )
        int onmessage_refid; // Registry Reference ID ( lua_ref ) for OnMessage
        int onclose_refid; // Registry Reference ID ( lua_ref ) for OnClose
        int onerr_ref; // Registry Reference ID ( lua_ref ) for OnError

        ~lua_websocket() {

            //LOGD("lua_websocket %p is being stopped!", this);

            socket.stop();
        }

        auto stop() -> void {

           // LOGD("lua_websocket %p is being stopped!", this);

            socket.stop();
        }

        auto send_msg(const std::string& msg) -> void
        {
            socket.sendText(msg.c_str());
        }

        auto is_connected() const -> bool
        {
            return socket.getReadyState() == ix::ReadyState::Open;
        }

        auto get_url() const -> std::string
        {
            return mUrl;
        }

        auto setupSocket(const std::string& url) -> bool;
    };
}
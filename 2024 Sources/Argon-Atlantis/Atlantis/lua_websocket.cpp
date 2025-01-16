#include "lua_websocket.hpp"
#include "oxorany/oxorany_include.h"
#include "Luau/lualib.h"
#include <sstream>

auto atlantis::lua_objects::lua_websocket::setupSocket(const std::string& url) -> bool
{
    //LOGD("pluto::websocket::setupSocket");

    ix::SocketTLSOptions tlsOptions;
    tlsOptions.caFile = "ca.crt";
    socket.setTLSOptions(tlsOptions);

    socket.setUrl(url);
    mUrl = url;
    socket.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg)
        {
            switch (msg->type)
            {
            case ix::WebSocketMessageType::Message:
            {
                if (onmessage_refid)
                {
                    lua_getref(L, onmessage_refid);
                    lua_getfield(L, -1, "Fire");
                    {
                        lua_pushvalue(L, -2);
                        lua_pushlstring(L, msg->str.c_str(), msg->str.size());
                    }
                    lua_pcall(L, 2, 0, 0);
                    lua_pop(L, 1);
                }

                break;
            }

            case ix::WebSocketMessageType::Close:
            {
                if (onclose_refid)
                {
                    lua_getref(L, onclose_refid);
                    lua_getfield(L, -1, "Fire");
                    {
                        lua_pushvalue(L, -2);
                    }
                    lua_pcall(L, 1, 0, 0);
                    lua_pop(L, 1);

                    lua_unref(L, onmessage_refid);
                    lua_unref(L, onclose_refid);
                    lua_unref(L, onerr_ref);
                    lua_unref(L, L_refid);
                }

                break;
            }

            case ix::WebSocketMessageType::Error:
            {
                std::stringstream ss;
                ss << "Error: " << msg->errorInfo.reason << "\n";
                ss << "#retries: " << msg->errorInfo.retries << "\n";
                ss << "Wait time(ms): " << msg->errorInfo.wait_time << "\n";
                ss << "HTTP Status: " << msg->errorInfo.http_status << "\n";

                if (onerr_ref)
                {
                    lua_getref(L, onerr_ref);
                    lua_getfield(L, -1, "Fire");
                    {
                        lua_pushvalue(L, -2);
                        lua_pushlstring(L, ss.str().c_str(), ss.str().size());
                    }
                    lua_call(L, 2, 0);
                    lua_pop(L, 1);
                }

                luaL_error(L, E("WebSocket Error: %s"), ss.str().c_str());
                break;
            }

            default: break;
            }
        });

    if (!socket.connect(30).success)
    {
        return false;
    }

    socket.start();
    return true;
}
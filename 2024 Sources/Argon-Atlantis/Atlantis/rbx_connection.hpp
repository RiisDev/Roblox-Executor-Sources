#pragma once

//#include "Execution.hpp"
#include <unordered_map>

namespace argon::lua_objects {
    // We only want one instance of this.
    inline std::unordered_map<slot_t*, int64_t> NodeMap;

    class RBXConnection_t {
    private:
        slot_t* node;
        int64_t sig;
        live_thread_ref* func_ref;

    public:
        RBXConnection_t(slot_t* node) : node(node), sig(node->sig)
        {
            if (node->storage && (node->storage->func_ref != nullptr))
            {
                func_ref = node->storage->func_ref;
            }
        }

        auto get_function_ref() -> int { return func_ref->ref_id; }
        auto get_thread_ref() -> int { return func_ref->thread_id; }
        auto get_luathread() -> lua_State* { return func_ref->th; }
        auto get_node() -> slot_t* { return this->node; }

        auto is_enabled() -> bool { return node->sig != NULL; }

        auto disable() -> void
        {
            if (!NodeMap[node])
            {
                NodeMap[node] = node->sig;
            }
            node->sig = NULL;
        }

        auto enable() -> void
        {
            if (!node->sig)
            { // dont wanna fuck up here
                node->sig = NodeMap[node];
            }
        }
    };
}
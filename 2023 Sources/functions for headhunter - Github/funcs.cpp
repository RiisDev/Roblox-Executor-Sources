int getgenv(uintptr_t rl)
    {
        lua_pushvalue((lua_State*)rl, LUA_GLOBALSINDEX);
        return 1;
    }


    int getreg(uintptr_t rl)
    {
        lua_pushvalue((lua_State*)rl, LUA_REGISTRYINDEX);
        return 1;
    }


    int getrawmetatable(uintptr_t rl)
    {
        if (lua_type((lua_State*)rl, 1) == LUA_TTABLE || lua_type((lua_State*)rl, 1) == LUA_TUSERDATA)
        {
            lua_getmetatable((lua_State*)rl, 1);
            return 1;
        }
        return 0;
    }

    int checkcaller(uintptr_t rl)
    {
        lua_pushboolean((lua_State*)rl, true);
        return 0;
    }


    int httpget(uintptr_t rl)
    {
        std::string url;
        url += lua_tostring((lua_State*)rl, 1);
        std::string content = download_string(url);
        lua_pushstring((lua_State*)rl, content.c_str());
        return 1;
    }
    int setreadonly(uintptr_t rl)
    {
        lua_setreadonly((lua_State*)rl, 1, lua_toboolean((lua_State*)rl, 2));
        return 0;
    }


    int getnamecallmethod(uintptr_t rl)
    {
        if (auto method = *reinterpret_cast<uintptr_t*>(rl + 68))
        {
            lua_pushstring((lua_State*)rl, reinterpret_cast<const char*>(method + 20));
            return 1;
        }
        return 0;
    }

    std::string GetDLLPath()
    {
        HMODULE hModule = GetModuleHandleA(0);
        if (hModule == NULL)
            return "";
        char buffer[MAX_PATH + 1];
        GetModuleFileNameA(hModule, buffer, MAX_PATH);
        std::string::size_type pos = std::string(buffer).find_last_of("\\/");
        return std::string(buffer).substr(0, pos);
    }
    int writefile(lua_State* L)
    {
        std::string filename = lua_tostring(L, 1);
        std::string content = lua_tostring(L, 2);

        std::string path = GetDLLPath() + "\\workspace\\" + filename;

        std::ofstream write(path);
        write << content;
        write.close();

        return 1;
    }

    int readfile(lua_State* L)
    {
        std::string filename = lua_tostring(L, 1);
        std::string path = GetDLLPath() + "\\workspace\\" + filename;

        if (std::filesystem::exists(path) != false)
        {
            std::ifstream f(path, std::ios::in | std::ios::binary);
            const auto sz = std::filesystem::file_size(path);
            std::string result(sz, '\0');
            f.read(result.data(), sz);
            lua_pushstring(L, result.c_str());
        }
        return 1;
    }


    int loadstring(uintptr_t rl)
    {
        size_t l = 0;
        bytecode_encoder_t enc;
        const char* s = lua_tolstring((lua_State*)rl, 1, &l);
        std::string bytecode = Luau::compile(std::string(s, l), {}, {}, &enc);
        std::string compressed = compress_source(bytecode);

        if (r_luavm_load(rl, &compressed, "", 0) == 0)
            return 1;

        lua_pushnil((lua_State*)rl);
        lua_insert((lua_State*)rl, -2);

        return 2;
    }
    int setidentity(uintptr_t rl)
    {
        auto identity = lua_tonumber((lua_State*)rl, 1);

        Roblox::set_identity(rl, identity);

        return 0;
    }
    INPUT Inputs[1] = { 0 };

    int Mouse1Down(lua_State* LS) {
        Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
        SendInput(1, Inputs, sizeof(INPUT));
        return 0;
    }

    int Mouse1Up(lua_State* L) {
        Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
        SendInput(1, Inputs, sizeof(INPUT));
        return 0;
    }

    int Mouse2Down(lua_State* L) {
        Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN;
        SendInput(1, Inputs, sizeof(INPUT));
        return 0;
    }

    int Mouse2Up(lua_State* L) {
        Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP;
        SendInput(1, Inputs, sizeof(INPUT));
        return 0;
    }

    int Mouse1Click(lua_State* L) {
        float n = lua_tonumber(L, -1);
        Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
        SendInput(1, Inputs, sizeof(INPUT));
        Sleep(n);
        Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
        SendInput(1, Inputs, sizeof(INPUT));
        return 0;
    }

    int Mouse2Click(lua_State* L) {
        float n = lua_tonumber(L, -1);
        Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
        SendInput(1, Inputs, sizeof(INPUT));
        Sleep(n);
        Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
        SendInput(1, Inputs, sizeof(INPUT));
        return 0;
    }

    int MouseScroll(lua_State* L) {
        int amount = lua_tonumber(L, -1);
        Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_HWHEEL;
        Inputs[0].mi.mouseData = amount;
        SendInput(1, Inputs, sizeof(INPUT));
        Inputs[0].mi.mouseData = NULL;
        return 0;
    }

    int MouseMoveRelative(lua_State* L) {
        LONG x = lua_tonumber(L, -2);
        LONG y = lua_tonumber(L, -1);
        POINT p;
        if (GetCursorPos(&p))
        {
            Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
            Inputs[0].mi.dx = ((p.x + x) * 0xFFFF / GetSystemMetrics(SM_CXSCREEN) + 1);
            Inputs[0].mi.dy = ((p.y + y) * 0xFFFF / GetSystemMetrics(SM_CYSCREEN) + 1);
            SendInput(1, Inputs, sizeof(INPUT));
        }
        return 0;
    }

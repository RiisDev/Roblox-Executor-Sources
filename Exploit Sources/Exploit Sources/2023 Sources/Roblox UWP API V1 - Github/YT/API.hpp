#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <fstream>
#include <sstream> 
#include <filesystem>

#include "Lua.hpp"
#include "Callcheck.hpp"
#include "Compressor/include/xxhash.h"
#include "Compressor/include/zstd.h"
#include "Compiler/include/BytecodeBuilder.h"
#include "Compiler/include/Compiler.h"
#include "Luau-Core/include/lua.h"
#include "Luau-Core/include/luaconf.h"
#include "Luau-Core/include/lualib.h"
#include <wininet.h>


class bytecode_encoder_t : public Luau::BytecodeEncoder
{
    std::uint8_t encodeOp(const std::uint8_t opcode)
    {
        return opcode * 227;
    }
};


std::string compress(const std::string& data)
{
    std::string output = "RSB1";
    std::size_t dataSize = data.size();
    std::size_t maxSize = ZSTD_compressBound(dataSize);
    std::vector<char> compressed(maxSize);
    std::size_t compSize = ZSTD_compress(&compressed[0], maxSize, data.c_str(), dataSize, ZSTD_maxCLevel());
    output.append(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
    output.append(&compressed[0], compSize);
    std::uint32_t firstHash = XXH32(&output[0], output.size(), 42U);
    std::uint8_t hashedBytes[4];
    std::memcpy(hashedBytes, &firstHash, sizeof(firstHash));
    for (std::size_t i = 0; i < output.size(); ++i)
        output[i] ^= hashedBytes[i % 4] + i * 41U;
    return output;
}


namespace API
{
    int getgenv(uintptr_t rl)
    {
        lua_pushvalue((lua_State*)rl, LUA_GLOBALSINDEX);
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

    int getnamecallmethod(uintptr_t rl)
    {
        if (auto method = *reinterpret_cast<uintptr_t*>(rl + 68))
        {
            lua_pushstring((lua_State*)rl, reinterpret_cast<const char*>(method + 20));
            return 1;
        }
        return 0;
    }

    int checkcaller(uintptr_t rl)
    {
        lua_pushboolean((lua_State*)rl, true);
        return 0;
    }

    int run_script52(uintptr_t rl, std::string source)
    {
        bytecode_encoder_t enc;
        std::string bytecode = Luau::compile(source, {}, {}, &enc);

        if (bytecode.at(0) != 0)
        {
            std::string compressed = compress(bytecode);

            using r_luavm_load_t = uintptr_t(__fastcall*)(uintptr_t rl, std::string* source, const char* chunk, int env);
            r_luavm_load_t r_luavm_load = (r_luavm_load_t)(ASLR(0x91F750));
            r_luavm_load(rl, &compressed, "", 0);

            using r_taskdefer_t = uintptr_t(__cdecl*)(uintptr_t rl);
            r_taskdefer_t r_taskdefer = (r_taskdefer_t)(ASLR(0x85E210));
            r_taskdefer(rl);

            uintptr_t* top_new = reinterpret_cast<uintptr_t*>(rl + 20);
            *top_new -= 16;
            return 0;
        }
        else
        {
            return 1;
        }
    }

    std::string replaceAll1(std::string subject, const std::string& search,
        const std::string& replace) {
        size_t pos = 0;
        while ((pos = subject.find(search, pos)) != std::string::npos) {
            subject.replace(pos, search.length(), replace);
            pos += replace.length();
        }
        return subject;
    }

    std::string download_string(std::string URL) {
        HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
        HINTERNET urlFile;
        std::string rtn;
        if (interwebs) {
            urlFile = InternetOpenUrlA(interwebs, URL.c_str(), NULL, NULL, NULL, NULL);
            if (urlFile) {
                char buffer[2000];
                DWORD bytesRead;
                do {
                    InternetReadFile(urlFile, buffer, 2000, &bytesRead);
                    rtn.append(buffer, bytesRead);
                    memset(buffer, 0, 2000);
                } while (bytesRead);
                InternetCloseHandle(interwebs);
                InternetCloseHandle(urlFile);
                std::string p = replaceAll1(rtn, "|n", "\r\n");
                return p;
            }
        }
        InternetCloseHandle(interwebs);
        std::string p = replaceAll1(rtn, "|n", "\r\n");

    }

    int HttpGet(uintptr_t rl)
    {
        std::string url;
        url += lua_tostring((lua_State*)rl, 1);
        std::string content = download_string(url);
        lua_pushstring((lua_State*)rl, content.c_str());
        return 1;
    }

    int loadstring(lua_State* L)
    {
        if (lua_gettop(L) > 0)
        {
            const char* code = luaL_checkstring(L, 1);

            // Execute the run_script52 function with exthread and code as arguments
            int status = run_script52(exthread, code);

            if (status != 0)
            {
                lua_pushstring(L, "Error executing Lua code");
                return 1; // Return 1 to indicate an error without a traceback
            }
        }
        else
        {
            lua_pushstring(L, "Empty Lua code");
            return 1; // Return 1 to indicate an error without a traceback
        }

        return 0; // Return 0 to indicate success without any additional return values
    }



    int SETLEVEL(lua_State* L)
    {
        const char* s = luaL_checkstring(L, 1);

        run_script52(exthread, "print('Set_LVL To: " + std::string(s) + "')");
        run_script52(exthread, std::string(s));
        return 0;
    }

    int printLEVEL(lua_State* L) {
        std::string source = "print('Exploit LVL: 8')";
        run_script52(exthread, source);
        return 0;
    }

    int SetClipboard(lua_State* L)
    {
        std::size_t Length;
        const char* Setting = lua_tolstring(L, 1, &Length);
        if (OpenClipboard(NULL))
        {
            if (EmptyClipboard())
            {
                HGLOBAL hglbCopy = GlobalAlloc(GMEM_FIXED, Length + 1);
                if (hglbCopy == NULL)
                {
                    CloseClipboard();
                }
                memcpy(hglbCopy, Setting, Length + 1);
                if (!SetClipboardData(CF_TEXT, hglbCopy))
                {
                    CloseClipboard();
                    GlobalFree(hglbCopy);
                }
                CloseClipboard();
            }
            else
            {

            }
        }
        else
        {
        }
        return 0;
    }

    int GetClipboard(lua_State* L)
    {
        std::size_t Length;
        const char* Setting = lua_tolstring(L, 1, &Length);
        if (OpenClipboard(NULL))
        {
            HANDLE Got = GetClipboardData(CF_TEXT);
            if (Got == nullptr)
            {
                CloseClipboard();
            }

            char* pszText = static_cast<char*>(GlobalLock(Got));
            if (pszText == nullptr)
            {
                CloseClipboard();
            }
            std::string text(pszText);
            GlobalUnlock(Got);
            CloseClipboard();
            lua_pushstring(L, text.c_str());
            return 1;
        }
        else
        {
        }
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

    int KeyPress(lua_State* L) {
        WORD key = lua_tonumber(L, -1);
        Inputs[0].ki.wVk = key;
        SendInput(1, Inputs, sizeof(INPUT));
        Inputs[0].ki.wVk = NULL;
        return 0;
    }

    int KeyRelease(lua_State* L) {
        WORD key = lua_tonumber(L, -1);
        Inputs[0].ki.wVk = key;
        Inputs[0].mi.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, Inputs, sizeof(INPUT));
        Inputs[0].mi.dwFlags = NULL;
        Inputs[0].ki.wVk = NULL;
        return 0;
    }

    int KeyTap(lua_State* L) {
        WORD key = lua_tonumber(L, -2);
        int delay = lua_tonumber(L, -1);
        Inputs[0].ki.wVk = key;
        SendInput(1, Inputs, sizeof(INPUT));
        Sleep(delay);
        Inputs[0].mi.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, Inputs, sizeof(INPUT));
        Inputs[0].mi.dwFlags = NULL;
        Inputs[0].ki.wVk = NULL;
        return 0;
    }

    int getreg(lua_State* rl) {
        lua_pushvalue((lua_State*)rl, LUA_REGISTRYINDEX);
        return 1;
    }

    int getrenv(uintptr_t rl)
    {
        lua_pushvalue((lua_State*)LuaState::get_robloxstate(), LUA_GLOBALSINDEX);
        lua_xmove((lua_State*)LuaState::get_robloxstate(), (lua_State*)rl, 1);
        return 1;
    }

    int getsenv(lua_State* rl) {
        lua_pushvalue((lua_State*)LuaState::get_robloxstate(), LUA_GLOBALSINDEX);
        lua_xmove((lua_State*)LuaState::get_robloxstate(), (lua_State*)rl, -1);
        return 1;
        return 1;
    }

    int getfenv(lua_State* rl) {
        lua_pushvalue(rl, LUA_GLOBALSINDEX);
        lua_xmove(rl, rl, 1);
        return 1;
    }

    int set_clipboard(lua_State* L) {
        std::string name = lua_tostring(L, -1);
        OpenClipboard(0);
        EmptyClipboard();
        HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, name.size());
        if (!hg) {
            CloseClipboard();
        }
        memcpy(GlobalLock(hg), name.c_str(), name.size());
        GlobalUnlock(hg);
        SetClipboardData(CF_TEXT, hg);
        CloseClipboard();
        GlobalFree(hg);
        return 1;
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
        const char* filename = lua_tostring(L, 1);
        const char* content = lua_tostring(L, 2);

        std::string path = std::string(getenv("USERPROFILE")) + "\\AppData\\Local\\Packages\\ROBLOXCORPORATION.ROBLOX_55nm5eh3cm0pr\\AC\\workspace";

        std::ofstream write(path);
        if (write.is_open())
        {
            write << content;
            write.close();
            lua_pushboolean(L, true);
        }
        else
        {
            lua_pushboolean(L, false);
        }

        return 1;
    }

    int read_file(lua_State* L)
    {
        const char* filename = lua_tostring(L, 1);

        std::string path = std::string(getenv("USERPROFILE")) + "\\AppData\\Local\\Packages\\ROBLOXCORPORATION.ROBLOX_55nm5eh3cm0pr\\AC\\workspace";

        if (std::filesystem::exists(path))
        {
            std::ifstream f(path, std::ios::in | std::ios::binary);
            if (f.is_open())
            {
                f.seekg(0, std::ios::end);
                std::streamsize fileSize = f.tellg();
                f.seekg(0, std::ios::beg);

                std::string result(fileSize, '\0');
                if (f.read(&result[0], fileSize))
                {
                    lua_pushlstring(L, result.data(), result.size());
                    f.close();
                    return 1;
                }
                else
                {
                    f.close();
                }
            }
        }

        lua_pushnil(L);
        return 1;
    }

    std::string http_request1(const std::string& url) {
        std::string response;

        HINTERNET hInternet = InternetOpen("HTTP Client", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (hInternet == NULL) {
            // Failed to open Internet handle
            return response;
        }

        std::unique_ptr<char[]> urlBuffer(new char[url.size() + 1]);
        strncpy_s(urlBuffer.get(), url.size() + 1, url.c_str(), url.size());

        HINTERNET hConnect = InternetOpenUrlA(hInternet, urlBuffer.get(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (hConnect == NULL) {
            // Failed to open URL
            InternetCloseHandle(hInternet);
            return response;
        }

        constexpr DWORD BufferSize = 4096;
        std::vector<char> buffer(BufferSize);
        DWORD bytesRead = 0;
        while (InternetReadFile(hConnect, buffer.data(), BufferSize, &bytesRead) && bytesRead > 0) {
            response.append(buffer.data(), bytesRead);
        }

        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);

        return response;
    }

    int http_request(lua_State* L) {
        const char* url = luaL_checkstring(L, 1);
        std::string response = http_request1(url);

        lua_pushstring(L, response.c_str());
        return 1;
    }


    int is_closure(lua_State* L) {
        int isClosure = lua_isfunction(L, 1);
        lua_pushboolean(L, isClosure);
        return 1;
    }

    int load_file(lua_State* L) {
        const char* filename = luaL_checkstring(L, 1);
        std::string content;

        std::ifstream file(filename);
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            content = buffer.str();
            file.close();
            lua_pushstring(L, content.c_str());
        }
        else {
            lua_pushnil(L);
        }

        return 1;
    }
    int newcclosure(lua_State* L) {
        luaL_checktype(L, 1, LUA_TFUNCTION); // Ensure that the argument is a function
        lua_pushvalue(L, 1); // Push a copy of the function onto the stack
        return 1; // Return 1 to indicate the number of values pushed onto the stack
    }


    int hookmetamethod(lua_State* L) {
        int originalMethodRef = lua_tointeger(L, lua_upvalueindex(1));

        lua_rawgeti(L, LUA_REGISTRYINDEX, originalMethodRef);

        lua_insert(L, 1);
        lua_call(L, lua_gettop(L) - 1, LUA_MULTRET);

        return lua_gettop(L);
    }

    int setidentity(uintptr_t rl)
    {
        auto identity = lua_tonumber((lua_State*)rl, 1);

        Lua::setlevel(exthread, identity);//exthread

        return 0;
    }

    int setreadonly(uintptr_t rl)
    {
        lua_setreadonly((lua_State*)rl, 1, lua_toboolean((lua_State*)rl, 2));
        return 0;
    }

    void registration(lua_State* rl)
    {
        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)setreadonly), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "setreadonly");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)setidentity), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "setidentity");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)hookmetamethod), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "hookmetamethod");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)is_closure), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "iscclosure");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)is_closure), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "islclosure");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)newcclosure), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "newcclosure");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)load_file), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "loadfile");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)http_request), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "http_request");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)writefile), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "writefile");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)read_file), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "readfile1");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)set_clipboard), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "set_clipboard");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)set_clipboard), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "setclipboard");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)printLEVEL), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "printLEVEL");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)getsenv), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "getsenv");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)getfenv), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "getfenv");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)KeyTap), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "KeyTap");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)KeyRelease), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "KeyRelease");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)KeyPress), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "KeyPress");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)MouseScroll), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "MouseScroll");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)Mouse2Click), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "Mouse2Click");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)Mouse1Click), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "mouse1press");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)Mouse2Up), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "Mouse2Up");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)Mouse2Down), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "Mouse2Down");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)Mouse1Up), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "Mouse1Up");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)Mouse1Down), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "mouse1release");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)SetClipboard), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "SetClipboard");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)GetClipboard), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "GetClipboard");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)getgenv), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "getgenv");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)getreg), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "getreg");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)getrenv), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "getrenv");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)getrawmetatable), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "getrawmetatable");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)getnamecallmethod), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "getnamecallmethod");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)setreadonly), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "setreadonly");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)checkcaller), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "checkcaller");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)HttpGet), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "HttpGet");

        lua_pushcclosure((lua_State*)rl, (lua_CFunction)ConvertClosure((uintptr_t)loadstring), 0, 0);
        lua_setfield((lua_State*)rl, LUA_GLOBALSINDEX, "loadstring");
    }
}

// NOTE: Some functions are skidded from https://v3rmillion.net/showthread.php?tid=1104309

#include "functions.h"

#pragma warning(disable : 4733 28183 6387)

CONTEXT context_record;
BYTE* int3_address = 0;

int error(UINT rL, const char* error) {
    pushinteger(rL, 9992);
    pushstring(rL, error);
    return 2;
}

bool inline isrbxactive() {
    HWND selected = GetForegroundWindow();
    DWORD selected_id;
    GetWindowThreadProcessId(selected, &selected_id);
    DWORD current_id = GetCurrentProcessId();
    return selected_id == current_id;
}

uint8_t inline getidentity(UINT rL) {
    return structptr_get(structptr_get(rL, UINT, rL, userdata), uint8_t, rextraspace, identity);
}

// Thank you Nezy!
std::string compress(const std::string& data) {
    std::string output = XorString("RSB1");
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

const int arg_offset = 3;
#define RETNUM_CALLER_UNAUTHORIZED -1
#define getarg(n) (index2addr(rL, arg_offset + n))
INPUT Inputs[1]{};
int c_function_handler_internal(UINT rL) {
    bool trusted_caller = false;
    //printf("Identity: %d\n", getidentity(rL));
    switch (getidentity(rL)) {
    case execution_level:
        // exploit threads can bypass the key
        trusted_caller = true;
        break;
    default:
        // roblox threads will need to provide a key
        //printf("Key provided: %s\n", struct_get(*(UINT*)(getarg(-1)), const char*, rTString, data));
        //printf("Key required: %s\n", hwid.c_str());
        if (strcmp(struct_get(*(UINT*)(getarg(-1)), const char*, rTString, data), hwid.c_str()) == 0) {
            trusted_caller = true;
        }
        break;
    }
    if (!trusted_caller) {
        return RETNUM_CALLER_UNAUTHORIZED;
    }
    UINT ts_function_name = (UINT)(tsvalue(getarg(0)));
    std::string function_name = struct_get(ts_function_name, const char*, rTString, data);
    //printf("%X\n", (ULONG_PTR)__readfsdword(PcTeb));
    //printf(XorString("Calling %s\n"), function_name.c_str());
    if (function_name == XorString("getgenv")) {
        pushvalue(rL, LUA_GLOBALSINDEX);
        return 1;
    } else if (function_name == XorString("getreg")) {
        pushvalue(rL, LUA_REGISTRYINDEX);
        return 1;
    } else if (function_name == XorString("getrenv")) {
        // get a state from roblox
        UINT scriptstate = get_rL((UINT)instance::FindFirstChildOfClass(get_datamodel(), XorString("ScriptContext")));
        pushvalue(scriptstate, LUA_GLOBALSINDEX); // getgenv
        xmove(scriptstate, rL, 1);
        return 1;
    } else if (function_name == XorString("loadstring")) {
        roblox_encoder encoder{};
        std::string whole_script = struct_get(tsvalue(getarg(1)), const char*, rTString, data);
        std::string compiled = Luau::compile(whole_script, {}, {}, &encoder);
        UINT8 version = compiled.c_str()[0];
        if (version < LBC_VERSION_MIN || version > LBC_VERSION_MAX) {
            functions::rbx_printf(Error, XorString("[TaaprWare V2] Loadstring Syntax Error: %s"), compiled.c_str() + 1);
            printf(XorString("Compile error in loadstring (check roblox output)\n"));
        }
        std::string compressed = compress(compiled);
        if (getarg(2)->tt == LUA_TSTRING) {
            functions::load(rL, &compressed, struct_get(tsvalue(getarg(2)), const char*, rTString, data), 0);
        } else {
            functions::load(rL, &compressed, XorString("TaaprWareV2_loadstring"), 0);
        }
        return 1;
    } else if (function_name == XorString("getthread")) {
        pushunsigned(rL, rL);
        return 1;
    } else if (function_name == XorString("getaddress")) {
        pushunsigned(rL, (UINT)(getarg(1)));
        return 1;
    } else if (function_name == XorString("getinstanceaddress")) {
        pushunsigned(rL, structptr_get(*(UINT*)(getarg(1)), UINT, rUdata, data));
        return 1;
    } else if (function_name == XorString("__debugbreak")) {
        __debugbreak();
        return 0;
    } else if (function_name == XorString("getclipboard")) {
        OpenClipboard(NULL);
        HANDLE hData = GetClipboardData(CF_TEXT);
        char* pszText = (char*)GlobalLock(hData);
        char* pszTextCopy = (char*)malloc(GlobalSize(hData) + 1);
        strcpy(pszTextCopy, pszText);
        GlobalUnlock(hData);
        CloseClipboard();
        pushstring(rL, pszTextCopy);
        return 1;
    } else if (function_name == XorString("setclipboard")) {
        const char* text = struct_get(tsvalue(getarg(1)), const char*, rTString, data);
        OpenClipboard(NULL);
        EmptyClipboard();
        HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE, strlen(text) + 1);
        char* pszText = (char*)GlobalLock(hData);
        strcpy(pszText, text);
        GlobalUnlock(hData);
        SetClipboardData(CF_TEXT, hData);
        CloseClipboard();
        return 0;
    } else if (function_name == XorString("isfile")) {
        pushboolean(rL, isfile(struct_get(tsvalue(getarg(1)), const char*, rTString, data)));
        return 1;
    } else if (function_name == XorString("isfolder")) {
        pushboolean(rL, isfolder(struct_get(tsvalue(getarg(1)), const char*, rTString, data)));
        return 1;
    } else if (function_name == XorString("readfile")) {
        const char* contents = readfile(struct_get(tsvalue(getarg(1)), const char*, rTString, data));
        if (contents) {
            pushstring(rL, contents);
            return 1;
        } else {
            return error(rL, XorString("Failed readfile"));
        }
    } else if (function_name == XorString("delfile")) {
        delfile(struct_get(tsvalue(getarg(1)), const char*, rTString, data));
        return 0;
    } else if (function_name == XorString("delfolder")) {
        delfolder(struct_get(tsvalue(getarg(1)), const char*, rTString, data));
        return 0;
    } else if (function_name == XorString("makefolder")) {

        return 0;
    } else if (function_name == XorString("writefile")) {
        bool success = writefile(struct_get(tsvalue(getarg(1)), const char*, rTString, data), struct_get(tsvalue(getarg(2)), const char*, rTString, data));
        if (!success) {
            return error(rL, XorString("Failed writefile"));
        }
        return 0;
    } else if (function_name == XorString("listfiles")) {
        std::vector<std::string>* paths = listfiles(struct_get(tsvalue(getarg(1)), const char*, rTString, data));
        if (!paths) {
            return error(rL, XorString("Failed listfiles"));
        }
        for (std::string file : *paths) {
            pushstring(rL, file.c_str());
        }
        return paths->size();
    } else if (function_name == XorString("listfolders")) {
        std::vector<std::string>* paths = listfolders(struct_get(tsvalue(getarg(1)), const char*, rTString, data));
        if (!paths) {
            return error(rL, XorString("Failed listfolders"));
        }
        for (std::string file : *paths) {
            pushstring(rL, file.c_str());
        }
        return paths->size();
    } else if (function_name == XorString("gettop")) {
        pushinteger(rL, gettop(rL));
        return 1;
    } else if (function_name == XorString("settop")) {
        settop(rL, (int)(nvalue(getarg(1))));
        return 0;
    } else if (function_name == XorString("gethash")) {
        pushunsigned(rL, structptr_get(tsvalue(getarg(1)), UINT, rTString, hash));
        return 1;
    } else if (function_name == XorString("sethash")) {
        structptr_set(tsvalue(getarg(1)), UINT, rTString, hash, (unsigned)nvalue(getarg(2)));
        return 0;
    } else if (function_name == XorString("getfpscap")) {
        pushnumber(rL, get_fps());
        return 1;
    } else if (function_name == XorString("setfpscap")) {
        set_fps(nvalue(getarg(1)));
        return 0;
    } else if (function_name == XorString("pushstring")) {
        pushstring(rL, XorString("The string has been pushed."));
        return 1;
    } else if (function_name == XorString("messagebox")) {
        MessageBoxA(NULL, struct_get(tsvalue(getarg(1)), const char*, rTString, data), struct_get(tsvalue(getarg(2)), const char*, rTString, data), (UINT)(nvalue(getarg(3))));
        return 0;
    } else if (function_name == XorString("rconsoleprint")) {
        std::cout << struct_get(tsvalue(getarg(1)), const char*, rTString, data);
        return 0;
    } else if (function_name == XorString("rconsoleinput")) {
        std::string str;
        std::getline(std::cin, str);
        pushstring(rL, str.c_str());
        return 1;
    } else if (function_name == XorString("rconsoleclear")) {
        COORD tl = {0,0};
        CONSOLE_SCREEN_BUFFER_INFO s;
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(console, &s);
        DWORD written, cells = s.dwSize.X * s.dwSize.Y;
        FillConsoleOutputCharacterA(console, ' ', cells, tl, &written);
        FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
        SetConsoleCursorPosition(console, tl);
        return 0;
    } else if (function_name == XorString("rconsolesettitle")) {
        SetConsoleTitleA(struct_get(tsvalue(getarg(1)), const char*, rTString, data));
        return 0;
    } else if (function_name == XorString("mousemoveabs")) {
        Inputs[0].mi.mouseData = NULL;
        LONG x = (long)nvalue(getarg(1));
        LONG y = (long)nvalue(getarg(2));
        Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
        Inputs[0].mi.dx = (x * 0xFFFF / GetSystemMetrics(SM_CXSCREEN) + 1);
        Inputs[0].mi.dy = (y * 0xFFFF / GetSystemMetrics(SM_CYSCREEN) + 1);
        SendInput(1, Inputs, sizeof(INPUT));
        return 0;
    } else if (function_name == XorString("mousemoverel")) {
        Inputs[0].mi.mouseData = NULL;
        LONG x = (long)nvalue(getarg(1));
        LONG y = (long)nvalue(getarg(2));
        POINT p;
        if (GetCursorPos(&p)) {
            Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
            Inputs[0].mi.dx = ((p.x + x) * 0xFFFF / GetSystemMetrics(SM_CXSCREEN) + 1);
            Inputs[0].mi.dy = ((p.y + y) * 0xFFFF / GetSystemMetrics(SM_CYSCREEN) + 1);
            SendInput(1, Inputs, sizeof(INPUT));
        }
        return 0;
    } else if (function_name == XorString("mousescroll")) {
        int amount = (int)nvalue(getarg(1));
        Inputs[0].type = INPUT_MOUSE;
        Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_HWHEEL;
        Inputs[0].mi.mouseData = amount;
        SendInput(1, Inputs, sizeof(INPUT));
        return 0;
    } else if (function_name == XorString("isrbxactive")) {
        pushboolean(rL, isrbxactive());
        return 1;
    } else if (function_name == XorString("rawprint")) {
        functions::rbx_printf((PrintType)nvalue(getarg(1)), struct_get(tsvalue(getarg(2)), const char*, rTString, data));
        return 0;
    } else if (function_name == XorString("getidentity")) {
        pushunsigned(rL, getidentity(rL));
        return 1;
    } else if (function_name == XorString("setidentity")) {
        structptr_set(structptr_get(rL, UINT, rL, userdata), uint8_t, rextraspace, identity, (UINT)nvalue(getarg(1)));
        return 0;
    } else if (function_name == XorString("setrawmetatable")) {
        StkId object = (getarg(1));
        switch (object->tt) {
        case LUA_TUSERDATA:
            offsets::rUdata::metatable_set(*(UINT*)(object) + offsets::rUdata::metatable, struct_get(tsvalue(getarg(2)), UINT, rTString, data));
            break;
        case LUA_TTABLE:
            offsets::rTable::metatable_set(*(UINT*)(object) + offsets::rTable::metatable, struct_get(tsvalue(getarg(2)), UINT, rTString, data));
            break;
        default:
            pushnil(rL);
            break;
        }
        return 0;
    } else if (function_name == XorString("getrawmetatable")) {
        // when confused, paste pseudocode
        // after more thought, i believe the problems were caused from doing (object->value.gc.h) instead of (*(UINT*)object)
        UINT metatable;
        StkId object = (getarg(1));
        switch (object->tt) {
        case LUA_TUSERDATA:
            metatable = offsets::rUdata::metatable_get(*(UINT*)(object) + offsets::rUdata::metatable);
            sethvalue(rL, rL_get(StkId, top), metatable);
            break;
        case LUA_TTABLE:
            metatable = offsets::rTable::metatable_get(*(UINT*)(object) + offsets::rTable::metatable);
            sethvalue(rL, rL_get(StkId, top), metatable);
            break;
        default:
            pushnil(rL);
            break;
        }
        api_incr_top(rL);
        return 1;
    } else if (function_name == XorString("setreadonly")) {
        *(uint8_t*)(*(UINT*)(getarg(1)) + offsets::rTable::readonly) = bvalue(getarg(2));
        return 0;
    } else if (function_name == XorString("getnamecallmethod")) {
        setsvalue(rL, rL_get(StkId, top), structptr_get(rL, UINT, rL, namecall));
        api_incr_top(L)
        return 1;
    } else if (function_name == XorString("setnamecallmethod")) {
        structptr_set(rL, UINT, rL, namecall, tsvalue(getarg(2)));
        return 0;
    } else if (function_name == XorString("getsimulationradius")) {
        UINT player_addy = structptr_get(*(UINT*)(getarg(1)), UINT, rUdata, data);
        pushnumber(rL, (double)(*reinterpret_cast<float*>(player_addy + offsets::rPlayer::simulationradius)));
        return 1;
    } else if (function_name == XorString("setsimulationradius")) {
        UINT player_addy = structptr_get(*(UINT*)(getarg(1)), UINT, rUdata, data);
        *reinterpret_cast<float*>(player_addy + offsets::rPlayer::maximumsimulationradius) = (float)nvalue(getarg(2));
        *reinterpret_cast<float*>(player_addy + offsets::rPlayer::simulationradius) = (float)nvalue(getarg(2));
        return 0;
    } else if (function_name == XorString("gethwid")) {
        pushstring(rL, hwid.c_str());
        return 1;
    } else if (function_name == XorString("hookfunction")) {
        StkId f1 = getarg(1);
        StkId f2 = getarg(2);
        printf(XorString("start\n"));
        memcpy(*(UINT**)(f1), *(UINT**)(f1), sizeof(GCHeader_size));
        printf(XorString("end\n"));
        return 0;
    }
    //__debugbreak();
    //pushvalue(rL, LUA_REGISTRYINDEX);
    //pushvalue(rL, LUA_ENVIRONINDEX);
    //pushvalue(rL, LUA_GLOBALSINDEX);
    return error(rL, XorString("Invalid function!"));
}

//uintptr_t SEH_offset = base + 0x10172B5;
int retnum = 0;
int __declspec(naked) c_function_handler(UINT rL) {
    //printf("%d trusted\n", getidentity(rL));
    retnum = c_function_handler_internal(rL); // To freely make custom functions without the restrictions of __declspec(naked)
    if (retnum == RETNUM_CALLER_UNAUTHORIZED) {
        __asm {
            rbx_writevoxels_overwritten_instructions;
            mov ebx, addresses::rbx_writevoxels; // Address we jumped from
            add ebx, rbx_writevoxels_instsize; // Address to the next instruction
            push ebx; // Push address to stack
            ret; // Pop address from stack and jump to it
        }
    }

    // Epilogue of WriteVoxels (show stack pointer in your disassembler to help you update this if changed)
    __asm {
        mov eax, retnum;
        pop edi;
        pop esi;
        mov ecx, [ebp - 0xC];
        mov fs:[0], ecx;
        mov esp, ebp;
        pop ebp;
        retn 4;
    }
}
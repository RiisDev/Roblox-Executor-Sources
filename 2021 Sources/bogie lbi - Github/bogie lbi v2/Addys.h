const auto ModuleHandle = reinterpret_cast<std::uintptr_t>(GetModuleHandleA(NULL));
const auto aslr(std::uintptr_t Address) { return (Address - 0x400000u + ModuleHandle); }

//Offsets
constexpr auto instance = 12;

//Addresses
const auto lua_getfield_addr = aslr(0x01648E00u);
const auto lua_setfield_addr = aslr(0x0164A4A0u);
const auto lua_newthread_addr = aslr(0x01649310u);
const auto lua_pushlstring_addr = aslr(0x01649890u);
const auto lua_settop_addr = aslr(0x0164A7C0u);
const auto lua_pcall_addr = aslr(0x01649580u);
const auto luau_deserialize_addr = aslr(0x01655490u);
const auto getdatamodel_addr = aslr(0x01077B30u);
const auto getdatamodel2_addr = aslr(0x010779E0u);

//Calling conventions
#define lua_getfield_CCV __cdecl
#define lua_setfield_CCV __cdecl
#define lua_newthread_CCV __cdecl
#define lua_pushlstring_CCV __cdecl
#define lua_settop_CCV __cdecl
#define lua_pcall_CCV __cdecl
#define luau_deserialize_CCV __cdecl
#define getdatamodel_CCV __thiscall
#define getdatamodel2_CCV __cdecl

const auto r_lua_getfield = reinterpret_cast<void(lua_getfield_CCV*)(std::uintptr_t, int, const char*)>(unprotect(lua_getfield_addr));
const auto r_lua_setfield = reinterpret_cast<void(lua_setfield_CCV*)(std::uintptr_t, int, const char*)>(unprotect(lua_setfield_addr));
const auto r_lua_newthread = reinterpret_cast<std::uintptr_t(lua_newthread_CCV*)(std::uintptr_t)>(unprotect(lua_newthread_addr));
const auto r_lua_pushlstring = reinterpret_cast<void(lua_pushlstring_CCV*)(std::uintptr_t, const char*, size_t)> (unprotect(lua_pushlstring_addr));
const auto r_lua_settop = reinterpret_cast<void(lua_settop_CCV*)(std::uintptr_t, int)>(unprotect(lua_settop_addr));
const auto r_lua_pcall = reinterpret_cast<void(lua_pcall_CCV*)(std::uintptr_t, int, int, int)>(unprotect(lua_pcall_addr));
const auto r_lua_pop(std::uintptr_t state, int n) { r_lua_settop(state, -(n)-1); }

const auto deserialize = reinterpret_cast<void(luau_deserialize_CCV*)(std::uintptr_t, const char*, const char*, size_t, bool)>(unprotect(luau_deserialize_addr));
const auto getdatamodel1 = reinterpret_cast<void(getdatamodel_CCV*)(std::uintptr_t, std::uintptr_t)>(unprotect(getdatamodel_addr));
const auto getdatamodel2 = reinterpret_cast<std::uintptr_t(getdatamodel2_CCV*)()>(unprotect(getdatamodel2_addr));

const auto state_get(std::uintptr_t ScriptContext) { return  *reinterpret_cast<std::uintptr_t*>(ScriptContext + 276) + (ScriptContext + 276); }

const auto getdatamodel() {
    std::uintptr_t pad[8];
    getdatamodel1(getdatamodel2(), reinterpret_cast<std::uintptr_t>(pad));
    return pad[0] + instance;
}

const auto findfirstchildofclass(std::uintptr_t inst, const char* nclass)
{
    const auto start = *reinterpret_cast<std::uintptr_t*>(inst + 44);
    const auto end = *reinterpret_cast<std::uintptr_t*>(start + 4);

    for (int i = *reinterpret_cast<int*>(start); i != end; i += 8)
        if (!memcmp(reinterpret_cast<const char*>((*(int(**)(void))(*reinterpret_cast<int*>(*reinterpret_cast<int*>(i)) + 16))()), nclass, strlen(nclass)))   
            return *reinterpret_cast<int*>(i);
    
    return 0;
}

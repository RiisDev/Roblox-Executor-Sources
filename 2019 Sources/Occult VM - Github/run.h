BOOL Occult_luaA_run(DWORD rL, const char* src)
{
// DbgPrint("\nIn luaA_execute\n");
Proto* fptn;
LClosure* lcl;
TValue* cl;
// DbgPrint("realloc: %x\n", (DWORD)r_luaM_realloc_);
//DbgPrint("r_luaD_precall: %x\nr_luaV_gettable: %x\nXorConstant: %x\n", (DWORD)r_luaD_precall, (DWORD)r_luaV_gettable, (DWORD)XorConstant);
lua_State* L = luaL_newstate();
luaL_loadstring(L, src);
if (lua_type(L, -1) != LUA_TFUNCTION) {
printf("[Parsetime-Error] %s\n", lua_tostring(L, -1));
lua_pop(L, -1);
return FALSE;
}
cl = (L->top - 1);
lcl = &(((Closure*)((L->top - 1)->value.p))->l);
fptn = lcl->p;
//DbgPrint("(o)Lua State: %x\nProto: %x", (DWORD)L, (DWORD)fptn);
//DbgPrint("\n/!\\ Launching Roblox's virtual machine /!\\\n");
DWORD th = lua::rblua_newthread(rL);

//Occult_luaB_pop(th, 1);



//DbgPrint("Initiated roblox thread.\n");
//DbgPrint("kek2.\n");

//DbgPrint("Set globals.\n");



try {
if (luaD_precall(L, cl, 0) == 0) {
// DbgPrint("luaD_precall return'd 0 executing.\n");
rev_luaV_execute(th, L, 1);
}
}
catch (std::exception e) {
printf("[Runtime-Error] %s\n", e.what());
}

return TRUE;
}

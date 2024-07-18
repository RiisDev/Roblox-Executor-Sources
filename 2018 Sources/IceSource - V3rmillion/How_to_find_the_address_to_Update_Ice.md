# To update Ice you need to update the address, use IDA to find the correct addresses for your needs from this list
## Guide by Rover656(so ask him if you don't understand)
- lua_getfield (With 3 args) – "isdst” – Pseudocode look for sub_xxxxxx(a2, -2, "isdst”)
- lua_getfield (With 4 args) – "tostring” – Pseudocode look for sub_xxxxxx(v2, -10002, "tostring”, -1)
- lua_setfield – "noise” – Pseudocode look for sub_xxxxxx(a1, -2, "noise”)
- lua_tolstring – "Unprotected error in call to Lua API (%s)\n” – Pseudocode look for v1 = sub_xxxxxx(a1, -1, 0);
- lua_settop – "name conflict for module '%s'" – Disassembly view, sub_xxxxxx 1st call
- lua_toboolean  "%s: %p” –  Pseudocode look for case 3: v6=sub_xxxxxx(a1, 1)
- lua_pushvalue - "__index" - Disassembly First Call
- lua_pushnumber - "huge" - Pseudocode look for sub_xxxxxx(a1, qword_xxxxxx); above sub_xxxxxx(a1, -2, "huge");
- lua_pushstring - "The metatable is locked" - Pseudocode look for sub_xxxxxx(a1, "The metatable is locked");
- lua_pcall - "*** value not found ***" - Pseudocode look for if (sub_xxxxxx(v10, 0, 1, 0))
- lua_pushnil - "foreach" - Sub below the foreach offset, Pseudocode for sub_xxxxxx(a1); above if (sub_xxxxxx(a1, 1))
- lua_next - "foreach" - Sub below the foreach offset, Psuedocode for if (sub_xxxxxx(a1, 1))
- lua_pushboolean - "collect" - Pseudocode look for sub_xxxxxx(a1, v5); in the else if statement
- lua_tonumber - "tables cannot be cyclic" - Pseudocode look for case 2: v47 = sub_xxxxxx(a1, v7);
- lua_pushcclosure - "noise" - Pseudocode look for sub_xxxxxx(a1, sub_xxxxxx, 0); above sub_xxxxxx(a1, -2, "noise");
- luaD_precall - "C stack overflow" - Pseudocode look for if ( !sub_xxxxxx(a1, v6, 1) )
- lua_checkstack - "stack overflow (%s)" - Pseudocode look for if ( !sub_xxxxxx(a1, 40) )
- lua_createtable - "The metatable is locked" - Pseudocode look for sub_xxxxxx(a1, 0, 0); (occurs twice)
- lua_insert - "'setfenv' cannot change environment of given object" - Pseudocode for sub_xxxxxx(a1, -2); (middle one)
- lua_newthread - "oldResult, moduleRef  = ...\nnewResult " ... (string is very long) - Pseudocode look for v9 = sub_xxxxxx(a2);
- lua_newuserdata - "newproxy only supports the arguments nil and true" - Pseudocode look for v1 = sub_xxxxxx(a1, 0);
- lua_pushfstring - "%s expected, got %s" - Pseudocode look for v2 = sub_xxxxxx(a1, (int)"%s expected, got %s", (unsigned int)"table");
- lua_pushthread - "function" - Psuedocode look for sub_xxxxxx(a1); 2 spaces up from (avoid 2 subs from) sub_xxxxxx(a2, "function")
- lua_rawgeti - "wrong number of arguments to 'insert'" - Pseudocode look for sub_xxxxxx(v1, 1, v4 - 1);
- lua_rawseti - "wrong number of arguments to 'insert'" - Pseudocode look for sub_xxxxxx(v1, 1, v4--);
- lua_rawget - "function" - Pseudocode find sub_xxxxxx(a1, v2); above sub_xxxxxx(a1, "function");
- lua_rawset - "newproxy only supports the arguments nil and true" - Pseudocode find sub_xxxxxx(a1, -10003);
- lua_remove - "cannot %s %s: %s" - Pseudocode look for sub_xxxxxx(a1, a3);
- lua_setfenv - "'setfenv' cannot change environment of given object" - Pseudocode find !sub_xxxxxx(a1, -2) as part of the last if statement
- lua_setmetatable - "kv" - Pseudocode find sub_xxxxxx(a1, -2); above sub_xxxxxx(a1, "kv", 2);
- lua_getmetatable - "value expected" - Pseudocode find if ( sub_xxxxxx(a1, 1) )
- lua_xmove - "Lua function expected" - Pseudocode find sub_xxxxxx(a1, v4, 1); above return 1;
- lua_close - "Changed" - Pseudocode look for sub_xxxxxx(&v43); before the } of the contained if statement
- lua_touserdata - "%s expected, got %s" - Psuedocode look for v2 = (__int64 *)sub_xxxxxx(a1, 2);
- lua_call - "invalid replacement value (a %s)" - sub_xxxxxx(v4, v8, 1); above break for case 6
- lua_type - "tables cannot be cyclic" - Pseudocode look for switch ( sub_xxxxxx(a1, a2) )
- luaS_newlstr - "not enough memory" - Pseudocode look for v4 = sub_xxxxxx(a1, (int)"not enough memory", 0x11u);
- luaV_gettable - "'for' step must be a number" - Pseudocode look for sub_xxxxxx(a3, v326 + 16 * (v14 & 0x1FF), v27, v320, 1); (around lines 443)
- luaV_settable - "perform arithmetic on" - Pseudocode look for sub_xxxxxx(a3, &v292, v322 + 16 * (v14 & 0x3FFFF), v326 + 16 * (unsigned __int8)(v14 >> 18)); (around lines 452)
- lua_resume - "too many arguments to resume" - Pseudocode look for v5 = sub_xxxxxx(a2, a3);
- luastate - "Unable to create a new thread for %s" - look for the definition of v50
- XORCONS:
"%s expected, got %s" should appear as v2 = sub_72B570(a1, (int)"%s expected, got %s", (unsigned int)"table");
Find:
```
    v7 = *(_DWORD *)(a1 + 12);
    *(_QWORD *)v7 = (unsigned __int128)_mm_xor_ps(
    (__m128)COERCE_UNSIGNED_INT64((double)v6),
    *(__m128 *)&dword_xxxxxxx); //This here is your address
```

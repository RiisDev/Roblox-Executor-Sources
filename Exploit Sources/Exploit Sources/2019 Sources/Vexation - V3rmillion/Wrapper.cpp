static int CallMethod(lua_State * L) {
  try {
    RLua::lua_getfield(RLua::lua_State, -2, wtf.c_str());
    RLua::lua_pushvalue(RLua::lua_State, -2);
    int ArgCount = 0;
    for (int x = 1; x != lua_gettop(L) + 1; x += 1) {
      switch (lua_type(L, x)) {
      case LUA_TFUNCTION:
        //RLua::lua_pushstring(RLua::lua_State, "lol-e a function");
        break;
      case LUA_TNIL: {
        break;
      }
      case LUA_TNUMBER: {
        double no = lua_tonumber(L, x);
        RLua::lua_pushnumber(RLua::lua_State, no);
        break;
      }
      case LUA_TSTRING: {
        const char * str = lua_tostring(L, x);
        RLua::lua_pushstring(RLua::lua_State, str);
        break;
      }

      case LUA_TUSERDATA: {
        lua_getmetatable(L, x);
        lua_getfield(L, -1, "__type");
        const char * type = lua_tostring(L, -1);
        lua_pop(L, 2);
        if (strcmp(type, "Object") == 0) {

          int * u = (int * ) lua_touserdata(L, x);
          int * userdata = (int * ) RLua::lua_newuserdata(RLua::lua_State, 8);
          * userdata = * u;
          *(userdata + 1) = 0;
          RLua::lua_getfield(RLua::lua_State, -10000, "Object");
          RLua::lua_setmetatable(RLua::lua_State, -2);

        } else if (strcmp(type, "CFrame") == 0) {
          CFrame * u = (CFrame * ) lua_touserdata(L, x);
          CFrame * cf = (CFrame * ) RLua::lua_newuserdata(RLua::lua_State, sizeof(CFrame));
          * cf = * u;
          RLua::lua_getfield(RLua::lua_State, -10000, "CFrame");
          RLua::lua_setmetatable(RLua::lua_State, -2);
        }

        /*
        else if (strcmp(type, "RBXScriptSignal") == 0) {
        CFrame* u = (CFrame*)lua_touserdata(L, x);
        CFrame* cf = (CFrame*)RLua::lua_newuserdata(RLua::lua_State, sizeof(CFrame));
        *cf = *u;
        RLua::lua_getfield(RLua::lua_State, -10000, "RBXScriptSignal");
        RLua::lua_setmetatable(RLua::lua_State, -2);
        }
        */
        break;
      }
      case LUA_TTABLE: {
        lua_getfield(L, x, "__address");
        if (!lua_isnoneornil(L, -1)) {
          int * userdata = (int * ) RLua::lua_newuserdata(RLua::lua_State, 8);
          * userdata = lua_tonumber(L, -1);
          *(userdata + 1) = 0;
          RLua::lua_getfield(RLua::lua_State, -10000, "Object");
          RLua::lua_setmetatable(RLua::lua_State, -2);
          lua_pop(L, 1);
          break;
        } else {
          lua_pop(L, 1);
          lua_getfield(L, x, "type");
          if (!lua_isnil(L, -1)) {
            lua_pop(L, 1);
            lua_pushvalue(L, x);
            TransferCFrameToRoblox(L);
            lua_pop(L, 1);
            break;
          }

          lua_pop(L, 1);
          lua_getfield(L, x, "v3");
          if (!lua_isnil(L, -1)) {
            lua_pop(L, 1);
            lua_pushvalue(L, x);
            TransferVector3ToRoblox(L);
            lua_pop(L, 1);
            break;
          }

          lua_pop(L, 1);
          lua_getfield(L, x, "cf");
          if (!lua_isnil(L, -1)) {
            lua_pop(L, 1);
            lua_pushvalue(L, x);
            TransferCFrameAnglesToRoblox(L);
            lua_pop(L, 1);
            break;
          }

          lua_pop(L, 1);
          lua_getfield(L, x, "v2");
          if (!lua_isnil(L, -1)) {
            lua_pop(L, 1);
            lua_pushvalue(L, x);
            TransferVector2ToRoblox(L);
            lua_pop(L, 1);
            break;
          }

          lua_pop(L, 1);
          lua_getfield(L, x, "u2");
          if (!lua_isnil(L, -1)) {
            lua_pop(L, 1);
            lua_pushvalue(L, x);
            TransferUDim2ToRoblox(L);
            lua_pop(L, 1);
            break;
          }

          lua_pop(L, 1);
          lua_getfield(L, x, "c3");
          if (!lua_isnil(L, -1)) {
            lua_pop(L, 1);
            lua_pushvalue(L, x);
            TransferColor3ToRoblox(L);
            lua_pop(L, x);
            break;
          }

          lua_pop(L, 1);
          lua_getfield(L, x, "bc");
          if (!lua_isnil(L, -1)) {
            lua_pop(L, 1);
            lua_pushvalue(L, x);
            TransferBrickColorToRoblox(L);
            lua_pop(L, 1);
            break;
          }
        }
      }
      default: {
        const char * str = lua_tostring(L, x * -1);
        RLua::lua_pushstring(RLua::lua_State, str);
        break;
      }
      }
      ArgCount += 1;
    }
    try {
      RLua::lua_pcall(RLua::lua_State, ArgCount, 1, 0);
      if (RLua::lua_touserdata(RLua::lua_State, -1) != RLUA_TNIL) {
        PushInstance(L, *(int * ) RLua::lua_touserdata(RLua::lua_State, -1));
      }

    } catch (std::exception e) {
      luaL_error(L, e.what());
      return 0;
    }
    TransferRBXToLua(L, -1);
  } catch (std::exception e) {
    luaL_error(L, e.what());
  }
  return 1;
}

int __meta(lua_State * L) {
  int RState = RLua::lua_State; //lua_tonumber(L, lua_upvalueindex(2));
  int nargs = lua_gettop(L);
  int before = RLua::lua_gettop(RState);
  r_unwrap(L);
  //r_wrap(L);
  RLua::lua_getmetafield(RState, -1, "__type");
  rlua_pop(RState, 1);
  string field = lua_tostring(L, -1);
  RLua::lua_getmetafield(RState, -1, field.c_str());
  if (RLua::lua_type(RState, -1) != 6) {
    if (field == "__eq") { // it's a bit of a hack, but it works, so who's to judge?
      r_unwrap(L);
      lua_pushboolean(L, RLua::lua_touserdata(RState, -2) == RLua::lua_touserdata(RState, -1));
      rlua_pop(RState, 2);
      return 1;
    }
    //luaL_error(L, "attempt to perform an unsupported userdata operation");
  }
  RLua::lua_pushvalue(RState, -2);
  RLua::lua_remove(RState, -3);
  for (int i = 1; i <= nargs - 1; i++) {
    TransferRBXToLua(L, 1);
    r_unwrap(L);
    //unwrap(L, RState, 1);
  }
  if (RLua::lua_pcall(RState, nargs, -1, 0) != 0) {
    if (RLua::lua_type(RState, -1) != 4) {
      RLua::lua_pushstring(RState, "an error occurred during a userdata operation");
    }
    luaL_error(L, RLua::lua_tostring(RState, -1));
  }
  int nret = RLua::lua_gettop(RState) - before;
  for (int i = 1; i <= nret; i++) {
    r_wrap(L);
    //r_unwrap(L); //== ROBLOX->Env
    //r_wrap = ENV_RBX;
  }
  return lua_gettop(L);
}
/*

  _    _           _       _          _____ _       _           _
 | |  | |         | |     | |        / ____| |     | |         | |
 | |  | |_ __   __| | __ _| |_ ___  | |  __| | ___ | |__   __ _| |___
 | |  | | '_ \ / _` |/ _` | __/ _ \ | | |_ | |/ _ \| '_ \ / _` | / __|
 | |__| | |_) | (_| | (_| | ||  __/ | |__| | | (_) | |_) | (_| | \__ \
  \____/| .__/ \__,_|\__,_|\__\___|  \_____|_|\___/|_.__/ \__,_|_|___/
        | |
        |_|
Update the Proto's and the Global's.
Required by the way.
*/
#define R_LUA_TNONE  -1
#define R_LUA_TUSERDATA  7
#define R_LUA_TFUNCTION  9
#define R_LUA_TSTRING  5
#define R_LUA_TBOOLEAN  1
#define R_LUA_TNUMBER  2
#define R_LUA_TTABLE  8
#define R_LUA_TNIL  0
#define R_LUA_TTHREAD  6
#define R_LUA_TVECTOR  3
#define R_LUA_TLIGHTUSERDATA  4
#define R_LUA_TPROTO  10
#define R_LUA_TUPVALUE  11

static constexpr int proto_code = 12;
static constexpr int proto_sizecode = 68;
static constexpr int proto_p = 8;
static constexpr int proto_sizep = 72;
static constexpr int proto_k = 36;
static constexpr int proto_sizek = 56;
static constexpr int proto_lineinfo = 16;
static constexpr int proto_sizelineinfo = 60;
static constexpr int proto_locvars = 32;
static constexpr int proto_sizelocvars = 76;
static constexpr int proto_upvalues = 24;
static constexpr int proto_sizeupvalues = 64;
static constexpr int proto_is_vararg = 20;
static constexpr int proto_source = 44;
static constexpr int proto_nups = 7;
static constexpr int proto_numparams = 237;
static constexpr int proto_maxstacksize = 80;
#pragma once
#define x(x) (x - 0x400000 + (DWORD)GetModuleHandleA(0))

#define RL_TNIL 0
#define RL_TLIGHTUSERDATA 1
#define RL_TNUMBER 2
#define RL_TBOOLEAN 3
#define RL_TSTRING 4
#define RL_TTHREAD 5
#define RL_TFUNCTION 6
#define RL_TTABLE 7
#define RL_TUSERDATA 8
#define RL_TPROTO 9
#define RL_TUPVALUE 10
#define RLUA_TNIL 0
#define RLUA_TLIGHTUSERDATA	1
#define RLUA_TNUMBER 2
#define RLUA_TBOOLEAN 3
#define RLUA_TSTRING 4
#define RLUA_TTHREAD 5
#define RLUA_TFUNCTION 6
#define RLUA_TTABLE 7
#define RLUA_TUSERDATA 8
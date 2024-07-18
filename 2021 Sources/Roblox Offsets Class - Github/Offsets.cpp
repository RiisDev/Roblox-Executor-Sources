#include "Offsets.h"



RobloxOffsets* rbx_offsets = new RobloxOffsets();
RobloxLuaTypes* T = new RobloxLuaTypes();
RobloxTValue* tval = new RobloxTValue();
RobloxClosure* rbxcl = new RobloxClosure();
RobloxGlobalStateOffsets* RbxglobalState = new RobloxGlobalStateOffsets();
RobloxCallInfo* rbx_callinfo = new RobloxCallInfo();

namespace Needed
{
	/* roblox lua types */
	DWORD TNONE = -1;
	DWORD TUSERDATA = 6;
	DWORD TFUNCTION = 7;
	DWORD TSTRING = 5;
	DWORD TBOOLEAN = 1;
	DWORD TNUMBER = 3;
	DWORD TTABLE = 9;
	DWORD TNIL = 0;
	DWORD TTHREAD = 8;
	DWORD TVECTOR = 2;
	DWORD TLIGHTUSERDATA = 4;
	DWORD TPROTO = 10;
	DWORD TUPVALUE = 11;
	/* LuaState offsets */
	DWORD lua_state_env = 64;
	DWORD lua_state_l_gt = 88;
	DWORD lua_state_base = 12;
	DWORD lua_state_top = 16;
	DWORD lua_stack_last = 24;
	DWORD lua_state_ci = 28;
	DWORD lua_state_stack = 32;
	DWORD lua_state_baseci = 40;
	DWORD lua_state_sizeci = 44;
	DWORD lua_state_savedpc = 20;
	/* Callinfo offsets */
	DWORD callinfo_func_offset = 28;
	DWORD global_state_currentwhite = 20;
	DWORD global_state_gcstate = 21;
	DWORD global_state_gcstepmul = 76;
	DWORD global_state_gray = 44;
	DWORD global_state_gcthreshold = 64;
	DWORD global_state_totalbytes = 60;
	DWORD global_state_grayagain = 36;
	DWORD global_state_mainthread = 1376;
	DWORD global_state_rootgc = 48;
	DWORD global_state_sweepstrgc = 44;
	DWORD global_state_weak = 32;
	/* Closure offsets */
	DWORD closure_env = 9;
	DWORD closure_isc = 7;
	DWORD closure_nups = 8;
	/* Roblox offsets */
	DWORD getnamecallmethod_offset = 104;
	DWORD networked_dm = 1256;
	DWORD Identity1 = 24;
	DWORD Identity2 = 116;
	/* Tvalue shit*/
	DWORD lua_tvalue_tt = 12;
}




void RobloxLuaTypes::PrintTypes()
{ 
	// Gonna do something magical with this.

	// RobloxLuaTypes, will be created as a new Type, but i will make a function to set the types.
	printf("R_LUA_TBOOLEAN IS: %i\n", this->R_LUA_TBOOLEAN);
	printf("R_LUA_TFUNCTION IS: %i\n", this->R_LUA_TFUNCTION);
	printf("R_LUA_TLIGHTUSERDATA IS: %i\n", this->R_LUA_TLIGHTUSERDATA);
	printf("R_LUA_TNIL IS: %i\n", this->R_LUA_TNIL);
	printf("R_LUA_TNONE IS: %i\n", this->R_LUA_TNONE);
	printf("R_LUA_TNUMBER IS: %i\n", this->R_LUA_TNUMBER);
	printf("R_LUA_TPROTO IS: %i\n", this->R_LUA_TPROTO);
	printf("R_LUA_TSTRING IS: %i\n", this->R_LUA_TSTRING);
	printf("R_LUA_TTABLE IS: %i\n", this->R_LUA_TTABLE);
	printf("R_LUA_TTHREAD IS: %i\n", this->R_LUA_TTHREAD);
	printf("R_LUA_TUPVALUE IS: %i\n", this->R_LUA_TUPVALUE);
	printf("R_LUA_TUSERDATA IS: %i\n", this->R_LUA_TUSERDATA);
	printf("R_LUA_TVECTOR IS: %i\n", this->R_LUA_TVECTOR);
}




void RobloxLuaTypes::SetRobloxTypes() // if you are to use this call this first, then print the roblox types after they are set + stored into the struct
{
	this->R_LUA_TBOOLEAN = Needed::TBOOLEAN;				// Set all the lua Type's here.
	this->R_LUA_TFUNCTION = Needed::TFUNCTION;				// Set all the lua Type's here.
	this->R_LUA_TLIGHTUSERDATA = Needed::TLIGHTUSERDATA;		// Set all the lua Type's here.
	this->R_LUA_TNIL = Needed::TNIL;					// Set all the lua Type's here.
	this->R_LUA_TNONE = Needed::TNONE;				// Set all the lua Type's here.
	this->R_LUA_TNUMBER = Needed::TNUMBER;				// Set all the lua Type's here.
	this->R_LUA_TPROTO = Needed::TPROTO;				// Set all the lua Type's here.
	this->R_LUA_TSTRING = Needed::TSTRING;				// Set all the lua Type's here.
	this->R_LUA_TTABLE = Needed::TTABLE;				// Set all the lua Type's here.
	this->R_LUA_TTHREAD = Needed::TTHREAD;				// Set all the lua Type's here.
	this->R_LUA_TUPVALUE = Needed::TUPVALUE;				// Set all the lua Type's here.
	this->R_LUA_TUSERDATA = Needed::TUSERDATA;				// Set all the lua Type's here.
	this->R_LUA_TVECTOR = Needed::TVECTOR;               // Set all the lua Type's here.
}	





void RobloxCallInfo::SetRobloxCallInfoOffsets()
{
	this->callinfo_func = Needed::callinfo_func_offset;
}

void RobloxLuaState::SetLuaStateOffsets()
{
	this->lstack_last = Needed::lua_stack_last;
	this->lstate_base = Needed::lua_state_base;
	this->lstate_baseci = Needed::lua_state_baseci;
	this->lstate_ci = Needed::lua_state_ci;
	this->lstate_env = Needed::lua_state_env;
	this->lstate_l_gt = Needed::lua_state_l_gt;
	this->lstate_savedpc = Needed::lua_state_savedpc;
	this->lstate_sizeci = Needed::lua_state_sizeci;
	this->lstate_stack = Needed::lua_state_stack;
	this->lstate_top = Needed::lua_state_top;
}

void RobloxGlobalStateOffsets::SetRobloxGlobalStateOffsets()
{
	this->gstate_currentwhite = Needed::global_state_currentwhite;
	this->gstate_gcstate = Needed::global_state_gcstate;
	this->gstate_gcstepmul = Needed::global_state_gcstepmul;
	this->gstate_gcthreshold = Needed::global_state_gcthreshold;
	this->gstate_gray = Needed::global_state_gray;
	this->gstate_grayagain = Needed::global_state_grayagain;
	this->gstate_mainthread = Needed::global_state_mainthread;
	this->gstate_rootgc = Needed::global_state_rootgc;
	this->gstate_sweepstrgc = Needed::global_state_sweepstrgc;
	this->gstate_totalbytes = Needed::global_state_totalbytes;
	this->gstate_weak = Needed::global_state_weak;
}

void RobloxClosure::SetRobloxClosureOffsets()
{
	this->cl_env = Needed::closure_env;
	this->cl_isC = Needed::closure_isc;
	this->cl_nups = Needed::closure_nups;
}

void RobloxOffsets::SetRobloxOffsets()
{
	this->GetNameCallMethod = Needed::getnamecallmethod_offset;
	this->Identity1 = Needed::Identity1;
	this->Identity2 = Needed::Identity2;
	this->networkedDM = Needed::networked_dm;
}

void RobloxTValue::SetRobloxTValueOffsets()
{
	this->luaTTValue = Needed::lua_tvalue_tt;
}

// Auto Lua Type: https://github.com/Mellonyt/Roblox-Lua-Type-Scanner 
// Scanning Time: 1.000000 Secs 
// Dumping Date: Mon Apr 19 23:35:40 2021
// For version-b69117cbf3b945ef 
// Dumped With Mellonyt aka Mellon Addy Dumper My Discord: mellonyt#1234 
// You need to update this file, mellon addies can be found with idk.
#include <Windows.h> 
#include "retcheck.h"
#include<cstdint> 
namespace Adresses {
	static constexpr std::uintptr_t addinfo_addr = 0x01534640;
#define addinfo_CCV __cdecl

	static constexpr std::uintptr_t adjuststack_addr = 0x01528860;
#define adjuststack_CCV __cdecl

	static constexpr std::uintptr_t arith_addr = 0x01542E70;
#define arith_CCV __cdecl

	static constexpr std::uintptr_t appendarray_addr = 0x005D1960;
#define appendarray_CCV __thiscall

	static constexpr std::uintptr_t atomic_addr = 0x0153E4F0;
#define atomic_CCV __cdecl

	static constexpr std::uintptr_t aux_upvalue_addr = 0x015261F0;
#define aux_upvalue_CCV __cdecl

	static constexpr std::uintptr_t auxgetinfo_addr = 0x015346C0;
#define auxgetinfo_CCV __cdecl

	static constexpr std::uintptr_t auxresume_addr = 0x01536250;
#define auxresume_CCV __thiscall

	static constexpr std::uintptr_t call_bintm_addr = 0x015423A0;
#define call_bintm_CCV __cdecl

	static constexpr std::uintptr_t call_ordertm_addr = 0x01542450;
#define call_ordertm_CCV __cdecl

	static constexpr std::uintptr_t calltmres_addr = 0x01542250;
#define calltmres_CCV __cdecl

	static constexpr std::uintptr_t cleartable_addr = 0x0153E5B0;
#define cleartable_CCV __cdecl

	static constexpr std::uintptr_t close_state_addr = 0x01533F90;
#define close_state_CCV __cdecl

	static constexpr std::uintptr_t currentline_addr = 0x01534870;
#define currentline_CCV __cdecl

	static constexpr std::uintptr_t err_geterrorstring_addr = 0x0095FAC0;
#define err_geterrorstring_CCV __cdecl

	static constexpr std::uintptr_t f_call_addr = 0x01526280;
#define f_call_CCV __cdecl

	static constexpr std::uintptr_t f_luaopen_addr = 0x01534040;
#define f_luaopen_CCV __cdecl

	static constexpr std::uintptr_t findfirstchild_addr = 0x00686160;
#define findfirstchild_CCV __thiscall

	static constexpr std::uintptr_t findfirstchildofclass_addr = 0x00686330;
#define findfirstchildofclass_CCV __stdcall

	static constexpr std::uintptr_t findlocal_addr = 0x01540750;
#define findlocal_CCV __cdecl

	static constexpr std::uintptr_t fireclickdetector_addr = 0x00C71090;
#define fireclickdetector_CCV __thiscall

	static constexpr std::uintptr_t firetouchinterest_addr = 0x01BDAC6C;
#define firetouchinterest_CCV __cdecl

	static constexpr std::uintptr_t fireproximityprompt_addr = 0x00CBE690;
#define fireproximityprompt_CCV __cdecl

	static constexpr std::uintptr_t flog_getvalue_addr = 0x0177B480;
#define flog_getvalue_CCV __cdecl

	static constexpr std::uintptr_t flog_setvalue_addr = 0x0177CF70;
#define flog_setvalue_CCV __stdcall

	static constexpr std::uintptr_t getdatamodel_addr = 0x00FDC080;
#define getdatamodel_CCV __thiscall

	static constexpr std::uintptr_t getdatamodel2_addr = 0x00FDBF30;
#define getdatamodel2_CCV __cdecl

	static constexpr std::uintptr_t getfunc_addr = 0x015359D0;
#define getfunc_CCV __cdecl

	static constexpr std::uintptr_t getjobsbyname_addr = 0x01070AC0;
#define getjobsbyname_CCV __cdecl

	static constexpr std::uintptr_t getnamecallmethod_addr = 0x015288D0;
#define getnamecallmethod_CCV __cdecl

	static constexpr std::uintptr_t grabscriptcontext_addr = 0x005ECA30;
#define grabscriptcontext_CCV __thiscall

	static constexpr std::uintptr_t grabglobalstateindex_addr = 0x007AC5D0;
#define grabglobalstateindex_CCV __thiscall

	static constexpr std::uintptr_t growci_addr = 0x01529800;
#define growci_CCV __cdecl

	static constexpr std::uintptr_t hashnum_addr = 0x01540EE0;
#define hashnum_CCV __cdecl

	static constexpr std::uintptr_t index2adr_addr = 0x015262D0;
#define index2adr_CCV __stdcall

	static constexpr std::uintptr_t report_touch_info_addr = 0x0093BA30;
#define report_touch_info_CCV __stdcall

	static constexpr std::uintptr_t propagateall_addr = 0x0153F870;
#define propagateall_CCV __cdecl

	static constexpr std::uintptr_t propagatemark_addr = 0x0153F9C0;
#define propagatemark_CCV __cdecl

	static constexpr std::uintptr_t sweeplist_addr = 0x0153FF00;
#define sweeplist_CCV __cdecl

	static constexpr std::uintptr_t luaopen_base_addr = 0x01535DE0;
#define luaopen_base_CCV __cdecl

	static constexpr std::uintptr_t luaopen_table_addr = 0x01536530;
#define luaopen_table_CCV __cdecl

	static constexpr std::uintptr_t luaopen_bit32_addr = 0x0153C680;
#define luaopen_bit32_CCV __cdecl

	static constexpr std::uintptr_t lua_call_addr = 0x01526400;
#define lua_call_CCV __cdecl

	static constexpr std::uintptr_t lua_checkstack_addr = 0x015276F0;
#define lua_checkstack_CCV __cdecl

	static constexpr std::uintptr_t lua_close_addr = 0x015341D0;
#define lua_close_CCV __cdecl

	static constexpr std::uintptr_t lua_concat_addr = 0x01526510;
#define lua_concat_CCV __cdecl

	static constexpr std::uintptr_t lua_createtable_addr = 0x015265B0;
#define lua_createtable_CCV __cdecl

	static constexpr std::uintptr_t lua_error_addr = 0x01526700;
#define lua_error_CCV __thiscall

	static constexpr std::uintptr_t lua_freeobj_addr = 0x0153EF50;
#define lua_freeobj_CCV __cdecl

	static constexpr std::uintptr_t lua_gc_addr = 0x01526710;
#define lua_gc_CCV __cdecl

	static constexpr std::uintptr_t lua_getargument_addr = 0x01534D00;
#define lua_getargument_CCV __cdecl

	static constexpr std::uintptr_t lua_getfenv_addr = 0x01526830;
#define lua_getfenv_CCV __cdecl

	static constexpr std::uintptr_t lua_getfield_addr = 0x015268D0;
#define lua_getfield_CCV __fastcall

	static constexpr std::uintptr_t lua_getinfo_addr = 0x01534DD0;
#define lua_getinfo_CCV __cdecl

	static constexpr std::uintptr_t lua_getlocal_addr = 0x01534EB0;
#define lua_getlocal_CCV __cdecl

	static constexpr std::uintptr_t lua_getmetatable_addr = 0x01526970;
#define lua_getmetatable_CCV __fastcall

	static constexpr std::uintptr_t lua_gettable_addr = 0x01526A20;
#define lua_gettable_CCV __cdecl

	static constexpr std::uintptr_t lua_gettop_addr = 0x01526A90;
#define lua_gettop_CCV __cdecl

	static constexpr std::uintptr_t lua_getupvalue_addr = 0x01526AB0;
#define lua_getupvalue_CCV __cdecl

	static constexpr std::uintptr_t lua_insert_addr = 0x01526B40;
#define lua_insert_CCV __cdecl

	static constexpr std::uintptr_t lua_iscfunction_addr = 0x01526C10;
#define lua_iscfunction_CCV __cdecl

	static constexpr std::uintptr_t lua_isnumber_addr = 0x01526C60;
#define lua_isnumber_CCV __cdecl

	static constexpr std::uintptr_t lua_isstring_addr = 0x01526CC0;
#define lua_isstring_CCV __cdecl

	static constexpr std::uintptr_t lua_isuserdata_addr = 0x01526BC0;
#define lua_isuserdata_CCV __cdecl

	static constexpr std::uintptr_t lua_lessthan_addr = 0x01526D10;
#define lua_lessthan_CCV __cdecl

	static constexpr std::uintptr_t lua_newstate_addr = 0x01534200;
#define lua_newstate_CCV __cdecl

	static constexpr std::uintptr_t lua_newthread_addr = 0x01526DC0;
#define lua_newthread_CCV __cdecl

	static constexpr std::uintptr_t lua_newuserdata_addr = 0x01526E50;
#define lua_newuserdata_CCV __cdecl

	static constexpr std::uintptr_t lua_next_addr = 0x01526ED0;
#define lua_next_CCV __cdecl

	static constexpr std::uintptr_t lua_objlen_addr = 0x01526F60;
#define lua_objlen_CCV __cdecl

	static constexpr std::uintptr_t lua_pcall_addr = 0x01527030;
#define lua_pcall_CCV __cdecl

	static constexpr std::uintptr_t lua_pushboolean_addr = 0x015270F0;
#define lua_pushboolean_CCV __cdecl

	static constexpr std::uintptr_t lua_pushcclosure_addr = 0x01527140;
#define lua_pushcclosure_CCV __fastcall

	static constexpr std::uintptr_t lua_pushfstring_addr = 0x01527210;
#define lua_pushfstring_CCV __cdecl

	static constexpr std::uintptr_t lua_pushinteger_addr = 0x01527280;
#define lua_pushinteger_CCV __cdecl

	static constexpr std::uintptr_t lua_pushlightuserdata_addr = 0x015272E0;
#define lua_pushlightuserdata_CCV __cdecl

	static constexpr std::uintptr_t lua_pushlstring_addr = 0x01527330;
#define lua_pushlstring_CCV __cdecl

	static constexpr std::uintptr_t lua_pushnil_addr = 0x015273A0;
#define lua_pushnil_CCV __cdecl

	static constexpr std::uintptr_t lua_pushnumber_addr = 0x015273F0;
#define lua_pushnumber_CCV __thiscall

	static constexpr std::uintptr_t lua_pushstring_addr = 0x01527450;
#define lua_pushstring_CCV __fastcall

	static constexpr std::uintptr_t lua_pushthread_addr = 0x015274E0;
#define lua_pushthread_CCV __cdecl

	static constexpr std::uintptr_t lua_pushvalue_addr = 0x015275B0;
#define lua_pushvalue_CCV __cdecl

	static constexpr std::uintptr_t lua_pushvfstring_addr = 0x01527680;
#define lua_pushvfstring_CCV __cdecl

	static constexpr std::uintptr_t lua_rawequal_addr = 0x01527760;
#define lua_rawequal_CCV __cdecl

	static constexpr std::uintptr_t lua_rawget_addr = 0x015277E0;
#define lua_rawget_CCV __cdecl

	static constexpr std::uintptr_t lua_rawgeti_addr = 0x01527900;
#define lua_rawgeti_CCV __cdecl

	static constexpr std::uintptr_t lua_rawset_addr = 0x01527980;
#define lua_rawset_CCV __cdecl

	static constexpr std::uintptr_t lua_rawseti_addr = 0x01527A50;
#define lua_rawseti_CCV __cdecl

	static constexpr std::uintptr_t lua_rawvalue_addr = 0x015263B0;
#define lua_rawvalue_CCV __cdecl

	static constexpr std::uintptr_t lua_remove_addr = 0x01527B20;
#define lua_remove_CCV __cdecl

	static constexpr std::uintptr_t lua_replace_addr = 0x01527BB0;
#define lua_replace_CCV __cdecl

	static constexpr std::uintptr_t lua_resume_addr = 0x01529D60;
#define lua_resume_CCV __cdecl

	static constexpr std::uintptr_t lua_setfenv_addr = 0x01527CB0;
#define lua_setfenv_CCV __cdecl

	static constexpr std::uintptr_t lua_setfield_addr = 0x01527D70;
#define lua_setfield_CCV __fastcall

	static constexpr std::uintptr_t lua_setlocal_addr = 0x01534F80;
#define lua_setlocal_CCV __cdecl

	static constexpr std::uintptr_t lua_setmetatable_addr = 0x01527E20;
#define lua_setmetatable_CCV __cdecl

	static constexpr std::uintptr_t lua_setreadonly_addr = 0x01527F30;
#define lua_setreadonly_CCV __cdecl

	static constexpr std::uintptr_t lua_setsafeenv_addr = 0x01527FA0;
#define lua_setsafeenv_CCV __cdecl

	static constexpr std::uintptr_t lua_settable_addr = 0x01528010;
#define lua_settable_CCV __cdecl

	static constexpr std::uintptr_t lua_settop_addr = 0x01528090;
#define lua_settop_CCV __stdcall

	static constexpr std::uintptr_t lua_setupvalue_addr = 0x01528110;
#define lua_setupvalue_CCV __cdecl

	static constexpr std::uintptr_t lua_toboolean_addr = 0x015281C0;
#define lua_toboolean_CCV __cdecl

	static constexpr std::uintptr_t lua_tointeger_addr = 0x01528210;
#define lua_tointeger_CCV __cdecl

	static constexpr std::uintptr_t lua_tolstring_addr = 0x01528290;
#define lua_tolstring_CCV __cdecl

	static constexpr std::uintptr_t lua_tonumber_addr = 0x015283B0;
#define lua_tonumber_CCV __cdecl

	static constexpr std::uintptr_t lua_topointer_addr = 0x01528430;
#define lua_topointer_CCV __cdecl

	static constexpr std::uintptr_t lua_tostring_addr = 0x015284F0;
#define lua_tostring_CCV __cdecl

	static constexpr std::uintptr_t lua_tothread_addr = 0x01528570;
#define lua_tothread_CCV __cdecl

	static constexpr std::uintptr_t lua_tounsignedx_addr = 0x015285B0;
#define lua_tounsignedx_CCV __cdecl

	static constexpr std::uintptr_t lua_touserdata_addr = 0x01528640;
#define lua_touserdata_CCV __cdecl

	static constexpr std::uintptr_t lua_type_addr = 0x01528730;
#define lua_type_CCV __cdecl

	static constexpr std::uintptr_t lua_typename_addr = 0x01528770;
#define lua_typename_CCV __cdecl

	static constexpr std::uintptr_t lua_xmove_addr = 0x015287E0;
#define lua_xmove_CCV __cdecl

	static constexpr std::uintptr_t lua_yeild_addr = 0x01529FC0;
#define lua_yeild_CCV __cdecl

	static constexpr std::uintptr_t luaa_pushobject_addr = 0x01526390;
#define luaa_pushobject_CCV __cdecl

	static constexpr std::uintptr_t luab_auxwrap_addr = 0x01536330;
#define luab_auxwrap_CCV __cdecl

	static constexpr std::uintptr_t luac_barrierback_addr = 0x0153EFD0;
#define luac_barrierback_CCV __cdecl

	static constexpr std::uintptr_t luac_barrierf_addr = 0x0153EFF0;
#define luac_barrierf_CCV __cdecl

	static constexpr std::uintptr_t luac_fullgc_addr = 0x0153F2D0;
#define luac_fullgc_CCV __cdecl

	static constexpr std::uintptr_t luac_link_addr = 0x0153F3C0;
#define luac_link_CCV __cdecl

	static constexpr std::uintptr_t luac_linkupval_addr = 0x0153F400;
#define luac_linkupval_CCV __cdecl

	static constexpr std::uintptr_t luac_step_addr = 0x0153F480;
#define luac_step_CCV __cdecl

	static constexpr std::uintptr_t luad_call_addr = 0x01529760;
#define luad_call_CCV __cdecl

	static constexpr std::uintptr_t luad_growstack_addr = 0x01529860;
#define luad_growstack_CCV __cdecl

	static constexpr std::uintptr_t luad_pcall_addr = 0x01529890;
#define luad_pcall_CCV __cdecl

	static constexpr std::uintptr_t luad_poscall_addr = 0x01533610;
#define luad_poscall_CCV __cdecl

	static constexpr std::uintptr_t luad_precall_addr = 0x01533690;
#define luad_precall_CCV __cdecl

	static constexpr std::uintptr_t luad_rawrunprotected_addr = 0x01529970;
#define luad_rawrunprotected_CCV __cdecl

	static constexpr std::uintptr_t luad_reallocci_addr = 0x01529AB0;
#define luad_reallocci_CCV __cdecl

	static constexpr std::uintptr_t luad_reallocstack_addr = 0x01529B30;
#define luad_reallocstack_CCV __cdecl

	static constexpr std::uintptr_t luad_seterrorObj_addr = 0x01529C40;
#define luad_seterrorObj_CCV __cdecl

	static constexpr std::uintptr_t luad_throw_addr = 0x01529CB0;
#define luad_throw_CCV __fastcall

	static constexpr std::uintptr_t luae_freethread_addr = 0x015340C0;
#define luae_freethread_CCV __cdecl

	static constexpr std::uintptr_t luae_newthread_addr = 0x01534140;
#define luae_newthread_CCV __cdecl

	static constexpr std::uintptr_t luaf_close_addr = 0x01540490;
#define luaf_close_CCV __cdecl

	static constexpr std::uintptr_t luaf_findupval_addr = 0x01540530;
#define luaf_findupval_CCV __cdecl

	static constexpr std::uintptr_t luaf_freeproto_addr = 0x01540620;
#define luaf_freeproto_CCV __cdecl

	static constexpr std::uintptr_t luaf_newcclosure_addr = 0x015407B0;
#define luaf_newcclosure_CCV __cdecl

	static constexpr std::uintptr_t luaf_newlclosure_addr = 0x01540810;
#define luaf_newlclosure_CCV __cdecl

	static constexpr std::uintptr_t luaf_newproto_addr = 0x01540880;
#define luaf_newproto_CCV __cdecl

	static constexpr std::uintptr_t luag_aritherror_addr = 0x01534900;
#define luag_aritherror_CCV __cdecl

	static constexpr std::uintptr_t luag_breakpoint_addr = 0x01534970;
#define luag_breakpoint_CCV __cdecl

	static constexpr std::uintptr_t luag_concaterror_addr = 0x01534AC0;
#define luag_concaterror_CCV __cdecl

	static constexpr std::uintptr_t luag_forerrorl_addr = 0x01534B00;
#define luag_forerrorl_CCV __cdecl

	static constexpr std::uintptr_t luag_getline_addr = 0x01534B30;
#define luag_getline_CCV __cdecl

	static constexpr std::uintptr_t luag_indexerror_addr = 0x01534B70;
#define luag_indexerror_CCV __cdecl

	static constexpr std::uintptr_t luag_ordererror_addr = 0x01534C10;
#define luag_ordererror_CCV __cdecl

	static constexpr std::uintptr_t luag_runerror_addr = 0x01534CA0;
#define luag_runerror_CCV __cdecl

	static constexpr std::uintptr_t luag_typeerror_addr = 0x015434A0;
#define luag_typeerror_CCV __cdecl

	static constexpr std::uintptr_t luah_clone_addr = 0x01540FD0;
#define luah_clone_CCV __cdecl

	static constexpr std::uintptr_t luah_free_addr = 0x01540B30;
#define luah_free_CCV __cdecl

	static constexpr std::uintptr_t luah_get_addr = 0x01541170;
#define luah_get_CCV __cdecl

	static constexpr std::uintptr_t luah_getn_addr = 0x01541220;
#define luah_getn_CCV __cdecl

	static constexpr std::uintptr_t luah_getnum_addr = 0x015412A0;
#define luah_getnum_CCV __cdecl

	static constexpr std::uintptr_t luah_getstr_addr = 0x01541380;
#define luah_getstr_CCV __cdecl

	static constexpr std::uintptr_t luah_new_addr = 0x015413F0;
#define luah_new_CCV __cdecl

	static constexpr std::uintptr_t luah_next_addr = 0x01541460;
#define luah_next_CCV __cdecl

	static constexpr std::uintptr_t luah_resizearray_addr = 0x01541640;
#define luah_resizearray_CCV __cdecl

	static constexpr std::uintptr_t luah_set_addr = 0x01541690;
#define luah_set_CCV __cdecl

	static constexpr std::uintptr_t luah_setnum_addr = 0x015417B0;
#define luah_setnum_CCV __cdecl

	static constexpr std::uintptr_t luah_setstr_addr = 0x01541820;
#define luah_setstr_CCV __cdecl

	static constexpr std::uintptr_t lual_addlstring_addr = 0x01528990;
#define lual_addlstring_CCV __cdecl

	static constexpr std::uintptr_t lual_addvalue_addr = 0x01528A70;
#define lual_addvalue_CCV __cdecl

	static constexpr std::uintptr_t lual_argerror_addr = 0x01528AF0;
#define lual_argerror_CCV __cdecl

	static constexpr std::uintptr_t lual_buffinit_addr = 0x01528B70;
#define lual_buffinit_CCV __cdecl

	static constexpr std::uintptr_t lual_checkany_addr = 0x01528C70;
#define lual_checkany_CCV __cdecl

	static constexpr std::uintptr_t lual_checkinteger_addr = 0x01528CB0;
#define lual_checkinteger_CCV __cdecl

	static constexpr std::uintptr_t lual_checklstring_addr = 0x01528CE0;
#define lual_checklstring_CCV __cdecl

	static constexpr std::uintptr_t lual_checknumber_addr = 0x01528D10;
#define lual_checknumber_CCV __cdecl

	static constexpr std::uintptr_t lual_checkstack_addr = 0x01528D40;
#define lual_checkstack_CCV __thiscall

	static constexpr std::uintptr_t lual_checktype_addr = 0x01528D80;
#define lual_checktype_CCV __cdecl

	static constexpr std::uintptr_t lual_checkunsigned_addr = 0x01528DB0;
#define lual_checkunsigned_CCV __cdecl

	static constexpr std::uintptr_t lual_error_addr = 0x01528DE0;
#define lual_error_CCV __cdecl

	static constexpr std::uintptr_t lual_findtable_addr = 0x01528EE0;
#define lual_findtable_CCV __cdecl

	static constexpr std::uintptr_t lual_getmetafield_addr = 0x01528FC0;
#define lual_getmetafield_CCV __cdecl

	static constexpr std::uintptr_t lual_newmetatable_addr = 0x01529020;
#define lual_newmetatable_CCV __cdecl

	static constexpr std::uintptr_t lual_optinteger_addr = 0x01529080;
#define lual_optinteger_CCV __cdecl

	static constexpr std::uintptr_t lual_optlstring_addr = 0x015290D0;
#define lual_optlstring_CCV __cdecl

	static constexpr std::uintptr_t lual_prepbuffer_addr = 0x015291B0;
#define lual_prepbuffer_CCV __cdecl

	static constexpr std::uintptr_t lual_pushresult_addr = 0x01529280;
#define lual_pushresult_CCV __cdecl

	static constexpr std::uintptr_t lual_ref_addr = 0x01529360;
#define lual_ref_CCV __cdecl

	static constexpr std::uintptr_t lual_register_addr = 0x01529410;
#define lual_register_CCV __cdecl

	static constexpr std::uintptr_t lual_typeerror_addr = 0x01529530;
#define lual_typeerror_CCV __cdecl

	static constexpr std::uintptr_t lual_unref_addr = 0x015295D0;
#define lual_unref_CCV __cdecl

	static constexpr std::uintptr_t lual_where_addr = 0x01529630;
#define lual_where_CCV __cdecl

	static constexpr std::uintptr_t luam_realloc_addr = 0x01543AA3;
#define luam_realloc_CCV __cdecl

	static constexpr std::uintptr_t luam_realloc__addr = 0x01543840;
#define luam_realloc__CCV __cdecl

	static constexpr std::uintptr_t luam_toobig_addr = 0x01543AB0;
#define luam_toobig_CCV __thiscall

	static constexpr std::uintptr_t luao_chunkid_addr = 0x01533B20;
#define luao_chunkid_CCV __cdecl

	static constexpr std::uintptr_t luao_pushfstring_addr = 0x01533CA0;
#define luao_pushfstring_CCV __cdecl

	static constexpr std::uintptr_t luao_pushvfstring_addr = 0x01533D40;
#define luao_pushvfstring_CCV __cdecl

	static constexpr std::uintptr_t luao_rawequalKey_addr = 0x01533DE0;
#define luao_rawequalKey_CCV __cdecl

	static constexpr std::uintptr_t luao_rawequalobj_addr = 0x01533E60;
#define luao_rawequalobj_CCV __cdecl

	static constexpr std::uintptr_t luao_str2d_addr = 0x01533EE0;
#define luao_str2d_CCV __cdecl

	static constexpr std::uintptr_t luas_newlstr_addr = 0x01540BB0;
#define luas_newlstr_CCV __fastcall

	static constexpr std::uintptr_t luas_newudata_addr = 0x01540D90;
#define luas_newudata_CCV __cdecl

	static constexpr std::uintptr_t luas_resize_addr = 0x01540DE0;
#define luas_resize_CCV __cdecl

	static constexpr std::uintptr_t luat_gettm_addr = 0x01543470;
#define luat_gettm_CCV __cdecl

	static constexpr std::uintptr_t luat_objtypename_addr = 0x015435C0;
#define luat_objtypename_CCV __cdecl

	static constexpr std::uintptr_t luau_backtrace_addr = 0x01532420;
#define luau_backtrace_CCV __cdecl

	static constexpr std::uintptr_t luau_betavm_addr = 0x0152E390;
#define luau_betavm_CCV __cdecl

	static constexpr std::uintptr_t luau_callhook_addr = 0x015329C0;
#define luau_callhook_CCV __cdecl

	static constexpr std::uintptr_t luau_calltm_addr = 0x015328A0;
#define luau_calltm_CCV __cdecl

	static constexpr std::uintptr_t luau_deserialize_addr = 0x01532B40;
#define luau_deserialize_CCV __cdecl

	static constexpr std::uintptr_t luau_execute_addr = 0x0155FE20;
#define luau_execute_CCV __cdecl

	static constexpr std::uintptr_t luau_loopforg_addr = 0x015335A0;
#define luau_loopforg_CCV __cdecl

	static constexpr std::uintptr_t luau_prepareforn_addr = 0x01533810;
#define luau_prepareforn_CCV __cdecl

	static constexpr std::uintptr_t luau_resolveimport_addr = 0x015338B0;
#define luau_resolveimport_CCV __cdecl

	static constexpr std::uintptr_t luau_tryfunctm_addr = 0x01533A50;
#define luau_tryfunctm_CCV __cdecl

	static constexpr std::uintptr_t luav_concat_addr = 0x015424D0;
#define luav_concat_CCV __cdecl

	static constexpr std::uintptr_t luav_doarith_addr = 0x015428F0;
#define luav_doarith_CCV __thiscall

	static constexpr std::uintptr_t luav_dolen_addr = 0x015470E0;
#define luav_dolen_CCV __cdecl

	static constexpr std::uintptr_t luav_execute_addr = 0x0152A330;
#define luav_execute_CCV __cdecl

	static constexpr std::uintptr_t luav_gettable_addr = 0x01542FC0;
#define luav_gettable_CCV __cdecl

	static constexpr std::uintptr_t luav_gettable_opt_addr = 0x01542FA0;
#define luav_gettable_opt_CCV __cdecl

	static constexpr std::uintptr_t luav_lessequal_addr = 0x015430D0;
#define luav_lessequal_CCV __cdecl

	static constexpr std::uintptr_t luav_lessthan_addr = 0x01543170;
#define luav_lessthan_CCV __cdecl

	static constexpr std::uintptr_t luav_settable_addr = 0x01543210;
#define luav_settable_CCV __cdecl

	static constexpr std::uintptr_t luav_tonumber_addr = 0x015433A0;
#define luav_tonumber_CCV __cdecl

	static constexpr std::uintptr_t luav_tostring_addr = 0x01543400;
#define luav_tostring_CCV __cdecl

	static constexpr std::uintptr_t luavm_compile_addr = 0x006E5F50;
#define luavm_compile_CCV __cdecl

	static constexpr std::uintptr_t luavm_load_addr = 0x006E6490;
#define luavm_load_CCV __cdecl

	static constexpr std::uintptr_t luax_init_addr = 0x01543510;
#define luax_init_CCV __cdecl

	static constexpr std::uintptr_t luaz_openspace_addr = 0x01547150;
#define luaz_openspace_CCV __cdecl

	static constexpr std::uintptr_t mainposition_addr = 0x01541860;
#define mainposition_CCV __cdecl

	static constexpr std::uintptr_t markmt_addr = 0x0153F700;
#define markmt_CCV __cdecl

	static constexpr std::uintptr_t markroot_addr = 0x0153F7E0;
#define markroot_CCV __cdecl

	static constexpr std::uintptr_t match_addr = 0x0153A180;
#define match_CCV __cdecl

	static constexpr std::uintptr_t newkey_addr = 0x015419A0;
#define newkey_CCV __cdecl

	static constexpr std::uintptr_t print_addr = 0x00674960;
#define print_CCV __cdecl

	static constexpr std::uintptr_t push_captures_addr = 0x0153ABD0;
#define push_captures_CCV __cdecl

	static constexpr std::uintptr_t push_instance_addr = 0x0087BC50;
#define push_instance_CCV __cdecl

	static constexpr std::uintptr_t rbx_get_property_addr = 0x00879BF0;
#define rbx_get_property_CCV __thiscall

	static constexpr std::uintptr_t rbx_network_replicator_onreceive_addr = 0x0138D2B0;
#define rbx_network_replicator_onreceive_CCV __fastcall

	static constexpr std::uintptr_t rbx_network_physicssenders2_sendpacket_addr = 0x0144CDE0;
#define rbx_network_physicssenders2_sendpacket_CCV __thiscall

	static constexpr std::uintptr_t rbx_disable_report_screenshot_gl2_addr = 0x01181370;
#define rbx_disable_report_screenshot_gl2_CCV __cdecl

	static constexpr std::uintptr_t rbx_disable_report_screenshot_gl_addr = 0x01181350;
#define rbx_disable_report_screenshot_gl_CCV __cdecl

	static constexpr std::uintptr_t rbx_disable_report_screenshot_metal_addr = 0x01181390;
#define rbx_disable_report_screenshot_metal_CCV __cdecl

	static constexpr std::uintptr_t rbx_disable_report_screenshot_vulkan_addr = 0x011813B0;
#define rbx_disable_report_screenshot_vulkan_CCV __cdecl

	static constexpr std::uintptr_t rbx_disable_report_screenshot_d3d_addr = 0x01181330;
#define rbx_disable_report_screenshot_d3d_CCV __cdecl

	static constexpr std::uintptr_t rbx_setsimulationradius_addr = 0x0144DA30;
#define rbx_setsimulationradius_CCV __thiscall

	static constexpr std::uintptr_t rbx_network_clientreplicator_processtag_addr = 0x013E2010;
#define rbx_network_clientreplicator_processtag_CCV __stdcall

	static constexpr std::uintptr_t rbx_corescript_getluavmcorescriptfilepath_addr = 0x00AFF170;
#define rbx_corescript_getluavmcorescriptfilepath_CCV __cdecl

	static constexpr std::uintptr_t rbx_datastore_runtransformfunction_addr = 0x00CFF040;
#define rbx_datastore_runtransformfunction_CCV __stdcall

	static constexpr std::uintptr_t rbx_datamodel_dodatamodelsetup_addr = 0x00708C30;
#define rbx_datamodel_dodatamodelsetup_CCV __cdecl

	static constexpr std::uintptr_t rbx_debugvalueconverte_userdatatostring_addr = 0x00C3A8B0;
#define rbx_debugvalueconverte_userdatatostring_CCV __cdecl

	static constexpr std::uintptr_t rbx_deu_uploadcrasheventfile_addr = 0x006344F0;
#define rbx_deu_uploadcrasheventfile_CCV __cdecl

	static constexpr std::uintptr_t rbx_lm_reporterror_addr = 0x00638DD0;
#define rbx_lm_reporterror_CCV __cdecl

	static constexpr std::uintptr_t rbx_lua_dumpthreadrefcounts_addr = 0x0084D4B0;
#define rbx_lua_dumpthreadrefcounts_CCV __cdecl

	static constexpr std::uintptr_t rbx_network_replicator_processdeserializedpacket_addr = 0x01391200;
#define rbx_network_replicator_processdeserializedpacket_CCV __thiscall

	static constexpr std::uintptr_t rbx_network_server_registerlegalscript_addr = 0x013BBEF0;
#define rbx_network_server_registerlegalscript_CCV __thiscall

	static constexpr std::uintptr_t rbx_rcr_processexception_addr = 0x00638C70;
#define rbx_rcr_processexception_CCV __cdecl

	static constexpr std::uintptr_t raknet_rakpeer_runupdatecycle_addr = 0x013C2D90;
#define raknet_rakpeer_runupdatecycle_CCV __thiscall

	static constexpr std::uintptr_t rbx_sc_closestate_addr = 0x007A7830;
#define rbx_sc_closestate_CCV __stdcall

	static constexpr std::uintptr_t rbx_taskscheduler_getlastcycletimestamp_addr = 0x0104BCF0;
#define rbx_taskscheduler_getlastcycletimestamp_CCV __cdecl

	static constexpr std::uintptr_t rbx_lua_bridge_on_index_addr = 0x0087B750;
#define rbx_lua_bridge_on_index_CCV __cdecl

	static constexpr std::uintptr_t rbx_security_context_current_addr = 0x01524A80;
#define rbx_security_context_current_CCV __thiscall

	static constexpr std::uintptr_t rbx_security_impersonator_impersonator_addr = 0x015249B0;
#define rbx_security_impersonator_impersonator_CCV __stdcall

	static constexpr std::uintptr_t rbx_datamodel_getjobsinfo_addr = 0x0070C6A0;
#define rbx_datamodel_getjobsinfo_CCV __thiscall

	static constexpr std::uintptr_t rbx_sc_crash_addr = 0x007A8400;
#define rbx_sc_crash_CCV __cdecl

	static constexpr std::uintptr_t rbx_sc_interrupt_addr = 0x007AE5B0;
#define rbx_sc_interrupt_CCV __cdecl

	static constexpr std::uintptr_t rbx_sc_loadstring_addr = 0x007AEF60;
#define rbx_sc_loadstring_CCV __cdecl

	static constexpr std::uintptr_t rbx_sc_scriptcontext_addr = 0x007A1450;
#define rbx_sc_scriptcontext_CCV __thiscall

	static constexpr std::uintptr_t rbx_sc_startscript_addr = 0x007BC010;
#define rbx_sc_startscript_CCV __stdcall

	static constexpr std::uintptr_t rbx_sc_watchdog_addr = 0x007BDD10;
#define rbx_sc_watchdog_CCV __thiscall

	static constexpr std::uintptr_t rbx_scriptcontext_getcontext_addr = 0x007C7980;
#define rbx_scriptcontext_getcontext_CCV __cdecl

	static constexpr std::uintptr_t rbx_scriptcontext_debuggermanager_addr = 0x007A8BF0;
#define rbx_scriptcontext_debuggermanager_CCV __cdecl

	static constexpr std::uintptr_t rbx_scriptcontext_pluginmanager_addr = 0x007B2040;
#define rbx_scriptcontext_pluginmanager_CCV __cdecl

	static constexpr std::uintptr_t rbx_scriptcontext_usersettings_addr = 0x007BD8A0;
#define rbx_scriptcontext_usersettings_CCV __cdecl

	static constexpr std::uintptr_t rbx_scriptcontext_settings_addr = 0x007BB320;
#define rbx_scriptcontext_settings_CCV __cdecl

	static constexpr std::uintptr_t rbx_scriptcontext_getglobalstate_addr = 0x007AC610;
#define rbx_scriptcontext_getglobalstate_CCV __cdecl

	static constexpr std::uintptr_t rbx_modulescript_vmstate_addr = 0x00C4ECF0;
#define rbx_modulescript_vmstate_CCV __stdcall

	static constexpr std::uintptr_t rbx_modulescript_pervmstate_setcompletedsuccess_addr = 0x00C4E9D0;
#define rbx_modulescript_pervmstate_setcompletedsuccess_CCV __thiscall

	static constexpr std::uintptr_t rbx_modulescript_pervmstate_getandclearyieldedimporters_addr = 0x00C4E740;
#define rbx_modulescript_pervmstate_getandclearyieldedimporters_CCV __thiscall

	static constexpr std::uintptr_t rbx_scriptcontext_requiremodulescriptsuccesscontinuation_addr = 0x007B8D30;
#define rbx_scriptcontext_requiremodulescriptsuccesscontinuation_CCV __cdecl

	static constexpr std::uintptr_t rbx_setthreadname_addr = 0x0104B6E0;
#define rbx_setthreadname_CCV __cdecl

	static constexpr std::uintptr_t rbx_serializer_loadinstances_addr = 0x0088C650;
#define rbx_serializer_loadinstances_CCV __cdecl

	static constexpr std::uintptr_t rbx_scriptcontext_printcallstack_addr = 0x007B22A0;
#define rbx_scriptcontext_printcallstack_CCV __cdecl

	static constexpr std::uintptr_t rbx_serializer_loadinstances_long_addr = 0x0088C670;
#define rbx_serializer_loadinstances_long_CCV __cdecl

	static constexpr std::uintptr_t rbx_standardout_printwithlink_addr = 0x00674660;
#define rbx_standardout_printwithlink_CCV __cdecl

	static constexpr std::uintptr_t rbx_ts_startscript_addr = 0x00B49C50;
#define rbx_ts_startscript_CCV __thiscall

	static constexpr std::uintptr_t rbx_ws_getrealphysicsfps_addr = 0x0094B5C0;
#define rbx_ws_getrealphysicsfps_CCV __thiscall

	static constexpr std::uintptr_t rbxcrash_addr = 0x01786FC0;
#define rbxcrash_CCV __cdecl

	static constexpr std::uintptr_t reallymarkobject_addr = 0x0153FB60;
#define reallymarkobject_CCV __cdecl

	static constexpr std::uintptr_t rehash_addr = 0x01541B50;
#define rehash_CCV __cdecl

	static constexpr std::uintptr_t reportError_addr = 0x007B6150;
#define reportError_CCV __thiscall

	static constexpr std::uintptr_t resize_addr = 0x01541DC0;
#define resize_CCV __cdecl

	static constexpr std::uintptr_t resume_addr = 0x0152A030;
#define resume_CCV __cdecl

	static constexpr std::uintptr_t resume_error_addr = 0x0152A110;
#define resume_error_CCV __cdecl

	static constexpr std::uintptr_t retcheck_addr = 0x006E6890;
#define retcheck_CCV __fastcall

	static constexpr std::uintptr_t robloxextraspace__robloxextraspace_addr = 0x007A5290;
#define robloxextraspace__robloxextraspace_CCV __thiscall

	static constexpr std::uintptr_t sandboxthread_addr = 0x007AEC20;
#define sandboxthread_CCV __cdecl

	static constexpr std::uintptr_t setarrayvector_addr = 0x01541FA0;
#define setarrayvector_CCV __cdecl

	static constexpr std::uintptr_t setnodevector_addr = 0x01542020;
#define setnodevector_CCV __cdecl

	static constexpr std::uintptr_t singlestep_addr = 0x0153FD00;
#define singlestep_CCV __cdecl

	static constexpr std::uintptr_t spawn_addr = 0x007BB4F0;
#define spawn_CCV __cdecl

	static constexpr std::uintptr_t stack_init_addr = 0x01534590;
#define stack_init_CCV __cdecl

	static constexpr std::uintptr_t str_find_aux_addr = 0x0153AF10;
#define str_find_aux_CCV __cdecl

	static constexpr std::uintptr_t tag_error_addr = 0x015296A0;
#define tag_error_CCV __cdecl

	static constexpr std::uintptr_t trustCheck_addr = 0x016FCE80;
#define trustCheck_CCV __cdecl

	static constexpr std::uintptr_t taskscheduler_addr = 0x0104BF10;
#define taskscheduler_CCV __cdecl

	static constexpr std::uintptr_t unbound_search_addr = 0x015420F0;
#define unbound_search_CCV __cdecl

	static constexpr std::uintptr_t vm_invoker_addr = 0x01532B20;
#define vm_invoker_CCV __thiscall

	static constexpr std::uintptr_t cum_check_bypass = 0x016FBDF7;

	static constexpr std::uintptr_t callcheck = 0x005FD6F0;

	static constexpr std::uintptr_t retcheckflag1 = 0x02A7D14C;

	static constexpr std::uintptr_t retcheckflag2 = 0x02A7D23C;

	static constexpr std::uintptr_t xorconst = 0x02A02890;

	static constexpr std::uintptr_t luao_nilobject = 0x022650E8;

	static constexpr std::uintptr_t luat_typenames = 0x02266AA0;

	static constexpr std::uintptr_t kname = 0x027B4A38;

	static constexpr std::uintptr_t op_index = 0x0152E0E0;

	static constexpr std::uintptr_t op_case_table = 0x0152E20C;

	static constexpr std::uintptr_t retcheck_jmp = 0x006E68B0;

	static constexpr std::uintptr_t trustcheck_bypass1 = 0x02680EC0;

	static constexpr std::uintptr_t scriptinstance = 0x0284F9D0;

	static constexpr std::uintptr_t scriptcontext = 0x0206CA34;

	// Roblox Offsets 
	static constexpr int getnamecallmethod_offset = 100;
	static constexpr int networked_dm = 1184;
	static constexpr int Identity1 = 24;
	static constexpr int Identity2 = 112;

	// Luastate Offsets 
	static constexpr int ls_readonly = 9;
	static constexpr int ls_base_ci = 0;
	static constexpr int ls_end_ci = 0;
	static constexpr int ls_ci = 16;
	static constexpr int ls_saved_pc = 16;
	static constexpr int ls_top = 20;
	static constexpr int ls_base = 24;
	static constexpr int ls_env = 64;
	static constexpr int ls_l_gt = 80;

	// Globalstate Offsets 
	static constexpr int ls_gs_totalbytes = 76;
	static constexpr int ls_gs_gcthreshold = 80;
	static constexpr int ls_gs_gcpause = 72;
	static constexpr int ls_gs_gcstepmul = 84;
	static constexpr int ls_gs_rootgc = 36;
	static constexpr int ls_gs_mainthread = 236;

	// Closure Offsets 
	static constexpr int ls_closure_nupvalues = 5;
	static constexpr int ls_closure_env = 16;
	static constexpr int ls_closure_isc = 0;

	// Types 
#define R_LUA_TNONE  -1
#define R_LUA_TUSERDATA  7
#define R_LUA_TFUNCTION  9
#define R_LUA_TSTRING  5
#define R_LUA_TBOOLEAN  1
#define R_LUA_TNUMBER  4
#define R_LUA_TTABLE  6
#define R_LUA_TNIL  0
#define R_LUA_TTHREAD  8
#define R_LUA_TVECTOR  3
#define R_LUA_TLIGHTUSERDATA  2
#define R_LUA_TPROTO  10
#define R_LUA_TUPVALUE  11

// Protos 
	static constexpr int proto_k = 8;
	static constexpr int proto_sizek = 76;
	static constexpr int proto_code = 24;
	static constexpr int proto_sizep = 60;
	static constexpr int proto_locvars = 28;
	static constexpr int proto_sizecode = 68;
	static constexpr int proto_sizeupvalues = 52;
	static constexpr int proto_maxstacksize = 80;
	static constexpr int proto_sizelineinfo = 82;
	static constexpr int proto_upvalues = 56;
	static constexpr int proto_size_locvars = 32;
	static constexpr int proto_source = 20;
	static constexpr int proto_numparams = 80;

	DWORD RBX_LuaState(DWORD ScriptContext) { return  *(DWORD*)(ScriptContext + 188) + (ScriptContext + 188); }
	std::uint32_t RBX_GlobalState(std::uint32_t a1) { std::uint32_t  v1 = *reinterpret_cast<std::uint32_t*>(a1 + 28) + (a1 + 28); return v1; }
	DWORD RBX_SetIdentity(DWORD LS, int identity) { *(DWORD*)(*(DWORD*)(LS + 112) + 24) = identity; return 0; }
	int RBX_ProtoObfuscate(int p, int v) { *(int*)p = p - v; return 0; }
	int RBX_ProtoDeobfuscate(int p) { return *(int*)p + p; }
}


#define Declare(address, returnValue, callingConvention, ...) (returnValue(callingConvention*)(__VA_ARGS__))(retcheckBypass(x(address)))
#define r_lua_tostring(rL,i)	r_lua_tolstring(rL, (i), NULL)
#define r_lua_pop(rL,n)		r_lua_settop(rL, -(n)-1)
#define r_lua_getglobal(rL,s)	r_lua_getfield(rL, LUA_GLOBALSINDEX, (s))
#define r_lua_newtable(rL) r_lua_createtable(rL, 0, 0)
#define x(x) (x - 0x400000 + (DWORD)GetModuleHandleA(0)) // Basically "ASLR".

int GDM;

typedef DWORD(getdatamodel2_CCV* gdm2)();
gdm2 getdatamodel2 = (gdm2)(x(Adresses::getdatamodel2_addr));

typedef DWORD(__thiscall* getdatamodel)(DWORD, DWORD);
getdatamodel r_getdatamodel = (getdatamodel)(x(Adresses::getdatamodel_addr));

typedef int(lual_error_CCV* ee)(int e, const char* n, ...);
static ee r_luaL_error = (ee)x(Adresses::lual_error_addr); // 22

typedef void(lua_getfield_CCV* rgetfield)(DWORD rL, int idx, const char* k);
rgetfield r_lua_getfield = (rgetfield)retcheckBypass(x(Adresses::lua_getfield_addr));

typedef int(lua_pushboolean_CCV* rboolean)(unsigned int, int);
rboolean r_lua_pushboolean = (rboolean)(retcheckBypass(x(Adresses::lua_pushboolean_addr)));

typedef char* (lua_tolstring_CCV* rtolstring)(DWORD rL, int idx, size_t* size);
rtolstring r_lua_tolstring = (rtolstring)(retcheckBypass(x(Adresses::lua_tolstring_addr)));

typedef bool(lua_toboolean_CCV* toboolean)(DWORD rL, bool idx);
toboolean r_lua_toboolean = (toboolean)(x(Adresses::lua_toboolean_addr));

typedef void(lua_pushvalue_CCV* pushvalue)(DWORD rL, DWORD idx);
pushvalue r_lua_pushvalue = (pushvalue)(retcheckBypass(x(Adresses::lua_pushvalue_addr)));

typedef double(lua_pushnumber_CCV* pushnumber)(DWORD rL, double idx);
pushnumber r_lua_pushnumber = (pushnumber)(retcheckBypass(x(Adresses::lua_pushnumber_addr)));

typedef void(lua_pushstring_CCV* rpushstring)(DWORD rL, const char*);
rpushstring r_lua_pushstring = (rpushstring)(retcheckBypass(x(Adresses::lua_pushstring_addr)));

typedef int(lua_pcall_CCV* rLua_pcall)(DWORD lst, int nargs, int nresults, int errfunc);
rLua_pcall r_lua_pcall = (rLua_pcall)retcheckBypass(x(Adresses::lua_pcall_addr));

typedef DWORD(lua_next_CCV* next2)(DWORD rL, int idx);
next2 r_lua_next = (next2)(retcheckBypass(x(Adresses::lua_next_addr)));

typedef double(lua_tonumber_CCV* rtonumber)(DWORD, int, int);
rtonumber r_lua_tonumber = (rtonumber)(x(Adresses::lua_tonumber_addr));

typedef void(lua_pushcclosure_CCV* rpushcclosure)(DWORD rL, int fn, int non, int a1, int xd);
rpushcclosure r_lua_pushcclosure = (rpushcclosure)(retcheckBypass(x(Adresses::lua_pushcclosure_addr))); //r_Lua_PushCCloser_Type

typedef void(lua_createtable_CCV* rcreatetable)(DWORD rL, int num, int fix);
rcreatetable r_lua_createtable = (rcreatetable)(retcheckBypass(x(Adresses::lua_createtable_addr)));

typedef DWORD(lua_newthread_CCV* rnewthread)(DWORD);
rnewthread r_lua_newthread = (rnewthread)retcheckBypass(x(Adresses::lua_newthread_addr));

typedef void* (lua_newuserdata_CCV* rnewuserdata)(DWORD, size_t, int);
rnewuserdata r_lua_newuserdata = (rnewuserdata)(retcheckBypass(x(Adresses::lua_newuserdata_addr)));

typedef void(lua_rawgeti_CCV* rrawgeti)(DWORD, DWORD, DWORD);
rrawgeti r_lua_rawgeti = (rrawgeti)retcheckBypass(x(Adresses::lua_rawgeti_addr));

typedef void(lua_rawseti_CCV* rrawgeti)(DWORD, DWORD, DWORD);
rrawgeti r_lua_rawseti = (rrawgeti)retcheckBypass(x(Adresses::lua_rawseti_addr));

typedef void* (lua_getmetatable_CCV* rgetmetatable)(DWORD rL, int idx);
rgetmetatable r_lua_getmetatable = (rgetmetatable)(retcheckBypass(x(Adresses::lua_getmetatable_addr)));

typedef int (lua_setmetatable_CCV* rsetmetatable)(DWORD rL, int idx);
rsetmetatable r_lua_setmetatable = (rsetmetatable)(retcheckBypass(x(Adresses::lua_setmetatable_addr)));

typedef int(lua_touserdata_CCV* rtouserdata)(DWORD, int);
rtouserdata r_lua_touserdata = (rtouserdata)(retcheckBypass(x(Adresses::lua_touserdata_addr)));

typedef void(fireclickdetector_CCV* fireclick)(DWORD, float, DWORD);
fireclick FireClick = (fireclick)x(Adresses::fireclickdetector_addr);

typedef DWORD(lua_type_CCV* rtype)(DWORD, int);
rtype r_lua_type = (rtype)(x(Adresses::lua_type_addr));

typedef void* (lua_settable_CCV* rsettable)(DWORD rL, int);
rsettable r_lua_settable = (rsettable)(retcheckBypass(x(Adresses::lua_settable_addr)));

typedef DWORD(lual_ref_CCV* rref)(DWORD, DWORD);
rref r_luaL_ref = (rref)(retcheckBypass(x(Adresses::lual_ref_addr)));

int r_lua_gettop(int rL)
{
	// ls_top is Top Address and ls_base is Base Address.
	return *(DWORD*)(rL + Adresses::ls_top) - *(DWORD*)(rL + Adresses::ls_base) >> 4;
}

typedef void(lua_settop_CCV* rsettop)(DWORD rL, int idx);
rsettop r_lua_settop = (rsettop)(retcheckBypass(x(Adresses::lua_settop_addr)));

typedef void(lua_pushnil_CCV* pushnil)(DWORD);
pushnil r_lua_pushnil = (pushnil)(retcheckBypass(x(Adresses::lua_pushnil_addr)));

typedef void(lua_pushlightuserdata_CCV* rpushlight)(DWORD, void*);
rpushlight r_lua_pushlightuserdata = (rpushlight)(retcheckBypass(x(Adresses::lua_pushlightuserdata_addr)));

typedef bool(flog_getvalue_CCV* FLog)(const std::string& name, std::string& value, bool alsoCheckUnknown);
FLog GetValue = reinterpret_cast<FLog>(x(Adresses::flog_getvalue_addr));

typedef bool(flog_setvalue_CCV* SLog)(const std::string& name, const std::string& value, int Type, bool loadedFromServer);
SLog SetValue = reinterpret_cast<SLog>(x(Adresses::flog_setvalue_addr));

typedef uint32_t(index2adr_CCV* T_index2adr)(uint32_t, uint32_t);
T_index2adr r_index2adr;

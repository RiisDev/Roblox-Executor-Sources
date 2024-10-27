#pragma once

#include <cstdint>

#define DEF_ADDR(name, address) constexpr std::uintptr_t name = address

namespace offsets
{
	DEF_ADDR( some_obscure_shit, 0x1135274 + 1 ); // dpiScale *updated 11/28/2022*
	
	//DEF_ADDR( native_pass_text, 0x011354AC + 1 ); //Java_com_roblox_engine_jni_NativeGLInterface_nativePassText *updated 10/31/2022*
	//DEF_ADDR( native_pass_input, 0x011CDD60 + 1 ); //*updated 10/31/2022*
	//DEF_ADDR( native_pass_key, 0x01134538 + 1 ); //*updated 10/31/2022*
	DEF_ADDR( gl_context_ctor, 0x019F8E48 + 1 ); //[FLog::Graphics] Window size: %dx%d *updated 11/28/2022*
	DEF_ADDR( gl_context_dtor, 0x019F9C10 + 1 ); //*updated 11/28/2022*
	DEF_ADDR( egl_swapbuffers, 0x19F9CD0 + 1 ); //*updated 11/28/2022*
	DEF_ADDR( vulkan_disabled, 0x369CF58 ); //DebugGraphicsDisableVulkan *updated 11/28/2022*

	DEF_ADDR( on_game_leave, 0x111F5E4 + 1 ); // onGameLeaveBegin() SessionReporterState_GameExitRequested placeId:%lld *updated 11/28/2022*

	DEF_ADDR( luau_load, 0x27911F0 + 1 );//*updated 11 / 28 / 2022 *

	DEF_ADDR(luaD_rawrunprotected, 0x277E268 + 1);//cannot resume non-suspended coroutine *updated 11 / 28 / 2022 *
	DEF_ADDR(luau_execute, 0x2788A08 + 1); // cannot resume dead coroutine -> function looks all sorts of fucked up *updated 11 / 28 / 2022 *

	DEF_ADDR(script_context_resume, 0x12BB0BC + 1); //[FLog::ScriptContext] Resuming script: %p *updated 11 / 28 / 2022 *

	DEF_ADDR(wsj_step, 0x012C39D0 + 1); //[FLog::DataModelJobs] Waiting scripts start, data model: %p *updated 11 / 28 / 2022 *

	DEF_ADDR(nilobject, 0xA32EA0 ); //'__index' chain too long; possible loop *updated 11 / 28 / 2022 *
	DEF_ADDR(dummynode, 0xA32FB0 ); //{\"type\":\"table\",\"cat\":%d,\"size\":%d *updated 11 / 28 / 2022 *

	DEF_ADDR(lua_error, 0x277E2D8 + 1); //*updated 11 / 28 / 2022 *

	//Finalize contacts
	DEF_ADDR(touch_start, 0x25A1F24 + 1); //0 == start *updated 11 / 11 / 2022 *
	DEF_ADDR(touch_end, 0x25A2000 + 1);// *updated 11 / 22 / 2022 *
	DEF_ADDR(fire_click_detector, 0x1F2B790 + 1); // *updated 11 / 28 / 2022
	DEF_ADDR(fire_proximity_prompt, 0x01E111B0 + 1);//ProximityPrompt_Triggered *updated 11 / 28 / 2022 * 

	DEF_ADDR(set_fflag, 0x20E1584 + 1); // Java_com_roblox_engine_jni_NativeSettingsInterface_nativeSetFFlag *updated 11 / 28 / 2022 *  ----------------------------
	DEF_ADDR(get_fflag, 0x20E157C + 1); // Java_com_roblox_engine_jni_NativeSettingsInterface_nativeGetFFlag  *updated 11 / 28 / 2022 * 

	DEF_ADDR(get_thread_identity, 0x2CBDA94 + 1); // set camera movement mode for touch devices  *updated 11 / 28 / 2022 *

	DEF_ADDR(networked_dm_check, 0x01E4D98A ); // Game:HttpGetAsync is only available in non-networked DMs *updated 11 / 28 / 2022 *
	DEF_ADDR(trust_check, 0x02CC953E ); //possibly might want to force func that return value for this check to return what we want but whatevs *updated 11 / 28 / 2022 *

	DEF_ADDR(push_instance, 0x1272640 + 1); //InvalidInstance *updated 11 / 28 / 2022 *

	/* actual offsets */
	DEF_ADDR( wsj_sc_offset, 0xF8 ); //*updated 11 / 28 / 2022 *
	DEF_ADDR(identity, 0x18);//*updated 11 / 28 / 2022 *
	DEF_ADDR(sc_lua_state, 0x10c);//*updated 11 / 28 / 2022 *
	DEF_ADDR(extraspace_script, 0x30);
	DEF_ADDR( extraspace_scriptcontext, 0x28 ); 
	DEF_ADDR( loadedmodules, 0x390 );
	DEF_ADDR(require_flag, 0x108); //Cannot require a RobloxScript module from a non RobloxScript context *updated 11 / 28 / 2022 *
	DEF_ADDR(property_table, 0x376FDF4 ); // $index *updated 11 / 28 / 2022 *
	DEF_ADDR( gl_context_wnd, 0x8 );
	DEF_ADDR(task_scheduler, 0x38EA0EC ); // *updated 11 / 28 / 2022 *

	DEF_ADDR(thread_node, 0xF4 ); //ScriptContext::disassociateState -- %s *updated 11 / 11 / 2022 *
	DEF_ADDR(node_first, 4); //ScriptContext::disassociateState -- %s -> erase all refs at bottom *updated 10 / 31 / 2022 *
	DEF_ADDR(node_next, 12);
	DEF_ADDR(state_off1, 16);
	DEF_ADDR(state_off2, 4);

	DEF_ADDR(world, 0x178 );
	DEF_ADDR(primitive, 0xD0);

	DEF_ADDR(proximity_prompt_duration, 0x70); // *updated 11 / 11 / 2022 *
}
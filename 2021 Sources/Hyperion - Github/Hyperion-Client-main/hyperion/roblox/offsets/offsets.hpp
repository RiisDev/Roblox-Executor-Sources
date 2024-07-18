#pragma once
#include <Windows.h>
#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include "../bypasses/returncheck/returncheck.hpp"
extern "C" {
#include "../../lua/lobject.h"
}

namespace hyperion::structs
{
    struct job
    {
        std::uintptr_t* functions; // *reinterpret_cast<uintptr_t**>(a1 + 0);
        byte pad0[12]; // covers 4 -> 16
        std::string name; // *reinterpret_cast<std::string*>(a1 + 16);
        byte pad1[16]; // covers 40 -> 56
        double time; // *reinterpret_cast<double*>(a1 + 56);
        byte pad2[16]; // covers 64 -> 80
        double time_spend; // *reinterpret_cast<double*>(a1 + 80);
        byte pad3[8]; // covers 88 -> 96
        std::uintptr_t state; // *reinterpret_cast<uintptr_t*>(a1 + 96);
    };
}

namespace hyperion::offsets
{
	inline std::uint32_t rebase(std::uint32_t address)
	{
		return reinterpret_cast<std::uint32_t>(GetModuleHandleA(nullptr)) + (address - 0x400000);
	}

	/* table offsets */

	const auto readonly = 11;

	/* table offsets */

	/* namecall offsets */

	const auto namecall = 104; //updated

	/* namecall offsets */

	/* DM offsets */

	const auto dm_net = 1184;
	const auto inst = 12;
	const auto loaded = 576;

	/* DM offsets */

	/* identity offsets */

	const auto extraspace = 116; // unable to create a new thread for %s
	const auto identity = 24; // unable to create a new thread for %s

	/* identity offsets */

	/* state offsets */

	const auto state_obf = 188;

	/* state offsets */

	/* gc offsets */

	const auto root_gc = 48;
	const auto tt = 5;

	/* gc offsets */


	const auto closure_deobf = 24;

	const auto prop_descriptor_vector = reinterpret_cast<uintptr_t*>(rebase(0x278EA48));
	const auto class_descriptor = 12;

	//datamodel addresses
	const auto getdatamodel_offset = rebase(0x00FC8810); // GameJoinFailure updated
	const auto getdatamodelinstance_offset = rebase(0x00FC86C0); // GameJoinFailure updated

	//script scheduler addresses
	const auto get_jobs_offset = rebase(0x10375E0); // Taskscheduler::Job
	const auto get_task_scheduleroffset = rebase(0x01037820); // Taskscheduler::Job:

	//thread
	const auto newthread_offset = rebase(0x0154DA30); // unable to create a new thread for %s updated

	//bypasses
	const auto retcheckjump = rebase(0x2A0DB5D); // Any retchecked func, the jump at the end
	const auto blankfunction_offset = rebase(0x00408B52); //Sig scan for 0xCCCCCCCCCCCCCCCC in ida
	const auto blankfunction2_offset = rebase(0x0041BCB3); //Sig scan for 0xCCCCCCCCCCCCCCCC in ida
	const auto blankfunction3_offset = rebase(0x0041EBD4); //Sig scan for 0xDEADBEEF in ida
	const auto test = 0x004062C2; //Sig scan for 0xCCCCCCCCCCCCCCCC in ida
	const auto jumpdwordptr = rebase(0x005F1EE0); //cannot create event to secure UNUSED

	//luac
	const auto getfield_offset = rebase(0x0154D500); // isdst second xref
	const auto pcall_offset = rebase(0x0154DCB0); // __tostring
	const auto pushlstring_offset = rebase(0x0154DFB0); // bytecode version mismatch
	const auto pushcclosure_offset = rebase(0x0154DDC0);
	const auto setfield_offset = rebase(0x0154EA20); // isdst first xref
	const auto topointer_offset = rebase(0x0154F120);
	const auto luau_load_offset = rebase(0x01559970); // : bytecode version mismatch
	const auto spawn_offset = rebase(0x007B8D20); // Spawn function requires 1 argument - must remake the func everytime in ida stack is raped idfk
	const auto pushvalue_offset = rebase(0x0154E240);
	const auto touserdata_offset = rebase(0x0154F360);
	const auto toboolean_offset = rebase(0x0154EE80);
	const auto tolstring_offset = rebase(0x0154EF60);
	const auto tonumber_offset = rebase(0x0154F090);
	const auto pushbool_offset = rebase(0x0154DD70);
	const auto iscfunction_offset = rebase(0x0154D880);
	const auto gettop_offset = rebase(0x0154D6F0);
	const auto pushstring_offset = rebase(0x0154DFB0); //pushlstring :)
	const auto settop_offset = rebase(0x0154ED40);
	const auto setreadonly_offset = rebase(0x0154EBE0);
	const auto tobool_offset = rebase(0x0154EE80);
	const auto remove_offset = rebase(0x0154E7C0);
	const auto getmetatable_offset = rebase(0x0154D5B0);
	const auto pushnil_offset = rebase(0x0154E020);
	const auto index2addr_offset = rebase(0x0154CF20);
	const auto tostring_offset = rebase(0x0154EF60); //tolstring :)
	const auto createtable_offset = rebase(0x0154D1F0);
	const auto rawseti_offset = rebase(0x0154E6F0);
	const auto sandboxthread_offset = rebase(0x7A74E0); //NOT THE ONE THAT SET IDENTITY TOO THE ONE INSIDE 
	const auto replace_offset = rebase(0x0154E850);
	const auto xmove_offset = rebase(0x0154F500);
	const auto call_offset = rebase(0x0154D030);
	const auto get_globalstate_offset = rebase(0x007AB3A0);
	const auto pushlightuserdata_offset = rebase(0x0154DF60);
	const auto settable_offset = rebase(0x0154ECC0);
	const auto yield_offset = rebase(0x01550D10);
	const auto rawgeti_offset = rebase(0x0154E5A0);
	const auto pushnumber_offset = rebase(0x0154E070);
	const auto getupvalue_offset = rebase(0x0154D710);
	const auto setmetatable_offset = rebase(0x0154EAD0);
	const auto getproperty_offset = rebase(0x876640); //$newindex
	const auto to_string = rebase(0x154F1E0); // NOT TOLSTRING
	 
	/* custom funcs addresses */
	const auto fireproximityprompt_offset = rebase(0xCB80D0);
	const auto fireclickdetector_offset = rebase(0x00C6B7F0);
	const auto firetouchinterest_offset = rebase(0x937350);
	const auto impersonator_offset = rebase(0x154B610);
	const auto getfflag_offset = rebase(0x01760280);
	const auto setfflag_offset = rebase(0x017609F0);
}

namespace hyperion::functions
{
	inline uintptr_t get_state(uintptr_t script_context)
	{
		return (script_context + hyperion::offsets::state_obf) + *reinterpret_cast<uintptr_t*>(script_context + hyperion::offsets::state_obf); // :)
	}

	//datamodel functions
	const auto getdatamodel = reinterpret_cast<std::uintptr_t(__thiscall*)(std::uint32_t, std::uint32_t)>(hyperion::offsets::getdatamodel_offset);
	const auto getdatamodel_instance = reinterpret_cast<std::uintptr_t(*)()>(hyperion::offsets::getdatamodelinstance_offset); 

	//script scheduler functions
	const auto get_jobs = reinterpret_cast<void(*)(std::vector<std::shared_ptr<hyperion::structs::job>>*)>(hyperion::offsets::get_jobs_offset); 
	const auto get_task_scheduler = reinterpret_cast<std::uintptr_t(*)()>(hyperion::offsets::get_task_scheduleroffset); 

	static auto newthread = reinterpret_cast<std::uintptr_t(*)(std::uintptr_t)>(bypasses::returncheck::return_safe_address(hyperion::offsets::newthread_offset));

	//luac
	const auto getfield = reinterpret_cast<std::uintptr_t(__fastcall*)(std::uintptr_t, std::int32_t, const char*)>(bypasses::returncheck::return_safe_address(hyperion::offsets::getfield_offset));
	const auto pcall = reinterpret_cast<std::uintptr_t(*)(std::uintptr_t, std::int32_t, std::int32_t, std::int32_t)>(bypasses::returncheck::return_safe_address(hyperion::offsets::pcall_offset));
	const auto pushlstring = reinterpret_cast<std::uint8_t*(*)(std::uintptr_t, const char*, std::int32_t)>(bypasses::returncheck::return_safe_address(hyperion::offsets::pushlstring_offset));
	const auto pushcclosure = reinterpret_cast<std::uint8_t*(*)(std::uintptr_t, std::int32_t, int, std::int32_t, std::int32_t)>(bypasses::returncheck::return_safe_address(hyperion::offsets::pushcclosure_offset));
	const auto setfield = reinterpret_cast<std::uintptr_t(__stdcall*)(std::uintptr_t, std::int32_t, const char*)>(bypasses::returncheck::return_safe_address(hyperion::offsets::setfield_offset));
	const auto topointer = reinterpret_cast<std::uintptr_t(*)(std::uintptr_t, std::int32_t)>(bypasses::returncheck::return_safe_address(hyperion::offsets::topointer_offset));
	const auto luau_load = reinterpret_cast<std::uintptr_t(*)(std::uintptr_t, const char*, const char*, int, int)>(hyperion::offsets::luau_load_offset);
	const auto spawn = reinterpret_cast<std::uintptr_t(*)(std::uintptr_t)>(hyperion::offsets::spawn_offset);
	const auto push_value = reinterpret_cast<std::uintptr_t(__fastcall*)(std::uintptr_t, std::int32_t)>(bypasses::returncheck::return_safe_address(hyperion::offsets::pushvalue_offset));
	const auto touserdata = reinterpret_cast<void*(*)(std::uintptr_t, std::int32_t)>(bypasses::returncheck::return_safe_address(hyperion::offsets::touserdata_offset));
	const auto toboolean = reinterpret_cast<bool(*)(std::uintptr_t, std::int32_t)>(bypasses::returncheck::return_safe_address(hyperion::offsets::toboolean_offset));
	const auto tolstring = reinterpret_cast<const char*(*)(std::uintptr_t, std::int32_t, size_t*)>(bypasses::returncheck::return_safe_address(hyperion::offsets::tolstring_offset));
	const auto tonumber = reinterpret_cast<double(*)(std::uintptr_t, std::int32_t, int unk)>(bypasses::returncheck::return_safe_address(hyperion::offsets::tonumber_offset));
	const auto push_bool = reinterpret_cast<int(*)(std::uintptr_t, bool)>(bypasses::returncheck::return_safe_address(hyperion::offsets::pushbool_offset));
	const auto is_cfunction = reinterpret_cast<bool(*)(std::uintptr_t, std::int32_t)>(bypasses::returncheck::return_safe_address(hyperion::offsets::iscfunction_offset));
	const auto gettop = reinterpret_cast<int(*)(std::uintptr_t)>(bypasses::returncheck::return_safe_address(hyperion::offsets::gettop_offset));
	const auto settop = reinterpret_cast<void(__stdcall*)(std::uintptr_t, int)>(bypasses::returncheck::return_safe_address(hyperion::offsets::settop_offset));
	const auto push_string = reinterpret_cast<void(*)(std::uintptr_t, const char*, int)>(bypasses::returncheck::return_safe_address(hyperion::offsets::pushstring_offset));
	const auto tobool = reinterpret_cast<bool(*)(std::uintptr_t, int)>(bypasses::returncheck::return_safe_address(hyperion::offsets::tobool_offset));
	const auto readreadonly = reinterpret_cast<void(*)(std::uintptr_t, int, bool)>(bypasses::returncheck::return_safe_address(hyperion::offsets::setreadonly_offset));
	const auto remove = reinterpret_cast<void(*)(std::uintptr_t, int)>(bypasses::returncheck::return_safe_address(hyperion::offsets::remove_offset));
	const auto getmetatable = reinterpret_cast<int(*)(std::uintptr_t, int)>(bypasses::returncheck::return_safe_address(hyperion::offsets::getmetatable_offset));
	const auto push_nil = reinterpret_cast<void(*)(std::uintptr_t)>(bypasses::returncheck::return_safe_address(hyperion::offsets::pushnil_offset));
	const auto index2addr = reinterpret_cast<int(*)(std::uintptr_t, int)>(bypasses::returncheck::return_safe_address(hyperion::offsets::index2addr_offset));
	const auto tostring = reinterpret_cast<const char*(*)(std::uintptr_t, int, size_t)>(bypasses::returncheck::return_safe_address(hyperion::offsets::tostring_offset));
	const auto create_table = reinterpret_cast<void(*)(std::uintptr_t, int, int)>(bypasses::returncheck::return_safe_address(hyperion::offsets::createtable_offset));
	const auto rawseti = reinterpret_cast<void(*)(std::uintptr_t, int, int)>(bypasses::returncheck::return_safe_address(hyperion::offsets::rawseti_offset));
	const auto sandboxthread = reinterpret_cast<int(*)(std::uintptr_t)>(bypasses::returncheck::return_safe_address(hyperion::offsets::sandboxthread_offset));
	const auto replace = reinterpret_cast<void(*)(std::uintptr_t, int)>(bypasses::returncheck::return_safe_address(hyperion::offsets::replace_offset));
	const auto xmove = reinterpret_cast<void(*)(std::uintptr_t, std::uintptr_t, int)>(bypasses::returncheck::return_safe_address(hyperion::offsets::xmove_offset));
	const auto call = reinterpret_cast<void(*)(std::uintptr_t, int, int)>(bypasses::returncheck::return_safe_address(hyperion::offsets::call_offset));
	const auto get_globalstate = reinterpret_cast<uintptr_t(*)(std::uintptr_t)>(bypasses::returncheck::return_safe_address(hyperion::offsets::get_globalstate_offset));
	const auto push_light_userdata = reinterpret_cast<void(*)(std::uintptr_t, void*)>(bypasses::returncheck::return_safe_address(hyperion::offsets::pushlightuserdata_offset));
	const auto settable = reinterpret_cast<int(*)(std::uintptr_t, int)>(bypasses::returncheck::return_safe_address(hyperion::offsets::settable_offset));
	const auto yield = reinterpret_cast<int(*)(std::uintptr_t, int)>(bypasses::returncheck::return_safe_address(hyperion::offsets::yield_offset));
	const auto rawgeti = reinterpret_cast<int(*)(std::uintptr_t, int, int)>(bypasses::returncheck::return_safe_address(hyperion::offsets::rawgeti_offset));
	const auto pushnumber = reinterpret_cast<int(*)(std::uintptr_t, double)>(bypasses::returncheck::return_safe_address(hyperion::offsets::pushnumber_offset));
	const auto getupvalue = reinterpret_cast<const char*(*)(std::uintptr_t, int64_t, int)>(bypasses::returncheck::return_safe_address(hyperion::offsets::getupvalue_offset));
	const auto getproperty = reinterpret_cast<uintptr_t (__thiscall*)(std::uintptr_t, int)>(bypasses::returncheck::return_safe_address(hyperion::offsets::getproperty_offset));
	const auto setmetatable = reinterpret_cast<uintptr_t(*)(std::uintptr_t, int)>(bypasses::returncheck::return_safe_address(hyperion::offsets::setmetatable_offset));
	const auto tostr = reinterpret_cast<std::uintptr_t(*)(std::uintptr_t, std::int32_t)>(offsets::to_string);

	/* custom funcs definitions */
	const auto fireproximityprompt = reinterpret_cast<void(__thiscall*)(std::uintptr_t)>(offsets::fireproximityprompt_offset);
	const auto fireclickdetector = reinterpret_cast<void(__thiscall*)(std::uintptr_t, float, std::uintptr_t)>(offsets::fireclickdetector_offset);
	const auto firetouchinterest = reinterpret_cast<void(__thiscall*)(std::uintptr_t, std::uintptr_t, std::uintptr_t, int)>(offsets::firetouchinterest_offset);
	const auto impersonator = reinterpret_cast<void(__thiscall*)(std::uintptr_t*, int)>(offsets::impersonator_offset);
	const auto setfflag = reinterpret_cast<bool(*)(const std::string & name, const std::string & value, int Type, bool loadedFromServer)>(offsets::setfflag_offset);
	const auto getfflag = reinterpret_cast<bool(*)(const std::string & name, std::string & value, bool alsoCheckUnknown)>(offsets::getfflag_offset);
}

namespace hyperion::obfuscation
{
	inline uintptr_t get_proto(uintptr_t closure)
	{
		return *reinterpret_cast<uintptr_t*>(closure + offsets::closure_deobf) - (closure + offsets::closure_deobf);
	}

	inline uintptr_t get_proto_member(uintptr_t closure, int member)
	{
		const auto proto = get_proto(closure);

		return (proto + member) - *reinterpret_cast<uintptr_t*>(proto + member);
	}

	inline uintptr_t index_mem_array(uintptr_t array, int size, int index)
	{
		return (array + size * (index));
	}

	inline uintptr_t get_closure_name(uintptr_t closure)
	{
		return (closure + 32) + *reinterpret_cast<uintptr_t*>(closure + 32);
	}

	inline void set_closure_name(uintptr_t closure, uintptr_t name)
	{
		*reinterpret_cast<uintptr_t*>(closure + 32) = closure + 32 + name;
	}

	inline uintptr_t get_global_state(uintptr_t ls)
	{
		return (ls + 20) ^ *reinterpret_cast<uintptr_t*>(ls + 20);
	}
}

namespace hyperion::types
{
	const auto r_lua_tnone = -1;
	const auto r_lua_tuserdata = 8;
	const auto r_lua_tfunction = 6;
	const auto r_lua_tstring = 5;
	const auto r_lua_tboolean = 1;
	const auto r_lua_tnumber = 2;
	const auto r_lua_ttable = 9;
	const auto r_lua_tnil = 0;
	const auto r_lua_tthread = 7;
	const auto r_lua_tvector = 4;
	const auto r_lua_tlightuserdata = 3;
	const auto r_lua_tproto = 10;
	const auto r_lua_tupvalue = 11;
}

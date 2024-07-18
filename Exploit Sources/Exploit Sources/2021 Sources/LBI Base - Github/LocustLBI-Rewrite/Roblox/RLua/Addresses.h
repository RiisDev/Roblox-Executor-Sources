#pragma once
// Auto Lua Type: https://github.com/Mellonyt/Roblox-Lua-Type-Scanner 
// Scanning Time: 3.000000 Secs 
// Dumping Date: Mon Jul 19 18:38:40 2021
// For version-f98ef77f473148f6 
// Dumped With Mellonyt aka Mellon Addy Dumper My Discord: mellonyt#1234 
#include <Windows.h> 
#include<cstdint> 
namespace Adresses {
	/*

	* addinfo_Decompiled_Args: int a1
	* addinfo_TypeDef: int
	* addinfo_Aob: Unable to generate aob.
	* addinfo_ProtectedByRetcheck: false
	* addinfo_IsApartOfLua: true
	* addinfo_Mask: ....
	* Usage (Not 100% Accurate):

	using r_addinfo_typedef = int(__cdecl*)(int a1);
	r_addinfo_typedef r_addinfo = (r_addinfo_typedef)(x(0x016691C0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_addinfo_typedef = int(addinfo_CCV*)(int a1);
	r_addinfo_typedef r_addinfo = (r_addinfo_typedef)(x(Adresses::addinfo_addr));



	*/
	static constexpr std::uintptr_t addinfo_addr = 0x016691C0;
#define addinfo_CCV __cdecl

	/*

	* arith_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* arith_TypeDef: int
	* arith_Aob: Unable to generate aob.
	* arith_ProtectedByRetcheck: false
	* arith_IsApartOfLua: true
	* arith_Mask: ....
	* Usage (Not 100% Accurate):

	using r_arith_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_arith_typedef r_arith = (r_arith_typedef)(x(0x01675710));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_arith_typedef = int(arith_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_arith_typedef r_arith = (r_arith_typedef)(x(Adresses::arith_addr));



	*/
	static constexpr std::uintptr_t arith_addr = 0x01675710;
#define arith_CCV __cdecl

	/*

	* appendarray_Decompiled_Args: int a1, int a2
	* appendarray_TypeDef: int
	* appendarray_Aob: Unable to generate aob.
	* appendarray_ProtectedByRetcheck: false
	* appendarray_IsApartOfLua: false
	* appendarray_Mask: ....
	* Usage (Not 100% Accurate):

	using r_appendarray_typedef = int(__thiscall*)(int a1, int a2);
	r_appendarray_typedef r_appendarray = (r_appendarray_typedef)(x(0x005CC9D0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_appendarray_typedef = int(appendarray_CCV*)(int a1, int a2);
	r_appendarray_typedef r_appendarray = (r_appendarray_typedef)(x(Adresses::appendarray_addr));



	*/
	static constexpr std::uintptr_t appendarray_addr = 0x005CC9D0;
#define appendarray_CCV __thiscall

	/*

	* aux_upvalue_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* aux_upvalue_TypeDef: int
	* aux_upvalue_Aob: Unable to generate aob.
	* aux_upvalue_ProtectedByRetcheck: false
	* aux_upvalue_IsApartOfLua: true
	* aux_upvalue_Mask: ....
	* Usage (Not 100% Accurate):

	using r_aux_upvalue_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_aux_upvalue_typedef r_aux_upvalue = (r_aux_upvalue_typedef)(x(0x0165A2C0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_aux_upvalue_typedef = int(aux_upvalue_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_aux_upvalue_typedef r_aux_upvalue = (r_aux_upvalue_typedef)(x(Adresses::aux_upvalue_addr));



	*/
	static constexpr std::uintptr_t aux_upvalue_addr = 0x0165A2C0;
#define aux_upvalue_CCV __cdecl

	/*

	* auxgetinfo_Decompiled_Args: std::uintptr_t a1, int a2, int a3, int a4, int a5
	* auxgetinfo_TypeDef: int
	* auxgetinfo_Aob: Unable to generate aob.
	* auxgetinfo_ProtectedByRetcheck: false
	* auxgetinfo_IsApartOfLua: true
	* auxgetinfo_Mask: ....
	* Usage (Not 100% Accurate):

	using r_auxgetinfo_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3, int a4, int a5);
	r_auxgetinfo_typedef r_auxgetinfo = (r_auxgetinfo_typedef)(x(0x01668850));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_auxgetinfo_typedef = int(auxgetinfo_CCV*)(std::uintptr_t a1, int a2, int a3, int a4, int a5);
	r_auxgetinfo_typedef r_auxgetinfo = (r_auxgetinfo_typedef)(x(Adresses::auxgetinfo_addr));



	*/
	static constexpr std::uintptr_t auxgetinfo_addr = 0x01668850;
#define auxgetinfo_CCV __cdecl

	/*

	* auxresume_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* auxresume_TypeDef: int
	* auxresume_Aob: Unable to generate aob.
	* auxresume_ProtectedByRetcheck: false
	* auxresume_IsApartOfLua: true
	* auxresume_Mask: ....
	* Usage (Not 100% Accurate):

	using r_auxresume_typedef = int(__thiscall*)(std::uintptr_t a1, int a2, int a3);
	r_auxresume_typedef r_auxresume = (r_auxresume_typedef)(x(0x0166A1E0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_auxresume_typedef = int(auxresume_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_auxresume_typedef r_auxresume = (r_auxresume_typedef)(x(Adresses::auxresume_addr));



	*/
	static constexpr std::uintptr_t auxresume_addr = 0x0166A1E0;
#define auxresume_CCV __thiscall

	/*

	* call_bintm_Decompiled_Args: std::uintptr_t a1, int a2, int a3, int a4
	* call_bintm_TypeDef: int
	* call_bintm_Aob: Unable to generate aob.
	* call_bintm_ProtectedByRetcheck: false
	* call_bintm_IsApartOfLua: true
	* call_bintm_Mask: ....
	* Usage (Not 100% Accurate):

	using r_call_bintm_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_call_bintm_typedef r_call_bintm = (r_call_bintm_typedef)(x(0x01674CA0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_call_bintm_typedef = int(call_bintm_CCV*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_call_bintm_typedef r_call_bintm = (r_call_bintm_typedef)(x(Adresses::call_bintm_addr));



	*/
	static constexpr std::uintptr_t call_bintm_addr = 0x01674CA0;
#define call_bintm_CCV __cdecl

	/*

	* call_ordertm_Decompiled_Args: std::uintptr_t a1, int a2
	* call_ordertm_TypeDef: int
	* call_ordertm_Aob: Unable to generate aob.
	* call_ordertm_ProtectedByRetcheck: false
	* call_ordertm_IsApartOfLua: true
	* call_ordertm_Mask: ....
	* Usage (Not 100% Accurate):

	using r_call_ordertm_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_call_ordertm_typedef r_call_ordertm = (r_call_ordertm_typedef)(x(0x01674D40));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_call_ordertm_typedef = int(call_ordertm_CCV*)(std::uintptr_t a1, int a2);
	r_call_ordertm_typedef r_call_ordertm = (r_call_ordertm_typedef)(x(Adresses::call_ordertm_addr));



	*/
	static constexpr std::uintptr_t call_ordertm_addr = 0x01674D40;
#define call_ordertm_CCV __cdecl

	/*

	* calltmres_Decompiled_Args: std::uintptr_t a1, int a2, int a3, int a4, int a5
	* calltmres_TypeDef: int
	* calltmres_Aob: Unable to generate aob.
	* calltmres_ProtectedByRetcheck: false
	* calltmres_IsApartOfLua: true
	* calltmres_Mask: ....
	* Usage (Not 100% Accurate):

	using r_calltmres_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3, int a4, int a5);
	r_calltmres_typedef r_calltmres = (r_calltmres_typedef)(x(0x01675870));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_calltmres_typedef = int(calltmres_CCV*)(std::uintptr_t a1, int a2, int a3, int a4, int a5);
	r_calltmres_typedef r_calltmres = (r_calltmres_typedef)(x(Adresses::calltmres_addr));



	*/
	static constexpr std::uintptr_t calltmres_addr = 0x01675870;
#define calltmres_CCV __cdecl

	/*

	* close_state_Decompiled_Args: int a1
	* close_state_TypeDef: int
	* close_state_Aob: Unable to generate aob.
	* close_state_ProtectedByRetcheck: false
	* close_state_IsApartOfLua: true
	* close_state_Mask: ....
	* Usage (Not 100% Accurate):

	using r_close_state_typedef = int(__cdecl*)(int a1);
	r_close_state_typedef r_close_state = (r_close_state_typedef)(x(0x01668100));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_close_state_typedef = int(close_state_CCV*)(int a1);
	r_close_state_typedef r_close_state = (r_close_state_typedef)(x(Adresses::close_state_addr));



	*/
	static constexpr std::uintptr_t close_state_addr = 0x01668100;
#define close_state_CCV __cdecl

	/*

	* err_geterrorstring_Decompiled_Args: int a1
	* err_geterrorstring_TypeDef: int
	* err_geterrorstring_Aob: Unable to generate aob.
	* err_geterrorstring_ProtectedByRetcheck: false
	* err_geterrorstring_IsApartOfLua: false
	* err_geterrorstring_Mask: ....
	* Usage (Not 100% Accurate):

	using r_err_geterrorstring_typedef = int(__cdecl*)(int a1);
	r_err_geterrorstring_typedef r_err_geterrorstring = (r_err_geterrorstring_typedef)(x(0x00968000));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_err_geterrorstring_typedef = int(err_geterrorstring_CCV*)(int a1);
	r_err_geterrorstring_typedef r_err_geterrorstring = (r_err_geterrorstring_typedef)(x(Adresses::err_geterrorstring_addr));



	*/
	static constexpr std::uintptr_t err_geterrorstring_addr = 0x00968000;
#define err_geterrorstring_CCV __cdecl

	/*

	* f_call_Decompiled_Args: std::uintptr_t a1, int a2
	* f_call_TypeDef: int
	* f_call_Aob: Unable to generate aob.
	* f_call_ProtectedByRetcheck: true
	* f_call_IsApartOfLua: true
	* f_call_Mask: ....
	* Usage (Not 100% Accurate):

	using r_f_call_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_f_call_typedef r_f_call = (r_f_call_typedef)unprotect(x(0x0165A350));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_f_call_typedef = int(f_call_CCV*)(std::uintptr_t a1, int a2);
	r_f_call_typedef r_f_call = (r_f_call_typedef)unprotect(x(Adresses::f_call_addr));



	*/
	static constexpr std::uintptr_t f_call_addr = 0x0165A350;
#define f_call_CCV __cdecl

	/*

	* f_luaopen_Decompiled_Args: int a1
	* f_luaopen_TypeDef: int
	* f_luaopen_Aob: Unable to generate aob.
	* f_luaopen_ProtectedByRetcheck: false
	* f_luaopen_IsApartOfLua: true
	* f_luaopen_Mask: ....
	* Usage (Not 100% Accurate):

	using r_f_luaopen_typedef = int(__cdecl*)(int a1);
	r_f_luaopen_typedef r_f_luaopen = (r_f_luaopen_typedef)(x(0x01668190));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_f_luaopen_typedef = int(f_luaopen_CCV*)(int a1);
	r_f_luaopen_typedef r_f_luaopen = (r_f_luaopen_typedef)(x(Adresses::f_luaopen_addr));



	*/
	static constexpr std::uintptr_t f_luaopen_addr = 0x01668190;
#define f_luaopen_CCV __cdecl

	/*

	* findfirstchild_Decompiled_Args: int a1, int a2
	* findfirstchild_TypeDef: int
	* findfirstchild_Aob: Unable to generate aob.
	* findfirstchild_ProtectedByRetcheck: false
	* findfirstchild_IsApartOfLua: false
	* findfirstchild_Mask: ....
	* Usage (Not 100% Accurate):

	using r_findfirstchild_typedef = int(__stdcall*)(int a1, int a2);
	r_findfirstchild_typedef r_findfirstchild = (r_findfirstchild_typedef)(x(0x00656700));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_findfirstchild_typedef = int(findfirstchild_CCV*)(int a1, int a2);
	r_findfirstchild_typedef r_findfirstchild = (r_findfirstchild_typedef)(x(Adresses::findfirstchild_addr));



	*/
	static constexpr std::uintptr_t findfirstchild_addr = 0x00656700;
#define findfirstchild_CCV __stdcall

	/*

	* findfirstchildofclass_Decompiled_Args: int a1, int a2
	* findfirstchildofclass_TypeDef: int
	* findfirstchildofclass_Aob: Unable to generate aob.
	* findfirstchildofclass_ProtectedByRetcheck: false
	* findfirstchildofclass_IsApartOfLua: false
	* findfirstchildofclass_Mask: ....
	* Usage (Not 100% Accurate):

	using r_findfirstchildofclass_typedef = int(__stdcall*)(int a1, int a2);
	r_findfirstchildofclass_typedef r_findfirstchildofclass = (r_findfirstchildofclass_typedef)(x(0x006569F0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_findfirstchildofclass_typedef = int(findfirstchildofclass_CCV*)(int a1, int a2);
	r_findfirstchildofclass_typedef r_findfirstchildofclass = (r_findfirstchildofclass_typedef)(x(Adresses::findfirstchildofclass_addr));



	*/
	static constexpr std::uintptr_t findfirstchildofclass_addr = 0x006569F0;
#define findfirstchildofclass_CCV __stdcall

	/*

	* findlocal_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* findlocal_TypeDef: int
	* findlocal_Aob: Unable to generate aob.
	* findlocal_ProtectedByRetcheck: false
	* findlocal_IsApartOfLua: true
	* findlocal_Mask: ....
	* Usage (Not 100% Accurate):

	using r_findlocal_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_findlocal_typedef r_findlocal = (r_findlocal_typedef)(x(0x01674970));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_findlocal_typedef = int(findlocal_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_findlocal_typedef r_findlocal = (r_findlocal_typedef)(x(Adresses::findlocal_addr));



	*/
	static constexpr std::uintptr_t findlocal_addr = 0x01674970;
#define findlocal_CCV __cdecl

	/*

	* fireclickdetector_Decompiled_Args: std::uintptr_t a1, float a2, int a3
	* fireclickdetector_TypeDef: int
	* fireclickdetector_Aob: Unable to generate aob.
	* fireclickdetector_ProtectedByRetcheck: false
	* fireclickdetector_IsApartOfLua: false
	* fireclickdetector_Mask: ....
	* Usage (Not 100% Accurate):

	using r_fireclickdetector_typedef = int(__cdecl*)(std::uintptr_t a1, float a2, int a3);
	r_fireclickdetector_typedef r_fireclickdetector = (r_fireclickdetector_typedef)(x(0x00D74E60));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_fireclickdetector_typedef = int(fireclickdetector_CCV*)(std::uintptr_t a1, float a2, int a3);
	r_fireclickdetector_typedef r_fireclickdetector = (r_fireclickdetector_typedef)(x(Adresses::fireclickdetector_addr));



	*/
	static constexpr std::uintptr_t fireclickdetector_addr = 0x00D74E60;
#define fireclickdetector_CCV __cdecl

	/*

	* fireproximityprompt_Decompiled_Args:
	* fireproximityprompt_TypeDef: int
	* fireproximityprompt_Aob: Unable to generate aob.
	* fireproximityprompt_ProtectedByRetcheck: false
	* fireproximityprompt_IsApartOfLua: false
	* fireproximityprompt_Mask: ....
	* Usage (Not 100% Accurate):

	using r_fireproximityprompt_typedef = int(__cdecl*)();
	r_fireproximityprompt_typedef r_fireproximityprompt = (r_fireproximityprompt_typedef)(x(0x00DE0D20));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_fireproximityprompt_typedef = int(fireproximityprompt_CCV*)();
	r_fireproximityprompt_typedef r_fireproximityprompt = (r_fireproximityprompt_typedef)(x(Adresses::fireproximityprompt_addr));



	*/
	static constexpr std::uintptr_t fireproximityprompt_addr = 0x00DE0D20;
#define fireproximityprompt_CCV __cdecl

	/*

	* flog_getvalue_Decompiled_Args: int a1, int a2, const char* a3
	* flog_getvalue_TypeDef: int
	* flog_getvalue_Aob: Unable to generate aob.
	* flog_getvalue_ProtectedByRetcheck: false
	* flog_getvalue_IsApartOfLua: false
	* flog_getvalue_Mask: ....
	* Usage (Not 100% Accurate):

	using r_flog_getvalue_typedef = int(__cdecl*)(int a1, int a2, const char* a3);
	r_flog_getvalue_typedef r_flog_getvalue = (r_flog_getvalue_typedef)(x(0x018C4AE0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_flog_getvalue_typedef = int(flog_getvalue_CCV*)(int a1, int a2, const char* a3);
	r_flog_getvalue_typedef r_flog_getvalue = (r_flog_getvalue_typedef)(x(Adresses::flog_getvalue_addr));



	*/
	static constexpr std::uintptr_t flog_getvalue_addr = 0x018C4AE0;
#define flog_getvalue_CCV __cdecl

	/*

	* flog_setvalue_Decompiled_Args: int a1, int a2, int a3, const char* a4
	* flog_setvalue_TypeDef: int
	* flog_setvalue_Aob: Unable to generate aob.
	* flog_setvalue_ProtectedByRetcheck: false
	* flog_setvalue_IsApartOfLua: false
	* flog_setvalue_Mask: ....
	* Usage (Not 100% Accurate):

	using r_flog_setvalue_typedef = int(__stdcall*)(int a1, int a2, int a3, const char* a4);
	r_flog_setvalue_typedef r_flog_setvalue = (r_flog_setvalue_typedef)(x(0x018C6C10));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_flog_setvalue_typedef = int(flog_setvalue_CCV*)(int a1, int a2, int a3, const char* a4);
	r_flog_setvalue_typedef r_flog_setvalue = (r_flog_setvalue_typedef)(x(Adresses::flog_setvalue_addr));



	*/
	static constexpr std::uintptr_t flog_setvalue_addr = 0x018C6C10;
#define flog_setvalue_CCV __stdcall

	/*

	* getdatamodel_Decompiled_Args: int a1, int a2
	* getdatamodel_TypeDef: int
	* getdatamodel_Aob: Unable to generate aob.
	* getdatamodel_ProtectedByRetcheck: false
	* getdatamodel_IsApartOfLua: false
	* getdatamodel_Mask: ....
	* Usage (Not 100% Accurate):

	using r_getdatamodel_typedef = int(__thiscall*)(int a1, int a2);
	r_getdatamodel_typedef r_getdatamodel = (r_getdatamodel_typedef)(x(0x01080F70));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_getdatamodel_typedef = int(getdatamodel_CCV*)(int a1, int a2);
	r_getdatamodel_typedef r_getdatamodel = (r_getdatamodel_typedef)(x(Adresses::getdatamodel_addr));



	*/
	static constexpr std::uintptr_t getdatamodel_addr = 0x01080F70;
#define getdatamodel_CCV __thiscall

	/*

	* getdatamodel2_Decompiled_Args:
	* getdatamodel2_TypeDef: int
	* getdatamodel2_Aob: Unable to generate aob.
	* getdatamodel2_ProtectedByRetcheck: false
	* getdatamodel2_IsApartOfLua: false
	* getdatamodel2_Mask: ....
	* Usage (Not 100% Accurate):

	using r_getdatamodel2_typedef = int(__cdecl*)();
	r_getdatamodel2_typedef r_getdatamodel2 = (r_getdatamodel2_typedef)(x(0x01080E20));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_getdatamodel2_typedef = int(getdatamodel2_CCV*)();
	r_getdatamodel2_typedef r_getdatamodel2 = (r_getdatamodel2_typedef)(x(Adresses::getdatamodel2_addr));



	*/
	static constexpr std::uintptr_t getdatamodel2_addr = 0x01080E20;
#define getdatamodel2_CCV __cdecl

	/*

	* getfunc_Decompiled_Args: std::uintptr_t a1, int a2
	* getfunc_TypeDef: int
	* getfunc_Aob: Unable to generate aob.
	* getfunc_ProtectedByRetcheck: false
	* getfunc_IsApartOfLua: true
	* getfunc_Mask: ....
	* Usage (Not 100% Accurate):

	using r_getfunc_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_getfunc_typedef r_getfunc = (r_getfunc_typedef)(x(0x016699B0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_getfunc_typedef = int(getfunc_CCV*)(std::uintptr_t a1, int a2);
	r_getfunc_typedef r_getfunc = (r_getfunc_typedef)(x(Adresses::getfunc_addr));



	*/
	static constexpr std::uintptr_t getfunc_addr = 0x016699B0;
#define getfunc_CCV __cdecl

	/*

	* getjobsbyname_Decompiled_Args: int a1, int a2
	* getjobsbyname_TypeDef: int
	* getjobsbyname_Aob: Unable to generate aob.
	* getjobsbyname_ProtectedByRetcheck: false
	* getjobsbyname_IsApartOfLua: false
	* getjobsbyname_Mask: ....
	* Usage (Not 100% Accurate):

	using r_getjobsbyname_typedef = int(__cdecl*)(int a1, int a2);
	r_getjobsbyname_typedef r_getjobsbyname = (r_getjobsbyname_typedef)(x(0x0113BAD0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_getjobsbyname_typedef = int(getjobsbyname_CCV*)(int a1, int a2);
	r_getjobsbyname_typedef r_getjobsbyname = (r_getjobsbyname_typedef)(x(Adresses::getjobsbyname_addr));



	*/
	static constexpr std::uintptr_t getjobsbyname_addr = 0x0113BAD0;
#define getjobsbyname_CCV __cdecl

	/*

	* getnamecallmethod_Decompiled_Args: int a1
	* getnamecallmethod_TypeDef: int
	* getnamecallmethod_Aob: Unable to generate aob.
	* getnamecallmethod_ProtectedByRetcheck: false
	* getnamecallmethod_IsApartOfLua: true
	* getnamecallmethod_Mask: ....
	* Usage (Not 100% Accurate):

	using r_getnamecallmethod_typedef = int(__cdecl*)(int a1);
	r_getnamecallmethod_typedef r_getnamecallmethod = (r_getnamecallmethod_typedef)(x(0x0165CB50));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_getnamecallmethod_typedef = int(getnamecallmethod_CCV*)(int a1);
	r_getnamecallmethod_typedef r_getnamecallmethod = (r_getnamecallmethod_typedef)(x(Adresses::getnamecallmethod_addr));



	*/
	static constexpr std::uintptr_t getnamecallmethod_addr = 0x0165CB50;
#define getnamecallmethod_CCV __cdecl

	/*

	* grabscriptcontext_Decompiled_Args: int a1, const char* a2, int a3
	* grabscriptcontext_TypeDef: int
	* grabscriptcontext_Aob: Unable to generate aob.
	* grabscriptcontext_ProtectedByRetcheck: false
	* grabscriptcontext_IsApartOfLua: false
	* grabscriptcontext_Mask: ....
	* Usage (Not 100% Accurate):

	using r_grabscriptcontext_typedef = int(__thiscall*)(int a1, const char* a2, int a3);
	r_grabscriptcontext_typedef r_grabscriptcontext = (r_grabscriptcontext_typedef)(x(0x005E7240));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_grabscriptcontext_typedef = int(grabscriptcontext_CCV*)(int a1, const char* a2, int a3);
	r_grabscriptcontext_typedef r_grabscriptcontext = (r_grabscriptcontext_typedef)(x(Adresses::grabscriptcontext_addr));



	*/
	static constexpr std::uintptr_t grabscriptcontext_addr = 0x005E7240;
#define grabscriptcontext_CCV __thiscall

	/*

	* grabglobalstateindex_Decompiled_Args: int a1, int a2
	* grabglobalstateindex_TypeDef: int
	* grabglobalstateindex_Aob: Unable to generate aob.
	* grabglobalstateindex_ProtectedByRetcheck: false
	* grabglobalstateindex_IsApartOfLua: false
	* grabglobalstateindex_Mask: ....
	* Usage (Not 100% Accurate):

	using r_grabglobalstateindex_typedef = int(__thiscall*)(int a1, int a2);
	r_grabglobalstateindex_typedef r_grabglobalstateindex = (r_grabglobalstateindex_typedef)(x(0x007B9920));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_grabglobalstateindex_typedef = int(grabglobalstateindex_CCV*)(int a1, int a2);
	r_grabglobalstateindex_typedef r_grabglobalstateindex = (r_grabglobalstateindex_typedef)(x(Adresses::grabglobalstateindex_addr));



	*/
	static constexpr std::uintptr_t grabglobalstateindex_addr = 0x007B9920;
#define grabglobalstateindex_CCV __thiscall

	/*

	* growci_Decompiled_Args: int a1
	* growci_TypeDef: int
	* growci_Aob: Unable to generate aob.
	* growci_ProtectedByRetcheck: false
	* growci_IsApartOfLua: true
	* growci_Mask: ....
	* Usage (Not 100% Accurate):

	using r_growci_typedef = int(__cdecl*)(int a1);
	r_growci_typedef r_growci = (r_growci_typedef)(x(0x0165DAC0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_growci_typedef = int(growci_CCV*)(int a1);
	r_growci_typedef r_growci = (r_growci_typedef)(x(Adresses::growci_addr));



	*/
	static constexpr std::uintptr_t growci_addr = 0x0165DAC0;
#define growci_CCV __cdecl

	/*

	* hashnum_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* hashnum_TypeDef: int
	* hashnum_Aob: Unable to generate aob.
	* hashnum_ProtectedByRetcheck: false
	* hashnum_IsApartOfLua: true
	* hashnum_Mask: ....
	* Usage (Not 100% Accurate):

	using r_hashnum_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_hashnum_typedef r_hashnum = (r_hashnum_typedef)(x(0x01673280));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_hashnum_typedef = int(hashnum_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_hashnum_typedef r_hashnum = (r_hashnum_typedef)(x(Adresses::hashnum_addr));



	*/
	static constexpr std::uintptr_t hashnum_addr = 0x01673280;
#define hashnum_CCV __cdecl

	/*

	* index2adr_Decompiled_Args: std::uintptr_t a1, int a2
	* index2adr_TypeDef: int
	* index2adr_Aob: Unable to generate aob.
	* index2adr_ProtectedByRetcheck: false
	* index2adr_IsApartOfLua: true
	* index2adr_Mask: ....
	* Usage (Not 100% Accurate):

	using r_index2adr_typedef = int(__fastcall*)(std::uintptr_t a1, int a2);
	r_index2adr_typedef r_index2adr = (r_index2adr_typedef)(x(0x0165A3A0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_index2adr_typedef = int(index2adr_CCV*)(std::uintptr_t a1, int a2);
	r_index2adr_typedef r_index2adr = (r_index2adr_typedef)(x(Adresses::index2adr_addr));



	*/
	static constexpr std::uintptr_t index2adr_addr = 0x0165A3A0;
#define index2adr_CCV __fastcall

	/*

	* report_touch_info_Decompiled_Args: int a1, int a2, int a3
	* report_touch_info_TypeDef: int
	* report_touch_info_Aob: Unable to generate aob.
	* report_touch_info_ProtectedByRetcheck: false
	* report_touch_info_IsApartOfLua: false
	* report_touch_info_Mask: ....
	* Usage (Not 100% Accurate):

	using r_report_touch_info_typedef = int(__stdcall*)(int a1, int a2, int a3);
	r_report_touch_info_typedef r_report_touch_info = (r_report_touch_info_typedef)(x(0x009587A0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_report_touch_info_typedef = int(report_touch_info_CCV*)(int a1, int a2, int a3);
	r_report_touch_info_typedef r_report_touch_info = (r_report_touch_info_typedef)(x(Adresses::report_touch_info_addr));



	*/
	static constexpr std::uintptr_t report_touch_info_addr = 0x009587A0;
#define report_touch_info_CCV __stdcall

	/*

	* propagateall_Decompiled_Args: int a1
	* propagateall_TypeDef: int
	* propagateall_Aob: Unable to generate aob.
	* propagateall_ProtectedByRetcheck: false
	* propagateall_IsApartOfLua: true
	* propagateall_Mask: ....
	* Usage (Not 100% Accurate):

	using r_propagateall_typedef = int(__cdecl*)(int a1);
	r_propagateall_typedef r_propagateall = (r_propagateall_typedef)(x(0x01671F00));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_propagateall_typedef = int(propagateall_CCV*)(int a1);
	r_propagateall_typedef r_propagateall = (r_propagateall_typedef)(x(Adresses::propagateall_addr));



	*/
	static constexpr std::uintptr_t propagateall_addr = 0x01671F00;
#define propagateall_CCV __cdecl

	/*

	* propagatemark_Decompiled_Args: int a1
	* propagatemark_TypeDef: int
	* propagatemark_Aob: Unable to generate aob.
	* propagatemark_ProtectedByRetcheck: false
	* propagatemark_IsApartOfLua: true
	* propagatemark_Mask: ....
	* Usage (Not 100% Accurate):

	using r_propagatemark_typedef = int(__cdecl*)(int a1);
	r_propagatemark_typedef r_propagatemark = (r_propagatemark_typedef)(x(0x01672120));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_propagatemark_typedef = int(propagatemark_CCV*)(int a1);
	r_propagatemark_typedef r_propagatemark = (r_propagatemark_typedef)(x(Adresses::propagatemark_addr));



	*/
	static constexpr std::uintptr_t propagatemark_addr = 0x01672120;
#define propagatemark_CCV __cdecl

	/*

	* sweeplist_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* sweeplist_TypeDef: int
	* sweeplist_Aob: Unable to generate aob.
	* sweeplist_ProtectedByRetcheck: false
	* sweeplist_IsApartOfLua: true
	* sweeplist_Mask: ....
	* Usage (Not 100% Accurate):

	using r_sweeplist_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_sweeplist_typedef r_sweeplist = (r_sweeplist_typedef)(x(0x01672770));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_sweeplist_typedef = int(sweeplist_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_sweeplist_typedef r_sweeplist = (r_sweeplist_typedef)(x(Adresses::sweeplist_addr));



	*/
	static constexpr std::uintptr_t sweeplist_addr = 0x01672770;
#define sweeplist_CCV __cdecl

	/*

	* luaopen_base_Decompiled_Args: int a1
	* luaopen_base_TypeDef: int
	* luaopen_base_Aob: Unable to generate aob.
	* luaopen_base_ProtectedByRetcheck: false
	* luaopen_base_IsApartOfLua: true
	* luaopen_base_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luaopen_base_typedef = int(__cdecl*)(int a1);
	r_luaopen_base_typedef r_luaopen_base = (r_luaopen_base_typedef)(x(0x01669DC0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luaopen_base_typedef = int(luaopen_base_CCV*)(int a1);
	r_luaopen_base_typedef r_luaopen_base = (r_luaopen_base_typedef)(x(Adresses::luaopen_base_addr));



	*/
	static constexpr std::uintptr_t luaopen_base_addr = 0x01669DC0;
#define luaopen_base_CCV __cdecl

	/*

	* luaopen_table_Decompiled_Args: int a1
	* luaopen_table_TypeDef: int
	* luaopen_table_Aob: Unable to generate aob.
	* luaopen_table_ProtectedByRetcheck: false
	* luaopen_table_IsApartOfLua: true
	* luaopen_table_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luaopen_table_typedef = int(__cdecl*)(int a1);
	r_luaopen_table_typedef r_luaopen_table = (r_luaopen_table_typedef)(x(0x0166A4D0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luaopen_table_typedef = int(luaopen_table_CCV*)(int a1);
	r_luaopen_table_typedef r_luaopen_table = (r_luaopen_table_typedef)(x(Adresses::luaopen_table_addr));



	*/
	static constexpr std::uintptr_t luaopen_table_addr = 0x0166A4D0;
#define luaopen_table_CCV __cdecl

	/*

	* luaopen_bit32_Decompiled_Args: int a1
	* luaopen_bit32_TypeDef: int
	* luaopen_bit32_Aob: Unable to generate aob.
	* luaopen_bit32_ProtectedByRetcheck: false
	* luaopen_bit32_IsApartOfLua: true
	* luaopen_bit32_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luaopen_bit32_typedef = int(__cdecl*)(int a1);
	r_luaopen_bit32_typedef r_luaopen_bit32 = (r_luaopen_bit32_typedef)(x(0x0166ED90));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luaopen_bit32_typedef = int(luaopen_bit32_CCV*)(int a1);
	r_luaopen_bit32_typedef r_luaopen_bit32 = (r_luaopen_bit32_typedef)(x(Adresses::luaopen_bit32_addr));



	*/
	static constexpr std::uintptr_t luaopen_bit32_addr = 0x0166ED90;
#define luaopen_bit32_CCV __cdecl

	/*

	* lua_call_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_call_TypeDef: int
	* lua_call_Aob: Unable to generate aob.
	* lua_call_ProtectedByRetcheck: true
	* lua_call_IsApartOfLua: true
	* lua_call_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_call_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_call_typedef r_lua_call = (r_lua_call_typedef)unprotect(x(0x0165A4A0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_call_typedef = int(lua_call_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_call_typedef r_lua_call = (r_lua_call_typedef)unprotect(x(Adresses::lua_call_addr));



	*/
	static constexpr std::uintptr_t lua_call_addr = 0x0165A4A0;
#define lua_call_CCV __cdecl

	/*

	* lua_checkstack_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_checkstack_TypeDef: int
	* lua_checkstack_Aob: Unable to generate aob.
	* lua_checkstack_ProtectedByRetcheck: true
	* lua_checkstack_IsApartOfLua: true
	* lua_checkstack_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_checkstack_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_checkstack_typedef r_lua_checkstack = (r_lua_checkstack_typedef)unprotect(x(0x0165B840));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_checkstack_typedef = int(lua_checkstack_CCV*)(std::uintptr_t a1, int a2);
	r_lua_checkstack_typedef r_lua_checkstack = (r_lua_checkstack_typedef)unprotect(x(Adresses::lua_checkstack_addr));



	*/
	static constexpr std::uintptr_t lua_checkstack_addr = 0x0165B840;
#define lua_checkstack_CCV __cdecl

	/*

	* lua_close_Decompiled_Args: int a1
	* lua_close_TypeDef: int
	* lua_close_Aob: Unable to generate aob.
	* lua_close_ProtectedByRetcheck: false
	* lua_close_IsApartOfLua: true
	* lua_close_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_close_typedef = int(__cdecl*)(int a1);
	r_lua_close_typedef r_lua_close = (r_lua_close_typedef)(x(0x01668330));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_close_typedef = int(lua_close_CCV*)(int a1);
	r_lua_close_typedef r_lua_close = (r_lua_close_typedef)(x(Adresses::lua_close_addr));



	*/
	static constexpr std::uintptr_t lua_close_addr = 0x01668330;
#define lua_close_CCV __cdecl

	/*

	* lua_concat_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_concat_TypeDef: int
	* lua_concat_Aob: Unable to generate aob.
	* lua_concat_ProtectedByRetcheck: true
	* lua_concat_IsApartOfLua: true
	* lua_concat_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_concat_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_concat_typedef r_lua_concat = (r_lua_concat_typedef)unprotect(x(0x0165A5B0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_concat_typedef = int(lua_concat_CCV*)(std::uintptr_t a1, int a2);
	r_lua_concat_typedef r_lua_concat = (r_lua_concat_typedef)unprotect(x(Adresses::lua_concat_addr));



	*/
	static constexpr std::uintptr_t lua_concat_addr = 0x0165A5B0;
#define lua_concat_CCV __cdecl

	/*

	* lua_createtable_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_createtable_TypeDef: int
	* lua_createtable_Aob: Unable to generate aob.
	* lua_createtable_ProtectedByRetcheck: true
	* lua_createtable_IsApartOfLua: true
	* lua_createtable_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_createtable_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_createtable_typedef r_lua_createtable = (r_lua_createtable_typedef)unprotect(x(0x0165A660));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_createtable_typedef = int(lua_createtable_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_createtable_typedef r_lua_createtable = (r_lua_createtable_typedef)unprotect(x(Adresses::lua_createtable_addr));



	*/
	static constexpr std::uintptr_t lua_createtable_addr = 0x0165A660;
#define lua_createtable_CCV __cdecl

	/*

	* lua_error_Decompiled_Args: int a1
	* lua_error_TypeDef: int
	* lua_error_Aob: Unable to generate aob.
	* lua_error_ProtectedByRetcheck: false
	* lua_error_IsApartOfLua: true
	* lua_error_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_error_typedef = int(__thiscall*)(int a1);
	r_lua_error_typedef r_lua_error = (r_lua_error_typedef)(x(0x0165A720));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_error_typedef = int(lua_error_CCV*)(int a1);
	r_lua_error_typedef r_lua_error = (r_lua_error_typedef)(x(Adresses::lua_error_addr));



	*/
	static constexpr std::uintptr_t lua_error_addr = 0x0165A720;
#define lua_error_CCV __thiscall

	/*

	* lua_freeobj_Decompiled_Args: int a1
	* lua_freeobj_TypeDef: int
	* lua_freeobj_Aob: Unable to generate aob.
	* lua_freeobj_ProtectedByRetcheck: false
	* lua_freeobj_IsApartOfLua: true
	* lua_freeobj_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_freeobj_typedef = int(__cdecl*)(int a1);
	r_lua_freeobj_typedef r_lua_freeobj = (r_lua_freeobj_typedef)(x(0x016713E0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_freeobj_typedef = int(lua_freeobj_CCV*)(int a1);
	r_lua_freeobj_typedef r_lua_freeobj = (r_lua_freeobj_typedef)(x(Adresses::lua_freeobj_addr));



	*/
	static constexpr std::uintptr_t lua_freeobj_addr = 0x016713E0;
#define lua_freeobj_CCV __cdecl

	/*

	* lua_gc_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_gc_TypeDef: int
	* lua_gc_Aob: Unable to generate aob.
	* lua_gc_ProtectedByRetcheck: false
	* lua_gc_IsApartOfLua: true
	* lua_gc_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_gc_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_gc_typedef r_lua_gc = (r_lua_gc_typedef)(x(0x0165A730));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_gc_typedef = int(lua_gc_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_gc_typedef r_lua_gc = (r_lua_gc_typedef)(x(Adresses::lua_gc_addr));



	*/
	static constexpr std::uintptr_t lua_gc_addr = 0x0165A730;
#define lua_gc_CCV __cdecl

	/*

	* lua_getargument_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_getargument_TypeDef: int
	* lua_getargument_Aob: Unable to generate aob.
	* lua_getargument_ProtectedByRetcheck: true
	* lua_getargument_IsApartOfLua: true
	* lua_getargument_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_getargument_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_getargument_typedef r_lua_getargument = (r_lua_getargument_typedef)unprotect(x(0x01668E60));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_getargument_typedef = int(lua_getargument_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_getargument_typedef r_lua_getargument = (r_lua_getargument_typedef)unprotect(x(Adresses::lua_getargument_addr));



	*/
	static constexpr std::uintptr_t lua_getargument_addr = 0x01668E60;
#define lua_getargument_CCV __cdecl

	/*

	* lua_getfenv_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_getfenv_TypeDef: int
	* lua_getfenv_Aob: Unable to generate aob.
	* lua_getfenv_ProtectedByRetcheck: true
	* lua_getfenv_IsApartOfLua: true
	* lua_getfenv_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_getfenv_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_getfenv_typedef r_lua_getfenv = (r_lua_getfenv_typedef)unprotect(x(0x0165A8D0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_getfenv_typedef = int(lua_getfenv_CCV*)(std::uintptr_t a1, int a2);
	r_lua_getfenv_typedef r_lua_getfenv = (r_lua_getfenv_typedef)unprotect(x(Adresses::lua_getfenv_addr));



	*/
	static constexpr std::uintptr_t lua_getfenv_addr = 0x0165A8D0;
#define lua_getfenv_CCV __cdecl

	/*

	* lua_getfield_Decompiled_Args: std::uintptr_t a1, int a2, const char* a3
	* lua_getfield_TypeDef: int
	* lua_getfield_Aob: Unable to generate aob.
	* lua_getfield_ProtectedByRetcheck: true
	* lua_getfield_IsApartOfLua: true
	* lua_getfield_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_getfield_typedef = int(__stdcall*)(std::uintptr_t a1, int a2, const char* a3);
	r_lua_getfield_typedef r_lua_getfield = (r_lua_getfield_typedef)unprotect(x(0x0165A970));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_getfield_typedef = int(lua_getfield_CCV*)(std::uintptr_t a1, int a2, const char* a3);
	r_lua_getfield_typedef r_lua_getfield = (r_lua_getfield_typedef)unprotect(x(Adresses::lua_getfield_addr));



	*/
	static constexpr std::uintptr_t lua_getfield_addr = 0x0165A970;
#define lua_getfield_CCV __stdcall

	/*

	* lua_getinfo_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_getinfo_TypeDef: int
	* lua_getinfo_Aob: Unable to generate aob.
	* lua_getinfo_ProtectedByRetcheck: true
	* lua_getinfo_IsApartOfLua: true
	* lua_getinfo_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_getinfo_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_getinfo_typedef r_lua_getinfo = (r_lua_getinfo_typedef)unprotect(x(0x01668F30));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_getinfo_typedef = int(lua_getinfo_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_getinfo_typedef r_lua_getinfo = (r_lua_getinfo_typedef)unprotect(x(Adresses::lua_getinfo_addr));



	*/
	static constexpr std::uintptr_t lua_getinfo_addr = 0x01668F30;
#define lua_getinfo_CCV __cdecl

	/*

	* lua_getlocal_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_getlocal_TypeDef: int
	* lua_getlocal_Aob: Unable to generate aob.
	* lua_getlocal_ProtectedByRetcheck: true
	* lua_getlocal_IsApartOfLua: true
	* lua_getlocal_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_getlocal_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_getlocal_typedef r_lua_getlocal = (r_lua_getlocal_typedef)unprotect(x(0x01669010));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_getlocal_typedef = int(lua_getlocal_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_getlocal_typedef r_lua_getlocal = (r_lua_getlocal_typedef)unprotect(x(Adresses::lua_getlocal_addr));



	*/
	static constexpr std::uintptr_t lua_getlocal_addr = 0x01669010;
#define lua_getlocal_CCV __cdecl

	/*

	* lua_getmetatable_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_getmetatable_TypeDef: int
	* lua_getmetatable_Aob: Unable to generate aob.
	* lua_getmetatable_ProtectedByRetcheck: false
	* lua_getmetatable_IsApartOfLua: true
	* lua_getmetatable_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_getmetatable_typedef = int(__stdcall*)(std::uintptr_t a1, int a2);
	r_lua_getmetatable_typedef r_lua_getmetatable = (r_lua_getmetatable_typedef)(x(0x0165AA20));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_getmetatable_typedef = int(lua_getmetatable_CCV*)(std::uintptr_t a1, int a2);
	r_lua_getmetatable_typedef r_lua_getmetatable = (r_lua_getmetatable_typedef)(x(Adresses::lua_getmetatable_addr));



	*/
	static constexpr std::uintptr_t lua_getmetatable_addr = 0x0165AA20;
#define lua_getmetatable_CCV __stdcall

	/*

	* lua_gettable_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_gettable_TypeDef: int
	* lua_gettable_Aob: Unable to generate aob.
	* lua_gettable_ProtectedByRetcheck: true
	* lua_gettable_IsApartOfLua: true
	* lua_gettable_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_gettable_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_gettable_typedef r_lua_gettable = (r_lua_gettable_typedef)unprotect(x(0x0165AAE0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_gettable_typedef = int(lua_gettable_CCV*)(std::uintptr_t a1, int a2);
	r_lua_gettable_typedef r_lua_gettable = (r_lua_gettable_typedef)unprotect(x(Adresses::lua_gettable_addr));



	*/
	static constexpr std::uintptr_t lua_gettable_addr = 0x0165AAE0;
#define lua_gettable_CCV __cdecl

	/*

	* lua_gettop_Decompiled_Args: int a1
	* lua_gettop_TypeDef: int
	* lua_gettop_Aob: Unable to generate aob.
	* lua_gettop_ProtectedByRetcheck: false
	* lua_gettop_IsApartOfLua: true
	* lua_gettop_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_gettop_typedef = int(__cdecl*)(int a1);
	r_lua_gettop_typedef r_lua_gettop = (r_lua_gettop_typedef)(x(0x0165AB50));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_gettop_typedef = int(lua_gettop_CCV*)(int a1);
	r_lua_gettop_typedef r_lua_gettop = (r_lua_gettop_typedef)(x(Adresses::lua_gettop_addr));



	*/
	static constexpr std::uintptr_t lua_gettop_addr = 0x0165AB50;
#define lua_gettop_CCV __cdecl

	/*

	* lua_getupvalue_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_getupvalue_TypeDef: int
	* lua_getupvalue_Aob: Unable to generate aob.
	* lua_getupvalue_ProtectedByRetcheck: true
	* lua_getupvalue_IsApartOfLua: true
	* lua_getupvalue_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_getupvalue_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_getupvalue_typedef r_lua_getupvalue = (r_lua_getupvalue_typedef)unprotect(x(0x0165AB70));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_getupvalue_typedef = int(lua_getupvalue_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_getupvalue_typedef r_lua_getupvalue = (r_lua_getupvalue_typedef)unprotect(x(Adresses::lua_getupvalue_addr));



	*/
	static constexpr std::uintptr_t lua_getupvalue_addr = 0x0165AB70;
#define lua_getupvalue_CCV __cdecl

	/*

	* lua_insert_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_insert_TypeDef: int
	* lua_insert_Aob: Unable to generate aob.
	* lua_insert_ProtectedByRetcheck: true
	* lua_insert_IsApartOfLua: true
	* lua_insert_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_insert_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_insert_typedef r_lua_insert = (r_lua_insert_typedef)unprotect(x(0x0165AC00));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_insert_typedef = int(lua_insert_CCV*)(std::uintptr_t a1, int a2);
	r_lua_insert_typedef r_lua_insert = (r_lua_insert_typedef)unprotect(x(Adresses::lua_insert_addr));



	*/
	static constexpr std::uintptr_t lua_insert_addr = 0x0165AC00;
#define lua_insert_CCV __cdecl

	/*

	* lua_iscfunction_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_iscfunction_TypeDef: int
	* lua_iscfunction_Aob: Unable to generate aob.
	* lua_iscfunction_ProtectedByRetcheck: false
	* lua_iscfunction_IsApartOfLua: true
	* lua_iscfunction_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_iscfunction_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_iscfunction_typedef r_lua_iscfunction = (r_lua_iscfunction_typedef)(x(0x0165ACD0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_iscfunction_typedef = int(lua_iscfunction_CCV*)(std::uintptr_t a1, int a2);
	r_lua_iscfunction_typedef r_lua_iscfunction = (r_lua_iscfunction_typedef)(x(Adresses::lua_iscfunction_addr));



	*/
	static constexpr std::uintptr_t lua_iscfunction_addr = 0x0165ACD0;
#define lua_iscfunction_CCV __cdecl

	/*

	* lua_isnumber_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_isnumber_TypeDef: int
	* lua_isnumber_Aob: Unable to generate aob.
	* lua_isnumber_ProtectedByRetcheck: false
	* lua_isnumber_IsApartOfLua: true
	* lua_isnumber_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_isnumber_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_isnumber_typedef r_lua_isnumber = (r_lua_isnumber_typedef)(x(0x0165AD20));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_isnumber_typedef = int(lua_isnumber_CCV*)(std::uintptr_t a1, int a2);
	r_lua_isnumber_typedef r_lua_isnumber = (r_lua_isnumber_typedef)(x(Adresses::lua_isnumber_addr));



	*/
	static constexpr std::uintptr_t lua_isnumber_addr = 0x0165AD20;
#define lua_isnumber_CCV __cdecl

	/*

	* lua_isstring_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_isstring_TypeDef: int
	* lua_isstring_Aob: Unable to generate aob.
	* lua_isstring_ProtectedByRetcheck: false
	* lua_isstring_IsApartOfLua: true
	* lua_isstring_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_isstring_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_isstring_typedef r_lua_isstring = (r_lua_isstring_typedef)(x(0x0165AD80));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_isstring_typedef = int(lua_isstring_CCV*)(std::uintptr_t a1, int a2);
	r_lua_isstring_typedef r_lua_isstring = (r_lua_isstring_typedef)(x(Adresses::lua_isstring_addr));



	*/
	static constexpr std::uintptr_t lua_isstring_addr = 0x0165AD80;
#define lua_isstring_CCV __cdecl

	/*

	* lua_isuserdata_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_isuserdata_TypeDef: int
	* lua_isuserdata_Aob: Unable to generate aob.
	* lua_isuserdata_ProtectedByRetcheck: false
	* lua_isuserdata_IsApartOfLua: true
	* lua_isuserdata_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_isuserdata_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_isuserdata_typedef r_lua_isuserdata = (r_lua_isuserdata_typedef)(x(0x0165AC80));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_isuserdata_typedef = int(lua_isuserdata_CCV*)(std::uintptr_t a1, int a2);
	r_lua_isuserdata_typedef r_lua_isuserdata = (r_lua_isuserdata_typedef)(x(Adresses::lua_isuserdata_addr));



	*/
	static constexpr std::uintptr_t lua_isuserdata_addr = 0x0165AC80;
#define lua_isuserdata_CCV __cdecl

	/*

	* lua_lessthan_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_lessthan_TypeDef: int
	* lua_lessthan_Aob: Unable to generate aob.
	* lua_lessthan_ProtectedByRetcheck: true
	* lua_lessthan_IsApartOfLua: true
	* lua_lessthan_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_lessthan_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_lessthan_typedef r_lua_lessthan = (r_lua_lessthan_typedef)unprotect(x(0x0165ADD0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_lessthan_typedef = int(lua_lessthan_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_lessthan_typedef r_lua_lessthan = (r_lua_lessthan_typedef)unprotect(x(Adresses::lua_lessthan_addr));



	*/
	static constexpr std::uintptr_t lua_lessthan_addr = 0x0165ADD0;
#define lua_lessthan_CCV __cdecl

	/*

	* lua_newstate_Decompiled_Args: int a1
	* lua_newstate_TypeDef: int
	* lua_newstate_Aob: Unable to generate aob.
	* lua_newstate_ProtectedByRetcheck: false
	* lua_newstate_IsApartOfLua: true
	* lua_newstate_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_newstate_typedef = int(__cdecl*)(int a1);
	r_lua_newstate_typedef r_lua_newstate = (r_lua_newstate_typedef)(x(0x01668360));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_newstate_typedef = int(lua_newstate_CCV*)(int a1);
	r_lua_newstate_typedef r_lua_newstate = (r_lua_newstate_typedef)(x(Adresses::lua_newstate_addr));



	*/
	static constexpr std::uintptr_t lua_newstate_addr = 0x01668360;
#define lua_newstate_CCV __cdecl

	/*

	* lua_newthread_Decompiled_Args: int a1
	* lua_newthread_TypeDef: int
	* lua_newthread_Aob: Unable to generate aob.
	* lua_newthread_ProtectedByRetcheck: true
	* lua_newthread_IsApartOfLua: true
	* lua_newthread_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_newthread_typedef = int(__cdecl*)(int a1);
	r_lua_newthread_typedef r_lua_newthread = (r_lua_newthread_typedef)unprotect(x(0x0165AE80));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_newthread_typedef = int(lua_newthread_CCV*)(int a1);
	r_lua_newthread_typedef r_lua_newthread = (r_lua_newthread_typedef)unprotect(x(Adresses::lua_newthread_addr));



	*/
	static constexpr std::uintptr_t lua_newthread_addr = 0x0165AE80;
#define lua_newthread_CCV __cdecl

	/*

	* lua_newuserdata_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_newuserdata_TypeDef: int
	* lua_newuserdata_Aob: Unable to generate aob.
	* lua_newuserdata_ProtectedByRetcheck: true
	* lua_newuserdata_IsApartOfLua: true
	* lua_newuserdata_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_newuserdata_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_newuserdata_typedef r_lua_newuserdata = (r_lua_newuserdata_typedef)unprotect(x(0x0165AF10));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_newuserdata_typedef = int(lua_newuserdata_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_newuserdata_typedef r_lua_newuserdata = (r_lua_newuserdata_typedef)unprotect(x(Adresses::lua_newuserdata_addr));



	*/
	static constexpr std::uintptr_t lua_newuserdata_addr = 0x0165AF10;
#define lua_newuserdata_CCV __cdecl

	/*

	* lua_next_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_next_TypeDef: int
	* lua_next_Aob: Unable to generate aob.
	* lua_next_ProtectedByRetcheck: true
	* lua_next_IsApartOfLua: true
	* lua_next_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_next_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_next_typedef r_lua_next = (r_lua_next_typedef)unprotect(x(0x0165AF90));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_next_typedef = int(lua_next_CCV*)(std::uintptr_t a1, int a2);
	r_lua_next_typedef r_lua_next = (r_lua_next_typedef)unprotect(x(Adresses::lua_next_addr));



	*/
	static constexpr std::uintptr_t lua_next_addr = 0x0165AF90;
#define lua_next_CCV __cdecl

	/*

	* lua_objlen_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_objlen_TypeDef: int
	* lua_objlen_Aob: Unable to generate aob.
	* lua_objlen_ProtectedByRetcheck: false
	* lua_objlen_IsApartOfLua: true
	* lua_objlen_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_objlen_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_objlen_typedef r_lua_objlen = (r_lua_objlen_typedef)(x(0x0165B020));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_objlen_typedef = int(lua_objlen_CCV*)(std::uintptr_t a1, int a2);
	r_lua_objlen_typedef r_lua_objlen = (r_lua_objlen_typedef)(x(Adresses::lua_objlen_addr));



	*/
	static constexpr std::uintptr_t lua_objlen_addr = 0x0165B020;
#define lua_objlen_CCV __cdecl

	/*

	* lua_pcall_Decompiled_Args: std::uintptr_t a1, int a2, int a3, int a4
	* lua_pcall_TypeDef: int
	* lua_pcall_Aob: Unable to generate aob.
	* lua_pcall_ProtectedByRetcheck: true
	* lua_pcall_IsApartOfLua: true
	* lua_pcall_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_pcall_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_lua_pcall_typedef r_lua_pcall = (r_lua_pcall_typedef)unprotect(x(0x0165B100));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_pcall_typedef = int(lua_pcall_CCV*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_lua_pcall_typedef r_lua_pcall = (r_lua_pcall_typedef)unprotect(x(Adresses::lua_pcall_addr));



	*/
	static constexpr std::uintptr_t lua_pcall_addr = 0x0165B100;
#define lua_pcall_CCV __cdecl

	/*

	* lua_pushboolean_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_pushboolean_TypeDef: int
	* lua_pushboolean_Aob: Unable to generate aob.
	* lua_pushboolean_ProtectedByRetcheck: true
	* lua_pushboolean_IsApartOfLua: true
	* lua_pushboolean_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_pushboolean_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_pushboolean_typedef r_lua_pushboolean = (r_lua_pushboolean_typedef)unprotect(x(0x0165B1C0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_pushboolean_typedef = int(lua_pushboolean_CCV*)(std::uintptr_t a1, int a2);
	r_lua_pushboolean_typedef r_lua_pushboolean = (r_lua_pushboolean_typedef)unprotect(x(Adresses::lua_pushboolean_addr));



	*/
	static constexpr std::uintptr_t lua_pushboolean_addr = 0x0165B1C0;
#define lua_pushboolean_CCV __cdecl

	/*

	* lua_pushcclosure_Decompiled_Args: std::uintptr_t a1, int a2, int a3, int a4, int a5
	* lua_pushcclosure_TypeDef: int
	* lua_pushcclosure_Aob: Unable to generate aob.
	* lua_pushcclosure_ProtectedByRetcheck: true
	* lua_pushcclosure_IsApartOfLua: true
	* lua_pushcclosure_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_pushcclosure_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3, int a4, int a5);
	r_lua_pushcclosure_typedef r_lua_pushcclosure = (r_lua_pushcclosure_typedef)unprotect(x(0x0165B210));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_pushcclosure_typedef = int(lua_pushcclosure_CCV*)(std::uintptr_t a1, int a2, int a3, int a4, int a5);
	r_lua_pushcclosure_typedef r_lua_pushcclosure = (r_lua_pushcclosure_typedef)unprotect(x(Adresses::lua_pushcclosure_addr));



	*/
	static constexpr std::uintptr_t lua_pushcclosure_addr = 0x0165B210;
#define lua_pushcclosure_CCV __cdecl

	/*

	* lua_pushfstring_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_pushfstring_TypeDef: int
	* lua_pushfstring_Aob: Unable to generate aob.
	* lua_pushfstring_ProtectedByRetcheck: false
	* lua_pushfstring_IsApartOfLua: true
	* lua_pushfstring_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_pushfstring_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_pushfstring_typedef r_lua_pushfstring = (r_lua_pushfstring_typedef)(x(0x0165B2F0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_pushfstring_typedef = int(lua_pushfstring_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_pushfstring_typedef r_lua_pushfstring = (r_lua_pushfstring_typedef)(x(Adresses::lua_pushfstring_addr));



	*/
	static constexpr std::uintptr_t lua_pushfstring_addr = 0x0165B2F0;
#define lua_pushfstring_CCV __cdecl

	/*

	* lua_pushinteger_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_pushinteger_TypeDef: int
	* lua_pushinteger_Aob: Unable to generate aob.
	* lua_pushinteger_ProtectedByRetcheck: true
	* lua_pushinteger_IsApartOfLua: true
	* lua_pushinteger_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_pushinteger_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_pushinteger_typedef r_lua_pushinteger = (r_lua_pushinteger_typedef)unprotect(x(0x0165B360));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_pushinteger_typedef = int(lua_pushinteger_CCV*)(std::uintptr_t a1, int a2);
	r_lua_pushinteger_typedef r_lua_pushinteger = (r_lua_pushinteger_typedef)unprotect(x(Adresses::lua_pushinteger_addr));



	*/
	static constexpr std::uintptr_t lua_pushinteger_addr = 0x0165B360;
#define lua_pushinteger_CCV __cdecl

	/*

	* lua_pushlightuserdata_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_pushlightuserdata_TypeDef: int
	* lua_pushlightuserdata_Aob: Unable to generate aob.
	* lua_pushlightuserdata_ProtectedByRetcheck: true
	* lua_pushlightuserdata_IsApartOfLua: true
	* lua_pushlightuserdata_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_pushlightuserdata_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_pushlightuserdata_typedef r_lua_pushlightuserdata = (r_lua_pushlightuserdata_typedef)unprotect(x(0x0165B3C0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_pushlightuserdata_typedef = int(lua_pushlightuserdata_CCV*)(std::uintptr_t a1, int a2);
	r_lua_pushlightuserdata_typedef r_lua_pushlightuserdata = (r_lua_pushlightuserdata_typedef)unprotect(x(Adresses::lua_pushlightuserdata_addr));



	*/
	static constexpr std::uintptr_t lua_pushlightuserdata_addr = 0x0165B3C0;
#define lua_pushlightuserdata_CCV __cdecl

	/*

	* lua_pushlstring_Decompiled_Args: std::uintptr_t a1, const char* a2, int a3
	* lua_pushlstring_TypeDef: int
	* lua_pushlstring_Aob: Unable to generate aob.
	* lua_pushlstring_ProtectedByRetcheck: true
	* lua_pushlstring_IsApartOfLua: true
	* lua_pushlstring_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_pushlstring_typedef = int(__cdecl*)(std::uintptr_t a1, const char* a2, int a3);
	r_lua_pushlstring_typedef r_lua_pushlstring = (r_lua_pushlstring_typedef)unprotect(x(0x0165B410));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_pushlstring_typedef = int(lua_pushlstring_CCV*)(std::uintptr_t a1, const char* a2, int a3);
	r_lua_pushlstring_typedef r_lua_pushlstring = (r_lua_pushlstring_typedef)unprotect(x(Adresses::lua_pushlstring_addr));



	*/
	static constexpr std::uintptr_t lua_pushlstring_addr = 0x0165B410;
#define lua_pushlstring_CCV __cdecl

	/*

	* lua_pushnil_Decompiled_Args: int a1
	* lua_pushnil_TypeDef: int
	* lua_pushnil_Aob: Unable to generate aob.
	* lua_pushnil_ProtectedByRetcheck: true
	* lua_pushnil_IsApartOfLua: true
	* lua_pushnil_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_pushnil_typedef = int(__cdecl*)(int a1);
	r_lua_pushnil_typedef r_lua_pushnil = (r_lua_pushnil_typedef)unprotect(x(0x0165B490));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_pushnil_typedef = int(lua_pushnil_CCV*)(int a1);
	r_lua_pushnil_typedef r_lua_pushnil = (r_lua_pushnil_typedef)unprotect(x(Adresses::lua_pushnil_addr));



	*/
	static constexpr std::uintptr_t lua_pushnil_addr = 0x0165B490;
#define lua_pushnil_CCV __cdecl

	/*

	* lua_pushnumber_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_pushnumber_TypeDef: int
	* lua_pushnumber_Aob: Unable to generate aob.
	* lua_pushnumber_ProtectedByRetcheck: true
	* lua_pushnumber_IsApartOfLua: true
	* lua_pushnumber_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_pushnumber_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_pushnumber_typedef r_lua_pushnumber = (r_lua_pushnumber_typedef)unprotect(x(0x0165B4E0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_pushnumber_typedef = int(lua_pushnumber_CCV*)(std::uintptr_t a1, int a2);
	r_lua_pushnumber_typedef r_lua_pushnumber = (r_lua_pushnumber_typedef)unprotect(x(Adresses::lua_pushnumber_addr));



	*/
	static constexpr std::uintptr_t lua_pushnumber_addr = 0x0165B4E0;
#define lua_pushnumber_CCV __cdecl

	/*

	* lua_pushstring_Decompiled_Args: std::uintptr_t a1, const char* a2
	* lua_pushstring_TypeDef: int
	* lua_pushstring_Aob: Unable to generate aob.
	* lua_pushstring_ProtectedByRetcheck: true
	* lua_pushstring_IsApartOfLua: true
	* lua_pushstring_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_pushstring_typedef = int(__fastcall*)(std::uintptr_t a1, const char* a2);
	r_lua_pushstring_typedef r_lua_pushstring = (r_lua_pushstring_typedef)unprotect(x(0x0165B540));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_pushstring_typedef = int(lua_pushstring_CCV*)(std::uintptr_t a1, const char* a2);
	r_lua_pushstring_typedef r_lua_pushstring = (r_lua_pushstring_typedef)unprotect(x(Adresses::lua_pushstring_addr));



	*/
	static constexpr std::uintptr_t lua_pushstring_addr = 0x0165B540;
#define lua_pushstring_CCV __fastcall

	/*

	* lua_pushthread_Decompiled_Args: int a1
	* lua_pushthread_TypeDef: int
	* lua_pushthread_Aob: Unable to generate aob.
	* lua_pushthread_ProtectedByRetcheck: true
	* lua_pushthread_IsApartOfLua: true
	* lua_pushthread_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_pushthread_typedef = int(__cdecl*)(int a1);
	r_lua_pushthread_typedef r_lua_pushthread = (r_lua_pushthread_typedef)unprotect(x(0x0165B630));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_pushthread_typedef = int(lua_pushthread_CCV*)(int a1);
	r_lua_pushthread_typedef r_lua_pushthread = (r_lua_pushthread_typedef)unprotect(x(Adresses::lua_pushthread_addr));



	*/
	static constexpr std::uintptr_t lua_pushthread_addr = 0x0165B630;
#define lua_pushthread_CCV __cdecl

	/*

	* lua_pushvalue_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_pushvalue_TypeDef: int
	* lua_pushvalue_Aob: Unable to generate aob.
	* lua_pushvalue_ProtectedByRetcheck: true
	* lua_pushvalue_IsApartOfLua: true
	* lua_pushvalue_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_pushvalue_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_pushvalue_typedef r_lua_pushvalue = (r_lua_pushvalue_typedef)unprotect(x(0x0165B700));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_pushvalue_typedef = int(lua_pushvalue_CCV*)(std::uintptr_t a1, int a2);
	r_lua_pushvalue_typedef r_lua_pushvalue = (r_lua_pushvalue_typedef)unprotect(x(Adresses::lua_pushvalue_addr));



	*/
	static constexpr std::uintptr_t lua_pushvalue_addr = 0x0165B700;
#define lua_pushvalue_CCV __cdecl

	/*

	* lua_pushvfstring_Decompiled_Args: std::uintptr_t a1, const char* a2, char* a3
	* lua_pushvfstring_TypeDef: int
	* lua_pushvfstring_Aob: Unable to generate aob.
	* lua_pushvfstring_ProtectedByRetcheck: false
	* lua_pushvfstring_IsApartOfLua: true
	* lua_pushvfstring_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_pushvfstring_typedef = int(__cdecl*)(std::uintptr_t a1, const char* a2, char* a3);
	r_lua_pushvfstring_typedef r_lua_pushvfstring = (r_lua_pushvfstring_typedef)(x(0x0165B7D0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_pushvfstring_typedef = int(lua_pushvfstring_CCV*)(std::uintptr_t a1, const char* a2, char* a3);
	r_lua_pushvfstring_typedef r_lua_pushvfstring = (r_lua_pushvfstring_typedef)(x(Adresses::lua_pushvfstring_addr));



	*/
	static constexpr std::uintptr_t lua_pushvfstring_addr = 0x0165B7D0;
#define lua_pushvfstring_CCV __cdecl

	/*

	* lua_rawequal_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_rawequal_TypeDef: int
	* lua_rawequal_Aob: Unable to generate aob.
	* lua_rawequal_ProtectedByRetcheck: false
	* lua_rawequal_IsApartOfLua: true
	* lua_rawequal_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_rawequal_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_rawequal_typedef r_lua_rawequal = (r_lua_rawequal_typedef)(x(0x0165B8B0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_rawequal_typedef = int(lua_rawequal_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_rawequal_typedef r_lua_rawequal = (r_lua_rawequal_typedef)(x(Adresses::lua_rawequal_addr));



	*/
	static constexpr std::uintptr_t lua_rawequal_addr = 0x0165B8B0;
#define lua_rawequal_CCV __cdecl

	/*

	* lua_rawget_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_rawget_TypeDef: int
	* lua_rawget_Aob: Unable to generate aob.
	* lua_rawget_ProtectedByRetcheck: true
	* lua_rawget_IsApartOfLua: true
	* lua_rawget_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_rawget_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_rawget_typedef r_lua_rawget = (r_lua_rawget_typedef)unprotect(x(0x0165B930));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_rawget_typedef = int(lua_rawget_CCV*)(std::uintptr_t a1, int a2);
	r_lua_rawget_typedef r_lua_rawget = (r_lua_rawget_typedef)unprotect(x(Adresses::lua_rawget_addr));



	*/
	static constexpr std::uintptr_t lua_rawget_addr = 0x0165B930;
#define lua_rawget_CCV __cdecl

	/*

	* lua_rawgeti_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_rawgeti_TypeDef: int
	* lua_rawgeti_Aob: Unable to generate aob.
	* lua_rawgeti_ProtectedByRetcheck: true
	* lua_rawgeti_IsApartOfLua: true
	* lua_rawgeti_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_rawgeti_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_rawgeti_typedef r_lua_rawgeti = (r_lua_rawgeti_typedef)unprotect(x(0x0165BA60));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_rawgeti_typedef = int(lua_rawgeti_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_rawgeti_typedef r_lua_rawgeti = (r_lua_rawgeti_typedef)unprotect(x(Adresses::lua_rawgeti_addr));



	*/
	static constexpr std::uintptr_t lua_rawgeti_addr = 0x0165BA60;
#define lua_rawgeti_CCV __cdecl

	/*

	* lua_rawset_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_rawset_TypeDef: int
	* lua_rawset_Aob: Unable to generate aob.
	* lua_rawset_ProtectedByRetcheck: true
	* lua_rawset_IsApartOfLua: true
	* lua_rawset_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_rawset_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_rawset_typedef r_lua_rawset = (r_lua_rawset_typedef)unprotect(x(0x0165BAE0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_rawset_typedef = int(lua_rawset_CCV*)(std::uintptr_t a1, int a2);
	r_lua_rawset_typedef r_lua_rawset = (r_lua_rawset_typedef)unprotect(x(Adresses::lua_rawset_addr));



	*/
	static constexpr std::uintptr_t lua_rawset_addr = 0x0165BAE0;
#define lua_rawset_CCV __cdecl

	/*

	* lua_rawseti_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_rawseti_TypeDef: int
	* lua_rawseti_Aob: Unable to generate aob.
	* lua_rawseti_ProtectedByRetcheck: true
	* lua_rawseti_IsApartOfLua: true
	* lua_rawseti_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_rawseti_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_rawseti_typedef r_lua_rawseti = (r_lua_rawseti_typedef)unprotect(x(0x0165BBB0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_rawseti_typedef = int(lua_rawseti_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_rawseti_typedef r_lua_rawseti = (r_lua_rawseti_typedef)unprotect(x(Adresses::lua_rawseti_addr));



	*/
	static constexpr std::uintptr_t lua_rawseti_addr = 0x0165BBB0;
#define lua_rawseti_CCV __cdecl

	/*

	* lua_rawvalue_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_rawvalue_TypeDef: int
	* lua_rawvalue_Aob: Unable to generate aob.
	* lua_rawvalue_ProtectedByRetcheck: false
	* lua_rawvalue_IsApartOfLua: true
	* lua_rawvalue_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_rawvalue_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_rawvalue_typedef r_lua_rawvalue = (r_lua_rawvalue_typedef)(x(0x0165A450));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_rawvalue_typedef = int(lua_rawvalue_CCV*)(std::uintptr_t a1, int a2);
	r_lua_rawvalue_typedef r_lua_rawvalue = (r_lua_rawvalue_typedef)(x(Adresses::lua_rawvalue_addr));



	*/
	static constexpr std::uintptr_t lua_rawvalue_addr = 0x0165A450;
#define lua_rawvalue_CCV __cdecl

	/*

	* lua_remove_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_remove_TypeDef: int
	* lua_remove_Aob: Unable to generate aob.
	* lua_remove_ProtectedByRetcheck: true
	* lua_remove_IsApartOfLua: true
	* lua_remove_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_remove_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_remove_typedef r_lua_remove = (r_lua_remove_typedef)unprotect(x(0x0165BD80));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_remove_typedef = int(lua_remove_CCV*)(std::uintptr_t a1, int a2);
	r_lua_remove_typedef r_lua_remove = (r_lua_remove_typedef)unprotect(x(Adresses::lua_remove_addr));



	*/
	static constexpr std::uintptr_t lua_remove_addr = 0x0165BD80;
#define lua_remove_CCV __cdecl

	/*

	* lua_replace_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_replace_TypeDef: int
	* lua_replace_Aob: Unable to generate aob.
	* lua_replace_ProtectedByRetcheck: true
	* lua_replace_IsApartOfLua: true
	* lua_replace_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_replace_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_replace_typedef r_lua_replace = (r_lua_replace_typedef)unprotect(x(0x0165BE10));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_replace_typedef = int(lua_replace_CCV*)(std::uintptr_t a1, int a2);
	r_lua_replace_typedef r_lua_replace = (r_lua_replace_typedef)unprotect(x(Adresses::lua_replace_addr));



	*/
	static constexpr std::uintptr_t lua_replace_addr = 0x0165BE10;
#define lua_replace_CCV __cdecl

	/*

	* lua_resume_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_resume_TypeDef: int
	* lua_resume_Aob: Unable to generate aob.
	* lua_resume_ProtectedByRetcheck: true
	* lua_resume_IsApartOfLua: true
	* lua_resume_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_resume_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_resume_typedef r_lua_resume = (r_lua_resume_typedef)unprotect(x(0x0165DFA0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_resume_typedef = int(lua_resume_CCV*)(std::uintptr_t a1, int a2);
	r_lua_resume_typedef r_lua_resume = (r_lua_resume_typedef)unprotect(x(Adresses::lua_resume_addr));



	*/
	static constexpr std::uintptr_t lua_resume_addr = 0x0165DFA0;
#define lua_resume_CCV __cdecl

	/*

	* lua_setfenv_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_setfenv_TypeDef: int
	* lua_setfenv_Aob: Unable to generate aob.
	* lua_setfenv_ProtectedByRetcheck: false
	* lua_setfenv_IsApartOfLua: true
	* lua_setfenv_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_setfenv_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_setfenv_typedef r_lua_setfenv = (r_lua_setfenv_typedef)(x(0x0165BF10));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_setfenv_typedef = int(lua_setfenv_CCV*)(std::uintptr_t a1, int a2);
	r_lua_setfenv_typedef r_lua_setfenv = (r_lua_setfenv_typedef)(x(Adresses::lua_setfenv_addr));



	*/
	static constexpr std::uintptr_t lua_setfenv_addr = 0x0165BF10;
#define lua_setfenv_CCV __cdecl

	/*

	* lua_setfield_Decompiled_Args: std::uintptr_t a1, int a2, const char* a3
	* lua_setfield_TypeDef: int
	* lua_setfield_Aob: Unable to generate aob.
	* lua_setfield_ProtectedByRetcheck: true
	* lua_setfield_IsApartOfLua: true
	* lua_setfield_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_setfield_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, const char* a3);
	r_lua_setfield_typedef r_lua_setfield = (r_lua_setfield_typedef)unprotect(x(0x0165BFD0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_setfield_typedef = int(lua_setfield_CCV*)(std::uintptr_t a1, int a2, const char* a3);
	r_lua_setfield_typedef r_lua_setfield = (r_lua_setfield_typedef)unprotect(x(Adresses::lua_setfield_addr));



	*/
	static constexpr std::uintptr_t lua_setfield_addr = 0x0165BFD0;
#define lua_setfield_CCV __cdecl

	/*

	* lua_setlocal_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_setlocal_TypeDef: int
	* lua_setlocal_Aob: Unable to generate aob.
	* lua_setlocal_ProtectedByRetcheck: true
	* lua_setlocal_IsApartOfLua: true
	* lua_setlocal_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_setlocal_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_setlocal_typedef r_lua_setlocal = (r_lua_setlocal_typedef)unprotect(x(0x016690E0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_setlocal_typedef = int(lua_setlocal_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_setlocal_typedef r_lua_setlocal = (r_lua_setlocal_typedef)unprotect(x(Adresses::lua_setlocal_addr));



	*/
	static constexpr std::uintptr_t lua_setlocal_addr = 0x016690E0;
#define lua_setlocal_CCV __cdecl

	/*

	* lua_setmetatable_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_setmetatable_TypeDef: int
	* lua_setmetatable_Aob: Unable to generate aob.
	* lua_setmetatable_ProtectedByRetcheck: true
	* lua_setmetatable_IsApartOfLua: true
	* lua_setmetatable_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_setmetatable_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_setmetatable_typedef r_lua_setmetatable = (r_lua_setmetatable_typedef)unprotect(x(0x0165C080));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_setmetatable_typedef = int(lua_setmetatable_CCV*)(std::uintptr_t a1, int a2);
	r_lua_setmetatable_typedef r_lua_setmetatable = (r_lua_setmetatable_typedef)unprotect(x(Adresses::lua_setmetatable_addr));



	*/
	static constexpr std::uintptr_t lua_setmetatable_addr = 0x0165C080;
#define lua_setmetatable_CCV __cdecl

	/*

	* lua_setreadonly_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_setreadonly_TypeDef: int
	* lua_setreadonly_Aob: Unable to generate aob.
	* lua_setreadonly_ProtectedByRetcheck: true
	* lua_setreadonly_IsApartOfLua: true
	* lua_setreadonly_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_setreadonly_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_setreadonly_typedef r_lua_setreadonly = (r_lua_setreadonly_typedef)unprotect(x(0x0165C190));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_setreadonly_typedef = int(lua_setreadonly_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_setreadonly_typedef r_lua_setreadonly = (r_lua_setreadonly_typedef)unprotect(x(Adresses::lua_setreadonly_addr));



	*/
	static constexpr std::uintptr_t lua_setreadonly_addr = 0x0165C190;
#define lua_setreadonly_CCV __cdecl

	/*

	* lua_setsafeenv_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_setsafeenv_TypeDef: int
	* lua_setsafeenv_Aob: Unable to generate aob.
	* lua_setsafeenv_ProtectedByRetcheck: true
	* lua_setsafeenv_IsApartOfLua: true
	* lua_setsafeenv_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_setsafeenv_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_setsafeenv_typedef r_lua_setsafeenv = (r_lua_setsafeenv_typedef)unprotect(x(0x0165C200));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_setsafeenv_typedef = int(lua_setsafeenv_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_setsafeenv_typedef r_lua_setsafeenv = (r_lua_setsafeenv_typedef)unprotect(x(Adresses::lua_setsafeenv_addr));



	*/
	static constexpr std::uintptr_t lua_setsafeenv_addr = 0x0165C200;
#define lua_setsafeenv_CCV __cdecl

	/*

	* lua_settable_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_settable_TypeDef: int
	* lua_settable_Aob: Unable to generate aob.
	* lua_settable_ProtectedByRetcheck: true
	* lua_settable_IsApartOfLua: true
	* lua_settable_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_settable_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_settable_typedef r_lua_settable = (r_lua_settable_typedef)unprotect(x(0x0165C270));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_settable_typedef = int(lua_settable_CCV*)(std::uintptr_t a1, int a2);
	r_lua_settable_typedef r_lua_settable = (r_lua_settable_typedef)unprotect(x(Adresses::lua_settable_addr));



	*/
	static constexpr std::uintptr_t lua_settable_addr = 0x0165C270;
#define lua_settable_CCV __cdecl

	/*

	* lua_settop_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_settop_TypeDef: int
	* lua_settop_Aob: Unable to generate aob.
	* lua_settop_ProtectedByRetcheck: true
	* lua_settop_IsApartOfLua: true
	* lua_settop_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_settop_typedef = int(__stdcall*)(std::uintptr_t a1, int a2);
	r_lua_settop_typedef r_lua_settop = (r_lua_settop_typedef)unprotect(x(0x0165C2F0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_settop_typedef = int(lua_settop_CCV*)(std::uintptr_t a1, int a2);
	r_lua_settop_typedef r_lua_settop = (r_lua_settop_typedef)unprotect(x(Adresses::lua_settop_addr));



	*/
	static constexpr std::uintptr_t lua_settop_addr = 0x0165C2F0;
#define lua_settop_CCV __stdcall

	/*

	* lua_setupvalue_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_setupvalue_TypeDef: int
	* lua_setupvalue_Aob: Unable to generate aob.
	* lua_setupvalue_ProtectedByRetcheck: true
	* lua_setupvalue_IsApartOfLua: true
	* lua_setupvalue_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_setupvalue_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_setupvalue_typedef r_lua_setupvalue = (r_lua_setupvalue_typedef)unprotect(x(0x0165C370));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_setupvalue_typedef = int(lua_setupvalue_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_setupvalue_typedef r_lua_setupvalue = (r_lua_setupvalue_typedef)unprotect(x(Adresses::lua_setupvalue_addr));



	*/
	static constexpr std::uintptr_t lua_setupvalue_addr = 0x0165C370;
#define lua_setupvalue_CCV __cdecl

	/*

	* lua_toboolean_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_toboolean_TypeDef: int
	* lua_toboolean_Aob: Unable to generate aob.
	* lua_toboolean_ProtectedByRetcheck: false
	* lua_toboolean_IsApartOfLua: true
	* lua_toboolean_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_toboolean_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_toboolean_typedef r_lua_toboolean = (r_lua_toboolean_typedef)(x(0x0165C420));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_toboolean_typedef = int(lua_toboolean_CCV*)(std::uintptr_t a1, int a2);
	r_lua_toboolean_typedef r_lua_toboolean = (r_lua_toboolean_typedef)(x(Adresses::lua_toboolean_addr));



	*/
	static constexpr std::uintptr_t lua_toboolean_addr = 0x0165C420;
#define lua_toboolean_CCV __cdecl

	/*

	* lua_tointeger_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_tointeger_TypeDef: int
	* lua_tointeger_Aob: Unable to generate aob.
	* lua_tointeger_ProtectedByRetcheck: false
	* lua_tointeger_IsApartOfLua: true
	* lua_tointeger_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_tointeger_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_tointeger_typedef r_lua_tointeger = (r_lua_tointeger_typedef)(x(0x0165C470));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_tointeger_typedef = int(lua_tointeger_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_tointeger_typedef r_lua_tointeger = (r_lua_tointeger_typedef)(x(Adresses::lua_tointeger_addr));



	*/
	static constexpr std::uintptr_t lua_tointeger_addr = 0x0165C470;
#define lua_tointeger_CCV __cdecl

	/*

	* lua_tolstring_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_tolstring_TypeDef: int
	* lua_tolstring_Aob: Unable to generate aob.
	* lua_tolstring_ProtectedByRetcheck: true
	* lua_tolstring_IsApartOfLua: true
	* lua_tolstring_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_tolstring_typedef = int(__stdcall*)(std::uintptr_t a1, int a2, int a3);
	r_lua_tolstring_typedef r_lua_tolstring = (r_lua_tolstring_typedef)unprotect(x(0x0165C4F0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_tolstring_typedef = int(lua_tolstring_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_tolstring_typedef r_lua_tolstring = (r_lua_tolstring_typedef)unprotect(x(Adresses::lua_tolstring_addr));



	*/
	static constexpr std::uintptr_t lua_tolstring_addr = 0x0165C4F0;
#define lua_tolstring_CCV __stdcall

	/*

	* lua_tonumber_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_tonumber_TypeDef: int
	* lua_tonumber_Aob: Unable to generate aob.
	* lua_tonumber_ProtectedByRetcheck: false
	* lua_tonumber_IsApartOfLua: true
	* lua_tonumber_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_tonumber_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_tonumber_typedef r_lua_tonumber = (r_lua_tonumber_typedef)(x(0x0165C620));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_tonumber_typedef = int(lua_tonumber_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_tonumber_typedef r_lua_tonumber = (r_lua_tonumber_typedef)(x(Adresses::lua_tonumber_addr));



	*/
	static constexpr std::uintptr_t lua_tonumber_addr = 0x0165C620;
#define lua_tonumber_CCV __cdecl

	/*

	* lua_topointer_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_topointer_TypeDef: int
	* lua_topointer_Aob: Unable to generate aob.
	* lua_topointer_ProtectedByRetcheck: false
	* lua_topointer_IsApartOfLua: true
	* lua_topointer_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_topointer_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_topointer_typedef r_lua_topointer = (r_lua_topointer_typedef)(x(0x0165C6A0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_topointer_typedef = int(lua_topointer_CCV*)(std::uintptr_t a1, int a2);
	r_lua_topointer_typedef r_lua_topointer = (r_lua_topointer_typedef)(x(Adresses::lua_topointer_addr));



	*/
	static constexpr std::uintptr_t lua_topointer_addr = 0x0165C6A0;
#define lua_topointer_CCV __cdecl

	/*

	* lua_tostring_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_tostring_TypeDef: int
	* lua_tostring_Aob: Unable to generate aob.
	* lua_tostring_ProtectedByRetcheck: false
	* lua_tostring_IsApartOfLua: true
	* lua_tostring_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_tostring_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_tostring_typedef r_lua_tostring = (r_lua_tostring_typedef)(x(0x0165C760));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_tostring_typedef = int(lua_tostring_CCV*)(std::uintptr_t a1, int a2);
	r_lua_tostring_typedef r_lua_tostring = (r_lua_tostring_typedef)(x(Adresses::lua_tostring_addr));



	*/
	static constexpr std::uintptr_t lua_tostring_addr = 0x0165C760;
#define lua_tostring_CCV __cdecl

	/*

	* lua_tothread_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_tothread_TypeDef: int
	* lua_tothread_Aob: Unable to generate aob.
	* lua_tothread_ProtectedByRetcheck: false
	* lua_tothread_IsApartOfLua: true
	* lua_tothread_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_tothread_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_tothread_typedef r_lua_tothread = (r_lua_tothread_typedef)(x(0x0165C7E0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_tothread_typedef = int(lua_tothread_CCV*)(std::uintptr_t a1, int a2);
	r_lua_tothread_typedef r_lua_tothread = (r_lua_tothread_typedef)(x(Adresses::lua_tothread_addr));



	*/
	static constexpr std::uintptr_t lua_tothread_addr = 0x0165C7E0;
#define lua_tothread_CCV __cdecl

	/*

	* lua_tounsignedx_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_tounsignedx_TypeDef: int
	* lua_tounsignedx_Aob: Unable to generate aob.
	* lua_tounsignedx_ProtectedByRetcheck: false
	* lua_tounsignedx_IsApartOfLua: true
	* lua_tounsignedx_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_tounsignedx_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_tounsignedx_typedef r_lua_tounsignedx = (r_lua_tounsignedx_typedef)(x(0x0165C820));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_tounsignedx_typedef = int(lua_tounsignedx_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_tounsignedx_typedef r_lua_tounsignedx = (r_lua_tounsignedx_typedef)(x(Adresses::lua_tounsignedx_addr));



	*/
	static constexpr std::uintptr_t lua_tounsignedx_addr = 0x0165C820;
#define lua_tounsignedx_CCV __cdecl

	/*

	* lua_touserdata_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_touserdata_TypeDef: int
	* lua_touserdata_Aob: Unable to generate aob.
	* lua_touserdata_ProtectedByRetcheck: false
	* lua_touserdata_IsApartOfLua: true
	* lua_touserdata_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_touserdata_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_touserdata_typedef r_lua_touserdata = (r_lua_touserdata_typedef)(x(0x0165C8B0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_touserdata_typedef = int(lua_touserdata_CCV*)(std::uintptr_t a1, int a2);
	r_lua_touserdata_typedef r_lua_touserdata = (r_lua_touserdata_typedef)(x(Adresses::lua_touserdata_addr));



	*/
	static constexpr std::uintptr_t lua_touserdata_addr = 0x0165C8B0;
#define lua_touserdata_CCV __cdecl

	/*

	* lua_type_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_type_TypeDef: int
	* lua_type_Aob: Unable to generate aob.
	* lua_type_ProtectedByRetcheck: false
	* lua_type_IsApartOfLua: true
	* lua_type_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_type_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_type_typedef r_lua_type = (r_lua_type_typedef)(x(0x0165C9A0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_type_typedef = int(lua_type_CCV*)(std::uintptr_t a1, int a2);
	r_lua_type_typedef r_lua_type = (r_lua_type_typedef)(x(Adresses::lua_type_addr));



	*/
	static constexpr std::uintptr_t lua_type_addr = 0x0165C9A0;
#define lua_type_CCV __cdecl

	/*

	* lua_typename_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_typename_TypeDef: int
	* lua_typename_Aob: Unable to generate aob.
	* lua_typename_ProtectedByRetcheck: false
	* lua_typename_IsApartOfLua: true
	* lua_typename_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_typename_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_typename_typedef r_lua_typename = (r_lua_typename_typedef)(x(0x0165C9E0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_typename_typedef = int(lua_typename_CCV*)(std::uintptr_t a1, int a2);
	r_lua_typename_typedef r_lua_typename = (r_lua_typename_typedef)(x(Adresses::lua_typename_addr));



	*/
	static constexpr std::uintptr_t lua_typename_addr = 0x0165C9E0;
#define lua_typename_CCV __cdecl

	/*

	* lua_xmove_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lua_xmove_TypeDef: int
	* lua_xmove_Aob: Unable to generate aob.
	* lua_xmove_ProtectedByRetcheck: true
	* lua_xmove_IsApartOfLua: true
	* lua_xmove_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_xmove_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lua_xmove_typedef r_lua_xmove = (r_lua_xmove_typedef)unprotect(x(0x0165CAD0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_xmove_typedef = int(lua_xmove_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lua_xmove_typedef r_lua_xmove = (r_lua_xmove_typedef)unprotect(x(Adresses::lua_xmove_addr));



	*/
	static constexpr std::uintptr_t lua_xmove_addr = 0x0165CAD0;
#define lua_xmove_CCV __cdecl

	/*

	* lua_yeild_Decompiled_Args: std::uintptr_t a1, int a2
	* lua_yeild_TypeDef: int
	* lua_yeild_Aob: Unable to generate aob.
	* lua_yeild_ProtectedByRetcheck: true
	* lua_yeild_IsApartOfLua: true
	* lua_yeild_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lua_yeild_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lua_yeild_typedef r_lua_yeild = (r_lua_yeild_typedef)unprotect(x(0x0165E200));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lua_yeild_typedef = int(lua_yeild_CCV*)(std::uintptr_t a1, int a2);
	r_lua_yeild_typedef r_lua_yeild = (r_lua_yeild_typedef)unprotect(x(Adresses::lua_yeild_addr));



	*/
	static constexpr std::uintptr_t lua_yeild_addr = 0x0165E200;
#define lua_yeild_CCV __cdecl

	/*

	* luaa_pushobject_Decompiled_Args: std::uintptr_t a1, int a2
	* luaa_pushobject_TypeDef: int
	* luaa_pushobject_Aob: Unable to generate aob.
	* luaa_pushobject_ProtectedByRetcheck: false
	* luaa_pushobject_IsApartOfLua: true
	* luaa_pushobject_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luaa_pushobject_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luaa_pushobject_typedef r_luaa_pushobject = (r_luaa_pushobject_typedef)(x(0x0165A430));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luaa_pushobject_typedef = int(luaa_pushobject_CCV*)(std::uintptr_t a1, int a2);
	r_luaa_pushobject_typedef r_luaa_pushobject = (r_luaa_pushobject_typedef)(x(Adresses::luaa_pushobject_addr));



	*/
	static constexpr std::uintptr_t luaa_pushobject_addr = 0x0165A430;
#define luaa_pushobject_CCV __cdecl

	/*

	* luab_auxwrap_Decompiled_Args: int a1
	* luab_auxwrap_TypeDef: int
	* luab_auxwrap_Aob: Unable to generate aob.
	* luab_auxwrap_ProtectedByRetcheck: false
	* luab_auxwrap_IsApartOfLua: true
	* luab_auxwrap_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luab_auxwrap_typedef = int(__cdecl*)(int a1);
	r_luab_auxwrap_typedef r_luab_auxwrap = (r_luab_auxwrap_typedef)(x(0x0166A260));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luab_auxwrap_typedef = int(luab_auxwrap_CCV*)(int a1);
	r_luab_auxwrap_typedef r_luab_auxwrap = (r_luab_auxwrap_typedef)(x(Adresses::luab_auxwrap_addr));



	*/
	static constexpr std::uintptr_t luab_auxwrap_addr = 0x0166A260;
#define luab_auxwrap_CCV __cdecl

	/*

	* luac_barrierback_Decompiled_Args: std::uintptr_t a1, int a2
	* luac_barrierback_TypeDef: int
	* luac_barrierback_Aob: Unable to generate aob.
	* luac_barrierback_ProtectedByRetcheck: false
	* luac_barrierback_IsApartOfLua: true
	* luac_barrierback_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luac_barrierback_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luac_barrierback_typedef r_luac_barrierback = (r_luac_barrierback_typedef)(x(0x01671500));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luac_barrierback_typedef = int(luac_barrierback_CCV*)(std::uintptr_t a1, int a2);
	r_luac_barrierback_typedef r_luac_barrierback = (r_luac_barrierback_typedef)(x(Adresses::luac_barrierback_addr));



	*/
	static constexpr std::uintptr_t luac_barrierback_addr = 0x01671500;
#define luac_barrierback_CCV __cdecl

	/*

	* luac_barrierf_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luac_barrierf_TypeDef: int
	* luac_barrierf_Aob: Unable to generate aob.
	* luac_barrierf_ProtectedByRetcheck: false
	* luac_barrierf_IsApartOfLua: true
	* luac_barrierf_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luac_barrierf_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luac_barrierf_typedef r_luac_barrierf = (r_luac_barrierf_typedef)(x(0x01671520));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luac_barrierf_typedef = int(luac_barrierf_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luac_barrierf_typedef r_luac_barrierf = (r_luac_barrierf_typedef)(x(Adresses::luac_barrierf_addr));



	*/
	static constexpr std::uintptr_t luac_barrierf_addr = 0x01671520;
#define luac_barrierf_CCV __cdecl

	/*

	* luac_fullgc_Decompiled_Args: int a1
	* luac_fullgc_TypeDef: int
	* luac_fullgc_Aob: Unable to generate aob.
	* luac_fullgc_ProtectedByRetcheck: false
	* luac_fullgc_IsApartOfLua: true
	* luac_fullgc_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luac_fullgc_typedef = int(__cdecl*)(int a1);
	r_luac_fullgc_typedef r_luac_fullgc = (r_luac_fullgc_typedef)(x(0x016717B0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luac_fullgc_typedef = int(luac_fullgc_CCV*)(int a1);
	r_luac_fullgc_typedef r_luac_fullgc = (r_luac_fullgc_typedef)(x(Adresses::luac_fullgc_addr));



	*/
	static constexpr std::uintptr_t luac_fullgc_addr = 0x016717B0;
#define luac_fullgc_CCV __cdecl

	/*

	* luac_link_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luac_link_TypeDef: int
	* luac_link_Aob: Unable to generate aob.
	* luac_link_ProtectedByRetcheck: false
	* luac_link_IsApartOfLua: true
	* luac_link_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luac_link_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luac_link_typedef r_luac_link = (r_luac_link_typedef)(x(0x01671970));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luac_link_typedef = int(luac_link_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luac_link_typedef r_luac_link = (r_luac_link_typedef)(x(Adresses::luac_link_addr));



	*/
	static constexpr std::uintptr_t luac_link_addr = 0x01671970;
#define luac_link_CCV __cdecl

	/*

	* luac_linkupval_Decompiled_Args: std::uintptr_t a1, int a2
	* luac_linkupval_TypeDef: int
	* luac_linkupval_Aob: Unable to generate aob.
	* luac_linkupval_ProtectedByRetcheck: false
	* luac_linkupval_IsApartOfLua: true
	* luac_linkupval_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luac_linkupval_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luac_linkupval_typedef r_luac_linkupval = (r_luac_linkupval_typedef)(x(0x016719A0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luac_linkupval_typedef = int(luac_linkupval_CCV*)(std::uintptr_t a1, int a2);
	r_luac_linkupval_typedef r_luac_linkupval = (r_luac_linkupval_typedef)(x(Adresses::luac_linkupval_addr));



	*/
	static constexpr std::uintptr_t luac_linkupval_addr = 0x016719A0;
#define luac_linkupval_CCV __cdecl

	/*

	* luac_step_Decompiled_Args: int a1
	* luac_step_TypeDef: int
	* luac_step_Aob: Unable to generate aob.
	* luac_step_ProtectedByRetcheck: false
	* luac_step_IsApartOfLua: true
	* luac_step_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luac_step_typedef = int(__cdecl*)(int a1);
	r_luac_step_typedef r_luac_step = (r_luac_step_typedef)(x(0x01671A20));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luac_step_typedef = int(luac_step_CCV*)(int a1);
	r_luac_step_typedef r_luac_step = (r_luac_step_typedef)(x(Adresses::luac_step_addr));



	*/
	static constexpr std::uintptr_t luac_step_addr = 0x01671A20;
#define luac_step_CCV __cdecl

	/*

	* luad_call_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luad_call_TypeDef: int
	* luad_call_Aob: Unable to generate aob.
	* luad_call_ProtectedByRetcheck: false
	* luad_call_IsApartOfLua: true
	* luad_call_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luad_call_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luad_call_typedef r_luad_call = (r_luad_call_typedef)(x(0x0165DA20));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luad_call_typedef = int(luad_call_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luad_call_typedef r_luad_call = (r_luad_call_typedef)(x(Adresses::luad_call_addr));



	*/
	static constexpr std::uintptr_t luad_call_addr = 0x0165DA20;
#define luad_call_CCV __cdecl

	/*

	* luad_growstack_Decompiled_Args: std::uintptr_t a1, int a2
	* luad_growstack_TypeDef: int
	* luad_growstack_Aob: Unable to generate aob.
	* luad_growstack_ProtectedByRetcheck: false
	* luad_growstack_IsApartOfLua: true
	* luad_growstack_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luad_growstack_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luad_growstack_typedef r_luad_growstack = (r_luad_growstack_typedef)(x(0x0165DB20));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luad_growstack_typedef = int(luad_growstack_CCV*)(std::uintptr_t a1, int a2);
	r_luad_growstack_typedef r_luad_growstack = (r_luad_growstack_typedef)(x(Adresses::luad_growstack_addr));



	*/
	static constexpr std::uintptr_t luad_growstack_addr = 0x0165DB20;
#define luad_growstack_CCV __cdecl

	/*

	* luad_pcall_Decompiled_Args: std::uintptr_t a1, int a2, int a3, int a4, int a5
	* luad_pcall_TypeDef: int
	* luad_pcall_Aob: Unable to generate aob.
	* luad_pcall_ProtectedByRetcheck: false
	* luad_pcall_IsApartOfLua: true
	* luad_pcall_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luad_pcall_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3, int a4, int a5);
	r_luad_pcall_typedef r_luad_pcall = (r_luad_pcall_typedef)(x(0x0165DB50));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luad_pcall_typedef = int(luad_pcall_CCV*)(std::uintptr_t a1, int a2, int a3, int a4, int a5);
	r_luad_pcall_typedef r_luad_pcall = (r_luad_pcall_typedef)(x(Adresses::luad_pcall_addr));



	*/
	static constexpr std::uintptr_t luad_pcall_addr = 0x0165DB50;
#define luad_pcall_CCV __cdecl

	/*

	* luad_poscall_Decompiled_Args: std::uintptr_t a1, int a2
	* luad_poscall_TypeDef: int
	* luad_poscall_Aob: Unable to generate aob.
	* luad_poscall_ProtectedByRetcheck: false
	* luad_poscall_IsApartOfLua: true
	* luad_poscall_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luad_poscall_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luad_poscall_typedef r_luad_poscall = (r_luad_poscall_typedef)(x(0x01667770));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luad_poscall_typedef = int(luad_poscall_CCV*)(std::uintptr_t a1, int a2);
	r_luad_poscall_typedef r_luad_poscall = (r_luad_poscall_typedef)(x(Adresses::luad_poscall_addr));



	*/
	static constexpr std::uintptr_t luad_poscall_addr = 0x01667770;
#define luad_poscall_CCV __cdecl

	/*

	* luad_precall_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luad_precall_TypeDef: int
	* luad_precall_Aob: Unable to generate aob.
	* luad_precall_ProtectedByRetcheck: false
	* luad_precall_IsApartOfLua: true
	* luad_precall_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luad_precall_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luad_precall_typedef r_luad_precall = (r_luad_precall_typedef)(x(0x01667800));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luad_precall_typedef = int(luad_precall_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luad_precall_typedef r_luad_precall = (r_luad_precall_typedef)(x(Adresses::luad_precall_addr));



	*/
	static constexpr std::uintptr_t luad_precall_addr = 0x01667800;
#define luad_precall_CCV __cdecl

	/*

	* luad_rawrunprotected_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luad_rawrunprotected_TypeDef: int
	* luad_rawrunprotected_Aob: Unable to generate aob.
	* luad_rawrunprotected_ProtectedByRetcheck: false
	* luad_rawrunprotected_IsApartOfLua: true
	* luad_rawrunprotected_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luad_rawrunprotected_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luad_rawrunprotected_typedef r_luad_rawrunprotected = (r_luad_rawrunprotected_typedef)(x(0x0165DC30));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luad_rawrunprotected_typedef = int(luad_rawrunprotected_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luad_rawrunprotected_typedef r_luad_rawrunprotected = (r_luad_rawrunprotected_typedef)(x(Adresses::luad_rawrunprotected_addr));



	*/
	static constexpr std::uintptr_t luad_rawrunprotected_addr = 0x0165DC30;
#define luad_rawrunprotected_CCV __cdecl

	/*

	* luad_reallocci_Decompiled_Args: std::uintptr_t a1, int a2
	* luad_reallocci_TypeDef: int
	* luad_reallocci_Aob: Unable to generate aob.
	* luad_reallocci_ProtectedByRetcheck: false
	* luad_reallocci_IsApartOfLua: true
	* luad_reallocci_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luad_reallocci_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luad_reallocci_typedef r_luad_reallocci = (r_luad_reallocci_typedef)(x(0x0165DCF0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luad_reallocci_typedef = int(luad_reallocci_CCV*)(std::uintptr_t a1, int a2);
	r_luad_reallocci_typedef r_luad_reallocci = (r_luad_reallocci_typedef)(x(Adresses::luad_reallocci_addr));



	*/
	static constexpr std::uintptr_t luad_reallocci_addr = 0x0165DCF0;
#define luad_reallocci_CCV __cdecl

	/*

	* luad_reallocstack_Decompiled_Args: std::uintptr_t a1, int a2
	* luad_reallocstack_TypeDef: int
	* luad_reallocstack_Aob: Unable to generate aob.
	* luad_reallocstack_ProtectedByRetcheck: false
	* luad_reallocstack_IsApartOfLua: true
	* luad_reallocstack_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luad_reallocstack_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luad_reallocstack_typedef r_luad_reallocstack = (r_luad_reallocstack_typedef)(x(0x0165DD70));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luad_reallocstack_typedef = int(luad_reallocstack_CCV*)(std::uintptr_t a1, int a2);
	r_luad_reallocstack_typedef r_luad_reallocstack = (r_luad_reallocstack_typedef)(x(Adresses::luad_reallocstack_addr));



	*/
	static constexpr std::uintptr_t luad_reallocstack_addr = 0x0165DD70;
#define luad_reallocstack_CCV __cdecl

	/*

	* luad_seterrorObj_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luad_seterrorObj_TypeDef: int
	* luad_seterrorObj_Aob: Unable to generate aob.
	* luad_seterrorObj_ProtectedByRetcheck: false
	* luad_seterrorObj_IsApartOfLua: true
	* luad_seterrorObj_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luad_seterrorObj_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luad_seterrorObj_typedef r_luad_seterrorObj = (r_luad_seterrorObj_typedef)(x(0x0165DE80));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luad_seterrorObj_typedef = int(luad_seterrorObj_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luad_seterrorObj_typedef r_luad_seterrorObj = (r_luad_seterrorObj_typedef)(x(Adresses::luad_seterrorObj_addr));



	*/
	static constexpr std::uintptr_t luad_seterrorObj_addr = 0x0165DE80;
#define luad_seterrorObj_CCV __cdecl

	/*

	* luad_throw_Decompiled_Args: std::uintptr_t a1, int a2
	* luad_throw_TypeDef: int
	* luad_throw_Aob: Unable to generate aob.
	* luad_throw_ProtectedByRetcheck: false
	* luad_throw_IsApartOfLua: true
	* luad_throw_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luad_throw_typedef = int(__fastcall*)(std::uintptr_t a1, int a2);
	r_luad_throw_typedef r_luad_throw = (r_luad_throw_typedef)(x(0x0165DEF0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luad_throw_typedef = int(luad_throw_CCV*)(std::uintptr_t a1, int a2);
	r_luad_throw_typedef r_luad_throw = (r_luad_throw_typedef)(x(Adresses::luad_throw_addr));



	*/
	static constexpr std::uintptr_t luad_throw_addr = 0x0165DEF0;
#define luad_throw_CCV __fastcall

	/*

	* luae_freethread_Decompiled_Args: std::uintptr_t a1, int a2
	* luae_freethread_TypeDef: int
	* luae_freethread_Aob: Unable to generate aob.
	* luae_freethread_ProtectedByRetcheck: false
	* luae_freethread_IsApartOfLua: true
	* luae_freethread_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luae_freethread_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luae_freethread_typedef r_luae_freethread = (r_luae_freethread_typedef)(x(0x01668220));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luae_freethread_typedef = int(luae_freethread_CCV*)(std::uintptr_t a1, int a2);
	r_luae_freethread_typedef r_luae_freethread = (r_luae_freethread_typedef)(x(Adresses::luae_freethread_addr));



	*/
	static constexpr std::uintptr_t luae_freethread_addr = 0x01668220;
#define luae_freethread_CCV __cdecl

	/*

	* luae_newthread_Decompiled_Args: int a1
	* luae_newthread_TypeDef: int
	* luae_newthread_Aob: Unable to generate aob.
	* luae_newthread_ProtectedByRetcheck: false
	* luae_newthread_IsApartOfLua: true
	* luae_newthread_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luae_newthread_typedef = int(__cdecl*)(int a1);
	r_luae_newthread_typedef r_luae_newthread = (r_luae_newthread_typedef)(x(0x016682A0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luae_newthread_typedef = int(luae_newthread_CCV*)(int a1);
	r_luae_newthread_typedef r_luae_newthread = (r_luae_newthread_typedef)(x(Adresses::luae_newthread_addr));



	*/
	static constexpr std::uintptr_t luae_newthread_addr = 0x016682A0;
#define luae_newthread_CCV __cdecl

	/*

	* luaf_close_Decompiled_Args: std::uintptr_t a1, int a2
	* luaf_close_TypeDef: int
	* luaf_close_Aob: Unable to generate aob.
	* luaf_close_ProtectedByRetcheck: false
	* luaf_close_IsApartOfLua: true
	* luaf_close_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luaf_close_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luaf_close_typedef r_luaf_close = (r_luaf_close_typedef)(x(0x016746C0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luaf_close_typedef = int(luaf_close_CCV*)(std::uintptr_t a1, int a2);
	r_luaf_close_typedef r_luaf_close = (r_luaf_close_typedef)(x(Adresses::luaf_close_addr));



	*/
	static constexpr std::uintptr_t luaf_close_addr = 0x016746C0;
#define luaf_close_CCV __cdecl

	/*

	* luaf_findupval_Decompiled_Args: std::uintptr_t a1, int a2
	* luaf_findupval_TypeDef: int
	* luaf_findupval_Aob: Unable to generate aob.
	* luaf_findupval_ProtectedByRetcheck: false
	* luaf_findupval_IsApartOfLua: true
	* luaf_findupval_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luaf_findupval_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luaf_findupval_typedef r_luaf_findupval = (r_luaf_findupval_typedef)(x(0x01672DA0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luaf_findupval_typedef = int(luaf_findupval_CCV*)(std::uintptr_t a1, int a2);
	r_luaf_findupval_typedef r_luaf_findupval = (r_luaf_findupval_typedef)(x(Adresses::luaf_findupval_addr));



	*/
	static constexpr std::uintptr_t luaf_findupval_addr = 0x01672DA0;
#define luaf_findupval_CCV __cdecl

	/*

	* luaf_freeproto_Decompiled_Args: std::uintptr_t a1, int a2
	* luaf_freeproto_TypeDef: int
	* luaf_freeproto_Aob: Unable to generate aob.
	* luaf_freeproto_ProtectedByRetcheck: false
	* luaf_freeproto_IsApartOfLua: true
	* luaf_freeproto_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luaf_freeproto_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luaf_freeproto_typedef r_luaf_freeproto = (r_luaf_freeproto_typedef)(x(0x01674840));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luaf_freeproto_typedef = int(luaf_freeproto_CCV*)(std::uintptr_t a1, int a2);
	r_luaf_freeproto_typedef r_luaf_freeproto = (r_luaf_freeproto_typedef)(x(Adresses::luaf_freeproto_addr));



	*/
	static constexpr std::uintptr_t luaf_freeproto_addr = 0x01674840;
#define luaf_freeproto_CCV __cdecl

	/*

	* luaf_newcclosure_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luaf_newcclosure_TypeDef: int
	* luaf_newcclosure_Aob: Unable to generate aob.
	* luaf_newcclosure_ProtectedByRetcheck: false
	* luaf_newcclosure_IsApartOfLua: true
	* luaf_newcclosure_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luaf_newcclosure_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luaf_newcclosure_typedef r_luaf_newcclosure = (r_luaf_newcclosure_typedef)(x(0x016749E0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luaf_newcclosure_typedef = int(luaf_newcclosure_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luaf_newcclosure_typedef r_luaf_newcclosure = (r_luaf_newcclosure_typedef)(x(Adresses::luaf_newcclosure_addr));



	*/
	static constexpr std::uintptr_t luaf_newcclosure_addr = 0x016749E0;
#define luaf_newcclosure_CCV __cdecl

	/*

	* luaf_newlclosure_Decompiled_Args: std::uintptr_t a1, int a2, int a3, int a4
	* luaf_newlclosure_TypeDef: int
	* luaf_newlclosure_Aob: Unable to generate aob.
	* luaf_newlclosure_ProtectedByRetcheck: false
	* luaf_newlclosure_IsApartOfLua: true
	* luaf_newlclosure_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luaf_newlclosure_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_luaf_newlclosure_typedef r_luaf_newlclosure = (r_luaf_newlclosure_typedef)(x(0x01674A40));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luaf_newlclosure_typedef = int(luaf_newlclosure_CCV*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_luaf_newlclosure_typedef r_luaf_newlclosure = (r_luaf_newlclosure_typedef)(x(Adresses::luaf_newlclosure_addr));



	*/
	static constexpr std::uintptr_t luaf_newlclosure_addr = 0x01674A40;
#define luaf_newlclosure_CCV __cdecl

	/*

	* luaf_newproto_Decompiled_Args: int a1
	* luaf_newproto_TypeDef: int
	* luaf_newproto_Aob: Unable to generate aob.
	* luaf_newproto_ProtectedByRetcheck: false
	* luaf_newproto_IsApartOfLua: true
	* luaf_newproto_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luaf_newproto_typedef = int(__cdecl*)(int a1);
	r_luaf_newproto_typedef r_luaf_newproto = (r_luaf_newproto_typedef)(x(0x01674AB0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luaf_newproto_typedef = int(luaf_newproto_CCV*)(int a1);
	r_luaf_newproto_typedef r_luaf_newproto = (r_luaf_newproto_typedef)(x(Adresses::luaf_newproto_addr));



	*/
	static constexpr std::uintptr_t luaf_newproto_addr = 0x01674AB0;
#define luaf_newproto_CCV __cdecl

	/*

	* luag_aritherror_Decompiled_Args: std::uintptr_t a1, int a2, int a3, int a4
	* luag_aritherror_TypeDef: int
	* luag_aritherror_Aob: Unable to generate aob.
	* luag_aritherror_ProtectedByRetcheck: false
	* luag_aritherror_IsApartOfLua: true
	* luag_aritherror_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luag_aritherror_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_luag_aritherror_typedef r_luag_aritherror = (r_luag_aritherror_typedef)(x(0x01668A90));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luag_aritherror_typedef = int(luag_aritherror_CCV*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_luag_aritherror_typedef r_luag_aritherror = (r_luag_aritherror_typedef)(x(Adresses::luag_aritherror_addr));



	*/
	static constexpr std::uintptr_t luag_aritherror_addr = 0x01668A90;
#define luag_aritherror_CCV __cdecl

	/*

	* luag_concaterror_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luag_concaterror_TypeDef: int
	* luag_concaterror_Aob: Unable to generate aob.
	* luag_concaterror_ProtectedByRetcheck: false
	* luag_concaterror_IsApartOfLua: true
	* luag_concaterror_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luag_concaterror_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luag_concaterror_typedef r_luag_concaterror = (r_luag_concaterror_typedef)(x(0x01668C20));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luag_concaterror_typedef = int(luag_concaterror_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luag_concaterror_typedef r_luag_concaterror = (r_luag_concaterror_typedef)(x(Adresses::luag_concaterror_addr));



	*/
	static constexpr std::uintptr_t luag_concaterror_addr = 0x01668C20;
#define luag_concaterror_CCV __cdecl

	/*

	* luag_forerrorl_Decompiled_Args: std::uintptr_t a1, int a2, const char* a3
	* luag_forerrorl_TypeDef: int
	* luag_forerrorl_Aob: Unable to generate aob.
	* luag_forerrorl_ProtectedByRetcheck: false
	* luag_forerrorl_IsApartOfLua: true
	* luag_forerrorl_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luag_forerrorl_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, const char* a3);
	r_luag_forerrorl_typedef r_luag_forerrorl = (r_luag_forerrorl_typedef)(x(0x01668C60));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luag_forerrorl_typedef = int(luag_forerrorl_CCV*)(std::uintptr_t a1, int a2, const char* a3);
	r_luag_forerrorl_typedef r_luag_forerrorl = (r_luag_forerrorl_typedef)(x(Adresses::luag_forerrorl_addr));



	*/
	static constexpr std::uintptr_t luag_forerrorl_addr = 0x01668C60;
#define luag_forerrorl_CCV __cdecl

	/*

	* luag_getline_Decompiled_Args: std::uintptr_t a1, int a2
	* luag_getline_TypeDef: int
	* luag_getline_Aob: Unable to generate aob.
	* luag_getline_ProtectedByRetcheck: false
	* luag_getline_IsApartOfLua: true
	* luag_getline_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luag_getline_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luag_getline_typedef r_luag_getline = (r_luag_getline_typedef)(x(0x01668C90));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luag_getline_typedef = int(luag_getline_CCV*)(std::uintptr_t a1, int a2);
	r_luag_getline_typedef r_luag_getline = (r_luag_getline_typedef)(x(Adresses::luag_getline_addr));



	*/
	static constexpr std::uintptr_t luag_getline_addr = 0x01668C90;
#define luag_getline_CCV __cdecl

	/*

	* luag_indexerror_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luag_indexerror_TypeDef: int
	* luag_indexerror_Aob: Unable to generate aob.
	* luag_indexerror_ProtectedByRetcheck: false
	* luag_indexerror_IsApartOfLua: true
	* luag_indexerror_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luag_indexerror_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luag_indexerror_typedef r_luag_indexerror = (r_luag_indexerror_typedef)(x(0x01668CD0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luag_indexerror_typedef = int(luag_indexerror_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luag_indexerror_typedef r_luag_indexerror = (r_luag_indexerror_typedef)(x(Adresses::luag_indexerror_addr));



	*/
	static constexpr std::uintptr_t luag_indexerror_addr = 0x01668CD0;
#define luag_indexerror_CCV __cdecl

	/*

	* luag_ordererror_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luag_ordererror_TypeDef: int
	* luag_ordererror_Aob: Unable to generate aob.
	* luag_ordererror_ProtectedByRetcheck: false
	* luag_ordererror_IsApartOfLua: true
	* luag_ordererror_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luag_ordererror_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luag_ordererror_typedef r_luag_ordererror = (r_luag_ordererror_typedef)(x(0x01668D80));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luag_ordererror_typedef = int(luag_ordererror_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luag_ordererror_typedef r_luag_ordererror = (r_luag_ordererror_typedef)(x(Adresses::luag_ordererror_addr));



	*/
	static constexpr std::uintptr_t luag_ordererror_addr = 0x01668D80;
#define luag_ordererror_CCV __cdecl

	/*

	* luag_runerror_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luag_runerror_TypeDef: int
	* luag_runerror_Aob: Unable to generate aob.
	* luag_runerror_ProtectedByRetcheck: false
	* luag_runerror_IsApartOfLua: true
	* luag_runerror_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luag_runerror_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luag_runerror_typedef r_luag_runerror = (r_luag_runerror_typedef)(x(0x01668DF0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luag_runerror_typedef = int(luag_runerror_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luag_runerror_typedef r_luag_runerror = (r_luag_runerror_typedef)(x(Adresses::luag_runerror_addr));



	*/
	static constexpr std::uintptr_t luag_runerror_addr = 0x01668DF0;
#define luag_runerror_CCV __cdecl

	/*

	* luag_typeerror_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luag_typeerror_TypeDef: int
	* luag_typeerror_Aob: Unable to generate aob.
	* luag_typeerror_ProtectedByRetcheck: false
	* luag_typeerror_IsApartOfLua: true
	* luag_typeerror_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luag_typeerror_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luag_typeerror_typedef r_luag_typeerror = (r_luag_typeerror_typedef)(x(0x01675D40));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luag_typeerror_typedef = int(luag_typeerror_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luag_typeerror_typedef r_luag_typeerror = (r_luag_typeerror_typedef)(x(Adresses::luag_typeerror_addr));



	*/
	static constexpr std::uintptr_t luag_typeerror_addr = 0x01675D40;
#define luag_typeerror_CCV __cdecl

	/*

	* luah_clone_Decompiled_Args: std::uintptr_t a1, int a2
	* luah_clone_TypeDef: int
	* luah_clone_Aob: Unable to generate aob.
	* luah_clone_ProtectedByRetcheck: false
	* luah_clone_IsApartOfLua: true
	* luah_clone_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luah_clone_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luah_clone_typedef r_luah_clone = (r_luah_clone_typedef)(x(0x01673370));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luah_clone_typedef = int(luah_clone_CCV*)(std::uintptr_t a1, int a2);
	r_luah_clone_typedef r_luah_clone = (r_luah_clone_typedef)(x(Adresses::luah_clone_addr));



	*/
	static constexpr std::uintptr_t luah_clone_addr = 0x01673370;
#define luah_clone_CCV __cdecl

	/*

	* luah_free_Decompiled_Args: std::uintptr_t a1, int a2
	* luah_free_TypeDef: int
	* luah_free_Aob: Unable to generate aob.
	* luah_free_ProtectedByRetcheck: false
	* luah_free_IsApartOfLua: true
	* luah_free_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luah_free_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luah_free_typedef r_luah_free = (r_luah_free_typedef)(x(0x01672E00));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luah_free_typedef = int(luah_free_CCV*)(std::uintptr_t a1, int a2);
	r_luah_free_typedef r_luah_free = (r_luah_free_typedef)(x(Adresses::luah_free_addr));



	*/
	static constexpr std::uintptr_t luah_free_addr = 0x01672E00;
#define luah_free_CCV __cdecl

	/*

	* luah_get_Decompiled_Args: std::uintptr_t a1, int a2
	* luah_get_TypeDef: int
	* luah_get_Aob: Unable to generate aob.
	* luah_get_ProtectedByRetcheck: false
	* luah_get_IsApartOfLua: true
	* luah_get_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luah_get_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luah_get_typedef r_luah_get = (r_luah_get_typedef)(x(0x01673500));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luah_get_typedef = int(luah_get_CCV*)(std::uintptr_t a1, int a2);
	r_luah_get_typedef r_luah_get = (r_luah_get_typedef)(x(Adresses::luah_get_addr));



	*/
	static constexpr std::uintptr_t luah_get_addr = 0x01673500;
#define luah_get_CCV __cdecl

	/*

	* luah_getn_Decompiled_Args: int a1
	* luah_getn_TypeDef: int
	* luah_getn_Aob: Unable to generate aob.
	* luah_getn_ProtectedByRetcheck: false
	* luah_getn_IsApartOfLua: true
	* luah_getn_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luah_getn_typedef = int(__cdecl*)(int a1);
	r_luah_getn_typedef r_luah_getn = (r_luah_getn_typedef)(x(0x016735B0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luah_getn_typedef = int(luah_getn_CCV*)(int a1);
	r_luah_getn_typedef r_luah_getn = (r_luah_getn_typedef)(x(Adresses::luah_getn_addr));



	*/
	static constexpr std::uintptr_t luah_getn_addr = 0x016735B0;
#define luah_getn_CCV __cdecl

	/*

	* luah_getnum_Decompiled_Args: std::uintptr_t a1, int a2
	* luah_getnum_TypeDef: int
	* luah_getnum_Aob: Unable to generate aob.
	* luah_getnum_ProtectedByRetcheck: false
	* luah_getnum_IsApartOfLua: true
	* luah_getnum_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luah_getnum_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luah_getnum_typedef r_luah_getnum = (r_luah_getnum_typedef)(x(0x01673640));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luah_getnum_typedef = int(luah_getnum_CCV*)(std::uintptr_t a1, int a2);
	r_luah_getnum_typedef r_luah_getnum = (r_luah_getnum_typedef)(x(Adresses::luah_getnum_addr));



	*/
	static constexpr std::uintptr_t luah_getnum_addr = 0x01673640;
#define luah_getnum_CCV __cdecl

	/*

	* luah_getstr_Decompiled_Args: std::uintptr_t a1, int a2
	* luah_getstr_TypeDef: int
	* luah_getstr_Aob: Unable to generate aob.
	* luah_getstr_ProtectedByRetcheck: false
	* luah_getstr_IsApartOfLua: true
	* luah_getstr_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luah_getstr_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luah_getstr_typedef r_luah_getstr = (r_luah_getstr_typedef)(x(0x01673720));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luah_getstr_typedef = int(luah_getstr_CCV*)(std::uintptr_t a1, int a2);
	r_luah_getstr_typedef r_luah_getstr = (r_luah_getstr_typedef)(x(Adresses::luah_getstr_addr));



	*/
	static constexpr std::uintptr_t luah_getstr_addr = 0x01673720;
#define luah_getstr_CCV __cdecl

	/*

	* luah_new_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luah_new_TypeDef: int
	* luah_new_Aob: Unable to generate aob.
	* luah_new_ProtectedByRetcheck: false
	* luah_new_IsApartOfLua: true
	* luah_new_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luah_new_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luah_new_typedef r_luah_new = (r_luah_new_typedef)(x(0x01673790));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luah_new_typedef = int(luah_new_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luah_new_typedef r_luah_new = (r_luah_new_typedef)(x(Adresses::luah_new_addr));



	*/
	static constexpr std::uintptr_t luah_new_addr = 0x01673790;
#define luah_new_CCV __cdecl

	/*

	* luah_next_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luah_next_TypeDef: int
	* luah_next_Aob: Unable to generate aob.
	* luah_next_ProtectedByRetcheck: false
	* luah_next_IsApartOfLua: true
	* luah_next_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luah_next_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luah_next_typedef r_luah_next = (r_luah_next_typedef)(x(0x01673800));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luah_next_typedef = int(luah_next_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luah_next_typedef r_luah_next = (r_luah_next_typedef)(x(Adresses::luah_next_addr));



	*/
	static constexpr std::uintptr_t luah_next_addr = 0x01673800;
#define luah_next_CCV __cdecl

	/*

	* luah_resizearray_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luah_resizearray_TypeDef: int
	* luah_resizearray_Aob: Unable to generate aob.
	* luah_resizearray_ProtectedByRetcheck: false
	* luah_resizearray_IsApartOfLua: true
	* luah_resizearray_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luah_resizearray_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luah_resizearray_typedef r_luah_resizearray = (r_luah_resizearray_typedef)(x(0x016739D0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luah_resizearray_typedef = int(luah_resizearray_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luah_resizearray_typedef r_luah_resizearray = (r_luah_resizearray_typedef)(x(Adresses::luah_resizearray_addr));



	*/
	static constexpr std::uintptr_t luah_resizearray_addr = 0x016739D0;
#define luah_resizearray_CCV __cdecl

	/*

	* luah_set_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luah_set_TypeDef: int
	* luah_set_Aob: Unable to generate aob.
	* luah_set_ProtectedByRetcheck: false
	* luah_set_IsApartOfLua: true
	* luah_set_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luah_set_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luah_set_typedef r_luah_set = (r_luah_set_typedef)(x(0x01673A20));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luah_set_typedef = int(luah_set_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luah_set_typedef r_luah_set = (r_luah_set_typedef)(x(Adresses::luah_set_addr));



	*/
	static constexpr std::uintptr_t luah_set_addr = 0x01673A20;
#define luah_set_CCV __cdecl

	/*

	* luah_setnum_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luah_setnum_TypeDef: int
	* luah_setnum_Aob: Unable to generate aob.
	* luah_setnum_ProtectedByRetcheck: false
	* luah_setnum_IsApartOfLua: true
	* luah_setnum_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luah_setnum_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luah_setnum_typedef r_luah_setnum = (r_luah_setnum_typedef)(x(0x01673B90));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luah_setnum_typedef = int(luah_setnum_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luah_setnum_typedef r_luah_setnum = (r_luah_setnum_typedef)(x(Adresses::luah_setnum_addr));



	*/
	static constexpr std::uintptr_t luah_setnum_addr = 0x01673B90;
#define luah_setnum_CCV __cdecl

	/*

	* luah_setstr_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luah_setstr_TypeDef: int
	* luah_setstr_Aob: Unable to generate aob.
	* luah_setstr_ProtectedByRetcheck: false
	* luah_setstr_IsApartOfLua: true
	* luah_setstr_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luah_setstr_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luah_setstr_typedef r_luah_setstr = (r_luah_setstr_typedef)(x(0x01673C00));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luah_setstr_typedef = int(luah_setstr_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luah_setstr_typedef r_luah_setstr = (r_luah_setstr_typedef)(x(Adresses::luah_setstr_addr));



	*/
	static constexpr std::uintptr_t luah_setstr_addr = 0x01673C00;
#define luah_setstr_CCV __cdecl

	/*

	* lual_addlstring_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lual_addlstring_TypeDef: int
	* lual_addlstring_Aob: Unable to generate aob.
	* lual_addlstring_ProtectedByRetcheck: false
	* lual_addlstring_IsApartOfLua: true
	* lual_addlstring_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_addlstring_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lual_addlstring_typedef r_lual_addlstring = (r_lual_addlstring_typedef)(x(0x0165CBD0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_addlstring_typedef = int(lual_addlstring_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lual_addlstring_typedef r_lual_addlstring = (r_lual_addlstring_typedef)(x(Adresses::lual_addlstring_addr));



	*/
	static constexpr std::uintptr_t lual_addlstring_addr = 0x0165CBD0;
#define lual_addlstring_CCV __cdecl

	/*

	* lual_addvalue_Decompiled_Args: int a1
	* lual_addvalue_TypeDef: int
	* lual_addvalue_Aob: Unable to generate aob.
	* lual_addvalue_ProtectedByRetcheck: false
	* lual_addvalue_IsApartOfLua: true
	* lual_addvalue_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_addvalue_typedef = int(__cdecl*)(int a1);
	r_lual_addvalue_typedef r_lual_addvalue = (r_lual_addvalue_typedef)(x(0x0165CC10));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_addvalue_typedef = int(lual_addvalue_CCV*)(int a1);
	r_lual_addvalue_typedef r_lual_addvalue = (r_lual_addvalue_typedef)(x(Adresses::lual_addvalue_addr));



	*/
	static constexpr std::uintptr_t lual_addvalue_addr = 0x0165CC10;
#define lual_addvalue_CCV __cdecl

	/*

	* lual_argerror_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lual_argerror_TypeDef: int
	* lual_argerror_Aob: Unable to generate aob.
	* lual_argerror_ProtectedByRetcheck: false
	* lual_argerror_IsApartOfLua: true
	* lual_argerror_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_argerror_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lual_argerror_typedef r_lual_argerror = (r_lual_argerror_typedef)(x(0x0165CC80));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_argerror_typedef = int(lual_argerror_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lual_argerror_typedef r_lual_argerror = (r_lual_argerror_typedef)(x(Adresses::lual_argerror_addr));



	*/
	static constexpr std::uintptr_t lual_argerror_addr = 0x0165CC80;
#define lual_argerror_CCV __cdecl

	/*

	* lual_buffinit_Decompiled_Args: std::uintptr_t a1, int a2
	* lual_buffinit_TypeDef: int
	* lual_buffinit_Aob: Unable to generate aob.
	* lual_buffinit_ProtectedByRetcheck: false
	* lual_buffinit_IsApartOfLua: true
	* lual_buffinit_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_buffinit_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lual_buffinit_typedef r_lual_buffinit = (r_lual_buffinit_typedef)(x(0x0165CD00));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_buffinit_typedef = int(lual_buffinit_CCV*)(std::uintptr_t a1, int a2);
	r_lual_buffinit_typedef r_lual_buffinit = (r_lual_buffinit_typedef)(x(Adresses::lual_buffinit_addr));



	*/
	static constexpr std::uintptr_t lual_buffinit_addr = 0x0165CD00;
#define lual_buffinit_CCV __cdecl

	/*

	* lual_checkany_Decompiled_Args: std::uintptr_t a1, int a2
	* lual_checkany_TypeDef: int
	* lual_checkany_Aob: Unable to generate aob.
	* lual_checkany_ProtectedByRetcheck: false
	* lual_checkany_IsApartOfLua: true
	* lual_checkany_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_checkany_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lual_checkany_typedef r_lual_checkany = (r_lual_checkany_typedef)(x(0x0165CD50));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_checkany_typedef = int(lual_checkany_CCV*)(std::uintptr_t a1, int a2);
	r_lual_checkany_typedef r_lual_checkany = (r_lual_checkany_typedef)(x(Adresses::lual_checkany_addr));



	*/
	static constexpr std::uintptr_t lual_checkany_addr = 0x0165CD50;
#define lual_checkany_CCV __cdecl

	/*

	* lual_checkinteger_Decompiled_Args: std::uintptr_t a1, int a2
	* lual_checkinteger_TypeDef: int
	* lual_checkinteger_Aob: Unable to generate aob.
	* lual_checkinteger_ProtectedByRetcheck: false
	* lual_checkinteger_IsApartOfLua: true
	* lual_checkinteger_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_checkinteger_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lual_checkinteger_typedef r_lual_checkinteger = (r_lual_checkinteger_typedef)(x(0x0165CD90));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_checkinteger_typedef = int(lual_checkinteger_CCV*)(std::uintptr_t a1, int a2);
	r_lual_checkinteger_typedef r_lual_checkinteger = (r_lual_checkinteger_typedef)(x(Adresses::lual_checkinteger_addr));



	*/
	static constexpr std::uintptr_t lual_checkinteger_addr = 0x0165CD90;
#define lual_checkinteger_CCV __cdecl

	/*

	* lual_checklstring_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lual_checklstring_TypeDef: int
	* lual_checklstring_Aob: Unable to generate aob.
	* lual_checklstring_ProtectedByRetcheck: false
	* lual_checklstring_IsApartOfLua: true
	* lual_checklstring_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_checklstring_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lual_checklstring_typedef r_lual_checklstring = (r_lual_checklstring_typedef)(x(0x0165CDC0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_checklstring_typedef = int(lual_checklstring_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lual_checklstring_typedef r_lual_checklstring = (r_lual_checklstring_typedef)(x(Adresses::lual_checklstring_addr));



	*/
	static constexpr std::uintptr_t lual_checklstring_addr = 0x0165CDC0;
#define lual_checklstring_CCV __cdecl

	/*

	* lual_checknumber_Decompiled_Args: std::uintptr_t a1, int a2
	* lual_checknumber_TypeDef: int
	* lual_checknumber_Aob: Unable to generate aob.
	* lual_checknumber_ProtectedByRetcheck: false
	* lual_checknumber_IsApartOfLua: true
	* lual_checknumber_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_checknumber_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lual_checknumber_typedef r_lual_checknumber = (r_lual_checknumber_typedef)(x(0x0165CDF0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_checknumber_typedef = int(lual_checknumber_CCV*)(std::uintptr_t a1, int a2);
	r_lual_checknumber_typedef r_lual_checknumber = (r_lual_checknumber_typedef)(x(Adresses::lual_checknumber_addr));



	*/
	static constexpr std::uintptr_t lual_checknumber_addr = 0x0165CDF0;
#define lual_checknumber_CCV __cdecl

	/*

	* lual_checkstack_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lual_checkstack_TypeDef: int
	* lual_checkstack_Aob: Unable to generate aob.
	* lual_checkstack_ProtectedByRetcheck: false
	* lual_checkstack_IsApartOfLua: true
	* lual_checkstack_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_checkstack_typedef = int(__thiscall*)(std::uintptr_t a1, int a2, int a3);
	r_lual_checkstack_typedef r_lual_checkstack = (r_lual_checkstack_typedef)(x(0x0165CE20));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_checkstack_typedef = int(lual_checkstack_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lual_checkstack_typedef r_lual_checkstack = (r_lual_checkstack_typedef)(x(Adresses::lual_checkstack_addr));



	*/
	static constexpr std::uintptr_t lual_checkstack_addr = 0x0165CE20;
#define lual_checkstack_CCV __thiscall

	/*

	* lual_checktype_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lual_checktype_TypeDef: int
	* lual_checktype_Aob: Unable to generate aob.
	* lual_checktype_ProtectedByRetcheck: false
	* lual_checktype_IsApartOfLua: true
	* lual_checktype_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_checktype_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lual_checktype_typedef r_lual_checktype = (r_lual_checktype_typedef)(x(0x0165CE60));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_checktype_typedef = int(lual_checktype_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lual_checktype_typedef r_lual_checktype = (r_lual_checktype_typedef)(x(Adresses::lual_checktype_addr));



	*/
	static constexpr std::uintptr_t lual_checktype_addr = 0x0165CE60;
#define lual_checktype_CCV __cdecl

	/*

	* lual_checkunsigned_Decompiled_Args: std::uintptr_t a1, int a2
	* lual_checkunsigned_TypeDef: int
	* lual_checkunsigned_Aob: Unable to generate aob.
	* lual_checkunsigned_ProtectedByRetcheck: false
	* lual_checkunsigned_IsApartOfLua: true
	* lual_checkunsigned_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_checkunsigned_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lual_checkunsigned_typedef r_lual_checkunsigned = (r_lual_checkunsigned_typedef)(x(0x0165CE90));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_checkunsigned_typedef = int(lual_checkunsigned_CCV*)(std::uintptr_t a1, int a2);
	r_lual_checkunsigned_typedef r_lual_checkunsigned = (r_lual_checkunsigned_typedef)(x(Adresses::lual_checkunsigned_addr));



	*/
	static constexpr std::uintptr_t lual_checkunsigned_addr = 0x0165CE90;
#define lual_checkunsigned_CCV __cdecl

	/*

	* lual_error_Decompiled_Args: int a1,  const char *a2, ...
	* lual_error_TypeDef: int
	* lual_error_Aob: Unable to generate aob.
	* lual_error_ProtectedByRetcheck: false
	* lual_error_IsApartOfLua: true
	* lual_error_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_error_typedef = int(__cdecl*)(int a1,  const char *a2, ...);
	r_lual_error_typedef r_lual_error = (r_lual_error_typedef)(x(0x0165CEC0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_error_typedef = int(lual_error_CCV*)(int a1,  const char *a2, ...);
	r_lual_error_typedef r_lual_error = (r_lual_error_typedef)(x(Adresses::lual_error_addr));



	*/
	static constexpr std::uintptr_t lual_error_addr = 0x0165CEC0;
#define lual_error_CCV __cdecl

	/*

	* lual_findtable_Decompiled_Args: std::uintptr_t a1, int a2, int a3, int a4
	* lual_findtable_TypeDef: int
	* lual_findtable_Aob: Unable to generate aob.
	* lual_findtable_ProtectedByRetcheck: false
	* lual_findtable_IsApartOfLua: true
	* lual_findtable_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_findtable_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_lual_findtable_typedef r_lual_findtable = (r_lual_findtable_typedef)(x(0x0165CFC0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_findtable_typedef = int(lual_findtable_CCV*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_lual_findtable_typedef r_lual_findtable = (r_lual_findtable_typedef)(x(Adresses::lual_findtable_addr));



	*/
	static constexpr std::uintptr_t lual_findtable_addr = 0x0165CFC0;
#define lual_findtable_CCV __cdecl

	/*

	* lual_getmetafield_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lual_getmetafield_TypeDef: int
	* lual_getmetafield_Aob: Unable to generate aob.
	* lual_getmetafield_ProtectedByRetcheck: false
	* lual_getmetafield_IsApartOfLua: true
	* lual_getmetafield_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_getmetafield_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lual_getmetafield_typedef r_lual_getmetafield = (r_lual_getmetafield_typedef)(x(0x0165D500));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_getmetafield_typedef = int(lual_getmetafield_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lual_getmetafield_typedef r_lual_getmetafield = (r_lual_getmetafield_typedef)(x(Adresses::lual_getmetafield_addr));



	*/
	static constexpr std::uintptr_t lual_getmetafield_addr = 0x0165D500;
#define lual_getmetafield_CCV __cdecl

	/*

	* lual_newmetatable_Decompiled_Args: std::uintptr_t a1, int a2
	* lual_newmetatable_TypeDef: int
	* lual_newmetatable_Aob: Unable to generate aob.
	* lual_newmetatable_ProtectedByRetcheck: false
	* lual_newmetatable_IsApartOfLua: true
	* lual_newmetatable_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_newmetatable_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lual_newmetatable_typedef r_lual_newmetatable = (r_lual_newmetatable_typedef)(x(0x0165D100));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_newmetatable_typedef = int(lual_newmetatable_CCV*)(std::uintptr_t a1, int a2);
	r_lual_newmetatable_typedef r_lual_newmetatable = (r_lual_newmetatable_typedef)(x(Adresses::lual_newmetatable_addr));



	*/
	static constexpr std::uintptr_t lual_newmetatable_addr = 0x0165D100;
#define lual_newmetatable_CCV __cdecl

	/*

	* lual_optinteger_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lual_optinteger_TypeDef: int
	* lual_optinteger_Aob: Unable to generate aob.
	* lual_optinteger_ProtectedByRetcheck: false
	* lual_optinteger_IsApartOfLua: true
	* lual_optinteger_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_optinteger_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lual_optinteger_typedef r_lual_optinteger = (r_lual_optinteger_typedef)(x(0x0165D160));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_optinteger_typedef = int(lual_optinteger_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lual_optinteger_typedef r_lual_optinteger = (r_lual_optinteger_typedef)(x(Adresses::lual_optinteger_addr));



	*/
	static constexpr std::uintptr_t lual_optinteger_addr = 0x0165D160;
#define lual_optinteger_CCV __cdecl

	/*

	* lual_optlstring_Decompiled_Args: std::uintptr_t a1, int a2, const char* a3, int a4
	* lual_optlstring_TypeDef: int
	* lual_optlstring_Aob: Unable to generate aob.
	* lual_optlstring_ProtectedByRetcheck: false
	* lual_optlstring_IsApartOfLua: true
	* lual_optlstring_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_optlstring_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, const char* a3, int a4);
	r_lual_optlstring_typedef r_lual_optlstring = (r_lual_optlstring_typedef)(x(0x0165D1B0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_optlstring_typedef = int(lual_optlstring_CCV*)(std::uintptr_t a1, int a2, const char* a3, int a4);
	r_lual_optlstring_typedef r_lual_optlstring = (r_lual_optlstring_typedef)(x(Adresses::lual_optlstring_addr));



	*/
	static constexpr std::uintptr_t lual_optlstring_addr = 0x0165D1B0;
#define lual_optlstring_CCV __cdecl

	/*

	* lual_prepbuffer_Decompiled_Args: int a1
	* lual_prepbuffer_TypeDef: int
	* lual_prepbuffer_Aob: Unable to generate aob.
	* lual_prepbuffer_ProtectedByRetcheck: false
	* lual_prepbuffer_IsApartOfLua: true
	* lual_prepbuffer_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_prepbuffer_typedef = int(__cdecl*)(int a1);
	r_lual_prepbuffer_typedef r_lual_prepbuffer = (r_lual_prepbuffer_typedef)(x(0x0165CEF0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_prepbuffer_typedef = int(lual_prepbuffer_CCV*)(int a1);
	r_lual_prepbuffer_typedef r_lual_prepbuffer = (r_lual_prepbuffer_typedef)(x(Adresses::lual_prepbuffer_addr));



	*/
	static constexpr std::uintptr_t lual_prepbuffer_addr = 0x0165CEF0;
#define lual_prepbuffer_CCV __cdecl

	/*

	* lual_pushresult_Decompiled_Args: int a1
	* lual_pushresult_TypeDef: int
	* lual_pushresult_Aob: Unable to generate aob.
	* lual_pushresult_ProtectedByRetcheck: false
	* lual_pushresult_IsApartOfLua: true
	* lual_pushresult_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_pushresult_typedef = int(__cdecl*)(int a1);
	r_lual_pushresult_typedef r_lual_pushresult = (r_lual_pushresult_typedef)(x(0x0165D330));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_pushresult_typedef = int(lual_pushresult_CCV*)(int a1);
	r_lual_pushresult_typedef r_lual_pushresult = (r_lual_pushresult_typedef)(x(Adresses::lual_pushresult_addr));



	*/
	static constexpr std::uintptr_t lual_pushresult_addr = 0x0165D330;
#define lual_pushresult_CCV __cdecl

	/*

	* lual_ref_Decompiled_Args: std::uintptr_t a1, int a2
	* lual_ref_TypeDef: int
	* lual_ref_Aob: Unable to generate aob.
	* lual_ref_ProtectedByRetcheck: false
	* lual_ref_IsApartOfLua: true
	* lual_ref_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_ref_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lual_ref_typedef r_lual_ref = (r_lual_ref_typedef)(x(0x0165BC80));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_ref_typedef = int(lual_ref_CCV*)(std::uintptr_t a1, int a2);
	r_lual_ref_typedef r_lual_ref = (r_lual_ref_typedef)(x(Adresses::lual_ref_addr));



	*/
	static constexpr std::uintptr_t lual_ref_addr = 0x0165BC80;
#define lual_ref_CCV __cdecl

	/*

	* lual_register_Decompiled_Args: int a1, const char* a2, int a3
	* lual_register_TypeDef: int
	* lual_register_Aob: Unable to generate aob.
	* lual_register_ProtectedByRetcheck: false
	* lual_register_IsApartOfLua: true
	* lual_register_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_register_typedef = int(__cdecl*)(int a1, const char* a2, int a3);
	r_lual_register_typedef r_lual_register = (r_lual_register_typedef)(x(0x0165D3E0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_register_typedef = int(lual_register_CCV*)(int a1, const char* a2, int a3);
	r_lual_register_typedef r_lual_register = (r_lual_register_typedef)(x(Adresses::lual_register_addr));



	*/
	static constexpr std::uintptr_t lual_register_addr = 0x0165D3E0;
#define lual_register_CCV __cdecl

	/*

	* lual_typeerror_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lual_typeerror_TypeDef: int
	* lual_typeerror_Aob: Unable to generate aob.
	* lual_typeerror_ProtectedByRetcheck: false
	* lual_typeerror_IsApartOfLua: true
	* lual_typeerror_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_typeerror_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_lual_typeerror_typedef r_lual_typeerror = (r_lual_typeerror_typedef)(x(0x0165D710));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_typeerror_typedef = int(lual_typeerror_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lual_typeerror_typedef r_lual_typeerror = (r_lual_typeerror_typedef)(x(Adresses::lual_typeerror_addr));



	*/
	static constexpr std::uintptr_t lual_typeerror_addr = 0x0165D710;
#define lual_typeerror_CCV __cdecl

	/*

	* lual_unref_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* lual_unref_TypeDef: int
	* lual_unref_Aob: Unable to generate aob.
	* lual_unref_ProtectedByRetcheck: false
	* lual_unref_IsApartOfLua: true
	* lual_unref_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_unref_typedef = int(__fastcall*)(std::uintptr_t a1, int a2, int a3);
	r_lual_unref_typedef r_lual_unref = (r_lual_unref_typedef)(x(0x01669710));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_unref_typedef = int(lual_unref_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_lual_unref_typedef r_lual_unref = (r_lual_unref_typedef)(x(Adresses::lual_unref_addr));



	*/
	static constexpr std::uintptr_t lual_unref_addr = 0x01669710;
#define lual_unref_CCV __fastcall

	/*

	* lual_where_Decompiled_Args: std::uintptr_t a1, int a2
	* lual_where_TypeDef: int
	* lual_where_Aob: Unable to generate aob.
	* lual_where_ProtectedByRetcheck: false
	* lual_where_IsApartOfLua: true
	* lual_where_Mask: ....
	* Usage (Not 100% Accurate):

	using r_lual_where_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_lual_where_typedef r_lual_where = (r_lual_where_typedef)(x(0x0165D7B0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_lual_where_typedef = int(lual_where_CCV*)(std::uintptr_t a1, int a2);
	r_lual_where_typedef r_lual_where = (r_lual_where_typedef)(x(Adresses::lual_where_addr));



	*/
	static constexpr std::uintptr_t lual_where_addr = 0x0165D7B0;
#define lual_where_CCV __cdecl

	/*

	* luam_realloc_Decompiled_Args: std::uintptr_t a1, int a2, int a3, int a4
	* luam_realloc_TypeDef: int
	* luam_realloc_Aob: Unable to generate aob.
	* luam_realloc_ProtectedByRetcheck: false
	* luam_realloc_IsApartOfLua: true
	* luam_realloc_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luam_realloc_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_luam_realloc_typedef r_luam_realloc = (r_luam_realloc_typedef)(x(0x01676321));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luam_realloc_typedef = int(luam_realloc_CCV*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_luam_realloc_typedef r_luam_realloc = (r_luam_realloc_typedef)(x(Adresses::luam_realloc_addr));



	*/
	static constexpr std::uintptr_t luam_realloc_addr = 0x01676321;
#define luam_realloc_CCV __cdecl

	/*

	* luam_realloc__Decompiled_Args: std::uintptr_t a1, int a2
	* luam_realloc__TypeDef: int
	* luam_realloc__Aob: Unable to generate aob.
	* luam_realloc__ProtectedByRetcheck: false
	* luam_realloc__IsApartOfLua: true
	* luam_realloc__Mask: ....
	* Usage (Not 100% Accurate):

	using r_luam_realloc__typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luam_realloc__typedef r_luam_realloc_ = (r_luam_realloc__typedef)(x(0x016760D0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luam_realloc__typedef = int(luam_realloc__CCV*)(std::uintptr_t a1, int a2);
	r_luam_realloc__typedef r_luam_realloc_ = (r_luam_realloc__typedef)(x(Adresses::luam_realloc__addr));



	*/
	static constexpr std::uintptr_t luam_realloc__addr = 0x016760D0;
#define luam_realloc__CCV __cdecl

	/*

	* luam_toobig_Decompiled_Args: int a1
	* luam_toobig_TypeDef: int
	* luam_toobig_Aob: Unable to generate aob.
	* luam_toobig_ProtectedByRetcheck: false
	* luam_toobig_IsApartOfLua: true
	* luam_toobig_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luam_toobig_typedef = int(__thiscall*)(int a1);
	r_luam_toobig_typedef r_luam_toobig = (r_luam_toobig_typedef)(x(0x01676330));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luam_toobig_typedef = int(luam_toobig_CCV*)(int a1);
	r_luam_toobig_typedef r_luam_toobig = (r_luam_toobig_typedef)(x(Adresses::luam_toobig_addr));



	*/
	static constexpr std::uintptr_t luam_toobig_addr = 0x01676330;
#define luam_toobig_CCV __thiscall

	/*

	* luao_chunkid_Decompiled_Args: char* a1, const char* a2, int a3
	* luao_chunkid_TypeDef: int
	* luao_chunkid_Aob: Unable to generate aob.
	* luao_chunkid_ProtectedByRetcheck: false
	* luao_chunkid_IsApartOfLua: true
	* luao_chunkid_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luao_chunkid_typedef = int(__cdecl*)(char* a1, const char* a2, int a3);
	r_luao_chunkid_typedef r_luao_chunkid = (r_luao_chunkid_typedef)(x(0x01667C90));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luao_chunkid_typedef = int(luao_chunkid_CCV*)(char* a1, const char* a2, int a3);
	r_luao_chunkid_typedef r_luao_chunkid = (r_luao_chunkid_typedef)(x(Adresses::luao_chunkid_addr));



	*/
	static constexpr std::uintptr_t luao_chunkid_addr = 0x01667C90;
#define luao_chunkid_CCV __cdecl

	/*

	* luao_pushfstring_Decompiled_Args: std::uintptr_t a1, const char* a2, ...
	* luao_pushfstring_TypeDef: int
	* luao_pushfstring_Aob: Unable to generate aob.
	* luao_pushfstring_ProtectedByRetcheck: false
	* luao_pushfstring_IsApartOfLua: true
	* luao_pushfstring_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luao_pushfstring_typedef = int(__cdecl*)(std::uintptr_t a1, const char* a2, ...);
	r_luao_pushfstring_typedef r_luao_pushfstring = (r_luao_pushfstring_typedef)(x(0x01667E10));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luao_pushfstring_typedef = int(luao_pushfstring_CCV*)(std::uintptr_t a1, const char* a2, ...);
	r_luao_pushfstring_typedef r_luao_pushfstring = (r_luao_pushfstring_typedef)(x(Adresses::luao_pushfstring_addr));



	*/
	static constexpr std::uintptr_t luao_pushfstring_addr = 0x01667E10;
#define luao_pushfstring_CCV __cdecl

	/*

	* luao_pushvfstring_Decompiled_Args: std::uintptr_t a1, const char* a2, char* a3
	* luao_pushvfstring_TypeDef: int
	* luao_pushvfstring_Aob: Unable to generate aob.
	* luao_pushvfstring_ProtectedByRetcheck: false
	* luao_pushvfstring_IsApartOfLua: true
	* luao_pushvfstring_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luao_pushvfstring_typedef = int(__cdecl*)(std::uintptr_t a1, const char* a2, char* a3);
	r_luao_pushvfstring_typedef r_luao_pushvfstring = (r_luao_pushvfstring_typedef)(x(0x01667EB0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luao_pushvfstring_typedef = int(luao_pushvfstring_CCV*)(std::uintptr_t a1, const char* a2, char* a3);
	r_luao_pushvfstring_typedef r_luao_pushvfstring = (r_luao_pushvfstring_typedef)(x(Adresses::luao_pushvfstring_addr));



	*/
	static constexpr std::uintptr_t luao_pushvfstring_addr = 0x01667EB0;
#define luao_pushvfstring_CCV __cdecl

	/*

	* luao_rawequalKey_Decompiled_Args: std::uintptr_t a1, int a2
	* luao_rawequalKey_TypeDef: int
	* luao_rawequalKey_Aob: Unable to generate aob.
	* luao_rawequalKey_ProtectedByRetcheck: false
	* luao_rawequalKey_IsApartOfLua: true
	* luao_rawequalKey_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luao_rawequalKey_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luao_rawequalKey_typedef r_luao_rawequalKey = (r_luao_rawequalKey_typedef)(x(0x01667F50));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luao_rawequalKey_typedef = int(luao_rawequalKey_CCV*)(std::uintptr_t a1, int a2);
	r_luao_rawequalKey_typedef r_luao_rawequalKey = (r_luao_rawequalKey_typedef)(x(Adresses::luao_rawequalKey_addr));



	*/
	static constexpr std::uintptr_t luao_rawequalKey_addr = 0x01667F50;
#define luao_rawequalKey_CCV __cdecl

	/*

	* luao_rawequalobj_Decompiled_Args: std::uintptr_t a1, int a2
	* luao_rawequalobj_TypeDef: int
	* luao_rawequalobj_Aob: Unable to generate aob.
	* luao_rawequalobj_ProtectedByRetcheck: false
	* luao_rawequalobj_IsApartOfLua: true
	* luao_rawequalobj_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luao_rawequalobj_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luao_rawequalobj_typedef r_luao_rawequalobj = (r_luao_rawequalobj_typedef)(x(0x01667FD0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luao_rawequalobj_typedef = int(luao_rawequalobj_CCV*)(std::uintptr_t a1, int a2);
	r_luao_rawequalobj_typedef r_luao_rawequalobj = (r_luao_rawequalobj_typedef)(x(Adresses::luao_rawequalobj_addr));



	*/
	static constexpr std::uintptr_t luao_rawequalobj_addr = 0x01667FD0;
#define luao_rawequalobj_CCV __cdecl

	/*

	* luao_str2d_Decompiled_Args: const char* a1, double* a2
	* luao_str2d_TypeDef: int
	* luao_str2d_Aob: Unable to generate aob.
	* luao_str2d_ProtectedByRetcheck: false
	* luao_str2d_IsApartOfLua: true
	* luao_str2d_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luao_str2d_typedef = int(__cdecl*)(const char* a1, double* a2);
	r_luao_str2d_typedef r_luao_str2d = (r_luao_str2d_typedef)(x(0x01668050));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luao_str2d_typedef = int(luao_str2d_CCV*)(const char* a1, double* a2);
	r_luao_str2d_typedef r_luao_str2d = (r_luao_str2d_typedef)(x(Adresses::luao_str2d_addr));



	*/
	static constexpr std::uintptr_t luao_str2d_addr = 0x01668050;
#define luao_str2d_CCV __cdecl

	/*

	* luas_newlstr_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luas_newlstr_TypeDef: int
	* luas_newlstr_Aob: Unable to generate aob.
	* luas_newlstr_ProtectedByRetcheck: false
	* luas_newlstr_IsApartOfLua: true
	* luas_newlstr_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luas_newlstr_typedef = int(__fastcall*)(std::uintptr_t a1, int a2, int a3);
	r_luas_newlstr_typedef r_luas_newlstr = (r_luas_newlstr_typedef)(x(0x01672F80));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luas_newlstr_typedef = int(luas_newlstr_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luas_newlstr_typedef r_luas_newlstr = (r_luas_newlstr_typedef)(x(Adresses::luas_newlstr_addr));



	*/
	static constexpr std::uintptr_t luas_newlstr_addr = 0x01672F80;
#define luas_newlstr_CCV __fastcall

	/*

	* luas_newudata_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luas_newudata_TypeDef: int
	* luas_newudata_Aob: Unable to generate aob.
	* luas_newudata_ProtectedByRetcheck: false
	* luas_newudata_IsApartOfLua: true
	* luas_newudata_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luas_newudata_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luas_newudata_typedef r_luas_newudata = (r_luas_newudata_typedef)(x(0x01673140));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luas_newudata_typedef = int(luas_newudata_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luas_newudata_typedef r_luas_newudata = (r_luas_newudata_typedef)(x(Adresses::luas_newudata_addr));



	*/
	static constexpr std::uintptr_t luas_newudata_addr = 0x01673140;
#define luas_newudata_CCV __cdecl

	/*

	* luas_resize_Decompiled_Args: std::uintptr_t a1, int a2
	* luas_resize_TypeDef: int
	* luas_resize_Aob: Unable to generate aob.
	* luas_resize_ProtectedByRetcheck: false
	* luas_resize_IsApartOfLua: true
	* luas_resize_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luas_resize_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luas_resize_typedef r_luas_resize = (r_luas_resize_typedef)(x(0x016731A0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luas_resize_typedef = int(luas_resize_CCV*)(std::uintptr_t a1, int a2);
	r_luas_resize_typedef r_luas_resize = (r_luas_resize_typedef)(x(Adresses::luas_resize_addr));



	*/
	static constexpr std::uintptr_t luas_resize_addr = 0x016731A0;
#define luas_resize_CCV __cdecl

	/*

	* luat_gettm_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luat_gettm_TypeDef: int
	* luat_gettm_Aob: Unable to generate aob.
	* luat_gettm_ProtectedByRetcheck: false
	* luat_gettm_IsApartOfLua: true
	* luat_gettm_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luat_gettm_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luat_gettm_typedef r_luat_gettm = (r_luat_gettm_typedef)(x(0x01675D10));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luat_gettm_typedef = int(luat_gettm_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luat_gettm_typedef r_luat_gettm = (r_luat_gettm_typedef)(x(Adresses::luat_gettm_addr));



	*/
	static constexpr std::uintptr_t luat_gettm_addr = 0x01675D10;
#define luat_gettm_CCV __cdecl

	/*

	* luat_objtypename_Decompiled_Args: std::uintptr_t a1, int a2
	* luat_objtypename_TypeDef: int
	* luat_objtypename_Aob: Unable to generate aob.
	* luat_objtypename_ProtectedByRetcheck: false
	* luat_objtypename_IsApartOfLua: true
	* luat_objtypename_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luat_objtypename_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luat_objtypename_typedef r_luat_objtypename = (r_luat_objtypename_typedef)(x(0x01675E60));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luat_objtypename_typedef = int(luat_objtypename_CCV*)(std::uintptr_t a1, int a2);
	r_luat_objtypename_typedef r_luat_objtypename = (r_luat_objtypename_typedef)(x(Adresses::luat_objtypename_addr));



	*/
	static constexpr std::uintptr_t luat_objtypename_addr = 0x01675E60;
#define luat_objtypename_CCV __cdecl

	/*

	* luau_backtrace_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luau_backtrace_TypeDef: int
	* luau_backtrace_Aob: Unable to generate aob.
	* luau_backtrace_ProtectedByRetcheck: false
	* luau_backtrace_IsApartOfLua: true
	* luau_backtrace_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luau_backtrace_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luau_backtrace_typedef r_luau_backtrace = (r_luau_backtrace_typedef)(x(0x01666590));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luau_backtrace_typedef = int(luau_backtrace_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luau_backtrace_typedef r_luau_backtrace = (r_luau_backtrace_typedef)(x(Adresses::luau_backtrace_addr));



	*/
	static constexpr std::uintptr_t luau_backtrace_addr = 0x01666590;
#define luau_backtrace_CCV __cdecl

	/*

	* luau_betavm_Decompiled_Args: int a1
	* luau_betavm_TypeDef: int
	* luau_betavm_Aob: Unable to generate aob.
	* luau_betavm_ProtectedByRetcheck: false
	* luau_betavm_IsApartOfLua: true
	* luau_betavm_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luau_betavm_typedef = int(__cdecl*)(int a1);
	r_luau_betavm_typedef r_luau_betavm = (r_luau_betavm_typedef)(x(0x01662570));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luau_betavm_typedef = int(luau_betavm_CCV*)(int a1);
	r_luau_betavm_typedef r_luau_betavm = (r_luau_betavm_typedef)(x(Adresses::luau_betavm_addr));



	*/
	static constexpr std::uintptr_t luau_betavm_addr = 0x01662570;
#define luau_betavm_CCV __cdecl

	/*

	* luau_callhook_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luau_callhook_TypeDef: int
	* luau_callhook_Aob: Unable to generate aob.
	* luau_callhook_ProtectedByRetcheck: true
	* luau_callhook_IsApartOfLua: true
	* luau_callhook_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luau_callhook_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luau_callhook_typedef r_luau_callhook = (r_luau_callhook_typedef)unprotect(x(0x01666B30));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luau_callhook_typedef = int(luau_callhook_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luau_callhook_typedef r_luau_callhook = (r_luau_callhook_typedef)unprotect(x(Adresses::luau_callhook_addr));



	*/
	static constexpr std::uintptr_t luau_callhook_addr = 0x01666B30;
#define luau_callhook_CCV __cdecl

	/*

	* luau_calltm_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luau_calltm_TypeDef: int
	* luau_calltm_Aob: Unable to generate aob.
	* luau_calltm_ProtectedByRetcheck: false
	* luau_calltm_IsApartOfLua: true
	* luau_calltm_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luau_calltm_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luau_calltm_typedef r_luau_calltm = (r_luau_calltm_typedef)(x(0x01666A10));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luau_calltm_typedef = int(luau_calltm_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luau_calltm_typedef r_luau_calltm = (r_luau_calltm_typedef)(x(Adresses::luau_calltm_addr));



	*/
	static constexpr std::uintptr_t luau_calltm_addr = 0x01666A10;
#define luau_calltm_CCV __cdecl

	/*

	* luau_deserialize_Decompiled_Args: std::uintptr_t a1, const char* a2, const char* a3, int a4, int a5
	* luau_deserialize_TypeDef: int
	* luau_deserialize_Aob: Unable to generate aob.
	* luau_deserialize_ProtectedByRetcheck: false
	* luau_deserialize_IsApartOfLua: true
	* luau_deserialize_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luau_deserialize_typedef = int(__cdecl*)(std::uintptr_t a1, const char* a2, const char* a3, int a4, int a5);
	r_luau_deserialize_typedef r_luau_deserialize = (r_luau_deserialize_typedef)(x(0x01666CB0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luau_deserialize_typedef = int(luau_deserialize_CCV*)(std::uintptr_t a1, const char* a2, const char* a3, int a4, int a5);
	r_luau_deserialize_typedef r_luau_deserialize = (r_luau_deserialize_typedef)(x(Adresses::luau_deserialize_addr));



	*/
	static constexpr std::uintptr_t luau_deserialize_addr = 0x01666CB0;
#define luau_deserialize_CCV __cdecl

	/*

	* luau_execute_Decompiled_Args: std::uintptr_t a1, int a2, int a3, int a4, int a5, int a6
	* luau_execute_TypeDef: int
	* luau_execute_Aob: Unable to generate aob.
	* luau_execute_ProtectedByRetcheck: false
	* luau_execute_IsApartOfLua: true
	* luau_execute_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luau_execute_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3, int a4, int a5, int a6);
	r_luau_execute_typedef r_luau_execute = (r_luau_execute_typedef)(x(0x016924A0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luau_execute_typedef = int(luau_execute_CCV*)(std::uintptr_t a1, int a2, int a3, int a4, int a5, int a6);
	r_luau_execute_typedef r_luau_execute = (r_luau_execute_typedef)(x(Adresses::luau_execute_addr));



	*/
	static constexpr std::uintptr_t luau_execute_addr = 0x016924A0;
#define luau_execute_CCV __cdecl

	/*

	* luau_loopforg_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luau_loopforg_TypeDef: int
	* luau_loopforg_Aob: Unable to generate aob.
	* luau_loopforg_ProtectedByRetcheck: false
	* luau_loopforg_IsApartOfLua: true
	* luau_loopforg_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luau_loopforg_typedef = int(__thiscall*)(std::uintptr_t a1, int a2, int a3);
	r_luau_loopforg_typedef r_luau_loopforg = (r_luau_loopforg_typedef)(x(0x01669BA0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luau_loopforg_typedef = int(luau_loopforg_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luau_loopforg_typedef r_luau_loopforg = (r_luau_loopforg_typedef)(x(Adresses::luau_loopforg_addr));



	*/
	static constexpr std::uintptr_t luau_loopforg_addr = 0x01669BA0;
#define luau_loopforg_CCV __thiscall

	/*

	* luau_prepareforn_Decompiled_Args: std::uintptr_t a1, int a2, int a3, int a4
	* luau_prepareforn_TypeDef: int
	* luau_prepareforn_Aob: Unable to generate aob.
	* luau_prepareforn_ProtectedByRetcheck: false
	* luau_prepareforn_IsApartOfLua: true
	* luau_prepareforn_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luau_prepareforn_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_luau_prepareforn_typedef r_luau_prepareforn = (r_luau_prepareforn_typedef)(x(0x01667980));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luau_prepareforn_typedef = int(luau_prepareforn_CCV*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_luau_prepareforn_typedef r_luau_prepareforn = (r_luau_prepareforn_typedef)(x(Adresses::luau_prepareforn_addr));



	*/
	static constexpr std::uintptr_t luau_prepareforn_addr = 0x01667980;
#define luau_prepareforn_CCV __cdecl

	/*

	* luau_resolveimport_Decompiled_Args: std::uintptr_t a1, int a2, int a3, int a4, int a5
	* luau_resolveimport_TypeDef: int
	* luau_resolveimport_Aob: Unable to generate aob.
	* luau_resolveimport_ProtectedByRetcheck: false
	* luau_resolveimport_IsApartOfLua: true
	* luau_resolveimport_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luau_resolveimport_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3, int a4, int a5);
	r_luau_resolveimport_typedef r_luau_resolveimport = (r_luau_resolveimport_typedef)(x(0x01667A20));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luau_resolveimport_typedef = int(luau_resolveimport_CCV*)(std::uintptr_t a1, int a2, int a3, int a4, int a5);
	r_luau_resolveimport_typedef r_luau_resolveimport = (r_luau_resolveimport_typedef)(x(Adresses::luau_resolveimport_addr));



	*/
	static constexpr std::uintptr_t luau_resolveimport_addr = 0x01667A20;
#define luau_resolveimport_CCV __cdecl

	/*

	* luau_tryfunctm_Decompiled_Args: std::uintptr_t a1, int a2
	* luau_tryfunctm_TypeDef: int
	* luau_tryfunctm_Aob: Unable to generate aob.
	* luau_tryfunctm_ProtectedByRetcheck: false
	* luau_tryfunctm_IsApartOfLua: true
	* luau_tryfunctm_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luau_tryfunctm_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luau_tryfunctm_typedef r_luau_tryfunctm = (r_luau_tryfunctm_typedef)(x(0x01667BC0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luau_tryfunctm_typedef = int(luau_tryfunctm_CCV*)(std::uintptr_t a1, int a2);
	r_luau_tryfunctm_typedef r_luau_tryfunctm = (r_luau_tryfunctm_typedef)(x(Adresses::luau_tryfunctm_addr));



	*/
	static constexpr std::uintptr_t luau_tryfunctm_addr = 0x01667BC0;
#define luau_tryfunctm_CCV __cdecl

	/*

	* luav_concat_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luav_concat_TypeDef: int
	* luav_concat_Aob: Unable to generate aob.
	* luav_concat_ProtectedByRetcheck: false
	* luav_concat_IsApartOfLua: true
	* luav_concat_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luav_concat_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luav_concat_typedef r_luav_concat = (r_luav_concat_typedef)(x(0x01674DD0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luav_concat_typedef = int(luav_concat_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luav_concat_typedef r_luav_concat = (r_luav_concat_typedef)(x(Adresses::luav_concat_addr));



	*/
	static constexpr std::uintptr_t luav_concat_addr = 0x01674DD0;
#define luav_concat_CCV __cdecl

	/*

	* luav_doarith_Decompiled_Args: std::uintptr_t a1, int a2, int a3, int a4, int a5, int a6
	* luav_doarith_TypeDef: int
	* luav_doarith_Aob: Unable to generate aob.
	* luav_doarith_ProtectedByRetcheck: false
	* luav_doarith_IsApartOfLua: true
	* luav_doarith_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luav_doarith_typedef = int(__thiscall*)(std::uintptr_t a1, int a2, int a3, int a4, int a5, int a6);
	r_luav_doarith_typedef r_luav_doarith = (r_luav_doarith_typedef)(x(0x01675160));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luav_doarith_typedef = int(luav_doarith_CCV*)(std::uintptr_t a1, int a2, int a3, int a4, int a5, int a6);
	r_luav_doarith_typedef r_luav_doarith = (r_luav_doarith_typedef)(x(Adresses::luav_doarith_addr));



	*/
	static constexpr std::uintptr_t luav_doarith_addr = 0x01675160;
#define luav_doarith_CCV __thiscall

	/*

	* luav_dolen_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luav_dolen_TypeDef: int
	* luav_dolen_Aob: Unable to generate aob.
	* luav_dolen_ProtectedByRetcheck: false
	* luav_dolen_IsApartOfLua: true
	* luav_dolen_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luav_dolen_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luav_dolen_typedef r_luav_dolen = (r_luav_dolen_typedef)(x(0x01676F10));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luav_dolen_typedef = int(luav_dolen_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luav_dolen_typedef r_luav_dolen = (r_luav_dolen_typedef)(x(Adresses::luav_dolen_addr));



	*/
	static constexpr std::uintptr_t luav_dolen_addr = 0x01676F10;
#define luav_dolen_CCV __cdecl

	/*

	* luav_execute_Decompiled_Args: int a1
	* luav_execute_TypeDef: int
	* luav_execute_Aob: Unable to generate aob.
	* luav_execute_ProtectedByRetcheck: false
	* luav_execute_IsApartOfLua: true
	* luav_execute_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luav_execute_typedef = int(__cdecl*)(int a1);
	r_luav_execute_typedef r_luav_execute = (r_luav_execute_typedef)(x(0x0165E4F0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luav_execute_typedef = int(luav_execute_CCV*)(int a1);
	r_luav_execute_typedef r_luav_execute = (r_luav_execute_typedef)(x(Adresses::luav_execute_addr));



	*/
	static constexpr std::uintptr_t luav_execute_addr = 0x0165E4F0;
#define luav_execute_CCV __cdecl

	/*

	* luav_gettable_Decompiled_Args: std::uintptr_t a1, int a2, int a3, int a4, int a5
	* luav_gettable_TypeDef: int
	* luav_gettable_Aob: Unable to generate aob.
	* luav_gettable_ProtectedByRetcheck: false
	* luav_gettable_IsApartOfLua: true
	* luav_gettable_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luav_gettable_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3, int a4, int a5);
	r_luav_gettable_typedef r_luav_gettable = (r_luav_gettable_typedef)(x(0x01675860));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luav_gettable_typedef = int(luav_gettable_CCV*)(std::uintptr_t a1, int a2, int a3, int a4, int a5);
	r_luav_gettable_typedef r_luav_gettable = (r_luav_gettable_typedef)(x(Adresses::luav_gettable_addr));



	*/
	static constexpr std::uintptr_t luav_gettable_addr = 0x01675860;
#define luav_gettable_CCV __cdecl

	/*

	* luav_gettable_opt_Decompiled_Args: std::uintptr_t a1, int a2, int a3, int a4
	* luav_gettable_opt_TypeDef: int
	* luav_gettable_opt_Aob: Unable to generate aob.
	* luav_gettable_opt_ProtectedByRetcheck: false
	* luav_gettable_opt_IsApartOfLua: true
	* luav_gettable_opt_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luav_gettable_opt_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_luav_gettable_opt_typedef r_luav_gettable_opt = (r_luav_gettable_opt_typedef)(x(0x01675840));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luav_gettable_opt_typedef = int(luav_gettable_opt_CCV*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_luav_gettable_opt_typedef r_luav_gettable_opt = (r_luav_gettable_opt_typedef)(x(Adresses::luav_gettable_opt_addr));



	*/
	static constexpr std::uintptr_t luav_gettable_opt_addr = 0x01675840;
#define luav_gettable_opt_CCV __cdecl

	/*

	* luav_lessequal_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luav_lessequal_TypeDef: int
	* luav_lessequal_Aob: Unable to generate aob.
	* luav_lessequal_ProtectedByRetcheck: false
	* luav_lessequal_IsApartOfLua: true
	* luav_lessequal_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luav_lessequal_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luav_lessequal_typedef r_luav_lessequal = (r_luav_lessequal_typedef)(x(0x01675970));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luav_lessequal_typedef = int(luav_lessequal_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luav_lessequal_typedef r_luav_lessequal = (r_luav_lessequal_typedef)(x(Adresses::luav_lessequal_addr));



	*/
	static constexpr std::uintptr_t luav_lessequal_addr = 0x01675970;
#define luav_lessequal_CCV __cdecl

	/*

	* luav_lessthan_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* luav_lessthan_TypeDef: int
	* luav_lessthan_Aob: Unable to generate aob.
	* luav_lessthan_ProtectedByRetcheck: false
	* luav_lessthan_IsApartOfLua: true
	* luav_lessthan_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luav_lessthan_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_luav_lessthan_typedef r_luav_lessthan = (r_luav_lessthan_typedef)(x(0x01675A10));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luav_lessthan_typedef = int(luav_lessthan_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_luav_lessthan_typedef r_luav_lessthan = (r_luav_lessthan_typedef)(x(Adresses::luav_lessthan_addr));



	*/
	static constexpr std::uintptr_t luav_lessthan_addr = 0x01675A10;
#define luav_lessthan_CCV __cdecl

	/*

	* luav_settable_Decompiled_Args: std::uintptr_t a1, int a2, int a3, int a4
	* luav_settable_TypeDef: int
	* luav_settable_Aob: Unable to generate aob.
	* luav_settable_ProtectedByRetcheck: false
	* luav_settable_IsApartOfLua: true
	* luav_settable_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luav_settable_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_luav_settable_typedef r_luav_settable = (r_luav_settable_typedef)(x(0x01675AB0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luav_settable_typedef = int(luav_settable_CCV*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_luav_settable_typedef r_luav_settable = (r_luav_settable_typedef)(x(Adresses::luav_settable_addr));



	*/
	static constexpr std::uintptr_t luav_settable_addr = 0x01675AB0;
#define luav_settable_CCV __cdecl

	/*

	* luav_tonumber_Decompiled_Args: std::uintptr_t a1, int a2
	* luav_tonumber_TypeDef: int
	* luav_tonumber_Aob: Unable to generate aob.
	* luav_tonumber_ProtectedByRetcheck: false
	* luav_tonumber_IsApartOfLua: true
	* luav_tonumber_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luav_tonumber_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luav_tonumber_typedef r_luav_tonumber = (r_luav_tonumber_typedef)(x(0x01675C40));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luav_tonumber_typedef = int(luav_tonumber_CCV*)(std::uintptr_t a1, int a2);
	r_luav_tonumber_typedef r_luav_tonumber = (r_luav_tonumber_typedef)(x(Adresses::luav_tonumber_addr));



	*/
	static constexpr std::uintptr_t luav_tonumber_addr = 0x01675C40;
#define luav_tonumber_CCV __cdecl

	/*

	* luav_tostring_Decompiled_Args: std::uintptr_t a1, int a2
	* luav_tostring_TypeDef: int
	* luav_tostring_Aob: Unable to generate aob.
	* luav_tostring_ProtectedByRetcheck: false
	* luav_tostring_IsApartOfLua: true
	* luav_tostring_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luav_tostring_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_luav_tostring_typedef r_luav_tostring = (r_luav_tostring_typedef)(x(0x01675CA0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luav_tostring_typedef = int(luav_tostring_CCV*)(std::uintptr_t a1, int a2);
	r_luav_tostring_typedef r_luav_tostring = (r_luav_tostring_typedef)(x(Adresses::luav_tostring_addr));



	*/
	static constexpr std::uintptr_t luav_tostring_addr = 0x01675CA0;
#define luav_tostring_CCV __cdecl

	/*

	* luavm_compile_Decompiled_Args: int a1
	* luavm_compile_TypeDef: int
	* luavm_compile_Aob: Unable to generate aob.
	* luavm_compile_ProtectedByRetcheck: false
	* luavm_compile_IsApartOfLua: false
	* luavm_compile_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luavm_compile_typedef = int(__cdecl*)(int a1);
	r_luavm_compile_typedef r_luavm_compile = (r_luavm_compile_typedef)(x(0x006D1450));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luavm_compile_typedef = int(luavm_compile_CCV*)(int a1);
	r_luavm_compile_typedef r_luavm_compile = (r_luavm_compile_typedef)(x(Adresses::luavm_compile_addr));



	*/
	static constexpr std::uintptr_t luavm_compile_addr = 0x006D1450;
#define luavm_compile_CCV __cdecl

	/*

	* luavm_load_Decompiled_Args: int a1, int a2, int a3
	* luavm_load_TypeDef: int
	* luavm_load_Aob: Unable to generate aob.
	* luavm_load_ProtectedByRetcheck: false
	* luavm_load_IsApartOfLua: false
	* luavm_load_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luavm_load_typedef = int(__cdecl*)(int a1, int a2, int a3);
	r_luavm_load_typedef r_luavm_load = (r_luavm_load_typedef)(x(0x006D1990));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luavm_load_typedef = int(luavm_load_CCV*)(int a1, int a2, int a3);
	r_luavm_load_typedef r_luavm_load = (r_luavm_load_typedef)(x(Adresses::luavm_load_addr));



	*/
	static constexpr std::uintptr_t luavm_load_addr = 0x006D1990;
#define luavm_load_CCV __cdecl

	/*

	* luax_init_Decompiled_Args: int a1
	* luax_init_TypeDef: int
	* luax_init_Aob: Unable to generate aob.
	* luax_init_ProtectedByRetcheck: false
	* luax_init_IsApartOfLua: true
	* luax_init_Mask: ....
	* Usage (Not 100% Accurate):

	using r_luax_init_typedef = int(__cdecl*)(int a1);
	r_luax_init_typedef r_luax_init = (r_luax_init_typedef)(x(0x01675DB0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_luax_init_typedef = int(luax_init_CCV*)(int a1);
	r_luax_init_typedef r_luax_init = (r_luax_init_typedef)(x(Adresses::luax_init_addr));



	*/
	static constexpr std::uintptr_t luax_init_addr = 0x01675DB0;
#define luax_init_CCV __cdecl

	/*

	* mainposition_Decompiled_Args: std::uintptr_t a1, int a2
	* mainposition_TypeDef: int
	* mainposition_Aob: Unable to generate aob.
	* mainposition_ProtectedByRetcheck: false
	* mainposition_IsApartOfLua: true
	* mainposition_Mask: ....
	* Usage (Not 100% Accurate):

	using r_mainposition_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_mainposition_typedef r_mainposition = (r_mainposition_typedef)(x(0x01673C50));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_mainposition_typedef = int(mainposition_CCV*)(std::uintptr_t a1, int a2);
	r_mainposition_typedef r_mainposition = (r_mainposition_typedef)(x(Adresses::mainposition_addr));



	*/
	static constexpr std::uintptr_t mainposition_addr = 0x01673C50;
#define mainposition_CCV __cdecl

	/*

	* markmt_Decompiled_Args: int a1
	* markmt_TypeDef: int
	* markmt_Aob: Unable to generate aob.
	* markmt_ProtectedByRetcheck: false
	* markmt_IsApartOfLua: true
	* markmt_Mask: ....
	* Usage (Not 100% Accurate):

	using r_markmt_typedef = int(__thiscall*)(int a1);
	r_markmt_typedef r_markmt = (r_markmt_typedef)(x(0x01677F20));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_markmt_typedef = int(markmt_CCV*)(int a1);
	r_markmt_typedef r_markmt = (r_markmt_typedef)(x(Adresses::markmt_addr));



	*/
	static constexpr std::uintptr_t markmt_addr = 0x01677F20;
#define markmt_CCV __thiscall

	/*

	* markroot_Decompiled_Args: int a1
	* markroot_TypeDef: int
	* markroot_Aob: Unable to generate aob.
	* markroot_ProtectedByRetcheck: false
	* markroot_IsApartOfLua: true
	* markroot_Mask: ....
	* Usage (Not 100% Accurate):

	using r_markroot_typedef = int(__cdecl*)(int a1);
	r_markroot_typedef r_markroot = (r_markroot_typedef)(x(0x01671260));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_markroot_typedef = int(markroot_CCV*)(int a1);
	r_markroot_typedef r_markroot = (r_markroot_typedef)(x(Adresses::markroot_addr));



	*/
	static constexpr std::uintptr_t markroot_addr = 0x01671260;
#define markroot_CCV __cdecl

	/*

	* match_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* match_TypeDef: int
	* match_Aob: Unable to generate aob.
	* match_ProtectedByRetcheck: false
	* match_IsApartOfLua: true
	* match_Mask: ....
	* Usage (Not 100% Accurate):

	using r_match_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_match_typedef r_match = (r_match_typedef)(x(0x0166D9E0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_match_typedef = int(match_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_match_typedef r_match = (r_match_typedef)(x(Adresses::match_addr));



	*/
	static constexpr std::uintptr_t match_addr = 0x0166D9E0;
#define match_CCV __cdecl

	/*

	* newkey_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* newkey_TypeDef: int
	* newkey_Aob: Unable to generate aob.
	* newkey_ProtectedByRetcheck: false
	* newkey_IsApartOfLua: true
	* newkey_Mask: ....
	* Usage (Not 100% Accurate):

	using r_newkey_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_newkey_typedef r_newkey = (r_newkey_typedef)(x(0x01673DA0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_newkey_typedef = int(newkey_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_newkey_typedef r_newkey = (r_newkey_typedef)(x(Adresses::newkey_addr));



	*/
	static constexpr std::uintptr_t newkey_addr = 0x01673DA0;
#define newkey_CCV __cdecl

	/*

	* print_Decompiled_Args: int a1, int a2, int a3
	* print_TypeDef: int
	* print_Aob: Unable to generate aob.
	* print_ProtectedByRetcheck: false
	* print_IsApartOfLua: false
	* print_Mask: ....
	* Usage (Not 100% Accurate):

	using r_print_typedef = int(__cdecl*)(int a1, int a2, int a3);
	r_print_typedef r_print = (r_print_typedef)(x(0x00642A10));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_print_typedef = int(print_CCV*)(int a1, int a2, int a3);
	r_print_typedef r_print = (r_print_typedef)(x(Adresses::print_addr));



	*/
	static constexpr std::uintptr_t print_addr = 0x00642A10;
#define print_CCV __cdecl

	/*

	* push_captures_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* push_captures_TypeDef: int
	* push_captures_Aob: Unable to generate aob.
	* push_captures_ProtectedByRetcheck: false
	* push_captures_IsApartOfLua: true
	* push_captures_Mask: ....
	* Usage (Not 100% Accurate):

	using r_push_captures_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_push_captures_typedef r_push_captures = (r_push_captures_typedef)(x(0x0166E370));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_push_captures_typedef = int(push_captures_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_push_captures_typedef r_push_captures = (r_push_captures_typedef)(x(Adresses::push_captures_addr));



	*/
	static constexpr std::uintptr_t push_captures_addr = 0x0166E370;
#define push_captures_CCV __cdecl

	/*

	* push_instance_Decompiled_Args: int a1, int a2
	* push_instance_TypeDef: int
	* push_instance_Aob: Unable to generate aob.
	* push_instance_ProtectedByRetcheck: false
	* push_instance_IsApartOfLua: false
	* push_instance_Mask: ....
	* Usage (Not 100% Accurate):

	using r_push_instance_typedef = int(__cdecl*)(int a1, int a2);
	r_push_instance_typedef r_push_instance = (r_push_instance_typedef)(x(0x00857CF0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_push_instance_typedef = int(push_instance_CCV*)(int a1, int a2);
	r_push_instance_typedef r_push_instance = (r_push_instance_typedef)(x(Adresses::push_instance_addr));



	*/
	static constexpr std::uintptr_t push_instance_addr = 0x00857CF0;
#define push_instance_CCV __cdecl

	/*

	* rbx_get_property_Decompiled_Args: int a1, int a2
	* rbx_get_property_TypeDef: int
	* rbx_get_property_Aob: Unable to generate aob.
	* rbx_get_property_ProtectedByRetcheck: false
	* rbx_get_property_IsApartOfLua: false
	* rbx_get_property_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_get_property_typedef = int(__thiscall*)(int a1, int a2);
	r_rbx_get_property_typedef r_rbx_get_property = (r_rbx_get_property_typedef)(x(0x00855950));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_get_property_typedef = int(rbx_get_property_CCV*)(int a1, int a2);
	r_rbx_get_property_typedef r_rbx_get_property = (r_rbx_get_property_typedef)(x(Adresses::rbx_get_property_addr));



	*/
	static constexpr std::uintptr_t rbx_get_property_addr = 0x00855950;
#define rbx_get_property_CCV __thiscall

	/*

	* rbx_network_replicator_onreceive_Decompiled_Args: int a1, int a2, int a3
	* rbx_network_replicator_onreceive_TypeDef: int
	* rbx_network_replicator_onreceive_Aob: Unable to generate aob.
	* rbx_network_replicator_onreceive_ProtectedByRetcheck: false
	* rbx_network_replicator_onreceive_IsApartOfLua: false
	* rbx_network_replicator_onreceive_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_network_replicator_onreceive_typedef = int(__fastcall*)(int a1, int a2, int a3);
	r_rbx_network_replicator_onreceive_typedef r_rbx_network_replicator_onreceive = (r_rbx_network_replicator_onreceive_typedef)(x(0x014BE100));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_network_replicator_onreceive_typedef = int(rbx_network_replicator_onreceive_CCV*)(int a1, int a2, int a3);
	r_rbx_network_replicator_onreceive_typedef r_rbx_network_replicator_onreceive = (r_rbx_network_replicator_onreceive_typedef)(x(Adresses::rbx_network_replicator_onreceive_addr));



	*/
	static constexpr std::uintptr_t rbx_network_replicator_onreceive_addr = 0x014BE100;
#define rbx_network_replicator_onreceive_CCV __fastcall

	/*

	* rbx_network_physicssenders2_sendpacket_Decompiled_Args: int a1, int a2, int a3
	* rbx_network_physicssenders2_sendpacket_TypeDef: int
	* rbx_network_physicssenders2_sendpacket_Aob: Unable to generate aob.
	* rbx_network_physicssenders2_sendpacket_ProtectedByRetcheck: false
	* rbx_network_physicssenders2_sendpacket_IsApartOfLua: false
	* rbx_network_physicssenders2_sendpacket_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_network_physicssenders2_sendpacket_typedef = int(__cdecl*)(int a1, int a2, int a3);
	r_rbx_network_physicssenders2_sendpacket_typedef r_rbx_network_physicssenders2_sendpacket = (r_rbx_network_physicssenders2_sendpacket_typedef)(x(0x015404C0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_network_physicssenders2_sendpacket_typedef = int(rbx_network_physicssenders2_sendpacket_CCV*)(int a1, int a2, int a3);
	r_rbx_network_physicssenders2_sendpacket_typedef r_rbx_network_physicssenders2_sendpacket = (r_rbx_network_physicssenders2_sendpacket_typedef)(x(Adresses::rbx_network_physicssenders2_sendpacket_addr));



	*/
	static constexpr std::uintptr_t rbx_network_physicssenders2_sendpacket_addr = 0x015404C0;
#define rbx_network_physicssenders2_sendpacket_CCV __cdecl

	/*

	* rbx_disable_report_screenshot_gl2_Decompiled_Args:
	* rbx_disable_report_screenshot_gl2_TypeDef: int
	* rbx_disable_report_screenshot_gl2_Aob: Unable to generate aob.
	* rbx_disable_report_screenshot_gl2_ProtectedByRetcheck: false
	* rbx_disable_report_screenshot_gl2_IsApartOfLua: false
	* rbx_disable_report_screenshot_gl2_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_disable_report_screenshot_gl2_typedef = int(__cdecl*)();
	r_rbx_disable_report_screenshot_gl2_typedef r_rbx_disable_report_screenshot_gl2 = (r_rbx_disable_report_screenshot_gl2_typedef)(x(0x01288800));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_disable_report_screenshot_gl2_typedef = int(rbx_disable_report_screenshot_gl2_CCV*)();
	r_rbx_disable_report_screenshot_gl2_typedef r_rbx_disable_report_screenshot_gl2 = (r_rbx_disable_report_screenshot_gl2_typedef)(x(Adresses::rbx_disable_report_screenshot_gl2_addr));



	*/
	static constexpr std::uintptr_t rbx_disable_report_screenshot_gl2_addr = 0x01288800;
#define rbx_disable_report_screenshot_gl2_CCV __cdecl

	/*

	* rbx_disable_report_screenshot_gl_Decompiled_Args:
	* rbx_disable_report_screenshot_gl_TypeDef: int
	* rbx_disable_report_screenshot_gl_Aob: Unable to generate aob.
	* rbx_disable_report_screenshot_gl_ProtectedByRetcheck: false
	* rbx_disable_report_screenshot_gl_IsApartOfLua: false
	* rbx_disable_report_screenshot_gl_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_disable_report_screenshot_gl_typedef = int(__cdecl*)();
	r_rbx_disable_report_screenshot_gl_typedef r_rbx_disable_report_screenshot_gl = (r_rbx_disable_report_screenshot_gl_typedef)(x(0x012887E0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_disable_report_screenshot_gl_typedef = int(rbx_disable_report_screenshot_gl_CCV*)();
	r_rbx_disable_report_screenshot_gl_typedef r_rbx_disable_report_screenshot_gl = (r_rbx_disable_report_screenshot_gl_typedef)(x(Adresses::rbx_disable_report_screenshot_gl_addr));



	*/
	static constexpr std::uintptr_t rbx_disable_report_screenshot_gl_addr = 0x012887E0;
#define rbx_disable_report_screenshot_gl_CCV __cdecl

	/*

	* rbx_disable_report_screenshot_metal_Decompiled_Args:
	* rbx_disable_report_screenshot_metal_TypeDef: int
	* rbx_disable_report_screenshot_metal_Aob: Unable to generate aob.
	* rbx_disable_report_screenshot_metal_ProtectedByRetcheck: false
	* rbx_disable_report_screenshot_metal_IsApartOfLua: false
	* rbx_disable_report_screenshot_metal_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_disable_report_screenshot_metal_typedef = int(__cdecl*)();
	r_rbx_disable_report_screenshot_metal_typedef r_rbx_disable_report_screenshot_metal = (r_rbx_disable_report_screenshot_metal_typedef)(x(0x01288820));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_disable_report_screenshot_metal_typedef = int(rbx_disable_report_screenshot_metal_CCV*)();
	r_rbx_disable_report_screenshot_metal_typedef r_rbx_disable_report_screenshot_metal = (r_rbx_disable_report_screenshot_metal_typedef)(x(Adresses::rbx_disable_report_screenshot_metal_addr));



	*/
	static constexpr std::uintptr_t rbx_disable_report_screenshot_metal_addr = 0x01288820;
#define rbx_disable_report_screenshot_metal_CCV __cdecl

	/*

	* rbx_disable_report_screenshot_vulkan_Decompiled_Args:
	* rbx_disable_report_screenshot_vulkan_TypeDef: int
	* rbx_disable_report_screenshot_vulkan_Aob: Unable to generate aob.
	* rbx_disable_report_screenshot_vulkan_ProtectedByRetcheck: false
	* rbx_disable_report_screenshot_vulkan_IsApartOfLua: false
	* rbx_disable_report_screenshot_vulkan_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_disable_report_screenshot_vulkan_typedef = int(__cdecl*)();
	r_rbx_disable_report_screenshot_vulkan_typedef r_rbx_disable_report_screenshot_vulkan = (r_rbx_disable_report_screenshot_vulkan_typedef)(x(0x01288840));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_disable_report_screenshot_vulkan_typedef = int(rbx_disable_report_screenshot_vulkan_CCV*)();
	r_rbx_disable_report_screenshot_vulkan_typedef r_rbx_disable_report_screenshot_vulkan = (r_rbx_disable_report_screenshot_vulkan_typedef)(x(Adresses::rbx_disable_report_screenshot_vulkan_addr));



	*/
	static constexpr std::uintptr_t rbx_disable_report_screenshot_vulkan_addr = 0x01288840;
#define rbx_disable_report_screenshot_vulkan_CCV __cdecl

	/*

	* rbx_disable_report_screenshot_d3d_Decompiled_Args:
	* rbx_disable_report_screenshot_d3d_TypeDef: int
	* rbx_disable_report_screenshot_d3d_Aob: Unable to generate aob.
	* rbx_disable_report_screenshot_d3d_ProtectedByRetcheck: false
	* rbx_disable_report_screenshot_d3d_IsApartOfLua: false
	* rbx_disable_report_screenshot_d3d_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_disable_report_screenshot_d3d_typedef = int(__cdecl*)();
	r_rbx_disable_report_screenshot_d3d_typedef r_rbx_disable_report_screenshot_d3d = (r_rbx_disable_report_screenshot_d3d_typedef)(x(0x012887C0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_disable_report_screenshot_d3d_typedef = int(rbx_disable_report_screenshot_d3d_CCV*)();
	r_rbx_disable_report_screenshot_d3d_typedef r_rbx_disable_report_screenshot_d3d = (r_rbx_disable_report_screenshot_d3d_typedef)(x(Adresses::rbx_disable_report_screenshot_d3d_addr));



	*/
	static constexpr std::uintptr_t rbx_disable_report_screenshot_d3d_addr = 0x012887C0;
#define rbx_disable_report_screenshot_d3d_CCV __cdecl

	/*

	* rbx_setsimulationradius_Decompiled_Args: int a1, int a2
	* rbx_setsimulationradius_TypeDef: int
	* rbx_setsimulationradius_Aob: Unable to generate aob.
	* rbx_setsimulationradius_ProtectedByRetcheck: false
	* rbx_setsimulationradius_IsApartOfLua: false
	* rbx_setsimulationradius_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_setsimulationradius_typedef = int(__stdcall*)(int a1, int a2);
	r_rbx_setsimulationradius_typedef r_rbx_setsimulationradius = (r_rbx_setsimulationradius_typedef)(x(0x009516B0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_setsimulationradius_typedef = int(rbx_setsimulationradius_CCV*)(int a1, int a2);
	r_rbx_setsimulationradius_typedef r_rbx_setsimulationradius = (r_rbx_setsimulationradius_typedef)(x(Adresses::rbx_setsimulationradius_addr));



	*/
	static constexpr std::uintptr_t rbx_setsimulationradius_addr = 0x009516B0;
#define rbx_setsimulationradius_CCV __stdcall

	/*

	* rbx_network_clientreplicator_processtag_Decompiled_Args: int a1, int a2, int a3, int a4
	* rbx_network_clientreplicator_processtag_TypeDef: int
	* rbx_network_clientreplicator_processtag_Aob: Unable to generate aob.
	* rbx_network_clientreplicator_processtag_ProtectedByRetcheck: false
	* rbx_network_clientreplicator_processtag_IsApartOfLua: false
	* rbx_network_clientreplicator_processtag_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_network_clientreplicator_processtag_typedef = int(__stdcall*)(int a1, int a2, int a3, int a4);
	r_rbx_network_clientreplicator_processtag_typedef r_rbx_network_clientreplicator_processtag = (r_rbx_network_clientreplicator_processtag_typedef)(x(0x01494B10));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_network_clientreplicator_processtag_typedef = int(rbx_network_clientreplicator_processtag_CCV*)(int a1, int a2, int a3, int a4);
	r_rbx_network_clientreplicator_processtag_typedef r_rbx_network_clientreplicator_processtag = (r_rbx_network_clientreplicator_processtag_typedef)(x(Adresses::rbx_network_clientreplicator_processtag_addr));



	*/
	static constexpr std::uintptr_t rbx_network_clientreplicator_processtag_addr = 0x01494B10;
#define rbx_network_clientreplicator_processtag_CCV __stdcall

	/*

	* rbx_corescript_getluavmcorescriptfilepath_Decompiled_Args: int a1, int a2, const char* a3
	* rbx_corescript_getluavmcorescriptfilepath_TypeDef: int
	* rbx_corescript_getluavmcorescriptfilepath_Aob: Unable to generate aob.
	* rbx_corescript_getluavmcorescriptfilepath_ProtectedByRetcheck: false
	* rbx_corescript_getluavmcorescriptfilepath_IsApartOfLua: false
	* rbx_corescript_getluavmcorescriptfilepath_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_corescript_getluavmcorescriptfilepath_typedef = int(__cdecl*)(int a1, int a2, const char* a3);
	r_rbx_corescript_getluavmcorescriptfilepath_typedef r_rbx_corescript_getluavmcorescriptfilepath = (r_rbx_corescript_getluavmcorescriptfilepath_typedef)(x(0x00B50180));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_corescript_getluavmcorescriptfilepath_typedef = int(rbx_corescript_getluavmcorescriptfilepath_CCV*)(int a1, int a2, const char* a3);
	r_rbx_corescript_getluavmcorescriptfilepath_typedef r_rbx_corescript_getluavmcorescriptfilepath = (r_rbx_corescript_getluavmcorescriptfilepath_typedef)(x(Adresses::rbx_corescript_getluavmcorescriptfilepath_addr));



	*/
	static constexpr std::uintptr_t rbx_corescript_getluavmcorescriptfilepath_addr = 0x00B50180;
#define rbx_corescript_getluavmcorescriptfilepath_CCV __cdecl

	/*

	* rbx_datastore_runtransformfunction_Decompiled_Args:
	* rbx_datastore_runtransformfunction_TypeDef: int
	* rbx_datastore_runtransformfunction_Aob: Unable to generate aob.
	* rbx_datastore_runtransformfunction_ProtectedByRetcheck: false
	* rbx_datastore_runtransformfunction_IsApartOfLua: false
	* rbx_datastore_runtransformfunction_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_datastore_runtransformfunction_typedef = int(__stdcall*)();
	r_rbx_datastore_runtransformfunction_typedef r_rbx_datastore_runtransformfunction = (r_rbx_datastore_runtransformfunction_typedef)(x(0x00E0C080));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_datastore_runtransformfunction_typedef = int(rbx_datastore_runtransformfunction_CCV*)();
	r_rbx_datastore_runtransformfunction_typedef r_rbx_datastore_runtransformfunction = (r_rbx_datastore_runtransformfunction_typedef)(x(Adresses::rbx_datastore_runtransformfunction_addr));



	*/
	static constexpr std::uintptr_t rbx_datastore_runtransformfunction_addr = 0x00E0C080;
#define rbx_datastore_runtransformfunction_CCV __stdcall

	/*

	* rbx_datamodel_dodatamodelsetup_Decompiled_Args: int a1, int a2, int a3, int a4, int a5
	* rbx_datamodel_dodatamodelsetup_TypeDef: int
	* rbx_datamodel_dodatamodelsetup_Aob: Unable to generate aob.
	* rbx_datamodel_dodatamodelsetup_ProtectedByRetcheck: false
	* rbx_datamodel_dodatamodelsetup_IsApartOfLua: false
	* rbx_datamodel_dodatamodelsetup_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_datamodel_dodatamodelsetup_typedef = int(__cdecl*)(int a1, int a2, int a3, int a4, int a5);
	r_rbx_datamodel_dodatamodelsetup_typedef r_rbx_datamodel_dodatamodelsetup = (r_rbx_datamodel_dodatamodelsetup_typedef)(x(0x00704940));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_datamodel_dodatamodelsetup_typedef = int(rbx_datamodel_dodatamodelsetup_CCV*)(int a1, int a2, int a3, int a4, int a5);
	r_rbx_datamodel_dodatamodelsetup_typedef r_rbx_datamodel_dodatamodelsetup = (r_rbx_datamodel_dodatamodelsetup_typedef)(x(Adresses::rbx_datamodel_dodatamodelsetup_addr));



	*/
	static constexpr std::uintptr_t rbx_datamodel_dodatamodelsetup_addr = 0x00704940;
#define rbx_datamodel_dodatamodelsetup_CCV __cdecl

	/*

	* rbx_debugvalueconverte_userdatatostring_Decompiled_Args: int a1, int a2, int a3
	* rbx_debugvalueconverte_userdatatostring_TypeDef: int
	* rbx_debugvalueconverte_userdatatostring_Aob: Unable to generate aob.
	* rbx_debugvalueconverte_userdatatostring_ProtectedByRetcheck: false
	* rbx_debugvalueconverte_userdatatostring_IsApartOfLua: false
	* rbx_debugvalueconverte_userdatatostring_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_debugvalueconverte_userdatatostring_typedef = int(__cdecl*)(int a1, int a2, int a3);
	r_rbx_debugvalueconverte_userdatatostring_typedef r_rbx_debugvalueconverte_userdatatostring = (r_rbx_debugvalueconverte_userdatatostring_typedef)(x(0x00C981E0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_debugvalueconverte_userdatatostring_typedef = int(rbx_debugvalueconverte_userdatatostring_CCV*)(int a1, int a2, int a3);
	r_rbx_debugvalueconverte_userdatatostring_typedef r_rbx_debugvalueconverte_userdatatostring = (r_rbx_debugvalueconverte_userdatatostring_typedef)(x(Adresses::rbx_debugvalueconverte_userdatatostring_addr));



	*/
	static constexpr std::uintptr_t rbx_debugvalueconverte_userdatatostring_addr = 0x00C981E0;
#define rbx_debugvalueconverte_userdatatostring_CCV __cdecl

	/*

	* rbx_deu_uploadcrasheventfile_Decompiled_Args: int a1
	* rbx_deu_uploadcrasheventfile_TypeDef: int
	* rbx_deu_uploadcrasheventfile_Aob: Unable to generate aob.
	* rbx_deu_uploadcrasheventfile_ProtectedByRetcheck: false
	* rbx_deu_uploadcrasheventfile_IsApartOfLua: false
	* rbx_deu_uploadcrasheventfile_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_deu_uploadcrasheventfile_typedef = int(__cdecl*)(int a1);
	r_rbx_deu_uploadcrasheventfile_typedef r_rbx_deu_uploadcrasheventfile = (r_rbx_deu_uploadcrasheventfile_typedef)(x(0x006269E0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_deu_uploadcrasheventfile_typedef = int(rbx_deu_uploadcrasheventfile_CCV*)(int a1);
	r_rbx_deu_uploadcrasheventfile_typedef r_rbx_deu_uploadcrasheventfile = (r_rbx_deu_uploadcrasheventfile_typedef)(x(Adresses::rbx_deu_uploadcrasheventfile_addr));



	*/
	static constexpr std::uintptr_t rbx_deu_uploadcrasheventfile_addr = 0x006269E0;
#define rbx_deu_uploadcrasheventfile_CCV __cdecl

	/*

	* rbx_lm_reporterror_Decompiled_Args: int a1, int a2
	* rbx_lm_reporterror_TypeDef: int
	* rbx_lm_reporterror_Aob: Unable to generate aob.
	* rbx_lm_reporterror_ProtectedByRetcheck: false
	* rbx_lm_reporterror_IsApartOfLua: false
	* rbx_lm_reporterror_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_lm_reporterror_typedef = int(__cdecl*)(int a1, int a2);
	r_rbx_lm_reporterror_typedef r_rbx_lm_reporterror = (r_rbx_lm_reporterror_typedef)(x(0x0062BB80));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_lm_reporterror_typedef = int(rbx_lm_reporterror_CCV*)(int a1, int a2);
	r_rbx_lm_reporterror_typedef r_rbx_lm_reporterror = (r_rbx_lm_reporterror_typedef)(x(Adresses::rbx_lm_reporterror_addr));



	*/
	static constexpr std::uintptr_t rbx_lm_reporterror_addr = 0x0062BB80;
#define rbx_lm_reporterror_CCV __cdecl

	/*

	* rbx_lua_dumpthreadrefcounts_Decompiled_Args:
	* rbx_lua_dumpthreadrefcounts_TypeDef: int
	* rbx_lua_dumpthreadrefcounts_Aob: Unable to generate aob.
	* rbx_lua_dumpthreadrefcounts_ProtectedByRetcheck: false
	* rbx_lua_dumpthreadrefcounts_IsApartOfLua: false
	* rbx_lua_dumpthreadrefcounts_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_lua_dumpthreadrefcounts_typedef = int(__cdecl*)();
	r_rbx_lua_dumpthreadrefcounts_typedef r_rbx_lua_dumpthreadrefcounts = (r_rbx_lua_dumpthreadrefcounts_typedef)(x(0x008230F0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_lua_dumpthreadrefcounts_typedef = int(rbx_lua_dumpthreadrefcounts_CCV*)();
	r_rbx_lua_dumpthreadrefcounts_typedef r_rbx_lua_dumpthreadrefcounts = (r_rbx_lua_dumpthreadrefcounts_typedef)(x(Adresses::rbx_lua_dumpthreadrefcounts_addr));



	*/
	static constexpr std::uintptr_t rbx_lua_dumpthreadrefcounts_addr = 0x008230F0;
#define rbx_lua_dumpthreadrefcounts_CCV __cdecl

	/*

	* rbx_network_replicator_processdeserializedpacket_Decompiled_Args: int a1, int a2
	* rbx_network_replicator_processdeserializedpacket_TypeDef: int
	* rbx_network_replicator_processdeserializedpacket_Aob: Unable to generate aob.
	* rbx_network_replicator_processdeserializedpacket_ProtectedByRetcheck: false
	* rbx_network_replicator_processdeserializedpacket_IsApartOfLua: false
	* rbx_network_replicator_processdeserializedpacket_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_network_replicator_processdeserializedpacket_typedef = int(__thiscall*)(int a1, int a2);
	r_rbx_network_replicator_processdeserializedpacket_typedef r_rbx_network_replicator_processdeserializedpacket = (r_rbx_network_replicator_processdeserializedpacket_typedef)(x(0x014C1DC0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_network_replicator_processdeserializedpacket_typedef = int(rbx_network_replicator_processdeserializedpacket_CCV*)(int a1, int a2);
	r_rbx_network_replicator_processdeserializedpacket_typedef r_rbx_network_replicator_processdeserializedpacket = (r_rbx_network_replicator_processdeserializedpacket_typedef)(x(Adresses::rbx_network_replicator_processdeserializedpacket_addr));



	*/
	static constexpr std::uintptr_t rbx_network_replicator_processdeserializedpacket_addr = 0x014C1DC0;
#define rbx_network_replicator_processdeserializedpacket_CCV __thiscall

	/*

	* rbx_network_server_registerlegalscript_Decompiled_Args: int a1, int a2
	* rbx_network_server_registerlegalscript_TypeDef: int
	* rbx_network_server_registerlegalscript_Aob: Unable to generate aob.
	* rbx_network_server_registerlegalscript_ProtectedByRetcheck: false
	* rbx_network_server_registerlegalscript_IsApartOfLua: false
	* rbx_network_server_registerlegalscript_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_network_server_registerlegalscript_typedef = int(__thiscall*)(int a1, int a2);
	r_rbx_network_server_registerlegalscript_typedef r_rbx_network_server_registerlegalscript = (r_rbx_network_server_registerlegalscript_typedef)(x(0x014EDA10));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_network_server_registerlegalscript_typedef = int(rbx_network_server_registerlegalscript_CCV*)(int a1, int a2);
	r_rbx_network_server_registerlegalscript_typedef r_rbx_network_server_registerlegalscript = (r_rbx_network_server_registerlegalscript_typedef)(x(Adresses::rbx_network_server_registerlegalscript_addr));



	*/
	static constexpr std::uintptr_t rbx_network_server_registerlegalscript_addr = 0x014EDA10;
#define rbx_network_server_registerlegalscript_CCV __thiscall

	/*

	* rbx_rcr_processexception_Decompiled_Args:
	* rbx_rcr_processexception_TypeDef: int
	* rbx_rcr_processexception_Aob: Unable to generate aob.
	* rbx_rcr_processexception_ProtectedByRetcheck: false
	* rbx_rcr_processexception_IsApartOfLua: false
	* rbx_rcr_processexception_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_rcr_processexception_typedef = int(__stdcall*)();
	r_rbx_rcr_processexception_typedef r_rbx_rcr_processexception = (r_rbx_rcr_processexception_typedef)(x(0x0062BA30));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_rcr_processexception_typedef = int(rbx_rcr_processexception_CCV*)();
	r_rbx_rcr_processexception_typedef r_rbx_rcr_processexception = (r_rbx_rcr_processexception_typedef)(x(Adresses::rbx_rcr_processexception_addr));



	*/
	static constexpr std::uintptr_t rbx_rcr_processexception_addr = 0x0062BA30;
#define rbx_rcr_processexception_CCV __stdcall

	/*

	* raknet_rakpeer_runupdatecycle_Decompiled_Args: int a1, int a2, int a3, int a4
	* raknet_rakpeer_runupdatecycle_TypeDef: int
	* raknet_rakpeer_runupdatecycle_Aob: Unable to generate aob.
	* raknet_rakpeer_runupdatecycle_ProtectedByRetcheck: false
	* raknet_rakpeer_runupdatecycle_IsApartOfLua: false
	* raknet_rakpeer_runupdatecycle_Mask: ....
	* Usage (Not 100% Accurate):

	using r_raknet_rakpeer_runupdatecycle_typedef = int(__thiscall*)(int a1, int a2, int a3, int a4);
	r_raknet_rakpeer_runupdatecycle_typedef r_raknet_rakpeer_runupdatecycle = (r_raknet_rakpeer_runupdatecycle_typedef)(x(0x014F45D0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_raknet_rakpeer_runupdatecycle_typedef = int(raknet_rakpeer_runupdatecycle_CCV*)(int a1, int a2, int a3, int a4);
	r_raknet_rakpeer_runupdatecycle_typedef r_raknet_rakpeer_runupdatecycle = (r_raknet_rakpeer_runupdatecycle_typedef)(x(Adresses::raknet_rakpeer_runupdatecycle_addr));



	*/
	static constexpr std::uintptr_t raknet_rakpeer_runupdatecycle_addr = 0x014F45D0;
#define raknet_rakpeer_runupdatecycle_CCV __thiscall

	/*

	* rbx_sc_closestate_Decompiled_Args: int a1
	* rbx_sc_closestate_TypeDef: int
	* rbx_sc_closestate_Aob: Unable to generate aob.
	* rbx_sc_closestate_ProtectedByRetcheck: false
	* rbx_sc_closestate_IsApartOfLua: false
	* rbx_sc_closestate_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_sc_closestate_typedef = int(__stdcall*)(int a1);
	r_rbx_sc_closestate_typedef r_rbx_sc_closestate = (r_rbx_sc_closestate_typedef)(x(0x007B4AF0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_sc_closestate_typedef = int(rbx_sc_closestate_CCV*)(int a1);
	r_rbx_sc_closestate_typedef r_rbx_sc_closestate = (r_rbx_sc_closestate_typedef)(x(Adresses::rbx_sc_closestate_addr));



	*/
	static constexpr std::uintptr_t rbx_sc_closestate_addr = 0x007B4AF0;
#define rbx_sc_closestate_CCV __stdcall

	/*

	* rbx_taskscheduler_getlastcycletimestamp_Decompiled_Args: int a1
	* rbx_taskscheduler_getlastcycletimestamp_TypeDef: int
	* rbx_taskscheduler_getlastcycletimestamp_Aob: Unable to generate aob.
	* rbx_taskscheduler_getlastcycletimestamp_ProtectedByRetcheck: false
	* rbx_taskscheduler_getlastcycletimestamp_IsApartOfLua: false
	* rbx_taskscheduler_getlastcycletimestamp_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_taskscheduler_getlastcycletimestamp_typedef = int(__cdecl*)(int a1);
	r_rbx_taskscheduler_getlastcycletimestamp_typedef r_rbx_taskscheduler_getlastcycletimestamp = (r_rbx_taskscheduler_getlastcycletimestamp_typedef)(x(0x01106570));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_taskscheduler_getlastcycletimestamp_typedef = int(rbx_taskscheduler_getlastcycletimestamp_CCV*)(int a1);
	r_rbx_taskscheduler_getlastcycletimestamp_typedef r_rbx_taskscheduler_getlastcycletimestamp = (r_rbx_taskscheduler_getlastcycletimestamp_typedef)(x(Adresses::rbx_taskscheduler_getlastcycletimestamp_addr));



	*/
	static constexpr std::uintptr_t rbx_taskscheduler_getlastcycletimestamp_addr = 0x01106570;
#define rbx_taskscheduler_getlastcycletimestamp_CCV __cdecl

	/*

	* rbx_lua_bridge_on_index_Decompiled_Args: int a1
	* rbx_lua_bridge_on_index_TypeDef: int
	* rbx_lua_bridge_on_index_Aob: Unable to generate aob.
	* rbx_lua_bridge_on_index_ProtectedByRetcheck: false
	* rbx_lua_bridge_on_index_IsApartOfLua: false
	* rbx_lua_bridge_on_index_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_lua_bridge_on_index_typedef = int(__cdecl*)(int a1);
	r_rbx_lua_bridge_on_index_typedef r_rbx_lua_bridge_on_index = (r_rbx_lua_bridge_on_index_typedef)(x(0x008577F0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_lua_bridge_on_index_typedef = int(rbx_lua_bridge_on_index_CCV*)(int a1);
	r_rbx_lua_bridge_on_index_typedef r_rbx_lua_bridge_on_index = (r_rbx_lua_bridge_on_index_typedef)(x(Adresses::rbx_lua_bridge_on_index_addr));



	*/
	static constexpr std::uintptr_t rbx_lua_bridge_on_index_addr = 0x008577F0;
#define rbx_lua_bridge_on_index_CCV __cdecl

	/*

	* rbx_security_context_current_Decompiled_Args: int a1
	* rbx_security_context_current_TypeDef: int
	* rbx_security_context_current_Aob: Unable to generate aob.
	* rbx_security_context_current_ProtectedByRetcheck: false
	* rbx_security_context_current_IsApartOfLua: false
	* rbx_security_context_current_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_security_context_current_typedef = int(__thiscall*)(int a1);
	r_rbx_security_context_current_typedef r_rbx_security_context_current = (r_rbx_security_context_current_typedef)(x(0x0189EA80));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_security_context_current_typedef = int(rbx_security_context_current_CCV*)(int a1);
	r_rbx_security_context_current_typedef r_rbx_security_context_current = (r_rbx_security_context_current_typedef)(x(Adresses::rbx_security_context_current_addr));



	*/
	static constexpr std::uintptr_t rbx_security_context_current_addr = 0x0189EA80;
#define rbx_security_context_current_CCV __thiscall

	/*

	* rbx_security_impersonator_impersonator_Decompiled_Args:
	* rbx_security_impersonator_impersonator_TypeDef: int
	* rbx_security_impersonator_impersonator_Aob: Unable to generate aob.
	* rbx_security_impersonator_impersonator_ProtectedByRetcheck: false
	* rbx_security_impersonator_impersonator_IsApartOfLua: false
	* rbx_security_impersonator_impersonator_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_security_impersonator_impersonator_typedef = int(__stdcall*)();
	r_rbx_security_impersonator_impersonator_typedef r_rbx_security_impersonator_impersonator = (r_rbx_security_impersonator_impersonator_typedef)(x(0x0189E9B0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_security_impersonator_impersonator_typedef = int(rbx_security_impersonator_impersonator_CCV*)();
	r_rbx_security_impersonator_impersonator_typedef r_rbx_security_impersonator_impersonator = (r_rbx_security_impersonator_impersonator_typedef)(x(Adresses::rbx_security_impersonator_impersonator_addr));



	*/
	static constexpr std::uintptr_t rbx_security_impersonator_impersonator_addr = 0x0189E9B0;
#define rbx_security_impersonator_impersonator_CCV __stdcall

	/*

	* rbx_datamodel_getjobsinfo_Decompiled_Args: int a1, int a2
	* rbx_datamodel_getjobsinfo_TypeDef: int
	* rbx_datamodel_getjobsinfo_Aob: Unable to generate aob.
	* rbx_datamodel_getjobsinfo_ProtectedByRetcheck: false
	* rbx_datamodel_getjobsinfo_IsApartOfLua: false
	* rbx_datamodel_getjobsinfo_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_datamodel_getjobsinfo_typedef = int(__thiscall*)(int a1, int a2);
	r_rbx_datamodel_getjobsinfo_typedef r_rbx_datamodel_getjobsinfo = (r_rbx_datamodel_getjobsinfo_typedef)(x(0x00708790));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_datamodel_getjobsinfo_typedef = int(rbx_datamodel_getjobsinfo_CCV*)(int a1, int a2);
	r_rbx_datamodel_getjobsinfo_typedef r_rbx_datamodel_getjobsinfo = (r_rbx_datamodel_getjobsinfo_typedef)(x(Adresses::rbx_datamodel_getjobsinfo_addr));



	*/
	static constexpr std::uintptr_t rbx_datamodel_getjobsinfo_addr = 0x00708790;
#define rbx_datamodel_getjobsinfo_CCV __thiscall

	/*

	* rbx_sc_crash_Decompiled_Args: int a1
	* rbx_sc_crash_TypeDef: int
	* rbx_sc_crash_Aob: Unable to generate aob.
	* rbx_sc_crash_ProtectedByRetcheck: false
	* rbx_sc_crash_IsApartOfLua: false
	* rbx_sc_crash_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_sc_crash_typedef = int(__cdecl*)(int a1);
	r_rbx_sc_crash_typedef r_rbx_sc_crash = (r_rbx_sc_crash_typedef)(x(0x007B5740));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_sc_crash_typedef = int(rbx_sc_crash_CCV*)(int a1);
	r_rbx_sc_crash_typedef r_rbx_sc_crash = (r_rbx_sc_crash_typedef)(x(Adresses::rbx_sc_crash_addr));



	*/
	static constexpr std::uintptr_t rbx_sc_crash_addr = 0x007B5740;
#define rbx_sc_crash_CCV __cdecl

	/*

	* rbx_sc_interrupt_Decompiled_Args: int a1
	* rbx_sc_interrupt_TypeDef: int
	* rbx_sc_interrupt_Aob: Unable to generate aob.
	* rbx_sc_interrupt_ProtectedByRetcheck: false
	* rbx_sc_interrupt_IsApartOfLua: false
	* rbx_sc_interrupt_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_sc_interrupt_typedef = int(__cdecl*)(int a1);
	r_rbx_sc_interrupt_typedef r_rbx_sc_interrupt = (r_rbx_sc_interrupt_typedef)(x(0x007BB8A0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_sc_interrupt_typedef = int(rbx_sc_interrupt_CCV*)(int a1);
	r_rbx_sc_interrupt_typedef r_rbx_sc_interrupt = (r_rbx_sc_interrupt_typedef)(x(Adresses::rbx_sc_interrupt_addr));



	*/
	static constexpr std::uintptr_t rbx_sc_interrupt_addr = 0x007BB8A0;
#define rbx_sc_interrupt_CCV __cdecl

	/*

	* rbx_sc_loadstring_Decompiled_Args: int a1
	* rbx_sc_loadstring_TypeDef: int
	* rbx_sc_loadstring_Aob: Unable to generate aob.
	* rbx_sc_loadstring_ProtectedByRetcheck: false
	* rbx_sc_loadstring_IsApartOfLua: false
	* rbx_sc_loadstring_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_sc_loadstring_typedef = int(__cdecl*)(int a1);
	r_rbx_sc_loadstring_typedef r_rbx_sc_loadstring = (r_rbx_sc_loadstring_typedef)(x(0x007BC4E0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_sc_loadstring_typedef = int(rbx_sc_loadstring_CCV*)(int a1);
	r_rbx_sc_loadstring_typedef r_rbx_sc_loadstring = (r_rbx_sc_loadstring_typedef)(x(Adresses::rbx_sc_loadstring_addr));



	*/
	static constexpr std::uintptr_t rbx_sc_loadstring_addr = 0x007BC4E0;
#define rbx_sc_loadstring_CCV __cdecl

	/*

	* rbx_sc_scriptcontext_Decompiled_Args: int a1
	* rbx_sc_scriptcontext_TypeDef: int
	* rbx_sc_scriptcontext_Aob: Unable to generate aob.
	* rbx_sc_scriptcontext_ProtectedByRetcheck: false
	* rbx_sc_scriptcontext_IsApartOfLua: false
	* rbx_sc_scriptcontext_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_sc_scriptcontext_typedef = int(__thiscall*)(int a1);
	r_rbx_sc_scriptcontext_typedef r_rbx_sc_scriptcontext = (r_rbx_sc_scriptcontext_typedef)(x(0x007AD000));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_sc_scriptcontext_typedef = int(rbx_sc_scriptcontext_CCV*)(int a1);
	r_rbx_sc_scriptcontext_typedef r_rbx_sc_scriptcontext = (r_rbx_sc_scriptcontext_typedef)(x(Adresses::rbx_sc_scriptcontext_addr));



	*/
	static constexpr std::uintptr_t rbx_sc_scriptcontext_addr = 0x007AD000;
#define rbx_sc_scriptcontext_CCV __thiscall

	/*

	* rbx_sc_startscript_Decompiled_Args:
	* rbx_sc_startscript_TypeDef: int
	* rbx_sc_startscript_Aob: Unable to generate aob.
	* rbx_sc_startscript_ProtectedByRetcheck: false
	* rbx_sc_startscript_IsApartOfLua: false
	* rbx_sc_startscript_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_sc_startscript_typedef = int(__stdcall*)();
	r_rbx_sc_startscript_typedef r_rbx_sc_startscript = (r_rbx_sc_startscript_typedef)(x(0x007C99A0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_sc_startscript_typedef = int(rbx_sc_startscript_CCV*)();
	r_rbx_sc_startscript_typedef r_rbx_sc_startscript = (r_rbx_sc_startscript_typedef)(x(Adresses::rbx_sc_startscript_addr));



	*/
	static constexpr std::uintptr_t rbx_sc_startscript_addr = 0x007C99A0;
#define rbx_sc_startscript_CCV __stdcall

	/*

	* rbx_sc_watchdog_Decompiled_Args: int a1
	* rbx_sc_watchdog_TypeDef: int
	* rbx_sc_watchdog_Aob: Unable to generate aob.
	* rbx_sc_watchdog_ProtectedByRetcheck: false
	* rbx_sc_watchdog_IsApartOfLua: false
	* rbx_sc_watchdog_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_sc_watchdog_typedef = int(__thiscall*)(int a1);
	r_rbx_sc_watchdog_typedef r_rbx_sc_watchdog = (r_rbx_sc_watchdog_typedef)(x(0x007CBA30));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_sc_watchdog_typedef = int(rbx_sc_watchdog_CCV*)(int a1);
	r_rbx_sc_watchdog_typedef r_rbx_sc_watchdog = (r_rbx_sc_watchdog_typedef)(x(Adresses::rbx_sc_watchdog_addr));



	*/
	static constexpr std::uintptr_t rbx_sc_watchdog_addr = 0x007CBA30;
#define rbx_sc_watchdog_CCV __thiscall

	/*

	* rbx_scriptcontext_getcontext_Decompiled_Args: int a1
	* rbx_scriptcontext_getcontext_TypeDef: int
	* rbx_scriptcontext_getcontext_Aob: Unable to generate aob.
	* rbx_scriptcontext_getcontext_ProtectedByRetcheck: false
	* rbx_scriptcontext_getcontext_IsApartOfLua: false
	* rbx_scriptcontext_getcontext_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_scriptcontext_getcontext_typedef = int(__cdecl*)(int a1);
	r_rbx_scriptcontext_getcontext_typedef r_rbx_scriptcontext_getcontext = (r_rbx_scriptcontext_getcontext_typedef)(x(0x007D5740));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_scriptcontext_getcontext_typedef = int(rbx_scriptcontext_getcontext_CCV*)(int a1);
	r_rbx_scriptcontext_getcontext_typedef r_rbx_scriptcontext_getcontext = (r_rbx_scriptcontext_getcontext_typedef)(x(Adresses::rbx_scriptcontext_getcontext_addr));



	*/
	static constexpr std::uintptr_t rbx_scriptcontext_getcontext_addr = 0x007D5740;
#define rbx_scriptcontext_getcontext_CCV __cdecl

	/*

	* rbx_scriptcontext_debuggermanager_Decompiled_Args: int a1
	* rbx_scriptcontext_debuggermanager_TypeDef: int
	* rbx_scriptcontext_debuggermanager_Aob: Unable to generate aob.
	* rbx_scriptcontext_debuggermanager_ProtectedByRetcheck: false
	* rbx_scriptcontext_debuggermanager_IsApartOfLua: false
	* rbx_scriptcontext_debuggermanager_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_scriptcontext_debuggermanager_typedef = int(__cdecl*)(int a1);
	r_rbx_scriptcontext_debuggermanager_typedef r_rbx_scriptcontext_debuggermanager = (r_rbx_scriptcontext_debuggermanager_typedef)(x(0x007B5F10));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_scriptcontext_debuggermanager_typedef = int(rbx_scriptcontext_debuggermanager_CCV*)(int a1);
	r_rbx_scriptcontext_debuggermanager_typedef r_rbx_scriptcontext_debuggermanager = (r_rbx_scriptcontext_debuggermanager_typedef)(x(Adresses::rbx_scriptcontext_debuggermanager_addr));



	*/
	static constexpr std::uintptr_t rbx_scriptcontext_debuggermanager_addr = 0x007B5F10;
#define rbx_scriptcontext_debuggermanager_CCV __cdecl

	/*

	* rbx_scriptcontext_pluginmanager_Decompiled_Args: int a1
	* rbx_scriptcontext_pluginmanager_TypeDef: int
	* rbx_scriptcontext_pluginmanager_Aob: Unable to generate aob.
	* rbx_scriptcontext_pluginmanager_ProtectedByRetcheck: false
	* rbx_scriptcontext_pluginmanager_IsApartOfLua: false
	* rbx_scriptcontext_pluginmanager_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_scriptcontext_pluginmanager_typedef = int(__cdecl*)(int a1);
	r_rbx_scriptcontext_pluginmanager_typedef r_rbx_scriptcontext_pluginmanager = (r_rbx_scriptcontext_pluginmanager_typedef)(x(0x007BF950));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_scriptcontext_pluginmanager_typedef = int(rbx_scriptcontext_pluginmanager_CCV*)(int a1);
	r_rbx_scriptcontext_pluginmanager_typedef r_rbx_scriptcontext_pluginmanager = (r_rbx_scriptcontext_pluginmanager_typedef)(x(Adresses::rbx_scriptcontext_pluginmanager_addr));



	*/
	static constexpr std::uintptr_t rbx_scriptcontext_pluginmanager_addr = 0x007BF950;
#define rbx_scriptcontext_pluginmanager_CCV __cdecl

	/*

	* rbx_scriptcontext_usersettings_Decompiled_Args: int a1
	* rbx_scriptcontext_usersettings_TypeDef: int
	* rbx_scriptcontext_usersettings_Aob: Unable to generate aob.
	* rbx_scriptcontext_usersettings_ProtectedByRetcheck: false
	* rbx_scriptcontext_usersettings_IsApartOfLua: false
	* rbx_scriptcontext_usersettings_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_scriptcontext_usersettings_typedef = int(__cdecl*)(int a1);
	r_rbx_scriptcontext_usersettings_typedef r_rbx_scriptcontext_usersettings = (r_rbx_scriptcontext_usersettings_typedef)(x(0x007CB6F0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_scriptcontext_usersettings_typedef = int(rbx_scriptcontext_usersettings_CCV*)(int a1);
	r_rbx_scriptcontext_usersettings_typedef r_rbx_scriptcontext_usersettings = (r_rbx_scriptcontext_usersettings_typedef)(x(Adresses::rbx_scriptcontext_usersettings_addr));



	*/
	static constexpr std::uintptr_t rbx_scriptcontext_usersettings_addr = 0x007CB6F0;
#define rbx_scriptcontext_usersettings_CCV __cdecl

	/*

	* rbx_scriptcontext_settings_Decompiled_Args: int a1
	* rbx_scriptcontext_settings_TypeDef: int
	* rbx_scriptcontext_settings_Aob: Unable to generate aob.
	* rbx_scriptcontext_settings_ProtectedByRetcheck: false
	* rbx_scriptcontext_settings_IsApartOfLua: false
	* rbx_scriptcontext_settings_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_scriptcontext_settings_typedef = int(__cdecl*)(int a1);
	r_rbx_scriptcontext_settings_typedef r_rbx_scriptcontext_settings = (r_rbx_scriptcontext_settings_typedef)(x(0x007C8BA0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_scriptcontext_settings_typedef = int(rbx_scriptcontext_settings_CCV*)(int a1);
	r_rbx_scriptcontext_settings_typedef r_rbx_scriptcontext_settings = (r_rbx_scriptcontext_settings_typedef)(x(Adresses::rbx_scriptcontext_settings_addr));



	*/
	static constexpr std::uintptr_t rbx_scriptcontext_settings_addr = 0x007C8BA0;
#define rbx_scriptcontext_settings_CCV __cdecl

	/*

	* rbx_scriptcontext_getglobalstate_Decompiled_Args: int a1
	* rbx_scriptcontext_getglobalstate_TypeDef: int
	* rbx_scriptcontext_getglobalstate_Aob: Unable to generate aob.
	* rbx_scriptcontext_getglobalstate_ProtectedByRetcheck: false
	* rbx_scriptcontext_getglobalstate_IsApartOfLua: false
	* rbx_scriptcontext_getglobalstate_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_scriptcontext_getglobalstate_typedef = int(__cdecl*)(int a1);
	r_rbx_scriptcontext_getglobalstate_typedef r_rbx_scriptcontext_getglobalstate = (r_rbx_scriptcontext_getglobalstate_typedef)(x(0x007B9960));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_scriptcontext_getglobalstate_typedef = int(rbx_scriptcontext_getglobalstate_CCV*)(int a1);
	r_rbx_scriptcontext_getglobalstate_typedef r_rbx_scriptcontext_getglobalstate = (r_rbx_scriptcontext_getglobalstate_typedef)(x(Adresses::rbx_scriptcontext_getglobalstate_addr));



	*/
	static constexpr std::uintptr_t rbx_scriptcontext_getglobalstate_addr = 0x007B9960;
#define rbx_scriptcontext_getglobalstate_CCV __cdecl

	/*

	* rbx_modulescript_vmstate_Decompiled_Args: int a1
	* rbx_modulescript_vmstate_TypeDef: int
	* rbx_modulescript_vmstate_Aob: Unable to generate aob.
	* rbx_modulescript_vmstate_ProtectedByRetcheck: false
	* rbx_modulescript_vmstate_IsApartOfLua: false
	* rbx_modulescript_vmstate_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_modulescript_vmstate_typedef = int(__stdcall*)(int a1);
	r_rbx_modulescript_vmstate_typedef r_rbx_modulescript_vmstate = (r_rbx_modulescript_vmstate_typedef)(x(0x00CAB0E0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_modulescript_vmstate_typedef = int(rbx_modulescript_vmstate_CCV*)(int a1);
	r_rbx_modulescript_vmstate_typedef r_rbx_modulescript_vmstate = (r_rbx_modulescript_vmstate_typedef)(x(Adresses::rbx_modulescript_vmstate_addr));



	*/
	static constexpr std::uintptr_t rbx_modulescript_vmstate_addr = 0x00CAB0E0;
#define rbx_modulescript_vmstate_CCV __stdcall

	/*

	* rbx_modulescript_pervmstate_setcompletedsuccess_Decompiled_Args: int a1, int a2, int a3
	* rbx_modulescript_pervmstate_setcompletedsuccess_TypeDef: int
	* rbx_modulescript_pervmstate_setcompletedsuccess_Aob: Unable to generate aob.
	* rbx_modulescript_pervmstate_setcompletedsuccess_ProtectedByRetcheck: false
	* rbx_modulescript_pervmstate_setcompletedsuccess_IsApartOfLua: false
	* rbx_modulescript_pervmstate_setcompletedsuccess_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_modulescript_pervmstate_setcompletedsuccess_typedef = int(__fastcall*)(int a1, int a2, int a3);
	r_rbx_modulescript_pervmstate_setcompletedsuccess_typedef r_rbx_modulescript_pervmstate_setcompletedsuccess = (r_rbx_modulescript_pervmstate_setcompletedsuccess_typedef)(x(0x00CAAE50));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_modulescript_pervmstate_setcompletedsuccess_typedef = int(rbx_modulescript_pervmstate_setcompletedsuccess_CCV*)(int a1, int a2, int a3);
	r_rbx_modulescript_pervmstate_setcompletedsuccess_typedef r_rbx_modulescript_pervmstate_setcompletedsuccess = (r_rbx_modulescript_pervmstate_setcompletedsuccess_typedef)(x(Adresses::rbx_modulescript_pervmstate_setcompletedsuccess_addr));



	*/
	static constexpr std::uintptr_t rbx_modulescript_pervmstate_setcompletedsuccess_addr = 0x00CAAE50;
#define rbx_modulescript_pervmstate_setcompletedsuccess_CCV __fastcall

	/*

	* rbx_modulescript_pervmstate_getandclearyieldedimporters_Decompiled_Args: int a1, int a2
	* rbx_modulescript_pervmstate_getandclearyieldedimporters_TypeDef: int
	* rbx_modulescript_pervmstate_getandclearyieldedimporters_Aob: Unable to generate aob.
	* rbx_modulescript_pervmstate_getandclearyieldedimporters_ProtectedByRetcheck: false
	* rbx_modulescript_pervmstate_getandclearyieldedimporters_IsApartOfLua: false
	* rbx_modulescript_pervmstate_getandclearyieldedimporters_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_modulescript_pervmstate_getandclearyieldedimporters_typedef = int(__cdecl*)(int a1, int a2);
	r_rbx_modulescript_pervmstate_getandclearyieldedimporters_typedef r_rbx_modulescript_pervmstate_getandclearyieldedimporters = (r_rbx_modulescript_pervmstate_getandclearyieldedimporters_typedef)(x(0x007C79A0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_modulescript_pervmstate_getandclearyieldedimporters_typedef = int(rbx_modulescript_pervmstate_getandclearyieldedimporters_CCV*)(int a1, int a2);
	r_rbx_modulescript_pervmstate_getandclearyieldedimporters_typedef r_rbx_modulescript_pervmstate_getandclearyieldedimporters = (r_rbx_modulescript_pervmstate_getandclearyieldedimporters_typedef)(x(Adresses::rbx_modulescript_pervmstate_getandclearyieldedimporters_addr));



	*/
	static constexpr std::uintptr_t rbx_modulescript_pervmstate_getandclearyieldedimporters_addr = 0x007C79A0;
#define rbx_modulescript_pervmstate_getandclearyieldedimporters_CCV __cdecl

	/*

	* rbx_scriptcontext_requiremodulescriptsuccesscontinuation_Decompiled_Args: int a1, int a2, int a3
	* rbx_scriptcontext_requiremodulescriptsuccesscontinuation_TypeDef: int
	* rbx_scriptcontext_requiremodulescriptsuccesscontinuation_Aob: Unable to generate aob.
	* rbx_scriptcontext_requiremodulescriptsuccesscontinuation_ProtectedByRetcheck: false
	* rbx_scriptcontext_requiremodulescriptsuccesscontinuation_IsApartOfLua: false
	* rbx_scriptcontext_requiremodulescriptsuccesscontinuation_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_scriptcontext_requiremodulescriptsuccesscontinuation_typedef = int(__cdecl*)(int a1, int a2, int a3);
	r_rbx_scriptcontext_requiremodulescriptsuccesscontinuation_typedef r_rbx_scriptcontext_requiremodulescriptsuccesscontinuation = (r_rbx_scriptcontext_requiremodulescriptsuccesscontinuation_typedef)(x(0x007C5970));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_scriptcontext_requiremodulescriptsuccesscontinuation_typedef = int(rbx_scriptcontext_requiremodulescriptsuccesscontinuation_CCV*)(int a1, int a2, int a3);
	r_rbx_scriptcontext_requiremodulescriptsuccesscontinuation_typedef r_rbx_scriptcontext_requiremodulescriptsuccesscontinuation = (r_rbx_scriptcontext_requiremodulescriptsuccesscontinuation_typedef)(x(Adresses::rbx_scriptcontext_requiremodulescriptsuccesscontinuation_addr));



	*/
	static constexpr std::uintptr_t rbx_scriptcontext_requiremodulescriptsuccesscontinuation_addr = 0x007C5970;
#define rbx_scriptcontext_requiremodulescriptsuccesscontinuation_CCV __cdecl

	/*

	* rbx_setthreadname_Decompiled_Args: int a1
	* rbx_setthreadname_TypeDef: int
	* rbx_setthreadname_Aob: Unable to generate aob.
	* rbx_setthreadname_ProtectedByRetcheck: false
	* rbx_setthreadname_IsApartOfLua: false
	* rbx_setthreadname_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_setthreadname_typedef = int(__cdecl*)(int a1);
	r_rbx_setthreadname_typedef r_rbx_setthreadname = (r_rbx_setthreadname_typedef)(x(0x01105E60));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_setthreadname_typedef = int(rbx_setthreadname_CCV*)(int a1);
	r_rbx_setthreadname_typedef r_rbx_setthreadname = (r_rbx_setthreadname_typedef)(x(Adresses::rbx_setthreadname_addr));



	*/
	static constexpr std::uintptr_t rbx_setthreadname_addr = 0x01105E60;
#define rbx_setthreadname_CCV __cdecl

	/*

	* rbx_serializer_loadinstances_Decompiled_Args: int a1, int a2
	* rbx_serializer_loadinstances_TypeDef: int
	* rbx_serializer_loadinstances_Aob: Unable to generate aob.
	* rbx_serializer_loadinstances_ProtectedByRetcheck: false
	* rbx_serializer_loadinstances_IsApartOfLua: false
	* rbx_serializer_loadinstances_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_serializer_loadinstances_typedef = int(__cdecl*)(int a1, int a2);
	r_rbx_serializer_loadinstances_typedef r_rbx_serializer_loadinstances = (r_rbx_serializer_loadinstances_typedef)(x(0x00881FF0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_serializer_loadinstances_typedef = int(rbx_serializer_loadinstances_CCV*)(int a1, int a2);
	r_rbx_serializer_loadinstances_typedef r_rbx_serializer_loadinstances = (r_rbx_serializer_loadinstances_typedef)(x(Adresses::rbx_serializer_loadinstances_addr));



	*/
	static constexpr std::uintptr_t rbx_serializer_loadinstances_addr = 0x00881FF0;
#define rbx_serializer_loadinstances_CCV __cdecl

	/*

	* rbx_scriptcontext_printcallstack_Decompiled_Args: int a1, int a2, const char* a3, int a4
	* rbx_scriptcontext_printcallstack_TypeDef: int
	* rbx_scriptcontext_printcallstack_Aob: Unable to generate aob.
	* rbx_scriptcontext_printcallstack_ProtectedByRetcheck: false
	* rbx_scriptcontext_printcallstack_IsApartOfLua: false
	* rbx_scriptcontext_printcallstack_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_scriptcontext_printcallstack_typedef = int(__cdecl*)(int a1, int a2, const char* a3, int a4);
	r_rbx_scriptcontext_printcallstack_typedef r_rbx_scriptcontext_printcallstack = (r_rbx_scriptcontext_printcallstack_typedef)(x(0x007BFBB0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_scriptcontext_printcallstack_typedef = int(rbx_scriptcontext_printcallstack_CCV*)(int a1, int a2, const char* a3, int a4);
	r_rbx_scriptcontext_printcallstack_typedef r_rbx_scriptcontext_printcallstack = (r_rbx_scriptcontext_printcallstack_typedef)(x(Adresses::rbx_scriptcontext_printcallstack_addr));



	*/
	static constexpr std::uintptr_t rbx_scriptcontext_printcallstack_addr = 0x007BFBB0;
#define rbx_scriptcontext_printcallstack_CCV __cdecl

	/*

	* rbx_serializer_loadinstances_long_Decompiled_Args: int a1, int a2, int a3
	* rbx_serializer_loadinstances_long_TypeDef: int
	* rbx_serializer_loadinstances_long_Aob: Unable to generate aob.
	* rbx_serializer_loadinstances_long_ProtectedByRetcheck: false
	* rbx_serializer_loadinstances_long_IsApartOfLua: false
	* rbx_serializer_loadinstances_long_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_serializer_loadinstances_long_typedef = int(__cdecl*)(int a1, int a2, int a3);
	r_rbx_serializer_loadinstances_long_typedef r_rbx_serializer_loadinstances_long = (r_rbx_serializer_loadinstances_long_typedef)(x(0x00882010));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_serializer_loadinstances_long_typedef = int(rbx_serializer_loadinstances_long_CCV*)(int a1, int a2, int a3);
	r_rbx_serializer_loadinstances_long_typedef r_rbx_serializer_loadinstances_long = (r_rbx_serializer_loadinstances_long_typedef)(x(Adresses::rbx_serializer_loadinstances_long_addr));



	*/
	static constexpr std::uintptr_t rbx_serializer_loadinstances_long_addr = 0x00882010;
#define rbx_serializer_loadinstances_long_CCV __cdecl

	/*

	* rbx_standardout_printwithlink_Decompiled_Args: int a1, int a2, int a3, int a4
	* rbx_standardout_printwithlink_TypeDef: int
	* rbx_standardout_printwithlink_Aob: Unable to generate aob.
	* rbx_standardout_printwithlink_ProtectedByRetcheck: false
	* rbx_standardout_printwithlink_IsApartOfLua: false
	* rbx_standardout_printwithlink_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_standardout_printwithlink_typedef = int(__cdecl*)(int a1, int a2, int a3, int a4);
	r_rbx_standardout_printwithlink_typedef r_rbx_standardout_printwithlink = (r_rbx_standardout_printwithlink_typedef)(x(0x00642790));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_standardout_printwithlink_typedef = int(rbx_standardout_printwithlink_CCV*)(int a1, int a2, int a3, int a4);
	r_rbx_standardout_printwithlink_typedef r_rbx_standardout_printwithlink = (r_rbx_standardout_printwithlink_typedef)(x(Adresses::rbx_standardout_printwithlink_addr));



	*/
	static constexpr std::uintptr_t rbx_standardout_printwithlink_addr = 0x00642790;
#define rbx_standardout_printwithlink_CCV __cdecl

	/*

	* rbx_ts_startscript_Decompiled_Args: int a1, int a2
	* rbx_ts_startscript_TypeDef: int
	* rbx_ts_startscript_Aob: Unable to generate aob.
	* rbx_ts_startscript_ProtectedByRetcheck: false
	* rbx_ts_startscript_IsApartOfLua: false
	* rbx_ts_startscript_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_ts_startscript_typedef = int(__thiscall*)(int a1, int a2);
	r_rbx_ts_startscript_typedef r_rbx_ts_startscript = (r_rbx_ts_startscript_typedef)(x(0x00B94F30));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_ts_startscript_typedef = int(rbx_ts_startscript_CCV*)(int a1, int a2);
	r_rbx_ts_startscript_typedef r_rbx_ts_startscript = (r_rbx_ts_startscript_typedef)(x(Adresses::rbx_ts_startscript_addr));



	*/
	static constexpr std::uintptr_t rbx_ts_startscript_addr = 0x00B94F30;
#define rbx_ts_startscript_CCV __thiscall

	/*

	* rbx_ws_getrealphysicsfps_Decompiled_Args: int a1
	* rbx_ws_getrealphysicsfps_TypeDef: int
	* rbx_ws_getrealphysicsfps_Aob: Unable to generate aob.
	* rbx_ws_getrealphysicsfps_ProtectedByRetcheck: false
	* rbx_ws_getrealphysicsfps_IsApartOfLua: false
	* rbx_ws_getrealphysicsfps_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbx_ws_getrealphysicsfps_typedef = int(__thiscall*)(int a1);
	r_rbx_ws_getrealphysicsfps_typedef r_rbx_ws_getrealphysicsfps = (r_rbx_ws_getrealphysicsfps_typedef)(x(0x009846A0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbx_ws_getrealphysicsfps_typedef = int(rbx_ws_getrealphysicsfps_CCV*)(int a1);
	r_rbx_ws_getrealphysicsfps_typedef r_rbx_ws_getrealphysicsfps = (r_rbx_ws_getrealphysicsfps_typedef)(x(Adresses::rbx_ws_getrealphysicsfps_addr));



	*/
	static constexpr std::uintptr_t rbx_ws_getrealphysicsfps_addr = 0x009846A0;
#define rbx_ws_getrealphysicsfps_CCV __thiscall

	/*

	* rbxcrash_Decompiled_Args: int a1, int a2
	* rbxcrash_TypeDef: int
	* rbxcrash_Aob: Unable to generate aob.
	* rbxcrash_ProtectedByRetcheck: false
	* rbxcrash_IsApartOfLua: false
	* rbxcrash_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rbxcrash_typedef = int(__cdecl*)(int a1, int a2);
	r_rbxcrash_typedef r_rbxcrash = (r_rbxcrash_typedef)(x(0x018D10D0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rbxcrash_typedef = int(rbxcrash_CCV*)(int a1, int a2);
	r_rbxcrash_typedef r_rbxcrash = (r_rbxcrash_typedef)(x(Adresses::rbxcrash_addr));



	*/
	static constexpr std::uintptr_t rbxcrash_addr = 0x018D10D0;
#define rbxcrash_CCV __cdecl

	/*

	* reallymarkobject_Decompiled_Args: std::uintptr_t a1, int a2
	* reallymarkobject_TypeDef: int
	* reallymarkobject_Aob: Unable to generate aob.
	* reallymarkobject_ProtectedByRetcheck: false
	* reallymarkobject_IsApartOfLua: true
	* reallymarkobject_Mask: ....
	* Usage (Not 100% Accurate):

	using r_reallymarkobject_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_reallymarkobject_typedef r_reallymarkobject = (r_reallymarkobject_typedef)(x(0x016723A0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_reallymarkobject_typedef = int(reallymarkobject_CCV*)(std::uintptr_t a1, int a2);
	r_reallymarkobject_typedef r_reallymarkobject = (r_reallymarkobject_typedef)(x(Adresses::reallymarkobject_addr));



	*/
	static constexpr std::uintptr_t reallymarkobject_addr = 0x016723A0;
#define reallymarkobject_CCV __cdecl

	/*

	* rehash_Decompiled_Args: std::uintptr_t a1, int a2, int a3, int a4
	* rehash_TypeDef: int
	* rehash_Aob: Unable to generate aob.
	* rehash_ProtectedByRetcheck: false
	* rehash_IsApartOfLua: true
	* rehash_Mask: ....
	* Usage (Not 100% Accurate):

	using r_rehash_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_rehash_typedef r_rehash = (r_rehash_typedef)(x(0x01673F50));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_rehash_typedef = int(rehash_CCV*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_rehash_typedef r_rehash = (r_rehash_typedef)(x(Adresses::rehash_addr));



	*/
	static constexpr std::uintptr_t rehash_addr = 0x01673F50;
#define rehash_CCV __cdecl

	/*

	* reportError_Decompiled_Args: int a1, int a2
	* reportError_TypeDef: int
	* reportError_Aob: Unable to generate aob.
	* reportError_ProtectedByRetcheck: false
	* reportError_IsApartOfLua: false
	* reportError_Mask: ....
	* Usage (Not 100% Accurate):

	using r_reportError_typedef = int(__stdcall*)(int a1, int a2);
	r_reportError_typedef r_reportError = (r_reportError_typedef)(x(0x007BB150));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_reportError_typedef = int(reportError_CCV*)(int a1, int a2);
	r_reportError_typedef r_reportError = (r_reportError_typedef)(x(Adresses::reportError_addr));



	*/
	static constexpr std::uintptr_t reportError_addr = 0x007BB150;
#define reportError_CCV __stdcall

	/*

	* resize_Decompiled_Args: std::uintptr_t a1, int a2, int a3, int a4
	* resize_TypeDef: int
	* resize_Aob: Unable to generate aob.
	* resize_ProtectedByRetcheck: false
	* resize_IsApartOfLua: true
	* resize_Mask: ....
	* Usage (Not 100% Accurate):

	using r_resize_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_resize_typedef r_resize = (r_resize_typedef)(x(0x016741D0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_resize_typedef = int(resize_CCV*)(std::uintptr_t a1, int a2, int a3, int a4);
	r_resize_typedef r_resize = (r_resize_typedef)(x(Adresses::resize_addr));



	*/
	static constexpr std::uintptr_t resize_addr = 0x016741D0;
#define resize_CCV __cdecl

	/*

	* resume_Decompiled_Args: std::uintptr_t a1, int a2
	* resume_TypeDef: int
	* resume_Aob: Unable to generate aob.
	* resume_ProtectedByRetcheck: false
	* resume_IsApartOfLua: true
	* resume_Mask: ....
	* Usage (Not 100% Accurate):

	using r_resume_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_resume_typedef r_resume = (r_resume_typedef)(x(0x0165E270));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_resume_typedef = int(resume_CCV*)(std::uintptr_t a1, int a2);
	r_resume_typedef r_resume = (r_resume_typedef)(x(Adresses::resume_addr));



	*/
	static constexpr std::uintptr_t resume_addr = 0x0165E270;
#define resume_CCV __cdecl

	/*

	* resume_error_Decompiled_Args: int a1, const char* a2
	* resume_error_TypeDef: int
	* resume_error_Aob: Unable to generate aob.
	* resume_error_ProtectedByRetcheck: true
	* resume_error_IsApartOfLua: true
	* resume_error_Mask: ....
	* Usage (Not 100% Accurate):

	using r_resume_error_typedef = int(__cdecl*)(int a1, const char* a2);
	r_resume_error_typedef r_resume_error = (r_resume_error_typedef)unprotect(x(0x0165E350));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_resume_error_typedef = int(resume_error_CCV*)(int a1, const char* a2);
	r_resume_error_typedef r_resume_error = (r_resume_error_typedef)unprotect(x(Adresses::resume_error_addr));



	*/
	static constexpr std::uintptr_t resume_error_addr = 0x0165E350;
#define resume_error_CCV __cdecl

	/*

	* retcheck_Decompiled_Args: int a1, int a2
	* retcheck_TypeDef: int
	* retcheck_Aob: Unable to generate aob.
	* retcheck_ProtectedByRetcheck: false
	* retcheck_IsApartOfLua: false
	* retcheck_Mask: ....
	* Usage (Not 100% Accurate):

	using r_retcheck_typedef = int(__fastcall*)(int a1, int a2);
	r_retcheck_typedef r_retcheck = (r_retcheck_typedef)(x(0x006D1D90));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_retcheck_typedef = int(retcheck_CCV*)(int a1, int a2);
	r_retcheck_typedef r_retcheck = (r_retcheck_typedef)(x(Adresses::retcheck_addr));



	*/
	static constexpr std::uintptr_t retcheck_addr = 0x006D1D90;
#define retcheck_CCV __fastcall

	/*

	* robloxextraspace__robloxextraspace_Decompiled_Args: int a1, int a2
	* robloxextraspace__robloxextraspace_TypeDef: int
	* robloxextraspace__robloxextraspace_Aob: Unable to generate aob.
	* robloxextraspace__robloxextraspace_ProtectedByRetcheck: false
	* robloxextraspace__robloxextraspace_IsApartOfLua: false
	* robloxextraspace__robloxextraspace_Mask: ....
	* Usage (Not 100% Accurate):

	using r_robloxextraspace__robloxextraspace_typedef = int(__thiscall*)(int a1, int a2);
	r_robloxextraspace__robloxextraspace_typedef r_robloxextraspace__robloxextraspace = (r_robloxextraspace__robloxextraspace_typedef)(x(0x018C0AD0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_robloxextraspace__robloxextraspace_typedef = int(robloxextraspace__robloxextraspace_CCV*)(int a1, int a2);
	r_robloxextraspace__robloxextraspace_typedef r_robloxextraspace__robloxextraspace = (r_robloxextraspace__robloxextraspace_typedef)(x(Adresses::robloxextraspace__robloxextraspace_addr));



	*/
	static constexpr std::uintptr_t robloxextraspace__robloxextraspace_addr = 0x018C0AD0;
#define robloxextraspace__robloxextraspace_CCV __thiscall

	/*

	* sandboxthread_Decompiled_Args: int a1, int a2, int a3
	* sandboxthread_TypeDef: int
	* sandboxthread_Aob: Unable to generate aob.
	* sandboxthread_ProtectedByRetcheck: false
	* sandboxthread_IsApartOfLua: false
	* sandboxthread_Mask: ....
	* Usage (Not 100% Accurate):

	using r_sandboxthread_typedef = int(__cdecl*)(int a1, int a2, int a3);
	r_sandboxthread_typedef r_sandboxthread = (r_sandboxthread_typedef)(x(0x007BBF40));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_sandboxthread_typedef = int(sandboxthread_CCV*)(int a1, int a2, int a3);
	r_sandboxthread_typedef r_sandboxthread = (r_sandboxthread_typedef)(x(Adresses::sandboxthread_addr));



	*/
	static constexpr std::uintptr_t sandboxthread_addr = 0x007BBF40;
#define sandboxthread_CCV __cdecl

	/*

	* setarrayvector_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* setarrayvector_TypeDef: int
	* setarrayvector_Aob: Unable to generate aob.
	* setarrayvector_ProtectedByRetcheck: false
	* setarrayvector_IsApartOfLua: true
	* setarrayvector_Mask: ....
	* Usage (Not 100% Accurate):

	using r_setarrayvector_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_setarrayvector_typedef r_setarrayvector = (r_setarrayvector_typedef)(x(0x016743E0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_setarrayvector_typedef = int(setarrayvector_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_setarrayvector_typedef r_setarrayvector = (r_setarrayvector_typedef)(x(Adresses::setarrayvector_addr));



	*/
	static constexpr std::uintptr_t setarrayvector_addr = 0x016743E0;
#define setarrayvector_CCV __cdecl

	/*

	* setnodevector_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* setnodevector_TypeDef: int
	* setnodevector_Aob: Unable to generate aob.
	* setnodevector_ProtectedByRetcheck: false
	* setnodevector_IsApartOfLua: true
	* setnodevector_Mask: ....
	* Usage (Not 100% Accurate):

	using r_setnodevector_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_setnodevector_typedef r_setnodevector = (r_setnodevector_typedef)(x(0x01674480));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_setnodevector_typedef = int(setnodevector_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_setnodevector_typedef r_setnodevector = (r_setnodevector_typedef)(x(Adresses::setnodevector_addr));



	*/
	static constexpr std::uintptr_t setnodevector_addr = 0x01674480;
#define setnodevector_CCV __cdecl

	/*

	* singlestep_Decompiled_Args: int a1
	* singlestep_TypeDef: int
	* singlestep_Aob: Unable to generate aob.
	* singlestep_ProtectedByRetcheck: false
	* singlestep_IsApartOfLua: true
	* singlestep_Mask: ....
	* Usage (Not 100% Accurate):

	using r_singlestep_typedef = int(__cdecl*)(int a1);
	r_singlestep_typedef r_singlestep = (r_singlestep_typedef)(x(0x01672540));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_singlestep_typedef = int(singlestep_CCV*)(int a1);
	r_singlestep_typedef r_singlestep = (r_singlestep_typedef)(x(Adresses::singlestep_addr));



	*/
	static constexpr std::uintptr_t singlestep_addr = 0x01672540;
#define singlestep_CCV __cdecl

	/*

	* spawn_Decompiled_Args: int a1
	* spawn_TypeDef: int
	* spawn_Aob: Unable to generate aob.
	* spawn_ProtectedByRetcheck: false
	* spawn_IsApartOfLua: false
	* spawn_Mask: ....
	* Usage (Not 100% Accurate):

	using r_spawn_typedef = int(__cdecl*)(int a1);
	r_spawn_typedef r_spawn = (r_spawn_typedef)(x(0x007C8ED0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_spawn_typedef = int(spawn_CCV*)(int a1);
	r_spawn_typedef r_spawn = (r_spawn_typedef)(x(Adresses::spawn_addr));



	*/
	static constexpr std::uintptr_t spawn_addr = 0x007C8ED0;
#define spawn_CCV __cdecl

	/*

	* stack_init_Decompiled_Args: std::uintptr_t a1, int a2
	* stack_init_TypeDef: int
	* stack_init_Aob: Unable to generate aob.
	* stack_init_ProtectedByRetcheck: false
	* stack_init_IsApartOfLua: true
	* stack_init_Mask: ....
	* Usage (Not 100% Accurate):

	using r_stack_init_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_stack_init_typedef r_stack_init = (r_stack_init_typedef)(x(0x016687A0));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_stack_init_typedef = int(stack_init_CCV*)(std::uintptr_t a1, int a2);
	r_stack_init_typedef r_stack_init = (r_stack_init_typedef)(x(Adresses::stack_init_addr));



	*/
	static constexpr std::uintptr_t stack_init_addr = 0x016687A0;
#define stack_init_CCV __cdecl

	/*

	* str_find_aux_Decompiled_Args: std::uintptr_t a1, int a2
	* str_find_aux_TypeDef: int
	* str_find_aux_Aob: Unable to generate aob.
	* str_find_aux_ProtectedByRetcheck: false
	* str_find_aux_IsApartOfLua: true
	* str_find_aux_Mask: ....
	* Usage (Not 100% Accurate):

	using r_str_find_aux_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_str_find_aux_typedef r_str_find_aux = (r_str_find_aux_typedef)(x(0x0166E520));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_str_find_aux_typedef = int(str_find_aux_CCV*)(std::uintptr_t a1, int a2);
	r_str_find_aux_typedef r_str_find_aux = (r_str_find_aux_typedef)(x(Adresses::str_find_aux_addr));



	*/
	static constexpr std::uintptr_t str_find_aux_addr = 0x0166E520;
#define str_find_aux_CCV __cdecl

	/*

	* tag_error_Decompiled_Args: std::uintptr_t a1, int a2, int a3
	* tag_error_TypeDef: int
	* tag_error_Aob: Unable to generate aob.
	* tag_error_ProtectedByRetcheck: false
	* tag_error_IsApartOfLua: true
	* tag_error_Mask: ....
	* Usage (Not 100% Accurate):

	using r_tag_error_typedef = int(__cdecl*)(std::uintptr_t a1, int a2, int a3);
	r_tag_error_typedef r_tag_error = (r_tag_error_typedef)(x(0x0165D820));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_tag_error_typedef = int(tag_error_CCV*)(std::uintptr_t a1, int a2, int a3);
	r_tag_error_typedef r_tag_error = (r_tag_error_typedef)(x(Adresses::tag_error_addr));



	*/
	static constexpr std::uintptr_t tag_error_addr = 0x0165D820;
#define tag_error_CCV __cdecl

	/*

	* trustCheck_Decompiled_Args: const char* a1, char* a2,  int a3
	* trustCheck_TypeDef: int
	* trustCheck_Aob: Unable to generate aob.
	* trustCheck_ProtectedByRetcheck: false
	* trustCheck_IsApartOfLua: false
	* trustCheck_Mask: ....
	* Usage (Not 100% Accurate):

	using r_trustCheck_typedef = int(__cdecl*)(const char* a1, char* a2,  int a3);
	r_trustCheck_typedef r_trustCheck = (r_trustCheck_typedef)(x(0x0170CC80));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_trustCheck_typedef = int(trustCheck_CCV*)(const char* a1, char* a2,  int a3);
	r_trustCheck_typedef r_trustCheck = (r_trustCheck_typedef)(x(Adresses::trustCheck_addr));



	*/
	static constexpr std::uintptr_t trustCheck_addr = 0x0170CC80;
#define trustCheck_CCV __cdecl

	/*

	* taskscheduler_Decompiled_Args:
	* taskscheduler_TypeDef: int
	* taskscheduler_Aob: Unable to generate aob.
	* taskscheduler_ProtectedByRetcheck: false
	* taskscheduler_IsApartOfLua: false
	* taskscheduler_Mask: ....
	* Usage (Not 100% Accurate):

	using r_taskscheduler_typedef = int(__cdecl*)();
	r_taskscheduler_typedef r_taskscheduler = (r_taskscheduler_typedef)(x(0x01106790));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_taskscheduler_typedef = int(taskscheduler_CCV*)();
	r_taskscheduler_typedef r_taskscheduler = (r_taskscheduler_typedef)(x(Adresses::taskscheduler_addr));



	*/
	static constexpr std::uintptr_t taskscheduler_addr = 0x01106790;
#define taskscheduler_CCV __cdecl

	/*

	* unbound_search_Decompiled_Args: std::uintptr_t a1, int a2
	* unbound_search_TypeDef: int
	* unbound_search_Aob: Unable to generate aob.
	* unbound_search_ProtectedByRetcheck: false
	* unbound_search_IsApartOfLua: true
	* unbound_search_Mask: ....
	* Usage (Not 100% Accurate):

	using r_unbound_search_typedef = int(__cdecl*)(std::uintptr_t a1, int a2);
	r_unbound_search_typedef r_unbound_search = (r_unbound_search_typedef)(x(0x01674550));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_unbound_search_typedef = int(unbound_search_CCV*)(std::uintptr_t a1, int a2);
	r_unbound_search_typedef r_unbound_search = (r_unbound_search_typedef)(x(Adresses::unbound_search_addr));



	*/
	static constexpr std::uintptr_t unbound_search_addr = 0x01674550;
#define unbound_search_CCV __cdecl

	/*

	* vm_invoker_Decompiled_Args: int a1
	* vm_invoker_TypeDef: int
	* vm_invoker_Aob: Unable to generate aob.
	* vm_invoker_ProtectedByRetcheck: false
	* vm_invoker_IsApartOfLua: true
	* vm_invoker_Mask: ....
	* Usage (Not 100% Accurate):

	using r_vm_invoker_typedef = int(__thiscall*)(int a1);
	r_vm_invoker_typedef r_vm_invoker = (r_vm_invoker_typedef)(x(0x01666C90));

	* (Compatable Format) Usage (Not 100% Accurate):

	using r_vm_invoker_typedef = int(vm_invoker_CCV*)(int a1);
	r_vm_invoker_typedef r_vm_invoker = (r_vm_invoker_typedef)(x(Adresses::vm_invoker_addr));



	*/
	static constexpr std::uintptr_t vm_invoker_addr = 0x01666C90;
#define vm_invoker_CCV __thiscall

	static constexpr std::uintptr_t cum_check_bypass = 0x0170BBD7;

	static constexpr std::uintptr_t callcheck = 0x005F7AF0;

	static constexpr std::uintptr_t retcheckflag1 = 0x0322F008;

	static constexpr std::uintptr_t retcheckflag2 = 0x0322F00C;

	static constexpr std::uintptr_t xorconst = 0x031B6C90;

	static constexpr std::uintptr_t luao_nilobject = 0x029B5BE0;

	static constexpr std::uintptr_t luat_typenames = 0x029B7400;

	static constexpr std::uintptr_t kname = 0x02F46910;

	static constexpr std::uintptr_t op_index = 0x016623F0;

	static constexpr std::uintptr_t op_case_table = 0x016622C4;

	static constexpr std::uintptr_t retcheck_jmp = 0x006D1DB0;

	static constexpr std::uintptr_t trustcheck_bypass1 = 0x02DFE8E8;

	static constexpr std::uintptr_t scriptinstance = 0x02FEA3A8;

	static constexpr std::uintptr_t scriptcontext = 0x02186798;

	// Roblox Offsets 
	static constexpr int getnamecallmethod_offset = 96;
	static constexpr int networked_dm = 1256;
	static constexpr int Identity1 = 24;
	static constexpr int Identity2 = 108;

	// Luastate Offsets 
	static constexpr int ls_readonly = 8;
	static constexpr int ls_base_ci = 0;
	static constexpr int ls_end_ci = 0;
	static constexpr int ls_ci = 16;
	static constexpr int ls_saved_pc = 16;
	static constexpr int ls_top = 24;
	static constexpr int ls_base = 32;
	static constexpr int ls_env = 80;
	static constexpr int ls_l_gt = 64;

	// Globalstate Offsets 
	static constexpr int ls_gs_totalbytes = 0;
	static constexpr int ls_gs_gcthreshold = 0;
	static constexpr int ls_gs_gcpause = 64;
	static constexpr int ls_gs_gcstepmul = 60;
	static constexpr int ls_gs_rootgc = 48;
	static constexpr int ls_gs_mainthread = 1232;

	// Closure Offsets 
	static constexpr int ls_closure_nupvalues = 6;
	static constexpr int ls_closure_env = 16;
	static constexpr int ls_closure_isc = 7;

	// Types 
#define R_LUA_TNONE  -1
#define R_LUA_TUSERDATA  7
#define R_LUA_TFUNCTION  9
#define R_LUA_TSTRING  5
#define R_LUA_TBOOLEAN  1
#define R_LUA_TNUMBER  3
#define R_LUA_TTABLE  8
#define R_LUA_TNIL  0
#define R_LUA_TTHREAD  6
#define R_LUA_TVECTOR  4
#define R_LUA_TLIGHTUSERDATA  2
#define R_LUA_TPROTO  10
#define R_LUA_TUPVALUE  11

// Protos 
	static constexpr int proto_k = 16;
	static constexpr int proto_sizek = 64;
	static constexpr int proto_code = 36;
	static constexpr int proto_sizep = 60;
	static constexpr int proto_locvars = 28;
	static constexpr int proto_sizecode = 72;
	static constexpr int proto_sizeupvalues = 56;
	static constexpr int proto_maxstacksize = 80;
	static constexpr int proto_sizelineinfo = 68;
	static constexpr int proto_upvalues = 12;
	static constexpr int proto_size_locvars = 76;
	static constexpr int proto_source = 8;
	static constexpr int proto_numparams = 83;

	DWORD RBX_LuaState(DWORD ScriptContext) { return  *(DWORD*)(ScriptContext + 276) - (ScriptContext + 276); }
	std::uint32_t RBX_GlobalState(std::uint32_t a1) { std::uint32_t  v1 = (a1 + 12) - *reinterpret_cast<std::uint32_t*>((a1 + 12)); return v1; }
	DWORD RBX_SetIdentity(DWORD LS, int identity) { *(DWORD*)(*(DWORD*)(LS + 108) + 24) = identity; return 0; }
	int RBX_ProtoObfuscate(int p, int v) { *(int*)p = p - v; return 0; }
	int RBX_ProtoDeobfuscate(int p) { return *(int*)p + p; }

	// Dumped With Mellonyt aka Mellon Addy Dumper My Discord: mellonyt#1234 
	// Addys Dumped: 323
	// No Duplicate Addys :D 
}

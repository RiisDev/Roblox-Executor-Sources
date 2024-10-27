#include "RBX.hpp"

template<class Ret, class... Args>
std::function<Ret(Args...)> Call(DWORD Address, std::string Convention)
{
	if (Convention == "Stdcall" || Convention == "__stdcall")
	{
		typedef Ret(__stdcall* Fn_t)(Args...);
		return (Fn_t)(Address);
	}
	else if (Convention == "Fastcall" || Convention == "__fastcall")
	{
		typedef Ret(__fastcall* Fn_t)(Args...);
		return (Fn_t)(Address);
	}
	else if (Convention == "Thiscall" || Convention == "__thiscall")
	{
		typedef Ret(__thiscall* Fn_t)(Args...);
		return (Fn_t)(Address);
	}

	typedef Ret(__cdecl* Fn_t)(Args...);
	return (Fn_t)(Address);
}

DWORD CleanThread;

namespace Fluxus {

	DWORD CallCheckAddress,FireClickDetectorAddress,DeserializeAddress, SpawnAddress, SettopAddress,GetDataModelAddress, DataModelLoadServices, NewThreadAddress, GetFieldAddress, PushLStringAddress, SetFieldAddress, TolstringAddress, ToBooleanAddress,PushValueAddress,PcallAddress,PushBooleanAddress, ToNumberAddress,TypeAddress,TypeNameAddress,PushCClosureAddress, PushNumberAddress, CreateTableAddress, SettableAddress, ToUserDataAddress, IdkWtfAddress, GetMetaTableAddress,SetMetaTableAddress, ToPointerAddress, XMoveAddress, ReplaceAddress,InsertAddress, YieldAddress,GettopAddress,RemoveAddress,NextAddress, GetInfoAddress,Index2Address,NewLClosureAddress;
	std::string DeserializeConv, SpawnConv, SettopConv, GetDataModelConv, DataModelLoadServicesConv, NewThreadConv, GetFieldConv, PushLStringConv, SetFieldConv, TolstringConv, ToBooleanConv, PushValueConv, PcallConv, PushBooleanConv, ToNumberConv, TypeConv, TypeNameConv, PushCClosureConv, PushNumberConv, CreateTableConv, SettableConv, ToUserDataConv, IdkWtfConv, GetMetaTableConv, SetMetaTableConv, ToPointerConv, XMoveConv, ReplaceConv,InsertConv, YieldConv,GettopConv,RemoveConv,NextConv,GetInfoConv,Index2Conv,NewLClosureConv;
	
	std::function<int(DWORD,const char*,const char*,size_t)> DeserializeDef;
	std::function<void(DWORD)> SpawnDef;
	std::function<void(DWORD, int)> SettopDef;
	std::function<int(DWORD)> NewThreadDef;
	std::function<void(DWORD, const char*, size_t)> PushLStringDef;
	std::function<void(DWORD, int, const char*)> GetFieldDef;
	std::function<void(DWORD, int, const char*)> SetFieldDef;
	std::function<bool(DWORD, int)> ToBooleanDef;
	std::function<char*(DWORD, int, size_t*)> TolstringDef;
	std::function<void(DWORD, int)> PushValueDef;
	std::function<int(DWORD, int, int, int)> PcallDef;
	std::function<double(DWORD, int, int)> ToNumberDef;
	std::function<int(DWORD, int)> TypeDef;
	std::function<const char* (DWORD, int)> TypeNameDef;
	std::function<void(DWORD, int, int, int, int)> PushCClosureDef;
	std::function<int(int, double)> PushNumberDef;
	std::function<void(DWORD, int, int)> CreateTableDef;
	std::function<void(DWORD, int)> SettableDef;
	std::function<int(DWORD, int)> ToUserDataDef;
	std::function<void(DWORD, bool)> PushBooleanDef;
	std::function<int(DWORD, const char*, int, size_t)> IdkWtfDef;
	std::function<int(DWORD, int)> GetMetaTableDef;
	std::function<bool(DWORD, int)> SetMetaTableDef;
	std::function<void(DWORD, DWORD, DWORD)> XmoveDef;
	std::function<void(DWORD, int)> ReplaceDef;
	std::function<void(DWORD, int)> InsertDef;
	std::function<int(DWORD, int)> RYieldDef;
	std::function<int(DWORD)> GettopDef;
	std::function<void(DWORD, DWORD)> RemoveDef;
	std::function<int(DWORD, DWORD)> NextDef;
	std::function<std::uintptr_t(std::uintptr_t, int, int, std::uintptr_t)> NewLClosureDef;
	std::function<TValue* (DWORD, DWORD)> Index2AdrDef;
	std::function<int(DWORD, const char*, rlua_Debug*)> GetInfoDef;
	std::function<std::uintptr_t(DWORD, int)> ToPointerDef;
	std::function<void(DWORD, float, DWORD)> FireClickDetectorDef;

	void RBX::IncrTop(std::uintptr_t State) {
		*reinterpret_cast<uintptr_t*>((State)+Fluxus::Offset::TOP) += sizeof(TValue);
	}
	
	int RBX::Deserialize(const char* v2, const char* v3, size_t v4) {
		return DeserializeDef(State,v2,v3,v4);
	}

	void RBX::Spawn() {
		SpawnDef(State);
	}

	void RBX::Settop(int a1) {
		SettopDef(State, a1);
	}

	void RBX::Pop(int a1) {
		SettopDef(State, -(a1)-1);
	}

	int RBX::NewThread() {
		State = NewThreadDef(State);
		return State;
	}

	void RBX::Pushlstring(const char* a1, size_t a2) {
		PushLStringDef(State,a1, a2);
	}

	void RBX::Pushstring(const char* a1) {
		if (!a1) {
			StkId& topstack = *(StkId*)(State + Fluxus::Offset::TOP);
			topstack->tt = 0;
			IncrTop(State);
		}
		else {
			PushLStringDef(State, a1, strlen(a1));
		}
	}

	void RBX::Getfield(int a1, const char* a2) {
		GetFieldDef(State,a1,a2);
	}

	void RBX::GetGlobal(const char* a1) {
		GetFieldDef(State, LUA_GLOBALSINDEX, a1);
	}

	void RBX::SetGlobal(const char* a1) {
		SetFieldDef(State, LUA_GLOBALSINDEX, a1);
	}

	void RBX::Setfield(int a1, const char* a2) {
		SetFieldDef(State, a1, a2);
	}

	bool RBX::ToBoolean(int a1) {
		return ToBooleanDef(State, a1);
	}

	char* RBX::Tolstring(int a1, size_t* a2) {
		return TolstringDef(State,a1,a2);
	}

	void RBX::PushValue(int a1) {
		PushValueDef(State,a1);
	}

	void RBX::ECall(int a1, int a2) {
		PcallDef(State, a1, a2, NULL);
	}

	int RBX::PCall(int a1, int a2, int a3) {
		return PcallDef(State,a1,a2,a3);
	}

	double RBX::ToNumber(int a1, int a2) {
		return ToNumberDef(State,a1,a2);
	}

	int RBX::Type(int a1) {
		return TypeDef(State,a1);
	}

	const char* RBX::TypeName(int a1) {
		return TypeNameDef(State,a1);
	}

	int RBX::NewMetaTable(const char* a1) {
		Fluxus::RBX RBX(State);
		RBX.Getfield(LUA_REGISTRYINDEX, a1);  /* get registry.name */
		if (RBX.Type(-1) != 0)  /* name already in use? */
			return 0;  /* leave previous value on top, but return 0 */
		RBX.Pop(1);
		RBX.CreateTable(0, 0);  /* create metatable */
		RBX.PushValue(-1);
		RBX.Setfield(LUA_REGISTRYINDEX, a1);  /* registry.name = metatable */
		return 1;
	}

	void RBX::RawPushCClosure(int a1, int a2, int a3, int a4) {
		 PushCClosureDef(State, a1, a2, a3, a4);
	}

	void RBX::PushNumber(double a1) {
		PushNumberDef(State, a1);
	}

	void RBX::PushCClosure(int a1(DWORD), int a2) {
		PushNumberDef(State, reinterpret_cast<DWORD>(a1));
		PushCClosureDef(State, CallCheckAddress, reinterpret_cast<int>(("Fluxus_Closure")), 1 + a2, 0);
	}

	void RBX::PushCFunction(int a1(DWORD)) {
		PushNumberDef(State, reinterpret_cast<DWORD>(a1));
		PushCClosureDef(State, CallCheckAddress, reinterpret_cast<int>(("Fluxus_Closure")), 1, 0);
	}

	void RBX::CreateTable(int a1, int a2) {
		CreateTableDef(State,a1,a2);
	}

	void RBX::Settable(int a1) {
		SettableDef(State,a1);
	}

	void RBX::PushRawObject(uintptr_t type, TValue* o) {
		StkId& topstack = *(StkId*)(State + Fluxus::Offset::TOP);
		topstack->tt = type;
		topstack->value.p = o;
		*reinterpret_cast<uintptr_t*>((State)+Fluxus::Offset::TOP) += sizeof(TValue);
	}

	int RBX::ToUserData(int a1) {
		return ToUserDataDef(State, a1);
	}

	void RBX::PushBoolean(bool a1) {
		return PushBooleanDef(State, a1);
	}

	int RBX::Idkwtf(const char* a1, int a2, size_t a3) {
		return IdkWtfDef(State, a1, a2, a3);
	}
	
	int RBX::GetMetaTable(int a1) {
		return GetMetaTableDef(State,a1); 
	}

	bool RBX::SetMetaTable(int a1) {
		return SetMetaTableDef(State,a1);
	}

	void RBX::XMove(DWORD a1, DWORD a2) {
		return XmoveDef(State,a1,a2);
	}

	void RBX::Replace(int a1) {
		ReplaceDef(State,a1);
	}

	void RBX::Insert(int a1) {
		InsertDef(State,a1);
	}

	int RBX::RYield(int a1) {
		return RYieldDef(State, a1);
	}

	int RBX::Gettop() {
		return GettopDef(State);
	}

	void RBX::Remove(DWORD a1) {
		RemoveDef(State,a1);
	}

	int RBX::Next(DWORD a1) {
		return NextDef(State, a1);
	}

	bool RBX::IsCFunction(DWORD Idx) {
		Fluxus::RBX RBX(State);
		return RBX.Type(Idx) == 7 && *(BYTE*)(RBX.ToPointer(Idx) + 6);
	}

	std::uintptr_t RBX::NewLClosure(int a1, int a2, std::uintptr_t a3) {
		return NewLClosureDef(State, a1, a2, a3);
	}

	TValue* RBX::Index2Adr(DWORD Idx) {
		return Index2AdrDef(State,Idx); 
	}

	int RBX::GetInfo(const char* a1, rlua_Debug* a2) {
		return GetInfoDef(State, a1, a2);
	}

	void RBX::Settabss(const char* i, const char* v)
	{
		Fluxus::RBX RBX(State);
		RBX.Pushstring(v);
		RBX.Setfield(-2, i);
	}

	void RBX::Settabsi(const char* i, int v)
	{
		Fluxus::RBX RBX(State);
		RBX.PushNumber(v);
		RBX.Setfield(-2, i);
	}

	void RBX::Settabsb(const char* i, bool v)
	{
		Fluxus::RBX RBX(State);
		RBX.PushBoolean(v);
		RBX.Setfield(-2, i);
	}

	void RBX::Treatstackoption(const char* fname) {
		Fluxus::RBX RBX(State);
		RBX.PushValue(-2);
		RBX.Remove(-3);
		RBX.Setfield(-2, fname);
	}

	void RBX::SandboxThread() {
		Fluxus::RBX RBX(State);

		RBX.CreateTable(0,0);
		RBX.CreateTable(0, 0);
		RBX.Pushstring("The metatable is locked");
		RBX.Setfield(-2, "__metatable");
		RBX.Pushlstring("__index",7);
		RBX.PushValue(LUA_GLOBALSINDEX);
		RBX.Settable(-3);
		RBX.SetMetaTable(-2);
		RBX.Replace(LUA_GLOBALSINDEX);
	}

	void RBX::Error(const char* a1) {
		throw std::exception(a1);
	}

	std::uintptr_t RBX::ToPointer(int a1) {
		return ToPointerDef(State,a1);
	}

	void RBX::Setreadonly(int a1, bool a2) {
		Fluxus::RBX RBX(State);
		*reinterpret_cast<BYTE*>(RBX.ToPointer(a1) + Fluxus::Offset::Readonly) = a2;
	}

	void RBX::CheckArg(DWORD Idx,DWORD Type) {
		Fluxus::RBX RBX(State);
		DWORD TypeGot = RBX.Type(Idx);
		const std::string ExpectedType = RBX.TypeName(Type);
		const std::string GotType = RBX.TypeName(TypeGot);
		const std::string Format = std::string("Bad argument to #") + std::to_string(Idx) + std::string("! Expected ") + ExpectedType + std::string(", got ") + GotType;
		if (TypeGot != Type) {		
			throw std::exception(Format.c_str());
		}
	}

	void RBX::CheckArg(DWORD Idx) {
		Fluxus::RBX RBX(State);
		DWORD TypeGot = RBX.Type(Idx);
		const std::string Format = std::string("Bad argument to #") + std::to_string(Idx) + std::string("! Expected any, got no value");
		if (TypeGot == -1) {
			throw std::exception(Format.c_str());
		}
	}

	void RBX::FireClickDetector(DWORD a1, float a2, DWORD a3) {
		FireClickDetectorDef(a1,a2,a3);
	}

	const char* RBX::CFunctionName(DWORD Idx, bool Index)
	{
		DWORD Closure;
		if (Index) {
			Fluxus::RBX RBX(State);
			Closure = RBX.ToPointer(Idx);
		}
		else {
			Closure = Idx;
		}


		const char* Name = reinterpret_cast<const char*>((Closure + 32) ^ *reinterpret_cast<DWORD*>(Closure + 32)); /*Updated*/
		return Name ? Name : "Not Found";
	}

	DWORD RBX::GetGlobalState() {
		return State + 8 + *reinterpret_cast<DWORD*>(State + 8); /*Updated*/
	}

	DWORD RBX::ProtoDeobf(DWORD Proto, DWORD Offset) {
		return Proto + Offset + *reinterpret_cast<DWORD*>(Proto + Offset); /*Updated*/
	}

	DWORD RBX::GetProto(DWORD Idx)
	{
		DWORD Closure;
		if (Idx > 3) {
			Closure = Idx;
		}
		else {
			Fluxus::RBX RBX(State);
			Closure = RBX.ToPointer(Idx);
		}
		return Closure + 24 - *reinterpret_cast<DWORD*>(Closure + 24); /*Updated*/
	}

	int RBX::SetCleanState(DWORD SC) {
		CleanThread = SC;
		return CleanThread;
	}

	int RBX::GetCleanState() {
		return CleanThread;
	}

	DWORD ReturnCallCheck() {
		return CallCheckAddress;
	}

	const char* GetClass(int Instance) {
		return (const char*)(*(int(**)(void))(*(int*)Instance + 16))();
	}


	DWORD FindChildOfInstance(DWORD Instance, const char* Child) {
		DWORD StartOfChildren = *(DWORD*)(Instance + 44);
		DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
		for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
		{
			if (memcmp(GetClass(*(int*)i), Child, strlen(Child)) == 0)
			{
				return *(int*)i;
			}
		}
		return 0;	
	}

	DWORD ReturnDataModel()
	{
		static DWORD DMPad[1];
		((void(__thiscall*)(DWORD, DWORD))unprotect(GetDataModelAddress))(((DWORD(__cdecl*)())unprotect(DataModelLoadServices))(), (DWORD)DMPad);
		return DMPad[0];
	}

	DWORD SetState(DWORD SC) {
		return (SC + 172) ^ *reinterpret_cast<DWORD*>(SC + 172); /*Updated*/
	}

	int ASLR(int Address) {
		return Address - 0x400000 + (DWORD)(GetModuleHandleA(0));
	}

	void Init() {
		PLOG_INFO << "Starting Address Class";

		CallCheckAddress = ASLR(0x5DD820); /*Updated*/

		GetFieldAddress = ASLR(20993824); /*Updated*/
		GetFieldConv = "__fastcall";

		SetFieldAddress = ASLR(20999024); /*Updated*/
		SetFieldConv = "__stdcall";

		SettopAddress = ASLR(20999824); /*Updated*/
		SettopConv = "__cdecl";

		PushValueAddress = ASLR(20997232); /*Updated*/
		PushValueConv = "__cdecl";

		SpawnAddress = ASLR(7977904); /*Updated*/
		SpawnConv = "__cdecl";

		DeserializeAddress = ASLR(21040016); /*Updated*/
		DeserializeConv = "__cdecl";

		SettableAddress = ASLR(20999696); /*Updated*/
		SettableConv = "__cdecl";

		CreateTableAddress = ASLR(20993248); /*Updated*/
		CreateTableConv = "__cdecl";

		PushCClosureAddress = ASLR(20996016); /*Updated*/
		PushCClosureConv = "__fastcall";

		PushNumberAddress = ASLR(20996704); /*Updated*/
		PushNumberConv = "__cdecl";

		TypeAddress = ASLR(21001536); /*Updated*/
		TypeConv = "__cdecl";

		TypeNameAddress = ASLR(21001600); /*Updated*/
		TypeNameConv = "__cdecl";

		ToNumberAddress = ASLR(21000640); /*Updated*/
		ToNumberConv = "__cdecl";

		TolstringAddress = ASLR(21000336); /*Updated*/
		TolstringConv = "__fastcall";

		ToBooleanAddress = ASLR(21000128); /*Updated*/
		ToBooleanConv = "__cdecl";

		PushLStringAddress = ASLR(20996512); /*Updated*/
		PushLStringConv = "__cdecl";

		GetDataModelAddress = ASLR(15718304); /*Updated*/
		GetDataModelConv = "__thiscall";

		DataModelLoadServices = ASLR(15717968); /*Updated*/
		DataModelLoadServicesConv = "__cdecl";

		NewThreadAddress = ASLR(20995104); /*Updated*/
		NewThreadConv = "__cdecl";

		PcallAddress = ASLR(20995744); /*Updated*/
		PcallConv = "__cdecl";

		ToUserDataAddress = ASLR(21001296); /*Updated*/
		ToUserDataConv = "__cdecl";

		PushBooleanAddress = ASLR(20995936); /*Updated*/
		PushBooleanConv = "__cdecl";

		IdkWtfAddress = ASLR(0x672C30); /*Updated*/
		IdkWtfConv = "__cdecl";

		SetMetaTableAddress = ASLR(20999200); /*Updated*/
		SetMetaTableConv = "__cdecl";

		GetMetaTableAddress = ASLR(20993984); /*Updated*/
		GetMetaTableConv = "__cdecl";

		ToPointerAddress = ASLR(21000768); /*Updated*/
		ToPointerConv = "__cdecl";

		XMoveAddress = ASLR(21001696); /*Updated*/
		XMoveConv = "__cdecl";

		ReplaceAddress = ASLR(20998560); /*Updated*/
		ReplaceConv = "__cdecl";

		InsertAddress = ASLR(20994464); /*Updated*/
		InsertConv = "__cdecl";

		YieldAddress = ASLR(21006704); /*Updated*/
		YieldConv = "__cdecl";

		GettopAddress = ASLR(20994288);
		GettopConv = "__cdecl";

		RemoveAddress = unprotect(ASLR(20998416)); /*Updated*/
		RemoveConv = "__cdecl";

		NextAddress = unprotect(ASLR(20995376)); /*Updated*/
		NextConv = "__cdecl";

		GetInfoAddress = unprotect(ASLR(0x14127F0));
		GetInfoConv = "__cdecl";

		Index2Address = unprotect(ASLR(20992544)); /*Updated*/
		Index2Conv = "__fastcall";

		NewLClosureAddress = ASLR(0x1419960); /*Updated*/
		NewLClosureConv = "__cdecl";

		FireClickDetectorAddress = ASLR(12852432); /*Updated*/

		SpawnDef = Call<void, DWORD>(unprotect(SpawnAddress), SpawnConv);
		GetFieldDef = Call<void, DWORD, int, const char*>(unprotect(GetFieldAddress), GetFieldConv);
		SetFieldDef = Call<void, DWORD, int, const char*>(unprotect(SetFieldAddress), SetFieldConv);
		SettopDef = Call<void, DWORD, int>(unprotect(SettopAddress), SettopConv);
		PushValueDef = Call<void, DWORD, int>(unprotect(PushValueAddress), PushValueConv);
		DeserializeDef = Call<int, DWORD, const char*, const char*, size_t>(unprotect(DeserializeAddress), DeserializeConv);
		ToNumberDef = Call<double, DWORD, int, int>(unprotect(ToNumberAddress), ToNumberConv);
		TolstringDef = Call<char*, DWORD, int, size_t*>(unprotect(TolstringAddress), TolstringConv);
		ToBooleanDef = Call<bool, DWORD, int>(unprotect(ToBooleanAddress), ToBooleanConv);
		PushLStringDef = Call<void, DWORD, const char*, size_t>(unprotect(PushLStringAddress), PushLStringConv);
		NewThreadDef = Call<int, int>(unprotect(NewThreadAddress), NewThreadConv);
		PcallDef = Call<int, DWORD, int, int, int>(unprotect(PcallAddress), PcallConv);
		TypeDef = Call<int, DWORD, int>(unprotect(TypeAddress), TypeConv);
		TypeNameDef = Call<const char*, DWORD, int>(unprotect(TypeNameAddress), TypeNameConv);
		PushCClosureDef = Call<void, DWORD, int, int, int, int>(unprotect(PushCClosureAddress), PushCClosureConv);
		PushNumberDef = Call<int, int, double>(unprotect(PushNumberAddress), PushNumberConv);
		CreateTableDef = Call<void, DWORD, int, int>(unprotect(CreateTableAddress), CreateTableConv);
		SettableDef = Call<void, DWORD, int>(unprotect(SettableAddress), SettableConv);
		ToUserDataDef = Call<int, DWORD, int>(unprotect(ToUserDataAddress), ToUserDataConv);
		PushBooleanDef = Call<void, DWORD, bool>(unprotect(PushBooleanAddress), PushBooleanConv);
		IdkWtfDef = Call<int, DWORD, const char*, int, size_t>(unprotect(IdkWtfAddress), IdkWtfConv);
		GetMetaTableDef = Call<int, DWORD, int>(unprotect(GetMetaTableAddress), GetMetaTableConv);
		SetMetaTableDef = Call<bool, DWORD, int>(unprotect(SetMetaTableAddress), SetMetaTableConv);
		XmoveDef = Call<void, DWORD, DWORD, DWORD>(unprotect(XMoveAddress), XMoveConv);
		ReplaceDef = Call<void, DWORD, int>(unprotect(ReplaceAddress), ReplaceConv);
		InsertDef = Call<void, DWORD, int>(unprotect(InsertAddress), InsertConv);
		RYieldDef = Call<int, DWORD, int>(unprotect(YieldAddress), YieldConv);
		GettopDef = Call<int, DWORD>(unprotect(GettopAddress), GettopConv);
		RemoveDef = Call<void, DWORD, DWORD>(RemoveAddress, RemoveConv);
		NextDef = Call<int, DWORD, DWORD>(NextAddress, NextConv);
		NewLClosureDef = Call<std::uintptr_t, std::uintptr_t, int, int, std::uintptr_t>(NewLClosureAddress, NewLClosureConv);
		Index2AdrDef = Call<TValue*, DWORD, DWORD>(Index2Address, Index2Conv);
		GetInfoDef = Call<int, DWORD, const char*, rlua_Debug*>(unprotect(GetInfoAddress), GetInfoConv);
		ToPointerDef = Call<std::uintptr_t, DWORD, int>(unprotect(ToPointerAddress), ToPointerConv);
		FireClickDetectorDef = Call<void, DWORD, float, DWORD>(unprotect(FireClickDetectorAddress), "__thiscall");
	}

	RBX::RBX(DWORD rL) {
		State = rL;
	}
}


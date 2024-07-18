// Re-Written 

class RobloxDataModel
{
public:
	DWORD GetDataModel()
	{

		DWORD Array[2]{};
		r_getdatamodel(r_getdatamodel_2(), (DWORD)Array);
		return Array[0] + 0xC;
	}

	const char* GetClass(int self)
	{
		return (const char*)(*(int(**)(void))(*(int*)self + 16))();
	}

	int FindFirstClass(int Instance, const char* Name)
	{
		DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
		DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);

		for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
		{
			if (memcmp(GetClass(*(int*)i), Name, strlen(Name)) == 0)
			{
				return *(int*)i;
			}
		}
	}
};

RobloxDataModel* DM = new RobloxDataModel;

DWORD DataModel;
DWORD rL;
DWORD SC;

DWORD RBX_LuaState(DWORD ScriptContext) { return  *(DWORD*)(ScriptContext +276) - (ScriptContext + 276);} 

void InitScanning()
{
	DataModel = DM->GetDataModel();
	SC = DM->FindFirstClass(DataModel, "ScriptContext");
	rL = r_lua_newthread(RBX_LuaState(SC));
	
}




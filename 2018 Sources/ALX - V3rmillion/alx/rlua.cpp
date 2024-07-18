#include <Windows.h>
#include "rlua.h"
#include "aobscan.h"

using namespace std;


void RLua::initiate(DWORD gameInstance) {
	int scriptContext = this->getChildByName(gameInstance, "Script Context");
	if (scriptContext)
		L = *(int*)(scriptContext + 124);

	results.failCount = 0;

	getInstanceIDFromName = aobscan::scan("\x55\x8B\xEC\x83\xEC\x10\x57\x8B\x7D\x08\x83\x3F\x00", "xxxxxxxxxxxxx");
	if (!getInstanceIDFromName) {
		results.failList[results.failCount] = 7;
		++results.failCount;
	}

	setInstanceParent = aobscan::scan("\x53\x56\x8B\xF1\xC7\x45\xF0\x00\x00\x00\x00\x8B\x5D\x08\x57", "xxxxxxxxxxxxxxx") - 30;														// 53 56 8B F1 C7 45 F0 00 00 00 00 8B 5D 08 57
	if (!setInstanceParent) {
		results.failList[results.failCount] = 8;
		++results.failCount;
	}

	setMinutesAfterMidnight = (SetMinutesAfterMidnight)(aobscan::scan("\x55\x8B\xEC\xF2\x0F\x10\x45\x08\xF2\x0F\x59\x05", "xxxxxxxxxxxx")); // 55 8B EC F2 0F 10 45 08 F2 0F 59 05
	if (!setMinutesAfterMidnight) {
		results.failList[results.failCount] = 9;
		++results.failCount;
	}

	setCFrame = (SetCFrame)(aobscan::scan("\x8B\x08\x83\xF9\x01\x74\x19\x83\xF9\x02\x74\x14\x83\xF9\x03", "xxxxxxxxxxxxxxx") - 25); 												// 8B 08 83 F9 01 74 19 83 F9 02 74 14 83 F9 03
	if (!setCFrame) {
		results.failList[results.failCount] = 10;
		++results.failCount;
	}

	getCFrame = (GetCFrame)(0x8F6420);
	if (!getCFrame) {
		results.failList[results.failCount] = 11;
		++results.failCount;
	}

	playSoundInstance = (PlaySoundInstance)(aobscan::scan("\x83\xEC\x08\x56\x8B\xF1\x84\xC0\x74\x12", "xxxxxxxxxx") - 8); 															// 83 EC 08 56 8B F1 84 C0 74 12
	if (!playSoundInstance) {
		results.failList[results.failCount] = 12;
		++results.failCount;
	}

	stopSoundInstance = (StopSoundInstance)(aobscan::scan("\x83\xC4\x14\x8B\x46\x78\x85\xC0\x0F\x84\x8A\x00", "xxxxxxxxxxxx") - 38); // \x83\xC4\x14\x8B\x46\x78\x85\xC0\x0F\x84\x8A\x00												// 83 C4 14 8B 46 78 85 C0 74 38 50 56
	if (!stopSoundInstance) {
		results.failList[results.failCount] = 13;
		++results.failCount;
	}

	setProperty_integer = (SetProperty)0x8C03D0;
	setProperty_bool = (SetProperty)0x87A270;
	setProperty_float = (SetProperty)0x8C0440;
	setProperty_double = (SetProperty)0x8C04B0;
	setProperty_string = (SetProperty)0x8C0520;
}

DWORD RLua::getLuaState() { return L; }

ScanResults RLua::getInitResults() { return results; }

void RLua::setProperty(DWORD instance, const char* propName, const char* propType, void* value) {
	PropertyInfo propertyInfo;

	int unkSub = 0x8C3840;

	__asm {
		mov ecx, instance;
		mov ecx, [ecx + 0x30]; // 48
		add ecx, 28;
		lea eax, propName;
		push eax;
		call unkSub;
		add eax, 4;
		mov eax, [eax];
		mov propertyInfo.propInfo, eax;
	}

	propertyInfo.instOffset = instance + 36;
	propertyInfo.luaState = L;
	propertyInfo.loc = 3;
	propertyInfo.instS = (int)&instance;

	if (strcmp(propType, "double") == 0) {
		setProperty_double(&propertyInfo, value);
	}
	else if (strcmp(propType, "float") == 0) {
		setProperty_float(&propertyInfo, value);
	}
	else if (strcmp(propType, "string") == 0) {
		string nString = (const char*)value;
		setProperty_string(&propertyInfo, &nString);
	}
	else if (strcmp(propType, "integer") == 0) {
		setProperty_integer(&propertyInfo, value);
	}
	else if (strcmp(propType, "bool") == 0) {
		setProperty_bool(&propertyInfo, value);
	}
	else if (strcmp(propType, "enum") == 0) {
		(*(int(__thiscall **)(int, int, int))(*(int*)propertyInfo.propInfo + 68))(propertyInfo.propInfo, propertyInfo.instOffset, *(int*)value);
	}
}
/*
void RLua::setProperty(DWORD instance, const char* propName, const char* valueType, void* value) {
	__try {
		if (instance) {
			int* v3 = (int*)newuserdata(L, 8);
			*v3 = instance;
			*(v3 + 1) = 0;

			getfield(L, -10000, "Object");
			setmetatable(L, -2);

			pushstring(L, propName);
	
			if (strcmp(valueType, "string") == 0) {
				pushstring(L, (const char*)(value));
			}
			else if (strcmp(valueType, "number") == 0) {
				pushnumber(L, *(double*)value);
			}
			else if (strcmp(valueType, "bool") == 0) {
				pushboolean(L, *(BYTE*)value);
			}
			else if (strcmp(valueType, "integer") == 0) {
				pushinteger(L, *(ptrdiff_t*)value);
			}

			lSetProperty(L);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {};

	settop(L, 0);
}*/

string* RLua::getName(DWORD instance) {
	try {
		return (string*)(*(DWORD *)(instance + 0x54) + 0x1C);
	}
	catch (...) {
		return NULL;
	}
};

string RLua::getClassName(DWORD instance) {
	try {
		DWORD className = (*(DWORD(__thiscall **)(DWORD))(*(DWORD *)(instance + 32) + 4))(instance + 32); // thanks ida
		return (string)(const char*)(className + 4);
	}
	catch (...) {
		return (string)"";
	}
};

InstanceTable RLua::getInstanceTable(DWORD instance, DWORD offset) {
	InstanceTable rtn;
	__try {
		if (!*(DWORD*)(instance + offset)) {
			rtn.valid = false;
			return rtn;
		}

		rtn.begin = *(DWORD *)(*(DWORD *)(instance + offset));
		rtn.stop = *(DWORD*)(*(DWORD *)(instance + offset) + 4);
		rtn.valid = true;
		return rtn;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		rtn.valid = false;
	}
}

vector<DWORD> RLua::getChildren(DWORD instance) {
	vector<DWORD> rtn;
	InstanceTable instanceTable = getInstanceTable(instance, 0x48);
	if (!instanceTable.valid)
		return rtn;

	DWORD inc = instanceTable.begin;
	DWORD incCompare = instanceTable.stop;

	while (inc != incCompare) {
		rtn.push_back(*(DWORD*)inc);
		inc += 8;
	}
	return rtn;
}

vector<DWORD> RLua::getRecursiveChildren(DWORD instance) {

}

DWORD RLua::getChildByName(DWORD instance, const char* name) {
	vector<DWORD> children = getChildren(instance);

	for (int i = 0; i < children.size(); ++i)
	if (*getName(children[i]) == name)
		return children[i];

	return 0;
};

DWORD RLua::getInstanceInHierarchy(DWORD gameInstance, char** names, DWORD size) {
	DWORD currInstance = 0;
	for (DWORD i = 0; i < size; ++i) {
		DWORD child = getChildByName(currInstance ? currInstance : gameInstance, names[i]);
		if (child)
			currInstance = child;
		else {
			currInstance = 0;
			break;
		}
	}
	return currInstance;
};

vector<DWORD> RLua::getPlayers(DWORD gameInst) {
	vector<DWORD> rtn;
	DWORD pInstance = getChildByName(gameInst, "Players");
	InstanceTable instanceTable = getInstanceTable(pInstance, 0x17C);
	if (!instanceTable.valid)
		return rtn;

	DWORD inc = instanceTable.begin;
	DWORD incCompare = instanceTable.stop;

	while (inc < incCompare) {
		rtn.push_back(*(DWORD*)inc);
		inc += 8;
	}
	return rtn;
};



DWORD RLua::getPlayerByName(DWORD gameInstance, const char* name) {
	vector<DWORD> players = getPlayers(gameInstance);

	for (int i = 0; i < players.size(); ++i) {
		if (*getName(players[i]) == name)
			return players[i];
	}

	return 0;
}

DWORD RLua::getCharacterFromPlayer(DWORD player) {
	__try {
		return *(DWORD *)(player + 0x70);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return 0;
	}
};

DWORD createInstance(RLua& rLua, char *instanceName, DWORD parent) // 99.5% credit to chirality ;-)
{
	//__try {
	DWORD setParent = rLua.setInstanceParent;
	DWORD pObj;
	UINT id;
	DWORD instIdFromName = rLua.getInstanceIDFromName;
	//cout << setParent << "\n";
	//cout << instIdFromName << "\n";
	char *name = instanceName;
	__asm
	{
		lea eax, name
			push eax
			call instIdFromName
			mov id, eax
			add esp, 4
	}

	DWORD p = *(DWORD*)(0x120DA34);
	DWORD table = *(DWORD*)(p + 4);
	DWORD v5;
	do
	{
		if (*(DWORD*)(table + 16) >= id)
		{
			v5 = table;
			table = *(DWORD*)table;
		}
		else
		{
			table = *(DWORD*)(table + 8);
		}
	} while (!*(BYTE*)(table + 13)); //copied right from ida LOL
	v5 = ***(DWORD***)(v5 + 0x14);
	//printf("rekd\n");
	__asm
	{
		lea eax, pObj
			push eax
			call v5
			mov eax, [eax]
			sub eax, 0x20
			mov pObj, eax
			mov ecx, eax
			mov ebx, -1
			push 0
			push parent
			call setParent
	}
	return pObj;
	//}
	//__except (EXCEPTION_EXECUTE_HANDLER) {
	//	return 0;
	//}
};

void RLua::destroyInstance(DWORD instance) {
	__try {
		__asm {
			mov ecx, instance
				mov eax, [ecx]
				call dword ptr[eax + 12]
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return;
	}
};

void RLua::setObjectCFrame(DWORD instance, float x, float y, float z) {
	__try {
		Vector3 newVector;
		newVector.x = x;
		newVector.y = y;
		newVector.z = z;

		CFrame newCFrame;
		// 1, 0, 0, 0, 1, 0, 0, 0, 1
		newCFrame.rotationmatrix[0] = 1;
		newCFrame.rotationmatrix[1] = 0;
		newCFrame.rotationmatrix[2] = 0;
		newCFrame.rotationmatrix[3] = 0;
		newCFrame.rotationmatrix[4] = 1;
		newCFrame.rotationmatrix[5] = 0;
		newCFrame.rotationmatrix[6] = 0;
		newCFrame.rotationmatrix[7] = 0;
		newCFrame.rotationmatrix[8] = 1;
		newCFrame.position = newVector;

		if (setCFrame) {
			setCFrame((void*)instance, &newCFrame);
		}
		else {
			//cout << "setCFrame is NULL\n";
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return;
	}
};

void RLua::setObjectCFrame(DWORD instance, CFrame* cfStruct) {
	__try {
		setCFrame((void*)instance, cfStruct);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return;
	}
};

CFrame* RLua::getObjectCFrame(DWORD instance) {
	__try {
		DWORD input = *(DWORD*)(instance + 0x0E8);
		return getCFrame((void*)input);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return 0;
	}
}

DWORD RLua::getLocalPlayer(DWORD gameInstance) {
	__try {
		DWORD localPlayer = getChildByName(gameInstance, "Players");
		return *(DWORD*)(localPlayer + 0x184);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return 0;
	}
}

void RLua::unanchorChildren(DWORD instance) {
	vector<DWORD> children = getChildren(instance);

	for (int i = 0; i < children.size(); ++i) {
		if (getClassName(children[i]) == "Part") {
			// if it's a part, unanchor it
			//setAnchored(children[i], false);
			BOOLEAN anchored = false;
			setProperty(children[i], "Anchored", "bool", &anchored);
		}

		unanchorChildren(children[i]);
	}
}

DWORD RLua::clearInstanceByName(DWORD instance, const char* name) {
	vector<DWORD> children = getChildren(instance);
	DWORD rtn = 0;

	for (int i = 0; i < children.size(); ++i) {
		if (*getName(children[i]) == name) {
			destroyInstance(children[i]);
			++rtn;
		}
	}

	return rtn;
}

DWORD RLua::clearInstanceByClass(DWORD instance, const char* cname) {
	vector<DWORD> children = getChildren(instance);
	DWORD rtn = 0;

	for (int i = 0; i < children.size(); ++i) {
		if (getClassName(children[i]) == cname) {
			destroyInstance(children[i]);
			++rtn;
		}
	}

	return rtn;
}

void RLua::unlockChildren(DWORD instance) {
	vector<DWORD> children = getChildren(instance);

	for (int i = 0; i < children.size(); ++i) {
		if (getClassName(children[i]) == "Part") {
			// if it's a part, unanchor it
			//setLocked(children[i], false);
			BOOLEAN locked = false;
			setProperty(children[i], "Locked", "bool", &locked);
		}

		unlockChildren(children[i]);
	}
}

//00584F40 
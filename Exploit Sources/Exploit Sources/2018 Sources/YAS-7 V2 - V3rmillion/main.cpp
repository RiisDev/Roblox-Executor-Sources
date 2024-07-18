
//#include "yasenv.h" //an unfinished lua env for yas7v2
//yas7v2 created by sloppey@v3rm
//disgusting ,,,,

void SetTransparency(int state, const char* plr, const char* bpa, float v){
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, bpa);
	safe_pushnumber(state, (double)v);
	safe_setfield(state, -2, "Transparency");

}


void ss_prop(int state, const char* prop, const char* str){
	safe_pushstring(state, str);
	safe_setfield(state, -2, prop);
}
void sn_prop(int state, const char* prop, double value){
	safe_pushnumber(state, value);
	safe_setfield(state, -2, prop);
}


int PushVector3(int state, const char* field, Vector3 vec){
	rgetglobal(rbx_state, "Vector3");
	f_index(rbx_state, "new");
	safe_pushnumber(state, vec.x);
	safe_pushnumber(state, vec.y);
	safe_pushnumber(state, vec.z);
	safe_pcall(state, 3, 1, 0);
	safe_setfield(state, -3, field);
	return 3;
}

int PushNumberSequence(int state, const char* field, double num){
		rgetglobal(rbx_state, "NumberSequence");
		f_index(rbx_state, "new");
		safe_pushnumber(state, num);
		safe_pcall(state, 1, 1, 0);
		safe_setfield(state, -3, field);
		return 1;
}

int PushNumberRange(int state, const char* field, double num){
	rgetglobal(rbx_state, "NumberRange");
	f_index(rbx_state, "new");
	safe_pushnumber(state, num);
	safe_pcall(state, 1, 1, 0);
	safe_setfield(state, -3, field);
	return 1;
}

int PushVector3idx(int state, int idx, const char* field, Vector3 vec){
	rgetglobal(rbx_state, "Vector3");
	f_index(rbx_state, "new");
	safe_pushnumber(state, vec.x);
	safe_pushnumber(state, vec.y);
	safe_pushnumber(state, vec.z);
	safe_pcall(state, 3, 1, 0);
	safe_setfield(state, idx, field);
	return 3;
}

int PushColor3(int state, const char* field, Color3 clr){
	rgetglobal(rbx_state, "Color3");
	f_index(rbx_state, "new");
	safe_pushnumber(state, clr.r);
	safe_pushnumber(state, clr.g);
	safe_pushnumber(state, clr.b);
	safe_pcall(state, 3, 1, 0);
	safe_setfield(state, -3, field);
	return 3;
}



int PushBrickColor(int state, const char* BrickColor){
	rgetglobal(rbx_state, "BrickColor");
	f_index(rbx_state, "new");
	safe_pushstring(state, BrickColor);
	safe_pcall(state, 1, 1, 0);
	safe_setfield(state, -3, "BrickColor");
	return 1;
}


int PushCFrame(int state, const char* field, CFrame cf){
	rgetglobal(rbx_state, "CFrame");
	f_index(rbx_state, "new");
	safe_pushnumber(state, cf.x);
	safe_pushnumber(state, cf.y);
	safe_pushnumber(state, cf.z);
	safe_pcall(state, 3, 1, 0);
	safe_setfield(state, -3, field);
	return 3;
}




int PushCFrameEA(int state, const char* field, CFrameEA cf){
	rgetglobal(rbx_state, "CFrame");
	f_index(rbx_state, "new");
	for (int i = 0; i < 12; i++){
		safe_pushnumber(state, cf.matrix[i]);
	}
	safe_pcall(state, 12, 1, 0);
	safe_setfield(state, -3, field);
	return 3;
}


int PushCFrameAngles(int state, const char* field, CFrame cf){
	rgetglobal(rbx_state, "CFrame");
	f_index(rbx_state, "Angles");
	safe_pushnumber(state, cf.x);
	safe_pushnumber(state, cf.y);
	safe_pushnumber(state, cf.z);
	safe_pcall(state, 3, 1, 0);
	safe_setfield(state, -3, field);
	return 3;
}

int PushUDim2(int state, const char* field, UDim2 udim){
	rgetglobal(rbx_state, "UDim2");
	f_index(rbx_state, "new");
	safe_pushnumber(state, udim.x1);
	safe_pushnumber(state, udim.y1);
	safe_pushnumber(state, udim.x2);
	safe_pushnumber(state, udim.y2);
	safe_pcall(state, 4, 1, 0);
	safe_setfield(state, -3, field);
	return 4;
}


int PushUDim2idx(int state, int idx, const char* field, UDim2 udim){
	rgetglobal(rbx_state, "UDim2");
	f_index(rbx_state, "new");
	safe_pushnumber(state, udim.x1);
	safe_pushnumber(state, udim.y1);
	safe_pushnumber(state, udim.x2);
	safe_pushnumber(state, udim.y2);
	safe_pcall(state, 4, 1, 0);
	safe_setfield(state, idx, field);
	return 4;
}



DWORD GetLuaState(DWORD ScriptContext){
	int ls = *(DWORD *)(ScriptContext + 56 * 1 + 164) - (ScriptContext + 56 * 1 + 164);
	return ls;
}

void InstanceNew(int state, const char* instance){
	rgetglobal(state, "Instance");
	f_index(state, "new");
	safe_pushstring(state, instance);
	safe_pushvalue(state, -4);
	safe_pcall(state, 2, 1, 0);
}

void LFindFirstChild(int state, const char* name){
	f_index(state, "FindFirstChild");
	safe_pushvalue(state, -2);
	safe_pushstring(state, name);
	safe_pcall(state, 2, 1, 0);
}

bool tpclick = false;
bool grabclick = false;
bool destroyclick = false;
bool unanchorclick = false;
bool noclip = false;

void TeleportPlrToPlr(int state, const char* p1, const char* p2){
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, p1);
	f_index(state, "Character");
	f_index(state, "Torso");
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, p2);
	f_index(state, "Character");
	f_index(state, "Head"); //r15 reasons
	f_index(state, "CFrame");
	safe_setfield(state, -7, "CFrame");
	
}


void Teleport(int state, const char* plr, float x, float y, float z){
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "HumanoidRootPart");
	rgetglobal(rbx_state, "CFrame");
	f_index(rbx_state, "new");
	safe_pushnumber(state, x);
	safe_pushnumber(state, y);
	safe_pushnumber(state, z);
	safe_pcall(state, 3, 1, 0);
	safe_setfield(state, -3, "CFrame");
}


void Explode(int state, const char* plr){
	rgetglobal(state, "game");
	f_index(state, "Workspace");
	rgetglobal(state, "Instance");
	f_index(state, "new");
	safe_pushstring(state, "Explosion");
	safe_pushvalue(state, -4);
	safe_pcall(state, 2, 1, 0);
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Torso");
	f_index(state, "Position");
	safe_setfield(state, -7, "Position");

}


void Noclip(int state){
	rgetglobal(state, "game");
	f_index(state, "Workspace");

}


void SetHealth(int state, const char* plr, float hp, bool maxhealth){
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Humanoid");
	safe_pushnumber(state, hp);
	if (maxhealth == false)
		safe_setfield(state, -2, "Health");
	else
		safe_setfield(state, -2, "MaxHealth");
	safe_pcall(state, 1, 0, 0);
}

void SetSpeed(int state, const char* plr, float speed){
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Humanoid");
	safe_pushnumber(state, speed);
	safe_setfield(state, -2, "WalkSpeed");
	safe_pcall(state, 1, 0, 0);
}

void SetJumpPower(int state, const char* plr, float power){
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Humanoid");
	safe_pushnumber(state, power);
	safe_setfield(state, -2, "JumpPower");
	safe_pcall(state, 1, 0, 0);
}



void EffectChar(int state, const char* instance, const char* loc, const char* plr, bool inchar){

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	if (inchar == false){
		f_index(state, loc);
	}
	rgetglobal(state, "Instance");
	f_index(state, "new");
	safe_pushstring(state, instance);
	safe_pushvalue(state, -4);
	safe_pcall(state, 2, 0, 0);
}


void Fling(int state, const char* plr){
	Vector3 vec;
	vec.x = 300;
	vec.y = 1000;
	vec.z = 300;



	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Humanoid");
	sn_prop(state, "Jump", 1);
	sn_prop(state, "Sit", 1);

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "HumanoidRootPart");
	PushVector3(state, "Velocity", vec);
}

void SetFogColor(int state, float r, float g, float b){
	rgetglobal(state, "game");
	f_index(state, "Lighting");
	safe_pushnumber(state, r / 255);
	safe_pushnumber(state, g / 255);
	safe_pushnumber(state, b / 255);
	safe_pcall(state, 3, 1, 0);
	safe_setfield(state, -2, "FogColor");
}

void SetFogEnd(int state, float val){
	rgetglobal(state, "game");
	f_index(state, "Lighting");
	safe_pushnumber(state, val);
	safe_setfield(state, -2, "FogEnd");
}

void rPrint(int state, const char* msg){
	rgetglobal(state, "print");
	safe_pushstring(state, msg);
	safe_pcall(state, 1, 0, 0);
}

void rWarn(int state, const char* msg){
	rgetglobal(state, "warn");
	safe_pushstring(state, msg);
	safe_pcall(state, 1, 0, 0);
}

void SkyDive(int state, const char* plr){
	Teleport(state, plr, 0, 999999, 0);
}

void GoTo(int state, const char* plr){
	TeleportPlrToPlr(state, "LocalPlayer", plr);
}

void Bring(int state, const char* plr){
	TeleportPlrToPlr(state, plr, "LocalPlayer");
}

void Clone(int state, const char* plr){
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	safe_pushnumber(state, 1);
	safe_setfield(state, -2, "Archivable");


	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Clone");
	safe_pushvalue(state, -2);
	safe_pcall(state, 1, 1, 0);
	rgetglobal(state, "game");
	f_index(state, "Workspace");
	safe_setfield(state, -3, "Parent");


}

void ChangeMesh(int state, const char* plr, const char* mid, const char* tid){

	Vector3 size;
	size.x = 5;
	size.y = 5;
	size.z = 5;
	std::string strid = mid;
	std::string strtid = tid;
	std::string masset = "rbxassetid://" + strid;
	std::string tasset = "rbxassetid://" + strtid;

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Torso");
	InstanceNew(state, "SpecialMesh");
	ss_prop(state, "MeshType", "FileMesh");
	ss_prop(state, "MeshId", masset.c_str());
	ss_prop(state, "TextureId", tasset.c_str());
	ss_prop(state, "Name", "duck_mesh"); //same

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Torso");
	f_index(state, "duck_mesh");
	PushVector3(state, "Scale", size);

	SetTransparency(rbx_state, plr, "Head", 1);
	SetTransparency(rbx_state, plr, "Right Arm", 1);
	SetTransparency(rbx_state, plr, "Left Arm", 1);
	SetTransparency(rbx_state, plr, "Left Leg", 1);
	SetTransparency(rbx_state, plr, "Right Leg", 1);

	rlua_pop(state, 1);
}

void btools(int state, const char* plr, int type){
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Backpack");
	rgetglobal(state, "Instance");
	f_index(state, "new");
	safe_pushstring(state, "HopperBin");
	safe_pushvalue(state, -4);
	safe_pcall(state, 2, 1, 0);

	safe_pushnumber(state, type);
	safe_setfield(state, -2, "BinType");

}


void Fire(int state, const char* plr){
	EffectChar(state, "Fire", "Torso", plr, false);
}
void Smoke(int state, const char* plr){
	EffectChar(state, "Smoke", "Torso", plr, false);
}
void Sparkles(int state, const char* plr){
	EffectChar(state, "Sparkles", "Torso", plr, false);
}

void ForceField(int state, const char* plr){
	EffectChar(state, "ForceField", "Torso", plr, true);
}

void Launch(int state, const char* plr){
	Vector3 vec;
	vec.x = 0;
	vec.y = 25000;
	vec.z = 0;

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Torso");
	rgetglobal(state, "Instance");
	f_index(state, "new");
	safe_pushstring(state, "BodyThrust"); //??
	safe_pushvalue(state, -4);
	safe_pcall(state, 2, 0, 0);
	int pv3 = PushVector3(state, "Power", vec); //3
	safe_pcall(state, pv3, 0, 0);
}


void EffectTeleport(int state, const char* plr){             //disgusting
	EffectChar(state, "Fire", "Torso", "LocalPlayer", false);
	Sleep(100);
	TeleportPlrToPlr(state, "LocalPlayer", plr);
	Sleep(100);
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, "LocalPlayer");
	f_index(state, "Character");
	f_index(state, "Torso");
	f_index(state, "Fire");
	f_index(state, "Destroy");
	safe_pushvalue(state, -2);
	safe_pcall(state, 1, 0, 0);
}



void f_setnumber(int state, int idx, float num, const char* prop){
	safe_pushnumber(state, num);
	safe_setfield(state, idx, prop);
}

void Fly(int state){ //unfinished so snipd



}
void bRecursive(int state){ //buggy so snip
	

}



std::string splr_idx = "";
void Recursive(int state){
	f_index(state, "GetChildren");
	safe_pushvalue(state, -2);
	safe_pcall(state, 1, 1, 0);
	int length = rlua_objlen(state, -1);
	//moved to VSplorer so snipd
}


void Jail(int state, const char* plr, const char* text){
	Vector3 v3;
	CFrame cf;
	CFrameEA cfe;
	rgetglobal(state, "game");
	f_index(state, "Workspace");
	InstanceNew(state, "Model");
	ss_prop(state, "Name", "plr_jail");


	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	InstanceNew(state, "Part");
	ss_prop(state, "Name", "jp_a");


	v3.x = 9;
	v3.y = 1;
	v3.z = 9.20000076;

	cf.x = 18.2999992;
	cf.y = 0.499990463;
	cf.z = 59.800010;

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_a");
	PushVector3(state, "Size", v3);
	

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_a");
	sn_prop(state, "Anchored", 1);


	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_a");
	PushCFrame(state, "CFrame", cf);

	//part b

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	InstanceNew(state, "Part");
	ss_prop(state, "Name", "jp_b");

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_b");
	InstanceNew(state, "SurfaceGui");
	ss_prop(state, "Face", "Back");

	UDim2 ud; 
	ud.x1 = 1;
	ud.y1 = 0;
	ud.x2 = 0.3;
	ud.y2 = 0;
	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_b");
	f_index(state, "SurfaceGui");
	InstanceNew(state, "TextLabel");
	PushUDim2(state, "Size", ud);

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_b");
	f_index(state, "SurfaceGui");
	f_index(state, "TextLabel");
	sn_prop(state, "TextScaled", 1);
	ss_prop(state, "Text", text);


	v3.x = 9;
	v3.y = 7.2;
	v3.z = 0.2;
	//18.2999992, 4.59999561, 64.3000107
	cf.x = 18.2999992;
	cf.y = 4.59999561;
	cf.z = 64.3000107;

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_b");
	PushVector3(state, "Size", v3);

	rlua_emptystack(state);

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_b");
	sn_prop(state, "Anchored", 1);
	sn_prop(state, "Transparency", 0.5);

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_b");
	PushCFrame(state, "CFrame", cf);


	//part c

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	InstanceNew(state, "Part");
	ss_prop(state, "Name", "jp_c");


	v3.x = 9;
	v3.y = 7.2;
	v3.z = 0.2;
	//22.6999989, 4.59999561, 59.7000122, 0, 0, 1, 0, 1, 0, -1, 0, 0



	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_c");
	PushVector3(state, "Size", v3);


	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_c");
	sn_prop(state, "Anchored", 1);
	sn_prop(state, "Transparency", 0.5);



	cfe.matrix[0] = 22.6999989;
	cfe.matrix[1] = 4.59999561;
	cfe.matrix[2] = 59.7000122;
	cfe.matrix[3] = 0;
	cfe.matrix[4] = 0;
	cfe.matrix[5] = 1;
	cfe.matrix[6] = 0;
	cfe.matrix[7] = 1;
	cfe.matrix[8] = 0;
	cfe.matrix[9] = -1;
	cfe.matrix[10] = 0;
	cfe.matrix[11] = 0;
	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_c");
	PushCFrameEA(state, "CFrame", cfe);



	//part d

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	InstanceNew(state, "Part");
	ss_prop(state, "Name", "jp_d");


	v3.x = 9;
	v3.y = 1;
	v3.z = 9.2;
	//18.2999992, 8.69999409, 59.8000107
	cf.x = 18.2999992;
	cf.y = 8.69999409;
	cf.z = 59.8000107;

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_d");
	PushVector3(state, "Size", v3);


	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_d");
	sn_prop(state, "Anchored", 1);

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_d");
	PushCFrame(state, "CFrame", cf);



	//part e


	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	InstanceNew(state, "Part");
	ss_prop(state, "Name", "jp_e");

	//8.6, 7.2, 0.2
	v3.x = 8.6;
	v3.y = 7.2;
	v3.z = 0.2;
	//18.2999992, 4.59999561, 55.3000107
	cf.x = 18.2999992;
	cf.y = 4.59999561;
	cf.z = 55.3000107;

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_e");
	PushVector3(state, "Size", v3);


	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_e");
	sn_prop(state, "Anchored", 1);
	sn_prop(state, "Transparency", 0.5);

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_e");
	PushCFrame(state, "CFrame", cf);



	//part f


	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	InstanceNew(state, "Part");
	ss_prop(state, "Name", "jp_f");

	//9, 7.2, 0.2
	v3.x = 9;
	v3.y = 7.2;
	v3.z = 0.2;
	//13.8999977, 4.59999561, 59.7000122, 0, 0, 1, 0, 1, 0, -1, 0, 0
	cfe.matrix[0] = 13.8999977;
	cfe.matrix[1] = 4.59999561;
	cfe.matrix[2] = 59.7000122;
	cfe.matrix[3] = 0;
	cfe.matrix[4] = 0;
	cfe.matrix[5] = 1;
	cfe.matrix[6] = 0;
	cfe.matrix[7] = 1;
	cfe.matrix[8] = 0;
	cfe.matrix[9] = -1;
	cfe.matrix[10] = 0;
	cfe.matrix[11] = 0;

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_f");
	PushVector3(state, "Size", v3);


	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_f");
	sn_prop(state, "Anchored", 1);
	sn_prop(state, "Transparency", 0.5);

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "jp_f");
	PushCFrameEA(state, "CFrame", cfe);

	Vector3 vec;
	vec.x = 18.2;
	vec.y = 2.9;
	vec.z = 59.8;
	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	InstanceNew(state, "Part");
	PushVector3(state, "Position", vec);
	
	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "Part");
	sn_prop(state, "Anchored", 1);
	sn_prop(state, "Transparency", 1);
	safe_pushboolean(state, 0);
	safe_setfield(state, -2, "CanCollide");
	//out

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Torso");
	f_index(state, "Position");
	safe_setfield(state, -10002, "plr_Torsopos");



	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "MoveTo");
	safe_pushvalue(state, -2);
	rgetglobal(state, "plr_Torsopos");
	safe_pcall(state, 2, 1, 0);


	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "HumanoidRootPart");
	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	f_index(state, "Part");
	f_index(state, "CFrame");
	safe_setfield(state, -6, "CFrame");


	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "plr_jail");
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	safe_setfield(state, -5, "Parent");


	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Humanoid");
	sn_prop(state, "WalkSpeed", 0);

	rlua_pop(state, 1);


}


void Unjail(int state, const char* plr){
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "plr_jail");  //*TODO* check if player is jailed before attempting to remove the model
	rDestroy(state);

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Humanoid");
	sn_prop(state, "WalkSpeed", 16);


}







void SSJ(int state, const char* color, const char* plr){
//snip - because gave it to a friend of mine same

}


void Trail(int state, const char* bcolor, const char* plr){ 
//i did not know there was an instance called 'Trail',,
//and I just spawned parts behind your player same ,,, snip

}


int sotptp = 0;

void ClickGrap(int state){
	if (sotptp == 0){
		
		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, "LocalPlayer");
		f_index(state, "Character");
		LFindFirstChild(state, "rop_name");
		if (rlua_isnil(state, -1)){
			rlua_pop(state, 1);
			rgetglobal(state, "game");
			f_index(state, "Players");
			f_index(state, "LocalPlayer");
			f_index(state, "Character");
			InstanceNew(state, "RopeConstraint");
			sn_prop(state, "Visible", 1);
			sn_prop(state, "Length", 15);
			ss_prop(state, "Name", "rop_name");
		}

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, "LocalPlayer");
		f_index(state, "Character");
		f_index(state, "rop_name");
		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, "LocalPlayer");
		f_index(state, "GetMouse");
		safe_pushvalue(state, -2);
		safe_pcall(state, 1, 1, 0);
		f_index(state, "Target");
		if (!rlua_isnil(state, -1)){
			InstanceNew(state, "Attachment");
			ss_prop(state, "Name", "ath_name");
		}


		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, "LocalPlayer");
		f_index(state, "Character");
		f_index(state, "rop_name");
		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, "LocalPlayer");
		f_index(state, "GetMouse");
		safe_pushvalue(state, -2);
		safe_pcall(state, 1, 1, 0);
		f_index(state, "Target");
		f_index(state, "ath_name");
		safe_setfield(state, -7, "Attachment0");


		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, "LocalPlayer");
		f_index(state, "Character");
		f_index(state, "rop_name");
		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, "LocalPlayer");
		f_index(state, "Character");
		f_index(state, "Head");
		f_index(state, "FaceCenterAttachment");
		safe_setfield(state, -7, "Attachment1");
		sotptp++;
	}
}



void ClickTP(int state){
	if (sotptp == 0){
		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, "LocalPlayer");
		f_index(state, "Character");
		f_index(state, "HumanoidRootPart");

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, "LocalPlayer");
		f_index(state, "GetMouse");
		safe_pushvalue(state, -2);
		safe_pcall(state, 1, 1, 0);
		f_index(state, "Hit");
		safe_setfield(state, -6, "CFrame");
		sotptp++;
	}
}





void ClickDestroy(int state){
	if (sotptp == 0){
		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, "LocalPlayer");
		f_index(state, "GetMouse");
		safe_pushvalue(state, -2);
		safe_pcall(state, 1, 1, 0);
		f_index(state, "Target");
		rDestroy(state);
		sotptp++;
		Sleep(1000);
	}
}

void DoNoclip(int state){
	do{
		Sleep(200); //decrease the sleep time
		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, "LocalPlayer");
		f_index(state, "Character");
		f_index(state, "Humanoid");
		f_index(state, "ChangeState");
		safe_pushvalue(state, -2);
		safe_pushnumber(state, 11);
		safe_pcall(state, 2, 0, 0);
		rlua_emptystack(state);
	} while (noclip == true);
}


void ClickUnanchor(int state){
	if (sotptp == 0){
		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, "LocalPlayer");
		f_index(state, "GetMouse");
		safe_pushvalue(state, -2);
		safe_pcall(state, 1, 1, 0);
		f_index(state, "Target");
		safe_pushboolean(state, 0);
		safe_setfield(state, -2, "Anchored");
		sotptp++;
		Sleep(300);
	}
}


void CheckTP(){
	do{
		Sleep(1);
		if ((GetAsyncKeyState(VK_LBUTTON)) != 0 && GetAsyncKeyState(VK_LCONTROL) != 0 && tpclick == true && sotptp == 0){
			ClickTP(rbx_state);
		}
		else{
			Sleep(100);
			sotptp = 0;
		}
	} while (true);
}

void CheckGrab(){
	do{
		Sleep(1);
		if ((GetAsyncKeyState(VK_LBUTTON)) != 0 && GetAsyncKeyState(VK_LCONTROL) != 0 && grabclick == true && sotptp == 0){
			ClickGrap(rbx_state);
		}
		else{
			Sleep(100);
			sotptp = 0;
		}
	} while (true);
}

void CheckClickDestroy(){
	do{
		Sleep(1);
		if ((GetAsyncKeyState(VK_LBUTTON)) != 0 && GetAsyncKeyState(VK_LCONTROL) != 0 && destroyclick == true && sotptp == 0){
			ClickDestroy(rbx_state);
		}
		else{
			Sleep(100);
			sotptp = 0;
		}
	} while (true);
}

void CheckNoclip(){
	DoNoclip(rbx_state);
}


void CheckUnanchor(){
	do{
		Sleep(1);
		if ((GetAsyncKeyState(VK_LBUTTON)) != 0 && GetAsyncKeyState(VK_LCONTROL) != 0 && unanchorclick == true && sotptp == 0){
			ClickUnanchor(rbx_state);
		}
		else{
			Sleep(100);
			sotptp = 0;
		}
	} while (true);
}


void r_PlaySound(int state, const char* id){
	std::string strid = id;
	std::string asset = "rbxassetid://" + strid;
	rgetglobal(state, "game");
	f_index(state, "Workspace");
	InstanceNew(state, "Sound");
	ss_prop(state, "SoundId", asset.c_str());
	sn_prop(state, "Volume", 10);
	ss_prop(state, "Name", "yAs7SouNd");

	rgetglobal(state, "game");
	f_index(state, "Workspace");
    f_index(state, "yAs7SouNd");
	f_index(state, "Play");
	safe_pushvalue(state, -2);
	safe_pcall(state, 1, 0, 0);
}
void ChangeUDim2(UDim2 &ud, float a, float b, float c, float d){
	UDim2 newud;
	newud.x1 = a;
	newud.y1 = b;
	newud.x2 = c;
	newud.y2 = d;
	ud = newud;
}

void ChangeVector3(Vector3 &v3, float x, float y, float z){
	Vector3 newv3;
	newv3.x = x;
	newv3.y = y;
	newv3.z = z;
	v3 = newv3;
}

void ESP(int state, const char* plr){ //yea untested so snip


}

void Gas(int state, const char* plr){

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	LFindFirstChild(state, "gs_1");
	if (rlua_isnil(state, -1)){
		Vector3 v3;
		v3.x = 38.2;
		v3.y = 1;
		v3.z = 20.4;
		//38.2, 1, 20.4
		rgetglobal(state, "game");
		f_index(state, "Workspace");
		InstanceNew(state, "Part");
		ss_prop(state, "Name", "gs_1");
		sn_prop(state, "Anchored", 1);

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_1");
		PushVector3(state, "Size", v3);

		v3.x = 9.1;
		v3.y = 0.5;
		v3.z = 13.2;

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_1");
		PushVector3(state, "Position", v3);

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_1");
		PushBrickColor(state, "Black");




		//wall1
		//38.2, 11.8, 0.2
		v3.x = 38.2;
		v3.y = 11.8;
		v3.z = 0.2;

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		InstanceNew(state, "Part");
		ss_prop(state, "Name", "gs_2");
		sn_prop(state, "Anchored", 1);

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_2");
		PushVector3(state, "Size", v3);
		//9.3, 6.9, 3.1
		v3.x = 9.3;
		v3.y = 6.9;
		v3.z = 3.1;

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_2");
		PushVector3(state, "Position", v3);

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_2");
		PushBrickColor(state, "Pastel Blue");

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_2");
		sn_prop(state, "Transparency", 0.5);


		//wall 2

		v3.x = 38.2;
		v3.y = 11.8;
		v3.z = 0.2;

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		InstanceNew(state, "Part");
		ss_prop(state, "Name", "gs_3");
		sn_prop(state, "Anchored", 1);

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_3");
		PushVector3(state, "Size", v3);
		v3.x = 9.1; //9.3, 6.9, 3.1
		v3.y = 6.9;
		v3.z = 23.3;

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_3");
		PushVector3(state, "Position", v3);

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_3");
		PushBrickColor(state, "Pastel Blue");

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_3");
		sn_prop(state, "Transparency", 0.5);

		//wall 3
		CFrame cf;
		cf.x = -9.89999962;
		cf.y = 7;
		cf.z = 13.2000008;


		v3.x = 0.2;
		v3.y = 11.6;
		v3.z = 20;

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		InstanceNew(state, "Part");
		ss_prop(state, "Name", "gs_4");
		sn_prop(state, "Anchored", 1);

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_4");
		PushVector3(state, "Size", v3);
		//9.1, 6.9, 23.3
		v3.x = 9.1;
		v3.y = 6.9;
		v3.z = 23.3;

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_4");
		PushCFrame(state, "CFrame", cf);

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_4");
		PushBrickColor(state, "Pastel Blue");

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_4");
		sn_prop(state, "Transparency", 0.5);


		//wall 4
		cf.x = 28.0999966;
		cf.y = 6.80001545;
		cf.z = 13.1999998;


		v3.x = 0.2;
		v3.y = 11.6;
		v3.z = 20;

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		InstanceNew(state, "Part");
		ss_prop(state, "Name", "gs_5");
		sn_prop(state, "Anchored", 1);

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_5");
		PushVector3(state, "Size", v3);

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_5");
		PushCFrame(state, "CFrame", cf);

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_5");
		PushBrickColor(state, "Pastel Blue");

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_5");
		sn_prop(state, "Transparency", 0.5);

		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_1");
		f_index(state, "Clone");
		safe_pushvalue(state, -2);
		safe_pcall(state, 1, 1, 0);
		ss_prop(state, "Name", "gs_5");


		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_1");
		InstanceNew(state, "Smoke");
		sn_prop(state, "Size", 15);

		Color3 clr;
		clr.r = 115;
		clr.g = 255;
		clr.b = 0;


		rgetglobal(state, "game");
		f_index(state, "Workspace");
		f_index(state, "gs_1");
		f_index(state, "Smoke");
		PushColor3(state, "Color", clr);

		

		rlua_pop(state, 1);
	}

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "HumanoidRootPart");
	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "gs_1");
	f_index(state, "CFrame");
	safe_setfield(state, -5, "CFrame");

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Head");
	safe_setfield(state, -10002, "plr_head");

	rgetglobal(state, "game");
	f_index(state, "Debris");
	f_index(state, "AddItem");
	safe_pushvalue(state, -2);
	rgetglobal(state, "plr_head");
	safe_pushnumber(state, 3);
	safe_pcall(state, 3, 0, 0);
}

void gSuicide(int state, const char* plr){
	/*
	
	Bleach = Instance.new("Part", char["Left Arm"])
	Bleach.CanCollide = false
	Mesh = Instance.new("SpecialMesh", Bleach)
	
	*/
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Left Arm");
	InstanceNew(state, "Part");
	ss_prop(state, "Name", "sui_1");


	Vector3 v3;
	v3.x = 0.03;
	v3.y = 0.03;
	v3.z = 0.04;

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Left Arm");
	f_index(state, "sui_1");
	InstanceNew(state, "SpecialMesh");
	ss_prop(state, "MeshId", "rbxassetid://443579590");
	ss_prop(state, "TextureId", "rbxassetid://443579679");
	ss_prop(state, "Name", "gn_ms");

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Left Arm");
	f_index(state, "sui_1");
	f_index(state, "gn_ms");
	PushVector3(state, "Scale", v3);

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Left Arm");
	f_index(state, "sui_1");
	InstanceNew(state, "Weld");
	ss_prop(state, "Name", "Weld"); //just incase :u


	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Left Arm");
	f_index(state, "sui_1");
	f_index(state, "Weld");
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Left Arm");
	f_index(state, "sui_1");
	safe_setfield(state, -7, "Part0");


	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Left Arm");
	f_index(state, "sui_1");
	f_index(state, "Weld");
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Left Arm");
	safe_setfield(state, -6, "Part1");

	CFrame cf;
	cf.x = 0;
	cf.y = -0.9;
	cf.z = 0;

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Left Arm");
	f_index(state, "sui_1");
	f_index(state, "Weld");
	PushCFrame(state, "C0", cf);

	/*
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Left Arm");
	f_index(state, "sui_1");
	f_index(state, "Weld");
	cf.x = 0;
	cf.y = 1;
	cf.z = 1;
	PushCFrameAngles(state, "C1", cf);
	*/

	Sleep(1500);
	Explode(state, plr);
	rlua_pop(state, 1);

}


void ClearChildren(int state){
	f_index(state, "ClearAllChildren");
	safe_pushvalue(state, -2);
	safe_pcall(state, 1, 0, 0);
}

void FencingReach(int state, Vector3 size){
	Vector3 vec;
	vec.x = 30;
	vec.y = 30;
	vec.z = 30;
	rgetglobal(rbx_state, "game");
	f_index(rbx_state, "Players");
	f_index(rbx_state, "LocalPlayer");
	f_index(rbx_state, "Backpack");
	f_index(rbx_state, "Foil"); // *TODO* use findfirstchild for it to work properly
	if (!rlua_isnil(state, -1)){
		f_index(rbx_state, "Handle");
		PushVector3(rbx_state, "Size", vec);
	}
	rlua_pop(state, 1);
}


void Unff(int state, const char* plr){
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	LFindFirstChild(state, "ForceField");
	if (!rlua_isnil(state, -1)){
		rDestroy(state);
	}
	rlua_pop(state, -1);
}

void Unfire(int state, const char* plr){
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	LFindFirstChild(state, "Fire");
	if (!rlua_isnil(state, -1)){
		rDestroy(state);
	}
	rlua_pop(state, -1);
}

void Unsmoke(int state, const char* plr){
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	LFindFirstChild(state, "Smoke");
	if (!rlua_isnil(state, -1)){
		rDestroy(state);
	}
	rlua_pop(state, -1);
}

void Unsparkles(int state, const char* plr){
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	LFindFirstChild(state, "Sparkles");
	if (!rlua_isnil(state, -1)){
		rDestroy(state);
	}
	rlua_pop(state, -1);
}

double GetPlrCoords(int state, const char* plr, std::string w){
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Torso");
	f_index(state, "Position");
	f_index(state, w.c_str()); //X, Y, Z
	return 0;
	//return rlua_tonumber(state, -1); 
}

void Hang(int state, const char* plr){
	Vector3 size;
	size.x = 3;
	size.y = 7;
	size.z = 3;


	rgetglobal(state, "game");
	f_index(state, "Workspace");
	InstanceNew(state, "Part");
	ss_prop(state, "Name", "hang_plr");
	sn_prop(state, "Anchored", 1);


	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "hang_plr");
	PushVector3(state, "Size", size);

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "hang_plr");
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Head");
	f_index(state, "Position");
	safe_setfield(state, -7, "Position");

	size.y = 1; // :u
	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "hang_plr");
	PushVector3(state, "Size", size);


	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "hang_plr");
	InstanceNew(state, "Attachment");



	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "hang_plr");
	InstanceNew(state, "RopeConstraint");
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Head");
	f_index(state, "FaceCenterAttachment");
	safe_setfield(state, -7, "Attachment1");

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "hang_plr");
	f_index(state, "RopeConstraint");
	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "hang_plr");
	f_index(state, "Attachment");
	safe_setfield(state, -5, "Attachment0");

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "hang_plr");
	f_index(state, "RopeConstraint");
	sn_prop(state, "Visible", 1);
	sn_prop(state, "Length", 3);

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "hang_plr");
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	safe_setfield(state, -5, "Parent");

	rlua_pop(state, 1);

}

void Unhang(int state, const char* plr){
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	LFindFirstChild(state, "hang_plr");
	if (!rlua_isnil(state, -1)){
		rDestroy(state);
	}
	rlua_pop(state, 1);
}

void Heal(int state, const char* plr){
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Humanoid");
	f_index(state, "MaxHealth");
	safe_setfield(state, -2, "Health");
}

void K3(int state, const char* plr){ //kkk same
	Vector3 vec;
	vec.x = 0;
	vec.y = -1;
	vec.z = 0;

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	InstanceNew(state, "Hat");
	safe_pushstring(state, "khat1");
	safe_setfield(state, -2, "Name");

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "khat1");
	PushVector3(state, "AttachmentPos", vec);

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "khat1");
	InstanceNew(state, "Part");
	safe_pushstring(state, "Handle");
	safe_setfield(state, -2, "Name");

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "khat1");
	f_index(state, "Handle");
	PushBrickColor(state, "White");

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "khat1");
	f_index(state, "Handle");
	InstanceNew(state, "SpecialMesh");
	safe_pushstring(state, "rbxassetid://431220348");
	safe_setfield(state, -2, "MeshId");

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "khat1");
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	safe_setfield(state, -5, "Parent");

	//part 2 :p
	vec.x = 0;
	vec.y = 1;
	vec.z = 0;
	rgetglobal(state, "game");
	f_index(state, "Workspace");
	InstanceNew(state, "Hat");
	safe_pushstring(state, "khat1");
	safe_setfield(state, -2, "Name");

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "khat1");
	PushVector3(state, "AttachmentPos", vec);

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "khat1");
	InstanceNew(state, "Part");
	safe_pushstring(state, "Handle");
	safe_setfield(state, -2, "Name");

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "khat1");
	f_index(state, "Handle");
	PushBrickColor(state, "White");

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "khat1");
	f_index(state, "Handle");
	InstanceNew(state, "SpecialMesh");
	safe_pushstring(state, "rbxassetid://1286427");
	safe_setfield(state, -2, "MeshId");

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "khat1");
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	safe_setfield(state, -5, "Parent");

	//part3 :u

	vec.x = 0;
	vec.y = 0;
	vec.z = 0;
	rgetglobal(state, "game");
	f_index(state, "Workspace");
	InstanceNew(state, "Hat");
	safe_pushstring(state, "khat1");
	safe_setfield(state, -2, "Name");

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "khat1");
	PushVector3(state, "AttachmentPos", vec);

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "khat1");
	InstanceNew(state, "Part");
	safe_pushstring(state, "Handle");
	safe_setfield(state, -2, "Name");

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "khat1");
	f_index(state, "Handle");
	PushBrickColor(state, "White");

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "khat1");
	f_index(state, "Handle");
	InstanceNew(state, "SpecialMesh");
	safe_pushstring(state, "rbxassetid://20637493");
	safe_setfield(state, -2, "MeshId");

	rgetglobal(state, "game");
	f_index(state, "Workspace");
	f_index(state, "khat1");
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	safe_setfield(state, -5, "Parent");


	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Torso");
	PushBrickColor(state, "White");

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Right Arm");
	PushBrickColor(state, "White");

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Left Arm");
	PushBrickColor(state, "White");

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Left Leg");
	PushBrickColor(state, "White");

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Right Leg");
	PushBrickColor(state, "White");

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Head");
	PushBrickColor(state, "White");

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Shirt");
	if (!rlua_isnil(state, -1)){
		rDestroy(state);
	}

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Pants");
	if (!rlua_isnil(state, -1)){
		rDestroy(state);
	}


	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "FindFirstChildOfClass");
	safe_pushstring(state, "Accessory");
	safe_pushvalue(state, -3);
	safe_pcall(state, 1, 1, 0);
	if (!rlua_isnil(state, -1)){
		rDestroy(state);
	}
	rlua_pop(state, 0);

}

void ChangeFace(int state, const char* plr, const char* face){
	UDim2 fsize;
	fsize.x1 = 2.5;
	fsize.y1 = 0;
	fsize.x2 = 2.5;
	fsize.y2 = 0;

	UDim2 tsize;
	tsize.x1 = 1;
	tsize.y1 = 0;
	tsize.x2 = 1;
	tsize.y2 = 0;

	UDim2 nudim;
	nudim.x1 = 0;
	nudim.y1 = 0;
	nudim.x2 = 0;
	nudim.y2 = 0;

	Vector3 sizevec;
	sizevec.x = 0;
	sizevec.y = 0.2;
	sizevec.z = 0;

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Head");
	InstanceNew(state, "BillboardGui");
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Head");
	safe_setfield(state, -6, "Adornee");

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Head");
	f_index(state, "BillboardGui");
	PushUDim2(state, "Size", fsize);

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Head");
	f_index(state, "BillboardGui");
	PushVector3(state, "StudsOffset", sizevec);

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Head");
	f_index(state, "BillboardGui");
	safe_pushnumber(state, 1);
	safe_setfield(state, -2, "AlwaysOnTop");

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Head");
	f_index(state, "BillboardGui");
	InstanceNew(state, "ImageLabel");


	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Head");
	f_index(state, "BillboardGui");
	f_index(state, "ImageLabel");
	safe_pushstring(state, face);
	safe_setfield(state, -2, "Image");

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Head");
	f_index(state, "BillboardGui");
	f_index(state, "ImageLabel");
	PushUDim2(state, "Size", tsize);

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Head");
	f_index(state, "BillboardGui");
	f_index(state, "ImageLabel");
	PushUDim2(state, "Position", nudim);

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Head");
	f_index(state, "BillboardGui");
	f_index(state, "ImageLabel");
	sn_prop(state, "Transparency", 1);

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Head");
	sn_prop(state, "Transparency", 1);
	rlua_pop(state, 1);
}

void Naked(int state, const char* plr){
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	LFindFirstChild(state, "Shirt");
	if (!rlua_isnil(state, -1)){
		rDestroy(state);
	}

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	LFindFirstChild(state, "Pants");
	if (!rlua_isnil(state, -1)){
		rDestroy(state);
	}

	rlua_pop(state, 1);
}

void Material(int state, const char * plr, const char* material){
	Naked(state, plr);
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Torso");
	safe_pushstring(state, material);
	safe_setfield(state, -2, "Material");

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Head");
	safe_pushstring(state, material);
	safe_setfield(state, -2, "Material");

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Right Arm");
	safe_pushstring(state, material);
	safe_setfield(state, -2, "Material");

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Left Arm");
	safe_pushstring(state, material);
	safe_setfield(state, -2, "Material");

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Right Leg");
	safe_pushstring(state, material);
	safe_setfield(state, -2, "Material");

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Left Leg");
	safe_pushstring(state, material);
	safe_setfield(state, -2, "Material");

	rlua_pop(state, 1);

}


void dk(int state, const char* plr){
	Vector3 sz;
	sz.x = 1;
	sz.y = 1.2;
	sz.z = 2;

	CFrame cf;
	cf.x = 0.6;
	cf.y = -1;
	cf.z = -1;

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		InstanceNew(state, "Part");
		ss_prop(state, "Name", "dkp_1");

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkp_1");
		PushVector3(state, "Size", sz);

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkp_1");
		PushBrickColor(state, "Dark orange");

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkp_1");
		InstanceNew(state, "SpecialMesh");
		ss_prop(state, "Name", "SpecialMesh");
		sz.x = 1;
		sz.y = 0.7;
		sz.z = 1;

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkp_1");
		f_index(state, "SpecialMesh");
		PushVector3(state, "Scale", sz);


		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		InstanceNew(state, "Weld");
		ss_prop(state, "Name", "dkweld1");

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkweld1");
		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "Torso");
		safe_setfield(state, -6, "Part0");

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkweld1");
		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkp_1");
		safe_setfield(state, -6, "Part1");

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkweld1");
		PushCFrame(state, "C0", cf);

		//part 2
		cf.x = -0.6;
		cf.y = -1;
		cf.z = -1;

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		InstanceNew(state, "Part");
		ss_prop(state, "Name", "dkp_2");

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkp_2");
		PushVector3(state, "Size", sz);

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkp_2");
		PushBrickColor(state, "Dark orange");

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkp_2");
		InstanceNew(state, "SpecialMesh");
		ss_prop(state, "Name", "SpecialMesh");
		sz.x = 1;
		sz.y = 0.7;
		sz.z = 1;

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkp_2");
		f_index(state, "SpecialMesh");
		PushVector3(state, "Scale", sz);


		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		InstanceNew(state, "Weld");
		ss_prop(state, "Name", "dkweld2");

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkweld2");
		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "Torso");
		safe_setfield(state, -6, "Part0");

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkweld2");
		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkp_2");
		safe_setfield(state, -6, "Part1");

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkweld2");
		PushCFrame(state, "C0", cf);


		//part 3
		cf.x = 0;
		cf.y = -1;
		cf.z = -1;

		sz.x = 1;
		sz.y = 1;
		sz.z = 4;

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		InstanceNew(state, "Part");
		ss_prop(state, "Name", "dkp_3");

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkp_3");
		PushVector3(state, "Size", sz);

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkp_3");
		PushBrickColor(state, "Dark orange");




		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		InstanceNew(state, "Weld");
		ss_prop(state, "Name", "dkweld3");

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkweld3");
		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "Torso");
		safe_setfield(state, -6, "Part0");

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkweld3");
		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkp_3");
		safe_setfield(state, -6, "Part1");

		rgetglobal(state, "game");
		f_index(state, "Players");
		f_index(state, plr);
		f_index(state, "Character");
		f_index(state, "dkweld3");
		PushCFrame(state, "C0", cf);

		rlua_pop(state, 1);
}

void Color(int state, const char * plr, const char* clr){
	Naked(state, plr);
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Torso");
	PushBrickColor(state, clr);

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Head");
	PushBrickColor(state, clr);

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Right Arm");
	PushBrickColor(state, clr);

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Left Arm");
	PushBrickColor(state, clr);

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Right Leg");
	PushBrickColor(state, clr);

	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, plr);
	f_index(state, "Character");
	f_index(state, "Left Leg");
	PushBrickColor(state, clr);

	rlua_pop(state, 1);

}

void SizePlr(int state, const char* plr, float size){ //R15 (didn't start this same)


}


void PlayAnimation(int state, const char* plr, std::string id){
	std::string formatid = "http://www.roblox.com/Asset?ID=" + id;
	//snip

}



std::string L_PlayerName(int state){
	rgetglobal(state, "game");
	f_index(state, "Players");
	f_index(state, "LocalPlayer");
	f_index(state, "Name");
	std::string name = safe_tolstring(state, -1, NULL);
	return name;
}



std::vector<const char*> allPlayers;
std::vector < const char*> storedChildren;


void GetChildren(int Instance) //Louka's findfirstchild but modified : u (i could have used lua c, but i didnt lOL)
{
	storedChildren.clear();
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		storedChildren.push_back((const char*)GetName(*(int*)i));

	}
}

void GetPlayers() //Louka's findfirstchild but modified : u
{
	allPlayers.clear();
	DWORD StartOfChildren = *(DWORD*)(Players + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		allPlayers.push_back((const char*)GetName(*(int*)i));
		
	}
}

int FindFirstChild(int instance, const char* name) //Louka's findfirstchild :u
{
	DWORD StartOfChildren = *(DWORD*)(Players + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		if (strcmp((const char*)GetName(*(int*)i), name) == 0){
			return *(int*)i;
		}
	}
	return 0;
}

void statEditorInit(int state){
	GetPlayers();
	//snip unfinished
}
unsigned int split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
	unsigned int pos = txt.find(ch);
	unsigned int initialPos = 0;
	strs.clear();
	while (pos != std::string::npos) {
		strs.push_back(txt.substr(initialPos, pos - initialPos + 1));
		initialPos = pos + 1;

		pos = txt.find(ch, initialPos);
	}
	strs.push_back(txt.substr(initialPos, min(pos, txt.size()) - initialPos + 1));

	return strs.size();
}

void doLuaC(int state, std::string str){
	std::istringstream f(str);
	std::string line;
	std::vector<std::string> v;
	int inc = 0;
	while (std::getline(f, line)) {
		split(line, v, ' ');
		if (v[0].find("getglobal") != std::string::npos){
			if (v.size() == 2){
				rgetglobal(state, v[1].c_str());
			}
		}
		if (v[0].find("getfield") != std::string::npos){
			if (v.size() == 3){
				safe_getfield(state, std::atoi(v[1].c_str()), v[2].c_str());
			}
		}
		
		if (v[0].find("setfield") != std::string::npos){
			if (v.size() == 3){
				safe_setfield(state, std::atoi(v[1].c_str()), v[2].c_str());
			}
		}
		if (v[0].find("pushvalue") != std::string::npos){
			if (v.size() == 2){
				rlua_pushvalue(state, std::atoi(v[1].c_str()));
			}
		}
		if (v[0].find("pushnumber") != std::string::npos){
			if (v.size() == 2){
				safe_pushnumber(state, std::atoi(v[1].c_str()));
			}
		}
		if (v[0].find("pushstring") != std::string::npos){ //disgusting
			if (v.size() > 1){
				std::string full = "";
				for (int e = 0; e != v.size(); e++){
					if (e != 0){
						full += (std::string)v[e];
					}
				}
				safe_pushstring(state, full.c_str());
			}
		}
		if (v[0].find("pcall") != std::string::npos){
			if (v.size() == 4){
				safe_pcall(state, std::atoi(v[1].c_str()), std::atoi(v[2].c_str()), std::atoi(v[3].c_str()));
			}
		}
		v.clear();
	}
}


void DoCommand(std::string exec, std::string effect, std::string newval){
	if (exec.find("-NOCLIP") != std::string::npos){
		if (std::atoi(newval.c_str()) == 1){
			noclip = true;
		}
		else{
			noclip = false;
		}
	}


	if (exec.find("-KILL") != std::string::npos){
		SetHealth(rbx_state, effect.c_str(), 0, false);
	}
	if (exec.find("-FIRE") != std::string::npos){
		Fire(rbx_state, effect.c_str());
	}
	if (exec.find("-SPARKLES") != std::string::npos){
		Sparkles(rbx_state, effect.c_str());
	}
	if (exec.find("-SUICIDE") != std::string::npos){
		gSuicide(rbx_state, effect.c_str());
	}
	if (exec.find("-CLICK TP") != std::string::npos){
		if (std::atoi(newval.c_str()) == 1){
			tpclick = true;
		}
		else{
			tpclick = false;
		}
	}


	if (exec.find("-CLICK UNANCHOR") != std::string::npos){
		if (std::atoi(newval.c_str()) == 1){
			unanchorclick = true;
		}
		else{
			unanchorclick = false;
		}
	}

	if (exec.find("-CLICK DESTROY") != std::string::npos){
		if (std::atoi(newval.c_str()) == 1){
			destroyclick = true;
		}
		else{
			destroyclick = false;
		}
	}


	if (exec.find("-CLICK GRAP") != std::string::npos){
		if (std::atoi(newval.c_str()) == 1){
			grabclick = true;
		}
		else{
			grabclick = false;
			rgetglobal(rbx_state, "game");
			f_index(rbx_state, "Players");
			f_index(rbx_state, "LocalPlayer");
			f_index(rbx_state, "Character");
			LFindFirstChild(rbx_state, "rop_name");
			if (!rlua_isnil(rbx_state, -1)){
				rDestroy(rbx_state);
			}
			rlua_pop(rbx_state, 1);
		}
	}
	if (exec.find("-HEAL") != std::string::npos){
		Heal(rbx_state, effect.c_str());
	}
	if (exec.find("-SMOKE") != std::string::npos){
		Smoke(rbx_state, effect.c_str());
	}
	if (exec.find("-GOTO") != std::string::npos){
		GoTo(rbx_state, effect.c_str());
	}
	if (exec.find("-BRING") != std::string::npos){
		Bring(rbx_state, effect.c_str());
	}
	if (exec.find("-SKYDIVE") != std::string::npos){
		SkyDive(rbx_state, effect.c_str());
	}
	if (exec.find("-INF HEALTH") != std::string::npos){
		SetHealth(rbx_state, effect.c_str(), HUGE_VAL, true);
		SetHealth(rbx_state, effect.c_str(), HUGE_VAL, false);
	}
	if (exec.find("-SET SPEED") != std::string::npos){
		SetSpeed(rbx_state, effect.c_str(), std::atoi(newval.c_str()));
	}

	if (exec.find("-SET JUMPPOWER") != std::string::npos){
		SetJumpPower(rbx_state, effect.c_str(), std::atoi(newval.c_str()));
	}

	if (exec.find("-FREEZE") != std::string::npos){
		SetSpeed(rbx_state, effect.c_str(), 0);
	}

	if (exec.find("-UNFREEZE") != std::string::npos){
		SetSpeed(rbx_state, effect.c_str(), 16);
	}

	if (exec.find("-SET HEALTH") != std::string::npos){
		SetHealth(rbx_state, effect.c_str(), std::atoi(newval.c_str()), true);
		SetHealth(rbx_state, effect.c_str(), std::atoi(newval.c_str()), false);
	}
	if (exec.find("-HIDE NAME") != std::string::npos){
		SetTransparency(rbx_state, effect.c_str(), "Head", 1);
	}
	if (exec.find("-INVISIBLE") != std::string::npos){
		SetTransparency(rbx_state, effect.c_str(), "Head", 1);
		SetTransparency(rbx_state, effect.c_str(), "Torso", 1);
		SetTransparency(rbx_state, effect.c_str(), "Right Arm", 1);
		SetTransparency(rbx_state, effect.c_str(), "Left Arm", 1);
		SetTransparency(rbx_state, effect.c_str(), "Left Leg", 1);
		SetTransparency(rbx_state, effect.c_str(), "Right Leg", 1);
	}
	if (exec.find("-VISIBLE") != std::string::npos){
		SetTransparency(rbx_state, effect.c_str(), "Head", 0);
		SetTransparency(rbx_state, effect.c_str(), "Torso", 0);
		SetTransparency(rbx_state, effect.c_str(), "Right Arm", 0);
		SetTransparency(rbx_state, effect.c_str(), "Left Arm", 0);
		SetTransparency(rbx_state, effect.c_str(), "Left Leg", 0);
		SetTransparency(rbx_state, effect.c_str(), "Right Leg", 0);
	}

	if (exec.find("-GHOST") != std::string::npos){
		SetTransparency(rbx_state, effect.c_str(), "Head", 0.7);
		SetTransparency(rbx_state, effect.c_str(), "Torso", 0.7);
		SetTransparency(rbx_state, effect.c_str(), "Right Arm", 0.7);
		SetTransparency(rbx_state, effect.c_str(), "Left Arm", 0.7);
		SetTransparency(rbx_state, effect.c_str(), "Left Leg", 0.7);
		SetTransparency(rbx_state, effect.c_str(), "Right Leg", 0.7);
	}

	if (exec.find("-FORCEFIELD") != std::string::npos){
		ForceField(rbx_state, effect.c_str());
	}
	if (exec.find("-GAS") != std::string::npos){
		Gas(rbx_state, effect.c_str());
	}
	if (exec.find("-FENCING REACH") != std::string::npos){
		Vector3 size;
		size.x = 30;
		size.y = 30;
		size.z = 30;
		FencingReach(rbx_state, size);
	}

	if (exec.find("-BTOOLS") != std::string::npos){
		btools(rbx_state, effect.c_str(), 1);
		btools(rbx_state, effect.c_str(), 2);
		btools(rbx_state, effect.c_str(), 3);
		btools(rbx_state, effect.c_str(), 4);
	}

	if (exec.find("-CLONE") != std::string::npos){
		Clone(rbx_state, effect.c_str());
	}

	if (exec.find("-SSJ") != std::string::npos){
		SSJ(rbx_state, "White", effect.c_str());
	}

	if (exec.find("-JAIL") != std::string::npos){
		Jail(rbx_state, effect.c_str(), newval.c_str());
	}
	if (exec.find("-UNJAIL") != std::string::npos){
		Unjail(rbx_state, effect.c_str());
	}
	if (exec.find("-EXPLODE") != std::string::npos){
		Explode(rbx_state, effect.c_str());
	}
	if (exec.find("-ESP") != std::string::npos){
		ESP(rbx_state, effect.c_str());
	}

	if (exec.find("-NOCLIP") != std::string::npos){
		Noclip(rbx_state);
	}

	if (exec.find("-BIG HEAD") != std::string::npos){
		Vector3 vec;
		vec.x = 3;
		vec.y = 3;
		vec.z = 3;
		rgetglobal(rbx_state, "game");
		f_index(rbx_state, "Players");
		f_index(rbx_state, effect.c_str());
		f_index(rbx_state, "Character");
		f_index(rbx_state, "Head");
		f_index(rbx_state, "Mesh");
		PushVector3(rbx_state, "Scale", vec);
	}
	if (exec.find("-SMALL HEAD") != std::string::npos){
		Vector3 vec;
		vec.x = 0.3;
		vec.y = 0.3;
		vec.z = 0.3;
		rgetglobal(rbx_state, "game");
		f_index(rbx_state, "Players");
		f_index(rbx_state, effect.c_str());
		f_index(rbx_state, "Character");
		f_index(rbx_state, "Head");
		f_index(rbx_state, "Mesh");
		PushVector3(rbx_state, "Scale", vec);
	}

	if (exec.find("-NORMAL HEAD") != std::string::npos){
		Vector3 vec;
		vec.x = 1;
		vec.y = 1;
		vec.z = 1;
		rgetglobal(rbx_state, "game");
		f_index(rbx_state, "Players");
		f_index(rbx_state, effect.c_str());
		f_index(rbx_state, "Character");
		f_index(rbx_state, "Head");
		f_index(rbx_state, "Mesh");
		PushVector3(rbx_state, "Scale", vec);
	}

	if (exec.find("-SODDING-T-T HEAD") != std::string::npos){ //MY HEAD DOES NOT LOOK LIKE A SODDING TIC TAC!
		Vector3 vec;
		vec.x = 1;
		vec.y = 4;
		vec.z = 1;
		rgetglobal(rbx_state, "game");
		f_index(rbx_state, "Players");
		f_index(rbx_state, effect.c_str());
		f_index(rbx_state, "Character");
		f_index(rbx_state, "Head");
		f_index(rbx_state, "Mesh");
		PushVector3(rbx_state, "Scale", vec);
	}

	if (exec.find("-TELEPORT") != std::string::npos){
		TeleportPlrToPlr(rbx_state, effect.c_str(), newval.c_str());
	}
	if (exec.find("-PLAY SOUND") != std::string::npos){
		r_PlaySound(rbx_state, newval.c_str());
	}

	if (exec.find("-UN FORCEFIELD") != std::string::npos){
		Unff(rbx_state, effect.c_str());
	}

	if (exec.find("-UN FIRE") != std::string::npos){
		Unfire(rbx_state, effect.c_str());
	}

	if (exec.find("-UN SMOKE") != std::string::npos){
		Unsmoke(rbx_state, effect.c_str());
	}

	if (exec.find("-UN SPARKLES") != std::string::npos){
		Unsparkles(rbx_state, effect.c_str());
	}

	if (exec.find("-FLING") != std::string::npos){
		Fling(rbx_state, effect.c_str());
	}
	if (exec.find("-KKKify") != std::string::npos){
		K3(rbx_state, effect.c_str());
	}

	if (exec.find("-TRUMP") != std::string::npos){
		ChangeFace(rbx_state, effect.c_str(), "rbxassetid://343377229"); 
	}
	if (exec.find("-HILLARY") != std::string::npos){
		ChangeFace(rbx_state, effect.c_str(), "rbxassetid://499105465");
	}

	if (exec.find("-HANG") != std::string::npos){
		Hang(rbx_state, effect.c_str());
	}
	if (exec.find("-UNHANG") != std::string::npos){
		Unhang(rbx_state, effect.c_str());
	}
	if (exec.find("-NAKED") != std::string::npos){
		Naked(rbx_state, effect.c_str());
	}
	if (exec.find("-MATERIAL") != std::string::npos){
		Material(rbx_state, effect.c_str(), newval.c_str());
	}
	if (exec.find("-COLOR") != std::string::npos){
		Color(rbx_state, effect.c_str(), newval.c_str());
	}
	if (exec.find("-D3K") != std::string::npos){
		dk(rbx_state, effect.c_str());
	}
	if (exec.find("-DUCK") != std::string::npos){
		ChangeMesh(rbx_state, effect.c_str(), "9419831", "9419827");
	}


	if (exec.find("-CLEAR WORKSPACE") != std::string::npos){
		rgetglobal(rbx_state, "game");
		f_index(rbx_state, "Workspace");
		ClearChildren(rbx_state);
	}
	if (exec.find("-BLINDL") != std::string::npos){
		SetFogEnd(rbx_state, 0);
	}
}
std::string parseLine(std::string buff){ 
	std::string exec = "";
	std::string effect = "";
	std::string newval = "";

	
	if (buff.find("exec") != std::string::npos) {
		unsigned cmds = buff.find("exec: ");
		unsigned cmde = buff.find("[end cmd]");
		exec = buff.substr(cmds, cmde - cmds);
	}

	if (buff.find("effect") != std::string::npos) {
		unsigned efcs = buff.find("effect: ");
		unsigned efce = buff.find("[end effect]");
		effect = buff.substr(efcs, efce - efcs);
	}
	if (buff.find("newvalue") != std::string::npos) {
		unsigned nvals = buff.find("newvalue: ");
		unsigned nvale = buff.find("[end val]");
		newval = buff.substr(nvals, nvale - nvals);
	}
	replaceAll(effect, "effect: ", "");
	replaceAll(exec, "exec: ", "");
	replaceAll(newval, "newvalue: ", "");
	

	if (effect.find("all") != std::string::npos){
		GetPlayers();
		if (allPlayers.size() != 0){
			for (int i = 0; i < allPlayers.size(); i++){
				Sleep(100);
				effect = allPlayers[i];
				DoCommand(exec, effect, newval);
			}
		}
	}
	else if (effect.find("random") != std::string::npos){
		GetPlayers();
		if (allPlayers.size() != 0){
			int i = (rand() % allPlayers.size());
			effect = allPlayers[i];
			DoCommand(exec, effect, newval);
		}
	}
	else if (effect.find("guests") != std::string::npos){
		GetPlayers();
		if (allPlayers.size() != 0){
			for (int i = 0; i < allPlayers.size(); i++){
				effect = allPlayers[i];
				if (effect.find("Guest ") != std::string::npos){
					Sleep(120);
					DoCommand(exec, effect, newval);
				}
			}
		}
	}
	else if (effect.find("others") != std::string::npos){
		GetPlayers();
		if (allPlayers.size() != 0){
			for (int i = 0; i < allPlayers.size(); i++){
				Sleep(120);
				effect = allPlayers[i];
				if (effect != L_PlayerName(rbx_state)){
						DoCommand(exec, effect, newval);
				}
			}
		}
	}
	else{
			GetPlayers();
			if (allPlayers.size() > 0){
				for (int i = 0; i < allPlayers.size(); i++){
					std::string strplr = allPlayers[i];

					std::string subplr = strplr.substr(0, effect.size());
					std::string subcplr = effect.substr(0, effect.size());
					if (strcmp(subplr.c_str(), subcplr.c_str()) == 0){
						DoCommand(exec, strplr.c_str(), newval);
					}
				}
			}
		allPlayers.clear();
		if (strcmp(effect.c_str(), "me") == 0){
			DoCommand(exec, "LocalPlayer", newval);
		}
		if (strcmp(effect.c_str(), "LocalPlayer") == 0){
			DoCommand(exec, "LocalPlayer", newval);
		}

		//DoCommand(exec, effect, newval);
	}


	return "";
}

namespace Memorys { //Louka's memory scanner
	bool Compare(const BYTE *pData, const BYTE *bMask, const char *szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++bMask)
			if (*szMask == 'x' && *pData != *bMask) return 0;
		return (*szMask) == NULL;
	}


	DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask){
		for (DWORD i = 0; i<dwLen; i++)
			if (Compare((BYTE*)(dwAddress + i), bMask, szMask))  return (DWORD)(dwAddress + i);
		return 0;
	}

	int Scan(DWORD mode, char* content, char* mask)
	{
		DWORD PageSize;
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		PageSize = si.dwPageSize;
		MEMORY_BASIC_INFORMATION mi;
		for (DWORD lpAddr = 0; lpAddr<0x7FFFFFFF; lpAddr += PageSize)
		{
			DWORD vq = VirtualQuery((void*)lpAddr, &mi, PageSize);
			if (vq == ERROR_INVALID_PARAMETER || vq == 0) break;
			if (mi.Type == MEM_MAPPED) continue;
			if (mi.Protect == mode)
			{
				int addr = FindPattern(lpAddr, PageSize, (PBYTE)content, mask);
				if (addr != 0)
				{
					return addr;
				}
			}
		}
	}


}



void Scan(){
	DWORD ScriptContextVFTable = format(adr_ScriptContext);
	DWORD PlayersVFtable = format(0x1004F74);

	ScriptContext = Memorys::Scan(PAGE_READWRITE, (char*)&ScriptContextVFTable, "xxxx");

	Players = Memorys::Scan(PAGE_READWRITE, (char*)&PlayersVFtable, "xxxx");
	DataModel = *(DWORD*)(ScriptContext + 52);
	rbx_state = (GetLuaState(ScriptContext));
}




void CheckLuaC(){ //yes,,, i used files instead of namedpipes
	do{
		Sleep(3);
		std::ifstream t(Path() + "\\REG_LUAC.YAS");
		std::string str((std::istreambuf_iterator<char>(t)),
			std::istreambuf_iterator<char>());
		if (str != ""){
			doLuaC(rbx_state, str);
			std::ofstream newFile(Path() + "\\REG_LUAC.YAS");
			newFile.close();
		}
	} while (true);


}

void CheckCmd(){
	do{
		Sleep(1);
		std::ifstream t(Path() + "\\REG_CMD.YAS");
		std::string str((std::istreambuf_iterator<char>(t)),
			std::istreambuf_iterator<char>());
		if (str != ""){
			parseLine(str);
			std::ofstream newFile(Path() + "\\REG_CMD.YAS");
			newFile.close();
		}

	} while (true);
}


inline void ChangeContext(){
	__asm{
			//snip
	}
}




void Init(){
	std::ofstream nnewFile(Path() + "\\REG_CMD.YAS");
	nnewFile.close();

	std::ofstream nnnewFile(Path() + "\\REG_LUAC.YAS");
	nnnewFile.close();

	Scan();
	//CheckWhitelist();
	//ChangeContext();
	MessageBoxA(0, "Scan complete, press ok to start using YAS7V2!", "Done", MB_TOPMOST);


	//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CheckExecution, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CheckLuaC, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CheckCmd, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CheckTP, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CheckGrab, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CheckClickDestroy, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CheckUnanchor, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CheckNoclip, NULL, NULL, NULL);
}



bool WINAPI DllMain(HMODULE hDll, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH){
		DisableThreadLibraryCalls(hDll);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Init, NULL, NULL, NULL);
	}
	return true;
}
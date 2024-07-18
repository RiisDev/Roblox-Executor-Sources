#pragma once

using namespace std;
using namespace Rlua;

void ExecuteSkId(std::string cmd) {
	std::vector<std::string> In = SkIdT(cmd, ' ');
	if (In.size() != 0) {
		if (toSkId(In.at(0)) == "ff") {
			if (toSkId(In.at(1)) == "me" || toSkId(In.at(1)) == "localplr") {
				SKID_globalSkId(SkidState, "game");
				SKID_getSkId(SkidState, -1, "Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_globalSkId(SkidState, "Instance");
				SKID_getSkId(SkidState, -1, "new");
				SKID_pushSkId(SkidState, "ForceField");
				SKID_SkIdvalue(SkidState, -4);
				SKID_SkId(SkidState, 2, 0);;
			}
			else {
				SKID_globalSkId(SkidState, "game");
				SKID_getSkId(SkidState, -1, "Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_globalSkId(SkidState, "Instance");
				SKID_getSkId(SkidState, -1, "new");
				SKID_pushSkId(SkidState, "ForceField");
				SKID_SkIdvalue(SkidState, -4);
				SKID_SkId(SkidState, 2, 0);;
			}
		}


		if (toSkId(In.at(0)) == "heaven") {
			if (toSkId(In.at(1)) == "me" || toSkId(In.at(1)) == "localplr") {
				SKID_globalSkId(SkidState, "game");
				getSKID("Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Head");
				SKID_globalSkId(SkidState, "Instance");
				SKID_getSkId(SkidState, -1, "new");
				SKID_pushSkId(SkidState, "BodyVelocity");
				SKID_SkIdvalue(SkidState, -4);
				SKID_SkId(SkidState, 2, 1);;

				SKID_globalSkId(SkidState, "game");
				getSKID("Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Head");
				SKID_pushSkId(SkidState, "BodyVelocity");
				SKID_getSkId(SkidState, -1, "Destroy");
				SKID_SkIdvalue(SkidState, -2);
				SKID_SkId(SkidState, 1, 0);;
			}
			else {
				SKID_globalSkId(SkidState, "game");
				getSKID("Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Head");
				SKID_globalSkId(SkidState, "Instance");
				SKID_getSkId(SkidState, -1, "new");
				SKID_pushSkId(SkidState, "BodyVelocity");
				SKID_SkIdvalue(SkidState, -4);
				SKID_SkId(SkidState, 2, 1);;

				SKID_globalSkId(SkidState, "game");
				getSKID("Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Head");
				SKID_pushSkId(SkidState, "BodyVelocity");
				SKID_getSkId(SkidState, -1, "Destroy");
				SKID_SkIdvalue(SkidState, -2);
				SKID_SkId(SkidState, 1, 0);;
			}
		}


		if (toSkId(In.at(0)) == "ghost") {
			if (toSkId(In.at(1)) == "me" || toSkId(In.at(1)) == "localplr") {
				SKID_globalSkId(SkidState, "game");
				SKID_getSkId(SkidState, -1, "Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Head");
				SKID_hOwMaNySkIdS(SkidState, .5);
				SKID_setSkId(SkidState, -2, "Transparency");
				SKID_SkId(SkidState, 1, 0);;
				SKID_SkIdtop(SkidState, 0);
				SKID_globalSkId(SkidState, "game");
				SKID_getSkId(SkidState, -1, "Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Head");
				SKID_getSkId(SkidState, -1, "face");
				SKID_hOwMaNySkIdS(SkidState, .5);
				SKID_setSkId(SkidState, -2, "Transparency");
				SKID_SkId(SkidState, 1, 0);;
				SKID_SkIdtop(SkidState, 0);
				SKID_globalSkId(SkidState, "game");
				SKID_getSkId(SkidState, -1, "Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Torso");
				SKID_hOwMaNySkIdS(SkidState, .5);
				SKID_setSkId(SkidState, -2, "Transparency");
				SKID_SkId(SkidState, 1, 0);;
				SKID_SkIdtop(SkidState, 0);
				SKID_globalSkId(SkidState, "game");
				SKID_getSkId(SkidState, -1, "Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Right Arm");
				SKID_hOwMaNySkIdS(SkidState, .5);
				SKID_setSkId(SkidState, -2, "Transparency");
				SKID_SkId(SkidState, 1, 0);;
				SKID_SkIdtop(SkidState, 0);
				SKID_globalSkId(SkidState, "game");
				SKID_getSkId(SkidState, -1, "Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Left Arm");
				SKID_hOwMaNySkIdS(SkidState, .5);
				SKID_setSkId(SkidState, -2, "Transparency");
				SKID_SkId(SkidState, 1, 0);;
				SKID_SkIdtop(SkidState, 0);
				SKID_globalSkId(SkidState, "game");
				SKID_getSkId(SkidState, -1, "Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Right Leg");
				SKID_hOwMaNySkIdS(SkidState, .5);
				SKID_setSkId(SkidState, -2, "Transparency");
				SKID_SkId(SkidState, 1, 0);;
				SKID_SkIdtop(SkidState, 0);
				SKID_globalSkId(SkidState, "game");
				SKID_getSkId(SkidState, -1, "Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Left Leg");
				SKID_setSkId(SkidState, -2, "Transparency");
				SKID_SkId(SkidState, 1, 0);;
				SKID_SkIdtop(SkidState, 0);
			}
			else {
				SKID_globalSkId(SkidState, "game");
				SKID_getSkId(SkidState, -1, "Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Head");
				SKID_hOwMaNySkIdS(SkidState, .5);
				SKID_setSkId(SkidState, -2, "Transparency");
				SKID_SkId(SkidState, 1, 0);;
				SKID_SkIdtop(SkidState, 0);
				SKID_globalSkId(SkidState, "game");
				SKID_getSkId(SkidState, -1, "Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Head");
				SKID_getSkId(SkidState, -1, "face");
				SKID_hOwMaNySkIdS(SkidState, .5);
				SKID_setSkId(SkidState, -2, "Transparency");
				SKID_SkId(SkidState, 1, 0);;
				SKID_SkIdtop(SkidState, 0);
				SKID_globalSkId(SkidState, "game");
				SKID_getSkId(SkidState, -1, "Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Torso");
				SKID_hOwMaNySkIdS(SkidState, .5);
				SKID_setSkId(SkidState, -2, "Transparency");
				SKID_SkId(SkidState, 1, 0);;
				SKID_SkIdtop(SkidState, 0);
				SKID_globalSkId(SkidState, "game");
				SKID_getSkId(SkidState, -1, "Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Right Arm");
				SKID_hOwMaNySkIdS(SkidState, .5);
				SKID_setSkId(SkidState, -2, "Transparency");
				SKID_SkId(SkidState, 1, 0);;
				SKID_SkIdtop(SkidState, 0);
				SKID_globalSkId(SkidState, "game");
				SKID_getSkId(SkidState, -1, "Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Left Arm");
				SKID_hOwMaNySkIdS(SkidState, .5);
				SKID_setSkId(SkidState, -2, "Transparency");
				SKID_SkId(SkidState, 1, 0);;
				SKID_SkIdtop(SkidState, 0);
				SKID_globalSkId(SkidState, "game");
				SKID_getSkId(SkidState, -1, "Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Right Leg");
				SKID_hOwMaNySkIdS(SkidState, .5);
				SKID_setSkId(SkidState, -2, "Transparency");
				SKID_SkId(SkidState, 1, 0);;
				SKID_SkIdtop(SkidState, 0);
				SKID_globalSkId(SkidState, "game");
				SKID_getSkId(SkidState, -1, "Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Left Leg");
				SKID_setSkId(SkidState, -2, "Transparency");
				SKID_SkId(SkidState, 1, 0);;
				SKID_SkIdtop(SkidState, 0);
			}
		}


		if (toSkId(In.at(0)) == "statchange") {
			if (toSkId(In.at(1)) == "me" || toSkId(In.at(1)) == "localplr") {
				SKID_globalSkId(SkidState, "game");
				SKID_getSkId(SkidState, -1, "Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "leaderstats");
				SKID_getSkId(SkidState, -1, In.at(2).c_str());
				SKID_hOwMaNySkIdS(SkidState, atoi(In.at(3).c_str()));
				SKID_setSkId(SkidState, -2, "Value");
			}
			else {
				SKID_globalSkId(SkidState, "game");
				SKID_getSkId(SkidState, -1, "Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "leaderstats");
				SKID_getSkId(SkidState, -1, In.at(2).c_str());
				SKID_hOwMaNySkIdS(SkidState, atoi(In.at(3).c_str()));
				SKID_setSkId(SkidState, -2, "Value");
			}
		}


		if (toSkId(In.at(0)) == "keemstar") {

			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Torso");
			SKID_globalSkId(SkidState, "Instance");
			SKID_getSkId(SkidState, -1, "new");
			SKID_pushSkId(SkidState, "SpecialMesh");
			SKID_SkIdvalue(SkidState, -4);
			SKID_SkId(SkidState, 2, 1);;
			std::string MeshId = "rbxassetid://471652548";
			std::string TextureId = "rbxassetid://471652580";//

			SKID_pushSkId(SkidState, TextureId.c_str());
			SKID_setSkId(SkidState, -2, "TextureId");
			SKID_pushSkId(SkidState, MeshId.c_str());;
			SKID_setSkId(SkidState, -2, "MeshId");
			SKID_globalSkId(SkidState, "Vector3");
			SKID_getSkId(SkidState, -1, "new");
			SKID_hOwMaNySkIdS(SkidState, 4);
			SKID_hOwMaNySkIdS(SkidState, 4);
			SKID_hOwMaNySkIdS(SkidState, 4);
			SKID_SkId(SkidState, 3, 1);;
			SKID_setSkId(SkidState, -3, "Scale");

			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Humanoid");
			SKID_pushSkId(SkidState, "20");
			SKID_setSkId(SkidState, -2, "HipHeight");

			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Left Leg");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Right Leg");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Left Arm");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (In.at(1) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Right Arm");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Head");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Humanoid");
			SKID_hOwMaNySkIdS(SkidState, 50);
			SKID_setSkId(SkidState, -2, "WalkSpeed");
			std::string music = "433992205";
			SKID_globalSkId(SkidState, "workspace");
			SKID_globalSkId(SkidState, "Instance");
			SKID_getSkId(SkidState, -1, "new");
			SKID_pushSkId(SkidState, "Sound");
			SKID_SkIdvalue(SkidState, -4);
			SKID_SkId(SkidState, 2, 1);;

			SKID_pushSkId(SkidState, ("rbxassetid://" + music).c_str());
			SKID_setSkId(SkidState, -2, "SoundId");

			SKID_pushSkId(SkidState, "KEEMSTAR_AUDIO_ICE");
			SKID_setSkId(SkidState, -2, "Name");

			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Volume");

			SKID_getSkId(SkidState, -1, "Play");
			SKID_SkIdvalue(SkidState, -2);
			SKID_SkId(SkidState, 1, 0);;
		}


		if (toSkId(In.at(0)) == "illuminati") {

			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Torso");
			SKID_globalSkId(SkidState, "Instance");
			SKID_getSkId(SkidState, -1, "new");
			SKID_pushSkId(SkidState, "SpecialMesh");
			SKID_SkIdvalue(SkidState, -4);
			SKID_SkId(SkidState, 2, 1);;
			std::string MeshId = "rbxassetid://438530093";
			std::string TextureId = "rbxassetid://438530120";//

			SKID_pushSkId(SkidState, TextureId.c_str());
			SKID_setSkId(SkidState, -2, "TextureId");
			SKID_pushSkId(SkidState, MeshId.c_str());;
			SKID_setSkId(SkidState, -2, "MeshId");
			SKID_globalSkId(SkidState, "Vector3");
			SKID_getSkId(SkidState, -1, "new");
			SKID_hOwMaNySkIdS(SkidState, 4);
			SKID_hOwMaNySkIdS(SkidState, 4);
			SKID_hOwMaNySkIdS(SkidState, 4);
			SKID_SkId(SkidState, 3, 1);;
			SKID_setSkId(SkidState, -3, "Scale");

			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Humanoid");
			SKID_pushSkId(SkidState, "20");
			SKID_setSkId(SkidState, -2, "HipHeight");

			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Left Leg");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Right Leg");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Left Arm");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (In.at(1) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Right Arm");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Head");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Humanoid");
			SKID_hOwMaNySkIdS(SkidState, 50);
			SKID_setSkId(SkidState, -2, "WalkSpeed");
			std::string music = "433992205";
			SKID_globalSkId(SkidState, "workspace");
			SKID_globalSkId(SkidState, "Instance");
			SKID_getSkId(SkidState, -1, "new");
			SKID_pushSkId(SkidState, "Sound");
			SKID_SkIdvalue(SkidState, -4);
			SKID_SkId(SkidState, 2, 1);;

			SKID_pushSkId(SkidState, ("rbxassetid://" + music).c_str());
			SKID_setSkId(SkidState, -2, "SoundId");
		}


		if (toSkId(In.at(0)) == "duck") {

			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Torso");
			SKID_globalSkId(SkidState, "Instance");
			SKID_getSkId(SkidState, -1, "new");
			SKID_pushSkId(SkidState, "SpecialMesh");
			SKID_SkIdvalue(SkidState, -4);
			SKID_SkId(SkidState, 2, 1);;
			std::string MeshId = "rbxassetid://521754610";
			std::string TextureId = "rbxassetid://521754612";

			SKID_pushSkId(SkidState, TextureId.c_str());
			SKID_setSkId(SkidState, -2, "TextureId");
			SKID_pushSkId(SkidState, MeshId.c_str());;
			SKID_setSkId(SkidState, -2, "MeshId");
			SKID_globalSkId(SkidState, "Vector3");
			SKID_getSkId(SkidState, -1, "new");
			SKID_hOwMaNySkIdS(SkidState, 4);
			SKID_hOwMaNySkIdS(SkidState, 4);
			SKID_hOwMaNySkIdS(SkidState, 4);
			SKID_SkId(SkidState, 3, 1);;
			SKID_setSkId(SkidState, -3, "Scale");

			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Humanoid");
			SKID_pushSkId(SkidState, "0");
			SKID_setSkId(SkidState, -2, "HipHeight");

			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Left Leg");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Right Leg");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Left Arm");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (In.at(1) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Right Arm");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Head");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Humanoid");
			SKID_hOwMaNySkIdS(SkidState, 20);
			SKID_setSkId(SkidState, -2, "WalkSpeed");
			std::string music = "";
			SKID_globalSkId(SkidState, "workspace");
			SKID_globalSkId(SkidState, "Instance");
			SKID_getSkId(SkidState, -1, "new");
			SKID_pushSkId(SkidState, "Sound");
			SKID_SkIdvalue(SkidState, -4);
			SKID_SkId(SkidState, 2, 1);;

			SKID_pushSkId(SkidState, ("rbxassetid://" + music).c_str());
			SKID_setSkId(SkidState, -2, "SoundId");
		}


		if (toSkId(In.at(0)) == "mlg") {

			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Torso");
			SKID_globalSkId(SkidState, "Instance");
			SKID_getSkId(SkidState, -1, "new");
			SKID_pushSkId(SkidState, "SpecialMesh");
			SKID_SkIdvalue(SkidState, -4);
			SKID_SkId(SkidState, 2, 1);;
			std::string MeshId = "rbxassetid://430452142";
			std::string TextureId = "rbxassetid://430452215";

			SKID_pushSkId(SkidState, TextureId.c_str());
			SKID_setSkId(SkidState, -2, "TextureId");
			SKID_pushSkId(SkidState, MeshId.c_str());;
			SKID_setSkId(SkidState, -2, "MeshId");
			SKID_globalSkId(SkidState, "Vector3");
			SKID_getSkId(SkidState, -1, "new");
			SKID_hOwMaNySkIdS(SkidState, 4);
			SKID_hOwMaNySkIdS(SkidState, 4);
			SKID_hOwMaNySkIdS(SkidState, 4);
			SKID_SkId(SkidState, 3, 1);;
			SKID_setSkId(SkidState, -3, "Scale");

			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Humanoid");
			SKID_pushSkId(SkidState, "17");
			SKID_setSkId(SkidState, -2, "HipHeight");

			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Left Leg");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Right Leg");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Left Arm");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (In.at(1) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Right Arm");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Head");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Humanoid");
			SKID_hOwMaNySkIdS(SkidState, 50);
			SKID_setSkId(SkidState, -2, "WalkSpeed");
			std::string music = "";
			SKID_globalSkId(SkidState, "workspace");
			SKID_globalSkId(SkidState, "Instance");
			SKID_getSkId(SkidState, -1, "new");
			SKID_pushSkId(SkidState, "Sound");
			SKID_SkIdvalue(SkidState, -4);
			SKID_SkId(SkidState, 2, 1);;

			SKID_pushSkId(SkidState, ("rbxassetid://" + music).c_str());
			SKID_setSkId(SkidState, -2, "SoundId");
		}


		if (toSkId(In.at(0)) == "pussy") {

			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Torso");
			SKID_globalSkId(SkidState, "Instance");
			SKID_getSkId(SkidState, -1, "new");
			SKID_pushSkId(SkidState, "SpecialMesh");
			SKID_SkIdvalue(SkidState, -4);
			SKID_SkId(SkidState, 2, 1);;
			std::string MeshId = "rbxassetid://477053700";
			std::string TextureId = "rbxassetid://477053711";

			SKID_pushSkId(SkidState, TextureId.c_str());
			SKID_setSkId(SkidState, -2, "TextureId");
			SKID_pushSkId(SkidState, MeshId.c_str());;
			SKID_setSkId(SkidState, -2, "MeshId");
			SKID_globalSkId(SkidState, "Vector3");
			SKID_getSkId(SkidState, -1, "new");
			SKID_hOwMaNySkIdS(SkidState, 4);
			SKID_hOwMaNySkIdS(SkidState, 4);
			SKID_hOwMaNySkIdS(SkidState, 4);
			SKID_SkId(SkidState, 3, 1);;
			SKID_setSkId(SkidState, -3, "Scale");

			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Humanoid");
			SKID_pushSkId(SkidState, "17");
			SKID_setSkId(SkidState, -2, "HipHeight");

			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Left Leg");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Right Leg");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Left Arm");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (In.at(1) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Right Arm");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Head");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Transparency");
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Players");
			if (toSkId(In.at(1)) == "me") {
				SKID_getSkId(SkidState, -1, "LocalPlayer");
			}
			else {
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
			}
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "Humanoid");
			SKID_hOwMaNySkIdS(SkidState, 50);
			SKID_setSkId(SkidState, -2, "WalkSpeed");
			std::string music = "";
			SKID_globalSkId(SkidState, "workspace");
			SKID_globalSkId(SkidState, "Instance");
			SKID_getSkId(SkidState, -1, "new");
			SKID_pushSkId(SkidState, "Sound");
			SKID_SkIdvalue(SkidState, -4);
			SKID_SkId(SkidState, 2, 1);;

			SKID_pushSkId(SkidState, ("rbxassetid://" + music).c_str());
			SKID_setSkId(SkidState, -2, "SoundId");
		}


		if (toSkId(In.at(0)) == "fog") {
			SKID_globalSkId(SkidState, "Game");
			SKID_getSkId(SkidState, -1, "GetService");
			SKID_SkIdvalue(SkidState, -2);
			SKID_pushSkId(SkidState, "Lighting");
			SKID_SkId(SkidState, 2, 1);;
			SKID_hOwMaNySkIdS(SkidState, stof(In.at(1).c_str()));
			SKID_setSkId(SkidState, -2, "FogEnd");
		}


		if (toSkId(In.at(0)) == "rfog") {
			SKID_globalSkId(SkidState, "Game");
			SKID_getSkId(SkidState, -1, "GetService");
			SKID_SkIdvalue(SkidState, -2);
			SKID_pushSkId(SkidState, "Lighting");
			SKID_SkId(SkidState, 2, 1);;
			SKID_hOwMaNySkIdS(SkidState, 1000000);
			SKID_setSkId(SkidState, -2, "FogEnd");
		}


		if (toSkId(In.at(0)) == "rhat") {
			if (toSkId(In.at(1)) == "me" || toSkId(In.at(1)) == "localplr") {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Head");

				SKID_getSkId(SkidState, LUA_GLOBALSKID, "Vector3");
				SKID_getSkId(SkidState, -1, "new");
				SKID_hOwMaNySkIdS(SkidState, 1);
				SKID_hOwMaNySkIdS(SkidState, 1);
				SKID_hOwMaNySkIdS(SkidState, 1);
				SKID_hOwMaNySkIdS(SkidState, 1);
				SKID_hOwMaNySkIdS(SkidState, 1);
				SKID_hOwMaNySkIdS(SkidState, 1);
				SKID_SkId(SkidState, 6, 1);;
				SKID_setSkId(SkidState, -3, "Size");
			}
			else {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Head");

				SKID_getSkId(SkidState, LUA_GLOBALSKID, "Vector3");
				SKID_getSkId(SkidState, -1, "new");
				SKID_hOwMaNySkIdS(SkidState, 1);
				SKID_hOwMaNySkIdS(SkidState, 1);
				SKID_hOwMaNySkIdS(SkidState, 1);
				SKID_hOwMaNySkIdS(SkidState, 1);
				SKID_hOwMaNySkIdS(SkidState, 1);
				SKID_hOwMaNySkIdS(SkidState, 1);
				SKID_SkId(SkidState, 6, 1);;
				SKID_setSkId(SkidState, -3, "Size");
			}
		}


		if (toSkId(In.at(0)) == "ws") {
			if (toSkId(In.at(1)) == "me" || toSkId(In.at(1)) == "localplr") {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Humanoid");
				SKID_hOwMaNySkIdS(SkidState, stof(In.at(2).c_str()));
				SKID_setSkId(SkidState, -2, "WalkSpeed");
			}
			else {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Humanoid");
				SKID_hOwMaNySkIdS(SkidState, stof(In.at(2).c_str()));
				SKID_setSkId(SkidState, -2, "WalkSpeed");
			}
		}


		if (toSkId(In.at(0)) == "sit") {
			if (toSkId(In.at(1)) == "me" || toSkId(In.at(1)) == "localplr") {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Humanoid");
				SKID_hOwMaNySkIdS(SkidState, 1);
				SKID_setSkId(SkidState, -2, "Sit");
			}
			else {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Humanoid");
				SKID_hOwMaNySkIdS(SkidState, 1);
				SKID_setSkId(SkidState, -2, "Sit");
			}
		}


		if (toSkId(In.at(0)) == "hipheight") {
			if (toSkId(In.at(1)) == "me" || toSkId(In.at(1)) == "localplr") {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Humanoid");
				SKID_hOwMaNySkIdS(SkidState, stof(In.at(2).c_str()));
				SKID_setSkId(SkidState, -2, "HipHeight");
			}
			else {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Humanoid");
				SKID_hOwMaNySkIdS(SkidState, stof(In.at(2).c_str()));
				SKID_setSkId(SkidState, -2, "HipHeight");
			}
		}


		if (toSkId(In.at(0)) == "jp") {
			if (toSkId(In.at(1)) == "me" || toSkId(In.at(1)) == "localplr") {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Humanoid");
				SKID_hOwMaNySkIdS(SkidState, stof(In.at(2).c_str()));
				SKID_setSkId(SkidState, -2, "JumpPower");
			}
			else {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Humanoid");
				SKID_hOwMaNySkIdS(SkidState, stof(In.at(2).c_str()));
				SKID_setSkId(SkidState, -2, "JumpPower");
			}
		}


		if (toSkId(In.at(0)) == "kill") {
			if (toSkId(In.at(1)) == "me" || toSkId(In.at(1)) == "localplr") {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Humanoid");
				SKID_pushSkId(SkidState, "0");
				SKID_setSkId(SkidState, -2, "Health");
			}
			else {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Humanoid");
				SKID_pushSkId(SkidState, "0");
				SKID_setSkId(SkidState, -2, "Health");
			}
		}


		if (toSkId(In.at(0)) == "drivebloxmoney") {
			if (toSkId(In.at(1)) == "me" || toSkId(In.at(1)) == "localplr") {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Playerdata");
				SKID_getSkId(SkidState, -1, "Money");
				SKID_hOwMaNySkIdS(SkidState, 999999999);
				SKID_setSkId(SkidState, -2, "Value");
			}
			else {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Playerdata");
				SKID_getSkId(SkidState, -1, "Money");
				SKID_hOwMaNySkIdS(SkidState, 999999999);
				SKID_setSkId(SkidState, -2, "Value");
			}
		}


		if (toSkId(In.at(0)) == "gravity") {
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Workspace");
			SKID_hOwMaNySkIdS(SkidState, stof(In.at(1).c_str()));
			SKID_setSkId(SkidState, -2, "Gravity");
			cout << ">-> ";
		}


		if (toSkId(In.at(0)) == "btools") {
			if (toSkId(In.at(1)) == "me" || toSkId(In.at(1)) == "localplr") {
				for (int i = 1; i <= 4; i++) {
					getSKID("Players");
					SKID_getSkId(SkidState, -1, "LocalPlayer");
					SKID_getSkId(SkidState, -1, "Backpack");
					SKID_getSkId(SkidState, LUA_GLOBALSKID, "Instance");
					SKID_getSkId(SkidState, -1, "new");
					SKID_pushSkId(SkidState, "HopperBin");
					SKID_SkIdvalue(SkidState, -4);
					SKID_SkId(SkidState, 2, 1);;

					SKID_hOwMaNySkIdS(SkidState, i);
					SKID_setSkId(SkidState, -2, "Name");
					SKID_hOwMaNySkIdS(SkidState, i);
					SKID_setSkId(SkidState, -2, "BinType");
				}
			}
			else {
				for (int i = 1; i <= 4; i++) {
					getSKID("Players");
					SKID_getSkId(SkidState, -1, In.at(1).c_str());
					SKID_getSkId(SkidState, -1, "Backpack");
					SKID_getSkId(SkidState, LUA_GLOBALSKID, "Instance");
					SKID_getSkId(SkidState, -1, "new");
					SKID_pushSkId(SkidState, "HopperBin");
					SKID_SkIdvalue(SkidState, -4);
					SKID_SkId(SkidState, 2, 1);;

					SKID_hOwMaNySkIdS(SkidState, i);
					SKID_setSkId(SkidState, -2, "Name");
					SKID_hOwMaNySkIdS(SkidState, i);
					SKID_setSkId(SkidState, -2, "BinType");
				}
			}
		}


		if (toSkId(In.at(0)) == "god") {
			if (toSkId(In.at(1)) == "me" || toSkId(In.at(1)) == "localplr") {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Humanoid");
				SKID_hOwMaNySkIdS(SkidState, HUGE_VAL);
				SKID_setSkId(SkidState, -2, "MaxHealth");
			}
			else {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Humanoid");
				SKID_hOwMaNySkIdS(SkidState, HUGE_VAL);
				SKID_setSkId(SkidState, -2, "MaxHealth");
			}
		}
		if (toSkId(In.at(0)) == "bigfire" || toSkId(In.at(0)) == "bodyfire") {
			if (toSkId(In.at(1)) == "me" || toSkId(In.at(1)) == "localplr") {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Torso");
				SKID_getSkId(SkidState, LUA_GLOBALSKID, "Instance");
				SKID_getSkId(SkidState, -1, "new");
				SKID_pushSkId(SkidState, "Fire");
				SKID_SkIdvalue(SkidState, -4);
				SKID_SkId(SkidState, 2, 1);;
				SKID_pushSkId(SkidState, "29");
				SKID_setSkId(SkidState, -2, "Size");
			}
			else {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Torso");
				SKID_getSkId(SkidState, LUA_GLOBALSKID, "Instance");
				SKID_getSkId(SkidState, -1, "new");
				SKID_pushSkId(SkidState, "Fire");
				SKID_SkIdvalue(SkidState, -4);
				SKID_SkId(SkidState, 2, 1);;
				SKID_pushSkId(SkidState, "29");
				SKID_setSkId(SkidState, -2, "Size");
			}
		}


		if (toSkId(In.at(0)) == "time") {
				getSKID("Lighting");
				SKID_hOwMaNySkIdS(SkidState, atoi(In.at(1).c_str()));
				SKID_setSkId(SkidState, -2, "TimeOfDay");
		}


		if (toSkId(In.at(0)) == "select" || toSkId(In.at(0)) == "box") {
			if (toSkId(In.at(1)) == "me") {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, LUA_GLOBALSKID, "Instance");
				SKID_getSkId(SkidState, -1, "new");
				SKID_pushSkId(SkidState, "SelectionBox");
				SKID_SkIdvalue(SkidState, -4);
				SKID_SkId(SkidState, 2, 1);;

				SKID_SkIdvalue(SkidState, -3);
				SKID_setSkId(SkidState, -2, "Adornee");

			}
			else {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, LUA_GLOBALSKID, "Instance");
				SKID_getSkId(SkidState, -1, "new");
				SKID_pushSkId(SkidState, "SelectionBox");
				SKID_SkIdvalue(SkidState, -4);
				SKID_SkId(SkidState, 2, 1);;

				SKID_SkIdvalue(SkidState, -3);
				SKID_setSkId(SkidState, -2, "Adornee");
			}
		}


		if (toSkId(In.at(0)) == "fencingr" || toSkId(In.at(0)) == "freach") {
			getSKID("Players");
			SKID_getSkId(SkidState, -1, "LocalPlayer");
			SKID_getSkId(SkidState, -1, "Backpack");
			SKID_getSkId(SkidState, -1, "Foil");
			SKID_getSkId(SkidState, -1, "Handle");

			SKID_getSkId(SkidState, LUA_GLOBALSKID, "Instance");
			SKID_getSkId(SkidState, -1, "new");
			SKID_pushSkId(SkidState, "SelectionBox");
			SKID_SkIdvalue(SkidState, -4);
			SKID_SkId(SkidState, 2, 1);;

			SKID_SkIdvalue(SkidState, -3);
			SKID_setSkId(SkidState, -2, "Adornee");

			getSKID("Players");
			SKID_getSkId(SkidState, -1, "LocalPlayer");
			SKID_getSkId(SkidState, -1, "Backpack");
			SKID_getSkId(SkidState, -1, "Foil");
			SKID_getSkId(SkidState, -1, "Handle");

			getSKID("Players");
			SKID_getSkId(SkidState, -1, "LocalPlayer");
			SKID_getSkId(SkidState, -1, "Backpack");
			SKID_getSkId(SkidState, -1, "Foil");
			SKID_getSkId(SkidState, -1, "Handle");

			SKID_getSkId(SkidState, LUA_GLOBALSKID, "Vector3");
			SKID_getSkId(SkidState, -1, "new");
			SKID_pushSkId(SkidState, "1");
			SKID_pushSkId(SkidState, "1");
			SKID_pushSkId(SkidState, "30");
			SKID_SkId(SkidState, 3, 1);;
			SKID_setSkId(SkidState, -7, "Size");
		}


		if (toSkId(In.at(0)) == "forcechat" || toSkId(In.at(0)) == "fchat") {
			if (toSkId(In.at(1)) == "me") {
				if (toSkId(In.at(2)) == "green") {
					std::string string;
					for (size_t i = 3; i < In.size(); i++) {
						if (i < (In.size() - 1)) {
							string.append(In.at(i) + ' ');
						}
						if (i == (In.size() - 1)) {
							string.append(In.at(i));
						}
					}
					getSKID("Players");
					SKID_getSkId(SkidState, -1, "LocalPlayer");
					SKID_getSkId(SkidState, -1, "Character");
					SKID_getSkId(SkidState, -1, "Head");

					SKID_getSkId(SkidState, LUA_GLOBALSKID, "game");
					SKID_getSkId(SkidState, -1, "Chat");
					SKID_getSkId(SkidState, -1, "Chat");
					SKID_SkIdvalue(SkidState, -2);
					SKID_SkIdvalue(SkidState, -5);
					SKID_pushSkId(SkidState, string.c_str());
					SKID_pushSkId(SkidState, "Green");
					SKID_SkId(SkidState, 4, 1);;

				}
				if (toSkId(In.at(2)) == "red") {
					std::string string;
					for (size_t i = 3; i < In.size(); i++) {
						if (i < (In.size() - 1)) {
							string.append(In.at(i) + ' ');
						}
						if (i == (In.size() - 1)) {
							string.append(In.at(i));
						}
					}
					getSKID("Players");
					SKID_getSkId(SkidState, -1, "LocalPlayer");
					SKID_getSkId(SkidState, -1, "Character");
					SKID_getSkId(SkidState, -1, "Head");

					SKID_getSkId(SkidState, LUA_GLOBALSKID, "game");
					SKID_getSkId(SkidState, -1, "Chat");
					SKID_getSkId(SkidState, -1, "Chat");
					SKID_SkIdvalue(SkidState, -2);
					SKID_SkIdvalue(SkidState, -5);
					SKID_pushSkId(SkidState, string.c_str());
					SKID_pushSkId(SkidState, "Red");
					SKID_SkId(SkidState, 4, 1);;
				}
				if (toSkId(In.at(2)) == "blue") {
					std::string string;
					for (size_t i = 3; i < In.size(); i++) {
						if (i < (In.size() - 1)) {
							string.append(In.at(i) + ' ');
						}
						if (i == (In.size() - 1)) {
							string.append(In.at(i));
						}
					}
					getSKID("Players");
					SKID_getSkId(SkidState, -1, "LocalPlayer");
					SKID_getSkId(SkidState, -1, "Character");
					SKID_getSkId(SkidState, -1, "Head");

					SKID_getSkId(SkidState, LUA_GLOBALSKID, "game");
					SKID_getSkId(SkidState, -1, "Chat");
					SKID_getSkId(SkidState, -1, "Chat");
					SKID_SkIdvalue(SkidState, -2);
					SKID_SkIdvalue(SkidState, -5);
					SKID_pushSkId(SkidState, string.c_str());
					SKID_pushSkId(SkidState, "Blue");
					SKID_SkId(SkidState, 4, 1);;
				}
			}
			else {
				if (toSkId(In.at(2)) == "green") {
					std::string string;
					for (size_t i = 3; i < In.size(); i++) {
						if (i < (In.size() - 1)) {
							string.append(In.at(i) + ' ');
						}
						if (i == (In.size() - 1)) {
							string.append(In.at(i));
						}
					}
					getSKID("Players");
					SKID_getSkId(SkidState, -1, In.at(1).c_str());
					SKID_getSkId(SkidState, -1, "Character");
					SKID_getSkId(SkidState, -1, "Head");

					SKID_getSkId(SkidState, LUA_GLOBALSKID, "game");
					SKID_getSkId(SkidState, -1, "Chat");
					SKID_getSkId(SkidState, -1, "Chat");
					SKID_SkIdvalue(SkidState, -2);
					SKID_SkIdvalue(SkidState, -5);
					SKID_pushSkId(SkidState, string.c_str());
					SKID_pushSkId(SkidState, "Green");
					SKID_SkId(SkidState, 4, 1);;
				}
				if (toSkId(In.at(2)) == "red") {
					std::string string;
					for (size_t i = 3; i < In.size(); i++) {
						if (i < (In.size() - 1)) {
							string.append(In.at(i) + ' ');
						}
						if (i == (In.size() - 1)) {
							string.append(In.at(i));
						}
					}
					getSKID("Players");
					SKID_getSkId(SkidState, -1, In.at(1).c_str());
					SKID_getSkId(SkidState, -1, "Character");
					SKID_getSkId(SkidState, -1, "Head");

					SKID_getSkId(SkidState, LUA_GLOBALSKID, "game");
					SKID_getSkId(SkidState, -1, "Chat");
					SKID_getSkId(SkidState, -1, "Chat");
					SKID_SkIdvalue(SkidState, -2);
					SKID_SkIdvalue(SkidState, -5);
					SKID_pushSkId(SkidState, string.c_str());
					SKID_pushSkId(SkidState, "Red");
					SKID_SkId(SkidState, 4, 1);;
				}
				if (toSkId(In.at(2)) == "blue") {
					std::string string;
					for (size_t i = 3; i < In.size(); i++) {
						if (i < (In.size() - 1)) {
							string.append(In.at(i) + ' ');
						}
						if (i == (In.size() - 1)) {
							string.append(In.at(i));
						}
					}
					getSKID("Players");
					SKID_getSkId(SkidState, -1, In.at(1).c_str());
					SKID_getSkId(SkidState, -1, "Character");
					SKID_getSkId(SkidState, -1, "Head");

					SKID_getSkId(SkidState, LUA_GLOBALSKID, "game");
					SKID_getSkId(SkidState, -1, "Chat");
					SKID_getSkId(SkidState, -1, "Chat");
					SKID_SkIdvalue(SkidState, -2);
					SKID_SkIdvalue(SkidState, -5);
					SKID_pushSkId(SkidState, string.c_str());
					SKID_pushSkId(SkidState, "Blue");
					SKID_SkId(SkidState, 4, 1);;
				}
			}
		}


		if (toSkId(In.at(0)) == "charapp" || toSkId(In.at(0)) == "char") {
			if (toSkId(In.at(1)) == "me" || toSkId(In.at(1)) == "localplr") {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				std::string InitialID = "http://www.roblox.com/Asset/CharacterFetch.ashx?userId=" + In.at(2);
				SKID_pushSkId(SkidState, InitialID.c_str());
				SKID_setSkId(SkidState, -2, "CharacterAppearance");
			}
			else {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				std::string InitialID = "http://www.roblox.com/Asset/CharacterFetch.ashx?userId=" + In.at(2);
				SKID_pushSkId(SkidState, InitialID.c_str());
				SKID_setSkId(SkidState, -2, "CharacterAppearance");
			}
		}


		if (toSkId(In.at(0)) == "noob") {
			if (toSkId(In.at(1)) == "me" || toSkId(In.at(1)) == "localplr") {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				std::string InitialID = "http://www.roblox.com/Asset/CharacterFetch.ashx?userId=5";
				SKID_pushSkId(SkidState, InitialID.c_str());
				SKID_setSkId(SkidState, -2, "CharacterAppearance");
			}
			else {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				std::string InitialID = "http://www.roblox.com/Asset/CharacterFetch.ashx?userId=5";
				SKID_pushSkId(SkidState, InitialID.c_str());
				SKID_setSkId(SkidState, -2, "CharacterAppearance");
			}
		}


		if (toSkId(In.at(0)) == "fire") {
			if (toSkId(In.at(1)) == "me" || toSkId(In.at(1)) == "localplr") {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Torso");
				SKID_getSkId(SkidState, LUA_GLOBALSKID, "Instance");
				SKID_getSkId(SkidState, -1, "new");
				SKID_pushSkId(SkidState, "Fire");
				SKID_SkIdvalue(SkidState, -4);
				SKID_SkId(SkidState, 2, 1);;
			}
			else {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Torso");
				SKID_getSkId(SkidState, LUA_GLOBALSKID, "Instance");
				SKID_getSkId(SkidState, -1, "new");
				SKID_pushSkId(SkidState, "Fire");
				SKID_SkIdvalue(SkidState, -4);
				SKID_SkId(SkidState, 2, 1);;
			}
		}


		if (toSkId(In.at(0)) == "smoke") {
			if (toSkId(In.at(1)) == "me" || toSkId(In.at(1)) == "localplr") {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Torso");
				SKID_getSkId(SkidState, LUA_GLOBALSKID, "Instance");
				SKID_getSkId(SkidState, -1, "new");
				SKID_pushSkId(SkidState, "Smoke");
				SKID_SkIdvalue(SkidState, -4);
				SKID_SkId(SkidState, 2, 1);;
			}
			else {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Torso");
				SKID_getSkId(SkidState, LUA_GLOBALSKID, "Instance");
				SKID_getSkId(SkidState, -1, "new");
				SKID_pushSkId(SkidState, "Smoke");
				SKID_SkIdvalue(SkidState, -4);
				SKID_SkId(SkidState, 2, 1);;
			}
		}


		if (toSkId(In.at(0)) == "sethealth" || toSkId(In.at(0)) == "shealth") {
			if (toSkId(In.at(1)) == "me" || toSkId(In.at(1)) == "localplr") {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_hOwMaNySkIdS(SkidState, atoi(In.at(2).c_str()));
				SKID_setSkId(SkidState, -2, "MaxHealth");
			}
			else {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_hOwMaNySkIdS(SkidState, atoi(In.at(2).c_str()));
				SKID_setSkId(SkidState, -2, "MaxHealth");
			}
		}

		if (toSkId(In.at(0)) == "sparkles" || toSkId(In.at(0)) == "sp") {
			if (toSkId(In.at(1)) == "me" || toSkId(In.at(1)) == "localplr") {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, "LocalPlayer");
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Torso");
				SKID_getSkId(SkidState, LUA_GLOBALSKID, "Instance");
				SKID_getSkId(SkidState, -1, "new");
				SKID_pushSkId(SkidState, "Sparkles");
				SKID_SkIdvalue(SkidState, -4);
				SKID_SkId(SkidState, 2, 1);;
				SKID_pushSkId(SkidState, "Ice Sparkles");
				SKID_setSkId(SkidState, -2, "Name");
			}
			else {
				getSKID("Players");
				SKID_getSkId(SkidState, -1, In.at(1).c_str());
				SKID_getSkId(SkidState, -1, "Character");
				SKID_getSkId(SkidState, -1, "Torso");
				SKID_getSkId(SkidState, LUA_GLOBALSKID, "Instance");
				SKID_getSkId(SkidState, -1, "new");
				SKID_pushSkId(SkidState, "Sparkles");
				SKID_SkIdvalue(SkidState, -4);
				SKID_SkId(SkidState, 2, 1);;
				SKID_pushSkId(SkidState, "Ice Sparkles");
				SKID_setSkId(SkidState, -2, "Name");
			}
		}

		if (toSkId(In.at(0)) == "criminal") {
			getSKID("Players");
			SKID_getSkId(SkidState, -1, "LocalPlayer");
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "HumanoidRootPart");
			SKID_globalSkId(SkidState, "CFrame");
			SKID_getSkId(SkidState, -1, "new");
			SKID_hOwMaNySkIdS(SkidState, -222.339157);
			SKID_hOwMaNySkIdS(SkidState, 17.9125443);
			SKID_hOwMaNySkIdS(SkidState, 1575.82336);
			SKID_SkId(SkidState, 3, 1);;
			SKID_setSkId(SkidState, -3, "CFrame");
		}


		if (toSkId(In.at(0)) == "garage") {
			getSKID("Players");
			SKID_getSkId(SkidState, -1, "LocalPlayer");
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "HumanoidRootPart");
			SKID_globalSkId(SkidState, "CFrame");
			SKID_getSkId(SkidState, -1, "new");
			SKID_hOwMaNySkIdS(SkidState, -342.130798);
			SKID_hOwMaNySkIdS(SkidState, 19.3135223);
			SKID_hOwMaNySkIdS(SkidState, 1182.73669);
			SKID_SkId(SkidState, 3, 1);;
			SKID_setSkId(SkidState, -3, "CFrame");
		}


		if (toSkId(In.at(0)) == "bank") {
			getSKID("Players");
			SKID_getSkId(SkidState, -1, "LocalPlayer");
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "HumanoidRootPart");
			SKID_globalSkId(SkidState, "CFrame");
			SKID_getSkId(SkidState, -1, "new");
			SKID_hOwMaNySkIdS(SkidState, 1.1110189);
			SKID_hOwMaNySkIdS(SkidState, 17.9213142);
			SKID_hOwMaNySkIdS(SkidState, 782.103455);
			SKID_SkId(SkidState, 3, 1);;
			SKID_setSkId(SkidState, -3, "CFrame");
		}


		if (toSkId(In.at(0)) == "prison") {
			getSKID("Players");
			SKID_getSkId(SkidState, -1, "LocalPlayer");
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "HumanoidRootPart");
			SKID_globalSkId(SkidState, "CFrame");
			SKID_getSkId(SkidState, -1, "new");
			SKID_hOwMaNySkIdS(SkidState, -1100.4469);
			SKID_hOwMaNySkIdS(SkidState, 17.7500038);
			SKID_hOwMaNySkIdS(SkidState, -1493.18286);
			SKID_SkId(SkidState, 3, 1);;
			SKID_setSkId(SkidState, -3, "CFrame");
		}


		if (toSkId(In.at(0)) == "nodoors") {
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Workspace");
			SKID_getSkId(SkidState, -1, "Doors");
			SKID_getSkId(SkidState, -1, "Destroy");
			SKID_SkIdvalue(SkidState, -2);
			SKID_SkId(SkidState, 1, 0);;
		}

		if (toSkId(In.at(0)) == "banklazers") {
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Workspace");
			SKID_getSkId(SkidState, -1, "Banks");
			SKID_getSkId(SkidState, -1, "Bank");
			SKID_getSkId(SkidState, -1, "Lasers");
			SKID_getSkId(SkidState, -1, "Destroy");
			SKID_SkIdvalue(SkidState, -2);
			SKID_SkId(SkidState, 1, 0);;
		}

		if (toSkId(In.at(0)) == "jewelrycameras") {
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Workspace");
			SKID_getSkId(SkidState, -1, "Jewelrys");
			SKID_getSkId(SkidState, -1, "Jewelry");
			SKID_getSkId(SkidState, -1, "Cameras");
			SKID_getSkId(SkidState, -1, "Destroy");
			SKID_SkIdvalue(SkidState, -2);
			SKID_SkId(SkidState, 1, 0);;
		}

		if (toSkId(In.at(0)) == "jewelrylazers") {
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Workspace");
			SKID_getSkId(SkidState, -1, "Jewelrys");
			SKID_getSkId(SkidState, -1, "Jewelry");
			SKID_getSkId(SkidState, -1, "Lasers");
			SKID_getSkId(SkidState, -1, "Destroy");
			SKID_SkIdvalue(SkidState, -2);
			SKID_SkId(SkidState, 1, 0);;
		}

		if (toSkId(In.at(0)) == "jewelryflazers") {
			SKID_globalSkId(SkidState, "game");
			SKID_getSkId(SkidState, -1, "Workspace");
			SKID_getSkId(SkidState, -1, "Jewelrys");
			SKID_getSkId(SkidState, -1, "Jewelry");
			SKID_getSkId(SkidState, -1, "FloorLasers");
			SKID_getSkId(SkidState, -1, "Destroy");
			SKID_SkIdvalue(SkidState, -2);
			SKID_SkId(SkidState, 1, 0);;
		}

		if (toSkId(In.at(0)) == "jewelry") {
			getSKID("Players");
			SKID_getSkId(SkidState, -1, "LocalPlayer");
			SKID_getSkId(SkidState, -1, "Character");
			SKID_getSkId(SkidState, -1, "HumanoidRootPart");
			SKID_globalSkId(SkidState, "CFrame");
			SKID_getSkId(SkidState, -1, "new");
			SKID_hOwMaNySkIdS(SkidState, 124);
			SKID_hOwMaNySkIdS(SkidState, 17);
			SKID_hOwMaNySkIdS(SkidState, 1317);
			SKID_SkId(SkidState, 3, 1);;
			SKID_setSkId(SkidState, -3, "CFrame");
		}

		if (toSkId(In.at(0)) == "rickroll") {
			SKID_globalSkId(SkidState, "Workspace");
			SKID_globalSkId(SkidState, "Instance");
			SKID_getSkId(SkidState, -1, "new");
			SKID_pushSkId(SkidState, "Sound");
			SKID_SkIdvalue(SkidState, -4);
			SKID_SkId(SkidState, 2, 0);;

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "Sound");
			SKID_pushSkId(SkidState, "LuaC_Music");
			SKID_setSkId(SkidState, -2, "Name");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Volume");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_pushSkId(SkidState, "rbxassetid://151758509");
			SKID_setSkId(SkidState, -2, "SoundId");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_getSkId(SkidState, -1, "Play");
			SKID_SkIdvalue(SkidState, -2);
			SKID_SkId(SkidState, 1, 0);;
		}


		if (toSkId(In.at(0)) == "ppap") {
			SKID_globalSkId(SkidState, "Workspace");
			SKID_globalSkId(SkidState, "Instance");
			SKID_getSkId(SkidState, -1, "new");
			SKID_pushSkId(SkidState, "Sound");
			SKID_SkIdvalue(SkidState, -4);
			SKID_SkId(SkidState, 2, 0);;

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "Sound");
			SKID_pushSkId(SkidState, "LuaC_Music");
			SKID_setSkId(SkidState, -2, "Name");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Volume");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_pushSkId(SkidState, "rbxassetid://514558366");
			SKID_setSkId(SkidState, -2, "SoundId");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_getSkId(SkidState, -1, "Play");
			SKID_SkIdvalue(SkidState, -2);
			SKID_SkId(SkidState, 1, 0);;
		}


		if (toSkId(In.at(0)) == "billnye") {
			SKID_globalSkId(SkidState, "Workspace");
			SKID_globalSkId(SkidState, "Instance");
			SKID_getSkId(SkidState, -1, "new");
			SKID_pushSkId(SkidState, "Sound");
			SKID_SkIdvalue(SkidState, -4);
			SKID_SkId(SkidState, 2, 0);;

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "Sound");
			SKID_pushSkId(SkidState, "LuaC_Music");
			SKID_setSkId(SkidState, -2, "Name");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Volume");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_pushSkId(SkidState, "rbxassetid://318733059");
			SKID_setSkId(SkidState, -2, "SoundId");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_getSkId(SkidState, -1, "Play");
			SKID_SkIdvalue(SkidState, -2);
			SKID_SkId(SkidState, 1, 0);;
		}


		if (toSkId(In.at(0)) == "illuminati") {
			SKID_globalSkId(SkidState, "Workspace");
			SKID_globalSkId(SkidState, "Instance");
			SKID_getSkId(SkidState, -1, "new");
			SKID_pushSkId(SkidState, "Sound");
			SKID_SkIdvalue(SkidState, -4);
			SKID_SkId(SkidState, 2, 0);;

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "Sound");
			SKID_pushSkId(SkidState, "LuaC_Music");
			SKID_setSkId(SkidState, -2, "Name");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Volume");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_pushSkId(SkidState, "rbxassetid://271908621");
			SKID_setSkId(SkidState, -2, "SoundId");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_getSkId(SkidState, -1, "Play");
			SKID_SkIdvalue(SkidState, -2);
			SKID_SkId(SkidState, 1, 0);;
		}


		if (toSkId(In.at(0)) == "rage") {
			SKID_globalSkId(SkidState, "Workspace");
			SKID_globalSkId(SkidState, "Instance");
			SKID_getSkId(SkidState, -1, "new");
			SKID_pushSkId(SkidState, "Sound");
			SKID_SkIdvalue(SkidState, -4);
			SKID_SkId(SkidState, 2, 0);;

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "Sound");
			SKID_pushSkId(SkidState, "LuaC_Music");
			SKID_setSkId(SkidState, -2, "Name");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Volume");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_pushSkId(SkidState, "rbxassetid://382795033");
			SKID_setSkId(SkidState, -2, "SoundId");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_getSkId(SkidState, -1, "Play");
			SKID_SkIdvalue(SkidState, -2);
			SKID_SkId(SkidState, 1, 0);;
		}


		if (toSkId(In.at(0)) == "cringe") {
			SKID_globalSkId(SkidState, "Workspace");
			SKID_globalSkId(SkidState, "Instance");
			SKID_getSkId(SkidState, -1, "new");
			SKID_pushSkId(SkidState, "Sound");
			SKID_SkIdvalue(SkidState, -4);
			SKID_SkId(SkidState, 2, 0);;

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "Sound");
			SKID_pushSkId(SkidState, "LuaC_Music");
			SKID_setSkId(SkidState, -2, "Name");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Volume");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_pushSkId(SkidState, "rbxassetid://555159200");
			SKID_setSkId(SkidState, -2, "SoundId");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_getSkId(SkidState, -1, "Play");
			SKID_SkIdvalue(SkidState, -2);
			SKID_SkId(SkidState, 1, 0);;
		}

		if (toSkId(In.at(0)) == "clearws") {
			getSKID("Workspace");
			SKID_getSkId(SkidState, -1, "ClearAllChildren");
			SKID_SkIdvalue(SkidState, -2);
			SKID_SkId(SkidState, 1, 0);;
		}

		if (toSkId(In.at(0)) == "fecheck") {
			SKID_globalSkId(SkidState, "workspace");
			SKID_getSkId(SkidState, -1, "FilteringEnabled");
			SKID_globalSkId(SkidState, "print");
			SKID_SkIdvalue(SkidState, -2);
			SKID_SkId(SkidState, 1, 0);;
		}

		if (toSkId(In.at(0)) == "play") {
			SKID_globalSkId(SkidState, "Workspace");
			SKID_globalSkId(SkidState, "Instance");
			SKID_getSkId(SkidState, -1, "new");
			SKID_pushSkId(SkidState, "Sound");
			SKID_SkIdvalue(SkidState, -4);
			SKID_SkId(SkidState, 2, 0);;

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "Sound");
			SKID_pushSkId(SkidState, "LuaC_Music");
			SKID_setSkId(SkidState, -2, "Name");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_hOwMaNySkIdS(SkidState, 1);
			SKID_setSkId(SkidState, -2, "Volume");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_pushSkId(SkidState, ("rbxassetid://" + In.at(1)).c_str());
			SKID_setSkId(SkidState, -2, "SoundId");

			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_getSkId(SkidState, -1, "Play");
			SKID_SkIdvalue(SkidState, -2);
			SKID_SkId(SkidState, 1, 0);;
		}

		if (toSkId(In.at(0)) == "stopmusic") {
			SKID_globalSkId(SkidState, "Workspace");
			SKID_getSkId(SkidState, -1, "LuaC_Music");
			SKID_getSkId(SkidState, -1, "Destroy");
			SKID_SkIdvalue(SkidState, -2);
			SKID_SkId(SkidState, 1, 0);;
		}

		SKID_SkIdtop(SkidState, 0);
	}
	else
	{
		return;
	}
}

std::vector<std::string> LuaCSplit(std::string s)
{
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	return vstrings;
}

void SkIdC(std::string input) {
	try {
		std::vector<std::string> In = LuaCSplit(input);

		if (In.at(0) == "getfield") {
			std::string field;
			for (size_t i = 2; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					field.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					field.append(In.at(i));
				}
			}
			SKID_getSkId(SkidState, stoi(In.at(1)), field.c_str());
		}

		else if (In.at(0) == "getglobal") {
			SKID_globalSkId(SkidState, In.at(1).c_str());
		}

		else if (In.at(0) == "setfield") {
			SKID_setSkId(SkidState, stoi(In.at(1)), In.at(2).c_str());
		}

		else if (In.at(0) == "pushvalue") {
			SKID_SkIdvalue(SkidState, stoi(In.at(1)));
		}

		else if (In.at(0) == "pushstring") {
			std::string string;
			for (size_t i = 1; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					string.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					string.append(In.at(i));
				}
			}
			SKID_pushSkId(SkidState, string.c_str());
		}

		else if (In.at(0) == "pushnumber") {
			SKID_hOwMaNySkIdS(SkidState, stoi(In.at(1)));
		}

		else if (In.at(0) == "pcall") {//this will be still pcall so you don't need to edit the lua c script
			SKID_SkId(SkidState, stoi(In.at(1)), stoi(In.at(2)));
		}

		else if (In.at(0) == "emptystack") {
			SKID_SkIdtop(SkidState, 0);
		}

		else if (In.at(0) == "settop") {
			SKID_SkIdtop(SkidState, stoi(In.at(1)));
		}

		else if (In.at(0) == "getservice") {
			getSKID(In.at(1).c_str());
		}

	}
	catch (...) {

	}
}
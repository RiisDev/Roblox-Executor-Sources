#include "Rosin.h"

using namespace Rosin;

using namespace Rosin::Roblox;

HMODULE GlobalDLLModule;

bool ProcessCommand(std::string Input) {
	return Rosin::CmdManager->ParseCommand(Input);
}

extern void LuaExecuteString(std::string Source);

int State = 0;
bool Changing = false;

bool rainbow = false;

double lolrain = 0;

void doRainbow() {
	if (rainbow == true) {
		float r, g, b;
		lolrain += 1;
		if (lolrain == 360) {
			lolrain = 0;
		}
		int R, G, B;
		HSV::HsvToRgb(lolrain, 1, 1, R, G, B);
		SetFogColor(Lighting, (float)R / 255, (float)G / 255, (float)B / 255); // Fixed :)
		Sleep(25);
	}
	else {
		lolrain = 0;
	}
}

void doState() {
	int LocalP = GetLocalPlayer(Players);
	int Char;
	if (LocalP) {
		Char = GetPlrCharacter(LocalP);
	}
	if (Changing == true) {
		if (Char != 0) {
			int Humanoid = FindFirstClass(Char, "Humanoid");
			if (Humanoid != 0) {
				SetHumanoidState(Humanoid, State);
			}
		}
	}
	Sleep(1);
}

bool breakStateLoop       = false;
bool breakRainbowLoop     = false;
bool breakLoopKill		  = false;
bool breakLoopCommandLoop = false;
bool breakSilentGodLoop	  = false;

void doStateLoop() {
	while (true) {
		if (Changing == false) std::this_thread::yield();
		if (breakStateLoop) { breakStateLoop = false; break; }
		doState();
		Sleep(1);
	}
	return;
}

void doRainbowLoop() {
	while (true) {
		if (rainbow == false) std::this_thread::yield();
		if (breakRainbowLoop) { breakRainbowLoop = false; break; }
		doRainbow();
		Sleep(1);
	}
	return;
}

std::vector<std::string> LoopKilled = {};

void doLoopKill() {
	for (int c = 0;c < LoopKilled.size();c++) {
		std::string Player = LoopKilled[c];
		std::vector<DWORD> Plr = GetPlayer(Player);
		if (Plr.size() >= 1) {
			DWORD Play = Plr[0];
			if (Play) {
				DWORD Char = GetPlrCharacter(Play);
				if (Char) {
					BreakJoints(Char);
				}
			}
			else {
				LoopKilled.erase(LoopKilled.begin() + c);
			}
		}
	}
}

void doLoopKillLoop() {
	while (true) {
		if (LoopKilled.size() == 0) std::this_thread::yield();
		if (breakLoopKill) { breakLoopKill = false; break; }
		doLoopKill();
		Sleep(750);
	}
	return;
}

std::vector<std::string> SilentGodded = {};

void doSilentGod() {
	for (int c = 0;c < SilentGodded.size();c++) {
		std::string Player = SilentGodded[c];
		std::vector<DWORD> Plr = GetPlayer(Player);
		if (Plr.size() >= 1) {
			DWORD Play = Plr[0];
			if (Play) {
				DWORD Char = GetPlrCharacter(Play);
				if (Char) {
					int Hum = FindFirstClass(Char, "Humanoid");
					if (Hum) {
						int MxHealth = GetMaxHealth(Hum);
						SetHealth(Hum, MxHealth);
					}
				}
			}
			else {
				SilentGodded.erase(LoopKilled.begin() + c);
			}
		}
	}
}

void doSilentGoddedLoop() {
	while (true) {
		if (SilentGodded.size() == 0) std::this_thread::yield();
		if (breakSilentGodLoop) { breakSilentGodLoop = false; break; }
		doSilentGod();
		Sleep(10);
	}
	return;
}

//std::string LoopCommandStr = "";
std::vector<std::string> LoopCommands = {};
int LoopCommandSleepTime = 750; // Default is 750

void doLoopCommand() {
	while (true) {
		if (LoopCommands.size() < 1) std::this_thread::yield();
		if (breakLoopCommandLoop) { breakLoopCommandLoop = false; break; }
		if(LoopCommands.size() >= 1) Rosin::CmdManager->ParseCommand(LoopCommands[0]);
		Sleep(LoopCommandSleepTime);
	}
	return;
}

void DeleteTools() {
	int Rep = FindFirstChild(DataModel, "ReplicatedStorage");
	if (Rep) {
		std::vector<DWORD> Children = GetChildren(Rep);
		for (int c = 0; c < Children.size(); c++) {
			const char* Name = GetName(Children.at(c))->c_str();
			if (strcmp(Name, "BuildToolsModel") == 0 || strcmp(Name, "OwnerToolsModel") == 0) {
				rSetParent(Children.at(c), SoundService);
			}
		}
	}
}

void SpamChat(std::string msg, int times) {
	for (int c = 0; c < times; c++) {
		SendChat(Players, msg);
		Sleep(100);
	}
}

void RecurseRbxLock(int Ins) {
	std::vector<DWORD> Children = GetChildren(Ins);
	for (int i = 0; i < Children.size(); i++) {
		DWORD Child = Children.at(i);
		if (strcmp(GetClass(Child), "Part") == 0) {
			SetRobloxLocked(Child, 1);
		}
		RecurseRbxLock(Child);
	}
	return;
}

void RecurseUnanchor(int Ins) {
	std::vector<DWORD> Children = GetChildren(Ins);
	for (int i = 0; i < Children.size(); i++) {
		DWORD Child = Children.at(i);
		if (strcmp(GetClass(Child), "Part") == 0) {
			SetAnchored(Child, 0);
		}
		RecurseUnanchor(Child);
	}
	return;
}

void Concat(std::vector<DWORD> V1, std::vector<DWORD> V2) {
	V1.insert(V1.end(), V2.begin(), V2.end());
}

std::vector<DWORD> Holder = {};
std::vector<DWORD> Holder2 = {};
std::vector<DWORD> Holder3 = {};

std::vector<DWORD> RecurseFindHat(int Ins) {
	std::vector<DWORD> Children = Roblox::GetChildren(Ins);
	for (size_t i = 0; i < Children.size(); i++) {
		DWORD Child = Children[i];
		if (strcmp(Roblox::GetClass(Child), "Hat") == 0) {
			Holder.push_back(Child);
		}
		else {
			RecurseFindHat(Child);
		}
	}
	//std::vector<DWORD> ret(Holder);
	//Holder.clear();
	return Holder;
}

std::vector<DWORD> RecurseFindGear(int Ins) {
	std::vector<DWORD> Children = Roblox::GetChildren(Ins);
	for (size_t i = 0; i < Children.size(); i++) {
		DWORD Child = Children[i];
		if (strcmp(Roblox::GetClass(Child), "Tool") == 0) {
			Holder2.push_back(Child);
		}
		else {
			RecurseFindGear(Child);
		}
	}
	//std::vector<DWORD> ret(Holder);
	//Holder.clear();
	return Holder2;
}

std::vector<DWORD> RecurseFindSky(int Ins) {
	std::vector<DWORD> Children = Roblox::GetChildren(Ins);
	for (size_t i = 0; i < Children.size(); i++) {
		DWORD Child = Children[i];
		if (strcmp(Roblox::GetClass(Child), "Sky") == 0) {
			Holder3.push_back(Child);
		}
		else {
			RecurseFindSky(Child);
		}
	}
	//std::vector<DWORD> ret(Holder);
	//Holder.clear();
	return Holder3;
}

void ReloadRosin() {
	Rosin::Form::GetBaseInst()->ClearOutput();

	breakLoopCommandLoop = true;
	breakLoopKill = true;
	breakRainbowLoop = true;
	breakSilentGodLoop = true;
	breakStateLoop = true;

	Rosin::Form::GetBaseInst()->RawOutputText("Finding addresses.. ", RGB(255, 0, 0));

	int test = Scanning::Scan(PAGE_READWRITE, "\x14\xAC\xFA\x00", "xxxx");
	if (test != 0) {
		Roblox::DataModel = Roblox::GetParent(test);
	}
	if (Roblox::DataModel == 0) {
		Rosin::Form::GetBaseInst()->OutputText("\nUh oh! It seems ROBLOX has updated!", RGB(255, 0, 0));
		Rosin::Form::GetBaseInst()->OutputText("Please wait until the next update.\n", RGB(255, 0, 0));
		std::string MOTD = DownloadURL("http://dark-hax.net/mo.td");
		Rosin::Form::GetBaseInst()->RawOutputText("Message of the day: ");
		Rosin::ColorFormat(MOTD + "\n");
		return;
	}
	
	Roblox::Workspace = Roblox::FindFirstClass(Roblox::DataModel, "Workspace");
	Roblox::Lighting = Roblox::FindFirstClass(Roblox::DataModel, "Lighting");
	Roblox::Players = Roblox::FindFirstClass(Roblox::DataModel, "Players");
	Roblox::Debris = Roblox::FindFirstClass(Roblox::DataModel, "Debris");
	Roblox::LogService = Roblox::FindFirstClass(Roblox::DataModel, "LogService");
	Roblox::SoundService = Roblox::FindFirstClass(Roblox::DataModel, "SoundService");
	Roblox::LocalPlayer = Roblox::GetLocalPlayer(Roblox::Players);
	Roblox::PlayerName = Roblox::GetName(Roblox::LocalPlayer);

	Rosin::Form::GetBaseInst()->OutputText("done!");

	std::thread stateThread(doStateLoop);
	stateThread.detach();

	std::thread rainbowThread(doRainbowLoop);
	rainbowThread.detach();

	std::thread loopKillThread(doLoopKillLoop);
	loopKillThread.detach();

	std::thread loopCmdThread(doLoopCommand);
	loopCmdThread.detach();

	std::thread loopGoddedThread(doSilentGoddedLoop);
	loopGoddedThread.detach();

	Rosin::Form::GetBaseInst()->OutputText("Rosin v2.1 has loaded! By: Slappy826\n", RGB(255, 69, 0));

	int Filtering = Roblox::GetFilteringEnabled(Roblox::Workspace);
	if (Filtering) {
		Rosin::Form::GetBaseInst()->OutputText("Filtering is enabled! Most commands will not work/replicate.", RGB(255, 0, 0));
	}
	else {
		Rosin::Form::GetBaseInst()->OutputText("Filtering is not enabled.");
	}

	std::string WelcomeText = "Hello, ";
	WelcomeText += Roblox::PlayerName->c_str();
	WelcomeText += "! Welcome to Rosin!\n";

	Rosin::Form::GetBaseInst()->OutputText(WelcomeText);

	std::string MOTD = DownloadURL("http://dark-hax.net/mo.td");
	Rosin::Form::GetBaseInst()->RawOutputText("Message of the day: ");
	Rosin::ColorFormat(MOTD + "\n");
}

void AddCommands() {
	Rosin::CmdManager->NewCommand("noclip", { "nclip","noclip" }, "Gives the user noclip", (CommandFunction)[](std::vector<std::string> Args)->void {
		State = 11;
		Changing = true;
	});
	Rosin::CmdManager->NewCommand("swim", { "swim" }, "Gives the user swim", (CommandFunction)[](std::vector<std::string> Args)->void {
		State = 4;
		Changing = true;
	});
	Rosin::CmdManager->NewCommand("superjump", { "superjump" }, "Gives the user superjump", (CommandFunction)[](std::vector<std::string> Args)->void {
		State = 13;
		Changing = true;
	});
	Rosin::CmdManager->NewCommand("airwalk", { "airwalk" }, "Gives the user airwalk", (CommandFunction)[](std::vector<std::string> Args)->void {
		State = 10;
		Changing = true;
	});
	Rosin::CmdManager->NewCommand("jesusfly", { "jesusfly" }, "Gives the user jesusfly", (CommandFunction)[](std::vector<std::string> Args)->void {
		State = 12;
		Changing = true;
	});
	Rosin::CmdManager->NewCommand("ragdoll", { "ragdoll" }, "Gives the user ragdoll", (CommandFunction)[](std::vector<std::string> Args)->void {
		State = 1;
		Changing = true;
	});
	Rosin::CmdManager->NewCommand("disable", { "disable" }, "disable", (CommandFunction)[](std::vector<std::string> Args)->void {
		Changing = false;
	});
	Rosin::CmdManager->NewCommand("walkspeed", { "ws" }, "ws", (CommandFunction)[](std::vector<std::string> Args)->void {
		if (Args.size() == 2) {
			float Speed = std::stof(Args[1].c_str());
			std::vector<DWORD> Characters = Rosin::Roblox::GetCharacter(Args[0]);
			for (size_t i = 0; i < Characters.size(); i++) {
				int Character = Characters[i];
				if (Character != 0) {
					int humanoid = Rosin::Roblox::FindFirstClass(Character, "Humanoid");
					if (humanoid != 0) {
						Roblox::SetWS(humanoid, Speed);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("jump", { "jump" }, "jump", (CommandFunction)[](std::vector<std::string> Args)->void {
		if (Args.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(Args.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int character = chars.at(i);
				if (character != 0) {
					int humanoid = Rosin::Roblox::FindFirstChild_PartOf(character, "Humanoid");
					if (humanoid != 0) {
						Roblox::Jump(humanoid, 1);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("sit", { "sit" }, "sit", (CommandFunction)[](std::vector<std::string> Args)->void {
		if (Args.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(Args.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int character = chars.at(i);
				if (character != 0) {
					int humanoid = Rosin::Roblox::FindFirstChild_PartOf(character, "Humanoid");
					if (humanoid != 0) {
						Roblox::Sit(humanoid, 1);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("plat", { "plat" }, "plat", (CommandFunction)[](std::vector<std::string> Args)->void {
		if (Args.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(Args.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int character = chars.at(i);
				if (character != 0) {
					int humanoid = Rosin::Roblox::FindFirstChild_PartOf(character, "Humanoid");
					if (humanoid != 0) {
						Roblox::PlatformStand(humanoid, 1);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("blockhead", { "blockhead" }, "blockhead", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int character = chars.at(i);
				if (character != 0) {
					int head = Rosin::Roblox::FindFirstChild_PartOf(character, "Head");
					if (head != 0) {
						int mesh = Rosin::Roblox::FindFirstChild_PartOf(head, "Mesh");
						if (mesh) {
							Roblox::rSetParent(mesh, Rosin::Roblox::Lighting);
						}
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("invis", { "invis" }, "invis", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int character = chars.at(i);
				if (character != 0) {
					std::vector<DWORD> Children = Rosin::Roblox::GetChildren(character);
					for (int i = 0; i < Children.size(); i++) {
						DWORD Child = Children[i];
						if (strcmp(Rosin::Roblox::GetClass(Child), "Part") == 0) {
							SetPartTrans(Child, 1);
						}
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("vis", { "vis" }, "vis", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int character = chars.at(i);
				if (character != 0) {
					std::vector<DWORD> Children = Rosin::Roblox::GetChildren(character);
					for (int i = 0; i < Children.size(); i++) {
						DWORD Child = Children[i];
						if (strcmp(Rosin::Roblox::GetClass(Child), "Part") == 0 && strcmp(Rosin::Roblox::GetName(Child)->c_str(), "HumanoidRootPart") != 0) {
							SetPartTrans(Child, 0);
						}
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("jp", { "jp" }, "jp", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 2) {
			float powa = ::atof(In.at(1).c_str());
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int Character = chars.at(i);
				if (Character != 0) {
					int Humanoid = Rosin::Roblox::FindFirstClass(Character, "Humanoid");
					if (Humanoid != 0) {
						SetJumpPower(Humanoid, powa);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("confuse", { "confuse" }, "confuse", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int Character = chars.at(i);
				if (Character != 0) {
					int Humanoid = Rosin::Roblox::FindFirstClass(Character, "Humanoid");
					if (Humanoid != 0) {
						AutoR(Humanoid, HUGE_VAL);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("btools", { "btools" }, "btools", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> plrs = Rosin::Roblox::GetPlayer(In.at(0));
			for (int i = 0; i < plrs.size(); i++) {
				int player = plrs.at(i);
				if (player) {
					DeleteTools();
					//PSRRR(player, 1);
					PSRRR(player, 255);
					SetName(Rosin::Roblox::Players, &std::string("Players"));
					int Joints = Rosin::Roblox::GetService("JointsService");
					if (Joints) {
						SetName(Joints, &std::string("JointsService"));
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("god", { "god" }, "god", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int Character = chars.at(i);
				if (Character != 0) {
					int Humanoid = Rosin::Roblox::FindFirstClass(Character, "Humanoid");
					if (Humanoid != 0) {
						SetMaxHealth(Humanoid, HUGE_VAL);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("nochar", { "nochar" }, "nochar", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> plrs = Rosin::Roblox::GetPlayer(In.at(0));
			for (int i = 0; i < plrs.size(); i++) {
				int player = plrs.at(i);
				if (player != 0)
				{
					int Character = Rosin::Roblox::GetPlrCharacter(player);
					CanLoadCharacterAppearance((int*)player, false);
					if (Character) {
						BreakJoints(Character);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("rechar", { "rechar" }, "rechar", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> plrs = Rosin::Roblox::GetPlayer(In.at(0));
			for (int i = 0; i < plrs.size(); i++) {
				int player = plrs.at(i);
				if (player != 0)
				{
					int Character = Rosin::Roblox::GetPlrCharacter(player);
					CanLoadCharacterAppearance((int*)player, true);
					if (Character) {
						BreakJoints(Character);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("freeze", { "freeze" }, "freeze", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int Character = chars.at(i);
				if (Character != 0) {
					int Humanoid = Rosin::Roblox::FindFirstChild_PartOf(Character, "Torso");
					if (Humanoid != 0) {
						SetAnchored(Humanoid, 1);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("unfreeze", { "unfreeze" }, "unfreeze", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int Character = chars.at(i);
				if (Character != 0) {
					int Humanoid = Rosin::Roblox::FindFirstChild_PartOf(Character, "Torso");
					if (Humanoid != 0) {
						SetAnchored(Humanoid, 0);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("ragdoll", { "ragdoll" }, "ragdoll", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1)
		{
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int Character = chars.at(i);
				if (Character != 0) {
					int Humanoid = Rosin::Roblox::FindFirstClass(Character, "Humanoid");
					if (Humanoid != 0) {
						rSetParent(Humanoid, Rosin::Roblox::Lighting);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("removehats", { "removehats" }, "removehats", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int Character = chars.at(i);
				if (Character != 0) {
					std::vector<DWORD> child = Rosin::Roblox::GetChildren(Character);
					for (int i = 0; i < child.size(); i++) {
						DWORD c = child[i];
						if (strcmp(Rosin::Roblox::GetClass(c), "Hat") == 0) {
							rSetParent(c, Rosin::Roblox::Lighting);
						}
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("seth", { "seth" }, "seth", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 2) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int Character = chars.at(i);
				if (Character) {
					int health = ::atof(In.at(1).c_str());
					int hum = Rosin::Roblox::FindFirstClass(Character, "Humanoid");
					if (hum) {
						SetMaxHealth(hum, health);
						SetHealth(hum, health);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("removeg", { "removeg" }, "removeg", (CommandFunction)[](std::vector<std::string> In)->void {
		std::vector<DWORD> plrs = Rosin::Roblox::GetPlayer(In.at(0));
		for (int i = 0; i < plrs.size(); i++) {
			int plr = plrs.at(i);
			if (plr) {
				int pack = Rosin::Roblox::FindFirstChild_PartOf(plr, "Backpack");
				if (pack) {
					std::vector<DWORD> children = Rosin::Roblox::GetChildren(pack);
					for (int i = 0; i < children.size(); i++) {
						DWORD child = children[i];
						rSetParent(child, Rosin::Roblox::Lighting);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("ungod", { "ungod" }, "ungod", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int Character = chars.at(i);
				if (Character != 0) {
					int Humanoid = Rosin::Roblox::FindFirstClass(Character, "Humanoid");
					if (Humanoid != 0) {
						SetMaxHealth(Humanoid, 100);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("kill", { "kill" }, "kill", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int Character = chars.at(i);
				if (Character != 0) {
					BreakJoints(Character);
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("punish", { "punish" }, "punish", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int Character = chars.at(i);
				if (Character != 0) {
					rSetParent(Character, Rosin::Roblox::Lighting);
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("unragdoll", { "unragdoll" }, "unragdoll", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1)
		{
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int Character = chars.at(i);
				int Humanoid = Rosin::Roblox::FindFirstClass(Rosin::Roblox::Lighting, "Humanoid");
				int Hum = Rosin::Roblox::FindFirstClass(Character, "Humanoid");
				if (Humanoid != 0 & Hum == 0) {
					if (Character != 0) {
						rSetParent(Humanoid, Character);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("naked", { "naked" }, "naked", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int Character = chars.at(i);
				if (Character != 0) {
					std::vector<DWORD> child = Rosin::Roblox::GetChildren(Character);
					for (int i = 0; i < child.size(); i++) {
						DWORD c = child[i];
						if (strcmp(Rosin::Roblox::GetClass(c), "Shirt") == 0 || strcmp(Rosin::Roblox::GetClass(c), "Pants") == 0 || strcmp(Rosin::Roblox::GetClass(c), "ShirtGraphic") == 0) {
							rSetParent(c, Rosin::Roblox::Lighting);
						}
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("kick", { "kick" }, "kick", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> plrs = Rosin::Roblox::GetPlayer(In.at(0));
			for (int i = 0; i < plrs.size(); i++) {
				int plr = plrs.at(i);
				if (plr != 0) {
					PSRRR(plr, 255);
					Sleep(20);
					PSRRR(plr, 0);
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("gocam", { "gocam" }, "gocam", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			int character = Rosin::Roblox::FindFirstChild_PartOf(Workspace, In.at(0));
			if (character != 0) {
				int rewt = Rosin::Roblox::FindFirstChild_PartOf(character, "HumanoidRootPart");
				int fcam = Rosin::Roblox::FindFirstChild_PartOf(rewt, "RootJoint");
				int ranimation = Rosin::Roblox::FindFirstChild_PartOf(character, "Animate");
				if (ranimation != 0) {
					rSetParent(ranimation, Rosin::Roblox::Lighting);
				}
				int charSound = Rosin::Roblox::FindFirstChild_PartOf(character, "Sound");
				if (charSound != 0) {
					rSetParent(charSound, Rosin::Roblox::Lighting);
				}
				if (fcam != 0) {
					rSetParent(fcam, Rosin::Roblox::Lighting);
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("fixcam", { "fixcam" }, "fixcam", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			int camera = Rosin::Roblox::FindFirstChild_PartOf(Rosin::Roblox::Lighting, "RootJoint");
			int sounds = Rosin::Roblox::FindFirstChild_PartOf(Rosin::Roblox::Lighting, "Sound");
			int animations = Rosin::Roblox::FindFirstChild_PartOf(Rosin::Roblox::Lighting, "Animate");
			int character = Rosin::Roblox::FindFirstChild_PartOf(Workspace, In.at(0));
			if (character != 0) {
				if (camera != 0) {
					int roots = Rosin::Roblox::FindFirstChild_PartOf(character, "HumanoidRootPart");
					if (roots != 0) {
						rSetParent(camera, roots);
					}
				}
				if (sounds != 0) {
					rSetParent(sounds, character);
				}
				if (animations != 0) {
					rSetParent(animations, character);
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("unpunish", { "unpunish" }, "unpunish", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> plrs = Rosin::Roblox::GetPlayer(In.at(0));
			for (int i = 0; i < plrs.size(); i++) {
				int plr = plrs.at(i);
				int thingy = Rosin::Roblox::FindFirstChild_PartOf(Rosin::Roblox::Lighting, std::string(Rosin::Roblox::GetName(plr)->c_str()));
				if (thingy) {
					rSetParent(thingy, Workspace);
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("hidename", { "hidename" }, "hidename", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int Character = chars.at(i);
				if (Character != 0) {
					int Humanoid = Rosin::Roblox::FindFirstClass(Character, "Humanoid");
					if (Humanoid != 0) {
						Rosin::Roblox::SetDisplayDistanceType(Humanoid, 2);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("showname", { "showname" }, "showname", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int Character = chars.at(i);
				if (Character != 0) {
					int Humanoid = Rosin::Roblox::FindFirstClass(Character, "Humanoid");
					if (Humanoid != 0) {
						Rosin::Roblox::SetDisplayDistanceType(Humanoid, 0);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("fog", { "fog" }, "fog", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			float No = ::atof(In.at(0).c_str());
			Rosin::Roblox::SetFogStart(Rosin::Roblox::Lighting, No);
			Rosin::Roblox::SetFogEnd(Rosin::Roblox::Lighting, No + 15);
		}
	});
	Rosin::CmdManager->NewCommand("time", { "time" }, "time", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			double a = ::atof(In.at(0).c_str());
			SetMinutes(Rosin::Roblox::Lighting, a);
		}
	});
	Rosin::CmdManager->NewCommand("rainbow", { "rainbow" }, "rainbow", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			if (In.at(0) == "on") {
				rainbow = true;
			}
			else if (In.at(0) == "off") {
				rainbow = false;
			}
		}
	});
	Rosin::CmdManager->NewCommand("fogcolor", { "fogcolor" }, "fogcolor", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 3) {
			float No1 = ::atof(In.at(0).c_str());
			float No2 = ::atof(In.at(1).c_str());
			float No3 = ::atof(In.at(2).c_str());
			Rosin::Roblox::SetFogColor(Rosin::Roblox::Lighting, No1, No2, No3);
		}
	});
	Rosin::CmdManager->NewCommand("waterc", { "waterc" }, "waterc", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 3) {
			int Terrain = Rosin::Roblox::FindFirstClass(Workspace, "Terrain");
			float No1 = ::atof(In.at(0).c_str());
			float No2 = ::atof(In.at(1).c_str());
			float No3 = ::atof(In.at(2).c_str());
			Rosin::Roblox::SetWaterColor(Terrain, No1, No2, No3);
		}
	});
	Rosin::CmdManager->NewCommand("waterheight", { "waterheight" }, "waterheight", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			float n = ::atof(In.at(0).c_str());
			int Terrain = Rosin::Roblox::FindFirstClass(Workspace, "Terrain");
			if (Terrain != 0) {
				Rosin::Roblox::SetWaveHeight(Terrain, n);
			}
		}
	});
	Rosin::CmdManager->NewCommand("tp", { "tp" }, "tp", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 2) {
			std::vector<DWORD> toTeleport = Rosin::Roblox::GetCharacter(In.at(0));
			int plr2 = Rosin::Roblox::GetCharacter(In.at(1)).at(0);
			if (plr2) {
				int tor2 = Rosin::Roblox::FindFirstChild_PartOf(plr2, "HumanoidRootPart");
				for (int c = 0; c < toTeleport.size(); c++) {
					int tor1 = Rosin::Roblox::FindFirstChild_PartOf(toTeleport.at(c), "HumanoidRootPart");
					if (tor1 & tor2) {
						CFrame* cf = GetCFrame((void*)tor2);
						Rosin::Roblox::setPartCFrame(tor1, cf->position.x, cf->position.y + 5, cf->position.z);
					}
					else {
						break;
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("music", { "music" }, "music", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chld = Rosin::Roblox::GetChildren(Workspace);
			for (int i = 0; i < chld.size(); i++) {
				if (strcmp(Rosin::Roblox::GetClass(chld.at(i)), "Sound") == 0) {
					stopMusic(chld.at(i));
				}
			}

			if (Rosin::Roblox::FindFirstChild(Workspace, "RosinSound")) {
				int Sound = Rosin::Roblox::FindFirstChild(Workspace, "RosinSound");
				if (Sound) {
					std::string id = "rbxassetid://";
					id += std::to_string((int)std::atof(In.at(0).c_str()));
					SoundId(Sound, id);
					SetName(Sound, &std::string("RosinSound"));
					rSetParent(Sound, Workspace);
					Sleep(50);
					playSound(Sound);
				}
			}
			else {
				int Sound = CreateInstance("Sound");
				if (Sound) {
					std::string id = "rbxassetid://";
					id += std::to_string((int)std::atof(In.at(0).c_str()));
					SoundId(Sound, id);
					SetName(Sound, &std::string("RosinSound"));
					rSetParent(Sound, Workspace);
					Sleep(50);
					playSound(Sound);
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("cmode", { "cmode" }, "cmode", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 2) {
			std::vector<DWORD> Players = Rosin::Roblox::GetPlayer(In.at(0));
			for (int c = 0; c < Players.size(); c++) {
				int plr = Players.at(c);
				int mode = ::atof(In.at(1).c_str());
				if (plr) {
					CameraMode(plr, mode);
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("stealstat", { "stealstat" }, "stealstat", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 2) {
			std::vector<DWORD> Plrs = Rosin::Roblox::GetPlayer(In.at(0));
			for (int c = 0; c < Plrs.size(); c++) {
				int plr = Plrs.at(c);
				if (plr) {
					int stats = Rosin::Roblox::FindFirstChild_PartOf(plr, "leaderstats");
					if (stats) {
						int stat = Rosin::Roblox::FindFirstChild_PartOf(stats, In.at(1));
						if (stat) {
							int lp = Rosin::Roblox::GetLocalPlayer(Rosin::Roblox::Players);
							if (lp) {
								int mystats = Rosin::Roblox::FindFirstChild_PartOf(lp, "leaderstats");
								if (mystats) {
									int myst = Rosin::Roblox::FindFirstChild_PartOf(mystats, In.at(1));
									if (myst) {
										rSetParent(stat, mystats);
										rSetParent(myst, stats);
									}
									else {
										rSetParent(stat, mystats);
									}
								}
							}
						}
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("changestat", { "changestat" }, "changestat", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 3) {
			std::vector<DWORD> Plrs = Rosin::Roblox::GetPlayer(In.at(0));
			for (int c = 0; c < Plrs.size(); c++) {
				int plr = Plrs.at(c);
				int stats = Rosin::Roblox::FindFirstChild_PartOf(plr, "leaderstats");
				if (stats) {
					int kos = Rosin::Roblox::FindFirstChild_PartOf(stats, In.at(1));
					if (kos) {
						int asd = ::atof(In.at(2).c_str());
						Rosin::Roblox::SetIntValue(kos, asd);
						Sleep(5);
						rSetParent(kos, Rosin::Roblox::Lighting);
						Sleep(5);
						rSetParent(kos, stats);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("spamchat", { "spamchat" }, "spamchat", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() >= 2) {
			int me = GetLocalPlayer(Players);
			int times = ::atof(In.at(0).c_str());
			std::string toChat;
			for (int i = 1; i < In.size(); i++) {
				toChat += In.at(i);
				toChat += " ";
			}
			std::thread t(SpamChat, toChat, times);
			t.detach();
		}
	});
	Rosin::CmdManager->NewCommand("clearterrain", { "clearterrain" }, "clearterrain", (CommandFunction)[](std::vector<std::string> In)->void {
		int terrain = Rosin::Roblox::FindFirstClass(Workspace, "Terrain");
		if (terrain) {
			ClearTerrain(terrain);
		}
	});
	Rosin::CmdManager->NewCommand("bighead", { "bighead" }, "bighead", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> Plrs = Rosin::Roblox::GetCharacter(In.at(0));
			for (int c = 0; c < Plrs.size(); c++) {
				int ch = Plrs.at(c);
				if (ch) {
					int head = Rosin::Roblox::FindFirstChild_PartOf(ch, "Head");
					if (head) {
						int mesh = Rosin::Roblox::FindFirstChild_PartOf(head, "Mesh");
						if (mesh) {
							Vector3 newSize{
								10,
								10,
								10
							};
							SetMeshScale(mesh, &newSize);
						}
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("reghead", { "reghead" }, "reghead", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> Plrs = Rosin::Roblox::GetCharacter(In.at(0));
			for (int c = 0; c < Plrs.size(); c++) {
				int ch = Plrs.at(c);
				if (ch) {
					int head = Rosin::Roblox::FindFirstChild_PartOf(ch, "Head");
					if (head) {
						int mesh = Rosin::Roblox::FindFirstChild_PartOf(head, "Mesh");
						if (mesh) {
							Vector3 newSize{
								1,
								1,
								1
							};
							SetMeshScale(mesh, &newSize);
						}
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("unlockchar", { "unlockchar" }, "unlockchar", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int character = chars.at(i);
				if (character != 0) {
					std::vector<DWORD> Children = Rosin::Roblox::GetChildren(character);
					for (int i = 0; i < Children.size(); i++) {
						DWORD Child = Children[i];
						if (strcmp(Rosin::Roblox::GetClass(Child), "Part") == 0) {
							SetPartLocked(Child, 0);
						}
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("lockchar", { "lockchar" }, "lockchar", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int character = chars.at(i);
				if (character != 0) {
					std::vector<DWORD> Children = Rosin::Roblox::GetChildren(character);
					for (int i = 0; i < Children.size(); i++) {
						DWORD Child = Children[i];
						if (strcmp(Rosin::Roblox::GetClass(Child), "Part") == 0) {
							SetPartLocked(Child, 1);
						}
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("removechar", { "removechar" }, "removechar", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> Plrs = Rosin::Roblox::GetPlayer(In.at(0));
			for (int c = 0; c < Plrs.size(); c++) {
				int plr = Plrs.at(c);
				if (plr) {
					SetPlrChar(plr, 0);
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("removesounds", { "removesounds" }, "removesounds", (CommandFunction)[](std::vector<std::string> In)->void {
		std::vector<DWORD> chld = Rosin::Roblox::GetChildren(Workspace);
		for (int i = 0; i < chld.size(); i++) {
			if (strcmp(Rosin::Roblox::GetClass(chld.at(i)), "Sound") == 0) {
				stopMusic(chld.at(i));
			}
		}
	});
	Rosin::CmdManager->NewCommand("setname", { "setname" }, "setname", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() >= 2) {
			std::vector<DWORD> Plrs = Rosin::Roblox::GetCharacter(In.at(0));
			for (int c = 0; c < Plrs.size(); c++) {
				int plr = Plrs.at(c);
				if (plr) {
					std::string builtString = "";
					for (int i = 1; i < In.size(); i++) {
						builtString += In.at(i);
						builtString += " ";
					}
					builtString = builtString.substr(0, builtString.length() - 1);
					SetName(plr, &builtString);
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("char", { "char" }, "char", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 2) {
			std::vector<DWORD> Plrs = Rosin::Roblox::GetPlayer(In.at(0));
			for (int c = 0; c < Plrs.size(); c++) {
				int plr = Plrs.at(c);
				if (plr) {
					std::string baseUrl = "http://www.roblox.com/Asset/CharacterFetch.ashx?userId=";
					if (In.at(1) == "off") {
						baseUrl += std::to_string(Rosin::Roblox::GetPlayerUserId(plr));
					}
					else {
						baseUrl += In.at(1);
					}
					SetCharApp(plr, &baseUrl);
					int chr = Rosin::Roblox::GetPlrCharacter(plr);
					if (chr) {
						BreakJoints(chr);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("clr", { "clr" }, "clr", (CommandFunction)[](std::vector<std::string> In)->void {
		Rosin::Form::GetBaseInst()->ClearOutput();
	});
	Rosin::CmdManager->NewCommand("rbxlock", { "rbxlock" }, "rbxlock", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> Plrs = Rosin::Roblox::GetPlayer(In.at(0));
			for (int c = 0; c < Plrs.size(); c++) {
				int p = Plrs.at(c);
				if (p) {
					Rosin::Roblox::SetRobloxLocked(p, 1); // hahahahah
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("unrbxlock", { "unrbxlock" }, "unrbxlock", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> Plrs = Rosin::Roblox::GetPlayer(In.at(0));
			for (int c = 0; c < Plrs.size(); c++) {
				int p = Plrs.at(c);
				if (p) {
					Rosin::Roblox::SetRobloxLocked(p, 0); // hahahahah
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("checkrbxlock", { "checkrbxlock" }, "checkrbxlock", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> Plrs = Rosin::Roblox::GetPlayer(In.at(0));
			for (int c = 0; c < Plrs.size(); c++) {
				int p = Plrs.at(c);
				if (p) {
					Rosin::Form::GetBaseInst()->OutputText(std::to_string(Rosin::Roblox::GetRobloxLocked(p)));
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("breakgame", { "breakgame" }, "breakgame", (CommandFunction)[](std::vector<std::string> In)->void {
		Rosin::Roblox::SetRobloxLocked(Workspace, 1);
		std::thread Wrksp(RecurseRbxLock, Workspace);
		Wrksp.detach();
		Rosin::Roblox::SetRobloxLocked(Rosin::Roblox::Lighting, 1);
		std::thread Lght(RecurseRbxLock, Rosin::Roblox::Lighting);
		Lght.detach();
		Rosin::Roblox::SetRobloxLocked(Rosin::Roblox::Players, 1);
		std::thread Plrs(RecurseRbxLock, Rosin::Roblox::Players);
		Plrs.detach();
	});
	Rosin::CmdManager->NewCommand("exe", { "exe" }, "exe", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() >= 1) {
			std::string builtString = "";
			for (int i = 0; i < In.size(); i++) {
				builtString += In.at(i);
				builtString += " ";
			}
			ExecuteS(Rosin::Roblox::LogService, builtString);
		}
	});
	Rosin::CmdManager->NewCommand("setbc", { "setbc" }, "setbc", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 2) {
			std::vector<DWORD> Plrs = Rosin::Roblox::GetPlayer(In.at(0));
			for (int c = 0; c < Plrs.size(); c++) {
				int p = Plrs.at(c);
				if (p) {
					int bc = std::stoi(In.at(1));
					SetMembershipType(p, bc);
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("setage", { "setage" }, "setage", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 2) {
			std::vector<DWORD> Plrs = Rosin::Roblox::GetPlayer(In.at(0));
			for (int c = 0; c < Plrs.size(); c++) {
				int plr = Plrs.at(c);
				if (plr) {
					int newAge = std::stoi(In.at(1));
					SetAccountAge(plr, newAge);
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("getage", { "getage" }, "getage", (CommandFunction)[](std::vector<std::string> In)->void {
		std::vector<DWORD> Plrs = Rosin::Roblox::GetPlayer(In.at(0));
		for (int c = 0; c < Plrs.size(); c++) {
			int plr = Plrs.at(c);
			if (plr) {
				int age = Rosin::Roblox::GetAccountAge(plr);
				Rosin::Form::GetBaseInst()->OutputText(*Rosin::Roblox::GetName(plr) + "'s age is " + std::to_string(age));
			}
		}
	});
	Rosin::CmdManager->NewCommand("shiny", { "shiny" }, "shiny", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int character = chars.at(i);
				if (character != 0) {
					std::vector<DWORD> Children = Rosin::Roblox::GetChildren(character);
					for (int i = 0; i < Children.size(); i++) {
						DWORD Child = Children[i];
						if (strcmp(Rosin::Roblox::GetClass(Child), "Part") == 0) {
							SetRelfectance(Child, 1);
						}
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("unshiny", { "unshiny" }, "unshiny", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int character = chars.at(i);
				if (character != 0) {
					std::vector<DWORD> Children = Rosin::Roblox::GetChildren(character);
					for (int i = 0; i < Children.size(); i++) {
						DWORD Child = Children[i];
						if (strcmp(Rosin::Roblox::GetClass(Child), "Part") == 0) {
							SetRelfectance(Child, 0);
						}
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("neon", { "neon" }, "neon", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int character = chars.at(i);
				if (character != 0) {
					std::vector<DWORD> Children = Rosin::Roblox::GetChildren(character);
					for (int i = 0; i < Children.size(); i++) {
						DWORD Child = Children[i];
						if (strcmp(Rosin::Roblox::GetClass(Child), "Part") == 0) {
							SetMaterial(Child, 288);
						}
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("unneon", { "unneon" }, "unneon", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 0) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int character = chars.at(i);
				if (character != 0) {
					std::vector<DWORD> Children = Rosin::Roblox::GetChildren(character);
					for (int i = 0; i < Children.size(); i++) {
						DWORD Child = Children[i];
						if (strcmp(Rosin::Roblox::GetClass(Child), "Part") == 0) {
							SetMaterial(Child, 256);
						}
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("exit", { "exit" }, "exit", (CommandFunction)[](std::vector<std::string> In)->void {
		exit(0);
	});
	Rosin::CmdManager->NewCommand("rlegs", { "rlegs" }, "rlegs", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int character = chars.at(i);
				if (character != 0) {
					std::vector<DWORD> Children = Rosin::Roblox::GetChildren(character);
					for (int i = 0; i < Children.size(); i++) {
						DWORD Child = Children[i];
						if (strcmp(Rosin::Roblox::GetClass(Child), "Part") == 0 & Rosin::Roblox::GetName(Child)->find("Leg") != std::string::npos) {
							rSetParent(Child, Rosin::Roblox::SoundService);
						}
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("rarms", { "rarms" }, "rarms", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int character = chars.at(i);
				if (character != 0) {
					std::vector<DWORD> Children = Rosin::Roblox::GetChildren(character);
					for (int i = 0; i < Children.size(); i++) {
						DWORD Child = Children[i];
						if (strcmp(Rosin::Roblox::GetClass(Child), "Part") == 0 & Rosin::Roblox::GetName(Child)->find("Arm") != std::string::npos) {
							rSetParent(Child, Rosin::Roblox::SoundService);
						}
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("rlimbs", { "rlimbs" }, "rlimbs", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			ProcessCommand("rlegs " + In.at(0));
			ProcessCommand("rarms " + In.at(0));
		}
	});
	Rosin::CmdManager->NewCommand("stealgear", { "stealgear" }, "stealgear", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> Plrs = Rosin::Roblox::GetPlayer(In.at(0));
			for (int c = 0; c < Plrs.size(); c++) {
				int p = Plrs.at(c);
				int bp = Rosin::Roblox::FindFirstClass(p, "Backpack");
				if (bp) {
					std::vector<DWORD> me = Rosin::Roblox::GetPlayer("me");
					if (me.size() == 1) {
						int ins = Rosin::Roblox::FindFirstClass(me.at(0), "Backpack");
						if (ins) {
							std::vector<DWORD> gears = Rosin::Roblox::GetChildren(bp);
							for (int c = 0; c < gears.size(); c++) {
								rSetParent(gears.at(c), ins);
							}
						}
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("stealhats", { "stealhats" }, "stealhats", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			std::vector<DWORD> MyChar = Rosin::Roblox::GetCharacter("me");
			for (int i = 0; i < chars.size(); i++) {
				int character = chars.at(i);
				if (character != 0 & MyChar.size() == 1) {
					std::vector<DWORD> Children = Rosin::Roblox::GetChildren(character);
					for (int i = 0; i < Children.size(); i++) {
						DWORD Child = Children[i];
						if (strcmp(Rosin::Roblox::GetClass(Child), "Hat") == 0) {
							rSetParent(Child, MyChar.at(0));
						}
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("showstats", { "showstats" }, "showstats", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> Plrs = Rosin::Roblox::GetPlayer(In.at(0));
			for (int c = 0; c < Plrs.size(); c++) {
				int plr = Plrs.at(c);
				int stats = Rosin::Roblox::FindFirstChild_PartOf(plr, "leaderstats");
				if (stats) {
					std::vector<DWORD> statList = GetChildren(stats);
					for (int c = 0; c < statList.size(); c++) {
						Rosin::Form::GetBaseInst()->OutputText(*GetName(statList[c]));
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("Skybox", { "sbox","skybox" }, "Changes the current skybox", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::string id = "rbxassetid://" + In.at(0);
			int sky = FindFirstClass(Lighting, "Sky");
			if (sky) {
				SkyboxBk(sky, &id);
				SkyboxFt(sky, &id);
				SkyboxUp(sky, &id);
				SkyboxDn(sky, &id);
				SkyboxLf(sky, &id);
				SkyboxRt(sky, &id);
			}
			else {
				int sky = CreateInstance("Sky");
				SkyboxBk(sky, &id);
				SkyboxFt(sky, &id);
				SkyboxUp(sky, &id);
				SkyboxDn(sky, &id);
				SkyboxLf(sky, &id);
				SkyboxRt(sky, &id);
				rSetParent(sky, Lighting);
			}
		}
	});
	Rosin::CmdManager->NewCommand("LoopKill", { "loopkill" }, "loopkill", (CommandFunction)[](std::vector<std::string> In)->void{
		if (In.size() == 1) {
			std::vector<DWORD> Plrs = Rosin::Roblox::GetPlayer(In.at(0));
			for (int c = 0; c < Plrs.size(); c++) {
				int plr = Plrs.at(c);
				LoopKilled.push_back(*GetName(plr));
			}
		}
	});
	Rosin::CmdManager->NewCommand("UnLoopKill", { "unloopkill" }, "unloopkill", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> Plrs = Rosin::Roblox::GetPlayer(In.at(0));
			for (int c = 0; c < Plrs.size(); c++) {
				int plr = Plrs.at(c);
				for (int i = 0;i < LoopKilled.size();i++) {
					if (LoopKilled[i] == *GetName(plr)) {
						LoopKilled.erase(LoopKilled.begin() + i);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("LoopKill List", { "lklist" }, "loopkill list", (CommandFunction)[](std::vector<std::string> In)->void {
		for (int i = 0;i < LoopKilled.size();i++) {
			Rosin::Form::GetBaseInst()->OutputText(LoopKilled[i]);
		}
	});
	Rosin::CmdManager->NewCommand("Skydive", { "skydive" }, "Skydive", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> toTeleport = Rosin::Roblox::GetCharacter(In.at(0));
			for (int c = 0; c < toTeleport.size(); c++) {
				int tor1 = Rosin::Roblox::FindFirstChild_PartOf(toTeleport.at(c), "HumanoidRootPart");
				if (tor1) {
					CFrame* cf = GetCFrame((void*)tor1);
					Rosin::Roblox::setPartCFrame(tor1, cf->position.x, cf->position.y + 10000, cf->position.z);
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("metal", { "metal" }, "metal", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int character = chars.at(i);
				if (character != 0) {
					std::vector<DWORD> Children = Rosin::Roblox::GetChildren(character);
					for (int i = 0; i < Children.size(); i++) {
						DWORD Child = Children[i];
						if (strcmp(Rosin::Roblox::GetClass(Child), "Part") == 0) {
							SetMaterial(Child, 1088);
						}
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("unmetal", { "unmetal" }, "unmetal", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> chars = Rosin::Roblox::GetCharacter(In.at(0));
			for (int i = 0; i < chars.size(); i++) {
				int character = chars.at(i);
				if (character != 0) {
					std::vector<DWORD> Children = Rosin::Roblox::GetChildren(character);
					for (int i = 0; i < Children.size(); i++) {
						DWORD Child = Children[i];
						if (strcmp(Rosin::Roblox::GetClass(Child), "Part") == 0) {
							SetMaterial(Child, 256);
						}
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("Loop Command", { "loopcmd" }, "Loop Command", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() >= 1) {
			std::string builtString = "";
			for (int i = 0; i < In.size(); i++) {
				builtString += In.at(i);
				if(i != In.size())	builtString += " ";
			}
			LoopCommands.clear();
			LoopCommands.push_back(builtString);
			
		}
	});
	Rosin::CmdManager->NewCommand("Stop Loop Command", { "stoploop" }, "Stop Loop Command", (CommandFunction)[](std::vector<std::string> In)->void {
		LoopCommands.clear();
	});
	Rosin::CmdManager->NewCommand("Get Loop Command", { "getloopcmd" }, "asd", (CommandFunction)[](std::vector<std::string> In)->void {
		if(LoopCommands.size() > 0)
			Rosin::Form::GetBaseInst()->OutputText(LoopCommands[0]);
	});
	Rosin::CmdManager->NewCommand("Set Loop Command Time", { "looptime" }, "asd", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			int asd = std::stoi(In[0]);
			LoopCommandSleepTime = asd;
		}
	});
	Rosin::CmdManager->NewCommand("SilentGod", { "sgod" }, "sgod", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> Plrs = Rosin::Roblox::GetPlayer(In.at(0));
			for (int c = 0; c < Plrs.size(); c++) {
				int plr = Plrs.at(c);
				SilentGodded.push_back(*GetName(plr));
			}
		}
	});
	Rosin::CmdManager->NewCommand("UnSilentGod", { "unsgod" }, "unsgod", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> Plrs = Rosin::Roblox::GetPlayer(In.at(0));
			for (int c = 0; c < Plrs.size(); c++) {
				int plr = Plrs.at(c);
				for (int i = 0;i < SilentGodded.size();i++) {
					if (SilentGodded[i] == *GetName(plr)) {
						SilentGodded.erase(SilentGodded.begin() + i);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("Silent God List", { "sgodlist" }, "sgod list", (CommandFunction)[](std::vector<std::string> In)->void {
		for (int i = 0;i < SilentGodded.size();i++) {
			Rosin::Form::GetBaseInst()->OutputText(SilentGodded[i]);
		}
	});
	Rosin::CmdManager->NewCommand("Reload", { "reload" }, "kek", (CommandFunction)[](std::vector<std::string> In)->void {
		std::thread reload(ReloadRosin);
		reload.detach();
	});
	Rosin::CmdManager->NewCommand("hat", { "hat" }, "kek", (CommandFunction)[](std::vector<std::string> In)->void {
		//RecurseUnanchor(Workspace);
		int srv = FindFirstClass(DataModel, "InsertService");
		if (srv) {
			int asdfat = std::atof(In.at(0).c_str());
			InsertAsset(srv, asdfat);

			Sleep(1000);

			srv = FindFirstClass(DataModel, "InsertService");

			std::vector<DWORD> MyChar = GetCharacter("me");
			if (MyChar.size() == 1) {
				Holder.clear();//
				std::vector<DWORD> hats = RecurseFindHat(srv);
				if (hats.size() > 0) {
					for (int c = 0; c < hats.size(); c++) {
						int hat = hats[c];
						rSetParent(hat, MyChar[0]);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("gear", { "gear" }, "gear", (CommandFunction)[](std::vector<std::string> In)->void {
		//RecurseUnanchor(Workspace);
		int srv = FindFirstClass(DataModel, "InsertService");
		if (srv) {
			int asdfat = std::atof(In.at(0).c_str());
			InsertAsset(srv, asdfat);

			Sleep(1000);

			srv = FindFirstClass(DataModel, "InsertService");

			std::vector<DWORD> MyChar = GetCharacter("me");
			if (MyChar.size() == 1) {
				Holder2.clear();//
				std::vector<DWORD> hats = RecurseFindGear(srv);
				if (hats.size() > 0) {
					for (int c = 0; c < hats.size(); c++) {
						int hat = hats[c];
						rSetParent(hat, MyChar[0]);
					}
				}
			}
		}
	});
	/*Rosin::CmdManager->NewCommand("johntron", { "johntron" }, "kek", (CommandFunction)[](std::vector<std::string> In)->void {
		int srv = FindFirstClass(DataModel, "InsertService");
		if (srv) {
			InsertAsset(srv, 47431);
			Sleep(1000);
			srv = FindFirstClass(DataModel, "InsertService");

			Holder3.clear();//
			std::vector<DWORD> skys = RecurseFindSky(srv);
			if (skys.size() > 0) {
				for (int c = 0; c < skys.size(); c++) {
					int sky = skys[c];
					rSetParent(sky, Lighting);
				}
			}
		}
	});*/
	Rosin::CmdManager->NewCommand("spectate", { "spectate" }, "kek", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> Chars = GetCharacter(In.at(0));
			for (int c = 0;c < Chars.size();c++) {
				int Hum = FindFirstClass(Chars[c], "Humanoid");
				if (Hum) {
					int Camera = GetCurrentCamera(Workspace);
					if (Camera) {
						CameraSubject(Camera, Hum);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("unspectate", { "unspectate" }, "kek", (CommandFunction)[](std::vector<std::string> In)->void {
		int me = GetLocalPlayer(Players);
		if (me) {
			int chr = GetPlrCharacter(me);
			if (chr) {
				int Hum = FindFirstClass(chr, "Humanoid");
				if (Hum) {
					int Camera = GetCurrentCamera(Workspace);
					if (Camera) {
						CameraSubject(Camera, Hum);
					}
				}
			}
		}
	});
	Rosin::CmdManager->NewCommand("exescript", { "exescript" }, "kek", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			char Buffer[2048] = {};
			GetModuleFileName(GlobalDLLModule, Buffer, sizeof(Buffer));
			std::string FilePath(Buffer);

			std::string Directory;
			if (FilePath.find_last_of('\\') != std::string::npos) {
				Directory = FilePath.substr(0, FilePath.find_last_of('\\'));
			}

			std::ifstream File;
			long FileLength;
			File.open(Directory + "\\scripts\\" + In.at(0), std::ios::binary | std::ios::in);
			if (File.is_open()) {
				File.seekg(0, std::ios::end);
				FileLength = File.tellg();
				File.seekg(0, std::ios::beg);

				char* Data = new char[FileLength];
				File.read(Data, FileLength);

				std::string Source = std::string(Data, FileLength);

				LuaExecuteString(Source);
			}
		}
	});

	Rosin::CmdManager->NewCommand("ff", { "ff" }, "", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> Chars = GetCharacter(In.at(0));
			for (int c = 0; c < Chars.size(); c++) {
				int Char = Chars[c];
				int newFF = CreateInstance("ForceField");
				if (newFF > 0) {
					rSetParent(newFF, Char);
				}
			}
		}
	});

	Rosin::CmdManager->NewCommand("fire", { "fire" }, "", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> Chars = GetCharacter(In.at(0));
			for (int c = 0; c < Chars.size(); c++) {
				int Char = Chars[c];
				int newFF = CreateInstance("Fire");
				if (newFF > 0) {
					int Torso = FindFirstChild_PartOf(Char, "Torso");
					if (Torso) {
						rSetParent(newFF, Torso);
					}
				}
			}
		}
	});

	Rosin::CmdManager->NewCommand("sp", { "sp" }, "", (CommandFunction)[](std::vector<std::string> In)->void {
		if (In.size() == 1) {
			std::vector<DWORD> Chars = GetCharacter(In.at(0));
			for (int c = 0; c < Chars.size(); c++) {
				int Char = Chars[c];
				int newFF = CreateInstance("Sparkles");
				if (newFF > 0) {
					int Torso = FindFirstChild_PartOf(Char, "Torso");
					if (Torso) {
						rSetParent(newFF, Torso);
					}
				}
			}
		}
	});

	Rosin::CmdManager->NewCommand("testasd", { "fatnerds" }, "", (CommandFunction)[](std::vector<std::string> In)->void {

		struct membuf : std::streambuf {
			membuf(char* b, char* e) { this->setg(b, b, e); }
		};
		std::string in = "<roblox></roblox>";
		char* Buffer = (char*)malloc(sizeof(char) * in.size());
		strcpy(Buffer, in.c_str());
		membuf StringBuffer(Buffer, std::find(Buffer, Buffer + strlen(Buffer), 0));
		std::istream ret(&StringBuffer);
		((void(__fastcall*)(int, std::istream*, int))0x6A7CE0)(0, &ret, DataModel);
	});
}

bool crackCheck = false;

int checkTwo = 5;

char* mfm;
/*
void inline TrampolineFunc(DWORD lib_function, void* Data, ...) {
	__asm {
		mov [ebx + 4], lib_function
		jmp dword ptr[lib_function]
		ret
	}
}*/

bool ProcessRunning(const char* name)
{
	HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (SnapShot == INVALID_HANDLE_VALUE)
		return false;

	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(SnapShot, &procEntry))
		return false;

	do
	{
		if (strcmp(procEntry.szExeFile, name) == 0)
			return true;
	} while (Process32Next(SnapShot, &procEntry));

	return false;
}

bool windowCheck() {
	if (ProcessRunning("Fiddler.exe")) {
		return true;
	}
	if (FindWindow(NULL, "Fiddler Web Debugger")) {
		return true;
	}
	return false;
}

#define _WIN32_WINNT 0x0400

#define AuthOffset 50204626 // ill change this every update ;)

/*std::string GetMac()
{
    VMProtectBeginUltra("iwannajumpintolavacuzofxyz");
    char data[4096];
    ZeroMemory(data, 4096);
    unsigned long  len = 4000;
    PIP_ADAPTER_INFO pinfo = (PIP_ADAPTER_INFO)data;
    char sbuf[20];
    std::string sret;

    DWORD ret = GetAdaptersInfo(pinfo, &len);
    if (ret != ERROR_SUCCESS)
        return std::string("ERROR");

    for (int k = 0; k < 5; k++) {
        sprintf(sbuf, "%02X-", pinfo->Address[k]);
        sret += sbuf;
    }
    sprintf(sbuf, "%02X", pinfo->Address[5]);
    sret += sbuf;
    VMProtectEnd();
    return(sret);
}*/

std::string encryptDecrypt(std::string toEncrypt) {
	char key[8] = { 'E', 'A', 'T', 'M', 'Y', 'A', 'S', 'S' }; //Any chars will work
	std::string output = toEncrypt;

	for (int i = 0; i < toEncrypt.size(); i++)
		output[i] = toEncrypt[i] ^ key[i % (sizeof(key) / sizeof(char))];

	return output;
}


std::string GetFilePath() {
	char Buffer[2048];
	GetModuleFileName(GlobalDLLModule, Buffer, sizeof(Buffer));
	return std::string(Buffer);
}

bool didRun = false;

bool HashChecker() {
	std::vector<char> Serial = {};
	MasterHardDiskSerial* SerialClass = new MasterHardDiskSerial();
	SerialClass->GetSerialNo(Serial);
	std::string Guid1 = std::string(Serial.begin(), Serial.end());
	HW_PROFILE_INFO hwProfileInfo;

	if (GetCurrentHwProfile(&hwProfileInfo) != NULL) {
		const char* guid = hwProfileInfo.szHwProfileGuid;
		std::string Guid2 = std::string(guid);
		std::string hashedGuid = md5(Guid1 + "-" + Guid2);

		std::string FilePath = GetFilePath();
		std::string Directory;
		if (FilePath.find_last_of('\\') != std::string::npos) {
			Directory = FilePath.substr(0, FilePath.find_last_of('\\'));
		}
		std::ifstream File;
		long FileLength;
		File.open(FilePath.c_str(), std::ios::binary | std::ios::in);
		File.seekg(0, std::ios::end);
		FileLength = File.tellg();
		File.seekg(0, std::ios::beg);

		char* Data = new char[FileLength];
		File.read(Data, FileLength);

		std::string hashed = md5(std::string(Data, FileLength));

		std::string LatestVersion = DownloadURL("http://dark-hax.net/Updating/checksum.php");
		if (LatestVersion != hashed) {
			return false;
		}
		else {
			didRun = true;
			return true;
		}
	}
	return false;
}

bool devMode1 = true;

bool Authenticate() {

	if (!devMode1) {
		bool GoodHash = HashChecker();
		if (GoodHash == false && didRun == true) {
			std::abort();
		}
		else if (didRun == false) {
			std::abort();
		}
	}

	std::vector<char> Serial = {};
	MasterHardDiskSerial* SerialClass = new MasterHardDiskSerial();
	SerialClass->GetSerialNo(Serial);
	std::string Guid1 = std::string(Serial.begin(), Serial.end());
	HW_PROFILE_INFO hwProfileInfo;

	if (GetCurrentHwProfile(&hwProfileInfo) != NULL) {
		const char* guid = hwProfileInfo.szHwProfileGuid;
		std::string Guid2 = std::string(guid);
		std::string hashedGuid = md5(Guid1 + "-" + Guid2);

		std::string url = "http://dark-hax.net/abc.php?h=";
		url += hashedGuid;

		std::string Response = DownloadURL(url.c_str());
		try {
			int Num = ::atof(Response.c_str());//ya
			if ((Num - AuthOffset) % 534059 != 0) {
				return false;
			}
		}
		catch (...) {
			return false;
		}
	}
	crackCheck = true;
	checkTwo = pow(checkTwo, 5);


	return true;
}

void Output(const char* Text) {
	Rosin::Form::GetBaseInst()->OutputText(std::string(Text));
}

void retCheckBypass() {

	DWORD FArg = (DWORD)LocalPlayer;
	DWORD SArg = (DWORD)1;
	DWORD FragmentAddr = (DWORD)0x4F2657;
	DWORD FuncAddr = (DWORD)0x4F2520;

	const char* T = "Testing!!";
	__asm {
		push returnshere
		push SArg
		push FArg
		push FragmentAddr
		jmp  FuncAddr
		returnshere:
		mov eax, dword ptr[Output]
		push dword ptr[T]
		call eax
		retn
	}
}

//kekke

std::vector<std::string> getFiles(std::string folder)
{
	std::vector<std::string> names;
	char search_path[200];
	sprintf(search_path, "%s/*.*", folder.c_str());
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

void AutoExecuteScripts() {
	std::string FilePath = GetFilePath();
	std::string Directory;
	if (FilePath.find_last_of('\\') != std::string::npos) {
		Directory = FilePath.substr(0, FilePath.find_last_of('\\'));
	}

	std::vector<std::string> files = getFiles(Directory + "\\autoexec");

	if (files.size() != 0) {
		for (int c = 0; c < files.size(); c++) {
			std::ifstream File;
			long FileLength;
			File.open(Directory + "\\autoexec\\" + files[c], std::ios::binary | std::ios::in);
			File.seekg(0, std::ios::end);
			FileLength = File.tellg();
			File.seekg(0, std::ios::beg);

			char* Data = new char[FileLength];
			File.read(Data, FileLength);

			std::string Source = std::string(Data, FileLength);
			
			LuaExecuteString(Source);
		}
	}
}

bool autoUpdate() {
	try { remove("Old_Rosin.dll"); }
	catch(...) { }
	std::string FilePath = GetFilePath();
	std::string Directory;
	if (FilePath.find_last_of('\\') != std::string::npos) {
		Directory = FilePath.substr(0, FilePath.find_last_of('\\'));
	}
	std::ifstream File;
	long FileLength;
	File.open(FilePath.c_str(), std::ios::binary | std::ios::in);
	File.seekg(0, std::ios::end);
	FileLength = File.tellg();
	File.seekg(0, std::ios::beg);

	char* Data = new char[FileLength];
	File.read(Data, FileLength);

	std::string hashed = md5(std::string(Data, FileLength));
	std::string LatestVersion = DownloadURL("http://dark-hax.net/Updating/checksum.php");
	File.close();
	if (hashed != LatestVersion) {
		Rosin::Form::GetBaseInst()->RawOutputText("Update is available, downloading newest update...");
		
		//Update Changelog
		std::ofstream ChangeLog;
		std::string ChangeLogData = DownloadURL("http://dark-hax.net/Updating/changelog.php");;
		ChangeLog.open(std::string(Directory + "\\Changelog.txt").c_str(), std::ofstream::out | std::ofstream::trunc);
		if (ChangeLog.is_open()) {
			ChangeLog.write(ChangeLogData.c_str(), ChangeLogData.size());
		}
		ChangeLog.close();

		std::ofstream LuaApi;
		std::string LuaApiStr = DownloadURL("http://dark-hax.net/Updating/luaapi.php");;
		LuaApi.open(std::string(Directory + "\\Lua Api.txt").c_str(), std::ofstream::out | std::ofstream::trunc);
		if (LuaApi.is_open()) {
			LuaApi.write(LuaApiStr.c_str(), LuaApiStr.size());
		}
		LuaApi.close();

		//Update DLL
		if (MoveFileEx(FilePath.c_str(), std::string(Directory + "\\Old_Rosin.dll").c_str(), MOVEFILE_REPLACE_EXISTING) == 0) {
			int Error = GetLastError();
			Rosin::Form::GetBaseInst()->RawOutputText("Failed " + std::to_string(Error));
			return true;
		}
		
		std::vector<char> Serial = {};
		MasterHardDiskSerial* SerialClass = new MasterHardDiskSerial();
		SerialClass->GetSerialNo(Serial);
		std::string Guid1 = std::string(Serial.begin(), Serial.end());
		HW_PROFILE_INFO hwProfileInfo;

		if (GetCurrentHwProfile(&hwProfileInfo) != NULL) {
			const char* guid = hwProfileInfo.szHwProfileGuid;
			std::string Guid2 = std::string(guid);
			std::string hashedGuid = md5(Guid1 + "-" + Guid2);

			std::ofstream RosinNew;
			std::string RosinNewData = DownloadURL(std::string("http://dark-hax.net/Updating/updaterosin.php?a=" + hashedGuid).c_str(), NULL, true);
			RosinNewData = base64_decode(RosinNewData);
			RosinNew.open(std::string(Directory + "\\RosinNew.dll").c_str(), std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
			if (RosinNew.is_open()) {
				RosinNew.write(RosinNewData.c_str(), RosinNewData.size());
			}
			RosinNew.close();
		}

		delete[] Data;
		//Rosin::Form::GetBaseInst()->OutputText(FilePath);
		return true;
	}
	else {
		Rosin::Form::GetBaseInst()->OutputText("up to date.");
	}

	delete[] Data;
	return false;
}
//

BOOL EnableDebugPrivilege() {
	HANDLE hToken;
	LUID luid;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		return FALSE;
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
	{
		return FALSE;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luid;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, false, &tkp, sizeof(tkp), NULL, NULL))
	{
		return FALSE;
	}

	if (!CloseHandle(hToken))
	{
		return FALSE;
	}

	return TRUE;
}

void rbxpushstring(DWORD a1, const char* str) {
	/*
	if ( *(_DWORD *)(*(_DWORD *)(a1 + 8) + a1 + 96) >= *(_DWORD *)(*(_DWORD *)(a1 + 8) + a1 + 84) )
    sub_758C80(a1);
  v3 = *(_DWORD *)(a1 + 16);
  *(_DWORD *)v3 = sub_518DD0(a1, a2, a3);
  *(_DWORD *)(v3 + 8) = 4;
  *(_DWORD *)(a1 + 16) += 16;*/

	if (*(DWORD *)(*(DWORD *)(a1 + 8) + a1 + 96) >= *(DWORD *)(*(DWORD *)(a1 + 8) + a1 + 84))
		((int(__cdecl*)(DWORD state))0x758C80)(a1);
	int v3 = *(DWORD *)(a1 + 16);
	*(DWORD *)v3 = ((int(__cdecl*)(DWORD state, int str, int len))0x518DD0)(a1, (int)str, strlen(str));
	*(DWORD *)(v3 + 8) = 4;
	*(DWORD *)(a1 + 16) += 16;
}

void rbxsettop(DWORD a1, int a2) {
	/* if ( a2 < 0 )
  {
    *(_DWORD *)(a1 + 16) += 16 * a2 + 16;
  }
  else
  {
    for ( i = 16 * a2; *(_DWORD *)(a1 + 16) < (unsigned int)(i + *(_DWORD *)(a1 + 28)); *(_DWORD *)(a1 + 16) += 16 )
      *(_DWORD *)(*(_DWORD *)(a1 + 16) + 8) = 0;
    *(_DWORD *)(a1 + 16) = i + *(_DWORD *)(a1 + 28);
  }*/

	if (a2 < 0)
	{
		*(DWORD *)(a1 + 16) += 16 * a2 + 16;
	}
	else
	{
		size_t i;
		for (i = 16 * a2; *(DWORD *)(a1 + 16) < (unsigned int)(i + *(DWORD *)(a1 + 28)); *(DWORD *)(a1 + 16) += 16)
			*(DWORD *)(*(DWORD *)(a1 + 16) + 8) = 0;
		*(DWORD *)(a1 + 16) = i + *(DWORD *)(a1 + 28);
	}
}

int rbxpcall(DWORD a1, int a2, int a3, int a4) {
	/*if ( a4 )
    v4 = sub_516340(a1, a4) - *(_DWORD *)(a1 + 24);
  else
    v4 = 0;
  v10 = *(_DWORD *)(a1 + 16) - 16 * (a2 + 1);
  v5 = v10 - *(_DWORD *)(a1 + 24);
  v11 = a3;
  v6 = sub_519B80(a1, (int)sub_516300, (int)&v10, v5, v4);
  if ( a3 == -1 )
  {
    v7 = *(_DWORD *)(a1 + 12);
    v8 = *(_DWORD *)(a1 + 16);
    if ( v8 >= *(_DWORD *)(v7 + 8) )
      *(_DWORD *)(v7 + 8) = v8;
  }*/
	int v4 = 0;
	if (a4)
		v4 = ((int(__cdecl*)(int dat, int dis))0x516340)(a1, a4) - *(DWORD *)(a1 + 24);
	else
		v4 = 0;
	int v10 = *(DWORD *)(a1 + 16) - 16 * (a2 + 1);
	int v5 = v10 - *(DWORD *)(a1 + 24);
	int v11 = a3;
	int v6 = ((int(__cdecl*)(int a1, int a2, int a3, int a4, int a5))0x519B80)(a1, (int)(0x516300), (int)&v10, v5, v4);
	if (a3 == -1)
	{
		int v7 = *(DWORD *)(a1 + 12);
		int v8 = *(DWORD *)(a1 + 16);
		if (v8 >= *(DWORD *)(v7 + 8))
			*(DWORD *)(v7 + 8) = v8;
	}
	return v6;
}

typedef int(__cdecl* test1)(int, int);
typedef int(__cdecl* test2)(int, const char*, int);
typedef void(__cdecl* test3)(int, int, int, int);

/* void rbxgetfield(DWORD a1, int a2, const char* a3) {
	/*v3 = sub_516340(a1, a2);
  v4 = sub_518DD0(a1, (int)a3, strlen(a3));
  v5 = *(_DWORD *)(a1 + 16);
  v6 = v4;
  v7 = 4;
  sub_75D120(a1, v3, (int)&v6, v5);
  *(_DWORD *)(a1 + 16) += 16;/
	test1 fat = (test1)0x516340;
	test2 fatter = (test2)0x518DD0;
	test3 fattest = (test3)0x75D120;
	int v3 = fat(a1, a2);
	int v4 = fatter(a1, a3, strlen(a3));
	int v5 = *(DWORD *)(a1 + 16); // df it did +4 not 16
	int v6 = v4;
	int v7 = 4;
	fattest(a1, v3, (int)&v6, v5);
	*(DWORD *)(a1 + 16) += 16;
}*/



/*
mov     eax, dword_125612C
or      dword_1256128, 200000h
push    esi
mov     dword_125612C, eax
mov     dword_169D07C, 0

.text:00516B20 var_10          = dword ptr -10h
.text:00516B20 var_8           = dword ptr -8
.text:00516B20 arg_0           = dword ptr  8
.text:00516B20 arg_4           = dword ptr  0Ch
.text:00516B20 arg_8           = dword ptr  10h
*/

DWORD sub_516340 = 0x516340;
DWORD sub_518DD0 = 0x518DD0;
DWORD sub_75D120 = 0x75D120;
DWORD sub_51B360 = 0x51B360; // <-- RetCheck rip
DWORD off_FAF3E0 = 0xFAF3E0;
DWORD off_FAF3DC = 0xFAF3DC;
DWORD dword_125612C = 0x125612C;
DWORD dword_1256128 = 0x1256128;
DWORD dword_169D07C = 0x169D07C;

/*__declspec(naked) void rbxgetfield(DWORD, int, const char*) {
	__asm {
		push    ebp
		mov     ebp, esp
		sub     esp, 10h
		push    ebx
		push    esi
		mov     esi, [ebp + 8]
		push    edi
		push    [ebp + 0Ch]
		
		mov     edx, 516340h
		push    esi
		call    edx
		mov     edx, [ebp + 10h]
		mov     ecx, edx
		add     esp, 8
		mov     edi, eax
		lea     ebx, [ecx + 1]
				
		hax:
			mov     al, [ecx]
			inc     ecx
			test    al, al
			jnz     hax
			sub     ecx, ebx
			push    ecx
			push    edx
			push    esi
			mov     edx, 518DD0h
			call    edx
			push    dword ptr[esi + 10h]
			mov     [ebp - 10h], eax
			lea     eax, [ebp - 10h]
			push    eax
			push    edi
			push    esi
			mov     [ebp - 8h], 4
			mov     edx, 75D120h
			call    edx
			add     dword ptr[esi + 10h], 10h
			pop     esi
			pop     ebx
			mov     esp, ebp
			pop     ebp
			retn
	}
}*/

void rbxgetfieldasd(DWORD a1, signed int a2, const char* a3) {
	/* v3 = sub_516340(a1, a2);
  v4 = sub_518DD0(a1, a3, strlen((const char *)a3));
  v5 = *(_DWORD *)(a1 + 16);
  v6 = v4;
  v7 = 4;
  sub_75D120(a1, v3, (int)&v6, v5);
  *(_DWORD *)(a1 + 16) += 16;*/
	int v3 = ((int(__cdecl*)(int,signed int))0x516340)(a1, a2);
	int v4 = ((int(__cdecl*)(int,const char*,unsigned int))0x518DD0)(a1, a3, strlen((const char *)a3));
	int v5 = *(DWORD *)(a1 + 16);
	int v6 = v4;
	int v7 = 4;
	((void(__cdecl*)(int,int,int,int))0x75D120)(a1, v3, v6, v5);
	*(DWORD *)(a1 + 16) += 16;
}



DWORD Fragment = 0x40310C;
void Success(int a, int b, const char* c) {
	int one = a;
	int two = b;
	const char* three = c;
	Rosin::Form::GetBaseInst()->OutputText("Omg it worked?");
}
DWORD RetCheckedFunc = (DWORD)Success;
__declspec(naked) void rbxgetfield(DWORD a1, int a2, const char* a3) {
	__asm {
		push a3
			push a2
			push a1
			push Fragment
			push funcret
			jmp  RetCheckedFunc
			funcret :
			nop
			nop
	}
}

void rbxsetfield(DWORD a1, int a2, const char* a3) {
	/*v3 = sub_516340(a1, a2);
  v6 = sub_518DD0(a1, a3, strlen((const char *)a3));
  v4 = *(_DWORD *)(a1 + 16) - 16;
  v7 = 4;
  sub_75D290(a1, v3, &v6, v4);
  *(_DWORD *)(a1 + 16) -= 16;*/
	int v3 = ((int(__cdecl*)(int dat, int dis))0x516340)(a1, a2);
	int v6 = ((int(__cdecl*)(int dat, const char* dis, int a3))0x518DD0)(a1, a3, strlen((const char *)a3));
	int v4 = *(DWORD *)(a1 + 16) - 16;
	int v7 = 4;
	((int(__cdecl*)(int dat, int dis, int a3, int a4))0x75D290)(a1, v3, (int)&v6, v4);
	*(DWORD *)(a1 + 16) -= 16;
}

const char* rbxtostring(DWORD State, int Idx) {
	return ((const char*(__cdecl*)(DWORD state, int idx))0x72E850)(State, Idx);
}

int rbxtype(DWORD State, int Idx) {
	return ((int(__cdecl*)(DWORD state, int idx))0x5185B0)(State, Idx);
}

int rbxgettop(DWORD a1) {
	return (*(DWORD *)(a1 + 16) - *(DWORD *)(a1 + 28)) >> 4;
}

void rbxreglib(DWORD a1, const char* a2, int a3) {
	((void(__cdecl*)(int, const char*, int))0x71F720)(a1, a2, a3);
}

// right now i'd like to stop and give my saying and prayers to the all mighty ida pro 6.6

void rbxpushfunc(DWORD a1, int a2, int a3) {
	/*int v3; // edi@1
  int v4; // eax@3
  int v5; // eax@4
  int v6; // ebx@6
  int v7; // eax@6
  int v8; // ecx@6
  int v9; // edx@9
  int v10; // ecx@10
  int v11; // eax@11
  void *retaddr; // [sp+8h] [bp+4h]@11
  int v13; // [sp+Ch] [bp+8h]@6
  int v14; // [sp+14h] [bp+10h]@6

  v3 = a1;
  if ( *(_DWORD *)(*(_DWORD *)(a1 + 8) + a1 + 96) >= *(_DWORD *)(*(_DWORD *)(a1 + 8) + a1 + 84) )
    sub_758C80(a1);
  v4 = *(_DWORD *)(a1 + 12);
  if ( v4 == *(_DWORD *)(a1 + 40) )
    v5 = *(_DWORD *)(a1 + 104);
  else
    v5 = *(_DWORD *)(**(_DWORD **)(v4 + 16) + 12);
  v6 = a3;
  v7 = sub_5193B0(a1, a3, v5);
  *(_DWORD *)(v7 + 16) = a2 - (v7 + 16);
  v8 = 16 * a3;
  *(_DWORD *)(a1 + 16) -= 16 * a3;
  v13 = v7;
  v14 = 16 * a3;
  if ( a2 - (signed int)unusedPadding >= (unsigned int)((char *)&loc_AC2EF6 + 4) )
  {
    sub_51B360();
    v8 = v14;
  }
  if ( v6 )
  {
    v9 = v8 + v13 + 24;
    do
    {
      v9 -= 16;
      v10 = -24 - v13 + *(_DWORD *)(v3 + 16);
      *(_DWORD *)v9 = *(_DWORD *)(v10 + v9);
      *(_DWORD *)(v9 + 4) = *(_DWORD *)(v10 + v9 + 4);
      *(_DWORD *)(v9 + 8) = *(_DWORD *)(v10 + v9 + 8);
      --v6;
    }
    while ( v6 );
  }
  v11 = *(_DWORD *)(v3 + 16);
  *(_DWORD *)v11 = v13;
  *(_DWORD *)(v11 + 8) = 6;
  *(_DWORD *)(v3 + 16) += 16;*/
	int v3; // edi@1
	int v4; // eax@3
	int v5; // eax@4
	int v6; // ebx@6
	int v7; // eax@6
	int v8; // ecx@6
	int v9; // edx@9
	int v10; // ecx@10
	int v11; // eax@11
	int v13; // [sp+Ch] [bp+8h]@6
	int v14; // [sp+14h] [bp+10h]@6

	v3 = a1;
	if (*(DWORD *)(*(DWORD *)(a1 + 8) + a1 + 96) >= *(DWORD *)(*(DWORD *)(a1 + 8) + a1 + 84))
		((int(__cdecl*)(int))0x758C80)(a1);
	v4 = *(DWORD *)(a1 + 12);
	if (v4 == *(DWORD *)(a1 + 40))
		v5 = *(DWORD *)(a1 + 104);
	else
		v5 = *(DWORD *)(**(DWORD **)(v4 + 16) + 12);
	v6 = a3;
	v7 = ((int(__cdecl*)(int,int,int))0x5193B0)(a1, a3, v5);
	*(DWORD *)(v7 + 16) = a2 - (v7 + 16);
	v8 = 16 * a3;
	*(DWORD *)(a1 + 16) -= 16 * a3;
	v13 = v7;
	v14 = 16 * a3;
	if (v6)
	{
		v9 = v8 + v13 + 24;
		do
		{
			v9 -= 16;
			v10 = -24 - v13 + *(DWORD *)(v3 + 16);
			*(DWORD *)v9 = *(DWORD *)(v10 + v9);
			*(DWORD *)(v9 + 4) = *(DWORD *)(v10 + v9 + 4);
			*(DWORD *)(v9 + 8) = *(DWORD *)(v10 + v9 + 8);
			--v6;
		} while (v6);
	}
	v11 = *(DWORD *)(v3 + 16);
	*(DWORD *)v11 = v13;
	*(DWORD *)(v11 + 8) = 6;
	*(DWORD *)(v3 + 16) += 16;
}

#define LUA_REGISTRYINDEX	(-10000)
#define LUA_ENVIRONINDEX	(-10001)
#define LUA_GLOBALSINDEX	(-10002)
#define lua_upvalueindex(i)	(LUA_GLOBALSINDEX-(i))


#define LUA_TNONE		(-1)

#define LUA_TNIL		0
#define LUA_TBOOLEAN		1
#define LUA_TLIGHTUSERDATA	2
#define LUA_TNUMBER		3
#define LUA_TSTRING		4
#define LUA_TTABLE		5
#define LUA_TFUNCTION		6
#define LUA_TUSERDATA		7
#define LUA_TTHREAD		8


#define lua_pop(L,n)		rbxsettop(L, -(n)-1)

#define lua_newtable(L)		lua_createtable(L, 0, 0)

#define lua_register(L,n,f) (lua_pushcfunction(L, (f)), lua_setglobal(L, (n)))

#define lua_pushcfunction(L,f)	lua_pushcclosure(L, (f), 0)

#define lua_strlen(L,i)		lua_objlen(L, (i))

#define lua_isfunction(L,n)	(rbxtype(L, (n)) == LUA_TFUNCTION)
#define lua_istable(L,n)	(rbxtype(L, (n)) == LUA_TTABLE)
#define lua_islightuserdata(L,n)	(rbxtype(L, (n)) == LUA_TLIGHTUSERDATA)
#define lua_isnil(L,n)		(rbxtype(L, (n)) == LUA_TNIL)
#define lua_isboolean(L,n)	(rbxtype(L, (n)) == LUA_TBOOLEAN)
#define lua_isthread(L,n)	(rbxtype(L, (n)) == LUA_TTHREAD)
#define lua_isnone(L,n)		(rbxtype(L, (n)) == LUA_TNONE)
#define lua_isnoneornil(L, n)	(rbxtype(L, (n)) <= 0)

#define lua_setglobal(L,s)	rbxsetfield(L, LUA_GLOBALSINDEX, (s))
#define lua_getglobal(L,s)	rbxgetfield(L, LUA_GLOBALSINDEX, (s))



/*
** compatibility macros and functions
*/

#define lua_open()	luaL_newstate()

#define lua_getregistry(L)	lua_pushvalue(L, LUA_REGISTRYINDEX)

#define lua_getgccount(L)	lua_gc(L, LUA_GCCOUNT, 0)

#define lua_Chunkreader		lua_Reader
#define lua_Chunkwriter		lua_Writer

void blankHolder(){
	return;
}

DWORD Memcheck = 0x6497D5;
void LimitedBypass() {
	Scanning::PlaceJMP((BYTE*)Memcheck, (DWORD)blankHolder, 2);
}

DWORD Retcheck = 0x51B360;
void JmpRetcheck() {
	Scanning::PlaceJMP((BYTE*)Retcheck, (DWORD)blankHolder, 2);
}


void retcheckeatmyass() {
	while (true) {
		*(int*)(0x169D07C) = -1;
		*(int*)(0x1256128) = 0xffffffff;
	}
}

bool AlwaysFalseVal = false;
DWORD luaState;
void Main() {
	if (AlwaysFalseVal == true) {
		rbxsetfield(luaState, -1337, "ptr__asd");
		rbxgetfield(luaState , -10002, "Hax");
	}
	new Rosin::Form("ROSINV2_KEK", "Rosin v2.1", (Rosin::OnLoad)[](Rosin::Form* arg1)->void {
		Rosin::Form::GetBaseInst()->RawOutputText("Authenticating.. ", RGB(0, 255, 0));

		bool Authed = Authenticate();

		if (Authed == false || windowCheck()) {
			Rosin::Form::GetBaseInst()->RawOutputText("failed.", RGB(255, 0, 0));
			Sleep(500);
			std::abort();
		}
		else if (crackCheck == false || checkTwo != 3125) {
			Rosin::Form::GetBaseInst()->RawOutputText("failed.", RGB(255, 0, 0));
			Sleep(500);
			std::abort();
		}
		else {
			Rosin::Form::GetBaseInst()->OutputText("done");
		}

		Rosin::Form::GetBaseInst()->RawOutputText("Checking for updates.. ", RGB(0, 255, 0));

		bool Updated = ProcessRunning("devenv.exe") ? false : autoUpdate();

		Rosin::Form::GetBaseInst()->OutputText("done.");
		
		if (Updated) {
			Rosin::Form::GetBaseInst()->OutputText("Please restart ROBLOX and inject Rosin.");
			Sleep(5000);
			return exit(0);
		}
		Rosin::Form::GetBaseInst()->RawOutputText("Finding addresses.. ", RGB(255, 0, 0));

		//Roblox::DataModel = Scanning::Scan(PAGE_READWRITE, "\x3C\x2E\xFB\x00", "xxxx");
		int ScriptContext = Scanning::Scan(PAGE_READWRITE, "\x14\xAC\xFA\x00", "xxxx");
		//Note: Scan for ScriptContext VF Table
		if (ScriptContext != 0) {
			Roblox::DataModel = Roblox::GetParent(ScriptContext);
		}
		if (Roblox::DataModel == 0) {
			Rosin::CmdManager->NewCommand("Reload", { "reload" }, "kek", (CommandFunction)[](std::vector<std::string> In)->void {
				std::thread reload(ReloadRosin);
				reload.detach();
			});
			Rosin::Form::GetBaseInst()->OutputText("\nUh oh! It seems ROBLOX has updated (or maybe it just failed)!", RGB(255, 0, 0));
			Rosin::Form::GetBaseInst()->OutputText("Please wait until the next update (you can try the 'reload' command).\n", RGB(255, 0, 0));
			std::string MOTD = DownloadURL("http://dark-hax.net/mo.td");
			Rosin::Form::GetBaseInst()->RawOutputText("Message of the day: ");
			Rosin::ColorFormat(MOTD + "\n");
			return;
		}

		Roblox::Workspace = Roblox::FindFirstClass(Roblox::DataModel, "Workspace");
		Roblox::Lighting = Roblox::FindFirstClass(Roblox::DataModel, "Lighting");
		Roblox::Players = Roblox::FindFirstClass(Roblox::DataModel, "Players");
		Roblox::Debris = Roblox::FindFirstClass(Roblox::DataModel, "Debris");
		Roblox::LogService = Roblox::FindFirstClass(Roblox::DataModel, "LogService");
		Roblox::SoundService = Roblox::FindFirstClass(Roblox::DataModel, "SoundService");
		Roblox::LocalPlayer = Roblox::GetLocalPlayer(Roblox::Players);
		Roblox::PlayerName = Roblox::GetName(Roblox::LocalPlayer);

		AddCommands();

		//LimitedBypass();
		//JmpRetcheck();

		Rosin::Form::GetBaseInst()->OutputText("done!");

		std::thread stateThread(doStateLoop);
		stateThread.detach();

		std::thread rainbowThread(doRainbowLoop);
		rainbowThread.detach();

		std::thread loopKillThread(doLoopKillLoop);
		loopKillThread.detach();

		std::thread loopCmdThread(doLoopCommand);
		loopCmdThread.detach();

		std::thread loopGoddedThread(doSilentGoddedLoop);
		loopGoddedThread.detach();

		//std::thread lmao(retcheckeatmyass);
		//lmao.detach();

		Rosin::Form::GetBaseInst()->OutputText("Autoexecuting scripts.");

		AutoExecuteScripts();

		Rosin::Form::GetBaseInst()->OutputText("Rosin v2.1 has loaded! By: Slappy826\n", RGB(255, 69, 0));

		((int(__thiscall*)(int con, int asd))0x50DB90)(ScriptContext, 2);
		luaState = *((DWORD*)ScriptContext + 52);

		//rbxpushstring(luaState, "asdtest");

		//((int(__thiscall*)(int con))0x72E970)(luaState);

		//rbxpushstring(luaState, "Hello from C++!");

		//int top = ((int(__cdecl*)(DWORD state))0x50EB90)(luaState);

		//Rosin::Form::GetBaseInst()->OutputText(std::to_string(top));

		//lua_getglobal(luaState, "_VERSION");
		//rbxpushstring(luaState, "fat");
		//rbxsetfield(luaState, -10002, "asd");
		//rbxgetfield(luaState, -10002, "print");
		//rbxpcall(luaState, 0, 0, 0);
		

		

		int Filtering = Roblox::GetFilteringEnabled(Roblox::Workspace);
		if (Filtering) {
			Rosin::Form::GetBaseInst()->OutputText("Filtering is enabled! Most commands will not work/replicate.", RGB(255, 0, 0));
		}
		else {
			Rosin::Form::GetBaseInst()->OutputText("Filtering is not enabled.");
		}

		std::string WelcomeText = "Hello, ";
		WelcomeText += Roblox::PlayerName->c_str();
		WelcomeText += "! Welcome to Rosin!\n";

		Rosin::Form::GetBaseInst()->OutputText(WelcomeText);

		std::string MOTD = DownloadURL("http://dark-hax.net/mo.td");
		Rosin::Form::GetBaseInst()->RawOutputText("Message of the day: ");
		Rosin::ColorFormat(MOTD + "\n");

		//retCheckBypass();
	});
}

void OnExit() {
	TerminateProcess(GetCurrentProcess(), 0);
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		std::atexit(OnExit);
		DisableThreadLibraryCalls(hModule);
		GlobalDLLModule = hModule;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&Main, NULL, 0, 0);
		break;
	case DLL_THREAD_ATTACH:  break;
	case DLL_THREAD_DETACH:  break;
	case DLL_PROCESS_DETACH: break;
	}
	return TRUE;
}
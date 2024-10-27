#include <string>
#include <queue>
#include <Windows.h>
#include "Compiler.hpp"
#include "CallCheck.hpp"
#include "Functions.hpp"
#include "Libs/Drawing/Drawing.hpp"
#include "Main.hpp"
#include "KeySystem.hpp"
void RegisterFunctions(DWORD);

int RenderstepHook(DWORD rL) {
	if (!Fluxus::KeySystem::ScriptQueueEmpty()) {
		const std::string Bytecode = Fluxus::KeySystem::ScriptQueueFront();

		PLOG_INFO << "Getting ready for Execution!";
		Fluxus::RBX RBX(rL);
		RBX.NewThread();
		PLOG_INFO << "Newthreaded!";
		RBX.SandboxThread();
		PLOG_INFO << "Sandboxed!";
		RBX.Deserialize("Nigga", Bytecode.c_str(), Bytecode.size());
		PLOG_INFO << "Deserialized!";
		RBX.Spawn();
		PLOG_INFO << "Spawned!";
		RBX.Pop(1);
		PLOG_INFO << "Stack Cleaned!";
	}
	return 1;
}

bool CheckState(DWORD State, DWORD InitalState, const char* TPState) {
	Fluxus::RBX RBX(State);

	RBX.GetGlobal("Enum");
	RBX.Getfield(-1, "TeleportState");
	RBX.Getfield(-1, TPState);
	DWORD CurrentState = RBX.ToPointer(-1);
	if (CurrentState == InitalState) {
		return true;
	}
	return false;
}

void StandBy() {
	Fluxus::KeySystem::ClearQueue();

	Sleep(5000);
	PLOG_INFO << "Teleport Detected. Reinit";

	std::uintptr_t DataModel = Fluxus::ReturnDataModel() + Fluxus::Offset::DataModelInstance; /*Gets the Datamodel Instance*/
	if (!DataModel) {
		PLOG_ERROR << "Failed to set DataModel!";
	}

	std::uintptr_t ScriptContext = Fluxus::FindChildOfInstance(DataModel, "ScriptContext"); /*Iterates through it until it finds Script Context*/
	if (!ScriptContext) {
		PLOG_ERROR << "Failed to set ScriptContext!";
	}

	Fluxus::RBX vRBX(Fluxus::SetState(ScriptContext));
	vRBX.SetCleanState(vRBX.NewThread()); /*Sets an original state backup*/

	std::uintptr_t State = vRBX.NewThread(); /*Sets Fluxus Globalstate*/
	vRBX.SandboxThread(); /*Gives thread own globals*/

	PLOG_INFO << "Setting Context to 7";

	*reinterpret_cast<int*>(*reinterpret_cast<int*>(State + Fluxus::Offset::EmptySpace) + 24) = 7; /*Sets the Level to 7*/
	RegisterFunctions(State);
	Fluxus::KeySystem::CreatePipe(State, Fluxus::KeySystem::EncryptHWID());
}

int TeleportHook(DWORD rL) {
	Fluxus::RBX RBX(rL);

	DWORD State = RBX.ToPointer(1);
	if (CheckState(rL, State, "InProgress")) {
		CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(StandBy), NULL, NULL, NULL);
	}
	return 1;
}

void RegisterFunctions(DWORD State) {
	PLOG_INFO << "Starting Functions";
	Fluxus::RBX RBX(State);
	Fluxus::Drawing::RegisterDrawing(State); /*Registers drawing library*/

	/*
		PushCFunction -> Pushes the function onto the stack
		Setglobal -> Sets the function to the global
	*/

	RBX.PushCFunction(Fluxus::Functions::Loadstring);
	RBX.SetGlobal("loadstring");

	RBX.PushCFunction(Fluxus::Functions::Getreg);
	RBX.SetGlobal("getreg");

	RBX.PushCFunction(Fluxus::Functions::Getrawmetatable);
	RBX.SetGlobal("getrawmetatable");

	RBX.PushCFunction(Fluxus::Functions::Setrawmetatable);
	RBX.SetGlobal("setrawmetatable");

	RBX.PushCFunction(Fluxus::Functions::Setreadonly);
	RBX.SetGlobal("setreadonly");

	RBX.PushCFunction(Fluxus::Functions::MakeReadonly);
	RBX.SetGlobal("make_readonly");

	RBX.PushCFunction(Fluxus::Functions::MakeWriteable);
	RBX.SetGlobal("make_writeable");

	RBX.PushCFunction(Fluxus::Functions::Isreadonly);
	RBX.SetGlobal("isreadonly");

	RBX.PushCFunction(Fluxus::Functions::Getnamecallmethod);
	RBX.SetGlobal("getnamecallmethod");

	RBX.PushCFunction(Fluxus::Functions::Setnamecallmethod);
	RBX.SetGlobal("setnamecallmethod");

	RBX.PushCFunction(Fluxus::Functions::CheckCaller);
	RBX.SetGlobal("checkcaller");

	RBX.PushCFunction(Fluxus::Functions::CheckCaller);
	RBX.SetGlobal("is_protosmasher_caller"); /*Compatibility*/

	RBX.PushCFunction(Fluxus::Functions::UnlockModule);
	RBX.SetGlobal("unlockmodule");

	RBX.PushCFunction(Fluxus::Functions::LockModule);
	RBX.SetGlobal("lockmodule");

	RBX.PushCFunction(Fluxus::Functions::Getgenv);
	RBX.SetGlobal("getgenv");

	RBX.PushCFunction(Fluxus::Functions::Getrenv);
	RBX.SetGlobal("getrenv");

	RBX.PushCFunction(Fluxus::Functions::TrustCheck);
	RBX.SetGlobal("TrustCheck");

	RBX.PushCFunction(Fluxus::Functions::NewCClosure);
	RBX.SetGlobal("newcclosure");

	RBX.PushCFunction(Fluxus::Functions::HttpRequest);
	RBX.SetGlobal("http_request");

	RBX.PushCFunction(Fluxus::Functions::HttpRequest);
	RBX.SetGlobal("request");

	RBX.PushCFunction(Fluxus::Functions::Fireclickdetector);
	RBX.SetGlobal("fireclickdetector");

	RBX.PushCFunction(Fluxus::Functions::Setclipboard);
	RBX.SetGlobal("setclipboard");

	RBX.PushCFunction(Fluxus::Functions::Setclipboard);
	RBX.SetGlobal("toclipboard");

	RBX.PushCFunction(Fluxus::Functions::Setclipboard);
	RBX.SetGlobal("copystring");

	RBX.PushCFunction(Fluxus::Functions::IsFluxusClosure);
	RBX.SetGlobal("is_fluxus_closure");

	RBX.PushCFunction(Fluxus::Functions::IsFluxusClosure);
	RBX.SetGlobal("is_synapse_function");

	RBX.PushCFunction(Fluxus::Functions::IsFluxusClosure);
	RBX.SetGlobal("is_protosmasher_closure");

	RBX.PushCFunction(Fluxus::Functions::ScriptHub);
	RBX.SetGlobal("ScriptHub");

	RBX.PushCFunction(Fluxus::Functions::Islclosure);
	RBX.SetGlobal("islclosure");

	RBX.PushCFunction(Fluxus::Functions::Iscclosure);
	RBX.SetGlobal("iscclosure");

	RBX.PushCFunction(Fluxus::Functions::GetGC);
	RBX.SetGlobal("getgc");

	RBX.PushCFunction(Fluxus::Functions::Getidentity);
	RBX.SetGlobal("get_thread_identity");

	RBX.PushCFunction(Fluxus::Functions::Getidentity);
	RBX.SetGlobal("get_thread_context");

	RBX.PushCFunction(Fluxus::Functions::Setidentity);
	RBX.SetGlobal("set_thread_identity");

	RBX.PushCFunction(Fluxus::Functions::Setidentity);
	RBX.SetGlobal("set_thread_context");

	RBX.PushCFunction(Fluxus::Functions::Writefile);
	RBX.SetGlobal("writefile");

	RBX.PushCFunction(Fluxus::Functions::Readfile);
	RBX.SetGlobal("readfile");

	RBX.PushCFunction(Fluxus::Functions::GetFFlag);
	RBX.SetGlobal("getfflag");

	RBX.PushCFunction(Fluxus::Functions::SetFFlag);
	RBX.SetGlobal("setfflag");

	RBX.PushCFunction(Fluxus::Functions::IsRbxActive);
	RBX.SetGlobal("isrbxactive");

	RBX.PushCFunction(Fluxus::Functions::Keypress);
	RBX.SetGlobal("keypress");

	RBX.PushCFunction(Fluxus::Functions::Keyrelease);
	RBX.SetGlobal("keyrelease");

	RBX.PushCFunction(Fluxus::Functions::Mouse1click);
	RBX.SetGlobal("mouse1click");

	RBX.PushCFunction(Fluxus::Functions::Mouse1press);
	RBX.SetGlobal("mouse1press");

	RBX.PushCFunction(Fluxus::Functions::Mouse1release);
	RBX.SetGlobal("mouse1release");

	RBX.PushCFunction(Fluxus::Functions::Mouse2click);
	RBX.SetGlobal("mouse2click");

	RBX.PushCFunction(Fluxus::Functions::Mouse2press);
	RBX.SetGlobal("mouse2press");

	RBX.PushCFunction(Fluxus::Functions::Mouse2release);
	RBX.SetGlobal("mouse2release");

	RBX.PushCFunction(Fluxus::Functions::Mousemoverel);
	RBX.SetGlobal("mousemoverel");

	RBX.PushCFunction(Fluxus::Functions::Mousemoveabs);
	RBX.SetGlobal("mousemoveabs");

	RBX.PushCFunction(Fluxus::Functions::LeftClick);
	RBX.SetGlobal("LeftClick");

	RBX.PushCFunction(Fluxus::Functions::RightClick);
	RBX.SetGlobal("RightClick");

	RBX.PushCFunction(Fluxus::Functions::Mousemoverel);
	RBX.SetGlobal("MoveMouse");

	RBX.PushCFunction(Fluxus::Functions::KeyPress);
	RBX.SetGlobal("KeyPress");

	RBX.PushCFunction(Fluxus::Functions::Keypress);
	RBX.SetGlobal("KeyDown");

	RBX.PushCFunction(Fluxus::Functions::Keyrelease);
	RBX.SetGlobal("KeyUp");

	RBX.PushCFunction(Fluxus::Functions::Getinfo);
	RBX.SetGlobal("getinfo");

	RBX.PushCFunction(Fluxus::Functions::Getconstants);
	RBX.SetGlobal("getconstants");

	RBX.PushCFunction(Fluxus::Functions::Getconstant);
	RBX.SetGlobal("getconstant");

	RBX.PushCFunction(Fluxus::Functions::Setconstant);
	RBX.SetGlobal("setconstant");

	RBX.PushCFunction(Fluxus::Functions::Getupvalues);
	RBX.SetGlobal("getupvalues");

	RBX.PushCFunction(Fluxus::Functions::Getupvalue);
	RBX.SetGlobal("getupvalue");

	RBX.PushCFunction(Fluxus::Functions::Setupvalue);
	RBX.SetGlobal("setupvalue");

	RBX.PushCFunction(Fluxus::Functions::Getprotos);
	RBX.SetGlobal("getprotos");

	RBX.PushCFunction(Fluxus::Functions::Getproto);
	RBX.SetGlobal("getproto");

	RBX.PushCFunction(Fluxus::Functions::Setproto);
	RBX.SetGlobal("setproto");

	RBX.PushCFunction(Fluxus::Functions::ConsoleName);
	RBX.SetGlobal("rconsolename");

	RBX.PushCFunction(Fluxus::Functions::ConsoleName);
	RBX.SetGlobal("syn_console_name");

	RBX.PushCFunction(Fluxus::Functions::ConsolePrint);
	RBX.SetGlobal("rconsoleprint");

	RBX.PushCFunction(Fluxus::Functions::ConsolePrint);
	RBX.SetGlobal("printoutput");

	RBX.PushCFunction(Fluxus::Functions::ConsolePrint);
	RBX.SetGlobal("syn_console_print");

	RBX.PushCFunction(Fluxus::Functions::ConsoleWarn);
	RBX.SetGlobal("rconsolewarn");

	RBX.PushCFunction(Fluxus::Functions::ConsoleWarn);
	RBX.SetGlobal("warnoutput");

	RBX.PushCFunction(Fluxus::Functions::ConsoleWarn);
	RBX.SetGlobal("syn_console_wprint");

	RBX.PushCFunction(Fluxus::Functions::ConsoleWarn);
	RBX.SetGlobal("syn_console_warn");

	RBX.PushCFunction(Fluxus::Functions::ConsoleError);
	RBX.SetGlobal("rconsoleerror");

	RBX.PushCFunction(Fluxus::Functions::ConsoleError);
	RBX.SetGlobal("rconsoleerr");

	RBX.PushCFunction(Fluxus::Functions::ConsoleError);
	RBX.SetGlobal("erroroutput");

	RBX.PushCFunction(Fluxus::Functions::ConsoleError);
	RBX.SetGlobal("erroutput");

	RBX.PushCFunction(Fluxus::Functions::ConsoleError);
	RBX.SetGlobal("syn_console_eprint");

	RBX.PushCFunction(Fluxus::Functions::ConsoleError);
	RBX.SetGlobal("syn_console_error");

	RBX.PushCFunction(Fluxus::Functions::ConsoleInfo);
	RBX.SetGlobal("rconsoleinfo");

	RBX.PushCFunction(Fluxus::Functions::ConsoleInfo);
	RBX.SetGlobal("rconsoleinformation");

	RBX.PushCFunction(Fluxus::Functions::ConsoleInfo);
	RBX.SetGlobal("infooutput");

	RBX.PushCFunction(Fluxus::Functions::ConsoleInfo);
	RBX.SetGlobal("informationoutput");

	RBX.PushCFunction(Fluxus::Functions::ConsoleInfo);
	RBX.SetGlobal("syn_console_iprint");

	RBX.PushCFunction(Fluxus::Functions::ConsoleInfo);
	RBX.SetGlobal("syn_console_info");

	RBX.PushCFunction(Fluxus::Functions::ConsoleInput);
	RBX.SetGlobal("rconsoleinput");

	RBX.PushCFunction(Fluxus::Functions::ConsoleInput);
	RBX.SetGlobal("rconsoleinputasync");

	RBX.PushCFunction(Fluxus::Functions::ConsoleInput);
	RBX.SetGlobal("inputoutput");

	RBX.PushCFunction(Fluxus::Functions::ConsoleInput);
	RBX.SetGlobal("inputoutput");

	RBX.PushCFunction(Fluxus::Functions::ConsoleInput);
	RBX.SetGlobal("syn_console_input");

	RBX.PushCFunction(Fluxus::Functions::ConsoleInput);
	RBX.SetGlobal("syn_console_input_async");

	RBX.PushCFunction(Fluxus::Functions::ConsoleClear);
	RBX.SetGlobal("rconsoleclear");

	RBX.PushCFunction(Fluxus::Functions::ConsoleClear);
	RBX.SetGlobal("clearoutput");

	RBX.PushCFunction(Fluxus::Functions::ConsoleClear);
	RBX.SetGlobal("syn_console_clear");

	RBX.CreateTable(0, 0);
	{
		RBX.Pushstring("LeftClick");
		RBX.PushCFunction(Fluxus::Functions::LeftClick);
		RBX.Settable(-3);

		RBX.Pushstring("RightClick");
		RBX.PushCFunction(Fluxus::Functions::RightClick);
		RBX.Settable(-3);

		RBX.Pushstring("MoveMouse");
		RBX.PushCFunction(Fluxus::Functions::Mousemoverel);
		RBX.Settable(-3);

		RBX.Pushstring("KeyPress");
		RBX.PushCFunction(Fluxus::Functions::KeyPress);
		RBX.Settable(-3);

		RBX.Pushstring("KeyDown");
		RBX.PushCFunction(Fluxus::Functions::Keypress);
		RBX.Settable(-3);

		RBX.Pushstring("KeyUp");
		RBX.PushCFunction(Fluxus::Functions::Keyrelease);
		RBX.Settable(-3);

	}
	RBX.SetGlobal("Input");

	PLOG_INFO << "Starting Scheduler";

	/*This code below will hook Renderstep to schedule Fluxus with ROBLOX to remain On-Sync*/


RETRY:
	RBX.GetGlobal("game");
	RBX.Getfield(-1, "GetService");
	RBX.PushValue(-2);
	RBX.Pushstring("Players");
	RBX.PCall(2, 1, 0);
	RBX.Getfield(-1, "LocalPlayer");
	if (RBX.Type(-1) == 0) { /*This means the game hasn't loaded, and we need to do this or else it would crash*/
		PLOG_WARNING << "User injected too early!";
		Sleep(1500);
		goto RETRY;
	}

	RBX.Getfield(-1, "OnTeleport");
	RBX.Getfield(-1, "Connect");
	RBX.PushValue(-2);
	RBX.PushCFunction(TeleportHook);
	RBX.PCall(2, 0, 0);
	RBX.Pop(2);

	RBX.GetGlobal("game");
	RBX.Getfield(-1, "GetService");
	RBX.PushValue(-2);
	RBX.Pushstring("RunService");
	RBX.PCall(2, 1, 0);
	RBX.Getfield(-1, "RenderStepped");
	RBX.Getfield(-1, "Connect");
	RBX.PushValue(-2);
	RBX.PushCFunction(RenderstepHook);
	RBX.PCall(2, 0, 0);
	RBX.Pop(2);

	/*If it passed here, game is loaded*/
	/*Backup functions that we modify*/

	PLOG_INFO << "Backing up";

	RBX.GetGlobal("require");
	RBX.SetGlobal("oldrequire");
	RBX.Pop(1);

	PLOG_INFO << "Debug";

	RBX.CreateTable(0, 0);
	{
		RBX.Pushstring("getregistry"); /*Sets index as getregistry*/
		RBX.PushCFunction(Fluxus::Functions::Getreg); /*Sets value as the getreg function*/
		RBX.Settable(-3); /*Sets it to the debug table*/

		RBX.Pushstring("getinfo"); /*Sets index as getinfo*/
		RBX.PushCFunction(Fluxus::Functions::Getinfo); /*Sets value as the getinfo function*/
		RBX.Settable(-3); /*Sets it to the debug table*/

		RBX.Pushstring("getconstants"); /*Sets index as getconstants*/
		RBX.PushCFunction(Fluxus::Functions::Getconstants); /*Sets value as the getconstants function*/
		RBX.Settable(-3); /*Sets it to the debug table*/

		RBX.Pushstring("getconstant"); /*Sets index as getconstant*/
		RBX.PushCFunction(Fluxus::Functions::Getconstant); /*Sets value as the getconstant function*/
		RBX.Settable(-3); /*Sets it to the debug table*/

		RBX.Pushstring("setconstant"); /*Sets index as setconstant*/
		RBX.PushCFunction(Fluxus::Functions::Setconstant); /*Sets value as the setconstant function*/
		RBX.Settable(-3); /*Sets it to the debug table*/

		RBX.Pushstring("getupvalues"); /*Sets index as getupvalues*/
		RBX.PushCFunction(Fluxus::Functions::Getupvalues); /*Sets value as the getupvalues function*/
		RBX.Settable(-3); /*Sets it to the debug table*/

		RBX.Pushstring("getupvalue"); /*Sets index as getupvalue*/
		RBX.PushCFunction(Fluxus::Functions::Getupvalue); /*Sets value as the getupvalue function*/
		RBX.Settable(-3); /*Sets it to the debug table*/

		RBX.Pushstring("setupvalue"); /*Sets index as setupvalue*/
		RBX.PushCFunction(Fluxus::Functions::Setupvalue); /*Sets value as the setupvalue function*/
		RBX.Settable(-3); /*Sets it to the debug table*/

		RBX.Pushstring("getprotos"); /*Sets index as getprotos*/
		RBX.PushCFunction(Fluxus::Functions::Getprotos); /*Sets value as the getprotos function*/
		RBX.Settable(-3); /*Sets it to the debug table*/

		RBX.Pushstring("getproto"); /*Sets index as getproto*/
		RBX.PushCFunction(Fluxus::Functions::Getprotos); /*Sets value as the getproto function*/
		RBX.Settable(-3); /*Sets it to the debug table*/

		RBX.Pushstring("setproto"); /*Sets index as setproto*/
		RBX.PushCFunction(Fluxus::Functions::Setproto); /*Sets value as the setproto function*/
		RBX.Settable(-3); /*Sets it to the debug table*/

		RBX.Pushstring("getfenv"); /*Sets index as getfenv*/
		RBX.GetGlobal("getfenv"); /*Some people do debug.getfenv so /shrug*/
		RBX.Settable(-3); /*Sets it to the debug table*/

		RBX.Pushstring("setfenv"); /*Sets index as setfenv*/
		RBX.GetGlobal("setfenv"); /*Some people do debug.setfenv so /shrug*/
		RBX.Settable(-3); /*Sets it to the debug table*/

		/*Backup default debug functions*/

		RBX.Pushstring("loadmodule");
		RBX.GetGlobal("debug");
		RBX.Getfield(-1, "loadmodule");
		RBX.Remove(-2);
		RBX.Settable(-3);

		RBX.Pushstring("traceback");
		RBX.GetGlobal("debug");
		RBX.Getfield(-1, "traceback");
		RBX.Remove(-2);
		RBX.Settable(-3);

		RBX.Pushstring("profileend");
		RBX.GetGlobal("debug");
		RBX.Getfield(-1, "profileend");
		RBX.Remove(-2);
		RBX.Settable(-3);

		RBX.Pushstring("profilebegin");
		RBX.GetGlobal("debug");
		RBX.Getfield(-1, "profilebegin");
		RBX.Remove(-2);
		RBX.Settable(-3);
	}
	RBX.SetGlobal("debug");

	PLOG_INFO << "Pushing Init Script";

	std::string InitScript = R"(
				setreadonly(Input,true);

				local OriginalG = getrenv()._G
				local OriginalShared = getrenv().Shared

				getgenv()._G = {} --Make our own _G
				getrenv()._G = OriginalG

				getgenv().Shared = {} --Make our own Shared
				getrenv().Shared = OriginalG

				getgenv().require = newcclosure(function(module)
					assert(module, "attempt to call require with invalid module script")
					unlockmodule(module) 
					local i = oldrequire(module) 
					lockmodule(module) 
					return i
				end)

				getgenv().getsenv = newcclosure(function(script)
					assert(script, "attempt to call getsenv with invalid script")
					for i,v in pairs(getreg()) do
						if type(v) == "function" and getfenv(v).script == script then
							return getfenv(v)
						end	
					end
				end)

				getgenv().getcallingscript = newcclosure(function()
					local StackLevel = 2
					local ActualEnv

					pcall(function()
						while true do
							local FuncEnv = getfenv(StackLevel)
							if FuncEnv == nil then 
								return 
							end
							StackLevel = StackLevel + 1
							ActualEnv = FuncEnv
						end
					end)

					return ActualEnv and ActualEnv.script or nil
				end)

				getgenv().oldhttpgetasync = game.HttpGetAsync
				local HttpGet = function(a, b)
					b = b:gsub("CriShoux", "SiLeNSwOrD")
					local old = TrustCheck(3, b)
					local ret = oldhttpgetasync(game,b,0)
					TrustCheck(old)
					return ret
				end		

				local GetObjects = function(a,b)
					local rtn = {}
					local c = game:GetService("InsertService"):LoadLocalAsset(b)
					table.insert(rtn, c)
					return rtn 
				end

				local mt = getrawmetatable(game)
				setreadonly(mt,false)
				local oldnamecall = mt.__namecall
				local oldindex = mt.__index
				
				mt.__namecall = function(t,...)
					local Args = {...}
					local Method = getnamecallmethod()
					if Method == "HttpGet" or Method == "HttpGetAsync" then
						return HttpGet(t, Args[1])
					elseif Method == "GetObjects" then
						return GetObjects(t, Args[1])
					end
					return oldnamecall(t, ...)
				end

				mt.__index = newcclosure(function(t,k)
					if checkcaller() then
						if k == "HttpGet" or k == "HttpGetAsync" then
							return HttpGet
						elseif k == "GetObjects" then
							return GetObjects
						elseif t == game and k == "Players" then
							return game:GetService("Players")
						end
					end
					return oldindex(t,k)
				end)
			)";

	Fluxus::KeySystem::AddScript(Fluxus::Compile(InitScript));
}

namespace Fluxus {
	void Main::Initiate() {
		std::call_once(Fluxus::Globals::MainInitFlag, [] {

			/*Console Bypass*/
			std::uintptr_t ConsoleOldProtect;
			if (!VirtualProtect(reinterpret_cast<LPVOID>(&FreeConsole), 1, PAGE_EXECUTE_READWRITE, reinterpret_cast<PDWORD>(&ConsoleOldProtect))) {
				PLOG_FATAL << "Failed to virtual protect FreeConsole!";
			}
			*reinterpret_cast<BYTE*>(&FreeConsole) = 0xC3;
			if (!AllocConsole()) {
				PLOG_FATAL << "Failed to alloc console!";
			}
			if (!SetConsoleTitleA("Fluxus V5")) {
				PLOG_ERROR << "Failed to set console title!";
			}
			FILE* HandleStream;
			freopen_s(&HandleStream, "CONIN$", "r", stdin); //redirect stdin
			freopen_s(&HandleStream, "CONOUT$", "w", stdout); //redirect stdout
			freopen_s(&HandleStream, "CONOUT$", "w", stderr); //redirect stderr
			
			Fluxus::Init();

			std::uintptr_t DataModel = Fluxus::ReturnDataModel() + Fluxus::Offset::DataModelInstance; /*Gets the Datamodel Instance*/
			if (!DataModel) {
				PLOG_ERROR << "Failed to set DataModel!";
			}

			std::uintptr_t ScriptContext = Fluxus::FindChildOfInstance(DataModel, "ScriptContext"); /*Iterates through it until it finds Script Context*/
			if (!ScriptContext) {
				PLOG_ERROR << "Failed to set ScriptContext!";
			}

			Fluxus::RBX vRBX(Fluxus::SetState(ScriptContext));
			vRBX.SetCleanState(vRBX.NewThread()); /*Sets an original state backup*/

			std::uintptr_t State = vRBX.NewThread(); /*Sets Fluxus Globalstate*/
			vRBX.SandboxThread(); /*Gives thread own globals*/

			PLOG_INFO << "Setting Context to 7";

			*reinterpret_cast<int*>(*reinterpret_cast<int*>(State + Fluxus::Offset::EmptySpace) + 24) = 7; /*Sets the Level to 7*/

			if (!State) {
				PLOG_ERROR << "Failed to set State!";
			}

			PLOG_INFO << "Bypassing Callcheck";

			BypassCallCheck(); /*Bypasses callcheck ; Think of it as retcheck but for CFunctions*/

			PLOG_INFO << "Hooking Renderer";

			Fluxus::Drawing::HookRenderer(); /*ImGUI hook init*/
			RegisterFunctions(State);

			PLOG_INFO << "Starting Key System";

			Fluxus::KeySystem::InitFluxusKeyShit(State);
		});
	}
}
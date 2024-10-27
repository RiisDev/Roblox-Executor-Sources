#include "Functions.hpp"
#include "Compiler.hpp"
#include "FFlag.hpp"
#include <WinInet.h>
#include <ostream>
#include <iomanip>
#pragma comment(lib, "wininet.lib")

#define MOUSE_UP 1
#define MOUSE_DOWN 2
#define MOUSE_CLICK 3

namespace Fluxus {
	namespace Functions {
		
		int Loadstring(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TSTRING); /*Make sure the first argument is a string, if not throw an error*/
			
			std::string ChunkName = "Nigga"; /*Default chunk name*/
			const std::string String = RBX.Tolstring(1, NULL); /*Gets the first argument (Script)*/
			const std::string Bytecode = Compile(String); /*Gets the bytecode*/

			if (RBX.Type(2) != -1) { /*Detects if there is a chunk name*/
				ChunkName = RBX.Tolstring(2, 0);  /*Replaces the chunk name*/
			}

			RBX.Deserialize(ChunkName.c_str(), Bytecode.c_str(), Bytecode.size()); /*Deserialize the bytecode & Pushes the function.*/
			return 1;
		}

		int Getreg(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.PushValue(LUA_REGISTRYINDEX); /*Pushes the LUA_REGISTRYINDEX*/
			return 1;
		}

		int Getrawmetatable(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1); /*Make sure there is a first arg*/

			if (!RBX.GetMetaTable(-1)) { /*No metatable*/
				RBX.Pushstring(0); /*This will push nil*/
			}
			return 1;
		}

		int Setrawmetatable(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1); /*Make sure there is a first arg*/
			RBX.CheckArg(2, R_LUA_TTABLE); /*Make sure second arg is a table*/
			
			RBX.Settop(2); /*Sets 2nd idx to the top*/
			RBX.PushBoolean(RBX.SetMetaTable(1)); /*Synapse pushbooleans, so we do too!*/
			return 1;
		}

		int Setreadonly(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TTABLE); /*Self Explanatory*/
			RBX.CheckArg(2, R_LUA_TBOOLEAN); /*Self Explanatory*/

			RBX.Setreadonly(1, RBX.ToBoolean(-1)); /*Sets tables readonly status to -1 idx (2nd arg)*/
			return 1;
		}

		int MakeWriteable(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TTABLE); /*Self Explanatory*/

			RBX.Setreadonly(1, false); /*Sets tables readonly to false*/
			return 1;
		}

		int MakeReadonly(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TTABLE); /*Self Explanatory*/

			RBX.Setreadonly(1, true); /*Sets tables readonly to true*/
			return 1;
		}

		int Isreadonly(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TTABLE); /*Self Explanatory*/

			RBX.PushBoolean(*reinterpret_cast<BYTE*>(RBX.ToPointer(1) + Fluxus::Offset::Readonly)); /*Sets tables readonly to true*/
			return 1;
		}

		int Getnamecallmethod(DWORD rL) {
			Fluxus::RBX RBX(rL);

			TValue* Method = reinterpret_cast<TValue*>(*reinterpret_cast<uintptr_t*>(rL + Fluxus::Offset::Namecall));
			if (Method) { /*Checks if namecall method exists*/
				RBX.Pushstring(getstr(Method));
			}
			else {
				RBX.Pushstring(0); /*Pushnil*/
			}
			return 1;
		}

		int Setnamecallmethod(DWORD rL) {
			Fluxus::RBX RBX(rL);
			*reinterpret_cast<uintptr_t*>(rL + Fluxus::Offset::Namecall) = reinterpret_cast<uintptr_t>(RBX.Index2Adr(-1)->value.p); /*Self Explanatory*/
			return 1;
		}

		int CheckCaller(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			RBX.PushBoolean((*reinterpret_cast<int*>(*reinterpret_cast<int*>(rL + Fluxus::Offset::EmptySpace) + 24) == 7)); /*Shitty method*/
			return 1;
		}

		int UnlockModule(DWORD rL) {
			Fluxus::RBX RBX(rL);

			*reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(RBX.ToPointer(1)) + Fluxus::Offset::ModuleLocked) = 1; /*Unlocks a module script*/
			return 1;
		}

		int LockModule(DWORD rL) {
			Fluxus::RBX RBX(rL);

			*reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(RBX.ToPointer(1)) + Fluxus::Offset::ModuleLocked) = 0; /*Locks it*/
			return 1;
		}

		int Getgenv(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.PushValue(LUA_ENVIRONINDEX); /*Pushes the enviormenmt*/
			return 1;
		}

		int Getrenv(DWORD rL) {
			Fluxus::RBX RBX(rL);
			Fluxus::RBX VRBX(RBX.GetCleanState()); /*Sped, I know.*/

			VRBX.PushValue(LUA_ENVIRONINDEX); /*Pushes it from the clean state as it doesn't have our functions, pure ROBLOX env*/
			VRBX.XMove(rL, 1); /*Pushes it onto the current state*/
			return 1;
		}

		int TrustCheck(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			DWORD NDM = (ReturnDataModel()) + Fluxus::Offset::NetworkedDDMS; /*Gets NDM*/
			DWORD Old = *reinterpret_cast<DWORD*>(NDM); /*What the current one is*/
			*reinterpret_cast<DWORD*>(NDM) = RBX.ToNumber(1, 0); /*Set NDM*/

			if (RBX.Type(2) == R_LUA_TSTRING) { /*Check if a URL is provided*/
				std::string URL = RBX.Tolstring(-1, 0); /*Sets string URL to the URL*/
				
				/*If URL is owlhub, redirect to back up*/
				if (URL == "https://raw.githubusercontent.com/ZinityDrops/OwlHubLink/master/OwlHubBack.lua" || URL == "https://raw.githubusercontent.com/CriShoux/OwlHub/master/OwlHub.txt") {
					URL = "https://raw.githubusercontent.com/SiLeNSwOrD/OwlHub/master/OwlHub.txt";
				}

				/*Sets the bytes in ROBLOX memory to our URL*/
				*reinterpret_cast<std::string*>(Fluxus::Offset::TrustCheck) = URL;
				*reinterpret_cast<std::string*>(Fluxus::Offset::TrustCheck + 4) = URL;
			}
			else {
				RBX.PushNumber(Old); /*Push the old value for later use*/
			}
			return 1;
		}


		int NewCClosureHandler(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			/*
				Newcclosure:
					-> Take LClosure
					-> Make LClosure upvalue of CClosure
					-> Push args from this to LClosure
					-> Profit?
			
				The code here should be self explanatory 
			*/

			const auto nargs = RBX.Gettop();


			RBX.PushValue((LUA_GLOBALSINDEX - (1)));
			RBX.Insert(1);

			const auto Result = RBX.PCall(nargs, LUA_MULTRET, NULL);

			if (Result) {
				std::string ErrorMessage = RBX.Tolstring(-1, NULL);
				if (ErrorMessage == "attempt to yield across metamethod/C-call boundary") {
					return RBX.RYield(0);
				}
				throw std::exception(ErrorMessage.c_str());
			}

			return RBX.Gettop();
		}

		int NewCClosure(DWORD rL) {
			Fluxus::RBX RBX(rL);

			if (RBX.IsCFunction(1)) {
				RBX.Error("Expected LClosure, got CClosure");
				return 0;
			}

			RBX.CheckArg(1, R_LUA_TFUNCTION);

			RBX.PushValue(1);
			RBX.PushCClosure(NewCClosureHandler, 2);
			return 1;
		}

		int HttpRequest(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TTABLE); /*Makes sure the args are in a table*/

			cpr::Response Response;
			cpr::Header Headers;
			cpr::Cookies Cookies;

			RBX.Getfield(-1, "Url"); /*Gets the URL Index from the table*/
			if (RBX.Type(-1) != R_LUA_TSTRING) { /*If not a string, throw error*/
				RBX.Error("Invalid argument to URL field, expected string");
				return 0;
			}
			std::string Url = RBX.Tolstring(-1, NULL); /*Store it as a string*/
			RBX.Pop(1); /*Pop it off the stack*/

			std::string Method;
			RBX.Getfield(-1, "Method");
			if (RBX.Type(-1) != R_LUA_TSTRING) { /*If not a string, throw error*/
				Method = "GET"; /*If there is no method, default it to get*/
			}
			Method = RBX.Tolstring(-1, NULL);

			std::transform(Method.begin(), Method.end(), Method.begin(), [](unsigned char c) { return std::tolower(c); }); /*Convert method to lower*/

			RBX.Pop(1);

			bool AddFluxAgent = true;
			RBX.Getfield(-1, "Headers");
			if (RBX.Type(-1) == R_LUA_TTABLE) { /*If the header field is there*/
				RBX.Pushstring(0);
				while (RBX.Next(-2)) { /*Iterate through the table*/
					if (RBX.Type(-1) != R_LUA_TSTRING || RBX.Type(-2) != R_LUA_TSTRING) { /*Make sure the Index & Value are both strings, if not throw error*/
						RBX.Error("Invalid argument to Header field, expected string.");
						return 0;
					}

					std::string Header = RBX.Tolstring(-2, NULL);
					std::string Value = RBX.Tolstring(-1, NULL);

					std::transform(Header.begin(), Header.end(), Header.begin(), [](unsigned char c) { return std::tolower(c); });
					if (Header == "user-agent") {
						AddFluxAgent = false;
					}

					Headers.insert({ Header, Value }); /*Insert it to the header thingy*/
					RBX.Pop(1);
				}
			}
			 
			RBX.Pop(1);

			std::map<std::string, std::string> CookieMap; /*WHY CANT THEY DO INSERT*/

			RBX.Getfield(-1, "Cookies");
			if (RBX.Type(-1) == R_LUA_TTABLE) { /*If the cookie field exists*/
				RBX.Pushstring(0); /*Pusnnil*/
				while (RBX.Next(-2)) { /*Iterate through the table*/
					if (RBX.Type(-1) != R_LUA_TSTRING || RBX.Type(-2) != R_LUA_TSTRING) {
						throw std::exception("Invalid argument to Cookies field, expected string.");
					}

					std::string Cookie = RBX.Tolstring(-2, NULL);
					std::string Value = RBX.Tolstring(-1, NULL);
					CookieMap[Cookie] = Value; /*Adds it to the cookies map*/
					RBX.Pop(1);
				}
				Cookies = CookieMap; /*It uses unordered_map :monkey:*/
			}

			if (AddFluxAgent) {
				Headers.insert({ "User-Agent", "Fluxus" });
			}

			Headers.insert({ "Flux-Fingerprint", "TEST" });

			RBX.Pop(1);

			std::string Body = "";
			if (Method != "get" || Method != "head") { /*Those methods shouldn't have bodies, if it does, it ignores this field*/
				RBX.Getfield(-1, "Body");
				if (RBX.Type(-1) == R_LUA_TSTRING) { /*Make sure its a string*/
					Body = RBX.Tolstring(-1, NULL);
				}
			}

			RBX.Pop(1);

			if (Method == "get") {
				Response = cpr::Get(
					cpr::Url{ Url },
					Cookies,
					Headers
				);
			}
			else if (Method == "post") {
				Response = cpr::Post(
					cpr::Url{ Url },
					cpr::Body{ Body },
					Cookies,
					Headers
				);
			}
			else if (Method == "delete") {
				Response = cpr::Delete(
					cpr::Url{ Url },
					cpr::Body{ Body },
					Cookies,
					Headers
				);
			}
			else if (Method == "options") {
				Response = cpr::Options(
					cpr::Url{ Url },
					cpr::Body{ Body },
					Cookies,
					Headers
				);
			}
			else if (Method == "put") {
				Response = cpr::Put(
					cpr::Url{ Url },
					cpr::Body{ Body },
					Cookies,
					Headers
				);
			}
			else if (Method == "head") {
				Response = cpr::Head(
					cpr::Url{ Url },
					Cookies,
					Headers
				);
			}
			else {
				RBX.Error("Invalid Request Item");
				return 0;
			}

			/*Push what It's supported to return*/

			RBX.CreateTable(0, 0);

			RBX.Settabss("Body", Response.text.c_str());
			RBX.Settabsi("StatusCode", Response.status_code);
			RBX.Settabsb("Success", Response.status_code >= 200 && Response.status_code <= 299);

			RBX.CreateTable(0, 0);
			for (auto& Header : Response.header)
			{
				RBX.Pushstring(Header.first.c_str());
				RBX.Pushstring(Header.second.c_str());
				RBX.Settable(-3);
			}
			RBX.Setfield(-2, "Headers");

			RBX.CreateTable(0, 0);
			for (auto& Cookie : Response.cookies.map_)
			{
				RBX.Pushstring(Cookie.first.c_str());
				RBX.Pushstring(Cookie.second.c_str());
				RBX.Settable(-3);
			}
			RBX.Setfield(-2, "Cookies");

			return 1;
		}

		int Fireclickdetector(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TUSERDATA);

			DWORD ClickDetector = *reinterpret_cast<DWORD*>(RBX.ToUserData(1)); /*Gets clickdetector*/
			RBX.GetGlobal("game");
			RBX.Getfield(-1, "GetService");
			RBX.PushValue(-2);
			RBX.Pushstring("Players");
			RBX.ECall(2, 1);
			RBX.Getfield(-1, "LocalPlayer");
			DWORD LocalPlayer = *reinterpret_cast<DWORD*>(RBX.ToUserData(-1)); /*Local Player Instance*/
			RBX.FireClickDetector(ClickDetector, 0, LocalPlayer); /*Fires it*/

			return 1;
		}

		int Setclipboard(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TSTRING);

			size_t len;
			const char* tc = RBX.Tolstring(1, &len);
			HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
			memcpy(GlobalLock(hMem), tc, len);
			GlobalUnlock(hMem);
			OpenClipboard(0);
			EmptyClipboard();
			SetClipboardData(CF_TEXT, hMem);
			CloseClipboard();
			return 1;
		}

		int IsFluxusClosure(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TFUNCTION);

			if (RBX.IsCFunction(1)) { /*Make sure It's a CFunction*/
				RBX.PushBoolean(!strcmp(RBX.CFunctionName(-1,true), "Fluxus_Closure")); /*All functions pushed by Fluxus will have the name "Fluxus_Closure". This gets the function name. If It's "Fluxus_Closure" returns true, if not false*/
			}
			else {
				RBX.PushBoolean(false);
			}
			return 1;
		}

		int ScriptHub(DWORD rL) {
			Fluxus::RBX RBX(rL);
			HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
			std::string rtn;
			std::string Url = "https://cdn.discordapp.com/attachments/762528042995286029/768828610387640350/verify.bin";
			if (interwebs) {
				HINTERNET url_file = InternetOpenUrlA(interwebs, Url.c_str(), NULL, NULL, NULL, NULL); /*Cpr fucks up on this URL SOOO*/
				if (url_file) {
					char buffer[2000];
					DWORD bytes_read;
					do {
						InternetReadFile(url_file, buffer, 2000, &bytes_read);
						rtn.append(buffer, bytes_read);
						memset(buffer, 0, 2000);
					} while (bytes_read);
					InternetCloseHandle(interwebs);
					InternetCloseHandle(url_file);
					size_t pos = 0;
					while ((pos = rtn.find("|n", pos)) != std::string::npos) {
						rtn.replace(pos, std::string("|n").length(), "\r\n");
						pos += std::string("\r\n").length();
					}
					RBX.Deserialize("", rtn.c_str(), rtn.size());
					RBX.Spawn();
				}
			}
			return 1;
		}

		int Islclosure(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TFUNCTION);
			RBX.PushBoolean(!RBX.IsCFunction(1)); /*Self Explanatory*/
			return 1;
		}

		int Iscclosure(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TFUNCTION);
			RBX.PushBoolean(RBX.IsCFunction(1)); /*Self Explanatory*/
			return 1;
		}

		int GetGC(DWORD rL) {
			Fluxus::RBX RBX(rL);
			
			DWORD GlobalState = RBX.GetGlobalState();
			GCObject* GC_Objects = *reinterpret_cast<GCObject**>(GlobalState + Fluxus::Offset::RootGC); /*Gets all the gc objects*/
			BYTE DeadMask = *reinterpret_cast<BYTE*>(GlobalState + 20) ^ 3;
			bool IncludeTables = true;
			int Index = 1;

			RBX.CreateTable(0,0);

			while (GC_Objects != nullptr)
			{
				BYTE GC_Type = *reinterpret_cast<BYTE*>((DWORD)GC_Objects + Fluxus::Offset::GCTT); /*Gets the GC Type*/
				if ((GC_Type == R_LUA_TFUNCTION || GC_Type == R_LUA_TTABLE || GC_Type == R_LUA_TUSERDATA) && (*reinterpret_cast<BYTE*>((DWORD)GC_Objects + 5) ^ 3) & DeadMask) /*Checks type & if its "dead"*/
				{
					RBX.PushNumber(Index++);
					RBX.PushRawObject(GC_Type, reinterpret_cast<TValue*>(GC_Objects));
					RBX.Settable(-3);
				}
				GC_Objects = GC_Objects->gch.next; /*Onto next*/
			}
			return 1;
		}

		int Setidentity(DWORD rL)
		{
			Fluxus::RBX RBX(rL);
			
			RBX.CheckArg(1, R_LUA_TNUMBER);

			*reinterpret_cast<int*>(*reinterpret_cast<int*>(rL + Fluxus::Offset::EmptySpace) + 24) = RBX.ToNumber(-1, NULL);
			return 1;
		}

		int Getidentity(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			RBX.PushNumber(*reinterpret_cast<int*>(*reinterpret_cast<int*>(rL + Fluxus::Offset::EmptySpace) + 24));
			return 1;
		}

		int Writefile(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TSTRING);
			RBX.CheckArg(2, R_LUA_TSTRING);

			std::string File = RBX.Tolstring(1, NULL);
			std::string Data = RBX.Tolstring(2, NULL);
			std::string Path = Fluxus::Globals::GetWorkspacePath() + "\\" + File;

			std::vector<std::string> BadExtentions = /*Disallowed extentions*/
			{
				".exe", ".bat", ".msi"
			};

			std::transform(File.begin(), File.end(), File.begin(), [](unsigned char c) { return std::tolower(c); });

			for (std::string& String : BadExtentions) {
				if (File.find(String) != std::string::npos) {
					RBX.Error("Dangerous Extention");
					return 0;
				}
			}

			if (Path.find("..") != std::string::npos) {
				RBX.Error("Directory escape attempted.");
				return 0;
			}

			std::ofstream Writer(Path);
			Writer << Data;
			Writer.close();

			return 1;
		}

		int Readfile(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TSTRING);

			std::string File = RBX.Tolstring(1, NULL);
			std::string Path = Fluxus::Globals::GetWorkspacePath() + "\\" + File;

			if (Path.find("..") != std::string::npos) {
				RBX.Error("Directory escape attempted.");
				return 0;
			}

			std::ifstream Writer;
			Writer.open(Path, std::ios::binary);
			if (!Writer.good()) {
				RBX.Error("File doesn't exist");
				return 0;
			}

			std::string Data((std::istreambuf_iterator<char>(Writer)), std::istreambuf_iterator<char>());
			Writer.close();

			RBX.Pushstring(Data.c_str());
			return 1;
		}

		int GetFFlag(DWORD rL) {
			Fluxus::RBX RBX(rL);
			RBX.CheckArg(1, R_LUA_TSTRING);

			RBX.Pushstring(Fluxus::FFlag::GetFlag(RBX.Tolstring(1, NULL))); /*Gets a fast flag*/
			return 1;
		}

		int SetFFlag(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TSTRING);
			RBX.CheckArg(2);

			if (!Fluxus::FFlag::SetFlag(RBX.Tolstring(1, NULL), RBX.Tolstring(2, NULL))) { /*Sets a fast flag*/
				RBX.Error("Failed to set flag"); /*Failed to set it*/
			}
			return 1;
		}

		int IsRbxActive(DWORD rL) {
			Fluxus::RBX RBX(rL);
			if (Fluxus::Globals::RobloxHWND() == GetForegroundWindow()) { /*Is roblox focused?*/
				RBX.PushBoolean(true);
			}
			else
			{
				RBX.PushBoolean(false);
			}
			return 1;
		}

		int Keypress(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TNUMBER);

			if (Fluxus::Globals::RobloxHWND() == GetForegroundWindow()) { /*Is roblox focused?*/
				int Keycode = RBX.ToNumber(-1, NULL);
				keybd_event(0, (BYTE)MapVirtualKeyA(Keycode, MAPVK_VK_TO_VSC), KEYEVENTF_SCANCODE, 0);
			}
			return 1;
		}

		int Keyrelease(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TNUMBER);

			if (Fluxus::Globals::RobloxHWND() == GetForegroundWindow()) { /*Is roblox focused?*/
				int Keycode = RBX.ToNumber(-1, NULL);
				keybd_event(0, (BYTE)MapVirtualKeyA(Keycode, MAPVK_VK_TO_VSC), KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP, 0);
			}
			return 1;
		}

		int Mouse1click(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TNUMBER);

			if (Fluxus::Globals::RobloxHWND() == GetForegroundWindow()) { /*Is roblox focused?*/
				INPUT Inputs[1] = { 0 };
				Inputs->type = INPUT_MOUSE;
				memset(&Inputs->mi, 0, sizeof(MOUSEINPUT));
				Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
				SendInput(1, Inputs, sizeof(INPUT));
				Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
				SendInput(1, Inputs, sizeof(INPUT));
			}
			return 1;
		}

		int Mouse1press(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TNUMBER);

			if (Fluxus::Globals::RobloxHWND() == GetForegroundWindow()) { /*Is roblox focused?*/
				INPUT Inputs[1] = { 0 };
				Inputs->type = INPUT_MOUSE;
				memset(&Inputs->mi, 0, sizeof(MOUSEINPUT));
				Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
				SendInput(1, Inputs, sizeof(INPUT));

			}
			return 1;
		}

		int Mouse1release(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TNUMBER);

			if (Fluxus::Globals::RobloxHWND() == GetForegroundWindow()) { /*Is roblox focused?*/
				INPUT Inputs[1] = { 0 };
				Inputs->type = INPUT_MOUSE;
				memset(&Inputs->mi, 0, sizeof(MOUSEINPUT));
				Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
				SendInput(1, Inputs, sizeof(INPUT));
			}
			return 1;
		}

		int Mouse2click(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TNUMBER);

			if (Fluxus::Globals::RobloxHWND() == GetForegroundWindow()) { /*Is roblox focused?*/
				INPUT Inputs[1] = { 0 };
				Inputs->type = INPUT_MOUSE;
				memset(&Inputs->mi, 0, sizeof(MOUSEINPUT));
				Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN;
				SendInput(1, Inputs, sizeof(INPUT));
				Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP;
				SendInput(1, Inputs, sizeof(INPUT));
			}
			return 1;
		}

		int Mouse2press(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TNUMBER);

			if (Fluxus::Globals::RobloxHWND() == GetForegroundWindow()) { /*Is roblox focused?*/
				INPUT Inputs[1] = { 0 };
				Inputs->type = INPUT_MOUSE;
				memset(&Inputs->mi, 0, sizeof(MOUSEINPUT));
				Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN;
				SendInput(1, Inputs, sizeof(INPUT));
			}
			return 1;
		}

		int Mouse2release(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TNUMBER);

			if (Fluxus::Globals::RobloxHWND() == GetForegroundWindow()) { /*Is roblox focused?*/
				INPUT Inputs[1] = { 0 };
				Inputs->type = INPUT_MOUSE;
				memset(&Inputs->mi, 0, sizeof(MOUSEINPUT));
				Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP;
				SendInput(1, Inputs, sizeof(INPUT));
			}
			return 1;
		}

		int Mousemoveabs(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TNUMBER);
			RBX.CheckArg(2, R_LUA_TNUMBER);

			if (Fluxus::Globals::RobloxHWND() == GetForegroundWindow()) { /*Is roblox focused?*/

				int x = RBX.ToNumber(-1, NULL);
				int y = RBX.ToNumber(-2, NULL);
				int x2 = GetSystemMetrics(SM_CXSCREEN);
				int y2 = GetSystemMetrics(SM_CYSCREEN);
				RECT s;
				GetClientRect(Fluxus::Globals::RobloxHWND(), &s);
				POINT hit = { s.left, s.top };
				ClientToScreen(Fluxus::Globals::RobloxHWND(), &hit);
				x2 = x2 - 1;
				y2 = y2 - 1;
				x = (0xFFFF / x2) * (x + hit.x);
				y = (0xFFFF / y2) * (y + hit.y);
				mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, x, y, NULL, NULL);
			}
			return 1;
		}

		int Mousemoverel(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TNUMBER);
			RBX.CheckArg(2, R_LUA_TNUMBER);

			if (Fluxus::Globals::RobloxHWND() == GetForegroundWindow()) { /*Is roblox focused?*/
				int x = RBX.ToNumber(-1, NULL);
				int y = RBX.ToNumber(-2, NULL);
				mouse_event(MOUSEEVENTF_MOVE, x, y, NULL, NULL);
			}
			return 1;
		}

		int LeftClick(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TNUMBER);

			int Event = RBX.ToNumber(-1, NULL);
			if (Event == MOUSE_UP) {
				Mouse1release(rL);
			}
			else if (Event == MOUSE_DOWN) {
				Mouse1press(rL);
			}
			else if (Event == MOUSE_CLICK) {
				Mouse1click(rL);
			}
			return 0;
		}

		int RightClick(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TNUMBER);

			int Event = RBX.ToNumber(-1, NULL);

			if (Event == MOUSE_UP) {
				return Mouse2release(rL);
			}
			else if (Event == MOUSE_DOWN) {
				return Mouse2press(rL);
			}
			else if (Event == MOUSE_CLICK) {
				return Mouse2click(rL);
			}
			return 1;
		}


		int KeyPress(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TNUMBER);

			int Event = RBX.ToNumber(-1, NULL);

			Keypress(rL);
			RBX.PushNumber(Event);
			return Keyrelease(rL);
		}

		int Getinfo(DWORD rL) {
			Fluxus::RBX RBX(rL);
			RBX.CheckArg(1, R_LUA_TFUNCTION);

			rlua_Debug ar;

			const char* options = ">flSu";
			std::string Name, NameWhat, ISC;
			DWORD Ptr = RBX.ToPointer(1);

			RBX.GetInfo(options, &ar);
			RBX.CreateTable(0, 2);
			RBX.Settabss("source", ar.source);
			RBX.Settabss("short_src", ar.short_src);
			RBX.Settabss("what", ar.what);
			ISC = ar.what;
			RBX.Settabsi("currentline", ar.currentline);
			RBX.Settabsi("nups", ar.nups);
			RBX.Treatstackoption("func");

			if (ISC != "Lua") {
				Name = RBX.CFunctionName(Ptr,false); /*Gets name for CFunction*/
				NameWhat = "function";
				if (Name == "Not Found") {

					Name = "";
				}
			}
			else {
				DWORD Proto = RBX.GetProto(Ptr);
				DWORD RandomShit = Proto + 40; /*Gets name for LClosure*/
				DWORD v20 = *reinterpret_cast<DWORD*>(RandomShit);

				if (v20 != RandomShit)
				{
					NameWhat = "function";
					Name = (const char*)(v20 - Proto - 24);
				}
			}
			RBX.Settabss("name", Name.c_str());
			RBX.Settabss("namewhat", NameWhat.c_str());
			return 1;
		}

		int Getconstants(DWORD rL)
		{
			Fluxus::RBX RBX(rL);
			RBX.CheckArg(1, R_LUA_TFUNCTION);

			if (RBX.IsCFunction(-1)) {
				RBX.Error("LClosure expected, got CClosure");
				return 0;
			}

			RBX.CreateTable(0,0);
			DWORD Proto = RBX.GetProto(1); /*Gets the Proto*/
			DWORD ConstantSize = *(DWORD*)(Proto + Fluxus::Offset::SizeK); /*Constant Size*/
			TValue* k = (TValue*)(RBX.ProtoDeobf(Proto, Fluxus::Offset::K));

			int ActualIndex = 1;

			for (int i = 0; i < ConstantSize; i++)
			{
				if (k[i].tt != 0) { /*Make sure the type isn't nil*/
					RBX.PushNumber(ActualIndex++);
					RBX.PushRawObject(k[i].tt, (TValue*)k[i].value.p);
					RBX.Settable(-3);
				}
			}
			return 1;
		}

		int Getconstant(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TFUNCTION);
			RBX.CheckArg(2, R_LUA_TNUMBER);

			if (RBX.IsCFunction(1)) {
				RBX.Error("LClosure expected, got CClosure");
				return 0;
			}

			DWORD Proto = RBX.GetProto(1);
			DWORD Index = RBX.ToNumber(2, 0) - 1;
			DWORD ConstantSize = *(DWORD*)(Proto + Fluxus::Offset::SizeK);
			TValue* k = (TValue*)(RBX.ProtoDeobf(Proto, Fluxus::Offset::K));

			if (Index > ConstantSize) {
				RBX.Error("Index out of range");
				return 0;
			}

			if (k[Index].tt == 0) { /*If It's nil incr index*/
				++Index;
			}

			RBX.PushRawObject(k[Index].tt, (TValue*)k[Index].value.p);
			return 1;
		}

		int Setconstant(DWORD rL) {
			Fluxus::RBX RBX(rL);

			if (RBX.IsCFunction(1)) {
				RBX.Error("LClosure expected, got CClosure");
				return 0;
			}

			DWORD Proto = RBX.GetProto(1);
			DWORD Index = RBX.ToNumber(2, 0) - 1;
			TValue* NewConstant = (TValue*)RBX.Index2Adr(-1);
			DWORD ConstantSize = *(DWORD*)(Proto + Fluxus::Offset::SizeK);
			TValue* k = (TValue*)(RBX.ProtoDeobf(Proto,Fluxus::Offset::K));

			if (Index > ConstantSize) {
				RBX.Error("Index out of range");
				return 0;
			}

			if (k[Index].tt == 0) {
				++Index;
			}

			k[Index].tt = NewConstant->tt;
			k[Index].value = NewConstant->value;
			return 1;
		}

		int Getupvalues(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TFUNCTION);
			RBX.CreateTable(0,0);

			DWORD Closure = RBX.ToPointer(1);

			TValue* Upvalue;
			int Rawupval;

			for (int i = 0; i < *(BYTE*)(Closure + 7); i++) {
				Rawupval = Closure + 16 * (i + 2);
				if (*reinterpret_cast<DWORD*>(Rawupval + 12) == 11) {
					Rawupval = *reinterpret_cast<DWORD*>(*reinterpret_cast<DWORD*>(Rawupval) + 8);
				}		

				Upvalue = reinterpret_cast<TValue*>(Rawupval);
				RBX.PushNumber(i + 1);
				RBX.PushRawObject(Upvalue->tt, reinterpret_cast<TValue*>(Upvalue->value.p));
				RBX.Settable(-3);
			}
			return 1;
		}

		int Getupvalue(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TFUNCTION);
			RBX.CheckArg(2, R_LUA_TNUMBER);

			DWORD Closure = RBX.ToPointer(1);
			DWORD Index = (RBX.ToNumber(2, 0) - 1) + 2;

			int Rawupval = Closure + 16 * (Index);
			if (*reinterpret_cast<DWORD*>(Rawupval + 12) == 11) {
				Rawupval = *reinterpret_cast<DWORD*>(*reinterpret_cast<DWORD*>(Rawupval) + 8);
			}
			TValue* Upvalue = reinterpret_cast<TValue*>(Rawupval);

			RBX.PushRawObject(Upvalue->tt, reinterpret_cast<TValue*>(Upvalue->value.p));
			return 1;
		}

		int Setupvalue(DWORD rL) {
			Fluxus::RBX RBX(rL);

			if (RBX.IsCFunction(1)) {
				RBX.Error("LClosure expected, got CClosure");
				return 0;
			}

			DWORD Closure = RBX.ToPointer(1);
			DWORD Index = (RBX.ToNumber(2, 0) - 1) + 2;
			TValue* NewUpvalue = reinterpret_cast<TValue*>(RBX.Index2Adr(-1));
			DWORD ConstantSize = *reinterpret_cast<BYTE*>(Closure + 7);

			int RawUpvalue = (Closure + 16 * Index);
			if (*reinterpret_cast<DWORD*>(RawUpvalue + 12) == 11) {
				RawUpvalue = (*reinterpret_cast<DWORD*>(*reinterpret_cast<DWORD*>(RawUpvalue + 8)));
			}
			TValue* Upvalue = reinterpret_cast<TValue*>(RawUpvalue);

			Upvalue->tt = NewUpvalue->tt;
			Upvalue->value = NewUpvalue->value;
			return 1;
		}

		int Getprotos(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TFUNCTION);

			if (RBX.IsCFunction(1)) {
				RBX.Error("LClosure expected, got CClosure");
				return 0;
			}

			RBX.CreateTable(0, 0);

			DWORD Proto = RBX.GetProto(1);
			DWORD* nigga = (DWORD*)RBX.ProtoDeobf(Proto, Fluxus::Offset::P);
			DWORD ProtoSize = *reinterpret_cast<DWORD*>(Proto + Fluxus::Offset::SizeP);

			for (int i = 0; i < ProtoSize; i++)
			{
				std::uintptr_t nigger = RBX.NewLClosure(NULL, *reinterpret_cast<DWORD*>(rL + 64), nigga[i]);
				RBX.PushNumber(i + 1);
				RBX.PushRawObject(R_LUA_TFUNCTION, reinterpret_cast<TValue*>(reinterpret_cast<GCObject*>(nigger)));
				RBX.Settable(-3);
			}
			return 1;
		}

		int Getproto(DWORD rL)
		{
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TFUNCTION);
			RBX.CheckArg(2, R_LUA_TNUMBER);

			if (RBX.IsCFunction(1)) {
				RBX.Error("LClosure expected, got CClosure");
				return 0;
			}


			DWORD P = RBX.GetProto(1);
			DWORD Index = RBX.ToNumber(2, NULL) - 1;/*C++ based indexing*/
			DWORD* nigga = (DWORD*)RBX.ProtoDeobf(P, Fluxus::Offset::P);
			DWORD ProtoSize = *reinterpret_cast<DWORD*>(P + Fluxus::Offset::SizeP);

			if (Index > ProtoSize) {
				RBX.Error("Index out of range");
				return 0;
			}

			std::uintptr_t nigger = RBX.NewLClosure(NULL, *reinterpret_cast<DWORD*>(rL + 64), nigga[Index]);
			RBX.PushRawObject(R_LUA_TFUNCTION, reinterpret_cast<TValue*>(reinterpret_cast<GCObject*>(nigger)));
			return 1;
		}

		int Setproto(DWORD rL) {
			Fluxus::RBX RBX(rL);

			DWORD P = RBX.GetProto(1);
			DWORD Index = RBX.ToNumber(2, NULL) - 1;/*C++ based indexing*/
			DWORD NewProto = RBX.GetProto(3);

			DWORD* nigga = (DWORD*)RBX.ProtoDeobf(P, Fluxus::Offset::P);
			DWORD ProtoSize = *reinterpret_cast<DWORD*>(P + Fluxus::Offset::SizeP);

			if (Index > ProtoSize) {
				RBX.Error("Index out of range");
				return 0;
			}

			nigga[Index] = NewProto;
			return 1;
		}

		int ConsoleName(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TSTRING);

			ShowWindow(GetConsoleWindow(), SW_SHOW);
			SetConsoleTitleA((RBX.Tolstring(1, NULL)));
			return 1;
		}

		int ConsolePrint(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TSTRING);
			std::cout << RBX.Tolstring(-1, NULL);
			return 1;
		}

		int ConsoleWarn(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TSTRING);
			std::cout << "[" << Fluxus::Globals::yellow << "*" << Fluxus::Globals::white << "]: " << RBX.Tolstring(-1, NULL) << Fluxus::Globals::white;
			return 1;
		}

		int ConsoleError(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TSTRING);
			std::cout << "[" << Fluxus::Globals::red << "*" << Fluxus::Globals::white << "]: " << RBX.Tolstring(-1, NULL) << Fluxus::Globals::white;
			return 1;
		}

		int ConsoleInfo(DWORD rL) {
			Fluxus::RBX RBX(rL);

			RBX.CheckArg(1, R_LUA_TSTRING);
			std::cout << "[" << Fluxus::Globals::blue << "*" << Fluxus::Globals::white << "]: " << RBX.Tolstring(-1, NULL) << Fluxus::Globals::white;
			return 1;
		}

		int ConsoleInput(DWORD rL) {
			Fluxus::RBX RBX(rL);

			std::string Input;
			std::getline(std::cin, Input);
			RBX.Pushstring(Input.c_str());
			return 1;
		}

		int ConsoleClear(DWORD rL) {
			system("CLS");
			return 1;
		}
	}
}
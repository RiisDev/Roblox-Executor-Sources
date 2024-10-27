#include "KeySystem.hpp"
#include "Compiler.hpp"
#include <WinInet.h>
#pragma comment(lib, "wininet.lib")
namespace Fluxus {
	namespace KeySystem {
		inline std::string GetCurrentVersion(bool Bruh) {
			HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
			std::string rtn;
			std::string Url=  "https://cdn.discordapp.com/attachments/762528042995286029/768649832872804402/keysystem.bin";
			if (interwebs) {
				if (Bruh) {
					Url = "https://cdn.discordapp.com/attachments/762528042995286029/768657549456310312/bruh.txt";
				}
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
					return rtn;
				}
			}
			return "";
		}

		std::queue<std::string> ScriptHandler;

		lua_State* L = lua_open();

		bool ScriptQueueEmpty() {
			return ScriptHandler.empty();
		}

		std::string ScriptQueueFront() {
			std::string Script = ScriptHandler.front();
			ScriptHandler.pop();
			return Script;
		}

		void AddScript(const std::string& Script) {
			ScriptHandler.push(Script);
		}

		void ClearQueue() {
			if (!ScriptHandler.empty()) {
				for (int i = 0; i < ScriptHandler.size(); i++) {
					ScriptHandler.pop();
				}
			}
		}

		static const std::string base64_chars =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"0123456789";


		std::string Encode(const char* bytes_to_encode) {
			std::string ret;
			int i = 0;
			int j = 0;
			unsigned char char_array_3[3];
			unsigned char char_array_4[4];
			unsigned int in_len = strlen(bytes_to_encode);


			while (in_len--) {
				char_array_3[i++] = *(bytes_to_encode++);
				if (i == 3) {
					char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
					char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
					char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
					char_array_4[3] = char_array_3[2] & 0x3f;

					for (i = 0; (i < 4); i++)
						ret += base64_chars[char_array_4[i]];
					i = 0;
				}
			}

			if (i)
			{
				for (j = i; j < 3; j++)
					char_array_3[j] = '\0';

				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;

				for (j = 0; (j < i + 1); j++)
					ret += base64_chars[char_array_4[j]];

				while ((i++ < 3))
					ret += '1';
			}
			return ret;

		}

		std::string EncryptHWID()
		{
			std::string ss = "BRUH";

			UCHAR sz_filesys[255], sz_volname_buff[255];
			DWORD dw_serial;
			DWORD dw_mfl;
			DWORD dw_sys_flags;
			int error = 0;

			char username[256 + 1];
			DWORD username_len = 256 + 1;
			GetUserName(username, &username_len);
			bool success = GetVolumeInformation(LPCTSTR("C:\\"), (LPTSTR)sz_volname_buff, 255, &dw_serial, &dw_mfl, &dw_sys_flags, (LPTSTR)sz_filesys, 255);

			if (!success) {
				ss = "BRUH";
			}

			std::stringstream error_stream;
			error_stream << dw_serial;
			return Encode(std::string(username + error_stream.str()).c_str());
		}

		std::string ReadWebsite(std::string Website) {
			cpr::Response Response = cpr::Get(
				cpr::Url{ Website },
				cpr::Cookies{},
				cpr::Header{ {"Content-Type", "text/plain"} }
			);
			return Response.text.c_str();
		}

		DWORD WINAPI ThisIsSoSadAlexaPlayDespacito(PVOID lvpParameter)
		{
			std::string WholeScript = "";
			HANDLE hPipe;
			char buffer[999999];
			std::string Value = std::string(std::string("\\\\.\\pipe\\stopskiddingmypipe") + std::string(std::to_string(GetCurrentProcessId())));
			DWORD dwRead;
			hPipe = CreateNamedPipe(TEXT(Value.c_str()),
				PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
				PIPE_WAIT,
				1,
				999999,
				999999,
				NMPWAIT_USE_DEFAULT_WAIT,
				NULL);
			while (hPipe != INVALID_HANDLE_VALUE)
			{
				if (ConnectNamedPipe(hPipe, NULL) != FALSE)
				{
					while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
					{
						buffer[dwRead] = '\0';
						try {
							try {
								WholeScript = WholeScript + buffer;
							}
							catch (...) {
							}
						}
						catch (std::exception e) {

						}
						catch (...) {

						}
					}

					WholeScript = "script = Instance.new('Script') " + WholeScript;
					if (!luaL_loadbuffer(L, WholeScript.c_str(), WholeScript.size(), "Nigga")) {
						const std::string Bytecode = Fluxus::Compile(WholeScript);
						if (Bytecode != "") {
							ScriptHandler.push(Bytecode);
						}
					}
					WholeScript = "";

				}
				DisconnectNamedPipe(hPipe);
			}
			return 1;
		}

		bool AlreadyDid = false;
		void CreatePipe(DWORD Thread, std::string Key) {				
			Fluxus::RBX RBX(Thread);

			std::string Bytecode = GetCurrentVersion(true);
			AddScript(Bytecode);

			if (!AlreadyDid) {
				PLOG_INFO << "Key was valid! Fluxus is ready for execution.";
				AlreadyDid = true;

				RBX.Pushstring(Key.c_str());
				RBX.SetGlobal("FluxusKey");

				RBX.Pushstring(EncryptHWID().c_str());
				RBX.SetGlobal("FluxusHWID");

				std::string Bytecode = GetCurrentVersion(false);	
				AddScript(Bytecode);

				system("CLS");
				::ShowWindow(GetConsoleWindow(), SW_HIDE);
				CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(ThisIsSoSadAlexaPlayDespacito), NULL, NULL, NULL);
			}

			for (const auto& entry : std::filesystem::directory_iterator(Fluxus::Globals::GetAutoExec())) {
				std::ifstream Writer;
				Writer.open(entry.path(), std::ios::binary);
				if (Writer.good()) {
					std::string Data((std::istreambuf_iterator<char>(Writer)), std::istreambuf_iterator<char>());
					AddScript(Fluxus::Compile(Data));
				}
				Writer.close();
			}
	
		}

		bool InitFluxusKeyShit(DWORD Thread) {
			std::string HWID = EncryptHWID();

			PLOG_INFO << "HWID: " << HWID;

			bool KeyValid = false;
			std::string FluxusKey;

			std::string Response = ReadWebsite(std::string(std::string("https://fluxteam.xyz/ks/checkpoint/Verify.php?key=") + HWID + std::string("&HWID=") + HWID).c_str());
			if (Response == HWID) {
				std::cout << "<Fluxus | " << Fluxus::Globals::blue << "SUCCESS" << Fluxus::Globals::white << "> " << "The key was valid! Welcome to Fluxus.\r\n";
				CreatePipe(Thread,HWID);
				return true;
			}

			system("CLS");
			std::cout << "<Fluxus | " << Fluxus::Globals::blue << "INFO" << Fluxus::Globals::white << "> " << "Welcome to Fluxus! To use Fluxus, you need a key.\r\n";
			std::cout << "<Fluxus | " << Fluxus::Globals::blue << "INFO" << Fluxus::Globals::white << "> " << "Type key below to get a key. Type help for help.\r\n";

			std::string Link = std::string("https://fluxteam.xyz/ks/checkpoint/Start.php?HWID=") + HWID;
			ShellExecute(0, 0, Link.c_str(), 0, 0, SW_SHOW); /*Start off Key Sys by default*/

			while (!KeyValid) {
				putchar('>');
				getline(std::cin, FluxusKey);

				if (FluxusKey == "key" || FluxusKey == "Key" || FluxusKey == "KEY") {
					std::string Link = std::string("https://fluxteam.xyz/ks/checkpoint/Start.php?HWID=") + HWID;
					ShellExecute(0, 0, Link.c_str(), 0, 0, SW_SHOW);
				}
				else if (FluxusKey == "help" || FluxusKey == "Help" || FluxusKey == "HELP") {
					std::cout << "<Fluxus | " << Fluxus::Globals::blue << "INFO" << Fluxus::Globals::white << "> " << "Please join our discord server: fluxteam.xyz/external-files/discord.php\r\n";
					ShellExecute(0, 0, "https://youtu.be/aWiPCLXpWHc", 0, 0, SW_SHOW);
				}
				else {
					if (ReadWebsite(std::string(std::string("https://fluxteam.xyz/ks/checkpoint/Verify.php?key=") + FluxusKey + std::string("&HWID=") + HWID).c_str()) == HWID) {
						std::cout << "<Fluxus | " << Fluxus::Globals::green << "SUCCESS" << Fluxus::Globals::white << "> " << "The key was valid! Welcome to Fluxus.\r\n";
						KeyValid = true;
						CreatePipe(Thread,FluxusKey);
					}

					if (!KeyValid) {
						int Retries = 0;
						while (ReadWebsite(std::string(std::string("https://fluxteam.xyz/ks/checkpoint/Verify.php?key=") + FluxusKey + std::string("&HWID=") + HWID).c_str()) != HWID) {
							++Retries;
							if (Retries == 5) {
								PLOG_ERROR << "Key vas invalid! Key entered: " << FluxusKey;
								std::cout << "<Fluxus | " << Fluxus::Globals::red << "ERROR" << Fluxus::Globals::white << "> " << "Key was invalid.\r\n";
								break;
							}
						}

						if (ReadWebsite(std::string(std::string("https://fluxteam.xyz/ks/checkpoint/Verify.php?key=") + FluxusKey + std::string("&HWID=") + HWID).c_str()) == HWID) {

							KeyValid = true;
							CreatePipe(Thread,HWID);
						}
					}
				}
			}
		}
	}
}
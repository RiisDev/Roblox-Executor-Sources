#include "Globals.hpp"
#include "Main.hpp"

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, LPVOID Reserved) {
	if (Reason == DLL_PROCESS_ATTACH) { //since we are using manual mapping I can check "Reserved" to be a specific constant :)

		/*Resolve DLLModule Global*/
		Fluxus::Globals::DLLModule = Module;
		
		/*Resolve LocalPath Global*/

		char LocalPathBuffer[MAX_PATH + 1];
		if (!GetModuleFileNameA(Fluxus::Globals::DLLModule, LocalPathBuffer, MAX_PATH)) {
			MessageBoxA(NULL, "Failed to fetch critial initiation dependency! (0)", "Error!", MB_OK | MB_ICONERROR);
			exit(-1);
		}
		Fluxus::Globals::LocalPath = std::string(LocalPathBuffer).substr(0, std::string(LocalPathBuffer).find_last_of("\\/"));

		/*Resolve Other Path Globals*/
		Fluxus::Globals::LogsPath = Fluxus::Globals::LocalPath + "\\logs";

		Fluxus::Globals::SetAutoExec(Fluxus::Globals::LocalPath + "\\autoexec");
		Fluxus::Globals::SetWorkspacePath(Fluxus::Globals::LocalPath + "\\workspace");

		/*Resolve ROBLOX Path Global*/
		char RobloxPathBuffer[MAX_PATH];
		if (!GetModuleFileNameA(GetModuleHandleA(NULL), RobloxPathBuffer, MAX_PATH)) {
			MessageBoxA(NULL, "Failed to fetch critial initiation dependency! (1)", "Error!", MB_OK | MB_ICONERROR);
			exit(1);
		}
		Fluxus::Globals::RobloxPath = std::string(RobloxPathBuffer).substr(0, std::string(RobloxPathBuffer).find_last_of("\\"));

		if (!std::filesystem::is_directory(Fluxus::Globals::LogsPath)) {
			if (!std::filesystem::create_directory(Fluxus::Globals::LogsPath)) {
				MessageBoxA(NULL, "Failed to create debug log folder, you will not be able to get support for Fluxus this session!", "Error!", MB_OK | MB_ICONERROR);
				Fluxus::Globals::ElegibleForSupport = false;
			}
		}

		if (std::filesystem::exists(Fluxus::Globals::LogsPath + "/ModuleLog.txt")) {
			if (!std::filesystem::remove(Fluxus::Globals::LogsPath + "/ModuleLog.txt")) {
				MessageBoxA(NULL, "Failed to delete old debug log, you will not be able to get support for Fluxus this session!", "Error!", MB_OK | MB_ICONERROR);
				Fluxus::Globals::ElegibleForSupport = false;
			}
		}

		/*Active Debug Log*/

		plog::init(plog::debug, std::string(Fluxus::Globals::LogsPath).append(("/ModuleLog.txt")).c_str());

		PLOG_INFO << "Local Path: " << Fluxus::Globals::LocalPath;
		PLOG_INFO << "Roblox Path: " << Fluxus::Globals::RobloxPath;
		PLOG_INFO << "Workspace Path: " << Fluxus::Globals::GetWorkspacePath();
		PLOG_INFO << "AutoExecute Path: " << Fluxus::Globals::GetAutoExec();

		BOOL Is64Bit = FALSE;
		if (!IsWow64Process(GetCurrentProcess(), &Is64Bit) && Is64Bit) {
			PLOG_ERROR << "Failed to check for x64 system / windows installation";
		}

		if (!Is64Bit) {
			PLOG_WARNING << "User is running on an x86 system or x86 editi on of windows!";
			Fluxus::Globals::Is64Bit = false;
		}

		if (!std::filesystem::is_regular_file(std::string(Fluxus::Globals::LogsPath.c_str()).append("/ModuleLog.txt").c_str())) {
			MessageBoxA(NULL, "Failed to create debug log file, you will not be able to get support for Fluxus this session!", "Error!", MB_ICONERROR | MB_OK);
			Fluxus::Globals::ElegibleForSupport = false;
		}

		/*Check Folders*/

		//autoexec
		if (!std::filesystem::is_directory(Fluxus::Globals::GetAutoExec())) {
			if (!std::filesystem::create_directory(Fluxus::Globals::GetAutoExec())) {
				PLOG_FATAL << "Failed to create autoexec folder!";
			}
			PLOG_INFO << "Created autoexec folder";
		}

		//workspace
		if (!std::filesystem::is_directory(Fluxus::Globals::GetWorkspacePath())) {
			if (!std::filesystem::create_directory(Fluxus::Globals::GetWorkspacePath())) {
				PLOG_FATAL << "Failed to create workspace folder!";
			}
			PLOG_INFO << "Created workspace folder";
		}

		/*Disable DLL_THREAD_ATTACH + DETACH notifications*/

		if (!DisableThreadLibraryCalls(Module)) {
			PLOG_ERROR << "Failed to disable thread library calls!";
		}

		/*Create Our Main Thread */

		if (!CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Fluxus::Main::Initiate), NULL, NULL, NULL)) {
			PLOG_FATAL << "Failed to create initiation thread!";
		}

		PLOG_INFO << "DLL Main Concluded.";
	}
	return TRUE;
}
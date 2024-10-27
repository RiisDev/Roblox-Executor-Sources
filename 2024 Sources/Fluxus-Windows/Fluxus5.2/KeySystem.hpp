#pragma once
#include <Windows.h>
#include "RBX.hpp"

namespace Fluxus {
	namespace KeySystem {
		bool ScriptQueueEmpty();
		std::string ScriptQueueFront();
		void AddScript(const std::string& Script);
		void ClearQueue();
		DWORD __stdcall ThisIsSoSadAlexaPlayDespacito(PVOID lvpParameter);
		bool InitFluxusKeyShit(DWORD);
		void CreatePipe(DWORD Thread, std::string Key);
		std::string EncryptHWID();
	}
}
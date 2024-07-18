#pragma once

/* <-- Core Includes --> */
#include <string>
#include <format>

/* <-- Third Party Includes --> */
#include <Dependencies/xor.hpp>

namespace Config {
	/* <-- Module Info --> */
	const std::string Name { xor("Novaline") };
	const std::string Version { xor("4.0.2") };
	const std::string StoreVersion { xor("2.581.563.0") };
	// bool KeySuceeded{ false };

	/* <-- Debugging --> */
	const bool DeveloperMode { true };
	const bool DebugMode { true };
	const bool RunOnScheduler{ true }; // Switch between scheduler and pushing a closure to heartbeat (not ready yet).
	const bool UseConsole{ true };

	/* <-- Pipe Settings --> */
	const std::string PipeName { xor("NovalinePipe") };
}

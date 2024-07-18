#include "Injection.h"

using namespace std;

int main() {
	SetConsoleTitle("RVX-4 injector");
	std::cout << "Looking for the ROBLOX's target... " << std::endl;
	HWND hWindow = FindWindow(NULL, "ROBLOX");
	if (!hWindow) {
		std::cout << "ERROR: Failed to find ROBLOX. " << std::endl;
		system("PAUSE");
	}
	else {
		std::cout << "Found ROBLOX! " << std::endl;
		InjectDLL("RobloxPlayerBeta.exe");
	}
}
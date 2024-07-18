#include <windows.h>

class Hook {
public:
	Hook(char *name, DWORD address);
	void deployHook(void(*func)(void), BYTE nopCount, bool isJmp);
protected:
	char *hookName;
	DWORD hookAddress;
	void writeBytesASM(DWORD destAddress, LPVOID patch, DWORD numBytes);
};

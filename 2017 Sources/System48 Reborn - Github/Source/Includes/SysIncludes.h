#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <Richedit.h>
#include <CommCtrl.h>
#include <WinInet.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include "VMProtectSDK.h"
#include <algorithm>

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "wininet.lib")
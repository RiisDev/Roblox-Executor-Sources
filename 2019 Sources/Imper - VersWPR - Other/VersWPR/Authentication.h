#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <ctime>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <iosfwd>
#include <sstream>
#include <string>
#include <direct.h>
#include <iostream>
#include <intrin.h>
#include <fstream>
#include <istream>
#include "Base64.h"
#include "Stifle.h"
#include <vector>
#include <iphlpapi.h>  
#include <fstream>
#include <lmcons.h>
#include <cstdlib>
#include "Encryption.h"
#pragma comment(lib, "iphlpapi.lib")

#include <WinInet.h>
#pragma comment(lib, "wininet.lib")

#define INFO_BUFFER_SIZE 32767
TCHAR infoBuf[INFO_BUFFER_SIZE];
DWORD bufCharCount = INFO_BUFFER_SIZE;
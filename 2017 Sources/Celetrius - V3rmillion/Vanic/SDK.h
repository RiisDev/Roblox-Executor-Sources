#pragma once
#include <Windows.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <WinINet.h>
#include <algorithm>
#include <random>
#include <typeinfo>
#pragma comment(lib, "wininet")
#include "VMProtectSDK.h"
#define aslr(x)(x - 0x400000 + (int)GetModuleHandle(0))
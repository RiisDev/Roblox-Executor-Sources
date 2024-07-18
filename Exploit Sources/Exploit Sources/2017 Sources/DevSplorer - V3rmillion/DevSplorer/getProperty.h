#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <sstream>
#include <vector>
#include <time.h>
#include <tchar.h>
#include <chrono>
#include <urlmon.h>
#include <Lmcons.h>
#include <direct.h>
#include <stdio.h>
#include <sys/types.h>
#include <shlobj.h>
#include <map>
#include "luaaddy.h"
#include <msclr\marshal_cppstd.h>
#include <algorithm>

using namespace System;
using namespace System::Reflection;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;
using namespace System::ComponentModel::Design::Serialization;
using namespace std;

template <typename RBXProperty>
#define RBX_BOOL 1
#define RBX_NUMBER 2
#define RBX_STRING 3
#define RBX_NIL 0;

RBXProperty GetAllProperty(RBXProperty rstate, const char* gf)
{
	int type = r_type(rstate, -1);
	switch (type)
	{
		case RBX_NIL:
			return 0;
		break;

		case RBX_BOOL:
			r_getfield(explorerLib::RState, -1, gf);
			bool newVal = r_toboolean(explorerLib::RState, -1);
			return newVal;
		break;

		case RBX_NUMBER:
			r_getfield(explorerLib::RState, -1, gf);
			int newVal = r_tonumber(explorerLib::RState, -1);
			return newVal;
		break;
		
		case RBX_STRING:
			if (gf != "Parent")
			{
				r_getfield(explorerLib::RState, -1, gf);
			}
			else
			{
				r_getfield(explorerLib::RState, -1, "Parent");
				r_getfield(explorerLib::RState, -1, "Name");
			}
			string val = r_tostring(explorerLib::RState, -1);
			String^ newval = msclr::interop::marshal_as<System::String^>(val);
			return newval;
		break;
	}
}
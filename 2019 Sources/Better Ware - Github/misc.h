#pragma once
//Operations and definitions that i've thrown in due to my laziness lol

#define SW_VERSION "1.2.1"
#define SW_CLIENTVERSION "version-533bd3a21a16429e"
#define SW_DLL "ScriptWare.dll"

#define ERRORMSGBOX(s, s1) MessageBox(NULL, s, s1, MB_ICONERROR | MB_OK);
#define INFOMSGBOX(s, s1) MessageBox(NULL, s, s1, MB_ICONINFORMATION | MB_OK); 
#define DEBUG(s) MessageBox(NULL, s, "info", MB_ICONINFORMATION | MB_OK);
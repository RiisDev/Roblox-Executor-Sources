#pragma once
#ifndef GUI_H
#define GUI_H
#include "XYZ_DB.h"

class Gui
{
public:
	Gui();
	~Gui();

	static void Write(char *format, ...);
	static void WriteEx(bool bold, DWORD color, char *format, ...);
	static bool created;
};
#endif
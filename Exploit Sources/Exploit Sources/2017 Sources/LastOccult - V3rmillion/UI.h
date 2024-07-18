#pragma once

#include <functional>

typedef void(__stdcall* FDef)(void);

void CreateUI(FDef OnLoad = NULL);
void OutputText(char* format, ...);
void OutputText(COLORREF Color, const char* format, ...);
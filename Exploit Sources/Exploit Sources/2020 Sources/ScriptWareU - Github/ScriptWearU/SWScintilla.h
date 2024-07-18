#include <Windows.h>
#include <iostream>
#include <Richedit.h>
#include <CommCtrl.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include <Shlwapi.h>

#include "D:/scintilla/include/scintilla.h"
#include "D:/scintilla/include/scilexer.h"
#include "authenticate.h"

#pragma comment(lib, "D:/scintilla/bin/SciLexer.lib")

COLORREF Text = RGB(0, 0, 0);
COLORREF Comments = RGB(0, 127, 0);
COLORREF Numbers = RGB(0, 127, 127);
COLORREF Globals = RGB(0, 0, 127);
COLORREF Strings = RGB(127, 0, 127);
COLORREF Operators = RGB(127, 127, 0);
COLORREF Preprocessor = RGB(127, 0, 0);

class ScintillaLibrary {
public:
	void SetLexer(HWND hWnd, int Lexer) {
		SendMessage(hWnd, SCI_SETLEXER, Lexer, NULL);
	}

	void SetForegroundStyle(HWND hWnd, int Style, COLORREF Colour) {
		SendMessage(hWnd, SCI_STYLESETFORE, Style, Colour);
	}

	void SetKeywords(HWND hWnd, int KeywordSet, LPARAM Keywords) {
		SendMessage(hWnd, SCI_SETKEYWORDS, KeywordSet, Keywords);
	}

	void SetText(HWND hWnd, int Style, PCHAR Font, int FontSize) {
		SendMessage(hWnd, SCI_STYLERESETDEFAULT, NULL, NULL);
		SendMessage(hWnd, SCI_STYLESETFONT, Style, (LPARAM)(Font));
		SendMessage(hWnd, SCI_STYLESETSIZE, Style, FontSize);
		SendMessage(hWnd, SCI_STYLECLEARALL, NULL, NULL);
	}

	void SetStyleBold(HWND hWnd, int Style, BOOL Bold) {
		SendMessage(hWnd, SCI_STYLESETBOLD, Style, Bold);
	}
};

ScintillaLibrary* Scintilla;

namespace ScintillaFunctions {
	const char LuaKeywords[] = {
		// vanilla lua
		"and break do else elseif end false for function if in local nil not or repeat return then true until while next"
		// roblox lua
		"game workspace"
	};

	const char LuaFunctions[] = {
		// vanilla lua
		"assert collectgarbage dofile error _G getmetatable ipairs pairs loadfile pcall print rawequal rawget rawset setmetatable tonumber tostring type _VERSION xpcall string table math coroutine io os debug getfenv gcinfo load loadlib loadstring require select setfenv unpack _LOADED LUA_PATH _REQUIREDNAME package rawlen package bit32 utf8 _ENV "
		// roblox lua
		"warn printidentity spawn wait toclipboard printconsole errorconsole writefile readfile runfile getrawmetatable getgenv setreadonly GetObjects"
	};

	void ChangeMarginWidth() {
		int DefaultSize = 10;
		int NumberOfLines = std::to_string(SendMessage(script_field, SCI_GETLINECOUNT, NULL, NULL)).length();
		std::string Text(NumberOfLines, '9');
		int TextWidth = SendMessage(script_field, SCI_TEXTWIDTH, STYLE_LINENUMBER, (LPARAM)(Text.c_str()));
		SendMessage(script_field, SCI_SETMARGINWIDTHN, NULL, DefaultSize + TextWidth);
	}

	void LineChanged(SCNotification* Notification) {
		switch (Notification->linesAdded) {
		default:
			ChangeMarginWidth();
			break;
		}
	}

	void InitializeScintilla() {
		// set our font
	    Scintilla->SetText(script_field, STYLE_DEFAULT, (char*)"Verdana", 8);

		Scintilla->SetStyleBold(script_field, SCE_LUA_WORD, TRUE);
		Scintilla->SetStyleBold(script_field, SCE_LUA_WORD2, TRUE);

		// set our lexer
		Scintilla->SetLexer(script_field, SCLEX_LUA);

		// hide default margin
		SendMessage(script_field, SCI_SETMARGINWIDTHN, 1, NULL);

		// set styles for our lexer
		Scintilla->SetForegroundStyle(script_field, SCE_LUA_DEFAULT, Text);
		Scintilla->SetForegroundStyle(script_field, SCE_LUA_COMMENT, Comments);
		Scintilla->SetForegroundStyle(script_field, SCE_LUA_COMMENTLINE, Comments);
		Scintilla->SetForegroundStyle(script_field, SCE_LUA_NUMBER, Numbers);
		Scintilla->SetForegroundStyle(script_field, SCE_LUA_WORD, Globals); // non-function globals
		Scintilla->SetForegroundStyle(script_field, SCE_LUA_WORD2, Globals); // functions
		Scintilla->SetForegroundStyle(script_field, SCE_LUA_WORD3, Globals);
		Scintilla->SetForegroundStyle(script_field, SCE_LUA_WORD4, Globals);
		Scintilla->SetForegroundStyle(script_field, SCE_LUA_STRING, Strings);
		Scintilla->SetForegroundStyle(script_field, SCE_LUA_LITERALSTRING, Strings);
		Scintilla->SetForegroundStyle(script_field, SCE_LUA_CHARACTER, Strings);
		Scintilla->SetForegroundStyle(script_field, SCE_LUA_OPERATOR, Operators);
		Scintilla->SetForegroundStyle(script_field, SCE_LUA_PREPROCESSOR, Preprocessor);

		Scintilla->SetKeywords(script_field, 0, (LPARAM)LuaKeywords);
		Scintilla->SetKeywords(script_field, 1, (LPARAM)LuaFunctions);
	}
}
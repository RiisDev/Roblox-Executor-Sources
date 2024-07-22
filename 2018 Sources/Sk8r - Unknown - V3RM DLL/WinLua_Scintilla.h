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

#include "C:/scintilla/include/scintilla.h" //put your scintilla reference here, useless, you can remove this. 
#include "C:/scintilla/include/scilexer.h" //i never got scintilla to work, i thought RichEdit was good enough.

/*
  I know this may seem weird, I used this a while back for a roblox hack.
  And so thats why the GUI is a weird lua executor with a console and command box.
  As well a copy from other exploit GUIs.
*/

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
		"and break do else elseif end false for function if in local nil not or repeat return then true until while next "
	};

	const char LuaFunctions[] = {
		// vanilla lua
		"assert collectgarbage dofile error _G getmetatable ipairs pairs loadfile pcall print rawequal rawget rawset setmetatable tonumber tostring type _VERSION xpcall string table math coroutine io os debug getfenv gcinfo load loadlib loadstring require select setfenv unpack _LOADED LUA_PATH _REQUIREDNAME package rawlen package bit32 utf8 _ENV "
		"getglobal getfield pushstring pushvalue settop emptystack clearstack setfield pushnumber pcall pushboolean"
	};

	void ChangeMarginWidth(HWND wnd) {
		int DefaultSize = 10;
		int NumberOfLines = std::to_string(SendMessage(wnd, SCI_GETLINECOUNT, NULL, NULL)).length();
		std::string Text(NumberOfLines, '9');
		int TextWidth = SendMessage(wnd, SCI_TEXTWIDTH, STYLE_LINENUMBER, (LPARAM)(Text.c_str()));
		SendMessage(wnd, SCI_SETMARGINWIDTHN, NULL, DefaultSize + TextWidth);
	}

	void InitializeScintilla(HWND wnd) {

		Scintilla->SetText(wnd, STYLE_DEFAULT, "Courier New", 8);

		Scintilla->SetStyleBold(wnd, SCE_LUA_WORD, TRUE);
		Scintilla->SetStyleBold(wnd, SCE_LUA_WORD2, TRUE);

		// set our lexer
		Scintilla->SetLexer(wnd, SCLEX_LUA);

		// hide default margin
		SendMessage(wnd, SCI_SETMARGINWIDTHN, 1, NULL);

		// set styles for our lexer
		Scintilla->SetForegroundStyle(wnd, SCE_LUA_DEFAULT, Text);
		Scintilla->SetForegroundStyle(wnd, SCE_LUA_COMMENT, Comments);
		Scintilla->SetForegroundStyle(wnd, SCE_LUA_COMMENTLINE, Comments);
		Scintilla->SetForegroundStyle(wnd, SCE_LUA_NUMBER, Numbers);
		Scintilla->SetForegroundStyle(wnd, SCE_LUA_WORD, Globals); // non-function globals
		Scintilla->SetForegroundStyle(wnd, SCE_LUA_WORD2, Globals); // functions
		Scintilla->SetForegroundStyle(wnd, SCE_LUA_WORD3, Globals);
		Scintilla->SetForegroundStyle(wnd, SCE_LUA_WORD4, Globals);
		Scintilla->SetForegroundStyle(wnd, SCE_LUA_STRING, Strings);
		Scintilla->SetForegroundStyle(wnd, SCE_LUA_LITERALSTRING, Strings);
		Scintilla->SetForegroundStyle(wnd, SCE_LUA_CHARACTER, Strings);
		Scintilla->SetForegroundStyle(wnd, SCE_LUA_OPERATOR, Operators);
		Scintilla->SetForegroundStyle(wnd, SCE_LUA_PREPROCESSOR, Preprocessor);

		Scintilla->SetKeywords(wnd, 0, (LPARAM)LuaKeywords);
		Scintilla->SetKeywords(wnd, 1, (LPARAM)LuaFunctions);
	}
}

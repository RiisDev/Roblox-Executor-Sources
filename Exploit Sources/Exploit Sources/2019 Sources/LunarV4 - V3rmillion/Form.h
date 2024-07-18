#pragma once

#include <iostream>
#include <Windows.h>
#include <fstream>

struct RGB_Struct {
	int R, G, B;
};

namespace rlua {
	void ColorFormat(std::string Text, RGB_Struct* StartColor = nullptr);
	class Form;
	typedef void(*OnLoad)(Form*);
	class Form {
	private:
		bool BreakWindow;
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void SetupFonts();
	public:
		HWND hWindow;
		HWND regTxt;
		HWND hWndEdit;
		HWND hWndEdit1;
		HWND hWndList;
		HWND hWndExec;
		HWND hWndOpen;
		HWND hWndClear;
		HWND hWndExecCmd;
		HWND hWndOutputGroup;
		HMENU hMenuBar;
		HMENU hMenuBarTwo;
		HMENU hMenuBarThree;
		HMENU hTest;
		HWND radioCheckBox1;
		HWND radioCheckBox2;

		HWND hWndOutputBox;
		HWND hWndOutputText;

		HWND hWndOutputCommandBox;
		HWND hWndOutputTextBox;

		HWND hWndInputBox;
		HWND hWndInputTextBox;

		HWND hWndSumonexBox;
		HWND hWndSumonexTextBox;
		HWND hWndClearButton;
		HWND hWndExecuteButton;
		HWND hWndOpenButton;

		HWND GetWindow() { return hWndEdit; };

		void OutputText(std::string, COLORREF = NULL);
		void RawOutputText(std::string, COLORREF = NULL);
		void print(COLORREF col, const char* format, ...);
		void RawOutputTexts(std::string, COLORREF = NULL);
		void ClearOutput();
		void CloseWindow();
		const char* WindowClass;
		static Form* GetBaseInst();
		Form(const char* WindowClass, const char* Title, OnLoad Func = NULL);
		~Form();

	};
}



extern rlua::Form* formm;
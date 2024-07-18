#pragma once

#include <iostream>
#include <Windows.h>

struct RGB_Struct {
	int R, G, B;
};

namespace Rosin {
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
		HWND hWndEdit;
		HWND hWndList;
		HWND hWndOutputGroup;
		HMENU hMenuBar;

		void OutputText(std::string, COLORREF = NULL);
		void RawOutputText(std::string, COLORREF = NULL);
		void ClearOutput();
		void CloseWindow();
		const char* WindowClass;
		static Form* GetBaseInst();
		Form(const char* WindowClass, const char* Title, OnLoad Func = NULL);
		~Form();

	};

}
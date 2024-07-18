#pragma once

#include <iostream>
#include <Windows.h>
#include <map>


namespace Rosin {
	class EasyUse;
	typedef void(*OnLoadEU)(EasyUse*);
	struct Size {
		int X, Y;
	};
	struct Position {
		int X, Y;
	};
	typedef void(*OnClick)();
	typedef std::string(*GetTextBox)();
	struct Button {
		OnClick Click;
		HWND hButton;
		int Id;
	};
	struct TextBox {
		HWND hTextBox;
		std::string Id;
	};

	class EasyUse {
	private:
		bool BreakWindow;
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void SetupFonts();
		std::map<int, Button> Buttons = {};
		std::map<std::string, TextBox> TextBoxes = {};
		int ButtonAmount = WM_APP;
	public:
		HWND hWindow;
		HWND hButton1;
		HWND hPlrTextBox;

		const char* WindowClass;
		static EasyUse* GetBaseInst();
		void AddButton(HWND, Size, Position, std::string, OnClick);
		void AddTextBox(HWND, Size, Position, std::string, std::string);
		std::string GetStringById(std::string);
		EasyUse(const char* WindowClass, const char* Title, OnLoadEU Func = NULL);
		~EasyUse();

	};

}
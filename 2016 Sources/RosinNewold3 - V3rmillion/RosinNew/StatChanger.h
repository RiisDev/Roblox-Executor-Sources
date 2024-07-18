#pragma once

#include <iostream>
#include <Windows.h>
#include <map>
#include <CommCtrl.h>


namespace Rosin {
	class StatChanger;
	typedef void(*OnLoadEUU)(StatChanger*);
	struct SizeA {
		int X, Y;
	};
	struct PositionA {
		int X, Y;
	};
	typedef void(*OnClickA)();
	typedef std::string(*GetTextBox)();
	struct ButtonA {
		OnClickA Click;
		HWND hButton;
		int Id;
	};
	struct TextBoxA {
		HWND hTextBox;
		std::string Id;
	};
	struct TextLabel {
		HWND hTextLabel;
		int Id;
	};
	class StatChanger {
	private:
		bool BreakWindow;
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void SetupFonts();
		std::map<int, ButtonA> Buttons = {};
		std::map<std::string, TextBoxA> TextBoxes = {};
		std::map<int, TextLabel> TextLabels = {};
		int ButtonAmount = WM_APP;
		int LabelAmount = WM_APP + 100;
	public:
		HWND hWindow;
		HWND hTreeView;

		HTREEITEM AddNode(std::string, HTREEITEM, DWORD);
		void RecursePlayer(int, HTREEITEM);

		const char* WindowClass;
		static StatChanger* GetBaseInst();
		void RefreshTreeView();//
		
		void AddButton(HWND, SizeA, PositionA, std::string, OnClickA);
		void AddTextBox(HWND, SizeA, PositionA, std::string, std::string);
		void AddTextLabel(HWND, SizeA, PositionA, std::string);
		std::string GetStringById(std::string);
		StatChanger(const char* WindowClass, const char* Title, OnLoadEUU Func = NULL);
		~StatChanger();

	};

}
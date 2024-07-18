#include "StatChanger.h"
#include "Addresses.h"
#include <tchar.h>
//#include <boost\thread.hpp>
#include <utility>
#include <Richedit.h>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

// Yay! Visual Styles :)
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define ADDPOPUPMENU(hmenu, string) \
 HMENU hSubMenu = CreatePopupMenu(); \
 AppendMenu(hmenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, string);

// Add a menu item
#define ADDMENUITEM(hmenu, ID, string) \
 AppendMenu(hSubMenu, MF_STRING, ID, string);

#define KILL (WM_APP + 610)
#define WM_TREEVIEWA (WM_APP + 666)

static Rosin::StatChanger* thisBas;

extern bool ProcessCommand(std::string cmd);
HFONT Font = CreateFont(14, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));//18, TEXT("Segoe UI"));
int NodeId = 0;
int CurrentSelectedInstance = 0;
int OldCurrentSelectedInstance = 0;
std::map<int, int> Nodes = {};

int GetCurrentSelIns() {
	return CurrentSelectedInstance;
}

void Load(Rosin::StatChanger* StatChanger, Rosin::OnLoadEUU Func) {
	(*Func)(StatChanger);
}

LPSTR ToLPSTR(std::string S) {
	LPSTR cString = _strdup(S.c_str());
	try {
		S = cString;
	}
	catch (...) {
		free(cString);
	}
	return cString;
}

const char* ToChar(std::string s) {
	return s.c_str();
}

namespace Rosin {
	extern void ExtOutputText(std::string);
	StatChanger::StatChanger(const char* szWindowClass, const char* Title, OnLoadEUU Func) {
		HINSTANCE hInstance = GetModuleHandle(NULL);
		thisBas = this;
		this->WindowClass = szWindowClass;

		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = &WindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		wcex.lpszClassName = szWindowClass;
		wcex.lpszMenuName = "random";
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

		if (!RegisterClassEx(&wcex)) {
			MessageBox(NULL, "Failed to register window class", "ERROR", NULL);
			return;
		}
		this->hWindow = CreateWindow(szWindowClass, Title, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 447, 350, 0, 0, hInstance, 0);
		if (!this->hWindow) {
			MessageBox(NULL, "Failed to create window", "ERROR", NULL);
			return;
		}
		ShowWindow(this->hWindow, SW_SHOWDEFAULT);
		UpdateWindow(this->hWindow);
		if (Func != NULL) {
			//(*Func)(this);
			std::thread t(*Func, this);
			t.detach();
		}
		MSG Message;
		while (GetMessage(&Message, NULL, 0, 0)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		UnregisterClass(thisBas->WindowClass, GetModuleHandle(NULL));
	}
	StatChanger::~StatChanger() {
		DestroyWindow(this->hWindow);
		BreakWindow = true;
	}

	StatChanger* StatChanger::GetBaseInst() {
		return thisBas;
	}

	void StatChanger::SetupFonts() {
		//SendMessage(thisBas->hPlrTextBox, WM_SETFONT, (WPARAM)hFont, 0);
		//SendMessage(thisBas->hWndList, WM_SETFONT, (WPARAM)hFont, 0);
		//SendMessage(thisBas->hWndOutputGroup, WM_SETFONT, (WPARAM)hFont, 0);
		//thisBas->OutputText(std::to_string(GetLastError()));
	}

	void StatChanger::AddButton(HWND Form, SizeA s, PositionA p, std::string name, OnClickA clicked) {
		ButtonAmount++;
		HWND hButton = CreateWindowEx(WS_EX_WINDOWEDGE, TEXT("BUTTON"), TEXT(name.c_str()),
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, p.X, p.Y, s.X, s.Y, Form, (HMENU)ButtonAmount, (HINSTANCE)GetWindowLong(Form, GWL_HINSTANCE), NULL);
		SendMessage(hButton, WM_SETFONT, (WPARAM)Font, 0);
		ButtonA NewButton = ButtonA{
			clicked,
			hButton,
			ButtonAmount
		};
		this->Buttons[ButtonAmount] = NewButton;
	}

	void StatChanger::AddTextBox(HWND Form, SizeA s, PositionA p, std::string startingText, std::string StringId) {
		HWND hTextBox = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT(startingText.c_str()),
			WS_CHILD | WS_VISIBLE, p.X, p.Y, s.X,
			s.Y, Form, NULL, NULL, NULL);
		SendMessage(hTextBox, WM_SETFONT, (WPARAM)Font, 0);

		TextBoxA newTextBox = TextBoxA{
			hTextBox,
			StringId
		};
		TextBoxes.insert(std::pair<std::string, TextBoxA>(StringId, newTextBox));
	}

	void StatChanger::AddTextLabel(HWND Form, SizeA s, PositionA p, std::string name) {
		LabelAmount++;
		HWND hLabel = CreateWindowEx(WS_EX_WINDOWEDGE, TEXT("STATIC"), TEXT(name.c_str()),
			WS_TABSTOP | WS_VISIBLE | WS_CHILD, p.X, p.Y, s.X, s.Y, Form, (HMENU)ButtonAmount, (HINSTANCE)GetWindowLong(Form, GWL_HINSTANCE), NULL);
		SendMessage(hLabel, WM_SETFONT, (WPARAM)Font, 0);
		TextLabel NewLabel = TextLabel{
			hLabel,
			LabelAmount
		};
		this->TextLabels[LabelAmount] = NewLabel;
	}

	std::string StatChanger::GetStringById(std::string BoxId) {
		if (TextBoxes.find(BoxId) != TextBoxes.end()) {
			char buffer[1024];
			GetWindowText(TextBoxes.at(BoxId).hTextBox, buffer, 1024);
			std::string str(buffer);
			return str;
		}
		return "";
	}
	
	namespace Roblox {
		extern int GetLocalPlayer(int);
		extern int GetPlayers();
		extern int FindFirstChild(int, std::string);
		extern int FindFirstChild_PartOf(int, std::string);
		extern const char* GetClass(int);
		extern std::string* GetName(int);
		extern void SetIntValue(int, int);
		extern std::vector<DWORD> GetChildren(int);
		extern int GetDataModel();
		extern int GetParent(int);
		extern void SetStringValue(int, std::string);
		extern void SetBoolValue(int BoolValue, bool Val);
		extern void SetNumberValue(int NumberValue, __int64 Val);
		extern int GetService(std::string Name);
		extern void RemoveInstance(int);
		extern std::string GetFullName(int);
		
	}

	void doStatChange() {
		int stat = CurrentSelectedInstance;
		std::string StatVal = thisBas->GetStringById("STATVAL_BOX");
		if (strcmp(Roblox::GetClass(stat), "IntValue") == 0) {
			signed int val = std::atof(StatVal.c_str());
			if (val > MAXINT32) val = MAXINT32;
			Roblox::SetIntValue(stat, val);
			int OldParent = Roblox::GetParent(stat);
			int kek = Roblox::GetParent(OldParent);
			Roblox::rSetParent(stat, kek);
			Roblox::rSetParent(stat, OldParent);
		}
		else if (strcmp(Roblox::GetClass(stat), "StringValue") == 0) {
			Roblox::SetStringValue(stat, StatVal);
			int OldParent = Roblox::GetParent(stat);
			int kek = Roblox::GetParent(OldParent);
			Roblox::rSetParent(stat, kek);
			Roblox::rSetParent(stat, OldParent);
		}
		else if (strcmp(Roblox::GetClass(stat), "BoolValue") == 0) {
			bool Val = false;
			if (StatVal == "false" || StatVal == "0" || StatVal == "f")  Val = false;
			else if (StatVal == "true" || StatVal == "1" || StatVal == "t") Val = true;
			Roblox::SetBoolValue(stat, Val);
			int OldParent = Roblox::GetParent(stat);
			int kek = Roblox::GetParent(OldParent);
			Roblox::rSetParent(stat, kek);
			Roblox::rSetParent(stat, OldParent);
		}
		else if (strcmp(Roblox::GetClass(stat), "NumberValue") == 0) {
			*(long double*)(stat + 0x48) = (double)pow(10, *IntVal_ZeroVal);
			Roblox::Replicate(stat, (DWORD*)0x16AF740);
		}        
		else {
			MessageBox(thisBas->hWindow, "Selected Instance is not a IntValue/StringValue/BoolValue!", "Uh oh!", NULL);
		}
	}

	void* getValue() {
		int stat = CurrentSelectedInstance;
		const char* Class = Roblox::GetClass(stat);
		if (strcmp(Class, "IntValue") == 0) {
			int ret = *(IntVal_ZeroVal) ^ *(DWORD*)(stat + 72);
			return &*(DWORD*)ret;
		}
		else if (strcmp(Class, "StringValue") == 0) {
			return (std::string*)(stat + 0x48);
		}
		else if (strcmp(Class, "BoolValue") == 0) {
			return (bool*)(stat + 0x48);
		}
		else if (strcmp(Class, "NumberValue") == 0) {
			long double ret = *(IntVal_ZeroVal) ^ *(DWORD*)(stat + 72);
			return (void*)&ret;
		}
		else if(strcmp(Class, "ObjectValue") == 0) {
			return (int*)Roblox::GetObjectValue(stat);
		}
	}


	HTREEITEM StatChanger::AddNode(std::string Text, HTREEITEM Root, DWORD lParam = NULL) {
		NodeId++;
		TVINSERTSTRUCT tvis = { 0 };
		tvis.item.mask = TVIF_TEXT | TVIF_PARAM;
		tvis.item.pszText = ToLPSTR(Text);
		tvis.hInsertAfter = TVI_LAST;
		tvis.hParent = Root;
		tvis.item.lParam = (LPARAM)(int)NodeId;
		Nodes.insert(std::pair<int, int>(NodeId, (int)lParam));
		
		HTREEITEM Item = reinterpret_cast<HTREEITEM>(SendMessage(hTreeView, TVM_INSERTITEM, 0, reinterpret_cast<LPARAM>(&tvis)));
		if ((int)lParam == CurrentSelectedInstance) {
			TreeView_SelectItem(hTreeView, Item);
		}
		return Item;
	}


	void StatChanger::RecursePlayer(int Parent, HTREEITEM CurrentNode) {
		std::string n;
		std::vector<DWORD> Children = Roblox::GetChildren(Parent);
		for (size_t i = 0; i < Children.size(); i++) {
			DWORD Child = Children[i];
			n = *Roblox::GetName(Child);
			n += " [";
			n += Roblox::GetClass(Child);
			n += "]";
			HTREEITEM ChildNode = AddNode(n, CurrentNode, (DWORD)Child);
			RecursePlayer(Child, ChildNode);
		}
	}

	void StatChanger::RefreshTreeView() {
		Nodes.clear();
		NodeId = 0;

		std::map<std::string, bool> asd = {
			{"Workspace",true},
			{"Lighting",true},
			{"Players",true},
			{"ReplicatedStorage",true},
			{"StarterGui",true},
			{"InsertService",true}
		};

		TreeView_DeleteAllItems(hTreeView);
		std::string n = "DataModel [";
		n += Roblox::GetClass(Roblox::GetDataModel());
		n += "]";
		HTREEITEM hRootItem = AddNode(n, TVI_ROOT, (DWORD)Roblox::GetDataModel());
		int Players = Roblox::GetDataModel();
		std::vector<DWORD> Plrs = Roblox::GetChildren(Players);
		for (size_t i = 0; i < Plrs.size(); i++) {
			int Player = Plrs[i];
			std::string Class = Roblox::GetClass(Player);
			HTREEITEM CurrentNode;
			if (asd.find(Class) != asd.end() && asd[Class] == true) {
				n = *Roblox::GetName(Player);
				n += " [";
				n += Roblox::GetClass(Player);
				n += "]";
				CurrentNode = AddNode(n, hRootItem, (DWORD)Player);
				RecursePlayer(Player, CurrentNode);
			}
		}
		TreeView_Expand(thisBas->hTreeView, hRootItem, TVE_EXPAND);
	}


	LRESULT CALLBACK StatChanger::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		int Error;
		int ID;
		std::thread t;
		switch (uMsg) {
		case WM_CREATE: {
			thisBas->hTreeView = CreateWindowEx(WS_EX_WINDOWEDGE, WC_TREEVIEW,
				TEXT("Tree View"), WS_CHILD | WS_VISIBLE | WS_BORDER | TVS_HASLINES |
				WS_CLIPSIBLINGS | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS, 10, 10, 230, 300, hWnd, (HMENU)WM_TREEVIEWA, GetModuleHandle(NULL), NULL);
			thisBas->RefreshTreeView();
			thisBas->AddButton(hWnd, { 85, 25 }, { 245, 10 }, "Refresh", []()->void {
				thisBas->RefreshTreeView();
			});
			thisBas->AddButton(hWnd, { 85, 25 }, { 340, 10 }, "Change Val", []()->void {
				doStatChange();
			});
			thisBas->AddTextBox(hWnd, { 180, 25 }, { 245, 45 }, "New Value Here", "STATVAL_BOX");
			thisBas->AddButton(hWnd, { 85, 25 }, { 245, 80 }, "Remove", []()->void {
				//734110
				//((void(__thiscall*)(int ths))0x4EF630)(CurrentSelectedInstance);
				Roblox::RemoveInstance(CurrentSelectedInstance);
				//Roblox::rSetParent(CurrentSelectedInstance, 0);
			});
			thisBas->AddButton(hWnd, { 85, 25 }, { 340, 80 }, "Move", []()->void {
				if (OldCurrentSelectedInstance != 0) {
					Roblox::rSetParent(CurrentSelectedInstance, OldCurrentSelectedInstance);
					//Roblox::rSetParent(OldCurrentSelectedInstance, CurrentSelectedInstance);
				}
			});
			thisBas->AddButton(hWnd, { 85, 26 }, { 245, 115 }, "Get Value", []()->void {
				if (strcmp(Roblox::GetClass(CurrentSelectedInstance), "IntValue") == 0) {
					int Val = (int)getValue();
					ExtOutputText(std::to_string(Val));
				}
				else if (strcmp(Roblox::GetClass(CurrentSelectedInstance), "StringValue") == 0) {
					std::string Val = *((std::string*)getValue());
					ExtOutputText((Val));
				}
				else if (strcmp(Roblox::GetClass(CurrentSelectedInstance), "BoolValue") == 0) {
					bool asd = *(bool*)getValue();
					ExtOutputText(asd ? "true" : "false");
				}
				else if (strcmp(Roblox::GetClass(CurrentSelectedInstance), "ObjectValue") == 0) {
					int inst = (int)getValue();
					if (inst != 0) {
						ExtOutputText(Roblox::GetFullName(inst));
					}
				}
			});
			/*thisBas->AddButton(hWnd, { 85, 25 }, { 340, 115 }, "Clone Ins", []()->void {
				//__try {
					typedef int(__thiscall *CloneTDef)(int,int,int);
					CloneTDef CloneFunction = (CloneTDef)0x4E7850;
					int Clone;
					/*_asm {
						;-4 = Clone
						; VC++ __thiscall uses ecx to define as thiscall

						mov ecx, CurrentSelectedInstance
						push 3
						lea eax, [ebp - 4h];
						push eax
						mov ebx, 4E7850h
						call ebx
					}
					CloneFunction(CurrentSelectedInstance, (int)&Clone, 3);

					if (Clone != 0) {
						Roblox::rSetParent(Clone, Roblox::GetParent(Clone));
						MessageBox(NULL, "fat", "ass", NULL);
						
					}

				//}
				/*__except (EXCEPTION_EXECUTE_HANDLER) {
					Sleep(1);
				}
			});*/
			
		}
		break;
		case WM_NOTIFY:
			switch (wParam) {
			case WM_TREEVIEWA:
				switch (((LPNMHDR)lParam)->code) {
				case TVN_SELCHANGED: {
					HTREEITEM hSelectedItem = TreeView_GetSelection(thisBas->hTreeView);
					if (hSelectedItem == NULL) break;

					TCHAR buffer[1024];
					TVITEM item;
					item.hItem = hSelectedItem;
					item.mask = TVIF_TEXT | TVIF_PARAM;
					item.cchTextMax = 1024;
					item.pszText = buffer;
					if (TreeView_GetItem(thisBas->hTreeView, &item)){
						//MessageBox(NULL, item.pszText, "Test", NULL);
						int Id = item.lParam;
						if (Nodes.find(Id) != Nodes.end()) {
							OldCurrentSelectedInstance = CurrentSelectedInstance;
							CurrentSelectedInstance = Nodes[Id];
						}
					}
				}
				break;
				default: break;
				}
				break;
			}
			break;
		case WM_COMMAND:
			ID = LOWORD(wParam);
			for (auto i : thisBas->Buttons) {
				if (i.first == ID) {
					t = std::thread(*i.second.Click);
					t.detach();
				}
			}
			break;
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_SETFOCUS:
			uMsg = WM_KILLFOCUS;
			break;
		default:
			break;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}
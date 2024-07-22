#ifndef WINLUA_INCLUDED
#define WINLUA_INCLUDED
#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <codecvt>
#include <Richedit.h>
#include <CommCtrl.h>

#pragma comment(lib, "Comctl32.lib")
#pragma comment(linker, "\"/manifestdependency:type='win32' \
					   name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
					   processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

bool loaded = false; //only set 1 time to load 2 libararys (Richedit, CommonControls)


#define BeginUltra(s) ((void)0) //I had virtual protect on this, you can put whatever you want here, it begins and ends at all functions.
#define VEnd() ((void)0)

HINSTANCE hInstance = GetModuleHandleA(NULL);

#ifndef XVALUE_INCLUDED
#include "xvalue.h"
#endif

#ifndef WINLUA_UTIL_INCLUDED
#include "winlua_util.h"
#endif

#include "WinLua_Scintilla.h"

extern "C" {
#include "Lua\lua.h"
#include "Lua\lobject.h"
#include "Lua\lvm.h"
#include "Lua\lauxlib.h"
#include "Lua\lualib.h"
#include "Lua\lobject.h"
}

#pragma region Data Structures
struct Form_Event {
	int event_key;
	int function;
	lua_State* env;
};

struct Form_Control {
	std::vector<Form_Event*> events;
	HWND window;
	bool checkbox;
	HMENU menu;
	bool ismenu;
	bool basemenu;
	bool ismenuevent;
	bool istextbox;
	bool isscintilla;
	bool cmdbox;
	std::string cmdbox_ret;
	std::string cmdbox_last;
	std::string last_who_gives_a_shit;
};

struct Form {
	lua_State* message_handler;
	HWND window;
	int form_key;
	bool quit;
	std::vector<Form_Control*> controls;
};
#pragma endregion

#pragma region Storage
ValueX* var = new ValueX();

std::vector<Form*> Forms;
#pragma endregion

Form* find_form(HWND window) {
	for (int i = 0; i < Forms.size(); i++)
		if (Forms.at(i)->window == window)
			return Forms.at(i);
}

Form* find_form(Form_Control* control) {
	for (int i = 0; i < Forms.size(); i++)
		for (int i2 = 0; i2 < Forms.at(i)->controls.size(); i2++)
			if (Forms.at(i)->controls.at(i2) == control)
				return Forms.at(i);
}

Form_Control* find_control(HWND hwnd) {
	for (int fi = 0; fi < Forms.size(); fi++) {
		Form* form = Forms.at(fi);
		for (int i = 0; i < form->controls.size(); i++) {
			Form_Control* control = form->controls.at(i);
			if (control->window == hwnd) {
				return control;
			}
		}
	}
}

void Remove_All_Forms() {
	for (int i = 0; i < Forms.size(); i++) {
		Forms[i]->quit = true;
	}
}

Form_Control* find_control(int event_key) {
	for (int fi = 0; fi < Forms.size(); fi++) {
		Form* form = Forms.at(fi);
		for (int i = 0; i < form->controls.size(); i++) {
			Form_Control* control = form->controls.at(i);
			for (int i2 = 0; i2 < control->events.size(); i2++)
				if (control->events.at(i2)->event_key == event_key)
					return control;
		}
	}
}

Form_Event* find_event(int event_key) {
	for (int fi = 0; fi < Forms.size(); fi++) {
		Form* form = Forms.at(fi);
		for (int i = 0; i < form->controls.size(); i++) {
			Form_Control* control = form->controls.at(i);
			for (int i2 = 0; i2 < control->events.size(); i2++)
				if (control->events.at(i2)->event_key == event_key)
					return control->events.at(i2);
		}
	}
}

#pragma region Implements
int impl_MessageBox_Show(lua_State* lua_env) {
	try {
		if (lua_gettop(lua_env) == 3) {
			int button;
			const char* body, *title;
			if (lua_type(lua_env, 1) == LUA_TNUMBER)
				button = lua_tonumber(lua_env, 1);
			if (lua_type(lua_env, 2) == LUA_TSTRING && lua_type(lua_env, 3) == LUA_TSTRING)
				body = lua_tostring(lua_env, 2); title = lua_tostring(lua_env, 3);

			lua_pushnumber(lua_env, MessageBoxA(NULL, body, title, button));
			return 1;
		}
	}
	catch (...) {
		return 0;
	}
}

/* MessageBox API */

void MessageBoxLibrary(lua_State* lua_env) {
	BeginUltra("msgboxlib");
	lua_createtable(lua_env, 0, 0);
	{
		lua_pushcfunction(lua_env, impl_MessageBox_Show);
		lua_setfield(lua_env, -2, "Show");
	}
	lua_setglobal(lua_env, "MessageBox");

	lua_createtable(lua_env, 0, 0);
	{
		lua_pushnumber(lua_env, MB_OK);
		lua_setfield(lua_env, -2, "Ok");

		lua_pushnumber(lua_env, MB_OKCANCEL);
		lua_setfield(lua_env, -2, "OkCancel");

		lua_pushnumber(lua_env, MB_ABORTRETRYIGNORE);
		lua_setfield(lua_env, -2, "AbortRetryIgnore");

		lua_pushnumber(lua_env, MB_YESNO);
		lua_setfield(lua_env, -2, "YesNo");

		lua_pushnumber(lua_env, MB_YESNOCANCEL);
		lua_setfield(lua_env, -2, "YesNoCancel");

		lua_pushnumber(lua_env, MB_CANCELTRYCONTINUE);
		lua_setfield(lua_env, -2, "CancelTryContinue");

		lua_pushnumber(lua_env, MB_RETRYCANCEL);
		lua_setfield(lua_env, -2, "RetryCancel");
	}
	lua_setglobal(lua_env, "Buttons");

	lua_createtable(lua_env, 0, 0);
	{
		lua_pushnumber(lua_env, IDOK);
		lua_setfield(lua_env, -2, "Ok");

		lua_pushnumber(lua_env, IDCANCEL);
		lua_setfield(lua_env, -2, "Cancel");

		lua_pushnumber(lua_env, IDABORT);
		lua_setfield(lua_env, -2, "Abort");

		lua_pushnumber(lua_env, IDRETRY);
		lua_setfield(lua_env, -2, "Retry");

		lua_pushnumber(lua_env, IDIGNORE);
		lua_setfield(lua_env, -2, "Ignore");

		lua_pushnumber(lua_env, IDYES);
		lua_setfield(lua_env, -2, "Yes");

		lua_pushnumber(lua_env, IDNO);
		lua_setfield(lua_env, -2, "No");

		lua_pushnumber(lua_env, IDTRYAGAIN);
		lua_setfield(lua_env, -2, "Try");

		lua_pushnumber(lua_env, IDCONTINUE);
		lua_setfield(lua_env, -2, "Continue");
	}
	lua_setglobal(lua_env, "ButtonResults");
	VEnd();
}

/* WindowState API */

int WindowState(lua_State* lua_env) {
	BeginUltra("window_state");
	if (lua_gettop(lua_env) == 2) {
		bool isnum;
		bool boolean;
		int num;
		Form* fr = new Form();

		if (lua_type(lua_env, 1) == LUA_TLIGHTUSERDATA) {
			fr = (Form*)lua_touserdata(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
		}

		if (lua_type(lua_env, 2) == LUA_TSTRING) {
			isnum = true;
			num = lua_tonumber(lua_env, 2);
		}
		else if (lua_type(lua_env, 2) == LUA_TBOOLEAN) {
			isnum = false;
			boolean = lua_toboolean(lua_env, 2);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 2");
			return 1;
		}

		ShowWindow(fr->window, (isnum ? num : boolean));
	}
	else return 0;
	VEnd();
}

void WindowStateLibrary(lua_State* lua_env) {
	BeginUltra("statelib2");
	lua_createtable(lua_env, 0, 0);
	{
		lua_pushnumber(lua_env, SW_HIDE);
		lua_setfield(lua_env, -2, "Hide");

		lua_pushnumber(lua_env, SW_SHOW);
		lua_setfield(lua_env, -2, "Show");

		lua_pushnumber(lua_env, SW_MAXIMIZE);
		lua_setfield(lua_env, -2, "Maximize");

		lua_pushnumber(lua_env, SW_MINIMIZE);
		lua_setfield(lua_env, -2, "Minimize");

		lua_pushnumber(lua_env, SW_NORMAL);
		lua_setfield(lua_env, -2, "Normal");
	}
	lua_setglobal(lua_env, "WindowState");

	lua_pushcfunction(lua_env, WindowState);
	lua_setglobal(lua_env, "ShowWindow");
	VEnd();
}

/* Form API */

int createMenu(lua_State* lua_env) {
	BeginUltra("createmenu");
	if (lua_gettop(lua_env) == 0) {
		HMENU menu = CreateMenu();
		Form_Control* control = new Form_Control();

		control->ismenu = true;
		control->basemenu = true;
		control->menu = menu;
		lua_pushlightuserdata(lua_env, (void*)control);
		return 1;
	}
	if (lua_gettop(lua_env) == 1) {
		HMENU menu = CreateMenu();
		Form_Control* control = new Form_Control();
		Form* form = new Form();

		if (lua_type(lua_env, 1) == LUA_TLIGHTUSERDATA) {
			form = (Form*)lua_touserdata(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		control->ismenu = true;
		control->menu = menu;
		control->basemenu = false;
		form->controls.push_back(control);
		lua_pushlightuserdata(lua_env, (void*)control);
		return 1;
	}
	return 0;
	VEnd();
}

int createFont(lua_State* lua_env) {
	BeginUltra("createfont");
	if (lua_gettop(lua_env) == 2) {
		int size;
		const char* font_name;

		if (lua_type(lua_env, 1) == LUA_TNUMBER) {
			size = lua_tonumber(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		if (lua_type(lua_env, 2) == LUA_TSTRING) {
			font_name = lua_tostring(lua_env, 2);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 2");
			return 1;
		}

		HFONT font = CreateFontA(size, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, font_name);
		lua_pushlightuserdata(lua_env, (void*)font);
		return 1;
	}
	else if (lua_gettop(lua_env) == 0) {
		HFONT font = CreateFontA(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Courier New");
		lua_pushlightuserdata(lua_env, (void*)font);
		return 1;
	}
	return 0;
	VEnd();
}

int appendMenu(lua_State* lua_env) {
	BeginUltra("appendmneu");
	if (lua_gettop(lua_env) == 2) {
		Form_Control* menu = new Form_Control();
		const char* text;

		if (lua_type(lua_env, 1) == LUA_TLIGHTUSERDATA) {
			menu = (Form_Control*)lua_touserdata(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		if (lua_type(lua_env, 2) == LUA_TSTRING) {
			text = lua_tostring(lua_env, 2);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 2");
			return 1;
		}

		if (!menu->ismenu || menu->basemenu) {
			lua_pushstring(lua_env, "Invalid menu");
			return 1;
		}

		Form_Event* event = new Form_Event();
		event->event_key = gen_random_event_key();
		AppendMenuA(menu->menu, MF_STRING, (UINT_PTR)event->event_key, text);
		menu->events.push_back(event);
		lua_pushlightuserdata(lua_env, (void*)event);
		return 1;
	}
	else if (lua_gettop(lua_env) == 3) {
		Form_Control* menu = new Form_Control();
		Form_Control* child_menu = new Form_Control();
		const char* text;

		if (lua_type(lua_env, 1) == LUA_TLIGHTUSERDATA) {
			menu = (Form_Control*)lua_touserdata(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		if (lua_type(lua_env, 2) == LUA_TLIGHTUSERDATA) {
			child_menu = (Form_Control*)lua_touserdata(lua_env, 2);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 2");
			return 1;
		}

		if (lua_type(lua_env, 3) == LUA_TSTRING) {
			text = lua_tostring(lua_env, 3);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 2");
			return 1;
		}

		if (!menu->ismenu) {
			lua_pushstring(lua_env, "Invalid menu");
			return 1;
		}

		if (!child_menu->ismenu || child_menu->basemenu) {
			lua_pushstring(lua_env, "Invalid child menu");
			return 1;
		}

		AppendMenuA(menu->menu, MF_POPUP, (UINT_PTR)child_menu->menu, text);
		return 0;
	}
	return 0;
	VEnd();
}

int createWindow(lua_State* lua_env) {
	BeginUltra("createwindow");
	if (lua_gettop(lua_env) >= 8) {
		std::string class_name, title;
		int size_x, size_y, offset_x, offset_y;
		std::vector<int> flags;
		Form* form = new Form();
		Form_Control* control = new Form_Control();

		if (lua_type(lua_env, 1) == LUA_TSTRING) {
			class_name = lua_tostring(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		if (lua_type(lua_env, 2) == LUA_TSTRING) {
			title = lua_tostring(lua_env, 2);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 2");
			return 1;
		}

		if (lua_type(lua_env, 3) == LUA_TNUMBER) {
			size_x = lua_tonumber(lua_env, 3);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 3");
			return 1;
		}

		if (lua_type(lua_env, 4) == LUA_TNUMBER) {
			size_y = lua_tonumber(lua_env, 4);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 4");
			return 1;
		}

		if (lua_type(lua_env, 5) == LUA_TNUMBER) {
			offset_x = lua_tonumber(lua_env, 5);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 5");
			return 1;
		}

		if (lua_type(lua_env, 6) == LUA_TNUMBER) {
			offset_y = lua_tonumber(lua_env, 6);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 6");
			return 1;
		}

		if (lua_type(lua_env, 7) == LUA_TLIGHTUSERDATA) {
			form = (Form*)lua_touserdata(lua_env, 7);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 7");
			return 1;
		}

		if (lua_gettop(lua_env) > 8) {
			for (int i = 8; i <= lua_gettop(lua_env); i++) {
				if (lua_type(lua_env, i) == LUA_TNUMBER)
					flags.push_back(lua_tonumber(lua_env, i));
				else {
					std::string tmp = "Invalid flag @ ";
					tmp += std::to_string(i).c_str();
					tmp += " (not a number type)";
					lua_pushstring(lua_env, tmp.c_str());
					return 1;
				}
			}
		}

		int flgs;

		for (int i = 0; i < flags.size(); i++) {
			if (i > 0)
				flgs |= flags.at(i);
			else
				flgs = flags.at(i);
		}

		if (class_name == "LAEBEL")
			flgs |= SS_SIMPLE;

		control->checkbox = false;

		for (int i = 0; i < flags.size(); i++) {
			if (flags.at(i) == BS_CHECKBOX)
				control->checkbox = true;
		}

		Form_Event* event = new Form_Event();
		event->event_key = gen_random_event_key();
		control->events.push_back(event);
		control->ismenu = false;
		if (class_name.c_str() == RICHEDIT_CLASSA) {
			control->istextbox = true; control->isscintilla = false;
		}
		/*if (util->lowercase(class_name).c_str() == "edit") {
			control->istextbox = true; control->isscintilla = false;
		}*/
		if (class_name == "Scintilla") {
			control->istextbox = true; control->isscintilla = true;
		}
		if (class_name == "LAEBEL") {
			control->window = CreateWindowExA(NULL, "STATIC", title.c_str(), flgs, offset_x, offset_y, size_x, size_y, form->window, (HMENU)event->event_key, hInstance, NULL);
		}
		if (class_name != "Scintilla" && class_name != "LAEBEL") {
			control->window = CreateWindowExA(NULL, class_name.c_str(), title.c_str(), flgs, offset_x, offset_y, size_x, size_y, form->window, (HMENU)event->event_key, hInstance, NULL);
		}
		if (class_name == "Scintilla") {
			control->window = CreateWindowExA(NULL, class_name.c_str(), title.c_str(), flgs, offset_x, offset_y, size_x, size_y, form->window, (HMENU)event->event_key, hInstance, NULL);
			ScintillaFunctions::InitializeScintilla(control->window);
		}
		lua_pushlightuserdata(lua_env, (void*)event);
		form->controls.push_back(control);
		return 1;
	}
	return 0;
	VEnd();
}

struct dual {
	HWND hwnd;
	WNDPROC CommandExecutionProcHandler;
};

std::vector<dual*> duals;

LRESULT CALLBACK CommandExecutionProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	BeginUltra("cmdexe");
	dual* dua = new dual();

	for (int i = 0; i < duals.size(); i++) {
		if (duals.at(i)->hwnd == hwnd)
			dua = duals.at(i);
	}

	switch (msg) {
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_RETURN:
			/*
			this is terrible code, it is messy and it is not
			code that you should keep/use. I used this for a
			very simple system to execute commands.
			
			Not too sure what does what or how it does it, I
			used and created this a long time ago.
			*/
			Form_Control* control = find_control(hwnd);
			int len = GetWindowTextLength(control->window) + 1;
			char* buffer = new char[len];
			GetWindowTextA(control->window, (LPSTR)buffer, len);
			if (buffer != control->last_who_gives_a_shit.c_str()) {
				std::string res = execute_command(buffer);
				control->cmdbox_ret = res;
				control->cmdbox_last = buffer;
				SendMessageA(control->window, WM_SETTEXT, (WPARAM)buffer, 0);
				control->last_who_gives_a_shit = buffer;
			}
			break;
		}
	}

	return CallWindowProc(dua->CommandExecutionProcHandler, hwnd, msg, wParam, lParam);
	VEnd();
}

LRESULT CALLBACK BaseWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BeginUltra("basewndproc");
	switch (message) {
	case WM_COMMAND: {
		int cmd = LOWORD(wParam);
		for (int i = 0; i < event_keys.size(); i++) {
			if (cmd == event_keys.at(i)) {
				Form_Control* control = find_control(event_keys.at(i));
				Form_Event* event = find_event(event_keys.at(i));
				if (control->ismenuevent) {
					lua_rawgeti(event->env, LUA_REGISTRYINDEX, event->function);
					lua_call(event->env, 0, 0);
				}
				else {
					if (control->checkbox == true) {
						bool checked = SendMessage(control->window, BM_GETCHECK, 0, 0);
						SendMessage(control->window, BM_SETCHECK, (checked ? BST_UNCHECKED : BST_CHECKED), 0);

						if (event->function) {
							lua_rawgeti(event->env, LUA_REGISTRYINDEX, event->function);
							lua_call(event->env, 0, 0);
						}
					}
					if (control->checkbox == false && event->function && control->istextbox == false) {
						lua_rawgeti(event->env, LUA_REGISTRYINDEX, event->function);
						lua_call(event->env, 0, 0);
					}
				}
			}
		}
		break;
	}

	case WM_NOTIFY: {
		/*
		Also, code that should probably be developed or removed.
		This code really isnt harmful but it is used for scintilla,
		which I never took the time to get working.
		*/
		Form* form = find_form(hwnd);
		for (int i = 0; i < form->controls.size(); i++) {
			if (form->controls.at(i)->istextbox) {
				if (form->controls.at(i)->isscintilla) {
					SCNotification* n = (SCNotification*)lParam;
					switch (n->linesAdded) {
					default:
						ScintillaFunctions::ChangeMarginWidth(form->controls.at(i)->window);
						break;
					}
				}
			}
		}
	}
	}

	if (message == WM_CLOSE || message == WM_QUIT) {
		Form* form = find_form(hwnd);
		form->quit = true;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
	VEnd();
}

int registerForm(lua_State* lua_env) {
	BeginUltra("register");
	if (lua_gettop(lua_env) >= 8) {
		Form* form = new Form();
		std::string class_name, title;
		int size_x, size_y, offset_x, offset_y;
		std::vector<int> flags;
		Form_Control* menu = new Form_Control();

		if (lua_type(lua_env, 1) == LUA_TSTRING) {
			class_name = lua_tostring(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		if (lua_type(lua_env, 2) == LUA_TSTRING) {
			title = lua_tostring(lua_env, 2);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 2");
			return 1;
		}

		if (lua_type(lua_env, 3) == LUA_TNUMBER) {
			size_x = lua_tonumber(lua_env, 3);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 3");
			return 1;
		}

		if (lua_type(lua_env, 4) == LUA_TNUMBER) {
			size_y = lua_tonumber(lua_env, 4);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 4");
			return 1;
		}

		if (lua_type(lua_env, 5) == LUA_TNUMBER) {
			offset_x = lua_tonumber(lua_env, 5);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 5");
			return 1;
		}

		if (lua_type(lua_env, 6) == LUA_TNUMBER) {
			offset_y = lua_tonumber(lua_env, 6);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 6");
			return 1;
		}

		if (lua_type(lua_env, 7) == LUA_TLIGHTUSERDATA) {
			menu = (Form_Control*)lua_touserdata(lua_env, 7);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 7");
			return 1;
		}

		if (lua_gettop(lua_env) > 8) {
			for (int i = 8; i <= lua_gettop(lua_env); i++) {
				if (lua_type(lua_env, i) == LUA_TNUMBER)
					flags.push_back(lua_tonumber(lua_env, i));
				else {
					std::string tmp = "Invalid flag @ ";
					tmp += std::to_string(i).c_str();
					tmp += " (not a number type)";
					lua_pushstring(lua_env, tmp.c_str());
					return 1;
				}
			}
		}


		UnregisterClassA(class_name.c_str(), hInstance);

		WNDCLASSEXA wc;

		ZeroMemory(&wc, sizeof(WNDCLASSEXA));
		wc.cbSize = sizeof(WNDCLASSEXA);
		wc.lpfnWndProc = BaseWndProc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.hInstance = hInstance;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.lpszClassName = class_name.c_str();
		RegisterClassExA(&wc);

		int flgs;
		bool topmost = false;

		for (int i = 0; i < flags.size(); i++) {
			int flag = flags.at(i);

			if (flag == WS_EX_TOPMOST) {
				topmost = true;
			}

			if (i == 0)
				flgs = flag;
			else
				flgs |= flag;
		}

		if (!menu->ismenu || !menu->basemenu) {
			lua_pushstring(lua_env, "Invalid menu");
			return 1;
		}

		HWND window;
		if (!topmost) {
			//window = CreateWindowExA(NULL, class_name.c_str(), (LPCSTR)util->s2ws(title).c_str(), flgs, offset_x, offset_y, size_x, size_y, NULL, menu->menu, hInstance, NULL);
		}
		else {
			//window = CreateWindowExA(WS_EX_TOPMOST, class_name.c_str(), (LPCSTR)util->s2ws(title).c_str(), flgs, offset_x, offset_y, size_x, size_y, NULL, menu->menu, hInstance, NULL);
		}

		form->form_key = gen_random_form_key();
		form->window = window;
		form->quit = false;
		Forms.push_back(form);
		lua_pushlightuserdata(lua_env, (void*)form);
		return 1;
	}
	return 0;
	VEnd();
}

int setChecked(lua_State* lua_env) {
	BeginUltra("setcec");
	if (lua_gettop(lua_env) == 2) {
		Form_Control* control = new Form_Control();
		Form_Event* event = new Form_Event();
		bool check;

		if (lua_type(lua_env, 1) == LUA_TLIGHTUSERDATA) {
			event = (Form_Event*)lua_touserdata(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		if (lua_type(lua_env, 2) == LUA_TBOOLEAN) {
			check = lua_toboolean(lua_env, 2);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 2");
		}

		control = find_control(event->event_key);

		SendMessage(control->window, BM_SETCHECK, check, 0);
		return 0;
	}
	return 0;
	VEnd();
}

int getChecked(lua_State* lua_env) {
	BeginUltra("getceheck");
	if (lua_gettop(lua_env) == 1) {
		Form_Control* control = new Form_Control();
		Form_Event* event = new Form_Event();

		if (lua_type(lua_env, 1) == LUA_TLIGHTUSERDATA) {
			event = (Form_Event*)lua_touserdata(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		control = find_control(event->event_key);

		bool checked = SendMessage(control->window, BM_GETCHECK, 0, 0);
		lua_pushboolean(lua_env, checked);
		return 1;
	}
	return 0;
	VEnd();
}

int getText(lua_State* lua_env) {
	BeginUltra("gettex");
	if (lua_gettop(lua_env) == 1) {
		Form_Control* control = new Form_Control();
		Form_Event* event = new Form_Event();

		if (lua_type(lua_env, 1) == LUA_TLIGHTUSERDATA) {
			event = (Form_Event*)lua_touserdata(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		control = find_control(event->event_key);

		if (control->cmdbox_last != "") {
			lua_pushstring(lua_env, control->cmdbox_last.c_str());
			return 1;
		}
		else {
			int len = GetWindowTextLength(control->window) + 1;
			char* buffer = new char[len];
			GetWindowTextA(control->window, (LPSTR)buffer, len);
			lua_pushstring(lua_env, buffer);
		}
		return 1;
	}
	return 0; VEnd();
}

int setText(lua_State* lua_env) {
	BeginUltra("settex");
	if (lua_gettop(lua_env) == 2) {
		Form_Control* control = new Form_Control();
		Form_Event* event = new Form_Event();
		const char* text;

		if (lua_type(lua_env, 1) == LUA_TLIGHTUSERDATA) {
			event = (Form_Event*)lua_touserdata(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		if (lua_type(lua_env, 2) == LUA_TSTRING) {
			text = lua_tostring(lua_env, 2);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 2");
		}

		control = find_control(event->event_key);

		if (control->isscintilla)
			SendMessage(control->window, SCI_SETTEXT, (WPARAM)"", (LPARAM)text);
		else SetWindowTextA(control->window, text);
		return 0;
	}
	return 0;
	VEnd();
}

int impl_downloadurl(lua_State* L) {
	BeginUltra("downloadurl");
	if (lua_type(L, 1) == LUA_TSTRING && lua_gettop(L) == 1) {
		std::string http = lua_tostring(L, 1);
		lua_pushstring(L, "download_url function not included");//util->download_url(http.c_str()).c_str()); //this was not used, but can be used for "auto-updating UIs"
		return 1;
	}
	else return luaL_error(L, "download_url needs to have arguments (string : url)");
	VEnd();
}

int impl_writefile(lua_State* L) {
	BeginUltra("writefile");
	if (lua_type(L, 1) == LUA_TSTRING && lua_type(L, 2) == LUA_TSTRING && lua_gettop(L) == 2) {
		std::string data_1 = lua_tostring(L, 1), data_2 = lua_tostring(L, 2);
		//util->write_file(data_1, data_2); //again, not included. not used either but can be used.
		return 0;
	}
	else return luaL_error(L, "writefile needs to have arguments (string : path) and (string : data)");
	VEnd();
}

int impl_readfile(lua_State* L) {
	BeginUltra("readfile");
	if (lua_type(L, 1) == LUA_TSTRING && lua_gettop(L) == 1) {
		std::string data_1 = lua_tostring(L, 1);
		//std::string data = util->read_file(data_1);
		std::string data = ""; //this is not included ^
		lua_pushstring(L, data.c_str());
		return 1;
	}
	else return luaL_error(L, "readfile needs to have arguments (string : path)");
	VEnd();
}

int impl_loadstring(lua_State* L) { //I dont know why I did this. It is retarded.
	BeginUltra("loadstring");
	if (lua_type(L, 1) == LUA_TSTRING && lua_gettop(L) == 1) {
		std::string script = lua_tostring(L, 1);
		luaL_loadstring(L, script.c_str());
		return 1;
	}
	else return luaL_error(L, "loadstring needs to have arguments (string : script)");
	VEnd();
}

int impl_execute_cscript(lua_State* L) {
	BeginUltra("executecsrcitp");
	if (lua_type(L, 1) == LUA_TSTRING && lua_gettop(L) == 1) {
		std::string data_1 = lua_tostring(L, 1);
		//rlua->execute_cscript(data_1); //this was to parse scripts to execute for roblox
		return 0;
	}
	else return luaL_error(L, "execute_cscript needs to have arguments (string : script)");
	VEnd();
}

int impl_execute_script(lua_State* L) {
	BeginUltra("executesrcitp");
	if (lua_type(L, 1) == LUA_TSTRING && lua_gettop(L) == 1) {
		std::string data_1 = lua_tostring(L, 1);
		/* //this was used to execute lua scripts, I never got working so I dumped the project no one cared anyway.
		//I was bored and so I moved on to other games.
		rlua->execute_script(rlua->g_L, data_1, "Cobalt v4 Script");
		if (lua_type(L, -1) != LUA_TNIL) {
			if (lua_tostring(L, -1) != "") {
				return luaL_error(L, lua_tostring(L, -1));
			}
			else return 0;
		}
		else return 0;		*/
	}
	else return luaL_error(L, "execute_script needs to have arguments (string : script)");
	VEnd();
}

int impl_execute_command(lua_State* L) {
	BeginUltra("execumd");
	if (lua_type(L, 1) == LUA_TSTRING && lua_gettop(L) == 1) {
		std::string data_1 = lua_tostring(L, 1);
		std::string data = ""; //execute_command(data_1); //this is used for executing commands
		lua_pushstring(L, data.c_str());
		return 1;
	}
	else return luaL_error(L, "execute_command needs to have arguments (string : cmd)");
	VEnd();
}

int impl_open_dialog(lua_State* L) {
	BeginUltra("opendialgo");
	if (lua_type(L, 1) == LUA_TLIGHTUSERDATA && lua_gettop(L) == 1) {
		Form_Control* control = new Form_Control();
		Form_Event* event = (Form_Event*)lua_touserdata(L, 1);

		control = find_control(event->event_key);

		char filename[MAX_PATH];
		OPENFILENAMEA ofn;
		ZeroMemory(&filename, sizeof(filename));
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrFilter = "Lua Files\0*.txt;*.lua\0All Files\0*.*";
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = "Open";
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

		if (GetOpenFileNameA(&ofn)) {
			std::string data = "";//util->read_file(filename); required for actually reading the data. not implemented.
			if (control->isscintilla)
				SendMessage(control->window, SCI_SETTEXT, (WPARAM)"", (LPARAM)data.c_str());
			else SetWindowTextA(control->window, data.c_str());
		}
		return 0;
	}
	else return luaL_error(L, "opendialog needs to have arguments (lightuserdata : wnd)");
	VEnd();
}

int impl_save_dialog(lua_State* L) {
	BeginUltra("savedialg");
	if (lua_type(L, 1) == LUA_TLIGHTUSERDATA && lua_gettop(L) == 1) {
		Form_Control* control = new Form_Control();
		Form_Event* event = (Form_Event*)lua_touserdata(L, 1);

		control = find_control(event->event_key);

		char asfi[MAX_PATH];
		OPENFILENAMEA ofn;
		ZeroMemory(&asfi, sizeof(asfi));
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrFilter = "Text\0*.txt\0Lua\0*.lua\0All\0*.*";
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFile = asfi;
		ofn.lpstrTitle = "Save";
		ofn.Flags = OFN_DONTADDTORECENT;

		if (GetSaveFileNameA(&ofn)) {
			int len = GetWindowTextLength(control->window) + 1;
			char* buffer = new char[len];
			GetWindowTextA(control->window, (LPSTR)buffer, len);

			if (ofn.nFilterIndex == 1) {
				std::string last = ofn.lpstrFile;
				last += ".txt";
				//util->write_file(last, buffer);
			}
			if (ofn.nFilterIndex == 2) {
				std::string last = ofn.lpstrFile;
				last += ".lua";
				//util->write_file(last, buffer);
			}
			if (ofn.nFilterIndex == 3) {
				//util->write_file(ofn.lpstrFile, buffer); //again not implemented
			}
		}
		return 0;
	}
	else return luaL_error(L, "opendialog needs to have arguments (lightuserdata : wnd)");
	VEnd();
}

int impl_exit(lua_State* L) {
	if (lua_type(L, 1) == LUA_TNUMBER) {
		exit(lua_tonumber(L, 1));
		return 0;
	}
	else return 0;
}

int impl_unload(lua_State* L) {
	BeginUltra("unload");
	//util->close(); //this was a special function, it just unloaded everything and unloaded the module I had all of this in
	return 0;
	VEnd();
}

int _RGB(lua_State* lua_env) {
	BeginUltra("rgb");
	if (lua_gettop(lua_env) == 3) {
		int r, g, b;

		if (lua_type(lua_env, 1) == LUA_TNUMBER) {
			r = lua_tonumber(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		if (lua_type(lua_env, 2) == LUA_TNUMBER) {
			g = lua_tonumber(lua_env, 2);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 2");
			return 1;
		}

		if (lua_type(lua_env, 3) == LUA_TNUMBER) {
			b = lua_tonumber(lua_env, 3);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 3");
			return 1;
		}

		lua_pushlightuserdata(lua_env, (void*)RGB(r, g, b));
		return 1;
	}
	return 0;
	VEnd();
}

Form_Control* g_control;

int printf_handler(lua_State* lua_env) {
	BeginUltra("printfhand");
	/*while (true) {
		if (util->printf_called) {
			std::string message = util->latest_printf;
			int len = SendMessageA(g_control->window, WM_GETTEXTLENGTH, NULL, NULL);
			SendMessageA(g_control->window, EM_SETSEL, len, len);

			CHARFORMATA cfd;
			CHARFORMATA cf;
			SendMessageA(g_control->window, EM_GETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cfd);
			memcpy(&cf, &cfd, sizeof(CHARFORMATA));

			cf.cbSize = sizeof(CHARFORMATA);
			cf.dwMask = CFM_COLOR;
			cf.crTextColor = (COLORREF)RGB(0, 0, 0);
			cf.dwEffects = 0;

			SendMessageA(g_control->window, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
			SendMessageA(g_control->window, EM_REPLACESEL, FALSE, (LPARAM)message.c_str());
			SendMessageA(g_control->window, WM_VSCROLL, SB_BOTTOM, 0L);
			SendMessageA(g_control->window, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cfd);

			util->printf_called = false;
		}
		Sleep(5);
	}*/
	//I should've just hooked. Idk why I didn't lol.

	return 0;
	VEnd();
}

bool xd = false;

int redirect_printf(lua_State* lua_env) {
	BeginUltra("rederect");
	if (lua_gettop(lua_env) == 1) {
		Form_Event* event = new Form_Event();

		if (lua_type(lua_env, 1) == LUA_TLIGHTUSERDATA) {
			event = (Form_Event*)lua_touserdata(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		g_control = find_control(event->event_key);

		if (!xd) {
			//util->thread(printf_handler);
			xd = true;
		}
		return 0;
	}
	else
		return 0;
	VEnd();
}

int printmsg(lua_State* lua_env) {
	BeginUltra("printmsg");
	if (lua_gettop(lua_env) >= 3) {
		std::string message;
		Form_Control* control = new Form_Control();
		COLORREF col;
		Form_Event* event = new Form_Event();

		if (lua_type(lua_env, 1) == LUA_TLIGHTUSERDATA) {
			col = (COLORREF)lua_touserdata(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		if (lua_type(lua_env, 2) == LUA_TLIGHTUSERDATA) {
			event = (Form_Event*)lua_touserdata(lua_env, 2);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 2");
			return 1;
		}

		if (lua_type(lua_env, 3) == LUA_TSTRING) {
			message = lua_tostring(lua_env, 3);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 3");
			return 1;
		}

		for (int i = 4; i <= lua_gettop(lua_env); i++) {
			if (lua_type(lua_env, i) == LUA_TSTRING)
				message += lua_tostring(lua_env, i);
			else {
				std::string msg = "Invalid argument ";
				msg += std::to_string(i).c_str();
				lua_pushstring(lua_env, msg.c_str());
				return 1;
			}
		}

		control = find_control(event->event_key);

		int len = SendMessageA(control->window, WM_GETTEXTLENGTH, NULL, NULL);
		SendMessageA(control->window, EM_SETSEL, len, len);

		CHARFORMATA cfd;
		CHARFORMATA cf;
		SendMessageA(control->window, EM_GETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cfd);
		memcpy(&cf, &cfd, sizeof(CHARFORMATA));

		cf.cbSize = sizeof(CHARFORMATA);
		cf.dwMask = CFM_COLOR;
		cf.crTextColor = col;
		cf.dwEffects = 0;

		SendMessageA(control->window, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
		SendMessageA(control->window, EM_REPLACESEL, FALSE, (LPARAM)message.c_str());
		SendMessageA(control->window, WM_VSCROLL, SB_BOTTOM, 0L);
		SendMessageA(control->window, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cfd);
		return 0;
	}
	else if (lua_gettop(lua_env) == 2) {
		std::string message;
		Form_Control* control = new Form_Control();
		Form_Event* event = new Form_Event();

		if (lua_type(lua_env, 1) == LUA_TLIGHTUSERDATA) {
			event = (Form_Event*)lua_touserdata(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		if (lua_type(lua_env, 2) == LUA_TSTRING) {
			message = lua_tostring(lua_env, 2);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 2");
			return 1;
		}

		control = find_control(event->event_key);

		int len = SendMessageA(control->window, WM_GETTEXTLENGTH, NULL, NULL);
		SendMessageA(control->window, EM_SETSEL, len, len);

		CHARFORMATA cfd;
		CHARFORMATA cf;
		SendMessageA(control->window, EM_GETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cfd);
		memcpy(&cf, &cfd, sizeof(CHARFORMATA));

		cf.cbSize = sizeof(CHARFORMATA);
		cf.dwMask = CFM_COLOR;
		cf.crTextColor = RGB(0, 0, 0);
		cf.dwEffects = 0;

		SendMessageA(control->window, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
		SendMessageA(control->window, EM_REPLACESEL, FALSE, (LPARAM)message.c_str());
		SendMessageA(control->window, WM_VSCROLL, SB_BOTTOM, 0L);
		SendMessageA(control->window, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cfd);
		return 0;
	}
	return 0;
	VEnd();
}

int hook_cmd_box(lua_State* lua_env) {
	BeginUltra("hookcmdbox");
	if (lua_gettop(lua_env) == 1) {
		Form_Control* control = new Form_Control();
		Form_Event* event = new Form_Event();

		if (lua_type(lua_env, 1) == LUA_TLIGHTUSERDATA) {
			event = (Form_Event*)lua_touserdata(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		control = find_control(event->event_key);

		control->cmdbox = true;
		dual* dua = new dual();
		dua->hwnd = control->window;
		dua->CommandExecutionProcHandler = (WNDPROC)SetWindowLongPtr(control->window, GWLP_WNDPROC, (LONG_PTR)CommandExecutionProc);
		duals.push_back(dua);
		return 0;
	}
	return 0;
	VEnd();
}

int get_cmd_box_ret(lua_State* lua_env) {
	BeginUltra("getcmdboxret");
	if (lua_gettop(lua_env) == 1) {
		Form_Control* control = new Form_Control();
		Form_Event* event = new Form_Event();

		if (lua_type(lua_env, 1) == LUA_TLIGHTUSERDATA) {
			event = (Form_Event*)lua_touserdata(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		control = find_control(event->event_key);

		if (control->cmdbox_ret != "") {
			lua_pushstring(lua_env, control->cmdbox_ret.c_str());
			return 1;
		}
		else {
			lua_pushstring(lua_env, "no data");
			return 1;
		}
	}
	return 0;
	VEnd();
}

int reset_cmd_box_ret(lua_State* lua_env) {
	BeginUltra("resetmcdbxo");
	if (lua_gettop(lua_env) == 1) {
		Form_Control* control = new Form_Control();
		Form_Event* event = new Form_Event();

		if (lua_type(lua_env, 1) == LUA_TLIGHTUSERDATA) {
			event = (Form_Event*)lua_touserdata(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		control = find_control(event->event_key);

		control->cmdbox_ret = "";
		control->last_who_gives_a_shit = "";
	}
	return 0;
	VEnd();
}

int hookWindowFunction(lua_State* lua_env) {
	BeginUltra("hookwndfun");
	if (lua_gettop(lua_env) > 2)
		return luaL_error(lua_env, "too many args in WinLua.HookEvent");

	if (lua_gettop(lua_env) == 2) {
		int func;
		Form_Event* event = new Form_Event();

		if (lua_type(lua_env, 1) == LUA_TLIGHTUSERDATA) {
			event = (Form_Event*)lua_touserdata(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		if (lua_type(lua_env, 2) == LUA_TFUNCTION) {
			int t = LUA_REGISTRYINDEX;
			t = ((t) > 0 || (t) <= LUA_REGISTRYINDEX ? (t) : lua_gettop(lua_env) + (t)+1);
			lua_rawgeti(lua_env, t, 0);
			int ref = (int)lua_tointeger(lua_env, -1);
			lua_pop(lua_env, 1);
			if (ref != 0) {
				lua_rawgeti(lua_env, t, ref);
				lua_rawseti(lua_env, t, 0);
			}
			else {
				ref = (int)lua_objlen(lua_env, t);
				ref++;
			}
			lua_rawseti(lua_env, t, ref);
			func = ref;
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 2");
			return 1;
		}

		event->env = lua_env;
		event->function = func;
	}
	return 0;
	VEnd();
}

int setColor(lua_State* lua_env) {
	BeginUltra("setcol");
	if (lua_gettop(lua_env) == 2) {
		Form_Control* control = new Form_Control();
		COLORREF color;
		Form_Event* event = new Form_Event();

		if (lua_type(lua_env, 1) == LUA_TLIGHTUSERDATA) {
			event = (Form_Event*)lua_touserdata(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		if (lua_type(lua_env, 2) == LUA_TLIGHTUSERDATA) {
			color = (COLORREF)lua_touserdata(lua_env, 2);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 2");
			return 1;
		}

		control = find_control(event->event_key);

		if (!control->ismenu)
			SendMessage(control->window, EM_SETBKGNDCOLOR, 0, color);
		else {
			lua_pushstring(lua_env, "Invalid control");
			return 1;
		}

		return 0;
	}
	return 0;
	VEnd();
}

int setFont(lua_State* lua_env) {
	BeginUltra("setfpmt");
	if (lua_gettop(lua_env) == 2) {
		Form_Control* control = new Form_Control();
		HFONT font;
		Form_Event* event = new Form_Event();

		if (lua_type(lua_env, 1) == LUA_TLIGHTUSERDATA) {
			event = (Form_Event*)lua_touserdata(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		if (lua_type(lua_env, 2) == LUA_TLIGHTUSERDATA) {
			font = (HFONT)lua_touserdata(lua_env, 2);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 2");
			return 1;
		}

		control = find_control(event->event_key);

		if (!control->ismenu)
			SendMessage(control->window, WM_SETFONT, (WPARAM)font, TRUE);
		else {
			lua_pushstring(lua_env, "Invalid control");
			return 1;
		}

		return 0;
	}
	return 0;
	VEnd();
}

int messageLoop(lua_State* lua_env) {
	BeginUltra("msglop");
	MSG msg;
	HWND hwnd = (HWND)lua_touserdata(lua_env, 1);

	while (GetMessage(&msg, 0, 0, 0)) {
		if (find_form(hwnd)->quit) {
			for (int i = 0; i < Forms.size(); i++) {
				if (Forms[i]->window == hwnd)
					Forms.erase(Forms.begin() + i);
			}
			break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
	VEnd();
}

int createMessageLoop(lua_State* lua_env) {
	BeginUltra("gayu");
	if (lua_gettop(lua_env) == 1) {
		Form* form = new Form();

		if (lua_type(lua_env, 1) == LUA_TLIGHTUSERDATA) {
			form = (Form*)lua_touserdata(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		lua_pushcfunction(lua_env, messageLoop);
		lua_State* coroutine = lua_newthread(lua_env);
		lua_pushvalue(lua_env, -2);
		lua_xmove(lua_env, coroutine, 1);
		lua_pushlightuserdata(coroutine, (void*)form->window);
		form->message_handler = coroutine;
		lua_resume(coroutine, 1);
		return 0;
	}
	else
		return 0;
	VEnd();
}

int impl_sleep(lua_State* lua_env) {
	BeginUltra("slep");
	if (lua_gettop(lua_env) == 1) {
		int miliseconds;

		if (lua_type(lua_env, 1) == LUA_TNUMBER) {
			miliseconds = lua_tonumber(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		Sleep(miliseconds);
		lua_pushboolean(lua_env, true);
		return 1;
	}
	return 0;
	VEnd();
}

int impl_wait(lua_State* lua_env) {
	BeginUltra("weait");
	if (lua_gettop(lua_env) == 1) {
		int miliseconds;

		if (lua_type(lua_env, 1) == LUA_TNUMBER) {
			miliseconds = lua_tonumber(lua_env, 1);
		}
		else {
			lua_pushstring(lua_env, "Invalid argument 1");
			return 1;
		}

		Sleep(miliseconds * 1000);
		lua_pushboolean(lua_env, true);
		return 1;
	}
	return 0;
	VEnd();
}

void FormLibrary(lua_State* lua_env) {
	BeginUltra("fprmlib");
	//lua_pushstring(lua_env, util->local_path().c_str());
	//lua_setglobal(lua_env, "cobalt_path");

	lua_pushcfunction(lua_env, impl_sleep);
	lua_setglobal(lua_env, "sleep");

	lua_pushcfunction(lua_env, impl_wait);
	lua_setglobal(lua_env, "wait");

	lua_pushcfunction(lua_env, _RGB);
	lua_setglobal(lua_env, "RGB");

	lua_pushcfunction(lua_env, impl_downloadurl);
	lua_setglobal(lua_env, "download_url");

	lua_pushcfunction(lua_env, impl_writefile);
	lua_setglobal(lua_env, "write_file");

	lua_pushcfunction(lua_env, impl_readfile);
	lua_setglobal(lua_env, "read_file");

	lua_pushcfunction(lua_env, impl_execute_cscript);
	lua_setglobal(lua_env, "execute_cscript");

	lua_pushcfunction(lua_env, impl_execute_script);
	lua_setglobal(lua_env, "execute_script");

	lua_pushcfunction(lua_env, impl_execute_command);
	lua_setglobal(lua_env, "execute_command");

	lua_pushcfunction(lua_env, impl_loadstring);
	lua_setglobal(lua_env, "loadstring");

	lua_pushcfunction(lua_env, impl_open_dialog);
	lua_setglobal(lua_env, "open_dialog");

	lua_pushcfunction(lua_env, impl_save_dialog);
	lua_setglobal(lua_env, "save_dialog");

	lua_pushcfunction(lua_env, impl_exit);
	lua_setglobal(lua_env, "exit");

	lua_pushcfunction(lua_env, impl_unload);
	lua_setglobal(lua_env, "unload_cobalt");

	lua_pushcfunction(lua_env, hook_cmd_box);
	lua_setglobal(lua_env, "hook_cmd_box");

	lua_pushcfunction(lua_env, get_cmd_box_ret);
	lua_setglobal(lua_env, "get_cmd_box_ret");

	lua_pushcfunction(lua_env, reset_cmd_box_ret);
	lua_setglobal(lua_env, "reset_cmd_box_ret");

	lua_pushcfunction(lua_env, redirect_printf);
	lua_setglobal(lua_env, "redirect_printf");

	lua_createtable(lua_env, 0, 0);
	{
		lua_pushcfunction(lua_env, setColor);
		lua_setfield(lua_env, -2, "SetColor");

		lua_pushcfunction(lua_env, setText);
		lua_setfield(lua_env, -2, "SetText");

		lua_pushcfunction(lua_env, getText);
		lua_setfield(lua_env, -2, "GetText");

		lua_pushcfunction(lua_env, createMenu);
		lua_setfield(lua_env, -2, "CreateMenu");

		lua_pushcfunction(lua_env, hookWindowFunction);
		lua_setfield(lua_env, -2, "HookEvent");

		lua_pushcfunction(lua_env, createMessageLoop);
		lua_setfield(lua_env, -2, "CreateMessageLoop");

		lua_pushcfunction(lua_env, createFont);
		lua_setfield(lua_env, -2, "CreateFont");

		lua_pushcfunction(lua_env, appendMenu);
		lua_setfield(lua_env, -2, "AppendMenu");

		lua_pushcfunction(lua_env, createWindow);
		lua_setfield(lua_env, -2, "CreateWindow");

		lua_pushcfunction(lua_env, registerForm);
		lua_setfield(lua_env, -2, "RegisterForm");

		lua_pushcfunction(lua_env, setFont);
		lua_setfield(lua_env, -2, "SetFont");

		lua_pushcfunction(lua_env, setChecked);
		lua_setfield(lua_env, -2, "SetChecked");

		lua_pushcfunction(lua_env, getChecked);
		lua_setfield(lua_env, -2, "GetChecked");

		lua_pushcfunction(lua_env, printmsg);
		lua_setfield(lua_env, -2, "print");
	}
	lua_setglobal(lua_env, "WinLua");

	lua_createtable(lua_env, 0, 0);
	{
		lua_pushnumber(lua_env, WS_ACTIVECAPTION);
		lua_setfield(lua_env, -2, "WS_ACTIVECAPTION");

		lua_pushnumber(lua_env, WS_BORDER);
		lua_setfield(lua_env, -2, "WS_BORDER");

		lua_pushnumber(lua_env, WS_CAPTION);
		lua_setfield(lua_env, -2, "WS_CAPTION");

		lua_pushnumber(lua_env, WS_CHILD);
		lua_setfield(lua_env, -2, "WS_CHILD");

		lua_pushnumber(lua_env, WS_CHILDWINDOW);
		lua_setfield(lua_env, -2, "WS_CHILDWINDOW");

		lua_pushnumber(lua_env, WS_CLIPCHILDREN);
		lua_setfield(lua_env, -2, "WS_CLIPCHILDREN");

		lua_pushnumber(lua_env, WS_CLIPSIBLINGS);
		lua_setfield(lua_env, -2, "WS_CLIPSIBLINGS");

		lua_pushnumber(lua_env, WS_DISABLED);
		lua_setfield(lua_env, -2, "WS_DISABLED");

		lua_pushnumber(lua_env, WS_DLGFRAME);
		lua_setfield(lua_env, -2, "WS_DLGFRAME");

		lua_pushnumber(lua_env, WS_VSCROLL);
		lua_setfield(lua_env, -2, "WS_VSCROLL");

		lua_pushnumber(lua_env, WS_VISIBLE);
		lua_setfield(lua_env, -2, "WS_VISIBLE");

		lua_pushnumber(lua_env, WS_TILEDWINDOW);
		lua_setfield(lua_env, -2, "WS_TILEDWINDOW");

		lua_pushnumber(lua_env, WS_TILED);
		lua_setfield(lua_env, -2, "WS_TILED");

		lua_pushnumber(lua_env, WS_THICKFRAME);
		lua_setfield(lua_env, -2, "WS_THICKFRAME");

		lua_pushnumber(lua_env, WS_TABSTOP);
		lua_setfield(lua_env, -2, "WS_TABSTOP");

		lua_pushnumber(lua_env, WS_SYSMENU);
		lua_setfield(lua_env, -2, "WS_SYSMENU");

		lua_pushnumber(lua_env, WS_SIZEBOX);
		lua_setfield(lua_env, -2, "WS_SIZEBOX");

		lua_pushnumber(lua_env, WS_POPUPWINDOW);
		lua_setfield(lua_env, -2, "WS_POPUPWINDOW");

		lua_pushnumber(lua_env, WS_POPUP);
		lua_setfield(lua_env, -2, "WS_POPUP");

		lua_pushnumber(lua_env, WS_OVERLAPPEDWINDOW);
		lua_setfield(lua_env, -2, "WS_OVERLAPPEDWINDOW");

		lua_pushnumber(lua_env, WS_OVERLAPPED);
		lua_setfield(lua_env, -2, "WS_OVERLAPPED");

		lua_pushnumber(lua_env, WS_MINIMIZEBOX);
		lua_setfield(lua_env, -2, "WS_MINIMIZEBOX");

		lua_pushnumber(lua_env, WS_MINIMIZE);
		lua_setfield(lua_env, -2, "WS_MINIMIZE");

		lua_pushnumber(lua_env, WS_MAXIMIZEBOX);
		lua_setfield(lua_env, -2, "WS_MAXIMIZEBOX");

		lua_pushnumber(lua_env, WS_MAXIMIZE);
		lua_setfield(lua_env, -2, "WS_MAXIMIZE");

		lua_pushnumber(lua_env, WS_ICONIC);
		lua_setfield(lua_env, -2, "WS_ICONIC");

		lua_pushnumber(lua_env, WS_HSCROLL);
		lua_setfield(lua_env, -2, "WS_HSCROLL");

		lua_pushnumber(lua_env, WS_GROUP);
		lua_setfield(lua_env, -2, "WS_GROUP");

		lua_pushnumber(lua_env, WS_EX_ACCEPTFILES);
		lua_setfield(lua_env, -2, "WS_EX_ACCEPTFILES");

		lua_pushnumber(lua_env, WS_EX_APPWINDOW);
		lua_setfield(lua_env, -2, "WS_ACTIVECAPTION");

		lua_pushnumber(lua_env, WS_EX_CLIENTEDGE);
		lua_setfield(lua_env, -2, "WS_EX_CLIENTEDGE");

		lua_pushnumber(lua_env, WS_EX_COMPOSITED);
		lua_setfield(lua_env, -2, "WS_EX_COMPOSITED");

		lua_pushnumber(lua_env, WS_EX_CONTEXTHELP);
		lua_setfield(lua_env, -2, "WS_EX_CONTEXTHELP");

		lua_pushnumber(lua_env, WS_EX_CONTROLPARENT);
		lua_setfield(lua_env, -2, "WS_EX_CONTROLPARENT");

		lua_pushnumber(lua_env, WS_EX_DLGMODALFRAME);
		lua_setfield(lua_env, -2, "WS_EX_DLGMODALFRAME");

		lua_pushnumber(lua_env, WS_EX_LAYERED);
		lua_setfield(lua_env, -2, "WS_EX_LAYERED");

		lua_pushnumber(lua_env, WS_EX_LAYOUTRTL);
		lua_setfield(lua_env, -2, "WS_EX_LAYOUTRTL");

		lua_pushnumber(lua_env, WS_EX_LEFT);
		lua_setfield(lua_env, -2, "WS_EX_LEFT");

		lua_pushnumber(lua_env, WS_EX_LEFTSCROLLBAR);
		lua_setfield(lua_env, -2, "WS_EX_LEFTSCROLLBAR");

		lua_pushnumber(lua_env, WS_EX_LTRREADING);
		lua_setfield(lua_env, -2, "WS_EX_LTRREADING");

		lua_pushnumber(lua_env, WS_EX_MDICHILD);
		lua_setfield(lua_env, -2, "WS_EX_MDICHILD");

		lua_pushnumber(lua_env, WS_EX_NOACTIVATE);
		lua_setfield(lua_env, -2, "WS_EX_NOACTIVATE");

		lua_pushnumber(lua_env, WS_EX_NOINHERITLAYOUT);
		lua_setfield(lua_env, -2, "WS_EX_NOINHERITLAYOUT");

		lua_pushnumber(lua_env, WS_EX_NOPARENTNOTIFY);
		lua_setfield(lua_env, -2, "WS_EX_NOPARENTNOTIFY");

		lua_pushnumber(lua_env, WS_EX_NOREDIRECTIONBITMAP);
		lua_setfield(lua_env, -2, "WS_EX_NOREDIRECTIONBITMAP");

		lua_pushnumber(lua_env, WS_EX_OVERLAPPEDWINDOW);
		lua_setfield(lua_env, -2, "WS_EX_OVERLAPPEDWINDOW");

		lua_pushnumber(lua_env, WS_EX_PALETTEWINDOW);
		lua_setfield(lua_env, -2, "WS_EX_PALETTEWINDOW");

		lua_pushnumber(lua_env, WS_EX_RIGHT);
		lua_setfield(lua_env, -2, "WS_EX_RIGHT");

		lua_pushnumber(lua_env, WS_EX_RIGHTSCROLLBAR);
		lua_setfield(lua_env, -2, "WS_EX_RIGHTSCROLLBAR");

		lua_pushnumber(lua_env, WS_EX_RTLREADING);
		lua_setfield(lua_env, -2, "WS_EX_RTLREADING");

		lua_pushnumber(lua_env, WS_EX_STATICEDGE);
		lua_setfield(lua_env, -2, "WS_EX_STATICEDGE");

		lua_pushnumber(lua_env, WS_EX_TOOLWINDOW);
		lua_setfield(lua_env, -2, "WS_EX_TOOLWINDOW");

		lua_pushnumber(lua_env, WS_EX_TOPMOST);
		lua_setfield(lua_env, -2, "WS_EX_TOPMOST");

		lua_pushnumber(lua_env, WS_EX_TRANSPARENT);
		lua_setfield(lua_env, -2, "WS_EX_TRANSPARENT");

		lua_pushnumber(lua_env, WS_EX_WINDOWEDGE);
		lua_setfield(lua_env, -2, "WS_EX_WINDOWEDGE");

		lua_pushnumber(lua_env, ES_AUTOHSCROLL);
		lua_setfield(lua_env, -2, "ES_AUTOHSCROLL");

		lua_pushnumber(lua_env, ES_AUTOVSCROLL);
		lua_setfield(lua_env, -2, "ES_AUTOVSCROLL");

		lua_pushnumber(lua_env, ES_AWAYMODE_REQUIRED);
		lua_setfield(lua_env, -2, "ES_AWAYMODE_REQUIRED");

		lua_pushnumber(lua_env, ES_CENTER);
		lua_setfield(lua_env, -2, "ES_CENTER");

		lua_pushnumber(lua_env, ES_CONTINUOUS);
		lua_setfield(lua_env, -2, "ES_CONTINUOUS");

		lua_pushnumber(lua_env, ES_DISPLAY_REQUIRED);
		lua_setfield(lua_env, -2, "ES_DISPLAY_REQUIRED");

		lua_pushnumber(lua_env, ES_LEFT);
		lua_setfield(lua_env, -2, "ES_LEFT");

		lua_pushnumber(lua_env, ES_LOWERCASE);
		lua_setfield(lua_env, -2, "ES_LOWERCASE");

		lua_pushnumber(lua_env, ES_MULTILINE);
		lua_setfield(lua_env, -2, "ES_MULTILINE");

		lua_pushnumber(lua_env, ES_NOHIDESEL);
		lua_setfield(lua_env, -2, "ES_NOHIDESEL");

		lua_pushnumber(lua_env, ES_NUMBER);
		lua_setfield(lua_env, -2, "ES_NUMBER");

		lua_pushnumber(lua_env, ES_OEMCONVERT);
		lua_setfield(lua_env, -2, "ES_OEMCONVERT");

		lua_pushnumber(lua_env, ES_PASSWORD);
		lua_setfield(lua_env, -2, "ES_PASSWORD");

		lua_pushnumber(lua_env, ES_READONLY);
		lua_setfield(lua_env, -2, "ES_READONLY");

		lua_pushnumber(lua_env, ES_RIGHT);
		lua_setfield(lua_env, -2, "ES_RIGHT");

		lua_pushnumber(lua_env, ES_SYSTEM_REQUIRED);
		lua_setfield(lua_env, -2, "ES_SYSTEM_REQUIRED");

		lua_pushnumber(lua_env, ES_UPPERCASE);
		lua_setfield(lua_env, -2, "ES_UPPERCASE");

		lua_pushnumber(lua_env, ES_USER_PRESENT);
		lua_setfield(lua_env, -2, "ES_USER_PRESENT");

		lua_pushnumber(lua_env, ES_WANTRETURN);
		lua_setfield(lua_env, -2, "ES_WANTRETURN");

		/* Button Stuff */
		lua_pushnumber(lua_env, BS_3STATE);
		lua_setfield(lua_env, -2, "BS_3STATE");

		lua_pushnumber(lua_env, BS_AUTO3STATE);
		lua_setfield(lua_env, -2, "BS_AUTO3STATE");

		lua_pushnumber(lua_env, BS_AUTOCHECKBOX);
		lua_setfield(lua_env, -2, "BS_AUTOCHECKBOX");

		lua_pushnumber(lua_env, BS_AUTORADIOBUTTON);
		lua_setfield(lua_env, -2, "BS_AUTORADIOBUTTON");

		lua_pushnumber(lua_env, BS_BITMAP);
		lua_setfield(lua_env, -2, "BS_BITMAP");

		lua_pushnumber(lua_env, BS_BOTTOM);
		lua_setfield(lua_env, -2, "BS_BOTTOM");

		lua_pushnumber(lua_env, BS_CENTER);
		lua_setfield(lua_env, -2, "BS_CENTER");

		lua_pushnumber(lua_env, BS_CHECKBOX);
		lua_setfield(lua_env, -2, "BS_CHECKBOX");

		lua_pushnumber(lua_env, BS_COMMANDLINK);
		lua_setfield(lua_env, -2, "BS_COMMANDLINK");

		lua_pushnumber(lua_env, BS_DEFCOMMANDLINK);
		lua_setfield(lua_env, -2, "BS_DEFCOMMANDLINK");

		lua_pushnumber(lua_env, BS_DEFPUSHBUTTON);
		lua_setfield(lua_env, -2, "BS_DEFPUSHBUTTON");

		lua_pushnumber(lua_env, BS_DEFSPLITBUTTON);
		lua_setfield(lua_env, -2, "BS_DEFSPLITBUTTON");

		lua_pushnumber(lua_env, BS_DIBPATTERN);
		lua_setfield(lua_env, -2, "BS_DIBPATTERN");

		lua_pushnumber(lua_env, BS_DIBPATTERN8X8);
		lua_setfield(lua_env, -2, "BS_DIBPATTERN8X8");

		lua_pushnumber(lua_env, BS_DIBPATTERNPT);
		lua_setfield(lua_env, -2, "BS_DIBPATTERNPT");

		lua_pushnumber(lua_env, BS_DIBPATTERNPT);
		lua_setfield(lua_env, -2, "BS_DIBPATTERNPT");

		lua_pushnumber(lua_env, BS_FLAT);
		lua_setfield(lua_env, -2, "BS_FLAT");

		lua_pushnumber(lua_env, BS_GROUPBOX);
		lua_setfield(lua_env, -2, "BS_GROUPBOX");

		lua_pushnumber(lua_env, BS_HATCHED);
		lua_setfield(lua_env, -2, "BS_HATCHED");

		lua_pushnumber(lua_env, BS_HOLLOW);
		lua_setfield(lua_env, -2, "BS_HOLLOW");

		lua_pushnumber(lua_env, BS_ICON);
		lua_setfield(lua_env, -2, "BS_ICON");

		lua_pushnumber(lua_env, BS_INDEXED);
		lua_setfield(lua_env, -2, "BS_INDEXED");

		lua_pushnumber(lua_env, BS_LEFT);
		lua_setfield(lua_env, -2, "BS_LEFT");

		lua_pushnumber(lua_env, BS_LEFTTEXT);
		lua_setfield(lua_env, -2, "BS_LEFTTEXT");

		lua_pushnumber(lua_env, BS_MONOPATTERN);
		lua_setfield(lua_env, -2, "BS_MONOPATTERN");

		lua_pushnumber(lua_env, BS_MULTILINE);
		lua_setfield(lua_env, -2, "BS_MULTILINE");

		lua_pushnumber(lua_env, BS_NOTIFY);
		lua_setfield(lua_env, -2, "BS_NOTIFY");

		lua_pushnumber(lua_env, BS_NULL);
		lua_setfield(lua_env, -2, "BS_NULL");

		lua_pushnumber(lua_env, BS_OWNERDRAW);
		lua_setfield(lua_env, -2, "BS_OWNERDRAW");

		lua_pushnumber(lua_env, BS_PATTERN);
		lua_setfield(lua_env, -2, "BS_PATTERN");

		lua_pushnumber(lua_env, BS_PATTERN8X8);
		lua_setfield(lua_env, -2, "BS_PATTERN8X8");

		lua_pushnumber(lua_env, BS_PUSHBOX);
		lua_setfield(lua_env, -2, "BS_PUSHBOX");

		lua_pushnumber(lua_env, BS_PUSHBUTTON);
		lua_setfield(lua_env, -2, "BS_PUSHBUTTON");

		lua_pushnumber(lua_env, BS_PUSHLIKE);
		lua_setfield(lua_env, -2, "BS_PUSHLIKE");

		lua_pushnumber(lua_env, BS_RADIOBUTTON);
		lua_setfield(lua_env, -2, "BS_RADIOBUTTON");

		lua_pushnumber(lua_env, BS_RIGHTBUTTON);
		lua_setfield(lua_env, -2, "BS_RIGHTBUTTON");

		lua_pushnumber(lua_env, BS_SOLID);
		lua_setfield(lua_env, -2, "BS_SOLID");

		lua_pushnumber(lua_env, BS_SPLITBUTTON);
		lua_setfield(lua_env, -2, "BS_SPLITBUTTON");

		lua_pushnumber(lua_env, BS_TEXT);
		lua_setfield(lua_env, -2, "BS_TEXT");

		lua_pushnumber(lua_env, BS_TOP);
		lua_setfield(lua_env, -2, "BS_TOP");

		lua_pushnumber(lua_env, BS_TYPEMASK);
		lua_setfield(lua_env, -2, "BS_TYPEMASK");

		lua_pushnumber(lua_env, BS_USERBUTTON);
		lua_setfield(lua_env, -2, "BS_USERBUTTON");

		lua_pushnumber(lua_env, BS_VCENTER);
		lua_setfield(lua_env, -2, "BS_VCENTER");
	}
	lua_setglobal(lua_env, "Flags");

	lua_createtable(lua_env, 0, 0);
	{
		lua_pushstring(lua_env, WC_BUTTONA);
		lua_setfield(lua_env, -2, "Button");

		lua_pushstring(lua_env, WC_COMBOBOXA);
		lua_setfield(lua_env, -2, "ComboBox");

		lua_pushstring(lua_env, WC_COMBOBOXEXA);
		lua_setfield(lua_env, -2, "ComboBoxEx");

		lua_pushstring(lua_env, RICHEDIT_CLASSA);
		lua_setfield(lua_env, -2, "TextBox");

		lua_pushstring(lua_env, WC_HEADERA);
		lua_setfield(lua_env, -2, "Header");

		lua_pushstring(lua_env, WC_IPADDRESSA);
		lua_setfield(lua_env, -2, "IpAddress");

		lua_pushstring(lua_env, WC_LISTBOXA);
		lua_setfield(lua_env, -2, "ListBox");

		lua_pushstring(lua_env, WC_LISTVIEWA);
		lua_setfield(lua_env, -2, "ListView");

		lua_pushstring(lua_env, util->ws2s(WC_LINK).c_str());
		lua_setfield(lua_env, -2, "Link");

		lua_pushstring(lua_env, "Scintilla");
		lua_setfield(lua_env, -2, "Editor");

		lua_pushstring(lua_env, WC_NATIVEFONTCTLA);
		lua_setfield(lua_env, -2, "NativeFontCtl");

		lua_pushstring(lua_env, WC_PAGESCROLLERA);
		lua_setfield(lua_env, -2, "PageScroller");

		lua_pushstring(lua_env, WC_SCROLLBARA);
		lua_setfield(lua_env, -2, "ScrollBar");

		lua_pushstring(lua_env, "LAEBEL");
		lua_setfield(lua_env, -2, "Label");

		lua_pushstring(lua_env, WC_TABCONTROLA);
		lua_setfield(lua_env, -2, "TabControl");

		lua_pushstring(lua_env, WC_TREEVIEWA);
		lua_setfield(lua_env, -2, "TreeView");
	}
	lua_setglobal(lua_env, "Controls");
	VEnd();
}

/* Load all the Librarys */

void WinLua_LoadLibararys(lua_State* lua_env) {
	BeginUltra("winlualioadilib");
	MessageBoxLibrary(lua_env); //MessageBox API (MessageBox.Show(Buttons.Ok, "Message Body", "Message Title") to show a messagebox with "MessageBody" and "Message Title" [does return value])
	WindowStateLibrary(lua_env); //For window state management (SW_HIDE, SW_SHOW, SW_MINIMIZE, SW_MAXIMIZE, and new being just a bool, true)
	FormLibrary(lua_env); //For forms
	VEnd();
}
#pragma endregion

int executestuff() {
	BeginUltra("execstuf");
	bool printerr = var->getVariable("print error").XVAL.XVAL_BOOL;
	std::string tempscript = var->getValue(XVAL_VSTRING, "script").XVAL.XVAL_STRING;
	std::string tempname = var->getValue(XVAL_VSTRING, "name").XVAL.XVAL_STRING;
	lua_State* tempstate = (lua_State*)var->getValue(XVAL_VVOIDPTR, "state").XVAL.XVAL_VOIDPTR;

	try {
		lua_State* L = lua_open();
		luaL_openlibs(L);
		WinLua_LoadLibararys(L);
		lua_State* NL = lua_newthread(L);

		if (luaL_loadbuffer(NL, tempscript.c_str(), tempscript.size(), "WinLua")) {
			const char* plain_error = lua_tostring(NL, -1);
			std::string error = "Error caught in WinLua::runScript (compiling)\r\n\r\n";
			error += plain_error;
			if (!printerr)
				lua_pushstring(tempstate, error.c_str());
			if (printerr)
				//util->print("File %s failed to run:\n%s\n", tempname.c_str(), error.c_str());
			lua_close(NL);
			return 1;
		}
		else {
			if (lua_pcall(NL, 0, 0, 0)) {
				const char* plain_execution_error = lua_tostring(NL, -1);
				std::string execution_error = "Error caught in WinLua::runScript (calling)\r\n\r\n";
				execution_error += plain_execution_error;
				if (!printerr)
					lua_pushstring(tempstate, execution_error.c_str());
				if (printerr)
					//util->print("File %s failed to run:\n%s\n", tempname.c_str(), execution_error.c_str());
				lua_close(NL);
				return 1;
			}
			else {
				if (printerr)
					//util->print("File \"%s\" has ran away\n", tempname.c_str());
				lua_close(NL);
				return 0;
			}
		}
	}
	catch (std::exception e) {
		std::string error = "Error caught in WinLua::runScript (exception)\r\n\r\n";
		error += e.what();
		if (!printerr)
			lua_pushstring(tempstate, error.c_str());
		if (printerr)
			//util->print("File %s failed to run:\n%s\n", tempname.c_str(), error.c_str());
		return 1;
	}
	VEnd();
}

class WinLua {
private:
	bool created_vars = false;
public:
	lua_State* gL;

	bool runScript(std::string script, std::string file_name, bool wait, bool print_errors_to_console, lua_State* L) {
		BeginUltra("runscrip");
		if (created_vars) {
			var->removeValue("script");
			var->removeValue("name");
			var->removeValue("print error");
			var->removeValue("state");
			created_vars = false;
		}

		inval in;

		if (!created_vars) {
			in.XVAL_STRING = script.c_str();
			var->createValue(XVAL_VSTRING, in, "script");
			in.XVAL_STRING = file_name.c_str();
			var->createValue(XVAL_VSTRING, in, "name");
			in.XVAL_BOOL = print_errors_to_console;
			var->createValue(XVAL_VBOOL, in, "print error");
			if (L) {
				in.XVAL_VOIDPTR = L;
			}
			else {
				in.XVAL_VOIDPTR = gL;
			}
			var->createValue(XVAL_VVOIDPTR, in, "state");
			created_vars = true;
		}

		HANDLE thread = CreateThread(0, 0, [](void*)->DWORD {
			return executestuff();
		}, 0, 0, 0);

		if (wait) {
			DWORD exitcode;

			if (thread) {
				DWORD result;

				do {
					result = WaitForSingleObject(thread, 0);
					if (result == WAIT_OBJECT_0) {
						GetExitCodeThread(thread, &exitcode);
						break;
					}
					Sleep(100);
				} while (1);
			}

			if (exitcode == 1)
				return false;
			else if (exitcode == 0)
				return true;
		}
		return true;
		VEnd();
	}

	WinLua() {
		BeginUltra("winlua");
		gL = lua_open();

		if (!loaded) {
			if (!LoadLibraryA("Riched20.dll")) {
				MessageBoxA(NULL, "WinLua failed to load a critical library.", "WinLua", MB_OK);
				exit(0);
			}
			//INITCOMMONCONTROLSEX icc;
			//icc.dwICC = ICC_WIN95_CLASSES;
			//icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
			//if (!InitCommonControlsEx(&icc)) {
			//	MessageBoxA(NULL, "WinLua failed to load a critical library.", "WinLua", MB_OK);
			//	exit(0);
			//}

			loaded = true;
		}
		VEnd();
	}
};
#endif

/*
Developed by Cyberhound#5672 on discord
feel free to friend me and spam me questions. :D

You may use this, however you want, and as much as you want.
Just please either credit me by a comment or something somewhere,
if you want to contribute to this please let me know! :D
*/

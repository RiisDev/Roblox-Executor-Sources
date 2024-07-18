#include <Windows.h>
#include <iostream>

#include "memscan.h"
#include "checkbypass.h"
#include <Richedit.h>
#include <CommCtrl.h>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

#include "convert.h"
#include "misc.h"
#include "SWScintilla.h"
#include <fstream>
#include <deque>
#include <climits>
#include <cstdio>
#include <cstdlib>

/*
Niggas iffy, uh, Blicky got the stiffy, uh
Got the blicky, uh, drum it holds fifty, uh
Move milli', all my niggas on fifty, uh
Talk down, pew pew pew, you silly, uh
Hit a stain, fifty bands, all hunnids
Spinning through ya block like a pop shove-it
Shoot at me I'm shooting back, I'm getting buckets
I ain't wanna take his life but nigga, fuck it
*/

void check_game(int rL); //checks if the game is a game that Script-Ware can do game specific scripts on

LONG roblox_wnd_proc;
HWND roblox_window;
void handle_command(const std::string& command); //handles command input via script-ware's command bar

/* boolean statements for the game specific scripts */
bool use_rare = false;
bool use_reach = false;
bool use_getitems = false;
bool use_admin = false;

bool remember_me_ineffect = false;

#pragma comment(lib, "Comctl32.lib")
#pragma comment(linker, "\"/manifestdependency:type='win32' \
					   name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
					   processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define USERNAME ""
#define PASSWORD ""

std::string sw_title;

void output_label(const char* str) {
	SendMessage(output, WM_SETTEXT, (WPARAM)"", (LPARAM)str);
}

#define BUFFER_SIZE 16777216

#define TXT_PASSWORDFIELD (WM_APP + 499)
#define TXT_CMDINPUT (WM_APP + 500)
#define BUTTON_CLEAR_SCRIPTEDITOR (WM_APP + 501)
#define BUTTON_OPEN_SCRIPT (WM_APP + 502)
#define BUTTON_EXECUTE_SCRIPT (WM_APP + 503)
#define BUTTON_LOGIN (WM_APP + 504)
#define BUTTON_SUBMIT (WM_APP + 505)
#define BUTTON_REMEMBERME (WM_APP + 506)
#define BUTTON_EXECUTE_SCRIPT_BIGGER (WM_APP + 507)
#define BUTTON_CLEAR_BIGGER (WM_APP + 508)
#define BUTTON_DEBUGMODE (WM_APP + 509)

#define MENU_EXIT (WM_APP + 800)
#define MENU_DEATTACH (WM_APP + 801)
#define MENU_ABOUT (WM_APP + 901)

#define MENU_TOPMOST (WM_APP + 902)
#define MENU_PARENTED (WM_APP + 903)

#define MENU_BIGGERWINDOW (WM_APP + 802)

#define LISTVIEW_SCRIPTS (WM_APP + 605)

#define UI_LV_RCM_EXEC (WM_APP + 700)
#define	UI_LV_RCM_LOAD (WM_APP + 701)

#define BUTTON_CONTINUE (WM_APP + 800)
#define CHECKBOX_PARENT (WM_APP + 801)
#define CHECKBOX_TOPMOST (WM_APP + 802)
#define CHECKBOX_CORESCRIPT (WM_APP + 803)
#define CHECKBOX_EXCEPTIONHANDLER (WM_APP + 804)

/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
//begin user interface functions

DWORD WINAPI exit_sw(void* p_mem)
{
	FreeLibrary(GetModuleHandleA((const char*)p_mem));
	return 0;
}

LRESULT CALLBACK patched_wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, message, wparam, lparam);
}

void hook_roblox_wndproc() {
	SetWindowLongPtr(roblox_window, GWLP_WNDPROC, (LONG)patched_wndproc);
}

void restore_roblox_wndproc() {
	SetWindowLongPtr(roblox_window, GWLP_WNDPROC, roblox_wnd_proc);
}

void tutorial() {
	MessageBox(NULL, "Welcome to the Script-Ware tutorial! Due to this being your first time using Script-Ware, you have been granted with an easy tutorial sequence of messageboxes. These are designed to help with how to use Script-Ware.\nIf you would like to re-play this, just execute in the command box; 'tutorial' and it will replay.", "Script-Ware", MB_OK);
	MessageBox(NULL, "You're able to automatically execute any script you want straight when Script-Ware has completed its loading. Just go to your Script-Ware folder, go into the data folder and then into the autoexec folder, place any .lua or .txt file you want to be auto ran inside. You can have as many as you would like!\n", "Script-Ware", MB_OK);
	MessageBox(NULL, "ScriptWare's main user interface consists of a main script box, an output box and a small strip under the output box, this box is called the command bar/box", "Script-Ware", MB_OK);
	MessageBox(NULL, "In this bar you can execute commands that enhance your user experience with Script-Ware, why dont you go ahead and run 'cmds' in that box right away!", "Script-Ware", MB_OK);
	MessageBox(NULL, "This command bar also contains several pre-built scripts, these scripts can be ran by running the 'run' command, a space and then the preset script from the short list you are given.\n(for example, lets say i wanted to run the rocky2u script, i would put in the command bar 'run rocky2u' and hit enter)", "Script-Ware", MB_OK);
	MessageBox(NULL, "After you're done running commands in the command bar, make sure to run the 'cls' command! This command will clear out the output box, making it neat and tidy; just as you recieved it!", "Script-Ware", MB_OK);
	MessageBox(NULL, "You are able to easily run scripts by inputting them into the big box, which is called the script box, once the script has been inserted you are free to hit the execute button and run the script.\nIf you would like to find a script, you can click the Open button and look through your files for a txt or lua file which once double clicked will automatically be sent to the script box.", "Script-Ware", MB_OK);
	MessageBox(NULL, "If you would like to close Script-Ware, you can either hit the big X and simply close the entire thing alongside Roblox, or you could go inside the Script-Ware tab (thing at the top of the UI that says ScriptWare) and select de-attach, this will remove Script-Ware but keep Roblox open", "Script-Ware", MB_OK);
	MessageBox(NULL, "Fun Fact: Did you know Script-Ware has a cool chat system!? If you run 'chat' in the command bar, you have an activated chat script which allows you to chat with other users using Script-Ware in your server!", "Script-Ware", MB_OK);
	MessageBox(NULL, "Fun Fact #2: You can run 'load' alongside any script you currently have inside data->scripts and it will directly load the script into the script box! This is super useful for simple adjustments and whatnot.", "Script-Ware", MB_OK);
	MessageBox(NULL, "If you have questions, do not hesitate to contact AzuLX#7384 via Discord!\nThank you and enjoy! Don't forget you can replay this anytime! Just run 'tutorial' in the command bar.", "Script-Ware", MB_OK);
}

void print_output(COLORREF col, const char * format, ...) { //austin is a legend
	char message[500];
	memset(message, 0, sizeof(message));
	va_list vl;
	va_start(vl, format);
	vsnprintf_s(message, 500, format, vl);
	va_end(vl);

	int len = SendMessage(output_field, WM_GETTEXTLENGTH, NULL, NULL);
	SendMessage(output_field, EM_SETSEL, len, len);

	CHARFORMAT cfd;
	CHARFORMAT cf;
	SendMessage(output_field, EM_GETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cfd);
	memcpy(&cf, &cfd, sizeof(CHARFORMAT));

	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = col;
	cf.dwEffects = 0;

	SendMessage(output_field, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
	SendMessage(output_field, EM_REPLACESEL, FALSE, (LPARAM)message);
	SendMessage(output_field, WM_VSCROLL, SB_BOTTOM, 0L);
	SendMessage(output_field, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cfd);
}

static void stop_roblox_input() {
	HWND roblox_window = FindWindowW(0, L"ROBLOX");
	LONG roblox_window_ptr = GetWindowLongPtr(roblox_window, GWLP_WNDPROC);

	while (TRUE) {
		if (GetForegroundWindow() == main_login_window | GetForegroundWindow() == main_window) {
			SetWindowLongPtr(roblox_window, GWLP_WNDPROC, (LONG)patched_wndproc);
		}

		else {
			SetWindowLongPtr(roblox_window, GWLP_WNDPROC, roblox_window_ptr);
		}
	}
}

void handle_command_r(const std::string &command) {
	try {
		handle_command(command);
	}

	catch (std::exception &error) {
		print_output(RGB(255, 0, 0), "\nException caught during the command, '%s'. This may due to invalid arguments or a bad command call.");
	}

	catch (...) {
		print_output(RGB(255, 0, 0), "\r\nAn unexpected error occured when executing '%s'", command.c_str());
	}
}

const char* commands = {
	"run [scriptname.txt/lua] - Runs the script, must be in data->scripts\r\n"
	"exeurl/url [url] - Runs a url in a raw text format. Must be ran with the 'run' command as the first argument (eg. run url [url])\r\n"
	"chat - Runs a pre-made chat roblox gui that allows you to speak with members who are using Script-Ware and are in your game. Must be ran with 'run' just as the above command\r\n"
	"load [scriptname.txt/lua] - Loads the script into the script window, must be in data->scripts\r\n"
	"reautoexec - Re-executes all your files inside data->autoexec\r\n"
	"checkfe/fe - Re-executes the filtering-enabled notification which is visible on the bottom right of your game. Make sure to run with 'run' as the first argument, similar to exeurl\r\n"
	"tutorial - Re-plays the tutorial (sequence of message-boxes) you are provided upon first-time launch of Script-Ware\r\n"
	"cls - Clears out the output-field\r\n"
	"(you can run the following preset scripts: grabknife, rocky2u, dex, noobbat, zanoob, pyro - run them by using the name as an argument instead of the file)\r\n"
};

void handle_command(const std::string& command) {
	std::vector<std::string> arguments = split_string(command, ' '); //the second argument is what is splits the string from, first is what from basically take over

	if (arguments.at(0) == "cmds" || arguments.at(0) == "commands") {
		MessageBox(NULL, commands, "Script-Ware", MB_OK);
	}

	else if (arguments.at(0) == "run" || arguments.at(0) == "Run") {
		print_output(RGB(0, 0, 0), "\r\n> %s", command.c_str());

		/* game specific*/

		if (arguments.at(1) == "reach") {
			if (use_reach) {
				std::string reach_script = download_url("https://pastebin.com/raw/StnrHbtU");
				execute_script(rL, L, reach_script);
			}

			else {
				print_output(RGB(255, 0, 0), "\nERROR: Reach can only be used on Fencing!\r\n");
			}

			return;
		}

		if (arguments.at(1) == "admin") {
			if (use_admin) {
				std::string admin_script = download_url("https://pastebin.com/raw/r1SHDHfY");
				execute_script(rL, L, admin_script);
			}

			else {
				print_output(RGB(255, 0, 0), "\nERROR: Admin can only be used on Work at Pizza Place!\r\n");
			}

			return;
		}

		if (arguments.at(1) == "checkfe" || arguments.at(1) == "fe") {
			print_output(RGB(0, 0, 0), "\r\n");
			std::string check_fe_script = "if workspace.FilteringEnabled == true then errorconsole('Filtering is enabled') else printconsole('Filtering is disabled') end";
			execute_script(rL, L, check_fe_script);
			return;
		}

		if (arguments.at(1) == "exeurl" || arguments.at(1) == "url") {
			std::string url = arguments.at(2);
			std::string raw_url = download_url(url);
			execute_script(rL, L, raw_url);
			return;
		}

		if (arguments.at(1) == "chat") {
			std::string chatsscript = download_url("https://raw.githubusercontent.com/azulx/ScriptWare/master/scripts/chatgui.lua");
			execute_script(rL, L, chatsscript);
			return;
		}

		if (arguments.at(1) == "rocky" || arguments.at(1) == "rocky2u") {
			std::string script = download_url("https://raw.githubusercontent.com/azulx/ScriptWare/master/scripts/rocky2u.lua");
			execute_script(rL, L, script);
			return;
		}

		if (arguments.at(1) == "iorb" || arguments.at(1) == "iOrb")
		{
			std::string script = download_url("https://raw.githubusercontent.com/azulx/ScriptWare/master/scripts/iorb.txt");
			execute_script(rL, L, script);
			return;
		}

		if (arguments.at(1) == "grabknife" || arguments.at(1) == "gknife") {
			std::string script = download_url("https://pastebin.com/raw/VL74xMcW");
			execute_script(rL, L, script);
			return;
		}

		if (arguments.at(1) == "dex") {
			std::string script = download_url("https://raw.githubusercontent.com/azulx/ScriptWare/master/scripts/dex.lua");
			execute_script(rL, L, script);
			return;
		}

		if (arguments.at(1) == "noobbat" || arguments.at(1) == "bat") {
			std::string script = download_url("https://raw.githubusercontent.com/azulx/ScriptWare/master/scripts/noobbat");
			execute_script(rL, L, script);
			return;
		}

		if (arguments.at(1) == "zanoob" || arguments.at(1) == "noob") {
			std::string script = download_url("https://raw.githubusercontent.com/azulx/ScriptWare/master/scripts/zanoob.lua");
			execute_script(rL, L, script);
			return;
		}

		if (arguments.at(1) == "pyro") {
			std::string script = download_url("https://raw.githubusercontent.com/azulx/ScriptWare/master/scripts/pyro.lua");
			execute_script(rL, L, script);
			return;
		}

		std::string arg_path = arguments[1];
		arg_path = "data\\scripts\\" + arg_path;
		char path[MAX_PATH];

		util::get_file(SW_DLL, arg_path.c_str(), path, MAX_PATH);

		std::string data;

		if (read_file(path, data, 0)) {
			execute_script(rL, L, data);
		}

		else {
			print_output(RGB(255, 0, 0), "\nUnable to run script: %s\n", path);
		}
	}

	else if (arguments.at(0) == "load" || arguments.at(0) == "Load") {
		print_output(RGB(0, 0, 0), "\r\n> %s", command.c_str());

		std::string arg_path = arguments[1];
		arg_path = "data\\scripts\\" + arg_path;
		char path[MAX_PATH];

		util::get_file(SW_DLL, arg_path.c_str(), path, MAX_PATH);

		std::string data;

		if (read_file(path, data, 0)) {
			SendMessage(script_field, SCI_SETTEXT, (WPARAM)"", (LPARAM)data.c_str());
		}

		else {
			print_output(RGB(255, 0, 0), "\nUnable to load script: %s\n", path);
		}
	}

	else if (arguments.at(0) == "tutorial") {
		tutorial();
	}

	else if (arguments.at(0) == "reautoexec") {
		auto_exec();
	}

	else if (arguments.at(0) == "cls" || arguments.at(0) == "Cls") {
		SendMessage(output_field, WM_SETTEXT, (WPARAM)"", (LPARAM)"");
		print_output(RGB(0, 0, 0), "Script-Ware v%s Loaded! ", SW_VERSION);
		print_output(RGB(0, 0, 0), "(initialization took: %fs)", time_took);
	}

	else {
		print_output(RGB(255, 0, 0), "\nInvalid command provided.");
	}
}

static LRESULT CALLBACK dll_window_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wp)) {

		case TXT_CMDINPUT:
		{
			if (HIWORD(wp) == EN_MAXTEXT)
			{
				char text[500];
				SendMessage((HWND)lp, WM_GETTEXT, 500, (LPARAM)text);

				if (strcmp(text, "") == 0)
					break;

				SendMessage((HWND)lp, WM_SETTEXT, NULL, (LPARAM)"");

				handle_command_r(text);
				break;
			}
			break;
		}

		case BUTTON_SUBMIT: {
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)initialize, 0, 0, 0);
			break;
		}

		case BUTTON_CLEAR_SCRIPTEDITOR:
		{
			SendMessage(script_field, WM_SETTEXT, (WPARAM)"", (LPARAM)"");
			break;
		}
		case BUTTON_OPEN_SCRIPT:
		{
			char file_name[MAX_PATH];

			OPENFILENAME ofn;
			ZeroMemory(&file_name, sizeof(file_name));
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = main_window;
			ofn.lpstrFilter = "Script (*.txt;*.lua)\0" "*.txt;*.lua\0" "All files (*.*)\0" "*.*\0\0";
			ofn.lpstrFile = file_name;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Select a File";
			ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn)) {
				std::string buffer;
				read_file(file_name, buffer, 0);
				SendMessage(script_field, SCI_SETTEXT, (WPARAM)"", (LPARAM)buffer.c_str()); //scintilla accepts strings in UTF-8 encoding not UTF-16
			}
			break;
		}
		case BUTTON_REMEMBERME:
		{
			if (!loadedscr)
			{
				MessageBox(NULL, "You're unable to store your data due to an error with the setting of files. Please re-launch script-ware and then attempt to re-do this", "Script-Ware", MB_ICONERROR | MB_OK);
				break;
			}

			std::string username_filename = "USERNAME.txt";
			std::string password_filename = "PASSWORD.txt";

			static const char* password_buffer = new char[64];
			static const char* username_buffer = new char[64];

			int len = GetWindowTextLength(password_field) + 1;
			GetWindowText(password_field, (LPSTR)password_buffer, len);

			int len1 = GetWindowTextLength(username_field) + 1;
			GetWindowText(username_field, (LPSTR)username_buffer, len1);

			std::string encoded_password = base64_encode(password_buffer);

			std::string file;
			file += "C:\\SW_AUTH\\";
			file += username_filename;

			std::string file_1;
			file_1 += "C:\\SW_AUTH\\";
			file_1 += password_filename;

			std::ofstream createfile(file.c_str());
			std::ofstream createfile1(file_1.c_str());

			createfile << username_buffer;
			createfile1 << encoded_password;

			break;
		}
		case BUTTON_EXECUTE_SCRIPT:
		{
			int len = GetWindowTextLength(script_field) + 1;
			auto buffer = new char[len];
			GetWindowText(script_field, buffer, len);

			execute_script(rL, L, buffer);

			delete[] buffer;
			break;
		}

		case BUTTON_DEBUGMODE:
		{
			bool ischecked = true;

			if (ischecked) {
				MessageBox(NULL, "DebugMode essientally displays a list of the operations which are currently taking place throughout your use. You can take advantage of this and use this to find out the where abouts of where you're encountering crashing/issues or prolonged wait times during your use of Script-Ware.", "Script-Ware", MB_ICONINFORMATION | MB_OK);
				ischecked = false;
				debug_mode = true;
				break;
			}

			//its not checked anymore nigga
			ischecked = true;
			break;
		}

		case MENU_EXIT: {
			if (MessageBox(NULL, "Are you sure you want to close Script-Ware? Roblox will close with it.", "Confirmation", MB_ICONWARNING | MB_YESNO) == IDYES) {
				exit(0);
			}
			break;
		}
		case MENU_DEATTACH:
		{
			if (MessageBox(NULL, "Are you sure you want to close Script-Ware? This will close Script-Ware but not Roblox", "Confirmation", MB_ICONWARNING | MB_YESNO) == IDYES) {
				ExitThread(0);
			}
			break;
		}
		case MENU_ABOUT:
		{
			MessageBox(NULL, "Script-Ware | The unrestricted script execution product\n\nWritten and developed by AzuLX and Quad Core", "Script-Ware", MB_ICONINFORMATION | MB_OK);
			break;
		}
		default:
			return DefWindowProc(hwnd, msg, wp, lp);
		}

		break;
	}

	case WM_NOTIFY:
	{
		ScintillaFunctions::LineChanged((SCNotification*)(lp));
		break;
	}

	case WM_DESTROY:
	{

		exit(0);
	}
	case WM_QUIT:
	{
		exit(0);
	}
	default:
		return DefWindowProc(hwnd, msg, wp, lp);
	}
	return 0;
}

static BOOL register_window_class(const char* wclassname) {
	WNDCLASSEX nclass;

	nclass.cbSize = sizeof(WNDCLASSEX);
	nclass.style = CS_DBLCLKS;
	nclass.lpfnWndProc = dll_window_proc;
	nclass.cbClsExtra = 0;
	nclass.cbWndExtra = 0;
	nclass.hInstance = GetModuleHandle(NULL);
	nclass.hIcon = LoadIcon(0, IDI_APPLICATION);
	nclass.hIconSm = LoadIcon(0, IDI_APPLICATION);
	nclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	nclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	nclass.lpszMenuName = "ScriptWare";
	nclass.lpszClassName = wclassname;

	if (!RegisterClassEx(&nclass))
		return 0;

	return 1;
}

void open_main() {
	ShowWindow(main_load_window, SW_HIDE);
	ShowWindow(main_window, SW_SHOW);
}

void progress_bar(int val) {
	Sleep(100);
	SendMessage(progressbar, PBM_SETPOS, val, 0);
}

void remove_style(HWND hwnd, int style) {
	LONG lex_style = GetWindowLong(hwnd, GWL_EXSTYLE);
	lex_style &= ~(style);
	SetWindowLong(hwnd, GWL_EXSTYLE, lex_style);
}

HWND create_tool_tip(HWND hwnd_tool, PTSTR psz_text)
{
	if (!hwnd_tool || !psz_text)
	{
		return FALSE;
	}

	HWND tool_tip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_ALWAYSTIP,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hwnd_tool, NULL,
		NULL, NULL);

	if (!hwnd_tool || !tool_tip)
	{
		return (HWND)NULL;
	}

	TOOLINFO tool_tip_info = { 0 };
	tool_tip_info.cbSize = sizeof(tool_tip_info);
	tool_tip_info.hwnd = hwnd_tool;
	tool_tip_info.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	tool_tip_info.uId = (UINT_PTR)hwnd_tool;
	tool_tip_info.lpszText = psz_text;
	SendMessage(tool_tip, TTM_ADDTOOL, 0, (LPARAM)&tool_tip_info);
	return tool_tip;
}

void create_toolbar_menu(HMENU hhmenu)
{
	HMENU scriptware_menu = CreatePopupMenu();

	AppendMenu(hhmenu, MF_POPUP, (UINT_PTR)scriptware_menu, TEXT("Script-Ware"));
	AppendMenu(scriptware_menu, MF_STRING, MENU_EXIT, TEXT("Exit"));
	AppendMenu(scriptware_menu, MF_STRING, MENU_DEATTACH, TEXT("De-Attach"));
	InsertMenu(scriptware_menu, 2, MF_SEPARATOR | MF_BYPOSITION, 0, NULL);
	AppendMenu(scriptware_menu, MF_STRING, MENU_ABOUT, TEXT("About"));
}

HMENU create_windowmenu()
{
	HMENU scriptware_menu = CreateMenu();

	if (scriptware_menu == NULL)
		return FALSE;

	create_toolbar_menu(scriptware_menu);
	return scriptware_menu;
}

void remember_me_functionality()
{
	check_authfolder();

	std::string username_filename = "USERNAME.txt";
	std::string password_filename = "PASSWORD.txt";

	std::string file;
	file += "C:\\SW_AUTH\\";
	file += username_filename;

	std::string file_1;
	file_1 += "C:\\SW_AUTH\\";
	file_1 += password_filename;

	std::string line;
	std::ifstream swfile(file);

	std::string line1;
	std::ifstream swfile1(file_1);


	if (swfile.is_open())
	{
		while (getline(swfile, line))
		{
			SendMessage(username_field, WM_SETTEXT, (WPARAM)"", (LPARAM)line.c_str());
		}
		swfile.close();
	}

	if (swfile1.is_open())
	{
		while (getline(swfile1, line1))
		{
			std::string decoded_password = base64_decode(line1);

			SendMessage(password_field, WM_SETTEXT, (WPARAM)"", (LPARAM)decoded_password.c_str());

		}
		swfile1.close();
	}
}

DWORD get_pc_name()
{
#define NAME_BUFFER_SIZE 30000 //some wham number
	TCHAR name_buffer[NAME_BUFFER_SIZE];
	DWORD bugger_char_count = NAME_BUFFER_SIZE;

	//print pc user name
	if (GetUserName(name_buffer, &bugger_char_count))
		return (long)name_buffer;
}

static void init_gui() {

	/* title declaring */
	if (is_lite)
	{
		sw_title = "Script-Ware | Lite Edition";
	}

	else if (is_extended)
	{
		sw_title = "Script-Ware | Extended Edition";
	}

	else
	{
		sw_title = "Script-Ware";
	}

	HINSTANCE hinstance = GetModuleHandle(0);

	UnregisterClass("ScriptWare", hinstance);

	register_window_class("ScriptWare");
	register_window_class("ScriptWareLoading");
	register_window_class("ScriptWareLogin");
	register_window_class("config_window");

	HMENU window_menu = create_windowmenu();

	LoadLibrary("Riched20.dll");
	LoadLibrary("SciLexer.dll");

	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_STANDARD_CLASSES;
	InitCommonControlsEx(&icex);

	HFONT segoe_font = CreateFont(16, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, "Segoe UI");
	HFONT sans_serif = CreateFont(15, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Sans Serif"));

	/* Login Screen */
	main_login_window = CreateWindowEx(0, "ScriptWareLogin", "Script-Ware", WS_SYSMENU | WS_MINIMIZEBOX, 100, 100, 316, 293, 0, 0, hinstance, 0);
	submit = CreateWindowEx(0, "BUTTON", "Submit", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 54, 131, 181, 23, main_login_window, (HMENU)BUTTON_SUBMIT, hinstance, 0);
	username_field = CreateWindowEx(0, "edit", USERNAME, WS_EX_CLIENTEDGE | WS_BORDER | WS_CHILD | WS_VISIBLE, 54, 79, 181, 20, main_login_window, NULL, hinstance, 0);
	password_field = CreateWindowEx(0, "edit", PASSWORD, ES_PASSWORD | WS_BORDER | WS_EX_CLIENTEDGE | WS_CHILD | WS_VISIBLE, 54, 105, 181, 20, main_login_window, (HMENU)TXT_PASSWORDFIELD, hinstance, 0);
	HWND label = CreateWindow(TEXT("static"), "Please enter your login credentials", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 51, 54, 199, 20, main_login_window, (HMENU)(501), hinstance, NULL);
	HWND label1 = CreateWindow(TEXT("static"), "Script-Ware - Developed by AzuLX and Quad Core", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 10, 245, 300, 190, main_login_window, (HMENU)(501), hinstance, NULL);
	check_box = CreateWindowEx(NULL, "BUTTON", "Remember Me", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 55, 155, 96, 17, main_login_window, (HMENU)BUTTON_REMEMBERME, hinstance, NULL);
	debug_checkbox = CreateWindowEx(NULL, "BUTTON", "Debug Mode", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 55, 170, 96, 17, main_login_window, (HMENU)BUTTON_DEBUGMODE, hinstance, NULL);

	/* Loading Screen */
	main_load_window = CreateWindowEx(0, "ScriptWareLoading", "Script-Ware", WS_SYSMENU, 100, 100, 316, 97, 0, 0, hinstance, 0);
	output = CreateWindowEx(0, "static", "", WS_VISIBLE | WS_CHILD, 16, 16, 490, 25, main_load_window, (HMENU)3, hinstance, NULL);
	progressbar = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE, 15, 41, 276, 20, main_load_window, NULL, hinstance, NULL);

	/* Main Screen */
	main_window = CreateWindowEx(0, "ScriptWare", sw_title.c_str(), WS_SYSMENU | WS_MINIMIZEBOX, 100, 100, 397, 362, 0, window_menu, hinstance, 0);
	script_field = CreateWindowEx(0, "scintilla", "", WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | WS_BORDER, 11, 9, 369, 159, main_window, 0, hinstance, 0);
	output_field = CreateWindowEx(0, RICHEDIT_CLASS, "", WS_CHILD | ES_READONLY | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | WS_BORDER, 11, 214, 369, 70, main_window, 0, hinstance, 0);
	execute = CreateWindowEx(0, "BUTTON", "Execute", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 11, 175, 119, 33, main_window, (HMENU)BUTTON_EXECUTE_SCRIPT, hinstance, 0);
	clear = CreateWindowEx(0, "BUTTON", "Clear", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 265, 175, 115, 33, main_window, (HMENU)BUTTON_CLEAR_SCRIPTEDITOR, hinstance, 0);
	open = CreateWindowEx(0, "BUTTON", "Open", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 136, 175, 123, 33, main_window, (HMENU)BUTTON_OPEN_SCRIPT, hinstance, 0);
	cmd_input_field = CreateWindowEx(0, "EDIT", "", WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VISIBLE, 12, 290, 369, 20, main_window, (HMENU)TXT_CMDINPUT, hinstance, 0);

	SendMessage(username_field, WM_SETFONT, (WPARAM)sans_serif, NULL);
	SendMessage(password_field, WM_SETFONT, (WPARAM)sans_serif, NULL);
	SendMessage(cmd_input_field, WM_SETFONT, (WPARAM)sans_serif, NULL);

	ScintillaFunctions::InitializeScintilla();
	SendMessage(script_field, SCI_SETMARGINWIDTHN, 0, 18);
	SendMessage(script_field, SCI_SETMARGINWIDTHN, 1, 0);

	SendMessage(script_field, SCI_STYLESETBOLD, SCE_LUA_WORD, TRUE);
	SendMessage(script_field, SCI_STYLESETBOLD, SCE_LUA_WORD2, TRUE);

	SendMessage(script_field, EM_SETLIMITTEXT, 0x7FFFFFFE, 0);
	remove_style(script_field, WS_EX_CLIENTEDGE);

	SendMessage(output_field, WM_SETFONT, (WPARAM)sans_serif, NULL);
	SendMessage(output_field, EM_SETLIMITTEXT, 0x7FFFFFFE, 0);
	remove_style(output_field, WS_EX_CLIENTEDGE);

	SendMessage(execute, WM_SETFONT, (WPARAM)segoe_font, NULL);
	SendMessage(clear, WM_SETFONT, (WPARAM)segoe_font, NULL);
	SendMessage(open, WM_SETFONT, (WPARAM)segoe_font, NULL);
	SendMessage(output, WM_SETFONT, (WPARAM)segoe_font, NULL);
	SendMessage(submit, WM_SETFONT, (WPARAM)segoe_font, NULL);
	SendMessage(label, WM_SETFONT, (WPARAM)segoe_font, NULL);
	SendMessage(label1, WM_SETFONT, (WPARAM)segoe_font, NULL);
	SendMessage(check_box, WM_SETFONT, (WPARAM)segoe_font, NULL);
	SendMessage(debug_checkbox, WM_SETFONT, (WPARAM)segoe_font, NULL);
	SendMessage(versionlabel, WM_SETFONT, (WPARAM)segoe_font, NULL);

	SetWindowPos(main_window, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOSIZE | SWP_NOMOVE);
	SetWindowPos(main_load_window, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOSIZE | SWP_NOMOVE);
	SetWindowPos(main_login_window, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOSIZE | SWP_NOMOVE);

	create_tool_tip(username_field, (char*)"Username field, enter your username here.");
	create_tool_tip(password_field, (char*)"Password field, enter your password here.");

	MSG msg;
	BOOL b_ret;

	ShowWindow(main_login_window, SW_SHOW);
	remember_me_functionality();

	EnableMenuItem(GetSystemMenu(main_load_window, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

	while ((b_ret = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if (b_ret == 0) {
			return;
		}
		else if (b_ret == -1)
		{
			return;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

}

/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
// begin scan/startup and other misc core related things

static int custom_printconsole(int rL) {
	std::string str = r_lua_tolstring(rL, -1, 0);
	str += "\r\n";

	print_output(RGB(0, 0, 0), str.c_str());
	return 0;
}

static int custom_errorconsole(int rL) {
	std::string str = r_lua_tolstring(rL, -1, 0);
	str += "\r\n";

	print_output(RGB(255, 0, 0), str.c_str());
	return 0;
}

static int teleport_handler(int rL) {
	//todo: make script-ware handle teleports to different servers by re-scanning
	EnableWindow(script_field, false);
	EnableWindow(cmd_input_field, false);
	EnableWindow(open, false);
	EnableWindow(execute, false);
	EnableWindow(clear, false);
	return 0;
}

static int rloadstring(int rL) {
	const char* script = r_lua_tolstring(rL, -1, 0);
	execute_script(rL, L, (PCHAR)script);
	return 0;
}

static int custom_runfile(int rbx_L) {
	const char * name = r_lua_tolstring(rbx_L, -1, 0);

	char path[MAX_PATH];
	util::get_file(SW_DLL, "", path, MAX_PATH);

	std::string file;
	file += path;
	file += "data\\savedscripts\\";
	file += name;

	std::ifstream swfile(file);
	std::string line;

	if (swfile.is_open())
	{
		while (getline(swfile, line))
		{
			execute_script(rbx_L, L, line);
		}
		swfile.close();
	}

	return 0;
}

static int custom_checkcaller(int rL) {
	r_lua_pushboolean(rL, true);
	return 1;
}

static int custom_getobjects(int rL) {
	VMProtectBeginMutation("getobjects");

	if (is_lite)
	{
		print_output(RGB(255, 0, 0), "ERROR: game:GetObjects(asset) is not avaliable on Script-Ware Lite!");
		return 1;
	}

	try {
		std::string asset = r_lua_tolstring(rL, -1, 0);
		R_LUA_POP(rL, 1);

		int buffer = 0;
		int asd[] = { 0, 0, 0, 0, 0, 0 };
		int asd1[] = { 0, 0, 0, 0, 0, 0 };
		int asd2[] = { 0, 0, 0, 0, 0, 0 };


		int content = r_getcontent(content_provider, &buffer, &asset, (int)asd, (int)asd1, (int)asd2);
		std::istream* result = *(std::istream**)(content);
		auto* instance_vector = new std::vector<instance>;

		r_load_instances((int)asset.c_str(), result, instance_vector);

		for (auto yeet : *instance_vector)
		{
			r_push_object(rL, yeet.ptr, yeet.ext);
		}

		int size = instance_vector->size();
		return size;
	}

	catch (std::exception)
	{
		print_output(RGB(255, 0, 0), "GetObjects encountered an exception while running. It's meant to be ran with a string as the argument (game:GetObjects(string asset))");
	}

	catch (...)
	{
		print_output(RGB(255, 0, 0), "GetObjects encountered an exception while running. It's meant to be ran with a string as the argument (game:GetObjects(string asset))");
	}

	VMProtectEnd();
}

void initialize() {
	XorS(a, "https://hastebin.com/raw/alisiduzog");

	VMProtectBeginVirtualization("auth_startup");

	EnableWindow(password_field, false);
	EnableWindow(username_field, false);
	EnableWindow(submit, false);

	int p_length = GetWindowTextLength(password_field) + 1;
	GetWindowText(password_field, (LPSTR)pass_buffer, p_length);

	int u_length = GetWindowTextLength(username_field) + 1;
	GetWindowText(username_field, (LPSTR)user_buffer, u_length);

	authorize(user_buffer, pass_buffer);

	if (!auth_success) {
		throw std::exception("bad auth");
	}

	ShowWindow(main_login_window, SW_HIDE);
	ShowWindow(main_load_window, SW_SHOW);

	VMProtectEnd();

	output_label("ScriptWare: Loading...");

	L = lua_open(); //initialize the vanilla lua state

	if (!L) {
		ERRORMSGBOX("Script-Ware", "Failed to initialize the lua state.");
		exit(1);
	}

	if (!is_wow64()) {
		output_label("ScriptWare: 32-bit system detected!");
		ERRORMSGBOX("Script-Ware currently does not support 32-bit systems. Please click OK to exit.", "Script-Ware");
		exit(0);
	}

	if (debug_mode) {
		output_label("ScriptWare: Beginning Scan... ");
	}

	main();

	if (MH_Initialize() != MH_OK)
	{
		ERRORMSGBOX("ERROR: Failed to initialize the MinHook library, please re-open Script-Ware", "Script-Ware");
		exit(0);
	}

	if (debug_mode)
		output_label("ScriptWare: Creating hooks for bypasses");

	Sleep(300);

	hook = winapi_hook;
	create_event_hook((DWORD)lua_jmpbackhandler);

	if (debug_mode)
		output_label("ScriptWare: Creating custom implementations to script-ware's environment");

	r_lua_pushnumber(rL, (double)(DWORD)custom_toclipboard);
	r_lua_pushcclosure(rL, (rlua_cclosure)hook, 1);
	r_lua_setfield((int)rL, LUA_GLOBALSINDEX, "toclipboard");

	r_lua_pushnumber(rL, (double)(DWORD)rloadstring);
	r_lua_pushcclosure(rL, (rlua_cclosure)hook, 1);
	r_lua_setfield((int)rL, LUA_GLOBALSINDEX, "loadstring");

	r_lua_pushnumber(rL, (double)(DWORD)custom_printconsole);
	r_lua_pushcclosure(rL, (rlua_cclosure)hook, 1);
	r_lua_setfield((int)rL, LUA_GLOBALSINDEX, "printconsole");

	r_lua_pushnumber(rL, (double)(DWORD)custom_checkcaller);
	r_lua_pushcclosure(rL, (rlua_cclosure)hook, 1);
	r_lua_setfield((int)rL, LUA_GLOBALSINDEX, "checkcaller");

	r_lua_pushnumber(rL, (double)(DWORD)custom_errorconsole);
	r_lua_pushcclosure(rL, (rlua_cclosure)hook, 1);
	r_lua_setfield((int)rL, LUA_GLOBALSINDEX, "errorconsole");

	r_lua_pushnumber(rL, (double)(DWORD)custom_getobjects);
	r_lua_pushcclosure(rL, (rlua_cclosure)hook, 1);
	r_lua_setfield((int)rL, LUA_GLOBALSINDEX, "GetObjects");

	r_lua_pushnumber(rL, (double)(DWORD)custom_writefile);
	r_lua_pushcclosure(rL, (rlua_cclosure)hook, 1);
	r_lua_setfield((int)rL, LUA_GLOBALSINDEX, "writefile");

	r_lua_pushnumber(rL, (double)(DWORD)teleport_handler);
	r_lua_pushcclosure(rL, (rlua_cclosure)hook, 1);
	r_lua_setfield((int)rL, LUA_GLOBALSINDEX, "teleporthandler");

	r_lua_pushnumber(rL, (double)(DWORD)custom_readfile);
	r_lua_pushcclosure(rL, (rlua_cclosure)hook, 1);
	r_lua_setfield((int)rL, LUA_GLOBALSINDEX, "readfile");

	r_lua_pushnumber(rL, (double)(DWORD)custom_runfile);
	r_lua_pushcclosure(rL, (rlua_cclosure)hook, 1);
	r_lua_setfield((int)rL, LUA_GLOBALSINDEX, "runfile");

	r_lua_pushnumber(rL, (double)(DWORD)custom_getrawmetatable);
	r_lua_pushcclosure(rL, (rlua_cclosure)hook, 1);
	r_lua_setfield((int)rL, LUA_GLOBALSINDEX, "getrawmetatable");

	if (debug_mode)
		output_label("ScriptWare: Executing scripts held inside data->autoexec");

	if (debug_mode)
		output_label("ScriptWare: Init script loading");

	progress_bar(100);
	Sleep(550); //to see that its done, aesthetic

	ShowWindow(main_load_window, SW_HIDE);
	ShowWindow(main_window, SW_SHOW);

	print_output(RGB(0, 0, 0), "Script-Ware v%s Loaded!\n", SW_VERSION);

	if (auth_firsttime) {
		if (MessageBox(NULL, "Would you like to view the Script-Ware tutorial documentation?", "Script-Ware", MB_ICONINFORMATION | MB_YESNO) == IDYES) {
			tutorial();
			print_output(RGB(0, 0, 0), "Tutorial concluded. Welcome to Script-Ware!");
		}
	}

	check_game(rL);
	R_CLEAR_STACK(rL);
}

DWORD WINAPI main() {
	progress_bar(25);

	try {
		int vf_table = addresses::scriptcontext;
		script_context = memory::scan(PAGE_READWRITE, (char*)&vf_table, (char*)"xxxx");

		if (debug_mode)
			output_label("ScriptWare: Initializing lua state");

		rL = (script_context + LocalScriptGlobalState) - *(int*)(script_context + LocalScriptGlobalState);

		if (debug_mode) {
			begin_console((PCHAR)"Script-Ware Debug Console");

			printf("ScriptContext: %X\n", script_context);
			printf("Lua State: %X\n", rL);

			printf("Debug Initialized!\r\n");
		}

		if (debug_mode)
			output_label("ScriptWare: Declaring contentprovider");

		r_lua_getglobal(rL, "game");
		r_lua_getfield(rL, -1, "ContentProvider");
		content_provider = rlua_touserdata(rL, -1);
		R_LUA_POP(rL, 2);
	}

	catch (...) {
		MessageBox(NULL, "Exception handled during main scan. Please try again.", "Script-Ware", MB_ICONERROR | MB_OK);
		exit(0);
	}

	if (debug_mode)
		output_label("ScriptWare: Concluded Scan");

	progress_bar(35);
	return 0;
}

void check_game(int rL) {
	r_lua_getglobal(rL, "game");
	r_lua_getfield(rL, -1, "PlaceId");
	double place_id = r_lua_tonumber(rL, -1);

	if (place_id == 12109643)
	{
		print_output(RGB(0, 0, 255), "Welcome to Fencing. You are able to use 'run reach' here!\r\n");
		use_reach = true;
	}

	else if (place_id == 1600503)
	{
		print_output(RGB(0, 0, 255), "Welcome to Apocolypse Rising. You are able to use 'run items' here!\r\n");
		use_getitems = true;
	}

	else if (place_id == 192800)
	{
		print_output(RGB(0, 0, 255), "Welcome to Work At A Pizza Place. You are able to use 'run admin' here!\r\n");
		use_admin = true;
	}

	else if (place_id == 221813725) {
		print_output(RGB(0, 0, 255), "Welcome to Shinobi Life. You are able to use 'run money' and 'run levels' here!\r\n");
	}
}

static void erase_peheaders() { //should stop most memory dumpers, thanks brandon
	VMProtectBeginMutation("erase pe");
	HMODULE hdll;
	char cpath[MAX_PATH];
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)split_string, &hdll);
	GetModuleFileName(hdll, cpath, MAX_PATH);

	DWORD old;
	VirtualProtect(hdll, 0x500, PAGE_READWRITE, &old);
	memset(hdll, 0, 0x500);
	VirtualProtect(hdll, 0x500, old, &old);
	VMProtectEnd();
}

BOOL WINAPI DllMain(HMODULE Dll, DWORD Reason, PVOID Reserved) {
	if (Reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(Dll);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)erase_peheaders, NULL, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)init_gui, NULL, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)stop_roblox_input, NULL, NULL, NULL);
	}
	return TRUE;
}
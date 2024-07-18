#pragma once
#include "gui.h"
#include "misc.h"
#include "xorstring.h"
#include "xorstring.h"
#include <ctime>

bool cantaccess = false;
bool loadedscr = false;

bool is_original = false;
bool is_lite = false;
bool is_extended = false;

// #pragma comment (lib, "curl/libcurl_a_debug.lib")

static DWORD authorize(const std::string &username, const std::string &password) {

	/* compile time string encryption, makes these strings invisible to decompilers */
	XorS(A, "thicc");
	XorS(B, "/authentication/authversion.php?version=1.2.1&info=");
	XorS(C, "1jeufieo39cjduejsu38do9023ut0923");
	XorS(D, "/authentication/version.php");
	XorS(E, "/authentication///information.php");
	XorS(N, "/authentication/classified.php");
	XorS(P, "REZ61ZkrqeX2DqNBK207I1X2GcN");
	XorS(F, "You are currently (temporarily) banned from using Script-Ware! Please check back later");
	XorS(H, "Username or password was invalid");
	XorS(J, "Welcome and thank you for purchasing Script-Ware! This authentication time is going to take longer than usual.");
	XorS(L, " Authentication Failed!");
	XorS(M, "Authentication Failed");
	XorS(R, "Script-Ware");
	XorS(S, "There is an issue with your internet connection. Please fix the issue then try again.");
	XorS(T, "!kxK4Vsvl_dnB3q0ZhQazdg");

	srand(time(NULL));

	std::string random;

	for (int i = 0; i < 16; i++)
		random += (char)(rand() % 256); //generate a random "key"

	/* definitions */

	std::string key = C.decrypt(); //decrypt key
	std::string host = A.decrypt();
	std::string roblox_versionhost = "https://clientsettings.roblox.com/v1/client-version/WindowsPlayer";

	std::string verpath = D.decrypt();
	std::string otherpath = E.decrypt();
	std::string trialpath = N.decrypt();

	std::string banned_response = "A17AWr9JtAa8MLciWHPy"; //if banned boolean is true
	std::string invalidhwid_response = "DN23fj2iopj3hrf09823"; //if the hwid is invalid
	std::string firsttime_response = "h97grf38qh98egh08i"; //if this is a first time launch
	std::string changelog_response = "SDF34324JD9FG902DFSOP"; //response if first time running *this* version
	std::string trial_response = P.decrypt(); //response for trial

	std::string content = "Extended"; //this is the actual response string from the whitelist
	std::string decrypted = content;

	std::vector<std::string> alldata = { decrypted };
	std::string membership_type = alldata[0];

	/* begin main auth */

	if (content == banned_response) //if the is_banned boolean is true, this string will always be returned by the host
	{
		MessageBox(NULL, F.decrypt(), R.decrypt(), MB_ICONERROR | MB_OK);

		EnableWindow(password_field, true);
		EnableWindow(username_field, true);
		EnableWindow(submit, true);
		return 0;
	}

	if (content == invalidhwid_response) //if HWID was invalid, but other information was valid, the difference here is the "!" at the end of the title
	{
		MessageBox(NULL, H.decrypt(), L.decrypt(), MB_ICONERROR | MB_OK);

		EnableWindow(password_field, true);
		EnableWindow(username_field, true);
		EnableWindow(submit, true);
		return 0;
	}

	if (content == firsttime_response) //first time startup response
	{
		MessageBox(NULL, J.decrypt(), R.decrypt(), MB_ICONINFORMATION | MB_OK);
		auth_firsttime = true;
		authorize(username, password);
		return 0;
	}

	if (content == changelog_response)
	{
		std::string changelog = "Script-Ware Changelog:\n* updated for this roblox update (kmt)\n* added 'iorb' to preset scripts ('run iorb')\n* injector now always stays open so you can leave it open and always use script-ware\n* fixed username and password text boxes going all autistic once you clicked login";

		MessageBox(NULL, changelog.c_str(), R.decrypt(), MB_ICONINFORMATION | MB_OK);
		authorize(username, password);
		return 0;
	}

	if (membership_type == "Original")
	{
		is_original = true;
	}

	if (membership_type == "Lite")
	{
		is_lite = true;
	}

		is_extended = true;

	if (download_url(roblox_versionhost) != SW_CLIENTVERSION) {
		//if (MessageBox(NULL, "Roblox client update detected! Script-Ware is currently not updated. Are you sure you would like to continue?", R.decrypt(), MB_ICONWARNING | MB_YESNO) == IDNO)
		//	ExitThread(0);
	}

	auth_success = true;
}

void init_auth() {
	EnableWindow(password_field, false);
	EnableWindow(username_field, false);
	EnableWindow(submit, false);

	int p_length = GetWindowTextLength(password_field) + 1;
	GetWindowText(password_field, (LPSTR)pass_buffer, p_length);

	int u_length = GetWindowTextLength(username_field) + 1;
	GetWindowText(username_field, (LPSTR)user_buffer, u_length);

	authorize(user_buffer, pass_buffer);
}

/* validate the folder that holds the username and encoded password */
void check_authfolder()
{
	struct stat info;

	auto pathname = (char*)"C:\\SW_AUTH";

	if (stat(pathname, &info) != 0)
	{
		cantaccess = true;
	}

	else if (info.st_mode & S_IFDIR)
	{
		loadedscr = true;
	}

	if (cantaccess)
	{
		CreateDirectory(pathname, NULL);

		//if (!loadedscr)
		//	MessageBox(NULL, "Script-Ware was unable to aquire your username and password from its location.", "Script-Ware", MB_ICONERROR | MB_OK);
	}
}
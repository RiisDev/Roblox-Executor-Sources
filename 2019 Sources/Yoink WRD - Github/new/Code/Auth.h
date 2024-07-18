#pragma once
#include "Scanner.h"
#include "Auth/SHA/sha.h"
#include "XorS.h"


string ReadRegValue(HKEY root, string key, string name)
{
	HKEY hKey;
	if (RegOpenKeyEx(root, key.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
		throw "Could not open registry key";

	DWORD type;
	DWORD cbData;
	if (RegQueryValueEx(hKey, name.c_str(), NULL, &type, NULL, &cbData) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		throw "Could not read registry value";
	}

	if (type != REG_SZ)
	{
		RegCloseKey(hKey);
		throw "Incorrect registry value type";
	}

	string value(cbData / sizeof(wchar_t), L'\0');
	if (RegQueryValueEx(hKey, name.c_str(), NULL, NULL, reinterpret_cast<LPBYTE>(&value[0]), &cbData) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		throw "Could not read registry value";
	}

	RegCloseKey(hKey);

	size_t firstNull = value.find_first_of('\0');
	if (firstNull != string::npos)
		value.resize(firstNull);

	return value;
}

bool AUTH() {
	try {
		XorS(Path, "Software\\YoinkAuth");
		XorS(Ha, "16554b69ab3919edac42813ebd5ab1ec355f6669c0b4f4668e559fc5f343fde7");
		auto Identity = ReadRegValue(HKEY_CURRENT_USER, Path.decrypt(), "Auth");
		string Hashed = sha256(string(Identity.c_str()));
		if (Hashed != Ha.decrypt()) {
			/*Scanner::Process::Kill("RobloxPlayerBeta.exe");
			exit(0);
			PostQuitMessage(0);*/
			system("start https://wearedevs.net/d/Yoink%20Executor");

			return false;
		}
		return true;
	}
	catch (exception e) {
		Scanner::Process::Kill("RobloxPlayerBeta.exe");
		exit(0);
		PostQuitMessage(0);
	}
	return false;
}

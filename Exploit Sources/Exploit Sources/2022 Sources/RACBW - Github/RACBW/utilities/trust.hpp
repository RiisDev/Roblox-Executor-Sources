#pragma once

#include <cstdint>
#include <Windows.h>
#include <Psapi.h>

#include <WinTrust.h>
#include <wincrypt.h>
#include <Softpub.h>
#pragma comment (lib, "wintrust")

inline bool is_signed( HANDLE roblox_handle, std::uintptr_t address )
{
	wchar_t name [ MAX_PATH ];
	GetModuleFileNameExW( roblox_handle, reinterpret_cast< HMODULE >( address ), name, MAX_PATH );

	GUID policy_guid = WINTRUST_ACTION_GENERIC_VERIFY_V2;
	WINTRUST_DATA wintrustdata { };

	WINTRUST_FILE_INFO file_info { };
	file_info.cbStruct = sizeof( WINTRUST_FILE_INFO );
	file_info.pcwszFilePath = name;

	wintrustdata.cbStruct = sizeof( WINTRUST_DATA );
	wintrustdata.dwUIChoice = WTD_UI_NONE;
	wintrustdata.fdwRevocationChecks = WTD_REVOKE_NONE;
	wintrustdata.dwUnionChoice = WTD_CHOICE_FILE;
	wintrustdata.dwStateAction = WTD_STATEACTION_VERIFY;
	wintrustdata.pFile = &file_info;

	const auto valid = WinVerifyTrust( nullptr, &policy_guid, &wintrustdata ) == ERROR_SUCCESS;

	wintrustdata.dwStateAction = WTD_STATEACTION_CLOSE;
	WinVerifyTrust( nullptr, &policy_guid, &wintrustdata );

	return valid;

}
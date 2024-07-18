
#include "security.hpp"
#include "Libraries/HashPP/sha3.h"
#include <comdef.h>
#include <WbemIdl.h>
#include <vector>
#include "Libraries/curl/cpr/cpr.h"
#pragma comment(lib, "wbemuuid.lib")
#define AUTO auto

DWORD DH_SCRIPTC;

class bytestream
{
private:
	std::stringstream ss;
public:
	void write(void* buff, size_t len)
	{
		ss.write((const char*)buff, len);
	}
	void read(void* buff, size_t len)
	{
		ss.read((char*)buff, len);
	}
	size_t size()
	{
		ss.seekg(0, std::ios::end);
		size_t size = ss.tellg();
		ss.seekg(0, std::ios::beg);
		return size;
	}
	bytestream()
	{
		ss = std::stringstream();
	}
};

PACKED_STRING* danghui_encrypt(STRING Text, size_t Length)
{
	PACKED_STRING* secstr = new PACKED_STRING;
	secstr->String = (STRING)(xxtea_encrypt(Text, Length, VMProtectDecryptStringA(DH_XXTEAKEY_CLIENT_KEY), &secstr->StringLength));
	return secstr;
}

PACKED_STRING* danghui_decrypt(STRING Text, size_t Length)
{
	PACKED_STRING* secstr = new PACKED_STRING;
	secstr->String = (STRING)(xxtea_decrypt(Text, Length, VMProtectDecryptStringA(DH_XXTEAKEY_SERVER_KEY), &secstr->StringLength));
	return secstr;
}

STRING danghui_makehwid()
{
	DWORD CpuHash = NULL;
	bytestream bs = bytestream();
	bs.write(DH_HEADER, sizeof(DH_HEADER));
#pragma region CPUID HWID Grab
	DWORD tEAX, tEBX, tECX, tEDX;
	__asm mov eax, 1
	__asm cpuid
	__asm
	{
		mov tEAX, eax
		mov tEBX, ebx
		mov tECX, ecx
		mov tEDX, edx
	}
	CpuHash = (tEAX * tEDX) + (tEBX ^ tECX);
	bs.write(&CpuHash, sizeof(DWORD));
#pragma endregion

#pragma region WMI Motherboard Serial Grab
	IWbemLocator* pLoc = NULL;
	IWbemServices* pSvc = NULL;
	IEnumWbemClassObject* pEnumerator = NULL;
	IWbemClassObject *pclsObj = NULL;
	VARIANT Win32BBSerial;
	std::wstring WSString;

	CoInitializeEx(0, COINIT_MULTITHREADED);
	CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
	pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, NULL, NULL, NULL, NULL, &pSvc);
	CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	pSvc->ExecQuery(bstr_t("WQL"), bstr_t("SELECT * FROM Win32_BaseBoard"), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);
	pclsObj->Get(L"SerialNumber", NULL, &Win32BBSerial, NULL, NULL);
	WSString = Win32BBSerial.bstrVal;
	bs.write((void*)WSString.c_str(), lstrlenW(WSString.c_str()));
	pclsObj->Release();
	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();
#pragma endregion

#pragma region Stream Hashing using SHA-3 :P
	SHA3 SHA3Obj = SHA3(SHA3::Bits512);
	std::string SHA3Hash;
	PVOID RawHWID;
	PVOID EncryptedHWID;

	RawHWID = VirtualAlloc(NULL, bs.size(), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	bs.read(RawHWID, bs.size()); /* just read the whole stream into the buffer ok */
	SHA3Obj.add(RawHWID, bs.size());
	SHA3Hash = SHA3Obj.getHash();
#pragma endregion

	return (STRING)(SHA3Hash.c_str());
}

PACKED_STRING* danghui_stagehwid(DWORD ServerKey)
{
	STRING HWID = danghui_makehwid();
	PACKED_STRING* eHWID = danghui_encrypt(HWID, strlen(HWID));
	for (ULONG x = 0; x < eHWID->StringLength; x++)
		eHWID->String[x] ^= ServerKey;
	return eHWID;
}

PACKED_STRING* danghui_unstageresponse(PACKED_STRING* Response, DWORD ServerKey)
{
	for (ULONG x = 0; x < Response->StringLength; x++)
		Response->String[x] ^= ServerKey;
	PACKED_STRING* DecryptedResponse = danghui_decrypt(Response->String, Response->StringLength);
	return DecryptedResponse;
}

BOOL danghui_authenticate()
{
	VMProtectBeginUltra("Authentication");
	PACKED_STRING* StagedHWID;
	PACKED_STRING* UnstagedResponse;
	DWORD ServerKey;

#pragma region Obtain Server Key
	AUTO HostURI = cpr::Url { DH_HOSTURL };
	cpr::Session KeySession;
	std::vector<cpr::Pair> PostData;
	PostData.push_back({ "hst", "getkey" });
	KeySession.SetPayload(cpr::Payload(PostData.begin(), PostData.end()));
	KeySession.SetUrl(HostURI);
	AUTO Response = KeySession.Post();
	ServerKey = *(DWORD*)(Response.text.c_str());
	if (!ServerKey)
		goto Untruthful;
#pragma endregion

#pragma region HWID Authentication
	cpr::Session AuthSession;
	std::vector<cpr::Pair> AuthData;
	StagedHWID = danghui_stagehwid(ServerKey);
	AuthData.push_back({ "hst", "auth" });
	AuthData.push_back({ "dat", StagedHWID->String });
	AuthData.push_back({ "len", StagedHWID->StringLength });
	AuthSession.SetPayload(cpr::Payload(AuthData.begin(), AuthData.end()));
	AuthSession.SetUrl(HostURI);
	AUTO AuthResponse = KeySession.Post();
	UnstagedResponse = danghui_decrypt((STRING)AuthResponse.text.c_str(), AuthResponse.text.size());
	if (!UnstagedResponse || !UnstagedResponse->String)
		goto Untruthful;
	if (memcmp(UnstagedResponse->String, DH_HEADER, sizeof(DH_HEADER)) != NULL)
		goto Untruthful;
	DH_SCRIPTC = *(DWORD*)(UnstagedResponse->String + sizeof(DH_HEADER));
	return TRUE;
#pragma endregion

Untruthful:
	VMProtectEnd();
	return FALSE;
}

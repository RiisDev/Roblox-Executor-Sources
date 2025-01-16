
#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>

#include "easywsclient/easywsclient.hpp"
#include <random>


#include "Execution.hpp"

#include "D3d11hook.hpp"
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

extern "C" __declspec(dllexport) LRESULT NextHook(int code, WPARAM wParam, LPARAM lParam) { return CallNextHookEx(nullptr, code, wParam, lParam); }

enum OpCode
{
	WS_NOP = 77,
	WS_EXEC,
	WS_SETFPS,
	WS_MSG
};

using easywsclient::WebSocket;
static WebSocket::pointer ws = nullptr;

void handleMessage(const std::string& message)
{
	if (message.empty()) {
		return;
	}

	OpCode op = static_cast<OpCode>(message[0]);
	const auto plain = message.substr(1);

	try {
		switch (op)
		{
		case OpCode::WS_EXEC:
			if (TaskScheduler::is_loaded())
			{
				coderun_queue.push(plain);
			}
			break;
		case OpCode::WS_MSG:

			break;
		case OpCode::WS_SETFPS: // TODO
		case OpCode::WS_NOP:
		default:
			break;
		}
	}
	catch (const std::exception& e) {
	}
	catch (...) {
	}
}

bool websocket_listening = false;

void socketLoop()
{
	while (websocket_listening)
	{
		try {
			if (!ws) {
				ws = WebSocket::from_url(xorstr_("ws://localhost:3080"));
				if (!ws) {
					std::this_thread::sleep_for(std::chrono::seconds(1));
					continue;
				}
			}


			while (websocket_listening && ws->getReadyState() != WebSocket::CLOSED)
			{
				ws->poll();
				ws->dispatch(handleMessage);

				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}


			delete ws;
			ws = nullptr;

		}
		catch (const std::exception& e) {
			if (ws) {
				delete ws;
				ws = nullptr;
			}
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		catch (...) {
			if (ws) {
				delete ws;
				ws = nullptr;
			}
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
}

void setupWebsocket()
{
	websocket_listening = true;
	std::thread t(socketLoop);
	t.detach();
}

void UnloadWebsocket()
{
	websocket_listening = false;
	if (ws) {
		ws->close();
		delete ws;
		ws = nullptr;
	}
}

void pipe()
{
	std::string ScriptBuffer;
	char Buffer[999999];
	DWORD ReadWords;
	void* PipeHandle = CreateNamedPipe(TEXT(xorstr_("\\\\.\\pipe\\atlantis_pipe")), PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, PIPE_WAIT, 1, 999999, 999999, NMPWAIT_USE_DEFAULT_WAIT, 0);
	while (true)
	{
		if (ConnectNamedPipe(PipeHandle, 0))
		{
			while (ReadFile(PipeHandle, Buffer, sizeof(Buffer) - 1, &ReadWords, 0))
			{
				Buffer[ReadWords] = '\0';
				ScriptBuffer.append(Buffer);
			}

			if (!TaskScheduler::is_loaded())
			{
				ScriptBuffer.clear();
			}
			else
			{
				coderun_queue.push(ScriptBuffer);
				ScriptBuffer.clear();
			}

			ScriptBuffer.clear();
		}
		DisconnectNamedPipe(PipeHandle); 
	}
}

__forceinline std::string readKey() {
	const std::string path = std::string(getenv("LOCALAPPDATA")) + "\\a_auth.bin"; 
	if (!std::filesystem::exists(path)) {
		TerminateProcess(GetCurrentProcess(), 0);
	}
	std::ifstream f(path, std::ios::in | std::ios::binary);
	const auto sz = std::filesystem::file_size(path);
	std::string result(sz, '\0');
	f.read(result.data(), sz);

	return result;
}

std::string GetProcessorId()
{
	//int cpuInfo[4] = { -1 };
	//__cpuid(cpuInfo, 0);
	//char idBuffer[17] = { 0 };
	//sprintf_s(idBuffer, "%08X%08X", cpuInfo[3], cpuInfo[0]);
	//return std::string(idBuffer);
	std::string processorId = "";

	HRESULT hres;

	// Initialize COM library
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres))
		return "";

	// Set general COM security levels
	hres = CoInitializeSecurity(
		NULL,
		-1,                          // COM authentication
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,    // Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE,  // Default Impersonation
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL                         // Reserved
	);

	if (FAILED(hres))
	{
		CoUninitialize();
		return "";
	}

	// Obtain the initial locator to WMI
	IWbemLocator* pLoc = NULL;
	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID*)&pLoc);

	if (FAILED(hres))
	{
		CoUninitialize();
		return "";
	}

	// Connect to WMI
	IWbemServices* pSvc = NULL;
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"),
		NULL, NULL, 0,
		NULL, 0, 0, &pSvc);

	if (FAILED(hres))
	{
		pLoc->Release();
		CoUninitialize();
		return "";
	}

	// Set security levels on the proxy
	hres = CoSetProxyBlanket(
		pSvc,
		RPC_C_AUTHN_WINNT,
		RPC_C_AUTHZ_NONE,
		NULL,
		RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE
	);

	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return "";
	}

	// Use the IWbemServices pointer to make requests of WMI
	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT ProcessorId FROM Win32_Processor"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return "";
	}

	// Get the data from the query
	IWbemClassObject* pclsObj = NULL;
	ULONG uReturn = 0;

	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
		if (0 == uReturn)
			break;

		VARIANT vtProp;
		hr = pclsObj->Get(L"ProcessorId", 0, &vtProp, 0, 0);
		if (SUCCEEDED(hr))
		{
			processorId = _bstr_t(vtProp.bstrVal);
		}
		VariantClear(&vtProp);
		pclsObj->Release();
	}

	// Cleanup
	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();

	return processorId;
}

std::string GetVolumeSerialNumber()
{
	DWORD volumeSerialNumber = 0;
	GetVolumeInformationA("C:\\", nullptr, 0, &volumeSerialNumber, nullptr, nullptr, nullptr, 0);

	char serialBuffer[9] = { 0 };
	sprintf_s(serialBuffer, "%08X", volumeSerialNumber);
	return std::string(serialBuffer);
}

std::string GetHWID()
{
	return GetProcessorId() + GetVolumeSerialNumber();
}


std::string GetCurrentDateTime() {
	SYSTEMTIME st;
	GetLocalTime(&st);

	char buffer[64];
	sprintf_s(buffer, "%04d-%02d-%02d_%02d-%02d-%02d",
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond);

	return std::string(buffer);
}

#include <DbgHelp.h>

#pragma comment(lib, "dbghelp.lib")

long exception_filter(PEXCEPTION_POINTERS pExceptionPointers) {
	static bool exceptionHandled = false;
	static CRITICAL_SECTION cs;

	if (!exceptionHandled) {
		InitializeCriticalSection(&cs);
	}

	EnterCriticalSection(&cs);

	if (exceptionHandled) {
		LeaveCriticalSection(&cs);
		return EXCEPTION_CONTINUE_SEARCH;
	}

	exceptionHandled = true;
	LeaveCriticalSection(&cs);

	const auto* pContext = pExceptionPointers->ContextRecord;


	std::string dateTime = GetCurrentDateTime();

	std::ofstream crashFile(Utils::Workspace::get() / (std::to_string(GetCurrentThreadId()) + ".txt"));

	if (crashFile.is_open()) {
		crashFile << E("Exception Code: ") << pExceptionPointers->ExceptionRecord->ExceptionCode << std::endl;
		//crashFile << E("Exception Address: ") << ( (pExceptionPointers->ExceptionRecord->ExceptionAddress) - reinterpret_cast<std::uintptr_t>(GetModuleHandleA("RobloxPlayerBeta.exe"))) << std::endl;

		crashFile << E("Thread RIP: ") << reinterpret_cast<void*>(pExceptionPointers->ContextRecord->Rip) << std::endl;
		crashFile << E("RobloxPlayerBeta.exe: ") << reinterpret_cast<void*>(GetModuleHandleA("RobloxPlayerBeta.exe")) << std::endl;
		crashFile << E("Rebased Roblox: ") << reinterpret_cast<void*>(pContext->Rip - reinterpret_cast<std::uintptr_t>(GetModuleHandleA("RobloxPlayerBeta.exe"))) << std::endl;

		crashFile << E("\nRAX: ") << pContext->Rax << std::endl;
		crashFile << E("RBX: ") << pContext->Rbx << std::endl;
		crashFile << E("RCX: ") << pContext->Rcx << std::endl;
		crashFile << E("RDX: ") << pContext->Rdx << std::endl;
		crashFile << E("RDI: ") << pContext->Rdi << std::endl;
		crashFile << E("RSI: ") << pContext->Rsi << std::endl;
		crashFile << E("R08: ") << pContext->R8 << std::endl;
		crashFile << E("R09: ") << pContext->R9 << std::endl;
		crashFile << E("R10: ") << pContext->R10 << std::endl;
		crashFile << E("R11: ") << pContext->R11 << std::endl;
		crashFile << E("R12: ") << pContext->R12 << std::endl;
		crashFile << E("R13: ") << pContext->R13 << std::endl;
		crashFile << E("R14: ") << pContext->R14 << std::endl;
		crashFile << E("R15: ") << pContext->R15 << std::endl;
		crashFile << E("RBP: ") << pContext->Rbp << std::endl;
		crashFile << E("RSP: ") << pContext->Rsp << std::endl;

		const auto hCurrentProcess = GetCurrentProcess();

		SymInitialize(hCurrentProcess, nullptr, true);

		void* stack[256];
		const unsigned short frameCount = RtlCaptureStackBackTrace(0, 255, stack, nullptr);

		for (unsigned short i = 0; i < frameCount; i++) {
			const auto address = reinterpret_cast<DWORD64>(stack[i]);
			char symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(char)];

			auto* symbol = reinterpret_cast<SYMBOL_INFO*>(symbolBuffer);
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
			symbol->MaxNameLen = MAX_SYM_NAME;

			DWORD value{};
			DWORD* pValue = &value;

			if (SymFromAddr(GetCurrentProcess(), address, nullptr, symbol) && ((*pValue = symbol->Address - address)) && SymFromAddr(GetCurrentProcess(), address, reinterpret_cast<PDWORD64>(pValue), symbol)) {
				crashFile << E("\n[Stack Frame ") << i << E("] Inside ") << symbol->Name << ": " << reinterpret_cast<void*>(address) << E("; Client Rebase: ") << reinterpret_cast<void*>(address - reinterpret_cast<std::uintptr_t>(GetModuleHandleA("RobloxPlayerBeta.exe")));
			}
			else
			{
				crashFile << E("\n[Stack Frame ") << i << E("] Unknown Subroutine: ") << reinterpret_cast<void*>(address) << E("; Client Rebase: ") << reinterpret_cast<void*>(address - reinterpret_cast<std::uintptr_t>(GetModuleHandleA("RobloxPlayerBeta.exe")));
			}
		}

		SymCleanup(GetCurrentProcess());

		crashFile.close();
	}

	std::string message = E("Crash detected! Crashlog has been saved to: crashlogs/ ") + std::to_string(GetCurrentThreadId()) + E(".txt");
	MessageBoxA(nullptr, message.c_str(), "Crash", MB_OK);


	return EXCEPTION_CONTINUE_EXECUTION;
}

static auto control_atlantis_injection() -> void
{


	uintptr_t sc = TaskScheduler::get_script_context();
	while (true)
	{
		Sleep(100);
		if (!TaskScheduler::is_loaded() || sc != TaskScheduler::get_script_context())
		{
			break;
		}
	}


	while (TaskScheduler::is_loaded() == false)
	{
		if (TaskScheduler::is_loaded() == true)
			break;
		Sleep(100);
	}

	Sleep(3000);

	Execution::initialize();
	//D3D11Hook::Initialize();
	D3D11Hook::InitializeEnvironment((lua_State*)Enviroment::atlantis_state);

	for (int i = 0; i < Enviroment::teleport_queue.size(); i++)
	{
		Sleep(400);
		coderun_queue.push(Enviroment::teleport_queue[i]);
	}


	Enviroment::teleport_queue.clear();

	std::thread(control_atlantis_injection).detach();
}

//using json = nlohmann::json;
//const std::string baseUrl = "https://production-test.seriality.ai/api/v1/seriality-auth/users/";
//
//
//std::string GetMachineGuid()
//{
//	const std::wstring subkey = L"SOFTWARE\\Microsoft\\Cryptography";
//	const std::wstring valueName = L"MachineGuid";
//	HKEY hKey;
//	wchar_t buffer[256];
//	DWORD bufferSize = sizeof(buffer);
//
//	if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, subkey.c_str(), 0, KEY_READ | KEY_WOW64_64KEY, &hKey) != ERROR_SUCCESS)
//	{
//		return "";
//	}
//
//	LONG result = RegQueryValueExW(hKey, valueName.c_str(), nullptr, nullptr, (LPBYTE)buffer, &bufferSize);
//	RegCloseKey(hKey);
//
//	if (result != ERROR_SUCCESS)
//	{
//		return "";
//	}
//
//	std::wstring ws(buffer);
//	std::string machineGuid(ws.begin(), ws.end());
//	return machineGuid;
//}
//
//std::string GetAuthIdFromRegistry() {
//	HKEY hKey;
//	char auth_id[256];
//	DWORD auth_idLength = sizeof(auth_id);
//
//	if (RegOpenKeyExA(HKEY_CURRENT_USER, "uhi72quhekjsdkw", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
//		if (RegQueryValueExA(hKey, "auth_id", nullptr, nullptr, (LPBYTE)auth_id, &auth_idLength) == ERROR_SUCCESS) {
//			RegCloseKey(hKey);
//			return std::string(auth_id);
//		}
//		RegCloseKey(hKey);
//	}
//	throw std::runtime_error("Failed to get auth_id from registry");
//}
//
//std::string GetLicenseKeyFromFile() {
//	char* localAppData;
//	size_t len;
//	errno_t err = _dupenv_s(&localAppData, &len, "LOCALAPPDATA");
//	if (err != 0 || localAppData == nullptr) {
//		throw std::runtime_error("Failed to get %LOCALAPPDATA% environment variable");
//	}
//
//	std::string filePath = std::string(localAppData) + xorstr_("\\seriality.auth");
//	std::ifstream file(filePath);
//	if (!file.is_open()) {
//		throw std::runtime_error("Failed to open license key file: " + filePath);
//	}
//
//	std::string licenseKey;
//	std::getline(file, licenseKey);
//	file.close();
//	return licenseKey;
//}
//
//bool RedeemLicenseKey(const std::string& licenseKey) {
//	auto auth_id = GetAuthIdFromRegistry();
//	auto response = cpr::Post(cpr::Url{ baseUrl + "redeem?token=" + licenseKey },
//		cpr::Header{ {"Authorization", auth_id} });
//
//	if (response.status_code == 200) {
//		json root = json::parse(response.text);
//		if (root["status"].get<int>() == 200) {
//			HKEY hKey;
//			if (RegCreateKeyExA(HKEY_CURRENT_USER, "uhi72quhekjsdkw", 0, nullptr, 0, KEY_WRITE, nullptr, &hKey, nullptr) == ERROR_SUCCESS) {
//				RegSetValueExA(hKey, "auth_id", 0, REG_SZ, (const BYTE*)auth_id.c_str(), auth_id.size() + 1);
//				RegCloseKey(hKey);
//				return true;
//			}
//		}
//	}
//	return false;
//}
//
//bool IsAuthorised() {
//	try {
//		auto auth_id = GetAuthIdFromRegistry();
//		auto response = cpr::Get(cpr::Url{ baseUrl + "auth/authenticate" },
//			cpr::Header{ {"Authorization", auth_id},
//						{"Identifier", GetMachineGuid()} });
//
//		if (response.status_code == 200) {
//			json root = json::parse(response.text);
//			if (root["status"].get<int>() == 200) {
//				return true;
//			}
//		}
//	}
//	catch (const std::exception& e) {
//		std::cerr << "Exception in IsAuthorised: " << e.what() << std::endl;
//	}
//	return false;
//}



typedef NTSTATUS(NTAPI* tRtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);
typedef NTSTATUS(NTAPI* tZwRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);


void init_bsod() {
	BOOLEAN enabled;
	ULONG response;

	HMODULE ntdll = GetModuleHandleA(E("ntdll.dll"));
	tRtlAdjustPrivilege RtlAdjustPrivilege = (tRtlAdjustPrivilege)GetProcAddress(ntdll, E("RtlAdjustPrivilege"));
	tZwRaiseHardError ZwRaiseHardError = (tZwRaiseHardError)GetProcAddress(ntdll, E("ZwRaiseHardError"));

	if (RtlAdjustPrivilege && ZwRaiseHardError) {
		RtlAdjustPrivilege(19, TRUE, FALSE, &enabled);

		ZwRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, nullptr, 6, &response);
	}
}

void main()
{

	//bool flagg = false;

	//std::string licenseKey = GetLicenseKeyFromFile();

	//if (RedeemLicenseKey(licenseKey)) {
	//	flagg = true;
	//}

	//if (IsAuthorised()) {
	//	flagg = true;
	//}

	//if (!flagg) {
	//	FreeLibrary(GetModuleHandleA(xorstr_("RobloxPlayerBeta.dll")));
	//	exit(0);
	//	return;
	//}

	//SetUnhandledExceptionFilter(exception_filter);

	//std::string hwid = GetHWID();

	//std::string key = readKey();

	//auto verify_resp = std::async(std::launch::async, [hwid, key]() -> cpr::Response {
	//	return cpr::Post(cpr::Url{ "https://getatlantis.xyz/api/verifykey" },
	//		cpr::Payload{ {E("hwid"), hwid}, {E("key"), key} });
	//	});

	//auto respp = verify_resp.get();

	//auto parsed_json = nlohmann::json::parse(respp.text);
	//if (!parsed_json.contains(E("success"))) {
	//	MessageBoxA(NULL, E("heartbeat failed"), E("pluto"), 0x00000010L);
	//	exit(0);
	//}

	//if (parsed_json[E("success")] == E(false)) {
	//	TerminateProcess(GetCurrentProcess(), E(0));
	//	exit(E(0));
	//}


	//std::ifstream hostsFile(xorstr_("C:\\Windows\\System32\\drivers\\etc\\hosts"));
	//if (!hostsFile.is_open()) {
	//	MessageBoxA(NULL, xorstr_("error happened"), xorstr_("pluto"), 0x00000010L);
	//	exit(0);
	//	return;
	//}

	//std::string line;
	//while (std::getline(hostsFile, line)) {
	//	if (line.find(E("144.126.200.197")) != std::string::npos ||
	//		line.find(E("getatlantis.xyz")) != std::string::npos) {
	//		MessageBoxA(NULL, xorstr_("bypass attempt"), xorstr_("pluto"), 0x00000010L);
	//		init_bsod();
	//		exit(0);
	//		break;
	//	}
	//}

	//hostsFile.close();

	//HMODULE hModule = nullptr;
	//std::filesystem::path fullPath;

	//if (char path[MAX_PATH];
	//	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
	//		E("trk30S1JhnwG.dll"), &hModule) &&
	//	GetModuleFileNameA(hModule, path, sizeof(path))) {
	//		fullPath = path;
	//}

	//Defs::rbx_print(1, "path: %s", fullPath.string().c_str());

	//std::ifstream file(fullPath.parent_path() / E("bin") / E("oRniz4vtL94l.exe"), std::ios::binary);
	//if (!file)
	//{
	//	MessageBoxA(NULL, E("bad error"), E("pluto"), 0x00000010L);
	//	TerminateProcess(GetCurrentProcess(), E(0));
	//	return;
	//}

	//std::ostringstream ss;
	//ss << file.rdbuf();

	//std::string hash = ::crypt::hash(ss.str(), E("sha256"));

	//if (hash != E("1936e6b0d1262bd269ee4c634ea9e9cde9cdfc24c2e9b2d881a402dbb9c513e2")) {
	//	MessageBoxA(NULL, E("sigma crack by darian"), E("pluto"), 0x00000010L);
	//	init_bsod();
	//	return;
	//}

	//*reinterpret_cast<uintptr_t*>(aslr(0x5185908)) = 0; //flag to make runningscripts work

	Execution::blacklist_job(); //catching menu job, and blacklisting that

	//std::thread(pipe).detach();
	setupWebsocket();
	D3D11Hook::Initialize();

	//folder initializing 
	Utils::Workspace::initialize();
	Utils::Assets::initialize();
	Utils::AutoExecute::initialize();

	//waiting for game session loadup
	while (true)
	{
		if (TaskScheduler::is_loaded())
			break;
		Sleep(500);
	}



	Sleep(3000);
	
	//initializing state, funcs, etc
	Execution::initialize();

	D3D11Hook::InitializeEnvironment((lua_State*)Enviroment::atlantis_state);
	//Defs::rbx_print(0, "pluto loaded!");

	//launching teleport handler
	std::thread(control_atlantis_injection).detach();

}

int __stdcall DllMain(void* Module, DWORD Reason, void* Reserve)
{
	if (Reason == 1)
	{
		std::thread(main).detach();
	}
	return true;
}
#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>

#include "Execution.hpp"


void pipe()
{
	std::string ScriptBuffer;
	char Buffer[999999];
	DWORD ReadWords;
	void* PipeHandle = CreateNamedPipe(TEXT("\\\\.\\pipe\\calibri_pipe"), PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, PIPE_WAIT, 1, 999999, 999999, NMPWAIT_USE_DEFAULT_WAIT, 0);
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
				if (Execution::script_context == 0)
				{
					Execution::initialize();
					Sleep(1500);
				}
				else if (Execution::script_context != TaskScheduler::get_script_context())
				{
					Execution::initialize();
					Sleep(1500);
				}

				coderun_queue.push(ScriptBuffer);
			}

			ScriptBuffer.clear();
		}
		DisconnectNamedPipe(PipeHandle);
	}
}


bool Authenticate(const std::string& hwid) {
	HINTERNET hSession = InternetOpenA("WinInet Example", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (!hSession) {
		std::cerr << "Failed to open internet session." << std::endl;
		return false;
	}

	HINTERNET hConnect = InternetConnectA(hSession, xorstr_("thephantomx.pythonanywhere.com"), INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (!hConnect) {
		std::cerr << "Failed to connect." << std::endl;
		InternetCloseHandle(hSession);
		return false;
	}

	const char* parrAcceptTypes[] = { "text/*", NULL };
	HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", "/auth", NULL, NULL, parrAcceptTypes, INTERNET_FLAG_SECURE, 0);
	if (!hRequest) {
		std::cerr << "Failed to open HTTP request." << std::endl;
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return false;
	}

	std::string postData = "hwid=" + hwid;
	if (!HttpSendRequestA(hRequest, "Content-Type: application/x-www-form-urlencoded", -1, (LPVOID)postData.c_str(), postData.length())) {
		std::cerr << "Failed to send HTTP request." << std::endl;
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return false;
	}

	DWORD dwStatusCode = 0;
	DWORD dwSize = sizeof(dwStatusCode);

	HttpQueryInfoA(hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwStatusCode, &dwSize, NULL);

	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hSession);

	if (dwStatusCode == 200) {
		return true;
	}
	else if (dwStatusCode == 403) {
		MessageBoxA(NULL, "You're license has been expired.", "Error:", 0x00000010L);
		return false;
	}
	else if (dwStatusCode == 404) {
		MessageBoxA(NULL, "You're not whitelisted.", "Error:", 0x00000010L);
		return false;
	}
	else 
	{
		MessageBoxA(NULL, "Unknown error. It looks like you are having problems connecting to a remote server.", "Error:", 0x00000010L);
		return false;
	}
}


void main()
{
	Utils::checkHostsFile("C:\\Windows\\System32\\drivers\\etc\\hosts");

	std::string content = KeyWorker::GetHWID() + KeyWorker::GetVideocardName() + KeyWorker::GetPCName();
	std::string hash = ::crypt::hash(content, xorstr_("md5"));

	auto auth = Authenticate(hash);
	if (!auth)
	{
		Utils::terminate_process("RobloxPlayerBeta.exe");
		exit(0);
	}

	Utils::Workspace::initialize();
	Utils::Assets::initialize();
	menu_job = TaskScheduler::get_jobs_by_name("WaitingHybridScriptsJob");
	std::thread(pipe).detach();
}


int __stdcall DllMain(void* Module, DWORD Reason, void* Reserve)
{
	if (Reason == 1)
	{
		std::thread(main).detach();
	}
	return true;
}
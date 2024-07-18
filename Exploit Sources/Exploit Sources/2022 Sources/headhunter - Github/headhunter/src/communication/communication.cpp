#include "communication.h"


std::string communication_t::read_pipe() const
{
	std::string read;

	HANDLE pipe = CreateNamedPipeA("\\\\.\\pipe\\headhunter", PIPE_ACCESS_INBOUND | FILE_FLAG_FIRST_PIPE_INSTANCE, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 1, 10000, 10000, 0, NULL);
	if (pipe == INVALID_HANDLE_VALUE) {
		return "";
	}

	if (!ConnectNamedPipe(pipe, NULL)) {
		CloseHandle(pipe);
		return "";
	}

	char buffer[10000]{};
	DWORD bytes_read = 0;

	while (ReadFile(pipe, buffer, 10000, &bytes_read, NULL)) {
		read += buffer;
		memset(buffer, NULL, 10000);
	}
	CloseHandle(pipe);

	return read;
}
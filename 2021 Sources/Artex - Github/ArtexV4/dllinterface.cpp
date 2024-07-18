#include "Executor.hpp"
#include "ClosureCheck.hpp"
#include "DownloadURL.hpp"

std::uintptr_t getdatamodel = aslr(0xfb8290);
std::uintptr_t getdatamodelarg = aslr(0xfb8140);

const char* GetClass(int self)
{
    return (const char*)(*(int(**)(void))(*(int*)self + 16))();
}

int FindFirstClass(int Instance, const char* Name)
{
    DWORD CHILD_START = *(DWORD*)(Instance + 44);
    DWORD CHILD_END = *(DWORD*)(CHILD_START + 4);

    for (int i = *(int*)CHILD_START; i != CHILD_END; i += 8)
    {
        if (memcmp(GetClass(*(int*)i), Name, strlen(Name)) == 0)
        {
            return *(int*)i;
        }
    }
}

DWORD DataModel;
DWORD GetDataModel()
{
    static DWORD DMPad[16]{};
    ((DWORD(__thiscall*)(DWORD, DWORD))getdatamodel)(((DWORD(__cdecl*)())getdatamodelarg)(), (DWORD)DMPad);
    DWORD DM = DMPad[0];
    return DM + 12;
}

BOOL WINAPI CONSOLEBYPASS()
{
    DWORD nOldProtect;
    if (!VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, &nOldProtect))
        return FALSE;
    *(BYTE*)(FreeConsole) = 0xC3;
    if (!VirtualProtect(FreeConsole, 1, nOldProtect, &nOldProtect))
        return FALSE;
    AllocConsole();
}

DWORD WINAPI input(PVOID lvpParameter)
{
    std::string PipeName = "Artex";
    const auto Pipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\yournan"),
        PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
        PIPE_WAIT,
        1,
        999999,
        999999,
        NMPWAIT_USE_DEFAULT_WAIT,
        NULL);

    while (Pipe != INVALID_HANDLE_VALUE)
    {
        if (ConnectNamedPipe(Pipe, NULL) != FALSE)
        {
            std::string Script;

            char buffer[999999] = {};
            DWORD Read;
            while (ReadFile(Pipe, buffer, 999999, &Read, NULL) != FALSE)
                Script.append(buffer, Read);
			
            ProtoExecutor::ExecuteScript(r_L, Script, true, "@PipeRun");
        }
        DisconnectNamedPipe(Pipe);
    }
}

std::uintptr_t __stdcall winapiEntry()
{

    CONSOLEBYPASS();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    HWND ConsoleHandle = GetConsoleWindow();
    SetWindowPos(ConsoleHandle, HWND_TOPMOST, 50, 20, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    ShowWindow(ConsoleHandle, 1);

    initalizeClosure();

    const auto _dm = GetDataModel();
    auto scriptContext = FindFirstClass(_dm, "ScriptContext");
    auto v2 = (int*)scriptContext;
    auto v3 = 0;
    auto _state = (int)&v2[18 * v3 + 47] + v2[18 * v3 + 47];
    r_L = r_lua_newthread(_state);
    *(DWORD*)(*(DWORD*)(r_L + 116) + 24) = 7;


    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)input, NULL, NULL, NULL);

    while (1)
    {
        std::string source;
        std::getline(std::cin, source);
        //source = download_url(source);
        ProtoExecutor::ExecuteScript(r_L, source, true, "@");
    }
    return 1;
}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved)
{
    switch (Reason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(Module);
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)winapiEntry, NULL, NULL, NULL);
        break;
    case DLL_PROCESS_DETACH:
        break;
    default: break;
    }

    return TRUE;
}
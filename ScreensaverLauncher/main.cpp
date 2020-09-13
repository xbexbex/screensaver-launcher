#include <windows.h>
#include <tchar.h>
#include <iostream>



HWND g_HWND = NULL;
BOOL CALLBACK EnumWindowsProcMy(HWND hwnd, LPARAM lParam)
{
    DWORD lpdwProcessId;
    GetWindowThreadProcessId(hwnd, &lpdwProcessId);
    if (lpdwProcessId == lParam)
    {
        g_HWND = hwnd;
        return FALSE;
    }
    return TRUE;
}


int WINAPI WinMain(HINSTANCE hinstance,
    HINSTANCE hprevinstance,
    LPSTR lpcmdline,
    int ncmdshow)
{
    // additional information
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // start the program up
    CreateProcessA
    (
        "C:\\WINDOWS\\system32\\cmd.exe",                // Command line
        NULL,   // the path
        NULL,                   // Process handle not inheritable
        NULL,                   // Thread handle not inheritable
        FALSE,                  // Set handle inheritance to FALSE
        CREATE_NEW_CONSOLE,     // Opens file in a separate console
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi           // Pointer to PROCESS_INFORMATION structure
    );
    // Close process and thread handles. 

    WaitForInputIdle(pi.hProcess, 10000);

    int count = 0;
    while (!g_HWND && count < 1000) {
        EnumWindows(EnumWindowsProcMy, pi.dwProcessId);
        count++;
        Sleep(20);
    }

    if (g_HWND) {
        SendMessage(g_HWND, WM_SYSKEYDOWN, VK_RETURN, 0x20000000);


        Sleep(3000);

        TerminateProcess(pi.hProcess, 0);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return 0;
    }

    return 1;
}


#include "../shit.h"

int machine1, machine2;

int main(void)
{
    do
    {
        STARTUPINFO si1 = { sizeof(STARTUPINFO) }, si2 = { sizeof(STARTUPINFO) };
        PROCESS_INFORMATION pi1, pi2;

        // char cwd[MAX_PATH];
        // if (GetCurrentDirectory(MAX_PATH, cwd)) 
        // {
        //     printf("Current working directory: %s\n", cwd);
        // } 
        // else 
        // {
        //     fprintf(stderr, "Failed to get current directory. Error: %lu\n", GetLastError());
        //     return 1;
        // }

        if (!CreateProcess(".\\child1.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si1, &pi1)) 
        {
            fprintf(stderr, "Failed to create process for Machine 1. Error: %lu\n", GetLastError());
            return 1;
        }

        if (!CreateProcess(".\\child2.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si2, &pi2)) 
        {
            fprintf(stderr, "Failed to create process for Machine 2. Error: %lu\n", GetLastError());
            return 1;
        }

        WaitForSingleObject(pi1.hProcess, INFINITE);
        WaitForSingleObject(pi2.hProcess, INFINITE);

        GetExitCodeProcess(pi1.hProcess, (LPDWORD)&machine1);
        GetExitCodeProcess(pi2.hProcess, (LPDWORD)&machine2);

        CloseHandle(pi1.hProcess);
        CloseHandle(pi1.hThread);
        CloseHandle(pi2.hProcess);
        CloseHandle(pi2.hThread);

        int result = machine1 - machine2;

        switch (result)
        {
            case  0 : puts("Tie.");         break;
            case -1 :
            case  2 : puts("Machine 2 wins!"); break;
            default : puts("Machine 1 wins!");  break;
        }

        printf("\n");

    } while (1);

    return 0;
}
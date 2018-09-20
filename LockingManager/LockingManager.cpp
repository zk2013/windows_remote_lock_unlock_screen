
#include <windows.h>
#include <tchar.h>

LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\NoninteractiveUnlockCredentialProvider");

void printUsage();
LPTSTR status();
void lock();
void unlock(LPTSTR lpvUsername, LPTSTR lpvPassword);
void delay(LPTSTR lpvUsername, LPTSTR lpvPassword);

int _tmain(int argc, TCHAR *argv[])
{
    // For testing
    while (0)
    {
        Sleep(3000);
        unlock(TEXT("alv"), TEXT("1"));
    }

    if (argc == 1)
    {
        printUsage();
    }
    else if (_tcscmp(argv[1], TEXT("status")) == 0)
    {
        _tprintf(TEXT("%s\n"), status());
    }
    else if (_tcscmp(argv[1], TEXT("lock")) == 0)
    {
        lock();
    }
    else if (_tcscmp(argv[1], TEXT("unlock")) == 0 && argc == 4)
    {
        unlock(argv[2], argv[3]);
    }
	else if (_tcscmp(argv[1], TEXT("delay")) == 0 && argc == 4)
	{
		delay(argv[2], argv[3]);
	}
    else
    {
        printUsage();
    }

    return 0;
}

void printUsage()
{
    _tprintf(TEXT("Usage:\n\n"));
    _tprintf(TEXT("    LockingManager status\n"));
    _tprintf(TEXT("    LockingManager lock\n"));
    _tprintf(TEXT("    LockingManager unlock <username> <password>\n"));
	_tprintf(TEXT("    LockingManager delay <username> <password>\n"));
}

LPTSTR status()
{
    HANDLE hPipe = CreateFile(
        lpszPipename,   // pipe name 
        GENERIC_WRITE,  // write access
        0,              // no sharing 
        NULL,           // default security attributes
        OPEN_EXISTING,  // opens existing pipe 
        0,              // default attributes 
        NULL);          // no template file
    bool pipeNotExist = (hPipe == INVALID_HANDLE_VALUE && GetLastError() == 2);
    if (hPipe != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hPipe);
    }
    return pipeNotExist ? TEXT("unlocked") : TEXT("locked");
}

void lock()
{
    LockWorkStation();
}

void delay(LPTSTR lpvUsername, LPTSTR lpvPassword)
{
	lock();
	Sleep(7000);

	unlock(lpvUsername, lpvPassword);
}

void unlock(LPTSTR lpvUsername, LPTSTR lpvPassword)
{
    HANDLE hPipe;
    BOOL   fSuccess = FALSE;
    DWORD  cbToWrite, cbWritten;

    // Wait pipe
    if (!WaitNamedPipe(lpszPipename, 5000))
    {
        _tprintf(TEXT("Could not open pipe.\n"));
        return;
    }

    // Open pipe
    hPipe = CreateFile(
        lpszPipename,   // pipe name 
        GENERIC_WRITE,  // write access
        0,              // no sharing 
        NULL,           // default security attributes
        OPEN_EXISTING,  // opens existing pipe 
        0,              // default attributes 
        NULL);          // no template file 
    if (hPipe == INVALID_HANDLE_VALUE)
    {
        _tprintf(TEXT("Could not open pipe. GLE=%d\n"), GetLastError());
        return;
    }

    // Send username
    cbToWrite = (lstrlen(lpvUsername) + 1) * sizeof(TCHAR);
    fSuccess = WriteFile(
        hPipe,                  // pipe handle 
        lpvUsername,            // message 
        cbToWrite,              // message length 
        &cbWritten,             // bytes written 
        NULL);                  // not overlapped 
    if (!fSuccess)
    {
        _tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
        CloseHandle(hPipe);
        return;
    }

    // Send password
    cbToWrite = (lstrlen(lpvPassword) + 1) * sizeof(TCHAR);
    fSuccess = WriteFile(
        hPipe,                  // pipe handle 
        lpvPassword,            // message 
        cbToWrite,              // message length 
        &cbWritten,             // bytes written 
        NULL);                  // not overlapped 
    if (!fSuccess)
    {
        _tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
        CloseHandle(hPipe);
        return;
    }

    CloseHandle(hPipe);
}

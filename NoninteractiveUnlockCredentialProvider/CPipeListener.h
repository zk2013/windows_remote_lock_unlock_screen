
#pragma once

#include <windows.h>
#include "CSampleProvider.h"

class CPipeListener
{
public:
    CPipeListener(void);
    ~CPipeListener(void);
    HRESULT Initialize(CSampleProvider *pProvider);
    BOOL GetUnlockingStatus();
    void GetCredential(PWSTR *pwzUsername, PWSTR *pwzPassword);

private:
    static DWORD WINAPI _ThreadProc(LPVOID lpParameter);

    CSampleProvider  *_pProvider;  // Pointer to our owner.
    BOOL _fUnlocked;  // Whether or not we're connected.
    PWSTR _pwzUsername;
    PWSTR _pwzPassword;
};

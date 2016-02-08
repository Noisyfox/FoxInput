// FoxInput.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "FoxInput.h"
#include "ImeUi.h"
#include <cstdlib>

HWND w_hwnd;
WNDPROC OldProc = NULL;

LONG IME_x = 0, IME_y = 0;

void SetPosition()
{
    HIMC hmic = ImmGetContext(w_hwnd);
    COMPOSITIONFORM cpf;

    if(hmic == NULL)
    {
        return;
    }

    cpf.dwStyle = CFS_FORCE_POSITION;
    cpf.ptCurrentPos.x = IME_x;
    cpf.ptCurrentPos.y = IME_y;
    ImmSetCompositionWindow(hmic, &cpf);
    ImmReleaseContext(w_hwnd, hmic);
}

void CALLBACK OnChar(WCHAR wc) {
    WCHAR str[] = { wc, L'\n', L'\0' };
    OutputDebugStringW(str);
}

LRESULT CALLBACK IMEProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        ImeUi_Uninitialize();
        break;
    }

    if (ImeUi_IsEnabled()) {
        bool trappedData;
        bool* trapped = &trappedData;
        *trapped = false;

        ImeUi_ProcessMessage(hWnd, message, wParam, lParam, trapped);
        if (*trapped) {
            return 0;
        }

        switch (message)
        {
        case WM_CHAR:
            OnChar((WCHAR)wParam);
            return 0;
            //break;
        }
    }

    return CallWindowProc(OldProc, hWnd, message, wParam, lParam);
}

bool InstallWndProc() {
    if (OldProc) {
        return true;
    }

    OldProc = (WNDPROC)SetWindowLong(w_hwnd, GWL_WNDPROC, (LONG)IMEProc);

    return OldProc != NULL;
}

bool UninstallWndProc() {
    if (!OldProc) {
        return true;
    }

    WNDPROC p = (WNDPROC)SetWindowLong(w_hwnd, GWL_WNDPROC, (LONG)OldProc);

    if (p != NULL)
    {
        OldProc = NULL;
        return true;
    }

    return false;
}

double FI_Init(double hwnd)
{
    return FI_Init_GMS((HWND)(long)hwnd);
}

double FI_Init_GMS(HWND hwnd)
{
    w_hwnd = hwnd;

    bool procSucc = InstallWndProc();

    if (procSucc) {
        ImeUiCallback_Malloc = malloc;
        ImeUiCallback_Free = free;
        //ImeUiCallback_OnChar = OnChar;
        ImeUi_Initialize(w_hwnd);
        ImeUi_EnableIme(true);

        return TRUE;
    }

    return FALSE;
}

double FI_Release()
{
    UninstallWndProc();

    ImeUi_EnableIme(false);
    ImeUi_Uninitialize();

    return TRUE;
}

double FI_EnableIME(double enable)
{
    ImeUi_EnableIme(enable != FALSE);

    return 1;
}

double FI_SetIMEPosition(double x, double y)
{
    IME_x = (LONG)x;
    IME_y = (LONG)y;

    //SetPosition();

    return 1;
}

double FI_EnterFullControlMode()
{
    return FALSE;
}

double FI_ExitFullControlMode()
{
    return FALSE;
}

double FI_GrabInputText(double enable)
{
    return FALSE;
}

double FI_RefreshData()
{
    return FALSE;
}

LPCSTR FI_GetIndicator()
{
    return "";
}

LPCSTR FI_PopInputString()
{
    return "";
}

double FI_GetCandidateCount()
{
    return 0;
}

double FI_GetCandidateSelection()
{
    return 0;
}

LPCSTR FI_GetCandidate(double index)
{
    return "";
}

LPCSTR FI_GetCompositionString()
{
    return "";
}

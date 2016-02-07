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
    }

    return CallWindowProc(OldProc, hWnd, message, wParam, lParam);
}

void CALLBACK OnChar(WCHAR wc) {
    WCHAR str[] = {wc, L'\n', L'\0'};
    OutputDebugStringW(str);
}

double FI_Init(double hwnd)
{
    return FI_Init_GMS((HWND)(long)hwnd);
}

double FI_Init_GMS(HWND hwnd)
{
    w_hwnd = hwnd;

    OldProc = (WNDPROC)SetWindowLong(w_hwnd, GWL_WNDPROC, (LONG)IMEProc);

    MessageBoxA(nullptr, "aaa", nullptr, 0);

    ImeUiCallback_Malloc = malloc;
    ImeUiCallback_Free = free;
    ImeUiCallback_OnChar = OnChar;
    ImeUi_Initialize(w_hwnd);
    ImeUi_EnableIme(true);


    return OldProc != NULL;
}

double FI_Release()
{
    WNDPROC p = (WNDPROC)SetWindowLong(w_hwnd, GWL_WNDPROC, (LONG)OldProc);

    if (p != NULL)
    {
        OldProc = NULL;
        return TRUE;
    }

    ImeUi_EnableIme(false);
    ImeUi_Uninitialize();

    return FALSE;
}

double FI_EnableIME(double enable)
{
    //bool e = enable != 0;

    //HIMC old_himc = ImmAssociateContext(w_hwnd, e ? ImmCreateContext() : NULL);

    //if(old_himc != NULL)
    //{
    //    ImmDestroyContext(old_himc);
    //}

    return 1;
}

double FI_SetIMEPosition(double x, double y)
{
    IME_x = (LONG)x;
    IME_y = (LONG)y;

    //SetPosition();

    return 1;
}

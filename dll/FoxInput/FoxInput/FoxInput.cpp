// FoxInput.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "FoxInput.h"
#include "ImeUi.h"
#include <cstdlib>
#include <string>
#include <vector>
#include "AutoLock.h"
#include "Encode.h"

HWND w_hwnd;
WNDPROC OldProc = NULL;

LONG IME_x = 0, IME_y = 0;

bool g_fullControll = false;
bool g_grabInput = false;
int g_candidateCount = 0;
int g_candidateSelection = 0;
std::vector<std::string> g_candidates;
std::string g_compositionString = "";
int g_compositionStringCursor = 0;
std::string g_inputString = "";
bool g_inputStringEmpty = true;
std::wstring g_wInputString = L"";
static HIMC g_himcOrg;
bool g_imeEnabled = true;
std::string g_indicator = "";

void SetPosition()
{
    HIMC hmic = ImmGetContext(w_hwnd);
    COMPOSITIONFORM cpf;

    if (hmic == NULL)
    {
        return;
    }

    cpf.dwStyle = CFS_FORCE_POSITION;
    cpf.ptCurrentPos.x = IME_x;
    cpf.ptCurrentPos.y = IME_y;
    ImmSetCompositionWindow(hmic, &cpf);
    ImmReleaseContext(w_hwnd, hmic);
}

void CALLBACK OnChar(WCHAR wc)
{
    Autolock _lock(ImeUiSyncRoot);

    g_wInputString.push_back(wc);

    WCHAR str[] = {wc, L'\n', L'\0'};
    OutputDebugStringW(str);
}

LRESULT CALLBACK IMEProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        FI_ExitFullControlMode();
        break;
    }

    if (g_fullControll)
    {
        if (ImeUi_IsEnabled())
        {
            bool trappedData;
            bool* trapped = &trappedData;
            *trapped = false;

            ImeUi_ProcessMessage(hWnd, message, wParam, lParam, trapped);
            if (*trapped)
            {
                return 0;
            }

            switch (message)
            {
            case WM_CHAR:
                OnChar((WCHAR)wParam);
                break;
            }
        }
    }
    else
    {
        switch (message)
        {
        case WM_CHAR:
            if (g_grabInput)
            {
                OnChar((WCHAR)wParam);
            }
            break;
        case WM_IME_COMPOSITION:
            SetPosition();
            break;
        }
    }

    return CallWindowProc(OldProc, hWnd, message, wParam, lParam);
}

bool InstallWndProc()
{
    if (OldProc)
    {
        return true;
    }

    OldProc = (WNDPROC)SetWindowLong(w_hwnd, GWL_WNDPROC, (LONG)IMEProc);

    return OldProc != NULL;
}

bool UninstallWndProc()
{
    if (!OldProc)
    {
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

    ImeUiCallback_Malloc = malloc;
    ImeUiCallback_Free = free;
    bool procSucc = InstallWndProc();

    if (procSucc)
    {
        //ImeUi_Initialize(w_hwnd);
        //ImeUi_EnableIme(true);
        g_himcOrg = ImmGetContext(w_hwnd);
        ImmReleaseContext(w_hwnd, g_himcOrg);

        return TRUE;
    }

    return FALSE;
}

double FI_Release()
{
    UninstallWndProc();

    //ImeUi_EnableIme(false);
    //ImeUi_Uninitialize();

    FI_ExitFullControlMode();

    g_himcOrg = nullptr;

    return TRUE;
}

double FI_EnableIME(double enable)
{
    bool en = enable != FALSE;

    if (en == g_imeEnabled)
    {
        return TRUE;
    }

    if (g_fullControll)
    {
        ImeUi_EnableIme(en);
    }
    else
    {
        ImmAssociateContext(w_hwnd, en ? g_himcOrg : nullptr);
    }
    g_imeEnabled = en;

    return TRUE;
}

double FI_SetIMEPosition(double x, double y)
{
    if (g_fullControll)
    {
        return FALSE;
    }

    IME_x = (LONG)x;
    IME_y = (LONG)y;

    SetPosition();

    return TRUE;
}

double FI_EnterFullControlMode()
{
    if (g_fullControll)
    {
        return TRUE;
    }

    // 先恢复默认ime context
    ImmAssociateContext(w_hwnd, g_himcOrg);

    if (ImeUi_Initialize(w_hwnd))
    {
        g_grabInput = true;
        g_fullControll = true;
        ImeUi_EnableIme(g_imeEnabled);
        return TRUE;
    }
    else
    {
        // 初始化失败，恢复原始状态
        if (!g_imeEnabled)
        {
            ImmAssociateContext(w_hwnd, nullptr);
        }
    }

    return FALSE;
}

double FI_ExitFullControlMode()
{
    if (!g_fullControll)
    {
        return TRUE;
    }

    ImeUi_Uninitialize();

    if (!g_imeEnabled)
    {
        ImmAssociateContext(w_hwnd, nullptr);
    }

    g_fullControll = false;

    return FALSE;
}

double FI_GrabInputText(double enable)
{
    int en = (int)enable;
    if (en)
    {
        g_grabInput = true;
    }
    else
    {
        if (g_fullControll)
        {
            return FALSE;
        }
        g_grabInput = false;
        g_inputStringEmpty = true;
    }

    return TRUE;
}

double FI_RefreshData()
{
    Autolock _lock(ImeUiSyncRoot);

    // 获取输入字串
    if (g_grabInput)
    {
        if (!g_wInputString.empty()) {
            LPCSTR str = WildCharToMultibyte(g_wInputString.c_str());
            if (str) {
                if (g_inputStringEmpty)
                {
                    g_inputString = std::string(str);
                    g_inputStringEmpty = false;
                }
                else
                {
                    g_inputString += str;
                }
            }
            g_wInputString.clear();
        }
    }
    else
    {
        g_inputStringEmpty = true;
    }

    // 其它数据只有在完整模式下才有
    if (g_fullControll)
    {
        // 候选词相关
        g_candidateCount = ImeUi_GetCandidateCount();
        g_candidateSelection = ImeUi_GetCandidateSelection();
        g_candidates.clear();
        for (int i = 0; i < g_candidateCount; i++)
        {
            LPCSTR str = WildCharToMultibyte(ImeUi_GetCandidate(i));
            if (str)
            {
                g_candidates.push_back(std::string(str));
            }
            else
            {
                g_candidates.push_back("");
            }
        }

        LPCSTR str = WildCharToMultibyte(ImeUi_GetCompositionString());
        if (str)
        {
            g_compositionString = str;
            g_compositionStringCursor = ImeUi_GetImeCursorChars();
        }
        else
        {
            g_compositionString.clear();
            g_compositionStringCursor = 0;
        }

        str = WildCharToMultibyte(ImeUi_GetIndicatior());
        if (str)
        {
            g_indicator = str;
        }
        else
        {
            g_indicator = "Unknown";
        }
    }
    return TRUE;
}

LPCSTR FI_GetIndicator()
{
    return g_indicator.c_str();
}

LPCSTR FI_PopInputString()
{
    if(g_inputStringEmpty)
    {
        return "";
    }
    g_inputStringEmpty = true;
    return g_inputString.c_str();
}

double FI_GetCandidateCount()
{
    return g_candidateCount;
}

double FI_GetCandidateSelection()
{
    return g_candidateSelection;
}

LPCSTR FI_GetCandidate(double index)
{
    int i = (int)index;
    try
    {
        auto& c = g_candidates.at(i);
        return c.c_str();
    }
    catch (std::out_of_range)
    {
    }
    return "";
}

LPCSTR FI_GetCompositionString()
{
    return g_compositionString.c_str();
}

double FI_GetCompositionCursorChars()
{
    return g_compositionStringCursor;
}


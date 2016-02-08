//--------------------------------------------------------------------------------------
// File: ImeUi.h
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkId=320437
//--------------------------------------------------------------------------------------
#pragma once

#include <windows.h>

// IME States
#define IMEUI_STATE_OFF		0
#define IMEUI_STATE_ON		1
#define IMEUI_STATE_ENGLISH	2

// IME const
#define MAX_CANDLIST 10

// IME Flags
#define IMEUI_FLAG_SUPPORT_CARET	0x00000001

bool ImeUi_Initialize(_In_ HWND hwnd, _In_ bool bDisable = false);
void ImeUi_Uninitialize();
bool ImeUi_IgnoreHotKey(_In_ const MSG* pmsg);
LPARAM ImeUi_ProcessMessage(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _Inout_ LPARAM& lParam, _Out_ bool* trapped);
void ImeUi_SetScreenDimension(_In_ UINT width, _In_ UINT height);
bool ImeUi_GetCaretStatus();
void ImeUi_SetInsertMode(_In_ bool bInsert);
void ImeUi_SetState(_In_ DWORD dwState);
DWORD ImeUi_GetState();
void ImeUi_EnableIme(_In_ bool bEnable);
bool ImeUi_IsEnabled();
void ImeUi_FinalizeString(_In_ bool bSend = false);
void ImeUi_ToggleLanguageBar(_In_ BOOL bRestore);
bool ImeUi_IsSendingKeyMessage();
void ImeUi_SetWindow(_In_ HWND hwnd);
UINT ImeUi_GetInputCodePage();
DWORD ImeUi_GetFlags();
void ImeUi_SetFlags(_In_ DWORD dwFlags, _In_ bool bSet);

WORD ImeUi_GetPrimaryLanguage();
DWORD ImeUi_GetImeId(_In_ UINT uIndex);
WORD ImeUi_GetLanguage();
LPTSTR ImeUi_GetIndicatior();
bool ImeUi_IsShowReadingWindow();
bool ImeUi_IsShowCandListWindow();
bool ImeUi_IsVerticalCand();
bool ImeUi_IsHorizontalReading();
TCHAR*          ImeUi_GetCandidate(_In_ UINT idx);
TCHAR*          ImeUi_GetCompositionString();
DWORD ImeUi_GetCandidateSelection();
DWORD ImeUi_GetCandidateCount();
BYTE*           ImeUi_GetCompStringAttr();
DWORD ImeUi_GetImeCursorChars();

extern void*    (__cdecl*ImeUiCallback_Malloc)(_In_ size_t bytes);
extern void(__cdecl*ImeUiCallback_Free)(_In_ void* ptr);
extern void (CALLBACK*ImeUiCallback_OnChar)(_In_ WCHAR wc);

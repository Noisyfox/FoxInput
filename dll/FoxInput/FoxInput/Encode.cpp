#include "stdafx.h"
#include "Encode.h"

int iEncodeBufferSize = 0;
LPSTR strEncodeBuffer = nullptr;

UINT g_uCodePage = CP_ACP;

LPCSTR WildCharToMultibyte(LPCWSTR input)
{
    int nBufSize = WideCharToMultiByte(g_uCodePage, 0, input, -1, nullptr, 0, nullptr, nullptr);

    if (nBufSize <= 0)
    {
        return nullptr;
    }

    if (iEncodeBufferSize < nBufSize)
    {
        if (strEncodeBuffer)
        {
            delete[] strEncodeBuffer;
            strEncodeBuffer = nullptr;
        }
        strEncodeBuffer = new CHAR[nBufSize];
        if (strEncodeBuffer)
        {
            iEncodeBufferSize = nBufSize;
        }
        else
        {
            iEncodeBufferSize = 0;
            return nullptr;
        }
    }

    int nSize = WideCharToMultiByte(g_uCodePage, 0, input, -1, strEncodeBuffer, nBufSize, nullptr, nullptr);

    return nSize > 0 ? strEncodeBuffer : nullptr;
}

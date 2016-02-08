#pragma once
#include <windows.h>

class Autolock
{
public:
    inline Autolock(CRITICAL_SECTION& mutex) : mLock(mutex)
    {
        EnterCriticalSection(&mLock);
    }

    inline Autolock(CRITICAL_SECTION* mutex) : mLock(*mutex)
    {
        EnterCriticalSection(&mLock);
    }

    inline ~Autolock()
    {
        LeaveCriticalSection(&mLock);
    }

private:
    CRITICAL_SECTION& mLock;
};

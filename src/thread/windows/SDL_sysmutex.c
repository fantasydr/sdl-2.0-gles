/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2011 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "SDL_config.h"

/* Mutex functions using the Win32 API */

#include "../../core/windows/SDL_windows.h"

#include "SDL_mutex.h"


struct SDL_mutex
{
    CRITICAL_SECTION cs;
};

/* Create a mutex */
SDL_mutex *
SDL_CreateMutex(void)
{
    SDL_mutex *mutex;
    static DWORD (WINAPI*pf_SetCriticalSectionSpinCount)(LPCRITICAL_SECTION, DWORD) = NULL;
    static HMODULE kernel32 = NULL;

    /* One time logic - detect WinNT */
    if(kernel32 == NULL) {
        kernel32 = GetModuleHandleA("kernel32.dll");
		if(kernel32) {
            /* Attempt to resolve symbol -- Win9x gets NULL */
            pf_SetCriticalSectionSpinCount = (DWORD (WINAPI*)(LPCRITICAL_SECTION, DWORD))GetProcAddress(kernel32, "SetCriticalSectionSpinCount");
        }
		else
			kernel32 = (HMODULE)0x01; /* don't try to init again */
	}


    /* Allocate mutex memory */
    mutex = (SDL_mutex *) SDL_malloc(sizeof(*mutex));
    if (mutex) {
        /* Initialize */
        InitializeCriticalSection(&mutex->cs);

        /* On SMP systems, a non-zero spin count generally helps performance */
        if(pf_SetCriticalSectionSpinCount) pf_SetCriticalSectionSpinCount(&mutex->cs, 2000);
    } else {
        SDL_OutOfMemory();
    }
    return (mutex);
}

/* Free the mutex */
void
SDL_DestroyMutex(SDL_mutex * mutex)
{
    if (mutex) {
        DeleteCriticalSection(&mutex->cs);
        SDL_free(mutex);
    }
}

/* Lock the mutex */
int
SDL_mutexP(SDL_mutex * mutex)
{
    if (mutex == NULL) {
        SDL_SetError("Passed a NULL mutex");
        return -1;
    }

    EnterCriticalSection(&mutex->cs);
    return (0);
}

/* Unlock the mutex */
int
SDL_mutexV(SDL_mutex * mutex)
{
    if (mutex == NULL) {
        SDL_SetError("Passed a NULL mutex");
        return -1;
    }

    LeaveCriticalSection(&mutex->cs);
    return (0);
}

/* vi: set ts=4 sw=4 expandtab: */

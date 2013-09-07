/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2012 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "SDL_config.h"

#if SDL_VIDEO_DRIVER_RPI

/* Dummy SDL video driver implementation; this is just enough to make an
 *  SDL-based application THINK it's got a working video driver, for
 *  applications that call SDL_Init(SDL_INIT_VIDEO) when they don't need it,
 *  and also for use as a collection of stubs when porting SDL to a new
 *  platform for which you haven't yet written a valid video driver.
 *
 * This is also a great way to determine bottlenecks: if you think that SDL
 *  is a performance problem for a given platform, enable this driver, and
 *  then see if your application runs faster without video overhead.
 *
 * Initial work by Ryan C. Gordon (icculus@icculus.org). A good portion
 *  of this was cut-and-pasted from Stephane Peter's work in the AAlib
 *  SDL video driver.  Renamed to "DUMMY" by Sam Lantinga.
 */

#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

#include "SDL_rpivideo.h"
#include "SDL_rpiwindow.h"
#include "SDL_rpiopengles.h"
/*
#include "SDL_rpievents_c.h"
#include "SDL_rpiframebuffer_c.h"
*/

#include "bcm_host.h"
#include "SDL_log.h"

#define RPIVID_DRIVER_NAME "rpi"

/* Initialization/Query functions */
static int RPI_VideoInit(_THIS);
static int RPI_SetDisplayMode(_THIS, SDL_VideoDisplay * display, SDL_DisplayMode * mode);
static void RPI_VideoQuit(_THIS);

/* RPI driver bootstrap functions */

static int
RPI_Available(void)
{
    return 1;
#if 0
    const char *envr = SDL_getenv("SDL_VIDEODRIVER");
    if ((envr) && (SDL_strcmp(envr, RPIVID_DRIVER_NAME) == 0)) {
        return (1);
    }

    return (0);
#endif
}

static void
RPI_DeleteDevice(SDL_VideoDevice * device)
{
    SDL_free(device);
}

static void
RPI_PumpEvents(_THIS)
{
    // SDL_Log("RPI_PumpEvents");
}


static SDL_VideoDevice *
RPI_CreateDevice(int devindex)
{
    SDL_VideoDevice *device;

    /* Initialize all variables that we clean on shutdown */
    device = (SDL_VideoDevice *) SDL_calloc(1, sizeof(SDL_VideoDevice));
    if (!device) {
        SDL_OutOfMemory();
        if (device) {
            SDL_free(device);
        }
        return (0);
    }

    /* Set the function pointers */
    device->VideoInit = RPI_VideoInit;
    device->VideoQuit = RPI_VideoQuit;
#define X(...)
    X(GetDisplayBounds)
    X(GetDisplayModes)
    X(SetDisplayMode)
    device->CreateWindow = RPI_CreateWindow;
    X(CreateWindow,RPI)
    X(CreateWindowFrom)
    X(SetWindowTitle)
    X(SetWindowIcon)
    X(SetWindowPosition)
    X(SetWindowSize)
    X(ShowWindow)
    X(HideWindow)
    X(RaiseWindow)
    X(MaximizeWindow)
    X(MinimizeWindow)
    X(RestoreWindow)
    X(SetWindowFullscreen)
    X(SetWindowGammaRamp)
    X(GetWindowGammaRamp)
    X(SetWindowGrab)
    X(DestroyWindow)
    X(CreateWindowFramebuffer)
    X(UpdateWindowFramebuffer)
    X(DestroyWindowFramebuffer)
    X(GetWindowWMInfo)
    X(GL_LoadLibrary)
    X(GL_GetProcAddress)
    X(GL_UnloadLibrary)
    X(GL_CreateContext)
    X(GL_MakeCurrent)
    X(GL_SetSwapInterval)
    X(GL_GetSwapInterval)
    X(GL_SwapWindow)
    X(GL_DeleteContext)
    X(PumpEvents)
    X(SuspendScreenSaver)
    X(StartTextInput)
    X(StopTextInput)
    X(SetTextInputRect)
    X(SetClipboardText)
    X(GetClipboardText,NULL)
    X(HasClipboardText,false)
    // device->num_displays = 1;
    // device->displays = display;
    // device->windows = window;
    // device->window_magic = 0;
    // device->next_object_id = 0;
    // device->clipboard_text = "";
    // device->GetDisplayModes = PND_getdisplaymodes;
    device->SetDisplayMode = RPI_SetDisplayMode;
    device->GL_LoadLibrary = RPI_GLES_LoadLibrary;
    device->GL_GetProcAddress = RPI_GLES_GetProcAddress;
    device->GL_UnloadLibrary = RPI_GLES_UnloadLibrary;
    device->GL_CreateContext = RPI_GLES_CreateContext;
    device->GL_MakeCurrent = RPI_GLES_MakeCurrent;
    device->GL_SetSwapInterval = RPI_GLES_SetSwapInterval;
    device->GL_GetSwapInterval = RPI_GLES_GetSwapInterval;
    device->GL_SwapWindow = RPI_GLES_SwapWindow;
    device->GL_DeleteContext = RPI_GLES_DeleteContext;
    device->PumpEvents = RPI_PumpEvents;
    // device->CreateWindowFramebuffer = SDL_RPI_CreateWindowFramebuffer;
    // device->UpdateWindowFramebuffer = SDL_RPI_UpdateWindowFramebuffer;
    // device->DestroyWindowFramebuffer = SDL_RPI_DestroyWindowFramebuffer;

    device->free = RPI_DeleteDevice;
    
    SDL_VideoData *data;
    data = (struct SDL_VideoData *) SDL_calloc(1, sizeof(SDL_VideoData));
    if (!data) {
        SDL_OutOfMemory();
        SDL_free(device);
        return NULL;
    }
    device->driverdata = data;

    struct SDL_PrivateGLESData* gles_data = (struct SDL_PrivateGLESData *) SDL_calloc(1, sizeof(SDL_PrivateGLESData));
    if (!gles_data) {
        SDL_OutOfMemory();
        return NULL;
    }
    gles_data->egl_display = EGL_DEFAULT_DISPLAY;
    device->gles_data = gles_data;
    
    return device;
}

VideoBootStrap RPI_bootstrap = {
    RPIVID_DRIVER_NAME, "SDL RaspberryPI Video Driver",
    RPI_Available, RPI_CreateDevice
};

int
RPI_VideoInit(_THIS)
{
    SDL_Log("RPI_VideoInit 23!");
    bcm_host_init();

    SDL_DisplayMode mode;

    int32_t success;
    uint32_t w;
    uint32_t h;
    success = graphics_get_display_size(0 /* LCD */, &w, &h);
    mode.w = (int) w;
    mode.h = (int) h;
    //dst_rect.x = 0;
    //dst_rect.y = 0;

    /* Use a fake 32-bpp desktop mode */
    mode.format = SDL_PIXELFORMAT_RGB888;
    //mode.w = 1600;
    //mode.h = 1200;
    mode.refresh_rate = 0;
    mode.driverdata = NULL;
    if (SDL_AddBasicVideoDisplay(&mode) < 0) {
        return -1;
    }

    SDL_zero(mode);
    SDL_AddDisplayMode(&_this->displays[0], &mode);

    /* We're done! */
    return 0;
}

static int
RPI_SetDisplayMode(_THIS, SDL_VideoDisplay * display, SDL_DisplayMode * mode)
{
    SDL_Log("RPI_SetDisplayMode");
    return 0;
}

void
RPI_VideoQuit(_THIS)
{
    SDL_Log("RPI_VideoQuit");
    bcm_host_deinit();
}

#endif /* SDL_VIDEO_DRIVER_RPI */

/* vi: set ts=4 sw=4 expandtab: */

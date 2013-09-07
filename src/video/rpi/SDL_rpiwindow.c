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

#include "../SDL_sysvideo.h"
#include "SDL_rpiwindow.h"
#include "SDL_rpiopengles.h"
#include "SDL_log.h"
#include <assert.h>

// static EGL_DISPMANX_WINDOW_T nativewindow;

int
RPI_CreateWindow(_THIS, SDL_Window * window)
{
    SDL_Log("RPI_CreateWindow %dx%d", window->w, window->h);
    
    DISPMANX_ELEMENT_HANDLE_T dispman_element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_UPDATE_HANDLE_T dispman_update;
    
    VC_RECT_T dst_rect;
    VC_RECT_T src_rect;

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = window->w;
    dst_rect.height = window->h;
      
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = window->w << 16;
    src_rect.height = window->h << 16;     

    dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
    dispman_update = vc_dispmanx_update_start( 0 );
         
    dispman_element = vc_dispmanx_element_add ( dispman_update, dispman_display,
      1/*layer*/, &dst_rect, 0/*src*/,
      &src_rect, DISPMANX_PROTECTION_NONE, 0 /*alpha*/, 0/*clamp*/, 0/*transform*/);
   
    RPI_GLES_SetupAttributes(_this);

    SDL_WindowData* data;
    data = (SDL_WindowData *) SDL_malloc(sizeof(*data));
    if (!data) {
      SDL_OutOfMemory();
      return -1;
    }

    

    data->window = window;
    window->driverdata = data;

    uint32_t screen_width = window->w;
    uint32_t screen_height = window->h;
      
    data->nativewindow.element = dispman_element;
    data->nativewindow.width = screen_width;
    data->nativewindow.height = screen_height;
    vc_dispmanx_update_submit_sync( dispman_update );

    _this->gles_data->egl_surface =
      _this->gles_data->eglCreateWindowSurface(_this->gles_data->egl_display,
        _this->gles_data->egl_config,
        (NativeWindowType) &data->nativewindow, NULL);
    if (_this->gles_data->egl_surface == EGL_NO_SURFACE) {
      SDL_SetError("Could not create GLES window surface");
      return -1;
    }

#if 0
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    EGLBoolean result;
    EGLConfig config;
    EGLint num_config;

    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    result = eglInitialize(display, NULL, NULL);
    assert(EGL_FALSE != result);
    assert(display!=EGL_NO_DISPLAY);
    static const EGLint attribute_list[] =
    {
#if 0
      EGL_RED_SIZE, 8,
      EGL_GREEN_SIZE, 8,
      EGL_BLUE_SIZE, 8,
      EGL_ALPHA_SIZE, 8,
      EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
#endif
      EGL_NONE
    };
    result = eglChooseConfig(display, attribute_list, &config, 1, &num_config);
    assert(EGL_FALSE != result);

    context = eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);
    assert(context!=EGL_NO_CONTEXT);

    surface = eglCreateWindowSurface( display, config, &data->nativewindow, NULL );
    assert(surface != EGL_NO_SURFACE);

    // connect the context to the surface
    result = eglMakeCurrent(display, surface, surface, context);
    assert(EGL_FALSE != result);
#endif

    return 0; 
}

void
RPI_DestroyWindow(_THIS, SDL_Window * window)
{
    SDL_Log("RPI_DestroyWindow");
    // TODO: remove dispman element 

}

#endif /* SDL_VIDEO_DRIVER_X11 */

/* vi: set ts=4 sw=4 expandtab: */

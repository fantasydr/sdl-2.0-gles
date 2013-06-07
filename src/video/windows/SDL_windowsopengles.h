/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>

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

#ifndef _SDL_windowsopengles_h
#define _SDL_windowsopengles_h

#if SDL_VIDEO_OPENGL_ES || SDL_VIDEO_OPENGL_ES2
#include <EGL/egl.h>

#include "../SDL_sysvideo.h"

struct SDL_PrivateGLESData
{
    //XVisualInfo *egl_visualinfo;
    void *egl_dll_handle;
    EGLDisplay egl_display;
    EGLContext egl_context;     /* Current GLES context */
    EGLSurface egl_surface;
    EGLConfig egl_config;
    int egl_swapinterval;

    EGLDisplay(KHRONOS_APIENTRY *eglGetDisplay) (NativeDisplayType display);
    EGLBoolean(KHRONOS_APIENTRY *eglInitialize) (EGLDisplay dpy, EGLint * major,
                                EGLint * minor);
    EGLBoolean(KHRONOS_APIENTRY *eglTerminate) (EGLDisplay dpy);

    void *(KHRONOS_APIENTRY *eglGetProcAddress) (const char * procName);

    EGLBoolean(KHRONOS_APIENTRY *eglChooseConfig) (EGLDisplay dpy,
                                  const EGLint * attrib_list,
                                  EGLConfig * configs,
                                  EGLint config_size, EGLint * num_config);

    EGLContext(KHRONOS_APIENTRY *eglCreateContext) (EGLDisplay dpy,
                                   EGLConfig config,
                                   EGLContext share_list,
                                   const EGLint * attrib_list);

    EGLBoolean(KHRONOS_APIENTRY *eglDestroyContext) (EGLDisplay dpy, EGLContext ctx);

    EGLSurface(KHRONOS_APIENTRY *eglCreateWindowSurface) (EGLDisplay dpy,
                                         EGLConfig config,
                                         NativeWindowType window,
                                         const EGLint * attrib_list);
    EGLBoolean(KHRONOS_APIENTRY *eglDestroySurface) (EGLDisplay dpy, EGLSurface surface);

    EGLBoolean(KHRONOS_APIENTRY *eglMakeCurrent) (EGLDisplay dpy, EGLSurface draw,
                                 EGLSurface read, EGLContext ctx);

    EGLBoolean(KHRONOS_APIENTRY *eglSwapBuffers) (EGLDisplay dpy, EGLSurface draw);

    EGLBoolean(KHRONOS_APIENTRY *eglSwapInterval) (EGLDisplay dpy, EGLint interval);

    const char *(KHRONOS_APIENTRY *eglQueryString) (EGLDisplay dpy, EGLint name);

    EGLBoolean(KHRONOS_APIENTRY *eglGetConfigAttrib) (EGLDisplay dpy, EGLConfig config,
                                     EGLint attribute, EGLint * value);
};

/* OpenGLES functions */
extern SDL_GLContext WIN_GLES_CreateContext(_THIS, SDL_Window * window);
extern int WIN_GLES_SetupWindow(_THIS, SDL_Window * window);
extern int WIN_GLES_MakeCurrent(_THIS, SDL_Window * window,
                                SDL_GLContext context);
extern int WIN_GLES_GetAttribute(_THIS, SDL_GLattr attrib, int *value);
extern int WIN_GLES_LoadLibrary(_THIS, const char *path);
extern void *WIN_GLES_GetProcAddress(_THIS, const char *proc);
extern void WIN_GLES_UnloadLibrary(_THIS);

extern int WIN_GLES_SetSwapInterval(_THIS, int interval);
extern int WIN_GLES_GetSwapInterval(_THIS);
extern void WIN_GLES_SwapWindow(_THIS, SDL_Window * window);
extern void WIN_GLES_DeleteContext(_THIS, SDL_GLContext context);

#endif /* SDL_VIDEO_OPENGL_ES || SDL_VIDEO_OPENGL_ES2 */

#endif /* _SDL_windowsopengles_h */

/* vi: set ts=4 sw=4 expandtab: */

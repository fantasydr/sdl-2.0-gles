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

#if SDL_VIDEO_DRIVER_WINDOWS && (SDL_VIDEO_OPENGL_ES || SDL_VIDEO_OPENGL_ES2)

/* EGL implementation of SDL OpenGL support */
#include "SDL_windowsvideo.h"
#include "SDL_windowsopengles.h"

#define DEFAULT_EGL "libEGL.dll"
#define DEFAULT_OGL_ES2 "libGLESv2.dll"
#define DEFAULT_OGL_ES_PVR "libGLES_CM.dll"
#define DEFAULT_OGL_ES "libGLESv1_CM.dll"

#define LOAD_FUNC(NAME) \
    *((void**)&_this->gles_data->NAME) = GetProcAddress(handle, #NAME); \
    if (!_this->gles_data->NAME) \
    { \
        SDL_UnloadObject(handle); \
        return SDL_SetError("Could not retrieve EGL function " #NAME); \
    }

int
WIN_GLES_LoadLibrary(_THIS, const char *path)
{
    LPTSTR wpath = 0;
    HANDLE handle = 0;

    SDL_VideoData *data = (SDL_VideoData *) _this->driverdata;

    if (_this->gles_data) {
        return SDL_SetError("OpenGL ES context already created");
    }

    /* If SDL_GL_CONTEXT_EGL has been changed to 0, switch over to WIN_GL functions  */
    if (_this->gl_config.use_egl == 0) {
#if SDL_VIDEO_OPENGL_WGL
        _this->GL_LoadLibrary = WIN_GL_LoadLibrary;
        _this->GL_GetProcAddress = WIN_GL_GetProcAddress;
        _this->GL_UnloadLibrary = WIN_GL_UnloadLibrary;
        _this->GL_CreateContext = WIN_GL_CreateContext;
        _this->GL_MakeCurrent = WIN_GL_MakeCurrent;
        _this->GL_SetSwapInterval = WIN_GL_SetSwapInterval;
        _this->GL_GetSwapInterval = WIN_GL_GetSwapInterval;
        _this->GL_SwapWindow = WIN_GL_SwapWindow;
        _this->GL_DeleteContext = WIN_GL_DeleteContext;
        return WIN_GL_LoadLibrary(_this, path);
#else
        return SDL_SetError("SDL not configured with OpenGL/WGL support");
#endif
    }

    if (path == NULL) {
        path = SDL_getenv("SDL_VIDEO_EGL_DRIVER");
    }
    if (path == NULL) {
        path = DEFAULT_EGL;
    }
    wpath = WIN_UTF8ToString(path);
    handle = LoadLibrary(wpath);;
    SDL_free(wpath);

    if (handle == NULL) {
        return SDL_SetError("Could not load OpenGL EGL library");
    }

    /* Unload the old driver and reset the pointers */
    WIN_GLES_UnloadLibrary(_this);

    /* Allocate OpenGL memory */
    _this->gles_data = (struct SDL_PrivateGLESData *) SDL_calloc(1, sizeof(struct SDL_PrivateGLESData));
    if (!_this->gles_data) {
        return SDL_OutOfMemory();
    }

    /* Load new function pointers */
    LOAD_FUNC(eglGetDisplay);
    LOAD_FUNC(eglInitialize);
    LOAD_FUNC(eglTerminate);
    LOAD_FUNC(eglGetProcAddress);
    LOAD_FUNC(eglChooseConfig);
    LOAD_FUNC(eglGetConfigAttrib);
    LOAD_FUNC(eglCreateContext);
    LOAD_FUNC(eglDestroyContext);
    LOAD_FUNC(eglCreateWindowSurface);
    LOAD_FUNC(eglDestroySurface);
    LOAD_FUNC(eglMakeCurrent);
    LOAD_FUNC(eglSwapBuffers);
    LOAD_FUNC(eglSwapInterval);
    
    _this->gles_data->egl_display = 
        _this->gles_data->eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if (!_this->gles_data->egl_display) {
        return SDL_SetError("Could not get EGL display");
    }

    if (_this->gles_data->
        eglInitialize(_this->gles_data->egl_display, NULL,
        NULL) != EGL_TRUE) {
            return SDL_SetError("Could not initialize EGL");
    }

    _this->gles_data->egl_dll_handle = handle;

    /* Load OpenGL ES Library */
    if (_this->gl_config.major_version > 1) {
        path = DEFAULT_OGL_ES2;
        wpath = WIN_UTF8ToString(path);
        handle = LoadLibrary(wpath);

    } else {
        path = DEFAULT_OGL_ES;
        wpath = WIN_UTF8ToString(path);
        handle = LoadLibrary(wpath);

        if (handle == NULL) {
            path = DEFAULT_OGL_ES_PVR;
            wpath = WIN_UTF8ToString(path);
            handle = LoadLibrary(wpath);
        }
    }
    SDL_free(wpath);

    if (!handle) {
        char message[1024];
        SDL_snprintf(message, SDL_arraysize(message), "LoadLibrary(\"%s\")", path);
        return WIN_SetError(message);
    }

    _this->gl_config.dll_handle = handle;
    _this->gl_config.driver_loaded = 1;

    SDL_strlcpy(_this->gl_config.driver_path, path, SDL_arraysize(_this->gl_config.driver_path));

    return 0;
}

void *
WIN_GLES_GetProcAddress(_THIS, const char *proc)
{
    void *func;

    /* This is to pick up extensions */
    func = _this->gles_data->eglGetProcAddress(proc);
    if (!func) {
        /* This is probably a normal GL function */
        func = GetProcAddress(_this->gl_config.dll_handle, proc);
    }
    return func;
}

void
WIN_GLES_UnloadLibrary(_THIS)
{
    FreeLibrary((HMODULE) _this->gl_config.dll_handle);
    _this->gl_config.dll_handle = NULL;

    /* Free OpenGL memory */
    SDL_free(_this->gles_data);
    _this->gles_data = NULL;
}

int
WIN_GLES_SetupWindow(_THIS, SDL_Window * window)
{
    HWND hwnd = ((SDL_WindowData *) window->driverdata)->hwnd;

    /* 64 seems nice. */
    EGLint attribs[64];
    EGLint found_configs = 0;
    int i;

    if (!_this->gles_data) {
        /* The EGL library wasn't loaded, SDL_GetError() should have info */
        return -1;
    }

    i = 0;
    attribs[i++] = EGL_RED_SIZE;
    attribs[i++] = _this->gl_config.red_size;
    attribs[i++] = EGL_GREEN_SIZE;
    attribs[i++] = _this->gl_config.green_size;
    attribs[i++] = EGL_BLUE_SIZE;
    attribs[i++] = _this->gl_config.blue_size;

    if (_this->gl_config.alpha_size) {
        attribs[i++] = EGL_ALPHA_SIZE;
        attribs[i++] = _this->gl_config.alpha_size;
    }

    if (_this->gl_config.buffer_size) {
        attribs[i++] = EGL_BUFFER_SIZE;
        attribs[i++] = _this->gl_config.buffer_size;
    }

    attribs[i++] = EGL_DEPTH_SIZE;
    attribs[i++] = _this->gl_config.depth_size;

    if (_this->gl_config.stencil_size) {
        attribs[i++] = EGL_STENCIL_SIZE;
        attribs[i++] = _this->gl_config.stencil_size;
    }

    if (_this->gl_config.multisamplebuffers) {
        attribs[i++] = EGL_SAMPLE_BUFFERS;
        attribs[i++] = _this->gl_config.multisamplebuffers;
    }

    if (_this->gl_config.multisamplesamples) {
        attribs[i++] = EGL_SAMPLES;
        attribs[i++] = _this->gl_config.multisamplesamples;
    }

    attribs[i++] = EGL_RENDERABLE_TYPE;
    if (_this->gl_config.major_version == 2) {
        attribs[i++] = EGL_OPENGL_ES2_BIT;
    } else {
        attribs[i++] = EGL_OPENGL_ES_BIT;
    }

    attribs[i++] = EGL_NONE;

    if (_this->gles_data->eglChooseConfig(_this->gles_data->egl_display,
                                          attribs,
                                          &_this->gles_data->egl_config, 1,
                                          &found_configs) == EGL_FALSE ||
        found_configs == 0) {

        return WIN_SetError("Couldn't find matching EGL config");
    }


    /* Create the GLES window surface */
    _this->gles_data->egl_surface = 
        _this->gles_data->eglCreateWindowSurface(_this->gles_data->egl_display,
                                                _this->gles_data->egl_config,
                                                (NativeWindowType) hwnd, NULL);

    if (_this->gles_data->egl_surface == EGL_NO_SURFACE) {
        return WIN_SetError("Could not create GLES window surface");
    }

    return 0;
}

SDL_GLContext
WIN_GLES_CreateContext(_THIS, SDL_Window * window)
{
    EGLint context_attrib_list[] = {
        EGL_CONTEXT_CLIENT_VERSION,
        1,
        EGL_NONE
    };

    SDL_WindowData *data = (SDL_WindowData *) window->driverdata;
    SDL_GLContext context = (SDL_GLContext)1;

    if (_this->gl_config.major_version) {
        context_attrib_list[1] = _this->gl_config.major_version;
    }

    _this->gles_data->egl_context =
        _this->gles_data->eglCreateContext(_this->gles_data->egl_display,
                                           _this->gles_data->egl_config,
                                           EGL_NO_CONTEXT, context_attrib_list);

    if (_this->gles_data->egl_context == EGL_NO_CONTEXT) {
        SDL_SetError("Could not create EGL context");
        return NULL;
    }

    _this->gles_data->egl_swapinterval = 0;

    if (WIN_GLES_MakeCurrent(_this, window, context) < 0) {
        WIN_GLES_DeleteContext(_this, context);
        return NULL;
    }

    return context;
}

int
WIN_GLES_MakeCurrent(_THIS, SDL_Window * window, SDL_GLContext context)
{
    if (!_this->gles_data) {
        return SDL_SetError("OpenGL not initialized");
    }

    if (!_this->gles_data->eglMakeCurrent(_this->gles_data->egl_display,
                                          _this->gles_data->egl_surface,
                                          _this->gles_data->egl_surface,
                                          _this->gles_data->egl_context)) {
        return SDL_SetError("Unable to make EGL context current");
    }

    return 1;
}

int
WIN_GLES_SetSwapInterval(_THIS, int interval)
{
    EGLBoolean status;

    if (_this->gles_data) {
        return SDL_SetError("OpenGL ES context not active");
    }

    status = _this->gles_data->eglSwapInterval(_this->gles_data->egl_display, interval);
    if (status == EGL_TRUE) {
        _this->gles_data->egl_swapinterval = interval;
        return 0;
    }

    return SDL_SetError("Unable to set the EGL swap interval");
}

int
WIN_GLES_GetSwapInterval(_THIS)
{
    if (_this->gles_data) {
        return SDL_SetError("OpenGL ES context not active");
    }

    return _this->gles_data->egl_swapinterval;
}

void
WIN_GLES_SwapWindow(_THIS, SDL_Window * window)
{
    _this->gles_data->eglSwapBuffers(_this->gles_data->egl_display,
                                     _this->gles_data->egl_surface);
}

void
WIN_GLES_DeleteContext(_THIS, SDL_GLContext context)
{
    /* Clean up GLES and EGL */
    if (!_this->gles_data) {
        return;
    }

    if (_this->gles_data->egl_context != EGL_NO_CONTEXT ||
        _this->gles_data->egl_surface != EGL_NO_SURFACE) {
        _this->gles_data->eglMakeCurrent(_this->gles_data->egl_display,
                                         EGL_NO_SURFACE, EGL_NO_SURFACE,
                                         EGL_NO_CONTEXT);

        if (_this->gles_data->egl_context != EGL_NO_CONTEXT) {
            _this->gles_data->eglDestroyContext(_this->gles_data->egl_display,
                                                _this->gles_data->
                                                egl_context);
            _this->gles_data->egl_context = EGL_NO_CONTEXT;
        }

        if (_this->gles_data->egl_surface != EGL_NO_SURFACE) {
            _this->gles_data->eglDestroySurface(_this->gles_data->egl_display,
                                                _this->gles_data->
                                                egl_surface);
            _this->gles_data->egl_surface = EGL_NO_SURFACE;
        }
    }
}


#endif /* SDL_VIDEO_DRIVER_WINDOWS && SDL_VIDEO_OPENGL_ES */

/* vi: set ts=4 sw=4 expandtab: */

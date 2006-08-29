/* DO NOT EDIT!  This file is generated by sdlgenblit.pl */
/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2006 Sam Lantinga

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

/* *INDENT-OFF* */

#define SDL_RENDERCOPY_MODULATE_COLOR   0x0001
#define SDL_RENDERCOPY_MODULATE_ALPHA   0x0002
#define SDL_RENDERCOPY_MASK             0x0010
#define SDL_RENDERCOPY_BLEND            0x0020
#define SDL_RENDERCOPY_ADD              0x0040
#define SDL_RENDERCOPY_MOD              0x0080
#define SDL_RENDERCOPY_NEAREST          0x0100

typedef struct {
    void *src;
    int src_w, src_h;
    int src_pitch;
    void *dst;
    int dst_w, dst_h;
    int dst_pitch;
    void *aux_data;
    int flags;
    Uint8 r, g, b, a;
} SDL_RenderCopyData;

typedef int (*SDL_RenderCopyFunc)(SDL_RenderCopyData *data);

extern SDL_RenderCopyFunc SDLCALL SDL_GetRenderCopyFunc(Uint32 src_format, Uint32 dst_format, int modMode, int blendMode, int scaleMode);

extern int SDLCALL SDL_RenderCopy_RGB888_RGB888_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGB888_RGB888_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGB888_RGB888_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGB888_RGB888_Modulate(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGB888_RGB888_Modulate_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGB888_RGB888_Modulate_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGB888_RGB888_Modulate_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGB888_BGR888_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGB888_BGR888_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGB888_BGR888_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGB888_BGR888_Modulate(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGB888_BGR888_Modulate_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGB888_BGR888_Modulate_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGB888_BGR888_Modulate_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGR888_RGB888_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGR888_RGB888_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGR888_RGB888_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGR888_RGB888_Modulate(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGR888_RGB888_Modulate_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGR888_RGB888_Modulate_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGR888_RGB888_Modulate_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGR888_BGR888_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGR888_BGR888_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGR888_BGR888_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGR888_BGR888_Modulate(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGR888_BGR888_Modulate_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGR888_BGR888_Modulate_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGR888_BGR888_Modulate_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ARGB8888_RGB888_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ARGB8888_RGB888_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ARGB8888_RGB888_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ARGB8888_RGB888_Modulate(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ARGB8888_RGB888_Modulate_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ARGB8888_RGB888_Modulate_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ARGB8888_RGB888_Modulate_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ARGB8888_BGR888_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ARGB8888_BGR888_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ARGB8888_BGR888_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ARGB8888_BGR888_Modulate(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ARGB8888_BGR888_Modulate_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ARGB8888_BGR888_Modulate_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ARGB8888_BGR888_Modulate_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGBA8888_RGB888_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGBA8888_RGB888_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGBA8888_RGB888_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGBA8888_RGB888_Modulate(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGBA8888_RGB888_Modulate_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGBA8888_RGB888_Modulate_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGBA8888_RGB888_Modulate_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGBA8888_BGR888_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGBA8888_BGR888_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGBA8888_BGR888_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGBA8888_BGR888_Modulate(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGBA8888_BGR888_Modulate_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGBA8888_BGR888_Modulate_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_RGBA8888_BGR888_Modulate_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ABGR8888_RGB888_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ABGR8888_RGB888_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ABGR8888_RGB888_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ABGR8888_RGB888_Modulate(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ABGR8888_RGB888_Modulate_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ABGR8888_RGB888_Modulate_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ABGR8888_RGB888_Modulate_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ABGR8888_BGR888_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ABGR8888_BGR888_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ABGR8888_BGR888_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ABGR8888_BGR888_Modulate(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ABGR8888_BGR888_Modulate_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ABGR8888_BGR888_Modulate_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_ABGR8888_BGR888_Modulate_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGRA8888_RGB888_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGRA8888_RGB888_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGRA8888_RGB888_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGRA8888_RGB888_Modulate(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGRA8888_RGB888_Modulate_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGRA8888_RGB888_Modulate_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGRA8888_RGB888_Modulate_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGRA8888_BGR888_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGRA8888_BGR888_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGRA8888_BGR888_Blend_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGRA8888_BGR888_Modulate(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGRA8888_BGR888_Modulate_Scale(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGRA8888_BGR888_Modulate_Blend(SDL_RenderCopyData *data);
extern int SDLCALL SDL_RenderCopy_BGRA8888_BGR888_Modulate_Blend_Scale(SDL_RenderCopyData *data);

/* *INDENT-ON* */

/* vi: set ts=4 sw=4 expandtab: */

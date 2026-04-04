#ifndef _6BC0_H_
#define _6BC0_H_

#include "global.h"

// image size in bits
typedef enum IMAGE_SIZE_BITS {
	IMAGE_SIZE_BITS_4b  = 0,
	IMAGE_SIZE_BITS_8b  = 1,
	IMAGE_SIZE_BITS_16b = 2,
	IMAGE_SIZE_BITS_32b = 3,
} IMAGE_SIZE_BITS;

typedef enum IMAGE_FMT {
	IMAGE_FMT_RGBA = 0,
	IMAGE_FMT_YUV  = 1,
	IMAGE_FMT_CI   = 2,
	IMAGE_FMT_IA   = 3,
	IMAGE_FMT_I    = 4,
} IMAGE_FMT;

typedef struct ColorBuffer {
	/* 0x00 */ u16 fmt;
	/* 0x02 */ u16 size;
	/* 0x04 */ u16 width;
	/* 0x06 */ u16 height;
	/* 0x08 */ u8* img_p;
	/* 0x0C */ struct ColorBuffer* depth_p;
} ColorBuffer; // size = 0x10

ScissorRect* ScissorRect_Set(ScissorRect* arg0, s16 x1, s16 y1, s16 x2, s16 y2);
s32 ScissorRect_Clip(ScissorRect* arg0);
void GFX_SetScissor(Gfx** gfx_p, s16 x, s16 y, s16 width, s16 height);
void GFX_RestoreScissor(Gfx** gfx_p);
void ColorBuffer_Init(ColorBuffer* arg0, s32 fmt, s32 size, s32 width, s32 height, u32 img_p);
ColorBuffer* ColorBuffer_Alloc(s32 fmt, s32 size, s32 width, s32 height, s32 side);
void ColorBuffer_AttachDepth(ColorBuffer* img, ColorBuffer* depth);
void GFX_ClearActiveBuffer(void);
ColorBuffer* ColorBuffer_GetActive(void);
void ColorBuffer_Activate(Gfx** gfx_p, ColorBuffer* arg1);
void GFX_FillRect(Gfx** gfx_p, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u16 fill_colour);
void GFX_ClearDepth(Gfx** gfx_p, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void GFX_ClearScreen(Gfx** gfx_p, u16 fill_colour);


#endif // _6BC0_H_

#ifndef _STAGE_LOADER_H_
#define _STAGE_LOADER_H_

#include "global.h"
#include "src/6BC0.h"

typedef struct FrameDesc {
    /* 0x00 */ u8 depth_type;
    /* 0x01 */ u8 res_mode;
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 buf_index;
    /* 0x04 */ Gfx* dl_start;
    /* 0x08 */ Gfx* dl_end;
    /* 0x0C */ void* framebuffer;
    /* 0x10 */ char pad[0x4];
} FrameDesc; // size >= 0x14

typedef struct RenderContext {
    /* 0x00 */ s16 width;
    /* 0x02 */ s16 height;
    /* 0x04 */ s16 scissor_x1;
    /* 0x06 */ s16 scissor_x2;
    /* 0x08 */ s16 scissor_y1;
    /* 0x0A */ s16 scissor_y2;
    /* 0x0C */ s8 res_mode;
    /* 0x0D */ s8 unk_0D;
    /* 0x0E */ s8 num_buffers;
    /* 0x0F */ s8 depth_flag;
    /* 0x10 */ s8 depth_type;
    /* 0x11 */ u8 fade_mode;
    /* 0x12 */ u8 fade_duration;
    /* 0x13 */ u8 fade_counter;
    /* 0x14 */ u16 fill_color;
    /* 0x16 */ s8 buf_index;
    /* 0x17 */ u8 pad17;
    /* 0x18 */ ColorBuffer* bufs[3];
} RenderContext; // size = 0x24

void Stage_DrawFade(void);
s32 Stage_StartFade(s32 arg0);
s32 Stage_FadeOut(s32 arg0);
s32 Stage_FadeIn(s32 arg0);
void Stage_SetFadeMode(s32 arg0);
void Stage_SetProfilerFlags(u32 arg0, u32 arg1);
void Stage_InitRDP(void);
void Stage_InitRSP(void);
void Stage_InitFrame(void);
void Stage_BeginFrameDraw(void);
void Stage_EndFrame(void);
void Stage_SwapBuffers(void);
void Stage_ClearScreen(void);
RenderContext* Stage_CreateRenderContext(s8 arg0, s8 arg1, s8 arg2, s8 arg3, s8 arg4, s32 arg5);
RenderContext* Stage_GetRenderContext(void);
s32 Stage_GetFadeMode(void);
void Stage_SwapRenderContext(RenderContext* arg0);
void Stage_SetRenderContext(RenderContext* arg0);
void Stage_FreeRenderContext(void);
void Stage_WaitFrame(void);
void Stage_SetSegments(void);
s32 Stage_AdvanceFrame(void);
void Stage_AdvanceFrames(s32 arg0);
s32 Stage_RunLoop(u32 arg0, s32 (*arg1)(u8));
s32 Stage_RunFadeLoop(s32 (*arg0)(u8), s32 arg1, s32 arg2);
void Stage_SetFillColor(u16 arg0);
void Stage_ActivateFramebuffer(void);
ColorBuffer* Stage_GetFramebuffer(void);
s32 Stage_IsHighRes(void);
void HAL_BeginDraw(void);
void HAL_EndDraw(void);
void HAL_DrawCharLow(s16 arg0, s16 arg1, s16 arg2);
void HAL_DrawCharHigh(s16 arg0, s16 arg1, s16 arg2);
void HAL_DrawString(s16 arg0, s16 arg1, s8* arg2, s32 arg3);
void HAL_Flush(Gfx** arg0, s32 arg1);
void HAL_ClearPersistent(void);
char* HAL_AppendStr(char* buffer, const char* data, size_t size);
s32 HAL_Printf(s16 x, s16 y, const char* fmt, ...);
s32 HAL_Printf_Persist(s16 x, s16 y, const char* fmt, ...);


#endif // _STAGE_LOADER_H_

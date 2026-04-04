#ifndef _SRC_DP_INTRO_H_
#define _SRC_DP_INTRO_H_

#include "global.h"
#include "3FB0.h"

void RSPTask_Init(RSPTask* arg0);
void RSPTask_Submit(RSPTask* arg0, struct FrameConfig* arg1, s32 arg2);
void VI_SetMode(s8 arg0, s8 arg1);
void Display_SetConfig(struct FrameConfig* arg0);
void Display_ProcessFrame(void);
void Display_DrainScheduler(void);
void DisplayThread_Main(UNUSED void* arg);
void Display_Init(void);
void Display_SetBorderColor(u16 color);
u16 Display_GetBorderColor(void);
s32 Display_IsShowingBorder(void);
void Display_WaitForFrame(void);
void Display_SendFrame(void* arg0);
void Display_FlushFrames(s32 arg0);
void FrameConfig_Init(struct FrameConfig* arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, s32 arg5, s32 arg6, s32 arg7);
s32 Display_GetVBlanks(void);
void VI_RestoreMode(void);
s32 Display_IsReady(void);
void VI_SetBlack(void);
void VI_ClearBlack(void);

#endif /* _SRC_DP_INTRO_H_ */

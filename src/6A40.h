#ifndef _6A40_H_
#define _6A40_H_

#include "global.h"

void DLBuf_Init(u32 arg0, s32 arg1);
void DLBuf_Free(void);
void DLBuf_Swap(void);
void DLBuf_GetInfo(Gfx** arg0, Gfx** arg1);
void* DLBuf_AllocTemp(s32 arg0);
s32 DLBuf_GetRemaining(void);

#endif // _6A40_H_

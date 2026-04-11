#ifndef _SRC_GB_TOWER_H_
#define _SRC_GB_TOWER_H_

#include "global.h"

s32 func_8000A630(s32 arg0, void* arg1);
s32 func_8000A6D8(s32 arg0, u8* arg1);
s32 func_8000A798(s32 arg0, u8* arg1, u8* arg2);
s32 func_8000A888(s32 arg0, u8 arg1);
void GbPak_PowerOffAll(void);
s32 GbPak_CheckGameTitle(OSGbpakId* header);
s32 GbPak_InitAll(void);
s32 GbPak_IsCartridgeConnected(s32 arg0);
s32 func_8000ACF4(s32 arg0);
s32 GbPak_InitPak(s32 arg0);
s32 func_8000AE28(s32 arg0, void* arg1);
s32 GbPak_ReadWriteMbc(s32 arg0, void* arg1, u16 arg2, u16 arg3);
s32 func_8000AF40(s32 arg0, void* arg1, u16 arg2, u16 arg3);
s32 GbPak_ReadWrite(s32 arg0, u8* arg1, u16 arg2, u16 arg3);
s32 func_8000B048(s32 arg0, u8* arg1, u16 arg2, u16 arg3);
int func_8000B0CC(s32 arg0, s32 arg1);
s32 func_8000B1C4(s32 arg0, u8* arg1, s32 arg2, s32 arg3);
u8 func_8000B318(u8 arg0);
void GbPak_UpdateStatus(void);

#endif /* _SRC_GB_TOWER_H_ */

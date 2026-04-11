#ifndef _11BA0_H_
#define _11BA0_H_

#include "global.h"
#include "src/memory.h"

typedef struct unk_func_80011B94 {
    /* 0x00 */ unk_D_86002F58_004_000_000 unk_00;
    /* 0x18 */ Gfx* unk_18;
} unk_func_80011B94; // size = 0x1C

typedef struct unk_D_8690A610_018 {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ Color_RGBA8_u32 unk_04;
} unk_D_8690A610_018; // size = 0x8

typedef struct unk_D_8690A610 {
    /* 0x00 */ unk_D_86002F58_004_000_000 unk_00;
    /* 0x18 */ unk_D_8690A610_018 unk_18;
} unk_D_8690A610; // size = 0x20

void GraphNode_Init(GraphNode* arg0, u8 arg1);
void GraphNode_SetCallback(GraphNode* arg0, func_D_86002F34_000_010 arg1, void* arg2);
GraphNode* GraphNode_Create(MainPoolState* arg0, GraphNode* arg1);
unk_D_86002F34_alt2* GraphNode_CreateRoot(MainPoolState* arg0, unk_D_86002F34_alt2* arg1, GraphNode* arg2);
unk_D_86002F34_00C* GraphNode_CreateCamera(MemoryBlock* arg0, unk_D_86002F34_00C* arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5);
unk_D_86002F34_alt13* GraphNode_CreateOrtho(MemoryBlock* arg0, unk_D_86002F34_alt13* arg1, s16 arg2);
unk_D_86002F34_alt1* GraphNode_CreatePerspective(MemoryBlock* arg0, unk_D_86002F34_alt1* arg1, s16 arg2);
unk_D_86002F34_alt14* GraphNode_CreateMaster(MainPoolState* arg0, unk_D_86002F34_alt14* arg1, s16 arg2);
unk_D_86002F34* GraphNode_CreateDisplayList(MainPoolState* arg0, unk_D_86002F34* arg1, s32 arg2, unk_D_86002F34_018* arg3, s32 arg4, unk_D_86002F34_01C* arg5, s32 arg6, Vtx* arg7);
unk_D_86002F34_alt5* GraphNode_CreateTranslationRotation(MainPoolState* arg0, unk_D_86002F34_alt5* arg1, Vec3f* arg2, Vec3s* arg3);
unk_D_86002F34_alt22* GraphNode_CreateTranslation(MainPoolState* arg0, unk_D_86002F34_alt22* arg1, Vec3f* arg2);
unk_D_86002F34_alt6* GraphNode_CreateBillboard(MainPoolState* arg0, unk_D_86002F34_alt6* arg1, s16 arg2, s16 arg3, s16 arg4, Vec3s* arg5, Vec3s* arg6, Vec3f* arg7);
void Camera_SetViewport(unk_D_86002F34_00C* arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
void Camera_SetPerspective(unk_D_86002F34_00C* arg0, f32 arg1, f32 arg2, f32 arg3);
void Camera_LookAt(unk_D_86002F34_00C* arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7);
void GraphNode_SetLighting(unk_D_86002F34_00C* arg0, s16 arg1, s16 arg2, u8 arg3, u8 arg4, u8 arg5);
void GraphNode_AppendChild(GraphNode* arg0, GraphNode* arg1);
void GraphNode_RemoveChild(GraphNode* arg0, GraphNode* arg1);
GraphNode* GraphNode_GetChildByIndex(GraphNode* arg0, s32 arg1);

#endif // _11BA0_H_

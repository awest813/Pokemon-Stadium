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
unk_D_86002F34_00C* GraphNode_CreateCamera(MainPoolState* arg0, unk_D_86002F34_00C* arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5);
unk_D_86002F34_alt13* GraphNode_CreateOrtho(MainPoolState* arg0, unk_D_86002F34_alt13* arg1, s16 arg2);
unk_D_86002F34_alt1* GraphNode_CreatePerspective(MainPoolState* arg0, unk_D_86002F34_alt1* arg1, s16 arg2);
unk_D_86002F34_alt14* GraphNode_CreateMaster(MainPoolState* arg0, unk_D_86002F34_alt14* arg1, s16 arg2);
unk_D_86002F34* GraphNode_CreateDisplayList(MainPoolState* arg0, unk_D_86002F34* arg1, s32 arg2, unk_D_86002F34_018* arg3, s32 arg4, unk_D_86002F34_01C* arg5, s32 arg6, Vtx* arg7);
unk_D_86002F34_alt5* GraphNode_CreateTranslationRotation(MainPoolState* arg0, unk_D_86002F34_alt5* arg1, Vec3f* arg2, Vec3s* arg3);
unk_D_86002F34_alt22* GraphNode_CreateTranslation(MainPoolState* arg0, unk_D_86002F34_alt22* arg1, Vec3f* arg2);
unk_D_86002F34_alt6* GraphNode_CreateBillboard(MainPoolState* arg0, unk_D_86002F34_alt6* arg1, s16 arg2, s16 arg3, s16 arg4, Vec3s* arg5, Vec3s* arg6, Vec3f* arg7);
unk_D_8690A610* GraphNode_CreateBackground(MainPoolState* arg0, unk_D_8690A610* arg1, s16 arg2, s16 arg3, u8 r, u8 g, u8 b, u8 a);
unk_D_86002F34_alt18* GraphNode_CreateClearDepth(MainPoolState* arg0, unk_D_86002F34_alt18* arg1, s16 arg2, s16 arg3, u8 r, u8 g, u8 b, u8 a);
unk_D_86002F34_alt19* GraphNode_CreateMasterChild(MainPoolState* arg0, unk_D_86002F34_alt19* arg1, u8 r, u8 g, u8 b);
unk_D_86002F58_004_000* GraphNode_CreateViewportChild(MainPoolState* arg0, unk_D_86002F58_004_000* arg1, s16 arg2, Vec3f* arg3, Vec3s* arg4, Vec3f* arg5);
void Camera_SetViewport(unk_D_86002F34_00C* arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
void Camera_SetPerspective(unk_D_86002F34_00C* arg0, f32 arg1, f32 arg2, f32 arg3);
void Camera_LookAt(unk_D_86002F34_00C* arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7);
void GraphNode_SetLighting(unk_D_86002F34_00C* arg0, s16 arg1, s16 arg2, u8 arg3, u8 arg4, u8 arg5);
// Still-unnamed node allocators (Pass B rename targets)
unk_D_86002F34_alt15* GraphNode_AllocNode(MainPoolState* arg0, unk_D_86002F34_alt15* arg1, s16 arg2);
unk_D_86002F34_alt16* GraphNode_CreateScreenColor(MainPoolState* arg0, unk_D_86002F34_alt16* arg1, u8 arg2, u8 arg3, u8 arg4);
unk_D_86002F34_alt17* GraphNode_AllocGroupNode(MainPoolState* arg0, unk_D_86002F34_alt17* arg1);
unk_D_86002F34_alt12* GraphNode_CreateViewport(MainPoolState* arg0, unk_D_86002F34_alt12* arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5);
unk_D_86002F34_alt20* GraphNode_CreateScissor(MainPoolState* arg0, unk_D_86002F34_alt20* arg1, s16 arg2, s16 arg3);
unk_D_86002F34_alt21* GraphNode_CreateSimplePerspective(MainPoolState* arg0, unk_D_86002F34_alt21* arg1, s16 arg2, s16 arg3);
unk_D_86002F34_alt5* GraphNode_CreateRotation(MainPoolState* arg0, unk_D_86002F34_alt5* arg1, Vec3f* arg2, Vec3s* arg3);
unk_D_86002F34_alt22* GraphNode_CreateTranslation(MainPoolState* arg0, unk_D_86002F34_alt22* arg1, Vec3f* arg2);
unk_D_86002F34_alt6* GraphNode_CreateBillboardNode(MainPoolState* arg0, unk_D_86002F34_alt6* arg1, s16 arg2, s16 arg3, s16 arg4, Vec3s* arg5, Vec3s* arg6, Vec3f* arg7);
unk_D_86002F34_alt7* GraphNode_CreateDisplayListWithPadding(MainPoolState* arg0, unk_D_86002F34_alt7* arg1, s32 arg2, Gfx* arg3, s16 arg4);
unk_D_86002F34_alt8* GraphNode_CreateMatrixTransform(MainPoolState* arg0, unk_D_86002F34_alt8* arg1, s32 arg2, Gfx* arg3, MtxF* arg4);
unk_D_86002F34_alt8* GraphNode_CreatePosRotTransform(MainPoolState* arg0, unk_D_86002F34_alt8* arg1, s32 arg2, Gfx* arg3, Vec3f* arg4, Vec3s* arg5);
unk_D_86002F34_alt9* GraphNode_CreateScale(MainPoolState* arg0, unk_D_86002F34_alt9* arg1, s32 arg2, Gfx* arg3, Vec3f* arg4, f32 arg5);
unk_func_80011B94* GraphNode_CreateDisplayListBranch(MainPoolState* arg0, unk_func_80011B94* arg1, s32 arg2, Gfx* arg3);
unk_D_86002F34_alt10* GraphNode_CreateFog(MainPoolState* arg0, unk_D_86002F34_alt10* arg1, s16 arg2, s16 arg3, Gfx* arg4, s16 arg5, s16 arg6, u8 r, u8 g, u8 b, u8 a);
unk_D_86002F34_alt23* GraphNode_CreateLayerSelect(MainPoolState* arg0, unk_D_86002F34_alt23* arg1, s16 arg2);
unk_D_86002F34_alt24* GraphNode_CreateAnimatedBillboard(MainPoolState* arg0, unk_D_86002F34_alt24* arg1, s32 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7, s16 arg8, s32 arg9, u8 r, u8 g, u8 b, u8 a);
void GraphNode_SetupFog(unk_D_86002F34_00C* arg0, s16 arg1, s16 arg2, UNUSED s32 arg3, UNUSED u32 arg4);
void GraphNode_AppendChild(GraphNode* arg0, GraphNode* arg1);
void GraphNode_RemoveChild(GraphNode* arg0, GraphNode* arg1);
GraphNode* GraphNode_GetChildByIndex(GraphNode* arg0, s32 arg1);

#endif // _11BA0_H_

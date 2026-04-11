#include "20470.h"
#include "src/1CF30.h"
#include "src/2E110.h"
#include "src/6A40.h"
#include "src/6BC0.h"
#include "src/hal_libc.h"
#include "src/stage_loader.h"

extern u8 D_10025D0[];
extern u8 D_1002650[];
extern u8 D_10026D0[];
extern u8 D_1002750[];
extern u8 D_10027D0[];
extern u8 D_1002850[];
extern u8 D_10028D0[];
extern u8 D_1002950[];
extern u8 D_10029D0[];
extern u8 D_1002A50[];
extern u8 D_1002AD0[];
extern u8 D_1002B50[];
extern u8 D_1002BD0[];
extern u8 D_1002C50[];
extern u8 D_1002CD0[];
extern u8 D_1002D50[];
extern u8 D_1002DD0[];
extern u8 D_1002E50[];
extern u8 D_1002ED0[];
extern u8 D_1002F50[];
extern u8 D_1002FD0[];
extern u8 D_1003050[];
extern u8 D_10030D0[];
extern u8 D_1003150[];
extern u8 D_10031D0[];
extern u8 D_1003750[];
extern u8 D_1003890[];
extern u8 D_1003930[];
extern u8 D_10039F0[];
extern u8 D_1003AB0[];
extern u8 D_1003B70[];
extern u8 D_1003C30[];
extern u8 D_1003CF0[];
extern u8 D_1003DB0[];
extern u8 D_1003E50[];
extern u8 D_1003EA0[];
extern u8 D_1003EC8[];
extern u8 D_1003EF0[];
extern u8 D_2004000[];
extern u8 D_2004320[];

void func_8001F870(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8* arg4) {
    s32 sp34 = (arg2 < 9) ? 0x400 : 0;
    s32 sp30 = (arg3 < 9) ? 0x400 : 0;

    gDPLoadTextureBlock(gDisplayListHead++, arg4, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 8, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    func_8001C330(arg0, arg1, arg2, arg3, 0, 0, sp34, sp30, 0);
}

void func_8001F9D4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_8001F870(arg0, arg1, 8, 8, D_10027D0);
    func_8001F870(arg0, (arg1 + arg3) - 8, 8, 8, D_1002850);
    func_8001F870((arg0 + arg2) - 8, (arg1 + arg3) - 8, 8, 8, D_10028D0);
    func_8001F870((arg0 + arg2) - 8, arg1, 8, 8, D_1002950);

    if (arg2 > 16) {
        func_8001F870(arg0 + 8, arg1, arg2 - 0x10, 8, D_10025D0);
        func_8001F870(arg0 + 8, (arg1 + arg3) - 8, arg2 - 0x10, 8, D_1002650);
    }

    if (arg3 > 16) {
        func_8001F870(arg0, arg1 + 8, 8, arg3 - 0x10, D_10026D0);
        func_8001F870((arg0 + arg2) - 8, arg1 + 8, 8, arg3 - 0x10, D_1002750);
    }
}

void func_8001FB40(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_8001F870(arg0, arg1, 8, 8, D_1002BD0);
    func_8001F870(arg0, (arg1 + arg3) - 8, 8, 8, D_1002C50);
    func_8001F870((arg0 + arg2) - 8, (arg1 + arg3) - 8, 8, 8, D_1002CD0);
    func_8001F870((arg0 + arg2) - 8, arg1, 8, 8, D_1002D50);

    if (arg2 > 16) {
        func_8001F870(arg0 + 8, arg1, arg2 - 0x10, 8, D_10029D0);
        func_8001F870(arg0 + 8, (arg1 + arg3) - 8, arg2 - 0x10, 8, D_1002A50);
    }

    if (arg3 > 16) {
        func_8001F870(arg0, arg1 + 8, 8, arg3 - 0x10, D_1002AD0);
        func_8001F870((arg0 + arg2) - 8, arg1 + 8, 8, arg3 - 0x10, D_1002B50);
    }
}

void func_8001FCAC(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_8001F870(arg0, arg1, 8, 8, D_1002FD0);
    func_8001F870(arg0, (arg1 + arg3) - 8, 8, 8, D_1003050);
    func_8001F870((arg0 + arg2) - 8, (arg1 + arg3) - 8, 8, 8, D_10030D0);
    func_8001F870((arg0 + arg2) - 8, arg1, 8, 8, D_1003150);

    if (arg2 > 16) {
        func_8001F870(arg0 + 8, arg1, arg2 - 0x10, 8, D_1002DD0);
        func_8001F870(arg0 + 8, (arg1 + arg3) - 8, arg2 - 0x10, 8, D_1002E50);
    }

    if (arg3 > 16) {
        func_8001F870(arg0, arg1 + 8, 8, arg3 - 0x10, D_1002ED0);
        func_8001F870((arg0 + arg2) - 8, arg1 + 8, 8, arg3 - 0x10, D_1002F50);
    }
}

void func_8001FE18(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4) {
    gSPDisplayList(gDisplayListHead++, D_8006F4C0);
    gDPSetFillColor(gDisplayListHead++, (arg4 << 0x10) | arg4);

    if (Stage_IsHighRes() != 0) {
        gDPFillRectangle(gDisplayListHead++, arg0 + 7, arg1 + 7, (arg0 + arg2) - 8, (arg1 + arg3) - 8);
    } else {
        gDPFillRectangle(gDisplayListHead++, arg0 + 3, arg1 + 3, (arg0 + arg2) - 4, (arg1 + arg3) - 4);
    }
}

void func_8001FF68(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4) {
    gSPDisplayList(gDisplayListHead++, D_8006F4C0);
    gDPSetFillColor(gDisplayListHead++, (arg4 << 0x10) | arg4);

    if (Stage_IsHighRes() != 0) {
        gDPFillRectangle(gDisplayListHead++, arg0 + 5, arg1 + 5, (arg0 + arg2) - 6, (arg1 + arg3) - 6);
    } else {
        gDPFillRectangle(gDisplayListHead++, arg0 + 3, arg1 + 3, (arg0 + arg2) - 4, (arg1 + arg3) - 4);
    }
}

void func_800200B8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, Color_RGB8* arg4, Color_RGB8* arg5) {
    Vtx* temp_s2;
    ColorBuffer* temp_s1;
    Mtx* sp7C;
    Vp* sp78;

    temp_s1 = ColorBuffer_GetActive();
    temp_s2 = DLBuf_AllocTemp(sizeof(Vtx) * 4);
    sp7C = DLBuf_AllocTemp(sizeof(Mtx) * 1);
    sp78 = DLBuf_AllocTemp(sizeof(Vp) * 1);

    func_8001E6E8(sp78, temp_s1->width, temp_s1->height);

    gSPViewport(gDisplayListHead++, (u32)sp78 & 0x1FFFFFFF);

    guOrtho(sp7C, 0.5f, temp_s1->width - 0.5f, temp_s1->height - 0.5f, 0.5f, -2.0f, 2.0f, 1.0f);
    func_8001E680(&temp_s2[0], arg0, arg1, -1, 0, 0, arg4->r, arg4->g, arg4->b, 0xFF);
    func_8001E680(&temp_s2[1], arg0 + arg2, arg1, -1, 0, 0, arg4->r, arg4->g, arg4->b, 0xFF);
    func_8001E680(&temp_s2[2], arg0, arg1 + arg3, -1, 0, 0, arg5->r, arg5->g, arg5->b, 0xFF);
    func_8001E680(&temp_s2[3], arg0 + arg2, arg1 + arg3, -1, 0, 0, arg5->r, arg5->g, arg5->b, 0xFF);

    gDPPipeSync(gDisplayListHead++);

    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
    gSPClearGeometryMode(gDisplayListHead++, G_ZBUFFER | G_LIGHTING);
    gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gSPPerspNormalize(gDisplayListHead++, 0xFFFF);

    gSPMatrix(gDisplayListHead++, (u32)sp7C & 0x1FFFFFFF, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, (u32)&D_8006F010 & 0x1FFFFFFF, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPVertex(gDisplayListHead++, (u32)temp_s2 & 0x1FFFFFFF, 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 2, 3, 0, 0, 3, 1, 0);
}

void func_80020460(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4) {
    gSPDisplayList(gDisplayListHead++, D_8006F518);

    if (Stage_IsHighRes() != 0) {
        func_8001F9D4(arg0, arg1, arg2, arg3);
    } else {
        func_8001FCAC(arg0, arg1, arg2, arg3);
    }

    if ((arg2 >= 0xF) && (arg3 >= 0xF)) {
        func_8001FE18(arg0, arg1, arg2, arg3, arg4);
    }

    gSPDisplayList(gDisplayListHead++, D_8006F630);
}

void func_8002053C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, Color_RGB8* arg4, Color_RGB8* arg5) {
    if ((arg2 >= 0xF) && (arg3 >= 0xF)) {
        if (Stage_IsHighRes() != 0) {
            func_800200B8(arg0 + 6, arg1 + 6, arg2 - 0xC, arg3 - 0xC, arg4, arg5);
        } else {
            func_800200B8(arg0 + 2, arg1 + 2, arg2 - 4, arg3 - 4, arg4, arg5);
        }
    }

    gSPDisplayList(gDisplayListHead++, D_8006F518);

    if (Stage_IsHighRes() != 0) {
        func_8001F9D4(arg0, arg1, arg2, arg3);
    } else {
        func_8001FCAC(arg0, arg1, arg2, arg3);
    }

    gSPDisplayList(gDisplayListHead++, D_8006F630);
}

void func_80020678(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4) {
    gSPDisplayList(gDisplayListHead++, D_8006F518);

    if (Stage_IsHighRes() != 0) {
        func_8001FB40(arg0, arg1, arg2, arg3);
    } else {
        func_8001FCAC(arg0, arg1, arg2, arg3);
    }

    if ((arg2 >= 0xF) && (arg3 >= 0xF)) {
        func_8001FF68(arg0, arg1, arg2, arg3, arg4);
    }

    gSPDisplayList(gDisplayListHead++, D_8006F630);
}

void func_80020754(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    gSPDisplayList(gDisplayListHead++, D_8006F518);

    if (Stage_IsHighRes() != 0) {
        func_8001F9D4(arg0, arg1, arg2, arg3);
    } else {
        func_8001FCAC(arg0, arg1, arg2, arg3);
    }

    gSPDisplayList(gDisplayListHead++, D_8006F630);
}

void func_800207FC(s32 arg0, s32 arg1) {
    if (Stage_IsHighRes() != 0) {
        gSPDisplayList(gDisplayListHead++, D_8006F4E0);
        func_8001C6AC(arg0, arg1, 0x20, 0x16, D_10031D0, 0x20, 0x200000);
    } else {
        gSPDisplayList(gDisplayListHead++, D_8006F5A0);
        func_8001D560(arg0, arg1, 0xC, 0xA, D_1003750, D_1003890, 0x10, 0x100000);
    }

    gSPDisplayList(gDisplayListHead++, D_8006F630);
}

void func_80020928(s16 arg0, s16 arg1) {
    static s16 D_8006F7D0 = 0;

    if (D_8006F7D0 < 6) {
        arg0 += D_8006F7D0;
    } else {
        arg0 = (arg0 - D_8006F7D0) + 0xC;
    }
    func_800207FC(arg0, arg1);
    D_8006F7D0 = (D_8006F7D0 + 1) % 12;
}

void func_800209B4(s32 arg0, s32 arg1, s32 arg2) {
    u8* var_t4;

    if (arg2 != 0) {
        var_t4 = D_2004320;
    } else {
        var_t4 = D_2004000;
    }

    gSPDisplayList(gDisplayListHead++, D_8006F4E0);
    gDPLoadTextureBlock(gDisplayListHead++, var_t4, G_IM_FMT_RGBA, G_IM_SIZ_16b, 20, 20, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gSPTextureRectangle(gDisplayListHead++, arg0 << 2, arg1 << 2, (arg0 + 0x13) << 2, (arg1 + 0x13) << 2,
                        G_TX_RENDERTILE, 0, 0, 0x1000, 0x0400);
    gSPDisplayList(gDisplayListHead++, D_8006F630);
}

void func_80020B88(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    u8* spDC;
    s32 spD8 = arg2 - 4;
    s32 temp_lo = ((arg4 - arg3) * (arg2 - 4)) / arg4;
    s32 sp1C = (arg2 - temp_lo) - 4;

    if ((arg3 * 0x30) < (arg4 * 0xA)) {
        spDC = D_1003CF0;
    } else if ((arg3 * 0x30) < (arg4 * 0x1B)) {
        spDC = D_1003C30;
    } else {
        spDC = D_1003B70;
    }

    gSPDisplayList(gDisplayListHead++, D_8006F4E0);

    gDPLoadTextureBlock(gDisplayListHead++, D_1003930, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 12, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPTextureRectangle(gDisplayListHead++, arg0 << 2, arg1 << 2, (arg0 + 1) << 2, (arg1 + 0xB) << 2, G_TX_RENDERTILE,
                        0, 0, 0x1000, 0x0400);

    gDPLoadTextureBlock(gDisplayListHead++, D_10039F0, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 12, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPTextureRectangle(gDisplayListHead++, ((arg0 + spD8) + 2) << 2, arg1 << 2, ((arg0 + spD8) + 3) << 2,
                        (arg1 + 0xB) << 2, G_TX_RENDERTILE, 0, 0, 0x1000, 0x0400);

    if (sp1C > 0) {
        gDPLoadTextureBlock(gDisplayListHead++, spDC, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 12, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        gSPTextureRectangle(gDisplayListHead++, (arg0 + 2) << 2, arg1 << 2, (arg0 + sp1C + 1) << 2, (arg1 + 0xB) << 2,
                            G_TX_RENDERTILE, 0, 0, 0, 0x0400);
    }

    if (temp_lo > 0) {
        gDPLoadTextureBlock(gDisplayListHead++, D_1003AB0, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 12, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        gSPTextureRectangle(gDisplayListHead++, ((arg0 + sp1C) + 2) << 2, arg1 << 2, ((arg0 + sp1C) + temp_lo + 1) << 2,
                            (arg1 + 0xB) << 2, G_TX_RENDERTILE, 0, 0, 0, 0x0400);
    }

    gSPDisplayList(gDisplayListHead++, D_8006F630);
}

void func_80021270(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8* spCC;
    s32 temp_lo = ((arg3 - arg2) * 0x30) / arg3;
    s32 sp14 = 0x30 - temp_lo;

    if ((arg2 * 0x30) < (arg3 * 0xA)) {
        spCC = D_1003EF0;
    } else if ((arg2 * 0x30) < (arg3 * 0x1B)) {
        spCC = D_1003EC8;
    } else {
        spCC = D_1003EA0;
    }

    gSPDisplayList(gDisplayListHead++, D_8006F518);

    gDPLoadTextureBlock(gDisplayListHead++, D_1003DB0, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 5, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPTextureRectangle(gDisplayListHead++, arg0 << 2, arg1 << 2, (arg0 + 0x10) << 2, (arg1 + 5) << 2, G_TX_RENDERTILE,
                        0, 0, 0x0400, 0x0400);
    gDPLoadTextureBlock(gDisplayListHead++, D_1003E50, G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, 5, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gSPTextureRectangle(gDisplayListHead++, (arg0 + 0x40) << 2, arg1 << 2, (arg0 + 0x41) << 2, (arg1 + 5) << 2,
                        G_TX_RENDERTILE, 0, 0, 0x0400, 0x0400);

    if (sp14 > 0) {
        gDPLoadTextureBlock(gDisplayListHead++, spCC, G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, 5, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        gSPTextureRectangle(gDisplayListHead++, (arg0 + 0x10) << 2, arg1 << 2, (arg0 + sp14 + 0x10) << 2,
                            (arg1 + 5) << 2, G_TX_RENDERTILE, 0, 0, 0, 0x0400);
    }

    if (temp_lo > 0) {
        gDPLoadTextureBlock(gDisplayListHead++, D_1003E78, G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, 5, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        gSPTextureRectangle(gDisplayListHead++, ((arg0 + sp14) + 0x10) << 2, arg1 << 2,
                            ((arg0 + sp14) + temp_lo + 0x10) << 2, (arg1 + 5) << 2, G_TX_RENDERTILE, 0, 0, 0, 0x0400);
    }

    gSPDisplayList(gDisplayListHead++, D_8006F630);
}

void func_80021920(void) {
    char** temp_s2 = func_8002D5AC(0x24);
    s32 i;

    for (i = 0; i < 151; i++) {
        HAL_Strcpy(D_8006FF00[i].name, func_8002D7C0(NULL, 0, temp_s2, i));
    }
}

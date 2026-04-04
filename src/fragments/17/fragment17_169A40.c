#include "fragment17.h"
#include "src/12D80.h"
#include "src/17300.h"
#include "src/geo_layout.h"
#include "src/19840.h"
#include "src/1C720.h"
#include "src/1CF30.h"
#include "src/20330.h"
#include "src/26820.h"
#include "src/3FB0.h"
#include "src/6A40.h"
#include "src/6BC0.h"
#include "src/DDC0.h"
#include "src/F420.h"
#include "src/controller.h"
#include "src/memory.h"
#include "src/stage_loader.h"

static u32 D_86B0E250[] = {
    0x0C00FFFF, 0x05000000, 0x0B00001E,  0x00000000, 0x014000F0, 0x00000032, 0x00000000, 0x00000118,
    0x05000000, 0x0D000000, 0x05000000,  0x14000000, 0x002B0012, 0xFFFFFF32, 0x16FFFFFF, 0x0F000003,
    0x05000000, 0x0A000000, &D_800AC840, 0x06000000, 0x06000000, 0x06000000, 0x06000000, 0x01000000,
};

static unk_D_86002F30* D_86B106B0;
static GraphNode* D_86B106B4;
static unk_D_86002F58_004_000 D_86B106B8;
static unk_D_86002F34_00C* D_86B10820;
static BinArchive* D_86B10824;
static u8* D_86B10828;
static u8 D_86B1082C;
static unk_func_80027FA0 D_86B10830;

void func_86B0A0B0(void) {
}

void func_86B0A0B8(void) {
    D_86B10820 = D_86B106B4->unk_0C;
    func_8000E88C(&D_86B10820->unk_60.at, 0.0f, 0.0f, 0.0f);
    func_8000E88C(&D_86B10820->unk_60.eye, 0.0f, 0.0f, 200.0f);
    D_86B10820->unk_24.near = 16.0f;
    D_86B10820->unk_24.far = 32768.0f;
    D_86B10820->unk_24.fovy = 45.0f;
}

void func_86B0A15C(void) {
    Cont_StartReadInputs();
    Cont_ReadInputs();
    func_8001F730();
}

void func_86B0A18C(u8* arg0) {
    s32 i;
    s32 j;

    if (1) {}

    gDPPipeSync(gDisplayListHead++);

    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetTexturePersp(gDisplayListHead++, G_TP_NONE);
    gDPSetCombineMode(gDisplayListHead++, G_CC_DECALRGB, G_CC_DECALRGB);

    gDPPipeSync(gDisplayListHead++);

    for (i = 0; i < 15; i++) {
        for (j = 0; j < 20; j++, arg0 += 0x200) {
            func_8001C6AC(j * 0x10, i * 0x10, 0x10, 0x10, arg0, 0x10, 0);
        }
    }

    gSPDisplayList(gDisplayListHead++, D_8006F630);
}

void func_86B0A30C(void) {
    Stage_ActivateFramebuffer();
    func_86B0A18C(D_86B10828);
    GFX_ClearDepth(&gDisplayListHead, 0, 0, 0x140, 0xF0);
    func_80015348();
    func_80015094(D_86B106B4);
    func_86B0A0B0();
    Stage_AdvanceFrame();
}

s32 func_86B0A37C(void) {
    s32 var_v0;

    if ((D_86B106B8.unk_040.unk_08 >> 0x10) >= 0x5F) {
        func_8001BE34(&D_86B106B8, 0, 0, 0, D_86B1082C += 0x14);
    }

    if ((D_86B106B8.unk_040.unk_08 >> 0x10) == 0x64) {
        Stage_SetFillColor(1);
        Stage_FadeIn(3);
    }

    var_v0 = func_800174E4(&D_86B106B8) != 0;
    if (var_v0) {
        return 1;
    }
}

void func_86B0A414(void) {
    s32 var_s0 = 1;

    Stage_FadeOut(0xC);
    Audio_PlayTrack(0x30);

    while (var_s0 != 0) {
        func_86B0A15C();
        if (func_86B0A37C() != 0) {
            var_s0 = 0;
        }
        func_86B0A30C();
    }

    Stage_AdvanceFrames(2);
}

void func_86B0A47C(void) {
    MemoryBlock* sp1C = func_80002D10(main_pool_get_available(), 0);

    D_86B106B4 = process_geo_layout(sp1C, &D_86B0E250);

    func_80002D60(sp1C);
    func_8001BB20();
    func_86B0A0B8();
    func_8001BB58(&D_86B106B8);
    func_8001987C();

    D_86B106B0 = func_80019D18(0xD6);
    func_8001BC34(&D_86B106B8, 0, 0xD6, D_86B106B0->unk_08->unk_00[0]);
    func_8001BD04(&D_86B106B8, 1);
    func_8000E88C(&D_86B106B8.unk_024, 0.0f, 0.0f, 0.0f);
    D_86B1082C = 0;
}

s32 func_86B0A554(void) {
    RenderContext* sp24;

    main_pool_push_state('MTWO');

    DLBuf_Init(0x10000, 0);
    sp24 = Stage_CreateRenderContext(0, 1, 3, 1, 2, 1);
    func_8001E94C(6, 0);

    ASSET_LOAD(D_1000000, common_menu1_ui, 0);
    FRAGMENT_LOAD(fragment31);

    func_80028AFC(2);
    func_80027FA0(&D_86B10830, D_800AE540.unk_11F2);

    if (D_800AE540.unk_11F2 == 1) {
        D_86B10824 = ASSET_LOAD2(backgrounds, 1, 1);
        D_86B10828 = func_8000484C(D_86B10824, 2);
    } else if (D_86B10830.unk_00 & 0x100) {
        D_86B10824 = ASSET_LOAD2(backgrounds, 1, 1);
        D_86B10828 = func_8000484C(D_86B10824, 1);
    } else {
        D_86B10824 = ASSET_LOAD2(backgrounds, 1, 1);
        D_86B10828 = func_8000484C(D_86B10824, 2);
    }

    func_86B0A47C();
    Stage_SetRenderContext(sp24);
    func_86B0A414();
    Stage_FreeRenderContext();
    func_8001E9CC();
    DLBuf_Free();

    main_pool_pop_state('MTWO');

    return 0;
}

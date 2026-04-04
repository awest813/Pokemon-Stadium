#include "fragment53.h"
#include "src/19840.h"
#include "src/1AB70.h"
#include "src/232C0.h"
#include "src/26820.h"
#include "src/3FB0.h"
#include "src/6A40.h"
#include "src/6BC0.h"
#include "src/controller.h"
#include "src/memory.h"
#include "src/stage_loader.h"

static unk_D_86002F58_004_000_010* D_82A00370;
static unk_func_8001A024* D_82A00374;
static unk_func_80026268_arg0 D_82A00378[6];

static s32 D_82A00360 = 0;

void func_82A00020(s32 arg0) {
    s32 i;
    unk_func_80022C28_ret* temp_v0;

    if (func_80023AF8(arg0) != 0) {
        temp_v0 = func_80022C28(0x20, arg0, 0, 0);
        D_82A00360 = func_80022E18(D_82A00378, 6, temp_v0);
        func_80022D8C(temp_v0);
    }

    for (i = 0; i < D_82A00360; i++) {
        func_8001A46C(D_82A00374, i, &D_82A00378[i], 1);
    }
}

s32 func_82A000F0(u8 arg0) {
    Cont_StartReadInputs();
    Cont_ReadInputs();
    func_8001ABAC(D_82A00374, 0);
    Stage_ActivateFramebuffer();
    GFX_ClearScreen(&gDisplayListHead, 1);
    return 0;
}

void func_82A00144(GBTowerState* arg0) {
    s32 i;
    unk_GameState_GBTower sp34;
    u16(*var_s1)[0x640] = arg0->unk_04[0];

    func_80027F24(&sp34);
    arg0->unk_02 = sp34.unk_00;
    arg0->unk_00 = sp34.unk_04;
    arg0->unk_03 = D_82A00360;

    for (i = 0; i < D_82A00360; i++, var_s1++) {
        _bcopy(func_8001AFD8(D_82A00374, i), var_s1, sizeof(var_s1[0]));
    }

    if (D_82A00360 < 6) {
        bzero(var_s1, 0x4B00 + (-D_82A00360 * 0xC80));
    }
}

s32 func_82A00224(s32 arg0, GBTowerState* arg1) {
    RenderContext* sp24;

    main_pool_push_state('PREP');

    DLBuf_Init(0x10000, 0);
    sp24 = Stage_CreateRenderContext(0, 1, 2, 0, 1, 1);

    ASSET_LOAD(D_1000000, common_menu1_ui, 0);
    FRAGMENT_LOAD(fragment31);

    D_82A00370 = func_80019760(1);
    D_82A00374 = func_8001A024(D_82A00370, 6, 0x28, 0x28);
    func_8001987C();
    func_82A00020(arg0);
    func_80028AFC(2);
    Stage_SetRenderContext(sp24);
    Stage_RunLoop(8, func_82A000F0);
    Stage_FreeRenderContext();
    func_82A00144(arg1);
    DLBuf_Free();

    main_pool_pop_state('PREP');

    return D_82A00360;
}

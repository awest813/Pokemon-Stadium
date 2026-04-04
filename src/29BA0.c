#include "29BA0.h"
#include "include/functions.h"
#include "src/20330.h"
#include "src/26820.h"
#include "src/373A0.h"
#include "src/30640.h"
#include "src/22630.h"
#include "src/3FB0.h"
#include "src/6A40.h"
#include "src/DDC0.h"
#include "src/2C1C0.h"
#include "src/26820.h"
#include "src/2FEA0.h"
#include "src/2E460.h"
#include "src/E1C0.h"
#include "src/E890.h"
#include "src/19840.h"
#include "src/232C0.h"
#include "src/controller.h"
#include "src/dp_intro.h"
#include "src/gb_tower.h"
#include "src/memory.h"
#include "src/rsp.h"
#include "src/stage_loader.h"
#include "lib/ultralib/include/PR/os_internal_reg.h"

static s32 pad_D_80075660 = 0;
static s32 pad_D_80075664 = 0;
s32 gCurrentGameState = STATE_N64_LOGO_INTRO;
static s32 gLastGameState = STATE_N64_LOGO_INTRO;
static s32 pad_D_80075670 = 0;
static s16 D_80075674 = 0;
static s16 D_80075678 = 0;
static s16 D_8007567C = 0;

DeferredFragment gDeferredFragment;
s32 pad_D_800AE534[2];
unk_D_800AE540 D_800AE540;
BinArchive* D_800AF738;

s32 Game_DoCopyProtection(s32 state) {
    u32 intr = osSetIntMask(1);

    // if the 16-bit value at ROM offset 0xE38 is not 0x828A, make the state an invalid one.
    if (!(HW_REG(PI_STATUS_REG, u32) & 3) && ((*(u32*)0xB0000E38 & 0xFFFF) != 0x828A)) {
        state = -0x10;
    }

    osSetIntMask(intr);

    return state;
}

s32 Fragment_LoadAndCall(s32 arg0, u8* romStart, u8* romEnd, u32 arg3, u32 arg4) {
    s32 result;
    FragmentEntry func;

    func = func_80004454(arg0, romStart, romEnd);

    result = func(arg3, arg4);

    main_pool_try_free(func);

    return result;
}

void Fragment_DeferAndCall(s32 arg0, u8* arg1, u8* arg2, s32 arg3, s32 arg4) {
    gDeferredFragment.unk_00 = 1;
    gDeferredFragment.fragment_id = arg0;
    gDeferredFragment.rom_start = arg1;
    gDeferredFragment.rom_end = arg2;
    gDeferredFragment.arg0 = arg3;
    gDeferredFragment.arg1 = arg4;
}

unk_D_800AE540_0004* Game_GetPlayers(void) {
    return D_800AE540.unk_0004;
}

s32 Game_GetSelectedPlayer(void) {
    return D_80075674;
}

void Game_SetSelectedPlayer(s32 arg0) {
    if ((arg0 >= 0) && (arg0 < 4)) {
        D_80075674 = arg0;
        D_80075678 = 1;
    }
}

void Game_PollControllers(void) {
    Cont_StartReadInputs();
    Cont_ReadInputs();
    func_8001F730();
}

s32 Game_RunDemo(s16 arg0) {
    s32 sp24;

    main_pool_push_state('Demo');

    FRAGMENT_LOAD(fragment34);
    FRAGMENT_LOAD(fragment2);
    FRAGMENT_LOAD(fragment3);

    D_800AE540.unk_0000 = arg0;

    sp24 = FRAGMENT_LOAD_AND_CALL(fragment17, arg0, 0);

    main_pool_pop_state('Demo');

    return sp24;
}

void Game_RunIntroSequence(void) {
    main_pool_push_state('demo');

    FRAGMENT_LOAD(fragment34);
    FRAGMENT_LOAD(fragment2);
    FRAGMENT_LOAD(fragment3);

    if (D_800AE540.unk_11F2 != 0) {
        FRAGMENT_LOAD_AND_CALL(fragment16, 0, 0);
    } else {
        FRAGMENT_LOAD_AND_CALL(fragment16, 0, 1);
    }

    main_pool_pop_state('demo');
}

void GameState_Intro(void) {
    FRAGMENT_LOAD_AND_CALL(fragment35, 0, 0);

    main_pool_push_state('TITL');

    DLBuf_Init(0x10000, 0);
    Stage_SetRenderContext(Stage_CreateRenderContext(0, 1, 3, 1, 2, 1));

    if (Game_RunDemo(0x12) == 2) {
        Stage_SetFadeMode(1);
        Stage_AdvanceFrames(2);
    }

    gCurrentGameState = STATE_TITLE_SCREEN;
}

void GameState_TitleScreen(void) {
    gCurrentGameState = FRAGMENT_LOAD_AND_CALL(fragment36, 0, &D_8007567C);

    if (gCurrentGameState == STATE_TITLE_SCREEN) {
        main_pool_push_state('DEMO');

        if (D_800AE540.unk_0000 == 0x10) {
            FRAGMENT_LOAD(fragment34);
            FRAGMENT_LOAD_AND_CALL(fragment62, 0, &D_800AE540);
        } else {
            ASSET_LOAD(D_3000000, kids_club_game_ui, 0); // borrows the Stadium logo from it
            FRAGMENT_LOAD(fragment2);
            FRAGMENT_LOAD(fragment34);

            switch (D_800AE540.unk_0003) {
                case 3:
                    FRAGMENT_LOAD(fragment4);
                    FRAGMENT_LOAD_AND_CALL(fragment8, 1, 0);
                    break;

                case 9:
                    FRAGMENT_LOAD(fragment3);
                    FRAGMENT_LOAD_AND_CALL(fragment14, 1, 0);
                    break;

                case 13:
                    FRAGMENT_LOAD(fragment3);
                    FRAGMENT_LOAD_AND_CALL(fragment18, 1, 0);
                    break;
            }
        }

        main_pool_pop_state('DEMO');
        return;
    }

    D_800AE540.unk_0002 = 0;
    D_800AE540.unk_11F6 = 0;
    D_80075674 = -1;
}

void GameState_N64DDBoot(void) {
    Audio_WaitDone(1);
    Display_FlushFrames(2);
    LeoBootGame(D_800AA680.unk_08);
}

void GameState_Debug(void) {
    s32 temp_v0;
    RenderContext* temp_s0;

    main_pool_push_state('DBUG');

    DLBuf_Init(0x10000, 0);
    temp_s0 = Stage_CreateRenderContext(1, 1, 2, 0, 2, 1);

    FRAGMENT_LOAD(fragment34);
    Stage_SetRenderContext(temp_s0);
    FRAGMENT_LOAD_AND_CALL2(fragment67, 1, 0);

    while ((gCurrentGameState == STATE_STUBBED_DEBUG) && (gDeferredFragment.unk_00 != 0)) {
        gDeferredFragment.unk_00 = 0;
        gCurrentGameState = Fragment_LoadAndCall(gDeferredFragment.fragment_id, gDeferredFragment.rom_start, gDeferredFragment.rom_end,
                                          gDeferredFragment.arg0, gDeferredFragment.arg1);
    }

    Stage_FreeRenderContext();
    DLBuf_Free();

    main_pool_pop_state('DBUG');
}

void GameState_AreaSelect(void) {
    gCurrentGameState = FRAGMENT_LOAD_AND_CALL(fragment37, 0, 0);
    D_80075674 = -1;
}

void GameState_EventBattle(void) {
    gCurrentGameState = FRAGMENT_LOAD_AND_CALL(fragment38, 0, 0);
}

void GameState_Options(void) {
    gCurrentGameState = FRAGMENT_LOAD_AND_CALL(fragment56, 0, 0);
}

void GameState_MenuSelect(void) {
    s32 from_title_screen = gLastGameState == STATE_TITLE_SCREEN;

    gCurrentGameState = FRAGMENT_LOAD_AND_CALL(fragment57, 0, from_title_screen);
}

s16 Stadium_RunCupSelect(s16 arg0) {
    main_pool_push_state('STAD');

    DLBuf_Init(0x18000, 0);
    Stage_SetRenderContext(Stage_CreateRenderContext(1, 1, 2, 0, 2, 1));

    while ((arg0 > 0) && (arg0 < 5)) {
        switch (arg0) {
            case 1:
                if (FRAGMENT_LOAD_AND_CALL(fragment59, 0, 0) != 0) {
                    arg0 = 2;
                } else {
                    arg0 = 0;
                }
                break;

            case 2:
                if (FRAGMENT_LOAD_AND_CALL(fragment60, 0, 0) != 0) {
                    arg0 = 3;
                } else {
                    arg0 = 1;
                }
                break;

            case 3:
                if (FRAGMENT_LOAD_AND_CALL(fragment54, 0, 0) != 0) {
                    arg0 = 4;
                } else {
                    arg0 = 1;
                }
                break;

            case 4:
                if (FRAGMENT_LOAD_AND_CALL(fragment61, 0, 0) != 0) {
                    arg0 = 5;
                } else {
                    arg0 = 3;
                }
                break;
        }
    }

    Stage_FreeRenderContext();
    DLBuf_Free();

    main_pool_pop_state('STAD');

    return arg0;
}

void GameState_Stadium(void) {
    s16 sp4E = 1;

    while ((sp4E > 0) && (sp4E < 5)) {
        if (D_800AE540.unk_11F5 & 1) {
            sp4E = 5;
        } else {
            sp4E = Stadium_RunCupSelect(sp4E);
        }

        if (sp4E == 5) {
            D_800AE540.unk_11F6 = 0;
            FRAGMENT_LOAD_AND_CALL(fragment63, 1, 0);

            while (!(D_800AE540.unk_11F6 & 1)) {
                FRAGMENT_LOAD_AND_CALL(fragment64, 0, 0);

                main_pool_push_state('STAD');

                FRAGMENT_LOAD(fragment34);
                FRAGMENT_LOAD_AND_CALL(fragment62, 0, &D_800AE540);

                main_pool_pop_state('STAD');

                FRAGMENT_LOAD_AND_CALL(fragment63, 2, 0);
            }

            if (D_800AE540.unk_11F6 & 0x20) {
                sp4E = 4;
            }

            if (D_800AE540.unk_11F6 & 0x800) {
                sp4E = 4;
            }

            if (D_800AE540.unk_11F6 & 2) {
                Game_RunDemo(0x11);
            }

            if (D_800AE540.unk_11F6 & 0x1000) {
                FRAGMENT_LOAD_AND_CALL(fragment66, 0, 0);
            }

            if (D_800AE540.unk_11F6 & 8) {
                Game_RunDemo(0x16);
            }

            if (D_800AE540.unk_11F6 & 0x10) {
                Game_RunDemo(0x17);
            }
        }
    }

    if (D_800AE540.unk_11F6 & 0x40) {
        gCurrentGameState = STATE_TITLE_SCREEN;
    } else {
        gCurrentGameState = STATE_AREA_SELECT;
    }
}

s32 FreeBattle_RunLoop(s16 arg0) {
    main_pool_push_state('FREE');

    DLBuf_Init(0x18000, 0);
    Stage_SetRenderContext(Stage_CreateRenderContext(1, 1, 2, 0, 2, 1));

    while ((arg0 > 0) && (arg0 < 4)) {
        switch (arg0) {
            case 1:
                if (FRAGMENT_LOAD_AND_CALL(fragment58, 0, 0) != 0) {
                    arg0 = 2;
                } else {
                    arg0 = 0;
                }
                break;

            case 2:
                if (FRAGMENT_LOAD_AND_CALL(fragment54, 0, 0) != 0) {
                    arg0 = 3;
                } else {
                    arg0 = 1;
                }
                break;

            case 3:
                if (FRAGMENT_LOAD_AND_CALL(fragment61, 0, 1) != 0) {
                    arg0 = 4;
                } else {
                    arg0 = 2;
                }
                break;
        }
    }

    Stage_FreeRenderContext();
    DLBuf_Free();
    main_pool_pop_state('FREE');

    return arg0;
}

void GameState_FreeBattle(void) {
    s16 sp4E = 1;

    while ((sp4E > 0) && (sp4E < 4)) {
        sp4E = FreeBattle_RunLoop(sp4E);

        switch (sp4E) {
            case 4:
                D_800AE540.unk_11F6 = 0;
                FRAGMENT_LOAD_AND_CALL(fragment63, 0, 0);

                while (!(D_800AE540.unk_11F6 & 1)) {
                    FRAGMENT_LOAD_AND_CALL(fragment64, 0, 0);

                    main_pool_push_state('FREE');

                    FRAGMENT_LOAD(fragment34);
                    FRAGMENT_LOAD_AND_CALL(fragment62, 0, &D_800AE540);

                    main_pool_pop_state('FREE');

                    FRAGMENT_LOAD_AND_CALL(fragment63, 2, 0);
                }

                if (D_800AE540.unk_11F6 & 0x20) {
                    sp4E = 3;
                }

                if (D_800AE540.unk_11F6 & 0x80) {
                    sp4E = 1;
                }
                break;
        }
    }

    gCurrentGameState = STATE_AREA_SELECT;
}

s16 VsMewtwo_RunLoop(s16 arg0) {
    main_pool_push_state('STAD');

    DLBuf_Init(0x18000, 0);
    Stage_SetRenderContext(Stage_CreateRenderContext(1, 1, 2, 0, 2, 1));

    while ((arg0 > 0) && (arg0 < 3)) {
        switch (arg0) {
            case 1:
                if (FRAGMENT_LOAD_AND_CALL(fragment54, 0, 0) != 0) {
                    arg0 = 2;
                } else {
                    arg0 = 0;
                }
                break;

            case 2:
                if (FRAGMENT_LOAD_AND_CALL(fragment61, 0, 0) != 0) {
                    arg0 = 3;
                } else {
                    arg0 = 1;
                }
                break;
        }
    }

    Stage_FreeRenderContext();
    DLBuf_Free();

    main_pool_pop_state('STAD');
    return arg0;
}

void GameState_VsMewtwo(void) {
    s16 sp4E = 1;

    Game_RunDemo(0x18);
    func_8002B5EC(8, 8, 0);
    func_8002B840(0, func_8002B700(0, 0, "1P", func_8002311C(1)));
    func_8002B840(1, func_8002B700(-1, 0, "COM", func_8002311C(3)));

    while ((sp4E > 0) && (sp4E < 3)) {
        sp4E = VsMewtwo_RunLoop(sp4E);

        switch (sp4E) {
            case 3:
                D_800AE540.unk_11F6 = 0;
                FRAGMENT_LOAD_AND_CALL(fragment63, 0, 0);

                while (!(D_800AE540.unk_11F6 & 1)) {
                    FRAGMENT_LOAD_AND_CALL(fragment64, 0, 0);

                    main_pool_push_state('STAD');

                    FRAGMENT_LOAD(fragment34);
                    FRAGMENT_LOAD_AND_CALL(fragment62, 0, &D_800AE540);

                    main_pool_pop_state('STAD');

                    FRAGMENT_LOAD_AND_CALL(fragment63, 2, 0);
                }

                if (D_800AE540.unk_11F6 & 0x20) {
                    sp4E = 2;
                }

                if (D_800AE540.unk_11F6 & 4) {
                    Game_RunIntroSequence();
                    gCurrentGameState = STATE_N64_LOGO_INTRO;
                    return;
                }
                break;
        }
    }

    gCurrentGameState = STATE_AREA_SELECT;
}

void GameState_KidsClub(void) {
    func_8002F2A0();
    gCurrentGameState = STATE_AREA_SELECT;
}

void GameState_Gallery(void) {
    func_8002FA40();
    gCurrentGameState = STATE_MENU_SELECT;
}

void GameState_VictoryPalace(void) {
    if (FRAGMENT_LOAD_AND_CALL(fragment50, 0, 0) == 1) {
        Game_RunDemo(0x1A);
    }
    gCurrentGameState = STATE_AREA_SELECT;
}

void GameState_GBTower(void) {
    s32 sp34;
    GBTowerState sp2C;
    unk_GameState_GBTower sp24;

    main_pool_push_state('EMU0');

    if ((FRAGMENT_LOAD_AND_CALL(fragment33, 0, 0) == 0) && (D_80075674 >= 0) && (D_80075674 < 4)) {
        Audio_StopTrack(4);
        Audio_WaitDone(1);
        Display_FlushFrames(2);
        Audio_Disable();

        sp2C.unk_04 = main_pool_alloc(sizeof(*sp2C.unk_04), 0);

        FRAGMENT_LOAD_AND_CALL(fragment53, D_80075674, &sp2C);
        GBTower_Start(&sp2C);

        sp34 = GBTower_WaitResult();

        GBTower_Teardown(&sp2C);
        Display_FlushFrames(2);
        Audio_Enable();

        if (sp34 != 0) {
            Game_PostBattle(D_80075674, sp34);
        }

        func_80024040(D_80075674);
        func_80028AFC(2);
        func_80027F24(&sp24);

        sp24.unk_04 = sp2C.unk_00;

        func_80027F40(&sp24);
        func_80026684(0x14, 0);
        func_800284B4(2);
    }

    main_pool_pop_state('EMU0');

    gCurrentGameState = STATE_AREA_SELECT;
}

s16 GymLeaderCastle_RunLoop(s16 arg0, s16 arg1) {
    main_pool_push_state('STAD');

    DLBuf_Init(0x18000, 0);
    Stage_SetRenderContext(Stage_CreateRenderContext(1, 1, 2, 0, 2, 1));

    while ((arg0 > 0) && (arg0 < 4)) {
        switch (arg0) {
            case 1:
                if (FRAGMENT_LOAD_AND_CALL(fragment54, 0, 0) != 0) {
                    arg0 = 2;
                } else {
                    arg0 = 0;
                }
                break;

            case 2:
                if (FRAGMENT_LOAD_AND_CALL(fragment65, arg1, 0) != 0) {
                    arg0 = 3;
                } else {
                    arg0 = 1;
                }
                break;

            case 3:
                if (FRAGMENT_LOAD_AND_CALL(fragment61, 0, 0) != 0) {
                    arg0 = 4;
                } else {
                    arg0 = 2;
                }
                break;
        }

        arg1 = 0;
    }

    Stage_FreeRenderContext();
    DLBuf_Free();

    main_pool_pop_state('STAD');

    return arg0;
}

void GameState_GymLeaderCastle(void) {
    s16 sp4E = 1;
    s16 sp4C = 0;

    if (!(D_800AE540.unk_11F5 & 1)) {
        func_8002B5EC(7, 7, 0);
        func_8002B840(0, func_8002B700(0, 0, "1P", func_8002311C(1)));
        func_8002B840(1, func_8002B700(-1, 0, "COM", func_8002311C(3)));
    }

    while ((sp4E > 0) && (sp4E < 4)) {
        if (D_800AE540.unk_11F5 & 1) {
            sp4E = 4;
        } else {
            sp4E = GymLeaderCastle_RunLoop(sp4E, sp4C);
        }

        if (sp4E == 4) {
            sp4C = 0;
            D_800AE540.unk_11F6 = 0;

            FRAGMENT_LOAD_AND_CALL(fragment63, 1, 0);

            while (!(D_800AE540.unk_11F6 & 1)) {
                FRAGMENT_LOAD_AND_CALL(fragment64, 0, 0);

                main_pool_push_state('STAD');

                FRAGMENT_LOAD(fragment34);
                FRAGMENT_LOAD_AND_CALL(fragment62, 0, &D_800AE540);

                main_pool_pop_state('STAD');

                FRAGMENT_LOAD_AND_CALL(fragment63, 2, 0);
            }

            if (D_800AE540.unk_11F6 & 0x200) {
                FRAGMENT_LOAD_AND_CALL(fragment40, 0, 0);
                Game_RunDemo(0x11);
                Game_RunDemo(0x15);
            }

            if (D_800AE540.unk_11F6 & 0x20) {
                sp4E = 3;
            }

            if (D_800AE540.unk_11F6 & 0x800) {
                sp4E = 3;
            }

            if (D_800AE540.unk_11F6 & 0x400) {
                sp4C = 1;
            }

            if (D_800AE540.unk_11F6 & 0x100) {
                sp4E = 2;
            }
        }
    }

    if (D_800AE540.unk_11F6 & 0x40) {
        gCurrentGameState = STATE_TITLE_SCREEN;
    } else {
        gCurrentGameState = STATE_AREA_SELECT;
    }
}

void GameState_BattleNow(s32 arg0) {
    main_pool_push_state('QUIK');
    func_8002C394(arg0);

    FRAGMENT_LOAD_AND_CALL(fragment63, 0, 0);
    FRAGMENT_LOAD_AND_CALL(fragment64, 0, 0);

    main_pool_push_state('FIGT');

    FRAGMENT_LOAD(fragment34);
    FRAGMENT_LOAD_AND_CALL(fragment62, 0, &D_800AE540);

    main_pool_pop_state('FIGT');

    FRAGMENT_LOAD_AND_CALL(fragment63, 2, 0);

    main_pool_pop_state('QUIK');

    gCurrentGameState = STATE_MENU_SELECT;
}

void GameState_BattleFromEvent(void) {
    main_pool_push_state('EVNT');

    FRAGMENT_LOAD_AND_CALL(fragment63, 0, 0);
    FRAGMENT_LOAD_AND_CALL(fragment64, 0, 0);

    main_pool_push_state('FIGT');

    FRAGMENT_LOAD(fragment34);
    FRAGMENT_LOAD_AND_CALL(fragment62, 0, &D_800AE540);

    main_pool_pop_state('FIGT');

    FRAGMENT_LOAD_AND_CALL(fragment63, 2, 0);

    main_pool_pop_state('EVNT');

    gCurrentGameState = STATE_EVENT_BATTLE;
}

void GameState_FastIntro(void) {
    void (*frag_43_entry)(void);

    FRAGMENT_LOAD_AND_CALL(fragment35, 0, 0);

    main_pool_push_state('DOSE');

    frag_43_entry = FRAGMENT_LOAD(fragment43);
    frag_43_entry();

    FRAGMENT_LOAD(fragment34);
    FRAGMENT_LOAD_AND_CALL(fragment75, 0, 0);

    gCurrentGameState = STATE_TITLE_SCREEN;

    main_pool_pop_state('DOSE');
}

void GameState_FastBattle(void) {
    main_pool_push_state('STAD');

    func_8002BC64(1, 0, 2, 0);
    func_800367A0(0xA0, 0xA, 0);

    FRAGMENT_LOAD(fragment34);
    FRAGMENT_LOAD_AND_CALL(fragment62, 0, &D_800AE540);

    main_pool_pop_state('STAD');

    gCurrentGameState = STATE_TITLE_SCREEN;
}

void func_8002B244(void) {
}

void GameState_KidsClubTitle(void) {
    func_8002F2A0();
    gCurrentGameState = STATE_TITLE_SCREEN;
}

void Game_PostBattle(s32 arg0, s32 arg1) {
    osViBlack(1);
    func_8000A924();
    Audio_StopAll();
    Game_OnPreNMI();
    Display_SetBorderColor(1);
    VI_RestoreMode();
    Display_FlushFrames(0xA);

    main_pool_pop_state('GAME');

    FRAGMENT_LOAD_AND_CALL(fragment32, arg0, arg1);
}

void Game_OnPreNMI(void) {
    func_80000DF4();
}

void Game_Thread(UNUSED LEODiskID* arg) {
    s32 state;

    __osSetFpcCsr(0x01000C01U);

    Cont_SetupControllers();
    func_800196DC();

    main_pool_push_state('GAME');

    D_800AE540.unk_11F2 = 0;

    // is this game connected to a Sticker Station?
    if (func_8000B4C4() != 0) {
        gCurrentGameState = STATE_FAST_N64_LOGO;
    }

    while (1) {
        state = Game_DoCopyProtection(gCurrentGameState);

        switch (state) {
            case STATE_N64_LOGO_INTRO:
                GameState_Intro();
                break;
            case STATE_TITLE_SCREEN:
                GameState_TitleScreen();
                break;
            case STATE_N64DD_BOOT_UNUSED:
                GameState_N64DDBoot();
                break;
            case STATE_AREA_SELECT:
                GameState_AreaSelect();
                break;
            case STATE_GALLERY:
                GameState_Gallery();
                break;
            case STATE_EVENT_BATTLE:
                GameState_EventBattle();
                break;
            case STATE_OPTIONS:
                GameState_Options();
                break;
            case STATE_MENU_SELECT:
                GameState_MenuSelect();
                break;
            case STATE_STADIUM_MENU:
                GameState_Stadium();
                break;
            case STATE_FREE_BATTLE:
                GameState_FreeBattle();
                break;
            case STATE_VS_MEWTWO:
                GameState_VsMewtwo();
                break;
            case STATE_KIDS_CLUB:
                GameState_KidsClub();
                break;
            case STATE_VICTORY_PALACE:
                GameState_VictoryPalace();
                break;
            case STATE_POKEMON_LAB:
                func_8002EF44();
                break;
            case STATE_GB_TOWER:
                GameState_GBTower();
                break;
            case STATE_GYM_LEADER_CASTLE:
                GameState_GymLeaderCastle();
                break;
            case STATE_BATTLE_NOW_1P:
                GameState_BattleNow(1);
                break;
            case STATE_BATTLE_NOW_2P:
                GameState_BattleNow(2);
                break;
            case STATE_BATTLE_FROM_EVENT:
                GameState_BattleFromEvent();
                break;
            case STATE_STUBBED_DEBUG:
                GameState_Debug();
                break;
            case STATE_FAST_BATTLE:
                GameState_FastBattle();
                break;
            case STATE_KIDS_CLUB_TITLE:
                GameState_KidsClubTitle();
                break;
            case STATE_FAST_N64_LOGO:
                GameState_FastIntro();
                break;
            // same as state 1: N64 Logo + Intro.
            default:
                GameState_Intro();
                break;
        }

        gLastGameState = state;
    }
}

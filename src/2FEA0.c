#include "2FEA0.h"
#include "src/29BA0.h"
#include "src/3FB0.h"
#include "src/memory.h"

/*
 * KidsClub_Dispatch
 * Original symbol: func_8002F2A0
 * Status:
 *     CONTROL_FLOW_MAPPED
 *     GLOBAL_STATE_HARDENED
 *
 * Summary:
 *     The primary dispatcher and lifecycle manager for Kids Club minigames.
 *
 * Verified:
 *     - Coordinates 'KDCM' and 'mini' memory pool states.
 *     - Dispatches to minigame fragments 6-21 based on selection ID.
 *     - Manages shared UI (kids_club_game_ui) and support fragment (3, 4, 5) loading.
 *     - Interprets minigame return codes (var_s0) to handle loop/exit flow.
 */
void KidsClub_Dispatch(void) {
    u32 var_s0;
    u32 sp60;
    void (*func)(void);
    s32 sp58;
    UNUSED s32 pad[2];

    main_pool_push_state('KDCM');

    sp60 = 0;
    var_s0 = 0;
    sp58 = 1;

    func = FRAGMENT_LOAD(fragment2);
    func();

    while (sp58 != 0) {
        if ((var_s0 == 0) || (var_s0 == 1) || (var_s0 == 4)) {
            sp60 = FRAGMENT_LOAD_AND_CALL(fragment39, var_s0, sp60);
        }

        if (sp60 != 0) {
            main_pool_push_state('mini');

            ASSET_LOAD(D_3000000, kids_club_game_ui, 0);
            FRAGMENT_LOAD(fragment34);

            switch (sp60) {
                case 1: // magikarp minigame
                    FRAGMENT_LOAD(fragment4);
                    var_s0 = FRAGMENT_LOAD_AND_CALL(fragment6, MagikarpMinigame_Entry, 0, 0);
                    break;
                case 2: // clefairy minigame
                    FRAGMENT_LOAD(fragment4);
                    var_s0 = FRAGMENT_LOAD_AND_CALL(fragment7, ClefairyMinigame_Entry, 0, 0);
                    break;
                case 3: // rattata minigame
                    FRAGMENT_LOAD(fragment4);
                    var_s0 = FRAGMENT_LOAD_AND_CALL(fragment8, RattataMinigame_Entry, 0, 0);
                    break;
                case 4: // unfinished jigglypuff minigame
                    FRAGMENT_LOAD(fragment4);
                    var_s0 = FRAGMENT_LOAD_AND_CALL(fragment9, JigglypuffMinigame_Entry, 0, 0);
                    break;
                case 5: // drowzee minigame
                    FRAGMENT_LOAD(fragment5);
                    var_s0 = FRAGMENT_LOAD_AND_CALL(fragment10, DrowzeeMinigame_Entry, 0, 0);
                    break;
                case 6: // pikachu minigame
                    FRAGMENT_LOAD(fragment5);
                    var_s0 = FRAGMENT_LOAD_AND_CALL(fragment11, PikachuMinigame_Entry, 0, 0);
                    break;
                case 7: // "minigame 7 sample" model viewer. debug placeholder?
                    FRAGMENT_LOAD(fragment5);
                    var_s0 = FRAGMENT_LOAD_AND_CALL(fragment12, SampleMinigame7_Entry, 0, 0);
                    break;
                case 8: // "minigame 8 sample" model viewer. debug placeholder?
                    FRAGMENT_LOAD(fragment5);
                    var_s0 = FRAGMENT_LOAD_AND_CALL(fragment13, SampleMinigame8_Entry, 0, 0);
                    break;
                case 9: // sushi go round
                    FRAGMENT_LOAD(fragment3);
                    var_s0 = FRAGMENT_LOAD_AND_CALL(fragment14, SushiGoRoundMinigame_Entry, 0, 0);
                    break;
                case 10: // seems to be the "snap mode" code. However, the asset UI isnt loaded, so this call will
                         // crash.
                    FRAGMENT_LOAD(fragment3);
                    var_s0 = FRAGMENT_LOAD_AND_CALL(fragment15, SnapMode_Entry, 0, 0);
                    break;
                case 11: // credits? why from the kids club?
                    FRAGMENT_LOAD(fragment5);
                    var_s0 = FRAGMENT_LOAD_AND_CALL(fragment16, KidsClubCredits_Entry, 0, 0);
                    break;
                case 12: // crashes. This fragment is the "Demo" cutscene bin, but this code passes an invalid demo ID,
                         // which doesnt exist.
                    FRAGMENT_LOAD(fragment3);
                    var_s0 = FRAGMENT_LOAD_AND_CALL(fragment17, DemoCutscene_Entry, 0, 0);
                    break;
                case 13: // ekans minigame
                    FRAGMENT_LOAD(fragment3);
                    var_s0 = FRAGMENT_LOAD_AND_CALL(fragment18, EkansMinigame_Entry, 0, 0);
                    break;
                case 14: // crashes...
                    FRAGMENT_LOAD(fragment3);
                    var_s0 = FRAGMENT_LOAD_AND_CALL(fragment19, UnusedMinigame_Entry, 0, 0);
                    break;
                case 15: // metapod minigame
                    FRAGMENT_LOAD(fragment3);
                    var_s0 = FRAGMENT_LOAD_AND_CALL(fragment20, MetapodMinigame_Entry, 0, 0);
                    break;
                case 16: // sandshrew minigame
                    FRAGMENT_LOAD(fragment3);
                    var_s0 = FRAGMENT_LOAD_AND_CALL(fragment21, SandshrewMinigame_Entry, 0, 0);
                    break;
            }

            main_pool_pop_state('mini');

            switch (var_s0) {
                case 0:
                case 1:
                case 3:
                case 4:
                    break;

                case 2:
                    sp58 = 0;
                    break;
            }
        } else {
            sp58 = 0;
        }
    }

    main_pool_pop_state('KDCM');
}

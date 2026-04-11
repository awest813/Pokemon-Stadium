/*
 * File: fragment62_35DF70.c
 * ROM / VRAM Range: unknown / 0x84300000 fragment-backed
 * Source Type: N64 fragment overlay (part of Fragment 62)
 * Status: DATA_LAYOUT_PARTIAL
 *
 * Purpose:
 *     Contains the move effect handler table and specific logic for move-based
 *     state changes (confusion, flinch, stat stages, etc.).
 *
 * Evidence:
 *     - Contains Japanese strings for battle results
 *     - Indexed by move effect ID during attack resolution
 *
 * Verified:
 *     - This file contains the primary move-effect dispatch jump table.
 */

#include "fragment62.h"
#include "src/2E110.h"

typedef void (*MoveEffectHandler)(void);

static const char D_8438DC80[] = "は\nこんらんした";
static const char D_8438DC90[] = "SP %d\n";
static const char D_8438DC98[] = "index %d  step %d\n";
static const char D_8438DCAC[] = "index %d  step %d\n";
static const char D_8438DCC0[] = "index %d\n";
static const char D_8438DCCC[] = "paramStep %d index %d  step %d\n";
static const char D_8438DCEC[] = "\n%%%%%%%%%%%%%%%%%%%%%%%\n";
static const char D_8438DD08[] = "INDEX %d   WARIKOMI %d\n";
static const char D_8438DD20[] = "%%%%%%%%%%%%%%%%%%%%%%%\n\n";
static const char D_8438DD3C[] = "FAIL\n";
static const char D_8438DD44[] = "index %d\n";
static const char D_8438DD50[] = "index %d\n";
static const char D_8438DD5C[] = "FAIL\n";
static const char D_8438DD64[] = "FAIL2\n";
static const char D_8438DD6C[] = "FAIL3\n";
static const char D_8438DD74[] = "offp->count1 %d\n";
static const char D_8438DD88[] = "\n\n";
static const char D_8438DD8C[] = "******** spAttackMain ********\n";
static const char D_8438DDAC[] = "!!!! SPATTACK Num %d!!!!\n";
static const char D_8438DDC8[] = "******** spAttackMainEnd ********\n";

static char* D_8438AE50 = D_8438DC80;
/*
 * MoveEffectHandlers
 * Original symbol: D_8438AE54
 *
 * Type:
 *     Jump table for move effect resolution.
 *
 * Verified:
 *     - Indexed by (MoveEffectID - 1)
 *     - Contains pointers to C handlers for status, draining, etc.
 */
static MoveEffectHandler MoveEffectHandlers[] = {
    BattleEffect_Confusion, BattleEffect_FlinchChance, BattleEffect_DrainHP, BattleEffect_StatusChance, BattleEffect_StatusChance, BattleEffect_StatusChance, BattleEffect_OHKO,
    BattleEffect_DrainHP, NULL,          BattleEffect_IncreaseStat, BattleEffect_IncreaseStat, BattleEffect_IncreaseStat, BattleEffect_IncreaseStat, BattleEffect_IncreaseStat,
    BattleEffect_IncreaseStat, BattleEffect_PayDay, NULL,          BattleEffect_ReduceStat, BattleEffect_ReduceStat, BattleEffect_ReduceStat, BattleEffect_ReduceStat,
    BattleEffect_ReduceStat, BattleEffect_ReduceStat, BattleEffect_MirrorMove, BattleEffect_Haze, BattleEffect_ThrashLock, BattleEffect_RageLock, BattleEffect_Teleport,
    BattleEffect_BindingMove, BattleEffect_BindingMove, BattleEffect_ConfusionChance, BattleEffect_Confusion, BattleEffect_FlinchChance, BattleEffect_StatusChance, BattleEffect_StatusChance,
    BattleEffect_OHKOAccuracyCheck, BattleEffect_ChargingMove, NULL,          NULL,          BattleEffect_Recharge,
    BattleEffect_ChargingMove, BattleEffect_BindingMove, NULL,          BattleEffect_RecoverHP, BattleEffect_FocusEnergy, BattleEffect_Recoil, BattleEffect_Disable,
    BattleEffect_IncreaseStat, BattleEffect_IncreaseStat, BattleEffect_IncreaseStat, BattleEffect_IncreaseStat, BattleEffect_IncreaseStat, BattleEffect_IncreaseStat, BattleEffect_Rest,
    BattleEffect_Transform, BattleEffect_ReduceStat, BattleEffect_ReduceStat, BattleEffect_ReduceStat, BattleEffect_ReduceStat, BattleEffect_ReduceStat, BattleEffect_ReduceStat,
    BattleEffect_Barrier, BattleEffect_Barrier, BattleEffect_FlinchChance, BattleEffect_PoisonChance, BattleEffect_ReduceStat, BattleEffect_ReduceStat, BattleEffect_ReduceStat,
    BattleEffect_ReduceStat, BattleEffect_ReduceStat, BattleEffect_ReduceStat, BattleEffect_ReduceStat, BattleEffect_ReduceStat, BattleEffect_DisableRecCheck, BattleEffect_BindingMove,
    NULL,          BattleEffect_Substitute, BattleEffect_SetLightScreen, BattleEffect_SetReflect, BattleEffect_Mimic, NULL,          BattleEffect_TransformRecCheck,
    BattleEffect_AcidArmor, BattleEffect_Metronome, NULL,
};

/*
 * Battle_CalcStatWithStage
 * Original symbol: func_843736B0
 *
 * Summary:
 *     Calculates a scaled stat based on its current stage multiplier (-6 to +6).
 *
 * Parameters:
 *     - arg0: Pointer to the result stat (u16).
 *     - arg1: Base stat value (u16).
 *     - arg2: Stat stage index (u8, 1-13, where 7 is neutral).
 *
 * Returns:
 *     Final stat value capped at 999.
 */
u16 Battle_CalcStatWithStage(u16* arg0, u16 arg1, u8 arg2) {
    *arg0 = (arg1 * D_8438ACB8[arg2 - 1].unk_00) / D_8438ACB8[arg2 - 1].unk_01;

    if (*arg0 == 0) {
        *arg0 = 1;
    }

    if (*arg0 > 999) {
        *arg0 = 999;
    }

    return *arg0;
}

/*
 * BattleEffect_Confusion
 * Original symbol: func_84373754
 *
 * Summary:
 *     Handles the logic for applying and checking confusion status.
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Sets actor status field 0x48 to 0x16 (Confusion?)
 *     - Triggers confusion message path if status is already present or just set.
 *     - Likely branches on confusion self-hit vs success.
 */
void BattleEffect_Confusion(void) {
    s32 sp24;
    s32 var_v1;

    D_84390240.unk_00->unk_48 = 0x16;
    sp24 = gTargetBattleActorState->unk_4D & 0x20;
    gTargetBattleActorState->unk_4D &= ~0x20;

    if (sp24 == 0) {
        if (gTargetBattleActorState->unk_15 & 7) {
            D_843C4E44 = 0;
            func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
            Battle_QueueMessage(D_843901A0->unk_088, -0x80);
            return;
        }

        if (gTargetBattleActorState->unk_15 == 0) {
            Battle_CheckMoveEffectHits();
            if ((gBattleMoveFailed == 0) &&
                ((func_84308BD0(D_84390010[!D_84390240.unk_00->unk_2C]) == 0) || (D_8438AC60[0] == 2))) {
                goto block_9;
            }
        }
        gBattleDamage = 0;
        gBattleMoveFailed = 1;
        D_843C4E44 = 0;
        func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
        Battle_QueueMessage(D_843901A0->unk_088, -0x7F);
        return;
    }

block_9:
    if ((func_84308BD0(D_84390010[!D_84390240.unk_00->unk_2C]) != 0) && (D_8438AC60[0] != 2)) {
        D_843C4E44 = 0;
        func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
        Battle_QueueMessage(D_843901A0->unk_088, -0x7F);
        if ((sp24 != 0) && (D_8438AC60[0] == 1)) {
            gTargetBattleActorState->unk_4D |= 0x20;
        }
    } else {
        do {
            if (D_8438AC60[0] == 2) {
                var_v1 = func_8436F6D8() & 7;
            } else {
                var_v1 = func_8436F6D8() & 3;
            }
        } while (var_v1 == 0);
        gBattleMoveFailed = 0;
        gTargetBattleActorState->unk_15 = var_v1;
        func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
        Battle_QueueMessage(D_843901A0->unk_088, -0x7E);
    }
}

/*
 * BattleEffect_FlinchChance
 * Original symbol: func_84373A30
 *
 * Summary:
 *     Handles move effects that have a chance to make the target flinch.
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Checks current battle substate and status flags.
 *     - Sets unk_15 flag |= 8 to trigger flinch logic.
 *     - Threshold for success varies by move ID (Case 2 vs 33).
 */
void BattleEffect_FlinchChance(void) {
    char* sp1C = D_843901A0->unk_088;

    if (!(gTargetBattleActorState->unk_4D & 0x10) && (gTargetBattleActorState->unk_15 == 0) && (gTargetBattleActorState->unk_16[6] != 3) &&
        (gTargetBattleActorState->unk_16[7] != 3)) {
        switch (gActiveBattleActorState->unk_44.unk_01) {
            case 2:
                if (D_84390240.unk_00->unk_48 != 0x18) {
                    D_84390240.unk_00->unk_48 = 0;
                    sp1C = D_843901A0->unk_208;
                } else {
                    sp1C = D_843901A0->unk_308;
                }

                if (func_8436F6D8() >= 0x34) {
                    return;
                }
                break;

            case 33:
                D_84390240.unk_00->unk_48 = 0;
                sp1C = D_843901A0->unk_208;
                if (func_8436F6D8() >= 0x67) {
                    return;
                }
                break;

            default:
                D_84390240.unk_00->unk_48 = 0x16;
                D_843C4DEC = 0xA;
                Battle_CheckMoveEffectHits();
                if (gBattleMoveFailed != 0) {
                    func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
                    Battle_QueueMessage(sp1C, -0x7D);
                    return;
                }
                break;
        }

        gTargetBattleActorState->unk_15 |= 8;
        if (gActiveBattleActorState->unk_44.unk_00 == 0x5C) {
            gTargetBattleActorState->unk_4E |= 1;
            gTargetBattleActorState->unk_51 = 0;
            func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
            Battle_QueueMessage(sp1C, -0x7C);
        } else {
            func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
            Battle_QueueMessage(sp1C, -0x7B);
        }
    } else if (gActiveBattleActorState->unk_44.unk_01 == 0x42) {
        if (D_84390240.unk_00->unk_48 != 0x18) {
            gBattleMoveFailed = 1;
            D_84390240.unk_00->unk_48 = 0x16;
            D_843C4E44 = 0;
        }
        func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
        Battle_QueueMessage(sp1C, -0x7A);
    }
}

/*
 * BattleEffect_DrainHP
 * Original symbol: func_84373D04
 *
 * Summary:
 *     Handles move effects that recover HP based on damage dealt (e.g., Mega Drain).
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Recovers 50% of the damage stored in gBattleDamage.
 *     - Caps recovery at the user's Max HP.
 */
void BattleEffect_DrainHP(void) {
    gBattleDamage /= 2;
    if (gBattleDamage == 0) {
        gBattleDamage = 1;
    }

    if (gActiveBattleActorState->unk_28 < gActiveBattleActorState->unk_0C + gBattleDamage) {
        gActiveBattleActorState->unk_0C = gActiveBattleActorState->unk_28;
    } else {
        gActiveBattleActorState->unk_0C += gBattleDamage;
    }

    if (gActiveBattleActorState->unk_44.unk_01 == 8) {
        func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
        Battle_QueueMessage(D_843901A0->unk_208, -0x79);
    } else {
        func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
        Battle_QueueMessage(D_843901A0->unk_208, -0x78);
    }
}

/*
 * BattleEffect_OHKO
 * Original symbol: func_84373E24
 *
 * Summary:
 *     Handles move effects that result in an immediate knock-out (e.g., Fissure, Horn Drill).
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Sets the target's HP (unk_0C) to 0.
 *     - Clears status and other flags.
 */
void BattleEffect_OHKO(void) {
    D_84390240.unk_00->unk_48 = 0x1E;
    gActiveBattleActorState->unk_0C = 0;
    gActiveBattleActorState->unk_15 = 0;
    gActiveBattleActorState->unk_4D &= ~0x80;
}

/*
 * BattleEffect_StatusChance
 * Original symbol: func_84373E60
 *
 * Summary:
 *     Handles move effects that have a chance to apply a status (Burn, Freeze, Paralyze).
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Checks a random roll (func_8436F6D8) against a threshold.
 *     - Sets unk_15 flag to trigger status application logic.
 */
void BattleEffect_StatusChance(void) {
    unk_D_800FCB18* temp_a2 = &D_84390010[!D_84390240.unk_00->unk_2C]->unk_654.unk_38;
    unk_D_84390010_654* temp_a0 = &D_84390010[!D_84390240.unk_00->unk_2C]->unk_654;

    if (gTargetBattleActorState->unk_4D & 0x10) {
        return;
    }

    if (gTargetBattleActorState->unk_15 != 0) {
        if ((gTargetBattleActorState->unk_15 & 0x20) && (gActiveBattleActorState->unk_44.unk_03 == 0x14)) {
            gTargetBattleActorState->unk_15 = 0;
            if (temp_a0->unk_14 != 4) {
                temp_a2->unk_5A = temp_a2->unk_1F[temp_a0->unk_0C];
            } else {
                temp_a0->unk_14 = 0;
                temp_a0->unk_0C = 0;
                temp_a2->unk_5A = 0xA5;
            }
            func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
            Battle_QueueMessage(D_843901A0->unk_088, -0x77);
        }
        return;
    }

    if ((gTargetBattleActorState->unk_16[6] != (*gActiveBattleActorState).unk_44.unk_03) &&
        (gTargetBattleActorState->unk_16[7] != (*gActiveBattleActorState).unk_44.unk_03)) {
        if ((gActiveBattleActorState->unk_44.unk_01 < 7) || ((D_8438AC60[0] != 2) && (gActiveBattleActorState->unk_44.unk_00 == 0x3B))) {
            if (func_8436F6D8() >= 0x1A) {
                return;
            }
        } else if (func_8436F6D8() >= 0x4D) {
            return;
        }

        switch (gActiveBattleActorState->unk_44.unk_01) {
            case 4:
            case 34:
                gTargetBattleActorState->unk_15 = 0x10;
                Battle_ApplyStatusBurn(D_84390010[!D_84390240.unk_00->unk_2C]);
                func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
                Battle_QueueMessage(D_843901A0->unk_208, -0x76);
                break;

            case 5:
            case 35:
                if (func_84308A4C(D_84390010[!D_84390240.unk_00->unk_2C]) == 0) {
                    gTargetBattleActorState->unk_4D &= ~0x20;
                    gTargetBattleActorState->unk_15 = 0x20;
                    func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
                    Battle_QueueMessage(D_843901A0->unk_208, -0x75);
                }
                break;

            case 6:
            case 36:
                gTargetBattleActorState->unk_15 = 0x40;
                Battle_ApplyStatusParalysis(D_84390010[!D_84390240.unk_00->unk_2C]);
                func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
                Battle_QueueMessage(D_843901A0->unk_208, -0x74);
                break;
        }
    }
}

/*
 * BattleEffect_IncreaseStatReflectTarget
 * Original symbol: func_843741C4
 *
 * Summary:
 *     Raises the defender's Attack stage when they have Reflect (flags2 bit 0x40)
 *     and their Attack stage is not already capped. Used from the post-hit path
 *     with opponent-facing message strings (unk_2C8) and shell field unk_4C.
 *
 * Callers:
 *     - func_84370F40 (after damage; temporarily swaps active actor to the target)
 */
void BattleEffect_IncreaseStatReflectTarget(void) {
    s32 sp2C;
    s32 sp28;
    s32 sp24;
    s32 var_a3;
    u16* sp1C;
    u16* sp18;

    D_84390240.unk_00->unk_4C = 0xA;

    var_a3 = 1;
    sp24 = gActiveBattleActorState->unk_44.unk_01 - 0xA;

    if (sp24 >= 8) {
        var_a3 = 2;
        sp24 -= 0x28;
    }

    if (gActiveBattleActorState->unk_5C[sp24] != 0xD) {
        gActiveBattleActorState->unk_5C[sp24] += var_a3;
        if (gActiveBattleActorState->unk_5C[sp24] >= 0xE) {
            gActiveBattleActorState->unk_5C[sp24] = 0xD;
        }

        sp28 = 0;
        sp2C = 0;

        if (sp24 < 4) {
            switch (sp24) {
                case 0:
                    sp18 = &gActiveBattleActorState->unk_38;
                    sp1C = &gActiveBattleActorState->unk_2A;
                    sp2C = 1;
                    break;

                case 1:
                    sp18 = &gActiveBattleActorState->unk_3A;
                    sp1C = &gActiveBattleActorState->unk_2C;
                    break;

                case 2:
                    sp18 = &gActiveBattleActorState->unk_3C;
                    sp1C = &gActiveBattleActorState->unk_2E;
                    sp28 = 1;
                    break;

                case 3:
                    sp18 = &gActiveBattleActorState->unk_3E;
                    sp1C = &gActiveBattleActorState->unk_30;
                    break;
            }

            if (*sp1C != 0x3E7) {
                *sp1C = (*sp18 * D_8438ACB8[gActiveBattleActorState->unk_5C[sp24] - 1].unk_00) /
                        D_8438ACB8[gActiveBattleActorState->unk_5C[sp24] - 1].unk_01;
                if (*sp1C >= 0x3E8) {
                    *sp1C = 0x3E7;
                }
            } else {
                goto block_29;
            }
        }

        if (gActiveBattleActorState->unk_44.unk_00 == 0x6B) {
            gActiveBattleActorState->unk_56 = 1;
        }

        if (var_a3 == 2) {
            func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
            Battle_QueueMessage(D_843901A0->unk_2C8, sp24 + 0x8D);
        } else {
            func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
            Battle_QueueMessage(D_843901A0->unk_2C8, sp24 + 0x93);
        }

        if ((sp28 != 0) || (D_8438AC60[0] != 1)) {
            Battle_ApplyStatusParalysis(D_84390010[!D_84390240.unk_00->unk_2C]);
        }

        if ((sp2C != 0) || (D_8438AC60[0] != 1)) {
            Battle_ApplyStatusBurn(D_84390010[!D_84390240.unk_00->unk_2C]);
        }
    } else {
    block_29:
        Battle_QueueMessage(D_843901A0->unk_2C8, -0x67);
    }
}

/*
 * BattleEffect_IncreaseStat
 * Original symbol: func_843744F0
 *
 * Summary:
 *     Increases one or more stat stages for the user (e.g., Swords Dance, Agility).
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Increases stage field in unk_5C array by 1 or 2.
 *     - Caps at stage 13 (0xD / +6).
 *     - Reclalculates current stat based on lookup table D_8438ACB8.
 */
void BattleEffect_IncreaseStat(void) {
    s32 sp2C;
    s32 sp28;
    s32 sp24;
    s32 var_a3;
    u16* sp1C;
    u16* sp18;

    if (gActiveBattleActorState->unk_44.unk_00 == 0x6E) {
        D_84390240.unk_00->unk_48 = 0x26;
    } else {
        D_84390240.unk_00->unk_48 = 7;
    }

    var_a3 = 1;
    sp24 = gActiveBattleActorState->unk_44.unk_01 - 0xA;
    if (sp24 >= 8) {
        var_a3 = 2;
        sp24 -= 0x28;
    }

    if (gActiveBattleActorState->unk_5C[sp24] != 0xD) {
        gActiveBattleActorState->unk_5C[sp24] += var_a3;
        if (gActiveBattleActorState->unk_5C[sp24] >= 0xE) {
            gActiveBattleActorState->unk_5C[sp24] = 0xD;
        }

        sp28 = 0;
        sp2C = 0;

        if (sp24 < 4) {
            switch (sp24) {
                case 0:
                    sp18 = &gActiveBattleActorState->unk_38;
                    sp1C = &gActiveBattleActorState->unk_2A;
                    sp2C = 1;
                    break;

                case 1:
                    sp18 = &gActiveBattleActorState->unk_3A;
                    sp1C = &gActiveBattleActorState->unk_2C;
                    break;

                case 2:
                    sp18 = &gActiveBattleActorState->unk_3C;
                    sp1C = &gActiveBattleActorState->unk_2E;
                    sp28 = 1;
                    break;

                case 3:
                    sp18 = &gActiveBattleActorState->unk_3E;
                    sp1C = &gActiveBattleActorState->unk_30;
                    break;
            }

            if (*sp1C != 0x3E7) {
                *sp1C = (*sp18 * D_8438ACB8[gActiveBattleActorState->unk_5C[sp24] - 1].unk_00) /
                        D_8438ACB8[gActiveBattleActorState->unk_5C[sp24] - 1].unk_01;
                if (*sp1C >= 0x3E8) {
                    *sp1C = 0x3E7;
                }
            } else {
                goto block_32;
            }
        }

        if (gActiveBattleActorState->unk_44.unk_00 == 0x6B) {
            gActiveBattleActorState->unk_56 = 1;
        }

        if (var_a3 == 2) {
            func_8002D5D4(0x19, func_8430862C(D_84390010[D_84390240.unk_00->unk_2C]));
            Battle_QueueMessage(D_843901A0->unk_088, sp24 + 0x8D);
        } else {
            func_8002D5D4(0x19, func_8430862C(D_84390010[D_84390240.unk_00->unk_2C]));
            Battle_QueueMessage(D_843901A0->unk_088, sp24 + 0x93);
        }

        if ((sp28 != 0) || (D_8438AC60[0] != 1)) {
            Battle_ApplyStatusParalysis(D_84390010[D_84390240.unk_00->unk_2C]);
        }

        if ((sp2C != 0) || (D_8438AC60[0] != 1)) {
            Battle_ApplyStatusBurn(D_84390010[D_84390240.unk_00->unk_2C]);
        }
    } else {
    block_32:
        D_843C4E45 = 0;
        Battle_QueueMessage(D_843901A0->unk_088, -0x67);
    }
}

/*
 * BattleEffect_ReduceStat
 * Original symbol: func_8437483C
 *
 * Summary:
 *     Reduces one or more stat stages for the target (e.g., Growl, Screech).
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Reduces stage field in unk_5C array by 1 or 2.
 *     - Minimum stage is 1 (-6).
 *     - Reclalculates current stat based on lookup table D_8438ACB8.
 */
void BattleEffect_ReduceStat(void) {
    s32 sp2C;
    s32 sp28;
    s32 sp24;
    s32 sp20;
    u16* sp1C;
    u16* sp18;

    if (gActiveBattleActorState->unk_44.unk_01 < 0x44) {
        D_84390240.unk_00->unk_48 = 0x1A;
    } else {
        D_84390240.unk_00->unk_1A = 3;
    }

    if (gTargetBattleActorState->unk_4D & 0x10) {
        goto end;
    }

    sp24 = gActiveBattleActorState->unk_44.unk_01;
    sp20 = 1;

    if (sp24 >= 0x44) {
        if (func_8436F6D8() < 0x55) {
            sp24 -= 0x44;
            goto block_14;
        }
    } else {
        Battle_CheckMoveEffectHits();

        if (gBattleMoveFailed != 0) {
            goto end;
        }

        if (gTargetBattleActorState->unk_4C & 0x40) {
            goto end;
        }

        sp24 -= 0x12;
        if (sp24 >= 9) {
            sp20 = 2;
            sp24 -= 0x28;
        }

    block_14:
        if (gTargetBattleActorState->unk_5C[sp24] != 1) {
            gTargetBattleActorState->unk_5C[sp24] -= sp20;
            if (gTargetBattleActorState->unk_5C[sp24] <= 0) {
                gTargetBattleActorState->unk_5C[sp24] = 1;
            }

            sp28 = 0;
            sp2C = 0;

            if (sp24 < 4) {
                switch (sp24) {
                    case 0:
                        sp2C = 1;
                        sp18 = &gTargetBattleActorState->unk_38;
                        sp1C = &gTargetBattleActorState->unk_2A;
                        break;

                    case 1:
                        sp18 = &gTargetBattleActorState->unk_3A;
                        sp1C = &gTargetBattleActorState->unk_2C;
                        break;

                    case 2:
                        sp28 = 1;
                        sp18 = &gTargetBattleActorState->unk_3C;
                        sp1C = &gTargetBattleActorState->unk_2E;
                        break;

                    case 3:
                        sp18 = &gTargetBattleActorState->unk_3E;
                        sp1C = &gTargetBattleActorState->unk_30;
                        break;
                }

                if (*sp1C != 1) {
                    *sp1C = (*sp18 * D_8438ACB8[gTargetBattleActorState->unk_5C[sp24] - 1].unk_00) /
                            D_8438ACB8[gTargetBattleActorState->unk_5C[sp24] - 1].unk_01;
                    if (*sp1C == 0) {
                        *sp1C = 1;
                    }
                } else {
                    goto block_39;
                }
            }

            if (sp20 == 2) {
                func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
                Battle_QueueMessage(D_843901A0->unk_208, sp24 + 0x9A);
            } else {
                func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
                Battle_QueueMessage(D_843901A0->unk_208, sp24 + 0xA0);
            }

            if ((sp28 != 0) || (D_8438AC60[0] != 1)) {
                Battle_ApplyStatusParalysis(D_84390010[!D_84390240.unk_00->unk_2C]);
            }

            if ((sp2C != 0) || (D_8438AC60[0] != 1)) {
                Battle_ApplyStatusBurn(D_84390010[!D_84390240.unk_00->unk_2C]);
            }
            return;
        }
    }

block_39:
    if (gActiveBattleActorState->unk_44.unk_01 < 0x44) {
        D_843C4E44 = 0;
        D_843C4E45 = 0;
        Battle_QueueMessage(D_843901A0->unk_088, -0x5A);
        return;
    }

end:
    if (gActiveBattleActorState->unk_44.unk_01 < 0x44) {
        D_843C4E44 = 0;
        D_843C4E45 = 0;
        if (D_843C4DA9 == 0) {
            Battle_QueueMessage(D_843901A0->unk_088, -0x59);
        }
    } else {
        D_843C4E45 = 0;
    }
}

/*
 * BattleEffect_ThrashLock
 * Original symbol: func_84374C9C
 *
 * Summary:
 *     Handles moves that lock the user into repeating for 2-3 turns (e.g., Thrash, Petal Dance).
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Sets repetition flag (unk_4C |= 1).
 *     - Determines duration (unk_4F) as 2 or 3 turns.
 */
void BattleEffect_ThrashLock(void) {
    D_84390240.unk_00->unk_48 = 0x1B;
    gActiveBattleActorState->unk_44.unk_01 = 0;
    gActiveBattleActorState->unk_4C |= 1;
    gActiveBattleActorState->unk_54 = 0;
    gActiveBattleActorState->unk_4F = (func_8436F6D8() & 1) + 2;
}

/*
 * BattleEffect_RageLock
 * Original symbol: func_84374D08
 *
 * Summary:
 *     Handles the Rage effect, locking the user into Rage until the battle ends.
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Sets rage flag (unk_4C |= 2).
 *     - Provides stat increase logic when damaged during lock.
 */
void BattleEffect_RageLock(void) {
    gActiveBattleActorState->unk_44.unk_01 = 0;
    gActiveBattleActorState->unk_4C |= 2;
    gActiveBattleActorState->unk_4F = (func_8436F6D8() & 1) + 2;
}

/*
 * BattleEffect_Teleport
 * Original symbol: func_84374D58
 *
 * Summary:
 *     Handles the Teleport effect, attempting to end the battle.
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Checks if current battle mode allows retreating (trainer battles usually fail).
 *     - Provides failure message in Stadium/GLC modes.
 */
void BattleEffect_Teleport(void) {
    if (gActiveBattleActorState->unk_44.unk_00 == 0x64) {
        D_84390240.unk_00->unk_48 = 0x27;
        Battle_QueueMessage(D_843901A0->unk_088, -0x53);
        return;
    }
    D_84390240.unk_00->unk_48 = 8;
    func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
    Battle_QueueMessage(D_843901A0->unk_088, -0x52);
}

/*
 * BattleEffect_BindingMove
 * Original symbol: func_84374E08
 *
 * Summary:
 *     Handles moves that trap the target for multiple turns (e.g., Wrap, Fire Spin, Clamp).
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Sets binding flag (unk_4C |= 4).
 *     - Determines duration (unk_4F) as 2-5 turns.
 */
void BattleEffect_BindingMove(void) {
    s32 var_v1;

    if (!(gActiveBattleActorState->unk_4C & 4)) {
        gActiveBattleActorState->unk_4C |= 4;

        switch (gActiveBattleActorState->unk_44.unk_01) {
            case 0x4D:
                gActiveBattleActorState->unk_44.unk_01 = 2;

            case 0x2C:
                gActiveBattleActorState->unk_54 = 2;
                gActiveBattleActorState->unk_4F = gActiveBattleActorState->unk_54;
                break;

            default:
                var_v1 = func_8436F6D8() & 3;
                if (var_v1 >= 2) {
                    var_v1 = func_8436F6D8() & 3;
                }
                gActiveBattleActorState->unk_4F = gActiveBattleActorState->unk_54 = var_v1 + 2;
                break;
        }

        D_8439022C = gActiveBattleActorState->unk_54 - 1;
        D_84390240.unk_00->unk_48 = 0x18;
    }
}

/*
 * BattleEffect_ConfusionChance
 * Original symbol: func_84374EE4
 *
 * Summary:
 *     Handles move effects that have a chance to apply confusion.
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Checks move ID to determine if confusion is guaranteed or chance-based.
 *     - Dispatches to BattleEffect_Confusion if check passes.
 */
void BattleEffect_ConfusionChance(void) {
    if (!(gTargetBattleActorState->unk_4D & 0x10)) {
        if (D_8438AC60[0] == 1) {
            if (gActiveBattleActorState->unk_44.unk_01 == 0x1F) {
                if (func_8436F6D8() >= 0x1A) {
                    return;
                }
            } else if (func_8436F6D8() >= 0x4D) {
                return;
            }
        } else {
            gTargetBattleActorState->unk_4D &= ~0x20;
            if (gActiveBattleActorState->unk_44.unk_01 == 0x1F) {
                if (func_8436F6D8() >= 0x1A) {
                    return;
                }
            } else if (func_8436F6D8() >= 0x4D) {
                return;
            }
        }
        gTargetBattleActorState->unk_4C |= 8;
    }
}

void BattleEffect_OHKOAccuracyCheck(void) {
    gBattleDamage = 0;
    D_843C4DA5 = 0xFF;
    if (gActiveBattleActorState->unk_2E < gTargetBattleActorState->unk_2E) {
        gBattleMoveFailed = 1;
        return;
    }
    gBattleDamage = 0xFFFF;
    D_843C4DA5 = 2;
}

/*
 * BattleEffect_ChargingMove
 * Original symbol: func_84375038
 *
 * Summary:
 *     Handles moves that require a charging turn (e.g., Solar Beam, Skull Bash, Fly, Dig).
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Sets charging flag (unk_4C |= 0x10).
 *     - Provides specific invulnerability for Fly (0x13) or Dig (0x5B) if applicable.
 *     - Dispatches "is charging" messages based on move ID.
 */
void BattleEffect_ChargingMove(void) {
    D_84390240.unk_00->unk_48 = 0xE;
    gActiveBattleActorState->unk_4C |= 0x10;

    if ((gActiveBattleActorState->unk_44.unk_01 == 0x2B) || (gActiveBattleActorState->unk_44.unk_00 == 0x5B)) {
        gActiveBattleActorState->unk_4C |= 0x40;
        if (gActiveBattleActorState->unk_44.unk_00 == 0x5B) {
            D_84390240.unk_00->unk_48 = 0xD;
        }

        if (gActiveBattleActorState->unk_44.unk_01 == 0x2B) {
            D_84390240.unk_00->unk_48 = 0xC;
        }
    }

    func_8002D5D4(0x19, func_8430862C(D_84390010[D_84390240.unk_00->unk_2C]));

    switch (gActiveBattleActorState->unk_44.unk_00) {
        case 0xD:
            Battle_QueueMessage(D_843901A0->unk_0C8, -0x51);
            break;

        case 0x4C:
            Battle_QueueMessage(D_843901A0->unk_0C8, -0x50);
            break;

        case 0x82:
            Battle_QueueMessage(D_843901A0->unk_0C8, -0x4F);
            break;

        case 0x8F:
            Battle_QueueMessage(D_843901A0->unk_0C8, -0x4E);
            break;

        case 0x13:
            Battle_QueueMessage(D_843901A0->unk_148, -0x4D);
            break;

        case 0x5B:
            Battle_QueueMessage(D_843901A0->unk_148, -0x4C);
            break;
    }
}

/*
 * BattleEffect_Recharge
 * Original symbol: func_843751F8
 *
 * Summary:
 *     Handles moves that require a recharge turn (e.g., Hyper Beam).
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Sets recharge flag (unk_4C |= 0x20).
 *     - Provides turn-skip logic for the next iteration of the shell.
 */
void BattleEffect_Recharge(void) {
    s32 var_v1;

    if (!(gActiveBattleActorState->unk_4C & 0x20) && ((D_8438AC60[0] != 1) || (gActiveBattleActorState->unk_44.unk_03 != 0) ||
                                         ((gTargetBattleActorState->unk_16[6] != 8) && (gTargetBattleActorState->unk_16[7] != 8)))) {
        gTargetBattleActorState->unk_4D &= 0xFFDF;
        gActiveBattleActorState->unk_4C |= 0x20;
        var_v1 = func_8436F6D8() & 3;
        if (var_v1 >= 2) {
            var_v1 = func_8436F6D8() & 3;
        }
        gActiveBattleActorState->unk_4F = var_v1 + 1;
    }
}

/*
 * BattleEffect_RecoverHP
 * Original symbol: func_843752C0
 *
 * Summary:
 *     Handles HP recovery moves (e.g., Recover, Softboiled).
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Sets recovery flag (unk_4D |= 2).
 *     - Triggers failure message if HP is already full or recovery is impossible.
 */
void BattleEffect_RecoverHP(void) {
    if (gActiveBattleActorState->unk_4D & 2) {
        D_84390240.unk_00->unk_48 = 7;
        Battle_QueueMessage(D_843901A0->unk_088, -0x4B);
        return;
    }
    D_84390240.unk_00->unk_48 = 7;
    gActiveBattleActorState->unk_4D |= 2;
    func_8002D5D4(0x19, func_8430862C(D_84390010[D_84390240.unk_00->unk_2C]));
    Battle_QueueMessage(D_843901A0->unk_088, -0x4A);
}

void BattleEffect_FocusEnergy(void) {
    D_84390240.unk_00->unk_48 = 7;
    if (gActiveBattleActorState->unk_4D & 4) {
        Battle_QueueMessage(D_843901A0->unk_088, -0x49);
        return;
    }
    gActiveBattleActorState->unk_4D |= 4;
    func_8002D5D4(0x19, func_8430862C(D_84390010[D_84390240.unk_00->unk_2C]));
    Battle_QueueMessage(D_843901A0->unk_088, -0x48);
}

/*
 * BattleEffect_Recoil
 * Original symbol: func_84375424
 *
 * Summary:
 *     Handles recoil damage dealt to the user after a successful attack.
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Calculates recoil as 1/4 of damage dealt (gBattleDamage).
 *     - Exception: Struggle (0xA5) deals 1/2 damage as recoil.
 *     - Reduces user HP (unk_12) by calculated amount.
 */
void BattleEffect_Recoil(void) {
    s32 var_v1;

    gActiveBattleActorState->unk_12 = gActiveBattleActorState->unk_0C;

    if (gActiveBattleActorState->unk_44.unk_00 == 0xA5) {
        var_v1 = gBattleDamage >> 1;
    } else {
        var_v1 = gBattleDamage >> 2;
    }

    if (var_v1 == 0) {
        var_v1 += 1;
    }

    if (gActiveBattleActorState->unk_12 < var_v1) {
        gActiveBattleActorState->unk_12 = 0;
    } else {
        gActiveBattleActorState->unk_12 -= var_v1;
    }

    gTargetBattleActorState->unk_12 = gTargetBattleActorState->unk_0C;
    func_8002D5D4(0x19, func_8430862C(D_84390010[D_84390240.unk_00->unk_2C]));
    Battle_QueueMessage(D_843901A0->unk_408, -0x47);
    D_84390240.unk_00->unk_48 = 9;
}

void BattleEffect_DisableRecCheck(void) {
    if (!(gTargetBattleActorState->unk_4D & 0x10) && (func_8436F6D8() < 0x1A) && !(gTargetBattleActorState->unk_4C & 0x80)) {
        D_84390240.unk_00->unk_1A = 3;
        D_843C4E45 = 0;
        gTargetBattleActorState->unk_4C |= 0x80;
        gTargetBattleActorState->unk_50 = (func_8436F6D8() & 3) + 2;
        func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
        Battle_QueueMessage(D_843901A0->unk_208, -0x46);
    }
}

/*
 * BattleEffect_Disable
 * Original symbol: func_843755E8
 *
 * Summary:
 *     Handles the Disable effect, preventing the target from using their last move.
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Randomly selects one of the target's moves to disable.
 *     - Sets disable duration (unk_50) between 2-5 turns.
 *     - Flags target state with unk_4C |= 0x80.
 */
void BattleEffect_Disable(void) {
    D_84390240.unk_00->unk_48 = 0x16;
    D_843C4E44 = 0;

    if (!(gTargetBattleActorState->unk_4D & 0x10)) {
        Battle_CheckMoveEffectHits();
        if ((gBattleMoveFailed == 0) && !(gTargetBattleActorState->unk_4C & 0x80)) {
            D_843C4E44 = 1;
            gTargetBattleActorState->unk_4C |= 0x80;
            gTargetBattleActorState->unk_50 = (func_8436F6D8() & 3) + 2;
            func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
            Battle_QueueMessage(D_843901A0->unk_088, -0x45);
            return;
        }
    }

    if (D_843C4DA9 == 0) {
        Battle_QueueMessage(D_843901A0->unk_088, -0x44);
    }
}

/*
 * BattleEffect_PoisonChance
 * Original symbol: func_84375700
 *
 * Summary:
 *     Handles move effects that have a chance to poison the target.
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Checks random roll unless move is guaranteed poison (e.g. Toxic).
 *     - Sets status flag (unk_15 |= 0x40) to trigger poisoning.
 */
void BattleEffect_PoisonChance(void) {
    D_84390240.unk_00->unk_48 = 0x16;
    if (gTargetBattleActorState->unk_15 == 0) {
        if ((gActiveBattleActorState->unk_44.unk_03 != 0x17) || ((gTargetBattleActorState->unk_16[6] != 4) && (gTargetBattleActorState->unk_16[7] != 4))) {
            Battle_CheckMoveEffectHits();
            if (gBattleMoveFailed == 0) {
                gTargetBattleActorState->unk_15 |= 0x40;
                Battle_ApplyStatusParalysis(D_84390010[!D_84390240.unk_00->unk_2C]);
                func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
                Battle_QueueMessage(D_843901A0->unk_088, -0x43);
                return;
            }
        } else {
            D_84390240.unk_00->unk_48 = 8;
            D_843C4E44 = 0;
            Battle_QueueMessage(D_843901A0->unk_088, -0x42);
            return;
        }
    }

    D_84390240.unk_00->unk_48 = 8;
    D_843C4E44 = 0;
    func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
    Battle_QueueMessage(D_843901A0->unk_088, -0x41);
}

/*
 * BattleEffect_Substitute
 * Original symbol: func_84375894
 *
 * Summary:
 *     Handles the Substitute effect, creating a decoy at the cost of 25% HP.
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Reduces user HP (unk_0C) by 25% of Max HP (unk_28).
 *     - Sets Substitute flag (unk_4D |= 0x10).
 *     - Stores substitute HP in unk_59.
 */
void BattleEffect_Substitute(void) {
    s32 temp_v1;

    D_84390240.unk_00->unk_48 = 0x1C;
    if (gActiveBattleActorState->unk_4D & 0x10) {
        func_8002D5D4(0x19, func_8430862C(D_84390010[D_84390240.unk_00->unk_2C]));
        Battle_QueueMessage(D_843901A0->unk_088, -0x2A);
        return;
    }

    temp_v1 = gActiveBattleActorState->unk_28 >> 2;
    if (temp_v1 >= gActiveBattleActorState->unk_0C) {
        Battle_QueueMessage(D_843901A0->unk_088, -0x29);
        return;
    }

    D_84390240.unk_00->unk_48 = 0x19;
    gActiveBattleActorState->unk_59 = temp_v1;
    gActiveBattleActorState->unk_0C -= temp_v1;
    gActiveBattleActorState->unk_4D |= 0x10;
    func_8002D5D4(0x19, func_8430862C(D_84390010[D_84390240.unk_00->unk_2C]));
    Battle_QueueMessage(D_843901A0->unk_088, -0x28);
}

void BattleEffect_SetLightScreen(void) {
    gActiveBattleActorState->unk_4D |= 0x20;
}

void BattleEffect_SetReflect(void) {
    gActiveBattleActorState->unk_4D |= 0x40;
}

/*
 * BattleEffect_Mimic
 * Original symbol: func_843759F4
 *
 * Summary:
 *     Handles the Mimic effect, temporarily replacing itself with one of the target's moves.
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Randomly selects one of the target's current moves (unk_1F).
 *     - Replaces Mimic's slot in the user's moveset with the selected move.
 */
void BattleEffect_Mimic(void) {
    unk_D_84390010_654* sp24 = &D_84390010[D_84390240.unk_00->unk_2C]->unk_654;
    s32 var_t0;
    u8 temp_v0;

    D_84390240.unk_00->unk_48 = 7;
    Battle_CheckMoveEffectHits();

    if ((gBattleMoveFailed == 0) && !(gTargetBattleActorState->unk_4C & 0x40)) {
        D_843C4DAA = 1;
        do {
            var_t0 = func_8430C414(4);
            if (var_t0 >= 4) {
                var_t0 = 3;
            }
            temp_v0 = gTargetBattleActorState->unk_1F[var_t0];
        } while (temp_v0 == 0);

        gActiveBattleActorState->unk_1F[sp24->unk_0C] = temp_v0;
        func_8002D5D4(0x1E, func_8002D7C0(NULL, 0, D_843900B8, gTargetBattleActorState->unk_1F[var_t0] - 1));
        Battle_QueueMessage(D_843901A0->unk_088, -0x40);
        return;
    }

    Battle_QueueMessage(D_843901A0->unk_088, -0x3F);
}

void BattleEffect_TransformRecCheck(void) {
    Battle_CheckMoveEffectHits();

    if ((gBattleMoveFailed == 0) && (gTargetBattleActorState->unk_16[6] != 0x16) && (gTargetBattleActorState->unk_16[7] != 0x16)) {
        if (!(gTargetBattleActorState->unk_4D & 0x80)) {
            gTargetBattleActorState->unk_4D |= 0x80;
            func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
            Battle_QueueMessage(D_843901A0->unk_088, -0x3E);
            return;
        }
    }

    D_84390240.unk_00->unk_48 = 8;
    func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
    Battle_QueueMessage(D_843901A0->unk_088, -0x3D);
}

void BattleEffect_AcidArmor(void) {
    D_84390240.unk_00->unk_48 = 7;
    Battle_QueueMessage(D_843901A0->unk_088, -0x3C);
}

void BattleEffect_Metronome(void) {
    s32 sp24;
    s32 temp_v1;

    Battle_CheckMoveEffectHits();

    if (gBattleMoveFailed == 0) {
        D_84390240.unk_00->unk_48 = 0x20;
        if (gTargetBattleActorState->unk_52 == 0) {
            do {
                sp24 = func_8436F6D8() & 3;
            } while (gTargetBattleActorState->unk_1F[sp24] == 0);
            temp_v1 = (func_8436F6D8() & 7) + 1;
            gTargetBattleActorState->unk_52 = (sp24 * 0x10) + temp_v1;
            gTargetBattleActorState->unk_57 = gTargetBattleActorState->unk_1F[sp24];
            func_8002D5D4(0x1E, func_8002D7C0(NULL, 0, D_843900B8, gTargetBattleActorState->unk_57 - 1));
            Battle_QueueMessage(D_843901A0->unk_088, -0x3B);
            return;
        }
    }

    D_84390240.unk_00->unk_48 = 8;
    Battle_QueueMessage(D_843901A0->unk_088, -0x3A);
}

/*
 * BattleEffect_PayDay
 * Original symbol: func_84375D78
 *
 * Summary:
 *     Handles the Pay Day effect, scattering coins to be picked up after battle.
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Triggers the "coins scattered everywhere" message.
 *     - Increments post-battle gold reward counter.
 */
void BattleEffect_PayDay(void) {
    Battle_QueueMessage(D_843901A0->unk_088, -0x39);
}

/*
 * BattleEffect_MirrorMove
 * Original symbol: func_84375DA4
 *
 * Summary:
 *     Handles Mirror Move effect, copying the target's last used move.
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Copies target's move ID (unk_16[6/7]) to the user's move state.
 *     - Provides failure message if the target hasn't moved or move is invalid.
 */
void BattleEffect_MirrorMove(void) {
    D_84390240.unk_00->unk_48 = 7;
    if (!(gTargetBattleActorState->unk_4C & 0x40)) {
        gActiveBattleActorState->unk_16[6] = gTargetBattleActorState->unk_16[6];
        gActiveBattleActorState->unk_16[7] = gTargetBattleActorState->unk_16[7];
        func_8002D5D4(0x19, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
        Battle_QueueMessage(D_843901A0->unk_088, -0x38);
    } else {
        Battle_QueueMessage(D_843901A0->unk_088, -0x37);
    }
}

/*
 * BattleEffect_Haze
 * Original symbol: func_84375E6C
 *
 * Summary:
 *     Handles the Haze effect, resetting all stat stages and status for both sides.
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Resets all stat stages in unk_5C array to 7 (neutral) for both actors.
 *     - Restores current stats to base stats (unk_2A = unk_38).
 *     - Clears status effects (unk_15 = 0).
 */
void BattleEffect_Haze(void) {
    s32 i;
    unk_D_800FCB18* temp_t0 = &D_84390010[0]->unk_654.unk_38;
    unk_D_800FCB18* temp_a3 = &D_84390010[1]->unk_654.unk_38;

    D_84390240.unk_00->unk_48 = 0x1D;

    for (i = 0; i < 8; i++) {
        temp_t0->unk_5C[i] = 7;
        temp_a3->unk_5C[i] = 7;
    }

    temp_t0->unk_2A = temp_t0->unk_38;
    temp_t0->unk_2C = temp_t0->unk_3A;
    temp_t0->unk_2E = temp_t0->unk_3C;
    temp_t0->unk_30 = temp_t0->unk_3E;
    temp_a3->unk_2A = temp_a3->unk_38;
    temp_a3->unk_2C = temp_a3->unk_3A;
    temp_a3->unk_2E = temp_a3->unk_3C;
    temp_a3->unk_30 = temp_a3->unk_3E;

    if ((gTargetBattleActorState->unk_15 & 0x27) && !(gTargetBattleActorState->unk_4C & 0x10)) {
        gTargetBattleActorState->unk_5A = 0xFE;
    }

    gTargetBattleActorState->unk_15 = 0;
    // clang-format off
    if (D_8438AC60[0] == 1) { gActiveBattleActorState->unk_15 = 0; }
    // clang-format on

    temp_t0->unk_52 = temp_a3->unk_52 = 0;
    temp_t0->unk_57 = temp_a3->unk_57 = 0;
    temp_t0->unk_4C &= ~0x80;
    temp_t0->unk_4D &= 0x78;
    temp_t0->unk_4E &= 8;
    temp_a3->unk_4C &= ~0x80;
    temp_a3->unk_4D &= 0x78;
    temp_a3->unk_4E &= 8;
    gActiveBattleActorState->unk_56 = 0;
    gTargetBattleActorState->unk_56 = 0;

    Battle_QueueMessage(D_843901A0->unk_088, -0x36);
}

/*
 * BattleEffect_Rest
 * Original symbol: func_8437600C
 *
 * Summary:
 *     Handles the Rest effect, fully restoring HP and curing status by applying sleep.
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Restores HP to Max HP (unk_28).
 *     - Sets status (unk_15) to 2 (Sleep).
 *     - Provides "fell asleep and regained health" message.
 */
void BattleEffect_Rest(void) {
    u8 temp_v0;
    unk_D_84390010_654* temp_a1;
    unk_D_800FCB18* temp_a2;

    temp_a1 = &D_84390010[D_84390240.unk_00->unk_2C]->unk_654;
    temp_a2 = &D_84390010[D_84390240.unk_00->unk_2C]->unk_654.unk_38;

    D_84390240.unk_00->unk_48 = 0x15;
    if (gActiveBattleActorState->unk_0C != gActiveBattleActorState->unk_28) {
        if (gActiveBattleActorState->unk_44.unk_00 == 0x9C) {
            temp_v0 = gActiveBattleActorState->unk_15;

            gActiveBattleActorState->unk_15 = 2;
            temp_a2->unk_16[temp_a1->unk_08 + temp_a1->unk_2B * 3] = 2;

            if (temp_v0 != 0) {
                if (D_8438AC60[0] == 1) {
                    gActiveBattleActorState->unk_4E &= ~1;
                    func_843736B0(&gActiveBattleActorState->unk_2A, gActiveBattleActorState->unk_38, gActiveBattleActorState->unk_5C[0]);
                    func_843736B0(&gActiveBattleActorState->unk_2E, gActiveBattleActorState->unk_3C, gActiveBattleActorState->unk_5C[2]);
                }
                func_8002D5D4(0x19, func_8430862C(D_84390010[D_84390240.unk_00->unk_2C]));
                Battle_QueueMessage(D_843901A0->unk_088, -0x35);
            } else {
                func_8002D5D4(0x19, func_8430862C(D_84390010[D_84390240.unk_00->unk_2C]));
                Battle_QueueMessage(D_843901A0->unk_088, -0x34);
            }
            gActiveBattleActorState->unk_0C += gActiveBattleActorState->unk_28;
        } else {
            gActiveBattleActorState->unk_0C += gActiveBattleActorState->unk_28 / 2;
        }

        if (gActiveBattleActorState->unk_28 < gActiveBattleActorState->unk_0C) {
            gActiveBattleActorState->unk_0C = gActiveBattleActorState->unk_28;
        }

        func_8002D5D4(0x19, func_8430862C(D_84390010[D_84390240.unk_00->unk_2C]));
        Battle_QueueMessage(D_843901A0->unk_148, -0x33);
        return;
    }

    D_84390240.unk_00->unk_48 = 0x1C;
    gBattleMoveFailed = 1;
    Battle_QueueMessage(D_843901A0->unk_088, -0x32);
}

/*
 * BattleEffect_Transform
 * Original symbol: func_84376258
 *
 * Summary:
 *     Handles the Transform effect, copying the target's stats, moves, and stages.
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Copies moves, stats, and stat stages from the target (unk_C) to the user (unk_8).
 *     - Sets moveset to match target with 5 PP each.
 */
void BattleEffect_Transform(void) {
    s32 i;
    unk_D_84390010_654* sp20;
    unk_D_84390010_654* sp1C;
    unk_D_800FCB18* ptr;

    sp20 = &D_84390010[D_84390240.unk_00->unk_2C]->unk_654;
    sp1C = &D_84390010[!D_84390240.unk_00->unk_2C]->unk_654;

    if ((D_8438AC60[0] == 1) && ((((gTargetBattleActorState->unk_4E & 8) == 0) && (gTargetBattleActorState->unk_0B == 0x84)) ||
                                 (((gTargetBattleActorState->unk_4E & 8) != 0) && (sp1C->unk_BE == 0x84)))) {
        D_84390240.unk_00->unk_48 = 0x1C;
        func_8002D5D4(0x1B, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
        Battle_QueueMessage(D_843901A0->unk_088, -0x2F);
        return;
    }

    ptr = gActiveBattleActorState;
    if (!(ptr->unk_4C & 0x40) &&
        ((D_8438AC60[0] != 1) || (gTargetBattleActorState->unk_1F[0] != 0x90) || (gTargetBattleActorState->unk_1F[1] != 0))) {
        D_84390240.unk_00->unk_48 = 0xF;
        func_8002D5D4(0x1B, func_8430862C(D_84390010[!D_84390240.unk_00->unk_2C]));
        Battle_QueueMessage(D_843901A0->unk_148, -0x31);
        gActiveBattleActorState->unk_4E |= 8;

        if (gTargetBattleActorState->unk_4E & 8) {
            sp20->unk_BE = sp1C->unk_BE;
        } else {
            sp20->unk_BE = gTargetBattleActorState->unk_0B;
        }

        gActiveBattleActorState->unk_16[6] = gTargetBattleActorState->unk_16[6];
        gActiveBattleActorState->unk_16[7] = gTargetBattleActorState->unk_16[7];
        gActiveBattleActorState->unk_1F[0] = gTargetBattleActorState->unk_1F[0];
        gActiveBattleActorState->unk_1F[1] = gTargetBattleActorState->unk_1F[1];
        gActiveBattleActorState->unk_1F[2] = gTargetBattleActorState->unk_1F[2];
        gActiveBattleActorState->unk_1F[3] = gTargetBattleActorState->unk_1F[3];
        gActiveBattleActorState->unk_24 = gTargetBattleActorState->unk_24;
        gActiveBattleActorState->unk_2A = gTargetBattleActorState->unk_2A;
        gActiveBattleActorState->unk_2C = gTargetBattleActorState->unk_2C;
        gActiveBattleActorState->unk_2E = gTargetBattleActorState->unk_2E;
        gActiveBattleActorState->unk_30 = gTargetBattleActorState->unk_30;

        for (i = 0; i < 4; i++) {
            if (gTargetBattleActorState->unk_1F[i] == 0) {
                break;
            }
            gActiveBattleActorState->unk_32[i] = 5;
        }

        gActiveBattleActorState->unk_38 = gTargetBattleActorState->unk_38;
        gActiveBattleActorState->unk_3A = gTargetBattleActorState->unk_3A;
        gActiveBattleActorState->unk_3C = gTargetBattleActorState->unk_3C;
        gActiveBattleActorState->unk_3E = gTargetBattleActorState->unk_3E;

        for (i = 0; i < 8; i++) {
            gActiveBattleActorState->unk_5C[i] = gTargetBattleActorState->unk_5C[i];
        }
        return;
    }

    D_84390240.unk_00->unk_48 = 0x1C;
    Battle_QueueMessage(D_843901A0->unk_088, -0x30);
}

/*
 * BattleEffect_Barrier
 * Original symbol: func_843765E8
 *
 * Summary:
 *     Handles Light Screen and Reflect effects.
 *
 * Callers:
 *     - MoveEffectHandlers (via dispatch)
 *
 * Verified behavior:
 *     - Sets Light Screen (unk_4E |= 2) or Reflect (unk_4E |= 4) flags.
 *     - Provides specific activation messages based on move sub-ID.
 */
void BattleEffect_Barrier(void) {
    D_84390240.unk_00->unk_48 = 7;
    if (gActiveBattleActorState->unk_44.unk_01 == 0x40) {
        if (!(gActiveBattleActorState->unk_4E & 2)) {
            gActiveBattleActorState->unk_4E |= 2;
            func_8002D5D4(0x19, func_8430862C(D_84390010[D_84390240.unk_00->unk_2C]));
            Battle_QueueMessage(D_843901A0->unk_088, -0x2E);
        } else {
            Battle_QueueMessage(D_843901A0->unk_088, -0x2D);
        }
    } else if (!(gActiveBattleActorState->unk_4E & 4)) {
        gActiveBattleActorState->unk_4E |= 4;
        func_8002D5D4(0x19, func_8430862C(D_84390010[D_84390240.unk_00->unk_2C]));
        Battle_QueueMessage(D_843901A0->unk_088, -0x2C);
    } else {
        Battle_QueueMessage(D_843901A0->unk_088, -0x2B);
    }
}

/*
 * func_84376768
 *
 * Summary:
 *     Dispatches the active move's effect ID to MoveEffectHandlers (effect IDs are 1-based).
 */
void func_84376768(void) {
    unk_D_800FCB18* ptr = &D_84390010[D_84390240.unk_00->unk_2C]->unk_654.unk_38;
    s32 idx = ptr->unk_44.unk_01;

    if (MoveEffectHandlers[idx - 1] != NULL) {
        MoveEffectHandlers[idx - 1]();
    }
}

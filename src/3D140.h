#ifndef _3D140_H_
#define _3D140_H_

#include "global.h"

/*
 * BattleMove
 * Original symbol: unk_D_800FCB18_044
 * 
 * Summary:
 *     Header data for an active move being executed or queried.
 */
typedef struct BattleMove {
    /* 0x00 */ u8 moveId;
    /* 0x01 */ u8 effectId;
    /* 0x02 */ u8 power;
    /* 0x03 */ u8 type;
    /* 0x04 */ u8 accuracy;
    /* 0x05 */ u8 pp;
} BattleMove; // size = 0x6

/*
 * BattleActorState
 * Original symbol: unk_D_800FCB18
 *
 * Summary:
 *     The active combat state of a Pokemon during a battle session.
 *     Maps closely to Gen 1 RAM structures.
 */
typedef struct BattleActorState {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ char unk02[0x2];
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ char unk06[0x5];
    /* 0x0B */ u8 species;
    /* 0x0C */ u16 hp;
    /* 0x0E */ u16 initialHP;
    /* 0x10 */ u16 targetHP;
    /* 0x12 */ u16 unk_12;
    /* 0x14 */ u8 partyIndex;
    /* 0x15 */ u8 status;
    /* 0x16 */ u8 unk_16[9];
    /* 0x1F */ u8 moves[4];
    /* 0x23 */ char unk23[0x1];
    /* 0x24 */ u16 unk_24;
    /* 0x26 */ u8 level;
    /* 0x28 */ u16 maxHP;
    /* 0x2A */ u16 attack;
    /* 0x2C */ u16 defense;
    /* 0x2E */ u16 speed;
    /* 0x30 */ u16 special;
    /* 0x32 */ u8 pp[4];
    /* 0x36 */ u16 baseMaxHP;
    /* 0x38 */ u16 baseAttack;
    /* 0x3A */ u16 baseDefense;
    /* 0x3C */ u16 baseSpeed;
    /* 0x3E */ u16 baseSpecial;
    /* 0x40 */ char unk40[0x4];
    /* 0x44 */ BattleMove activeMove;
    /* 0x4A */ u8 unk_4A;
    /* 0x4B */ u8 unk_4B;
    /* 0x4C */ u8 flags;
    /* 0x4D */ u8 flags2;
    /* 0x4E */ u8 flags3;
    /* 0x4F */ u8 turns;
    /* 0x50 */ u8 disableTurns;
    /* 0x51 */ u8 unk_51;
    /* 0x52 */ u8 unk_52;
    /* 0x53 */ u8 unk_53;
    /* 0x54 */ u16 unk_54;
    /* 0x56 */ u8 unk_56;
    /* 0x57 */ u8 unk_57;
    /* 0x58 */ u8 unk_58;
    /* 0x59 */ u8 unk_59;
    /* 0x5A */ u8 currentSpecies;
    /* 0x5B */ u8 unk_5B;
    /* 0x5C */ u8 stages[8];
} BattleActorState; // size = 0x64

typedef struct amConfig {
    /* 0x00 */ u32 outputRate;
    /* 0x04 */ u32 framesPerField;
    /* 0x08 */ u32 maxACMDSize;
} amConfig; // size = 0xC

extern s32 mus_active_fade_handle;
extern f32 mus_fade_start_volume;
extern u8 mus_fade_target_volume;
extern f32 mus_fade_step_rate;
extern f32 D_80077DD4;
extern f32 D_80077DD8;
extern f32 D_80077DDC;
extern f32 D_80077DE0;
extern u8 mus_sound_effect_blocked;

typedef BattleActorState unk_D_800FCB18;

extern BattleActorState* D_800FCB18[4];

void amCreateAudioMgr(ALSynConfig*, amConfig*, u32, s32, s32);
OSTask* func_8003CADC(OSTask*);
void __clearAudioDMA(void);
void func_8003D2B8(s32);
void func_8003D32C(void);
s32 func_8003D494(void);
s32 func_8003D4A0(s32);
void func_8003D68C(s32);
void func_8003D6B0(u8, u8, u8, u8);
void func_8003D828(u8, u8, u8, u8);
void func_8003D918(u32 arg0, u32 arg1);
void func_8003DB84(s32 arg0);
void func_8003EB40(s32, unk_D_800FCB18*);
void func_8003F1AC(u32);
void func_8003F4C0(u32);
void func_8003F624(u32 arg0);
void func_8003F660(s32);
void func_80040A70(unk_D_800FCB18* arg0, s32 arg1, s32 arg2, s32 arg3);
void func_800414B8(s32);
void func_8004153C(void);
void func_80041688(void);
void func_800416BC(void);
void func_80041A98(void);
void func_80041CEC(void);
void func_80041C70(u32 arg0);
void func_80041D50(u8* arg0, u8* arg1);
u16 func_80041D70(u16 arg0);
void func_80041DF4(u16 arg0);
void func_80041E40(u16 arg0);
u32 func_80041EF4(u16* arg0);
void func_800420C0(u16* arg0);

#endif // _3D140_H_

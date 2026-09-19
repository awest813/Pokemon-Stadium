#ifndef _5580_H_
#define _5580_H_

#include "global.h"

typedef struct RSPTask {
    /* 0x00 */ OSMesg mesg;
    /* 0x04 */ OSMesgQueue queue;
    /* 0x1C */ u16 state;     // 0=new, 1=running, 2=yielded, 3=done
    /* 0x1E */ u16 rdp_state; // 0=none, 1=queued, 2=complete
    /* 0x20 */ OSTask task;
    /* 0x60 */ char pad[0x8];
} RSPTask; // size = 0x68

typedef struct Scheduler {
    /* 0x000 */ OSThread thread;
    /* 0x1B0 */ char unk1B0[0x800];
    /* 0x9B0 */ u8 stack[64];
    /* 0x9F0 */ OSMesgQueue queue;
    /* 0xA08 */ u64 time;
    /* 0xA10 */ GBMainCtx* clients;
    /* 0xA14 */ RSPTask* cur_task;
    /* 0xA18 */ RSPTask* cur_audio;
    /* 0xA1C */ RSPTask* cur_gfx;
    /* 0xA20 */ RSPTask* next_audio;
    /* 0xA24 */ RSPTask* next_gfx;
    /* 0xA28 */ s32 audio_pending;
    /* 0xA2C */ s32 gfx_pending;
    /* 0xA30 */ s32 task4_pending;
    /* 0xA34 */ s32 vblank_count;
    /* 0xA38 */ s16 pre_nmi;
    /* 0xA3A */ char unkA3A[0x6];
} Scheduler; // size = 0xA40

struct unk_D_80068BB0;

typedef struct FrameConfig {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 unk_03;
    /* 0x04 */ u32 unk_04;
    /* 0x08 */ u32 unk_08;
    /* 0x0C */ struct unk_D_80068BB0* unk_0C;
} FrameConfig; // size = 0x10

typedef struct DisplayCtx {
    /* 0x000 */ OSThread thread;
    /* 0x1B0 */ OSMesg unk_1B0;
    /* 0x1B4 */ char unk1B4[0xC];
    /* 0x1C0 */ OSMesgQueue unk_1C0;
    /* 0x1D8 */ s32 unk_1D8;
    /* 0x1DC */ s32 unk_1DC;
    /* 0x1E0 */ u8 unk1E0[0x800];
    /* 0x9E0 */ struct unk_D_80068BB0* unk_9E0;
    /* 0x9E4 */ OSMesg unk_9E4;
    /* 0x9E8 */ OSMesg unk_9E8;
    /* 0x9EC */ OSMesgQueue unk_9EC;
    /* 0xA04 */ OSMesgQueue unk_A04;
    /* 0xA1C */ char unkA1C[0x4];
    /* 0xA20 */ RSPTask unk_A20;
    /* 0xA88 */ u16 unk_A88;
    /* 0xA8A */ u8 unk_A8A;
    /* 0xA8B */ u8 unk_A8B;
    /* 0xA8C */ FrameConfig unk_A8C;
    /* 0xA9C */ FrameConfig unk_A9C;
    /* 0xAAC */ FrameConfig unk_AAC;
    /* 0xABC */ char unkABC[0x4];
} DisplayCtx; // size = 0xAC0

#define SCHED_MSG_SP_DONE    0x64
#define SCHED_MSG_DP_DONE    0x65
#define SCHED_MSG_VBLANK     0x66
#define SCHED_MSG_START_GFX  0x67
#define SCHED_MSG_PRE_NMI    0x68
 
extern Scheduler gScheduler;
#define D_800A62E0 gScheduler

void SchedClient_Init(DisplayCtx*, s32, s32);
s32 SchedClient_WaitMsg(DisplayCtx*);
s32 SchedClient_PollMsg(DisplayCtx*);
void Sched_Init(void);
void Sched_RegisterClient(GBMainCtx*);
void RSPTask_Reset(RSPTask* arg0);
void RSPTask_WaitDone(RSPTask* arg0);
void Sched_UnregisterClient(GBMainCtx* arg0);
void Sched_QueueTask(RSPTask* arg0, s32 arg1);


#endif // _5580_H_

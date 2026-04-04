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

typedef struct FrameConfig {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 unk_03;
    /* 0x04 */ u32 unk_04;
    /* 0x08 */ u32 unk_08;
    /* 0x0C */ u32 unk_0C;
} FrameConfig; // size = 0x10

// there's some wonkyness going on like the compiler trying to 8-align filler arrays? I dont understand.
typedef struct DisplayCtx {
    /* 0x000 */ OSThread thread;
    /* 0x1B0 */ OSMesg unk_1B0;
    /* 0x1B4 */ char unk1B4[0xC];
    /* 0x1C0 */ OSMesgQueue unk_1C0;
    /* 0x1D8 */ s32 unk_1D8;
    /* 0x1DC */ s32 unk_1DC;
    /* 0x1E0 */ u8 unk1E0[0x800];
    /* 0x9E0 */ FrameConfig* unk_9E0;
    /* 0x9E4 */ char unk9E4[0x48];
    /* 0xA2C */ s32 unk_A2C;
    /* 0xA30 */ char unkA30[0x58];
    /* 0xA88 */ u16 unk_A88;
    /* 0xA8A */ u8 unk_A8A;
    /* 0xA8B */ u8 unk_A8B;
    /* 0xA8C */ u8 unk_A8C;
    /* 0xA8D */ u8 unk_A8D;
    /* 0xA8E */ u8 unk_A8E;
    /* 0xA8F */ u8 unk_A8F;
    /* 0xA90 */ s32 unk_A90;
    /* 0xA94 */ s32 unk_A94;
    /* 0xA98 */ s32 unk_A98;
    /* 0xA9C */ u8 unk_A9C;
    /* 0xA9D */ u8 unk_A9D;
    /* 0xA9E */ u8 unk_A9E;
    /* 0xA9F */ u8 unk_A9F;
    /* 0xAA0 */ s32 unk_AA0;
    /* 0xAA4 */ char unkAA4[0x4];
    /* 0xAA8 */ FrameConfig* unk_AA8;
    /* 0xAAC */ u8 unk_AAC;
    /* 0xAAD */ u8 unk_AAD;
    /* 0xAAE */ u8 unk_AAE;
    /* 0xAAF */ u8 unk_AAF;
    /* 0xAB0 */ char unkAB0[0x8];
    /* 0xAB8 */ s32 unk_AB8;
} DisplayCtx; // size >= 0xABC

extern Scheduler D_800A62E0;

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

#include <PR/os_internal_reg.h>
#include "global.h"
#include "dp_intro.h"
#include "profiler.h"
#include "4BDC0.h"
#include "5580.h"
#include "E1C0.h"
#include "373A0.h"
#include "src/50860.h"
#include "src/490A0.h"
#include "src/4BA90.h"
#include "src/4BDC0.h"

typedef struct unk_D_800A8480 {
  /* 0x0000 */ OSThread thread;
  /* 0x01B0 */ char unk01B0[0x2030];
} unk_D_800A8480; // size = 0x21E0

static s32 D_800A83A0;
static RSPTask D_800A83A8[2];
static s32 D_800A8478;
static s32 D_800A847C;
static unk_D_800A8480 D_800A8480;

void Audio_OnStart(void) {
}

void Audio_OnStop(void) {
}

void Audio_OnPause(void) {
}

void Audio_OnResume(void) {
}

void Audio_ResetTrack(void) {
    D_800A847C = -1;
}

void Audio_PlayTrack(s32 arg0) {
    if (arg0 != D_800A847C) {
        if (D_800A847C >= 0) {
            func_8004B9C4(0);
        }
        func_8004B1CC(arg0);
        D_800A847C = arg0;
    }
}

void Audio_SwitchTrack(s32 arg0) {
    if (arg0 != D_800A847C) {
        func_8004B1CC(arg0);
        D_800A847C = arg0;
    }
}

void Audio_StopTrack(s32 arg0) {
    if (D_800A847C >= 0) {
        func_8004B9C4(arg0);
        D_800A847C = -1;
    }
}

s32 Audio_WaitDone(s32 arg0) {
    s32 retvar = 0;

    if (arg0 != 0) {
        func_8004FCD8(2);
    }
    while (func_800484E0() != 0) {
        if (retvar < 1000000) {
            retvar++;
        } else if (retvar == 1000000) {
            func_8004FD44();
            retvar++;
        }
    }
    return retvar;
}

void Audio_StopAll(void) {
    func_8004FD64(0x10);
}

void Audio_Disable(void) {
    D_800A83A0 = 0;
    func_8003D4A0(0);
}

void Audio_Enable(void) {
    func_8003D4A0(1);
    D_800A83A0 = 1;
}

void AudioThread_Main(void* unused) {
    __osSetFpcCsr(0x01000C01);
    SchedClient_Init(&D_800A8480.thread, 1, 1);
    Sched_RegisterClient(&D_800A8480.thread);
    D_800A83A0 = 1;
    D_800A847C = -1;
    D_800A8478 = 0;
    osCreateMesgQueue(&D_800A83A8[0].queue, &D_800A83A8[0].mesg, 1);
    osCreateMesgQueue(&D_800A83A8[1].queue, &D_800A83A8[1].mesg, 1);
    osSendMesg(&D_800A83A8[0].queue, (void*)'DONE', 0);
    osSendMesg(&D_800A83A8[1].queue, (void*)'DONE', 0);
    Audio_Init();
    func_8004AF24(0);
    func_8004AE90(3, 4);

    // thread loop
    while (1) {
        SchedClient_WaitMsg(&D_800A8480.thread);
        profiler_log_thread4_time();
        if ((D_800A83A0 != 0) && (D_800A62E0.pre_nmi < 0x15)) {
            Audio_BuildTask(&D_800A83A8[D_800A8478].task);
            Sched_QueueTask(&D_800A83A8[D_800A8478], 0);
        }
        D_800A8478 ^= 1;
        profiler_log_thread4_time();
    }
}

void Audio_StartThread(void) {
    osCreateThread(&D_800A8480.thread, 4, AudioThread_Main, NULL, &D_800AA660, 0x50);
    osStartThread(&D_800A8480.thread);
}

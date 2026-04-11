#include "5580.h"
#include "src/29BA0.h"
#include "src/DDC0.h"
#include "src/profiler.h"

Scheduler gScheduler;

void RSPTask_Reset(RSPTask* arg0) {
    osWritebackDCacheAll();
    arg0->state = 0;
    arg0->rdp_state = 0;
}

void RSPTask_WaitDone(RSPTask* arg0) {
    void* sp1C;

    osRecvMesg(&arg0->queue, &sp1C, 1);
}

s32 RSPTask_TryRun(RSPTask* arg0) {
    s32 var_v1 = 0;

    if (arg0->state == 2) {
        osSpTaskLoad(&arg0->task);
        osSpTaskStartGo(&arg0->task);
        arg0->state = 1;
        var_v1 = 1;
    } else if (arg0->state == 0) {
        switch (arg0->task.t.type) {
            case M_AUDTASK:
                profiler_log_vblank_time();
                break;

            case M_GFXTASK:
                profiler_log_gfx_time(TASKS_QUEUED);
                arg0->rdp_state = 1;
                osDpSetStatus(0x3C0);
                break;

            case 4:
                arg0->rdp_state = 2;
                break;
        }

        osSpTaskLoad(&arg0->task);
        osSpTaskStartGo(&arg0->task);

        arg0->state = 1;
        var_v1 = 1;
    }
    return var_v1;
}

s32 RSPTask_TryYield(RSPTask* arg0) {
    s32 var_v1 = 0;

    switch (arg0->task.t.type) {
        case 1:
        case 4:
            if (arg0->state == 1) {
                arg0->state = 2;
                osSpTaskYield();
                var_v1 = 1;
            }
            break;
    }

    return var_v1;
}

s32 RSPTask_OnSPDone(RSPTask* arg0) {
    s32 sp24 = 0;

    if (arg0->state == 2) {
        if (osSpTaskYielded(&arg0->task) == 0) {
            arg0->state = 3;
            switch (arg0->task.t.type) {
                case M_GFXTASK:
                    profiler_log_gfx_time(RSP_COMPLETE);
                    if (arg0->rdp_state == 2) {
                        osSendMesg(&arg0->queue, 'DONE', 0);
                    }
                    break;

                case 4:
                    osSendMesg(&arg0->queue, 'DONE', 0);
                    break;
            }
        }
        sp24 = 1;
    } else {
        switch (arg0->task.t.type) {
            case M_AUDTASK:
                if (arg0->state == 1) {
                    profiler_log_vblank_time();
                    osSendMesg(&arg0->queue, 'DONE', 0);
                }
                break;

            case M_GFXTASK:
                profiler_log_gfx_time(RSP_COMPLETE);
                if (arg0->rdp_state == 2) {
                    osSendMesg(&arg0->queue, 'DONE', 0);
                }
                break;

            case 4:
                osSendMesg(&arg0->queue, 'DONE', 0);
                break;
        }
        arg0->state = 3;
    }
    return sp24;
}

void RSPTask_OnDPDone(RSPTask* arg0) {
    profiler_log_gfx_time(RDP_COMPLETE);
    if (arg0->state == 3) {
        osSendMesg(&arg0->queue, 'DONE', 0);
    }
    arg0->rdp_state = 2;
}

void SchedClient_Init(DisplayCtx* arg0, s32 arg1, s32 arg2) {
    arg0->unk_1DC = arg1;
    arg0->unk_1D8 = 0;
    osCreateMesgQueue(&arg0->unk_1C0, &arg0->unk_1B0, arg2);
}

s32 SchedClient_WaitMsg(DisplayCtx* arg0) {
    void* sp1C;

    osRecvMesg(&arg0->unk_1C0, &sp1C, 1);
    return sp1C;
}

s32 SchedClient_PollMsg(DisplayCtx* arg0) {
    u32 sp1C = 'NULL';

    if (arg0->unk_1C0.validCount != 0) {
        osRecvMesg(&arg0->unk_1C0, &sp1C, 1);
    }

    return sp1C;
}

void Sched_PromoteTasks(void) {
    if ((gScheduler.cur_audio == NULL) && (gScheduler.next_audio != NULL)) {
        gScheduler.cur_audio = gScheduler.next_audio;
        gScheduler.next_audio = NULL;
    }

    if ((gScheduler.cur_gfx == NULL) && (gScheduler.next_gfx != NULL)) {
        gScheduler.cur_gfx = gScheduler.next_gfx;
        gScheduler.next_gfx = NULL;
    }
}

void Sched_Noop(void) {
}

void Sched_BroadcastVBlank(u32 arg0) {
    GBMainCtx* var_s0 = gScheduler.clients;

    while (var_s0 != NULL) {
        if (gScheduler.pre_nmi == 0) {
            if (var_s0->unk_01DC != 2) {
                osSendMesg(&var_s0->queue1, arg0, 0);
            }
        } else if (var_s0->unk_01DC != 0) {
            osSendMesg(&var_s0->queue1, arg0, 0);
        }
        var_s0 = var_s0->unk_01D8;
    }
}

RSPTask* Sched_TryRunTask(RSPTask* arg0) {
    if (arg0 != NULL) {
        if (RSPTask_TryRun(arg0) == 0) {
            arg0 = NULL;
        }
    }
    return arg0;
}

void Sched_StartGFX(void) {
    if (gScheduler.cur_task == NULL) {
        gScheduler.cur_task = Sched_TryRunTask(gScheduler.cur_gfx);
    }
}

s32 Sched_YieldGFX(void) {
    s32 ret = 0;

    if (gScheduler.cur_task != NULL) {
        ret = RSPTask_TryYield(gScheduler.cur_task);
    }

    return ret;
}

void Sched_HandlePreNMI(void) {
    GBMainCtx* var_s0 = gScheduler.clients;

    while (var_s0 != NULL) {
        if (var_s0->unk_01DC == 0) {
            osStopThread(&var_s0->thread);
        }
        var_s0 = var_s0->unk_01D8;
    }
    gScheduler.pre_nmi = 1;
    Audio_StopAll();
    Game_OnPreNMI();
}

void Sched_HandleVBlank(void) {
    RSPTask* var_v0;

    gScheduler.time = osGetTime();
    gScheduler.vblank_count++;

    if ((gScheduler.pre_nmi > 0) && (gScheduler.pre_nmi < 0x1E)) {
        gScheduler.pre_nmi++;
        if (gScheduler.pre_nmi == 0x1A) {
            while (osAfterPreNMI() != 0) {
                IO_WRITE(SP_STATUS_REG, 0x8000);
            }
        }
    }

    Sched_PromoteTasks();
    Sched_Noop();

    if (gScheduler.cur_audio != NULL) {
        if (gScheduler.cur_task == NULL) {
            gScheduler.cur_task = Sched_TryRunTask(gScheduler.cur_audio);
        } else {
            Sched_YieldGFX();
        }
    } else if ((gScheduler.cur_gfx != NULL) && (gScheduler.cur_task == NULL)) {
        gScheduler.cur_task = Sched_TryRunTask(gScheduler.cur_gfx);
    }

    Sched_BroadcastVBlank('VTRE');
}

void Sched_HandleSPDone(void) {
    RSPTask* sp24 = gScheduler.cur_task;
    s32 tmp = RSPTask_OnSPDone(gScheduler.cur_task);

    gScheduler.cur_task = NULL;

    if (tmp) {
        gScheduler.cur_task = Sched_TryRunTask(gScheduler.cur_audio);
    }

    if (sp24->state == 3) {
        switch (sp24->task.t.type) {
            case M_AUDTASK:
                gScheduler.cur_audio = 0;
                gScheduler.audio_pending = 0;
                gScheduler.cur_task = Sched_TryRunTask(gScheduler.cur_gfx);
                break;

            case M_GFXTASK:
                if (gScheduler.cur_gfx->rdp_state == 2) {
                    gScheduler.cur_gfx = NULL;
                    gScheduler.gfx_pending = 0;
                }
                break;

            case 4:
                gScheduler.cur_gfx = NULL;
                gScheduler.task4_pending = 0;
                break;
        }
    }
}

void Sched_HandleDPDone(void) {
    RSPTask_OnDPDone(gScheduler.cur_gfx);
    if (gScheduler.cur_gfx->state == 3) {
        gScheduler.cur_gfx = NULL;
        gScheduler.gfx_pending = 0;
    }
}

void SchedThread_Main(void* arg0) {
    s32 sp24;

    __osSetFpcCsr(0x01000C01);
    osCreateMesgQueue(&gScheduler.queue, gScheduler.stack, 0x10);
    osViSetEvent(&gScheduler.queue, SCHED_MSG_VBLANK, 1);
    osSetEventMesg(4, &gScheduler.queue, SCHED_MSG_SP_DONE);
    osSetEventMesg(9, &gScheduler.queue, SCHED_MSG_DP_DONE);
    osSetEventMesg(0xE, &gScheduler.queue, SCHED_MSG_PRE_NMI);

    while (true) {
        osRecvMesg(&gScheduler.queue, &sp24, 1);

        switch (sp24) {
            case SCHED_MSG_VBLANK:
                Sched_HandleVBlank();
                break;

            case SCHED_MSG_SP_DONE:
                Sched_HandleSPDone();
                break;

            case SCHED_MSG_DP_DONE:
                Sched_HandleDPDone();
                break;

            case SCHED_MSG_START_GFX:
                Sched_StartGFX();
                break;

            case SCHED_MSG_PRE_NMI:
                Sched_HandlePreNMI();
                break;
        }
    }
}

void Sched_Init(void) {
    gScheduler.clients = NULL;
    gScheduler.cur_task = NULL;
    gScheduler.cur_audio = 0;
    gScheduler.cur_gfx = NULL;
    gScheduler.next_audio = NULL;
    gScheduler.next_gfx = NULL;
    gScheduler.audio_pending = 0;
    gScheduler.gfx_pending = 0;
    osCreateThread(&gScheduler.thread, 3, SchedThread_Main, NULL, gScheduler.stack, 0x64);
    osStartThread(&gScheduler.thread);
}

void Sched_RegisterClient(GBMainCtx* arg0) {
    GBMainCtx* var_v1;
    GBMainCtx* var_a1;

    if (gScheduler.clients == NULL) {
        gScheduler.clients = arg0;
    } else {
        var_v1 = gScheduler.clients;
        var_a1 = var_v1->unk_01D8;

        while (var_a1 != NULL) {
            var_v1 = var_a1;
            var_a1 = var_a1->unk_01D8;
        }
        var_v1->unk_01D8 = arg0;
    }
    arg0->unk_01D8 = NULL;
}

void Sched_UnregisterClient(GBMainCtx* arg0) {
    GBMainCtx** var_v0 = &gScheduler.clients;
    GBMainCtx* var_v1 = gScheduler.clients;

    while (var_v1 != NULL) {
        if (var_v1 == arg0) {
            *var_v0 = var_v1->unk_01D8;
            break;
        }
        var_v0 = &var_v1->unk_01D8;
        var_v1 = var_v1->unk_01D8;
    }
}

void Sched_QueueTask(RSPTask* arg0, s32 arg1) {
    if (arg0 != NULL) {
        RSPTask_Reset(arg0);

        switch (arg0->task.t.type) {
            case 2:
                gScheduler.next_audio = arg0;
                break;

            case 1:
                if ((arg1 != 0) && (gScheduler.cur_gfx == NULL)) {
                    gScheduler.cur_gfx = arg0;
                    gScheduler.next_gfx = NULL;
                    osSendMesg(&gScheduler.queue, SCHED_MSG_START_GFX, 0);
                } else {
                    gScheduler.next_gfx = arg0;
                }
                break;

            case 4:
                if (gScheduler.cur_gfx == NULL) {
                    gScheduler.cur_gfx = arg0;
                    gScheduler.next_gfx = NULL;
                    osSendMesg(&gScheduler.queue, SCHED_MSG_START_GFX, 0);
                }
                break;
        }
    }
}

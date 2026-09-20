#include "global.h"
#include "6A40.h"

typedef struct DLBuf {
    u32* buf0;
    u32* buf1;
    u32 alloc_top;
    s32 size;
    s16 cur;
} DLBuf; // size = 0x12

Gfx* gDisplayListHead;
DLBuf D_800A7428;
ScissorRect D_800A7440;

void DLBuf_Swap(void);

void DLBuf_Init(u32 arg0, s32 arg1) {
    D_800A7428.cur = 0;
    D_800A7428.size = arg0;
    D_800A7428.buf0 = main_pool_alloc_with_func(arg0, arg1, 'DYN0', NULL);
    D_800A7428.buf1 = main_pool_alloc_with_func(arg0, arg1, 'DYN1', NULL);
    DLBuf_Swap();
}

void DLBuf_Free(void) {
    main_pool_try_free(D_800A7428.buf1);
    main_pool_try_free(D_800A7428.buf0);
}

void DLBuf_Swap(void) {
    s32* temp_v0 = (s32*)&D_800A7428;

    D_800A7428.cur ^= 1;
    temp_v0 += D_800A7428.cur;

    D_800A7428.alloc_top = *temp_v0 + D_800A7428.size;
    gDisplayListHead = *temp_v0;
}

void DLBuf_GetInfo(Gfx** arg0, Gfx** arg1) {
    s32* ptr = (s32*)&D_800A7428;
    *arg1 = (Gfx*)((uintptr_t)gDisplayListHead - ptr[D_800A7428.cur]);
    *arg0 = (Gfx*)(ptr[D_800A7428.cur]);
}

void* DLBuf_AllocTemp(s32 arg0) {
    u32 temp_v0;
    u32 ret;

    arg0 = ALIGN8(arg0);
    temp_v0 = (D_800A7428.alloc_top - arg0);
    ret = NULL;
    if (temp_v0 >= (uintptr_t)gDisplayListHead) {
        D_800A7428.alloc_top = temp_v0;
        ret = temp_v0;
    }
    return ret;
}

s32 DLBuf_GetRemaining(void) {
    return D_800A7428.alloc_top - (uintptr_t)gDisplayListHead;
}

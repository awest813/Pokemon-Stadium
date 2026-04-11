#include "global.h"
#include "memmap.h"

struct MemoryMap {
    uintptr_t vaddr;
    size_t size;
};

struct MemoryMap gSegments[16];
struct MemoryMap gFragments[240];

/*
 * Set the base virtual address and size of the specified segment memmap.
 */
void Memmap_SetSegmentMap(u32 id, uintptr_t vaddr, size_t size) {
    gSegments[id].vaddr = vaddr;
    gSegments[id].size = size;
}

/*
 * Return the base address of the specified segment number.
 */
uintptr_t Memmap_GetSegmentBaseVaddr(u32 id) {
    return gSegments[id].vaddr;
}

/*
 * Return the current virtual address of a given offset into a segment. This is
 * calculated based on the input bitmask constructed of the following values:
 *
 * Input: XYZZZZZZ
 * X: Safety nibble. Only process if 0.
 * Y: Segment number.
 * Z: Segment offset.
 */
uintptr_t Memmap_GetSegmentVaddr(u32 mask) {
    if ((mask >> 0x1C) == 0) {
        u32 id = (mask & 0x0F000000) >> 0x18;
        u32 value = (mask & 0x00FFFFFF) >> 0x00;
        if (gSegments[id].vaddr != NULL) {
            mask = gSegments[id].vaddr + value;
        }
    }
    return mask;
}

/*
 * Returns the input bitmask equivalent for Memmap_GetSegmentVaddr's input. This function
 * appears to be unused.
 */
u32 Memmap_GetSegmentVaddrMask(u32 i, uintptr_t addr) {
    struct MemoryMap* entry = &gSegments[i];
    size_t diff = addr - entry->vaddr;

    if (diff >= entry->size) {
        return 0;
    }
    return (i << 0x18) | diff;
}

/*
 * Clear the memmap for the segment ID specified.
 */
void Memmap_ClearSegmentMemmap(u32 id) {
    gSegments[id].vaddr = NULL;
    gSegments[id].size = 0;
}

/*
 * Initialize the 16 segments with the segment memmap virtual addresses for the
 * display list head.
 */
void Memmap_SetSegments(Gfx** gfxDl) {
    u32 i;
    Gfx* gfx = *gfxDl;

    for (i = 0; i < 16; i++) {
        gSPSegment(gfx++, i, osVirtualToPhysical((void*)gSegments[i].vaddr));
    }
    *gfxDl = gfx;
}

static void RelocateWord32(u32* word) {
    *word = Memmap_GetFragmentVaddr(*word);
}

static void RelocateJump26(u32* inst) {
    *inst = (((u32)(Memmap_GetFragmentVaddr(((*inst * 4) & 0x0FFFFFFC) + 0x80000000) & 0x0FFFFFFF) >> 2) |
             (*inst & 0xFC000000));
}

static void CacheHi16(u32* inst, u32** refs, u32* vals, u8* valid) {
    u32 reg = (*inst >> 0x10) & 0x1F;
    refs[reg] = inst;
    vals[reg] = *inst;
    valid[reg] = 1;
}

static s32 RelocateLo16(u32* inst, u32** refs, u32* vals, u8* valid) {
    u32 reg = (*inst >> 0x15) & 0x1F;
    if (!valid[reg]) {
        return -1;
    }
    
    uintptr_t relocatedAddr = Memmap_GetFragmentVaddr((vals[reg] << 0x10) + (s16)*inst);
    u32 isLoNeg = (relocatedAddr & 0x8000) ? 1 : 0;
    
    *refs[reg] = (*refs[reg] & 0xFFFF0000) | (((u32)relocatedAddr >> 16) + isLoNeg);
    *inst = (*inst & 0xFFFF0000) | ((u32)relocatedAddr & 0xFFFF);
    
    valid[reg] = 0;
    return 0;
}

/*
 * Perform fragment relocation on a given Fragment.
 */
s32 Memmap_RelocateFragment(u32 id, struct Fragment* fragment) {
    u32* luiRefs[32];
    u32 luiVals[32];
    u8 luiValid[32];
    u32* relocDataP;
    u32 relocSize;
    struct RelocTable* relocInfo;
    u32 i;

    relocSize = fragment->sizeInRam - fragment->relocOffset;
    relocInfo = (struct RelocTable*)((uintptr_t)fragment->relocOffset + (uintptr_t)fragment);

    for (i = 0; i < 32; i++) {
        luiValid[i] = 0;
    }

    osInvalICache(fragment, fragment->sizeInRam);
    osInvalDCache(fragment, fragment->sizeInRam);
    Memmap_SetFragmentMap(id, (uintptr_t)fragment, fragment->sizeInRam);

    for (i = 0; i < relocInfo->nRelocations; i++) {
        u32 reloc = relocInfo->relocations[i];
        relocDataP = (u32*)(RELOC_OFFSET(reloc) + (uintptr_t)fragment);

        switch (RELOC_TYPE(reloc)) {
            case RELOC_MIPS_32:
                RelocateWord32(relocDataP);
                break;
            case RELOC_MIPS_26:
                RelocateJump26(relocDataP);
                break;
            case RELOC_MIPS_HI16:
                CacheHi16(relocDataP, luiRefs, luiVals, luiValid);
                break;
            case RELOC_MIPS_LO16:
                if (RelocateLo16(relocDataP, luiRefs, luiVals, luiValid) != 0) {
                    return -1;
                }
                break;
            default:
                return -2;
        }
    }
    if (relocSize != 0) {
        bzero((void*)((uintptr_t)fragment->relocOffset + (uintptr_t)fragment), relocSize);
    }
    osWritebackDCache(fragment, fragment->sizeInRam);

    return 0;
}

/*
 * Set the base virtual address and size of the specified fragment memmap.
 */
void Memmap_SetFragmentMap(u32 id, uintptr_t vaddr, size_t size) {
    gFragments[id].vaddr = vaddr;
    gFragments[id].size = size;
}

/*
 * Return the base vaddress of the fragment ID specified.
 */
uintptr_t Memmap_GetFragmentBaseVaddr(u32 id) {
    return gFragments[id].vaddr;
}

/*
 * Return the current virtual address of a given offset into a fragment. This is
 * calculated based on the input bitmask constructed of the following values:
 *
 * Input: XYYZZZZZ
 * X: Safety nibble. Only process if 0. This is handled by the address range check
 *    at the start.
 * Y: Fragment number.
 * Z: Fragment offset.
 */
uintptr_t Memmap_GetFragmentVaddr(void* addr) {
    if (((u32)addr >= 0x81000000U) && ((u32)addr < 0x90000000U)) {
        u32 id = (((u32)addr & 0x0FF00000) >> 0x14) - 0x10;
        u32 value = (((u32)addr & 0x000FFFFF));

        if (gFragments[id].vaddr != NULL) {
            addr = gFragments[id].vaddr + value;
        }
    }
    return (uintptr_t)addr;
}

/*
 * Returns the input bitmask equivalent for Memmap_GetFragmentVaddr's input. This function
 * appears to be unused.
 */
u32 Memmap_GetFragmentVaddrMask(u32 i, uintptr_t addr) {
    struct MemoryMap* entry = &gFragments[i];
    size_t diff = addr - entry->vaddr;

    if (diff >= entry->size) {
        return 0;
    }
    return ((i + 0x10) << 0x14) | 0x80000000 | diff;
}

/*
 * Clear the memmap for the fragment ID specified.
 */
void Memmap_ClearFragmentMemmap(u32 id) {
    gFragments[id].vaddr = NULL;
    gFragments[id].size = 0;
}

/*
 * Given a specified address, check to see if it is located inside of a fragment
 * currently loaded. Return a calculated address.
 */
uintptr_t Memmap_GetLoadedFragmentVaddr(uintptr_t addr) {
    struct MemoryMap* fraglist = gFragments;
    int i, UNUSED j;

    for (i = 0, fraglist = gFragments; i < 0xF0; i++, fraglist++) {
        if (addr >= fraglist->vaddr) {
            size_t diff = addr - fraglist->vaddr;
            if (diff < fraglist->size) {
                return ((i + 0x10) << 0x14) | 0x80000000 | diff;
            }
        }
    }
    return 0;
}

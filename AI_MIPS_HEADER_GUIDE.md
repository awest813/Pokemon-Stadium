# AI MIPS Header Guide for N64 ROMs, Disassembly, and Decompilation

## Purpose

This guide defines how AI coding agents should write headers, comments, labels, and metadata when working on:

* Nintendo 64 ROM disassembly
* MIPS III / VR4300 assembly
* overlays / fragments / compressed assets
* partial decompilation
* symbol promotion
* game state tracing
* battle/minigame/system audits

The goal is to make all reverse-engineering output:

* consistent
* honest
* easy to diff
* easy to upgrade from unknown → known
* safe for later cleanup into production-quality decomp code

---

# 1. Global Rules

## 1.1 Never pretend certainty

If something is not proven, say so.

Use:

* `Verified`
* `Likely`
* `Possible`
* `Unknown`
* `Unconfirmed`

Do **not** use:

* “This definitely does X”
* “This is the battle loop”
* “This struct is actor state”
  unless confirmed by call flow, data usage, or runtime evidence.

## 1.2 Separate fact from interpretation

Every header should distinguish:

* what is directly observed
* what is inferred
* what is still unknown

## 1.3 Prefer stable names over flashy names

Bad:

* `MegaBattleEngine_UpdateAllPokemonAndMoves`

Good:

* `BattleScene_Update`
* `func_800A1234`
* `BattleCmd_ProcessQueue`
* `PokemonState` only after enough evidence

## 1.4 Preserve address identity

Until fully promoted, keep the address visible somewhere.

Examples:

* `func_800A1234`
* `D_8012F8C0`
* `fragment62_2EA8E0.c`

Even if you add a semantic alias, keep the original address traceable.

---

# 2. Required File Header Format

Every disassembly/decomp file should begin with a structured header like this:

```c
/*
 * File: fragment62_2EA8E0.c
 * ROM / VRAM Range: unknown / 0x82XXXXXX overlay-backed
 * Source Type: N64 fragment overlay
 * Status: Partially understood
 *
 * Purpose:
 *     Likely contains shared battle scene logic used by multiple battle-capable modes.
 *
 * Evidence:
 *     - Loaded through common battle entry flow
 *     - Called after pre-battle wrapper fragments
 *     - Receives shared battle context struct
 *
 * Verified:
 *     - This file is loaded as a fragment/overlay
 *     - It participates in a common game-state path
 *
 * Likely:
 *     - Contains core battle scene update/render logic
 *
 * Unknown:
 *     - Exact ownership of command processing, rendering, and result-state transitions
 *
 * Notes for future RE:
 *     - Trace entrypoint first
 *     - Identify per-frame loop
 *     - Separate init/update/draw/teardown paths
 */
```

## Required sections

Each file header should contain:

* `File`
* `ROM / VRAM Range`
* `Source Type`
* `Status`
* `Purpose`
* `Evidence`
* `Verified`
* `Likely`
* `Unknown`
* `Notes for future RE`

---

# 3. Assembly File Header Format

For raw `.s` files:

```asm
# ============================================================================
# File: func_800A1234.s
# VRAM: 0x800A1234
# ROM:  0x00123456
# Type: Function
# Status: Unnamed / partially analyzed
#
# Summary:
#   Possible state-transition helper for battle/menu flow.
#
# Verified:
#   - Called from func_8009FABC
#   - Returns small integer status code in v0
#   - Reads global at D_800F1234
#
# Likely:
#   - Implements substate dispatch or confirmation handling
#
# Unknown:
#   - Meaning of arg0
#   - Meaning of return values 2, 3, 5
#
# Stack:
#   - Frame size: 0x30
#   - Saves: ra, s0, s1
#
# Calling convention:
#   a0 = unknown
#   a1 = unknown
#   v0 = status/result
# ============================================================================
```

---

# 4. Function Header Format

Every promoted or semi-promoted function should have a header.

```c
/*
 * func_800A1234
 *
 * Summary:
 *     Processes a substate transition and returns a status/result code.
 *
 * Callers:
 *     - func_8009FABC
 *     - func_8009FD10
 *
 * Callees:
 *     - func_800A1000
 *     - func_800B0040
 *
 * Arguments:
 *     a0 / arg0:
 *         Likely current state or context pointer.
 *     a1 / arg1:
 *         Unknown.
 *
 * Returns:
 *     v0:
 *         Observed values: 0, 1, 2, 5
 *         Likely a state/result code.
 *
 * Verified behavior:
 *     - Reads a halfword from context + 0x1C
 *     - Branches on values 0..3
 *     - Writes result flag before return
 *
 * Unknown:
 *     - Exact semantic meaning of return values
 *     - Whether this is menu or battle-local logic
 */
```

## Function naming rules

### Use semantic names only when earned

Allowed:

* `BattleScene_Init`
* `BattleScene_Update`
* `BattleScene_Draw`
* `BattleScene_Main`
* `BattleCmd_Execute`

Not allowed unless proven:

* `CalcCriticalHitChance`
* `ApplyParalysisSpeedReduction`
* `DrawPikachuTailLightning`

### Transitional naming is good

Use staged names like:

* `func_800A1234`
* `BattleScene_func_800A1234`
* `BattleScene_SubstateAdvance`
* `BattleScene_SubstateAdvanceConfirmed`

That lets the name evolve instead of lying early.

---

# 5. Data Header Format

For globals, tables, and unknown blobs:

```c
/*
 * D_800F1234
 *
 * Type:
 *     Unknown global struct or context block.
 *
 * Access pattern:
 *     - Read by func_800A1234
 *     - Written by func_800A1400
 *     - Passed indirectly through battle fragment entry flow
 *
 * Observed fields:
 *     0x00: s16 or flags
 *     0x02: s16 substate?
 *     0x1C: value used in switch
 *     0x24: pointer or segmented address
 *
 * Likely role:
 *     Shared scene or battle context.
 *
 * Unknown:
 *     Full struct layout
 *     Lifetime / owner
 */
```

For tables:

```c
/*
 * D_800F4000
 *
 * Type:
 *     Jump table or dispatch table.
 *
 * Verified:
 *     - Indexed by value range 0..5
 *     - Entries point into same function bank
 *
 * Likely:
 *     Substate dispatch for battle/menu/overlay-local flow
 */
```

---

# 6. Struct Header Format

When reconstructing a struct:

```c
/*
 * BattleContext (tentative)
 *
 * Size:
 *     At least 0x1200 bytes
 *
 * Confidence:
 *     Low-to-medium
 *
 * Verified fields:
 *     0x0000: s16 mode/state input
 *     0x0002: s16 current substate
 *     0x11F5: flags_in
 *     0x11F6: flags_out / result flags
 *
 * Likely:
 *     Shared battle/session state passed across wrapper fragments.
 *
 * Unknown:
 *     Ownership of party/team data
 *     Whether rendering state lives here or elsewhere
 */
typedef struct BattleContext {
    /* 0x0000 */ s16 unk_0000;
    /* 0x0002 */ s16 unk_0002;
    /* ...    */ u8  pad[0x11F5 - 0x0004];
    /* 0x11F5 */ u8  flags_in;
    /* 0x11F6 */ u16 flags_out;
} BattleContext;
```

## Struct rules

* Always annotate offsets
* Never hide unknown padding
* Keep uncertain fields as `unk_XXXX`
* Use tentative struct names only with confidence note

---

# 7. N64 ROM Header Guide

When documenting the ROM itself, use a header like this:

```c
/*
 * N64 ROM Header
 *
 * Offset: 0x00000000
 * Size:   0x40
 *
 * Standard fields:
 *     0x00: PI BSD domain register init / endianness signature
 *     0x04: Clock rate
 *     0x08: Program entry point
 *     0x0C: Release
 *     0x10: CRC1
 *     0x14: CRC2
 *     0x18: Reserved
 *     0x20: Internal ROM name
 *     0x34: Reserved
 *     0x3B: Media format / ID region fields
 *
 * Notes:
 *     - Endianness may be .z64, .n64, or .v64 formatted externally
 *     - Entry point alone does not describe overlay layout
 *     - Overlay/fragment systems are game-specific and must be mapped separately
 */
```

## For AI agents

Do not confuse:

* ROM header
* boot code
* game entrypoint
* overlay header
* fragment header
* archive header

These are different things.

---

# 8. Fragment / Overlay Header Guide

For games with fragment systems:

```c
/*
 * Fragment Header
 *
 * Expected layout:
 *     0x00: bootstrap instructions (often jump + nop)
 *     0x08: magic "FRAGMENT"
 *     0x10: headerSize
 *     0x14: relocOffset
 *     0x18: sizeInRom
 *     0x1C: sizeInRam
 *     0x20: payload
 *
 * Relocation:
 *     Applied after fragment is loaded into RAM.
 *
 * Notes:
 *     - Fragment entry may be executable immediately after relocation
 *     - sizeInRom and sizeInRam may differ
 *     - relocation table typically lives at relocOffset
 */
```

This matches the kind of fragment format already seen in projects using a `Fragment` struct and relocation pass.

---

# 9. Relocation Header Guide

When documenting relocation code:

```c
/*
 * Relocation Encoding
 *
 * Packed format:
 *     bits 31..24: relocation type
 *     bits 23..00: fragment-relative word offset
 *
 * Supported relocation types:
 *     R_MIPS_32   - full 32-bit pointer relocation
 *     R_MIPS_26   - J/JAL target relocation
 *     R_MIPS_HI16 - high half of split address
 *     R_MIPS_LO16 - low half of split address
 *
 * Important:
 *     HI16/LO16 are paired and must be resolved together.
 *
 * AI agent rules:
 *     - Never assume all overlays use identical relocation rules
 *     - Validate unknown relocation types before naming them
 *     - Add explicit error handling for unsupported types
 */
```

---

# 10. MIPS Function Anatomy Guide

AI agents should recognize common MIPS function structure.

## Typical prologue

```asm
addiu sp, sp, -0x30
sw    ra, 0x2C(sp)
sw    s0, 0x28(sp)
sw    s1, 0x24(sp)
```

## Typical epilogue

```asm
lw    ra, 0x2C(sp)
lw    s0, 0x28(sp)
lw    s1, 0x24(sp)
jr    ra
addiu sp, sp, 0x30
```

## Notes for AI agents

* Delay slot matters
* `jr ra` often has a useful instruction in the delay slot
* Do not move delay-slot semantics casually when rewriting to C
* Leaf functions may not save `ra`
* Tail calls can hide function boundaries

---

# 11. MIPS Register Meaning Guide

Use this reference in headers and comments.

```text
zero = constant 0
at   = assembler temporary
v0-v1 = return values
a0-a3 = first 4 arguments
t0-t9 = caller-saved temporaries
s0-s7 = callee-saved registers
k0-k1 = kernel/internal
gp    = global pointer
sp    = stack pointer
fp/s8 = frame pointer
ra    = return address
hi/lo = mult/div result registers
```

## AI rules

* Never assume `s0` means “state” or `a0` means “context”
* Register role is local to the function
* Track actual usage before naming variables

---

# 12. Comment Rules for Disassembly

## Good comments

```asm
# Loads current substate from context + 0x1C
lh    t6, 0x1C(a0)

# Branches to one of four handlers based on substate
sltiu at, t6, 4
beqz  at, .Ldefault
nop
```

## Bad comments

```asm
# Loads the current Pokémon attack phase engine state
lh    t6, 0x1C(a0)
```

unless you truly proved that.

---

# 13. Status Labels for Reverse Engineering

Use one of these in file or function headers:

```text
UNIDENTIFIED
PARTIALLY_UNDERSTOOD
CONTROL_FLOW_MAPPED
ARGUMENTS_PARTIAL
RETURNS_PARTIAL
DATA_LAYOUT_PARTIAL
READY_FOR_RENAME
READY_FOR_SPLIT
READY_FOR_DECOMP_CLEANUP
```

Example:

```c
/*
 * Status:
 *     CONTROL_FLOW_MAPPED
 *     ARGUMENTS_PARTIAL
 *     DATA_LAYOUT_PARTIAL
 */
```

---

# 14. Naming Confidence Tags

Use confidence tags next to semantic names.

```c
// BattleScene_Update [confidence: medium]
// ActorCmd_ApplyTransform [confidence: low]
// Menu_SelectMainMode [confidence: high]
```

Or in headers:

```c
/*
 * Proposed name:
 *     BattleScene_Update
 * Confidence:
 *     Medium
 */
```

---

# 15. Recommended Prefixes

Use consistent prefixes.

## Functions

* `func_800XXXXX` for unknown
* `Battle_`
* `BattleScene_`
* `BattleCmd_`
* `Overlay_`
* `Fragment_`
* `Menu_`
* `State_`
* `Actor_`
* `Anim_`
* `Audio_`
* `Camera_`
* `UI_`

## Data

* `D_800XXXXX` for unknown globals
* `sSomething` for file-static
* `gSomething` for global shared state

## Enums

* `BattleState`
* `BattleSubstate`
* `RelocType`
* `FragmentId`

---

# 16. Switch / State Machine Header Template

For dispatcher-style functions:

```c
/*
 * State machine summary:
 *
 * Input state source:
 *     context->unk_1C
 *
 * Observed substates:
 *     0 = init
 *     1 = fade-in / setup
 *     2 = active loop
 *     3 = exit / teardown
 *
 * Verified:
 *     - Returns 0 while active
 *     - Returns 1 or 2 on exit path
 *
 * Unknown:
 *     - Exact semantic meaning of states 1 and 3
 */
```

This is huge for N64 RE because a lot of code is switch-driven.

---

# 17. Runtime Evidence Header Template

If a function was analyzed with emulator/debug evidence, document that.

```c
/*
 * Runtime evidence:
 *     - Breakpoint hit when entering battle scene
 *     - Executes once per frame during active battle
 *     - Writes result flags before scene exit
 *
 * Debug notes:
 *     - a0 points to stable context block
 *     - v0 transitions from 0 -> 1 at end state
 */
```

That keeps AI agents from inventing conclusions from static reading alone.

---

# 18. Unknowns Template

Every serious RE header should include this block:

```c
/*
 * Unknowns:
 *     - True meaning of arg1
 *     - Whether this owns rendering or only dispatches it
 *     - Whether return value is a substate or high-level scene result
 *
 * To verify next:
 *     1. Trace writes to context + 0x1C
 *     2. Identify callers that branch on v0
 *     3. Check whether this is hit once per frame
 */
```

This is one of the best habits for AI agents.

---

# 19. What AI Agents Must Never Do

## Do not:

* rename everything at once
* replace all `unk_` fields with guessed names
* collapse overlays into one fake module too early
* delete address-based identifiers
* claim exact Pokémon/gameplay semantics without evidence
* turn jump tables into “obvious menu logic” without proving it
* assume every pointer-looking value is a real pointer
* ignore segmented addresses or relocation encoding

## Especially for N64:

* do not forget delay slots
* do not ignore sign extension on immediates
* do not misread HI16/LO16 pairs
* do not treat overlay-local addresses as fixed ROM addresses
* do not confuse ROM offset, VRAM address, and runtime relocated address

---

# 20. Minimum Header Standard for Any New Decompiled Function

If an AI agent touches a function, at least add this:

```c
/*
 * func_800A1234
 *
 * Summary:
 *     Unknown helper used in battle-related flow.
 *
 * Verified:
 *     - Called by func_8009FABC
 *     - Returns small integer in v0
 *
 * Unknown:
 *     - Arg meanings
 *     - Exact role
 */
```

That is the floor.

---

# 21. Gold-Standard Header Example

```c
/*
 * BattleScene_Main [proposed]
 * Original symbol: func_800A1234
 * VRAM: 0x800A1234
 * ROM:  0x00123456
 * Status:
 *     CONTROL_FLOW_MAPPED
 *     ARGUMENTS_PARTIAL
 *     READY_FOR_RENAME
 *
 * Summary:
 *     Likely main controller for a shared battle scene substate machine.
 *
 * Evidence:
 *     - Reached through common battle entry flow
 *     - Reads/writes shared context fields
 *     - Called repeatedly until nonzero return
 *
 * Arguments:
 *     arg0 / a0:
 *         Likely pointer to shared battle context.
 *     arg1 / a1:
 *         Unknown, possibly mode/config.
 *
 * Returns:
 *     v0:
 *         0 while scene remains active
 *         nonzero on completion / transition
 *
 * Verified:
 *     - Switches on halfword at context + 0x1C
 *     - Calls init-like path once
 *     - Calls update-like path repeatedly
 *     - Sets exit/result flag before returning nonzero
 *
 * Likely:
 *     - Owns high-level battle scene state progression
 *
 * Unknown:
 *     - Whether command resolution is internal or delegated
 *     - Whether rendering is owned here or in a callee
 *
 * Next RE steps:
 *     1. Name substate enum
 *     2. Identify update-loop callee
 *     3. Split init/update/exit helpers
 */
```

---

# 22. Short Agent Prompt Add-On

You can also give AI agents this short instruction block:

```text
When editing N64 MIPS disassembly/decomp:

1. Preserve original address identity.
2. Distinguish verified facts from inference.
3. Use cautious names until behavior is proven.
4. Annotate arguments, returns, callers, and callees.
5. Keep struct offsets explicit.
6. Document unknowns and next verification steps.
7. Respect MIPS delay slots, HI16/LO16 pairing, and overlay relocation.
8. Never replace uncertainty with confident-sounding fiction.
```

---

# 23. Best Practice for Pokémon Stadium / Overlay-Heavy Games

For overlay-heavy N64 games, use these extra labels:

* `FragmentEntry`
* `FragmentMain`
* `Fragment_Init`
* `Fragment_Update`
* `Fragment_Draw`
* `Fragment_Exit`

For shared systems:

* `BattleShell_*`
* `BattleScene_*`
* `MinigameShell_*`
* `MenuShell_*`

For cross-fragment shared context:

* `SharedGameContext`
* `BattleContext`
* `MinigameContext`
* `OverlayContext`

Only promote to specific gameplay names after confirming the path from:

* loader
* entrypoint
* per-frame loop
* exit/result flags

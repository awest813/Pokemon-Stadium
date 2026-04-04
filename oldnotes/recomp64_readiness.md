# Pokemon Stadium Recomp Readiness

Last updated: 2026-04-04

## Current fit

This repo is already a strong candidate for a future N64Recomp port because it has:

- A working splat/decomp layout with named C translation units and linker scripts.
- A build that is intended to produce an ELF and map file.
- `KEEP_MDEBUG ?= 1` in the Makefile already, which is useful because current N64Recomp workflows rely on ELF metadata.
- Explicit fragment/overlay structure in `src/fragments/`, which maps well to N64Recomp's overlay support.

## Immediate blockers

1. Get the normal WSL decomp build healthy again.
   The first hard blocker is not recomp-specific: `make build/src/19840.o` currently reaches the host syntax pass, but the IDO compile dies while trying to open `lib/ultralib/include/PR/gbi.h`.

2. Preserve a clean ELF-producing path.
   Current N64Recomp still expects an ELF as the metadata source, so "clean ROM build -> stable ELF -> stable map" needs to work reliably before deeper recomp work starts.

3. Reduce anonymous code in overlay-heavy areas.
   Remaining `GLOBAL_ASM` and large unnamed fragment code will make symbol generation and later patch work much more painful, even if the recompiler can technically operate on unnamed symbols.

## Stadium-specific concerns

1. Overlays/fragments
   Pokemon Stadium is fragment-heavy. This is workable because N64Recomp supports statically linked and relocatable overlays, but it means we should keep fragment boundaries, relocation info, and load/unload behavior well understood.

2. Runtime glue
   N64ModernRuntime covers libultra-style threads, controllers, audio, message queues, timers, RSP task handling, and VI timing. `librecomp` also covers overlay handling, PI DMA, and save backends. Stadium will still need project-side glue for renderer setup, input callbacks, and audio output.

3. RSP code
   N64Recomp can handle RSP microcode, but current upstream notes say RSP overlays are not supported yet. If Stadium only uses fixed microcode blobs, that is fine. If it relies on overlay-style RSP loading anywhere, that needs an early audit.

## Practical next steps

1. Fix the WSL/IDO include failure and restore a clean `make` path.
2. Keep converting easy `NON_MATCHING` functions into normal C so more symbol names survive into the ELF cleanly.
3. Produce and archive one known-good `build/pokestadium-us.elf` and `build/pokestadium-us.map`.
4. Add a small script or note for generating a first-pass N64Recomp TOML and symbol dump from that ELF.
5. Prioritize naming around:
   - fragment loader / relocator paths
   - DMA and ROM reads
   - scheduler + RSP task submission
   - framebuffer / VI ownership
   - audio manager startup

## Nice signs already present

- `src/fragments/` separation should help when mapping overlays.
- The decomp already distinguishes game code, assets, and generated asm cleanly.
- The Makefile is already Linux/WSL-oriented, which matches the most practical N64Recomp workflow.

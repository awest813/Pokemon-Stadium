# Pokemon Stadium Recomp Readiness

Last updated: 2026-09-20

Living plan: [UPSTREAM_AUDIT_AND_PLAN.md](../UPSTREAM_AUDIT_AND_PLAN.md).

## Current fit

This repo is a strong N64Recomp candidate because it has:

- A splat/decomp layout with named C APIs, linker scripts, and 77 relocatable fragments.
- `KEEP_MDEBUG ?= 1` in the Makefile (N64Recomp wants ELF metadata).
- Fixed RSP microcode only (F3DEX2, `njpgdspMain`, `aspMain`, `rspboot`). No RSP overlays.

## Immediate blockers

1. **Matching `make` with a US 1.0 ROM.** There is no `baseroms/us/baserom.z64` in the Cloud Agent workspace. The old `19840.c` / `gbi.h` IDO include failure was an environment issue; pret has since matched `19840.c`. Re-verify rather than assuming it is still broken.

2. **Archive a clean ELF + map** (`build/pokestadium-us.elf`) out of git.

3. **Anonymous overlay bodies.** Fragment 62’s shell is named; ~1926 `func_843*` functions and 93 `GLOBAL_ASM` stubs still make symbol generation noisy.

## Stadium-specific concerns

1. **Overlays/fragments** — relocatable, documented in `FRAGMENT_ROLES.md`. Keep load/unload and reloc tables intact.

2. **Runtime glue** — N64ModernRuntime covers threads, controllers, audio queues, timers, RSP tasks, VI timing, overlay handling, PI DMA, save backends. Project-side: renderer setup, input callbacks, audio output.

3. **RSP** — audited; no overlay-style RSP loads. N64Recomp’s RSP-overlay gap does not apply.

## Practical next steps

1. `make init && make` with US 1.0 baserom.
2. Match the two remaining `12D80.c` `GLOBAL_ASM` functions; continue Fragment 62 structural names.
3. Archive `build/pokestadium-us.elf`.
4. Optional: script overlay bounds + named functions → first-pass N64Recomp TOML.
5. Do not start asset format decomp until P0–P1 in the audit plan are done.

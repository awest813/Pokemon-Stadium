# Pokemon Stadium (US)
A WIP decomp of Pokemon Stadium (US).

It builds the following ROMs:

* pokestadium.z64: `md5: ed1378bc12115f71209a77844965ba50`

Note: To use this repository, you must already have a rom for the game.

## Reverse-engineering guides

These documents are for human contributors and AI coding agents working on disassembly, naming, and promotion:

* **[UPSTREAM_AUDIT_AND_PLAN.md](UPSTREAM_AUDIT_AND_PLAN.md)** — Current audit, pret catch-up notes, and ordered major-gaps plan.
* **[AI_MIPS_HEADER_GUIDE.md](AI_MIPS_HEADER_GUIDE.md)** — How to write headers, comments, and metadata for N64/MIPS decomp work (certainty labels, address identity, overlay conventions).
* **[POKEMON_STADIUM_USA_PARTIAL_SYSTEMS_BLOCKER_GUIDE.md](POKEMON_STADIUM_USA_PARTIAL_SYSTEMS_BLOCKER_GUIDE.md)** — Current frontier: Fragment 62 battle *body*, `12D80.c` remaining stubs/structs.
* **[FRAGMENT_ROLES.md](FRAGMENT_ROLES.md)** — Overlay 1–77 roles from `GameState_*` / Kids Club / gallery load sites.

# Decomp Progress

Source recount: 2026-09-20. Full tables and the ordered work list are in **[UPSTREAM_AUDIT_AND_PLAN.md](UPSTREAM_AUDIT_AND_PLAN.md)**.

The repository uses [splat](https://github.com/ethteck/splat) for ROM splitting and IDO 7.1 as the matching compiler. Game code is split into C translation units. A matching `make` (`NON_MATCHING=0`) is intended to reproduce the original ROM; that has **not** been re-verified in the current Cloud Agent workspace (no US 1.0 baserom).

### Code Segment Breakdown

| Area | Status |
|---|---|
| Named core C files (src root) | **22** TUs with descriptive filenames (`main`, `rsp`, `dma`, `dp_intro`, `memmap`, `memory_main`, `memory`, `util`, `reset`, `controller`, `crash_screen`, `profiler`, `math_util`, `hal_libc`, `gb_tower`, `gb_mbc`, `jpegutils`, `jpeg_decoder`, `geo_layout`, `stage_loader`, `heap`, `bss_pad`) |
| Hex-named src-root C files | **55** — many already have named *functions* (`Sched_*` in `5580.c`, `SceneGraph_*` in `12D80.c`, `GameState_*` in `29BA0.c`, …) but the files themselves are still address-named |
| Overlay fragments | **77 fragments**, **210 C files** — all split; roles catalogued in [FRAGMENT_ROLES.md](FRAGMENT_ROLES.md) |
| Nonmatching functions | **93** `GLOBAL_ASM` stubs in **38** C files (was documented as 174). Largest piles: fragment 1 GB emulator, `33FE0.c`, audio (`3D140.c` / `4A3E0.c`), fragment 23, fragment 62 |
| Libnaudio / libnumus / libleo | Present and compiling (`src/libnaudio`, `src/libnumus`, `src/libleo`) |
| Assets | Still **`bin`** — Yay0 / PRESJPEG / FRAGMENT magics are documented; no format extractors merged |

### Key Systems Status

| System | Status |
|---|---|
| Boot / entry / idle / `Game_Thread` | ✅ Named (`main.c`, `29BA0.c`) |
| Memory pool / allocation | ✅ Named (`memory.c`, `memory_main.c`, `heap.c`) |
| DMA | ✅ Named (`dma.c`; extra ROM DMA API still in `4B940.c`) |
| Math / matrix | ✅ Named (`math_util.c`, `F420.c`) |
| Controller / crash / reset / profiler | ✅ Named |
| Stage / overlay loader | ✅ Named (`stage_loader.c`); fragment reloc still hex-file (`19840.c`) |
| Scheduler / RSP tasks / VI / display lists | ⚠️ Functions named (`Sched_*`, `RSPTask_*`, `DLBuf_*`, `VI_SetMode`); TUs still hex (`5580.c`, `6A40.c`, `dp_intro.c`) |
| Audio | ⚠️ Split API: `Audio_StartThread` (`DDC0.c`) vs `Audio_Init` / libnumus (`373A0.c`) |
| GB Tower / MBC | ✅ Named (`gb_tower.c`, `gb_mbc.c`, `E1C0.c` thread) |
| JPEG / Yay0 | ✅ Named (`jpegutils.c`, `jpeg_decoder.c`, `3FB0.c` stream parse, `51740.c` Yay0) |
| Scene graph | ⚠️ Traversal skeleton named in `12D80.c`; 2 `GLOBAL_ASM` left; node structs still `unk_*` |
| Battle engine (Fragment 62) | ⚠️ Shell named (`BattleScene_*` / `BattleTurn_*` / `BattleAI_*` / `BattleEvent_*` / `BattleEffect_*`); ~1926 `func_843*` defs remain |
| RSP microcode | ✅ Fixed F3DEX2 + JPEG `njpgdsp` + audio `aspMain`. No RSP overlays |
| Asset formats | ❌ Binary blobs |

---

## Next Phase: N64Recomp Readiness

Do **not** start Recomp TOML or asset extractors before a green matching `make`. Order of work is in **[UPSTREAM_AUDIT_AND_PLAN.md](UPSTREAM_AUDIT_AND_PLAN.md)**.

### Blockers (must fix first)

1. **Matching `make` with US 1.0 `baseroms/us/baserom.z64`.** Treat leftover IDO include failures as environment issues.

2. **Archive a known-good ELF.** N64Recomp uses the ELF, not the ROM. `KEEP_MDEBUG ?= 1` is already set. Keep `build/pokestadium-us.elf` + `.map` out of git.

3. **Keep `symbol_addrs` in sync.** `tools/sync_promoted_symbol_addrs.py`; preserve `orig:func_*`.

4. **Finish Fragment 62 / `12D80.c` mid-level names**, then rename the hex TUs that already have named APIs. See the [partial-systems guide](POKEMON_STADIUM_USA_PARTIAL_SYSTEMS_BLOCKER_GUIDE.md) and [fragment roles](FRAGMENT_ROLES.md).

5. **Knock down `GLOBAL_ASM` starting with the two `12D80.c` stubs**, then fragment 62, then fragment 1 / `33FE0.c` / audio.

### Stadium-specific (recomp)

- Overlays: 77 relocatable fragments; load funnel is documented. N64Recomp supports this; keep relocation/load behavior intact.
- RSP: no overlay-style RSP loads (see the audit). N64Recomp’s RSP-overlay gap does not apply.
- Runtime glue still needed on the project side: VI/RDP output, input callbacks, audio output. N64ModernRuntime covers threads, queues, PI DMA, overlays.

### Nice-to-have (after P0–P1)

- Rename hex TUs (`5580.c` → `sched.c`, `12D80.c` → `scene_graph.c`, …).
- Script: overlay bounds + named functions → first-pass N64Recomp TOML.
- Asset format notes beyond the existing Yay0 / PRESJPEG / FRAGMENT magics.

---

# Prerequisites

Under Debian / Ubuntu (which we recommend using), you can install them with the following commands:

```bash
sudo apt update
sudo apt install make git build-essential binutils-mips-linux-gnu python3 python3-pip python3-venv
```

**Please also ensure that the Python version installed is >3.7.**

The build process has a few python packages required that are located in `requirements.txt`.

To install them simply run in a terminal:

```bash
python3 -m pip install -r requirements.txt
```

# To use
1. Place the US Pokemon Stadium 1.0 rom into the repository's "/baseroms/us/" folder as "baserom.z64".
2. Set up tools and extract the rom: `make init`
3. Re-assemble the rom: `make`

For contacts and other pret projects, see [pret.github.io](https://pret.github.io/).

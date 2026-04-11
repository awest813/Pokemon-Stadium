# Pokemon Stadium (US)
A WIP decomp of Pokemon Stadium (US).

It builds the following ROMs:

* pokestadium.z64: `md5: ed1378bc12115f71209a77844965ba50`

Note: To use this repository, you must already have a rom for the game.

# Decomp Progress

## Current State

The repository uses [splat](https://github.com/ethteck/splat) for ROM splitting and IDO 7.1 as the matching compiler.  The ROM is fully split into C translation units — no raw asm blobs remain for game code — and the build reproduces the original binary byte-for-byte when `NON_MATCHING=0`.

### Code Segment Breakdown

| Area | Status |
|---|---|
| Named core C files (src root) | **24** fully-named translation units — `main`, `rsp`, `dma`, `dp_intro`, `memmap`, `memory_main`, `memory`, `util`, `reset`, `controller`, `crash_screen`, `profiler`, `math_util`, `hal_libc`, `gb_tower`, `gb_mbc`, `jpegutils`, `geo_layout`, `stage_loader`, and several others |
| Anonymous main-text C units | **49** units still named only by ROM address (e.g. `3FB0.c`, `5580.c`) — content is split correctly but function/purpose not yet identified |
| Address-named C files (src root) | **56** files with hex-address names — split and decompiled but not yet renamed |
| Overlay fragments | **77 fragments** (fragment01–fragment77) across **209 C files** — all split and compiled, none yet given descriptive names |
| Nonmatching functions | **174** `NONMATCH`/`GLOBAL_ASM` stubs across **56 C files** — these compile and link but do not yet match the original assembly |
| Libnaudio (audio driver) | **28 C files** present and compiling |
| Libleo (64DD driver) | **37 C files** present and compiling |
| Assets | All asset blocks (textures, models, UI, sound, GB Tower ROMs) remain as **`bin`** — no asset format decomp has started |

### Key Systems Status

| System | Status |
|---|---|
| Boot / entry / idle thread | ✅ Named and documented |
| Memory pool / allocation | ✅ Named (`memory.c`, `memory_main.c`) |
| DMA transfers | ✅ Named (`dma.c`) |
| Math / matrix utilities | ✅ Named (`math_util.c`, `F420.c`) |
| Controller | ✅ Named (`controller.c`) |
| Crash screen | ✅ Named (`crash_screen.c`) |
| Soft reset | ✅ Named (`reset.c`) |
| Stage / overlay loader | ✅ Named (`stage_loader.c`) |
| RSP task dispatch (graphics) | ✅ Named (`rsp.c`, `main.c`) |
| GB Tower / MBC | ✅ Named (`gb_tower.c`, `gb_mbc.c`) |
| Yay0 / JPEG decoders | ✅ Named (`jpegutils.c`, hasm stubs) |
| Profiler | ✅ Named (`profiler.c`) |
| Geometry / scene graph | ✅ Named (`geo_layout.c`) |
| Scheduler / audio manager start | ⚠️ Split but still unnamed (`6A40.c` area) |
| Fragment load/unload engine | ⚠️ Split across fragments, relocation understood, not yet renamed |
| Battle / minigame logic | ⚠️ Unnamed fragments — code is decompiled, purpose not documented |
| Asset formats (textures, models, sound) | ❌ All binary blobs — no extraction tooling merged yet |

---

## Next Phase: N64Recomp Readiness

The decomp is structurally ready to begin preparing for [N64Recomp](https://github.com/N64Recomp/N64Recomp).  The following items must be completed in order:

### Blockers (must fix first)

1. **Restore a clean `make` path.**  
   The IDO compile currently fails on at least one translation unit (`src/19840.c`) while trying to open `lib/ultralib/include/PR/gbi.h`.  A verified byte-identical `make` with no errors is required before any recomp work can start.

2. **Produce and archive a known-good ELF.**  
   N64Recomp uses the ELF (not the raw ROM) as its metadata source.  `KEEP_MDEBUG ?= 1` is already set in the Makefile, which preserves debug info.  Once `make` is clean, archive `build/pokestadium-us.elf` and verify the symbol table is complete.

3. **Name the 49 anonymous main-text C units.**  
   Anonymous symbols degrade N64Recomp output quality and make patch work painful.  Priority targets: the scheduler, audio-manager startup, and the remaining scene-dispatch code.

4. **Resolve 174 nonmatching functions.**  
   Each `NONMATCH` stub means the corresponding symbol may not survive cleanly into the ELF.  Convert easy ones to normal C first; use `NON_MATCHING=1` build as a fallback only for the hardest cases.

### Stadium-Specific Concerns

5. **Fragment / overlay audit.**  
   All 77 fragments use N64-style relocatable overlays.  N64Recomp supports statically linked and relocatable overlays, but fragment load/unload logic and relocation tables need to be clearly understood and documented before recomp patching can begin.

6. **RSP microcode audit.**  
   The repo uses fixed RSP microcode blobs (`rsp.c`, `main.c`).  Confirm that no overlay-style RSP loading occurs — N64Recomp does not yet support RSP overlays.

7. **Runtime glue planning.**  
   `N64ModernRuntime` covers threads, controllers, audio queues, VI timing, PI DMA, and overlay handling — most of Stadium's OS usage is covered.  Project-side glue still needed: renderer setup (VI/RDP output), input callbacks, and audio output driver.

### Nice-to-Have (after blockers cleared)

- Name all 77 overlay fragments by game system (battle, minigame type, etc.).
- Add a script to generate a first-pass N64Recomp TOML and symbol dump from the clean ELF.
- Begin asset format documentation (Yay0-compressed textures, PRESJPEG, FRAGMENT header format).

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

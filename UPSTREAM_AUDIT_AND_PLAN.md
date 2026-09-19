# Upstream catch-up, audit, and improvement plan

Last updated: 2026-09-19  
Upstream: [pret/pokestadium](https://github.com/pret/pokestadium) `0b614c2`  
This fork: `awest813/Pokemon-Stadium`

This document records what pret landed after this fork diverged, how that interacts with the local RE/naming work, and a concrete order of next improvements.

---

## 1. What this merge brought in

The fork had been based at pret `756f7e3` (jpeg-utils match). Pret then landed **97 commits / ~40 PRs**. This branch merges that history and keeps this repo’s named APIs where they already exist.

### pret work that matters most

| Area | What pret did | Why it matters here |
|---|---|---|
| Matching C | Many `func_*` matches: `19840`, `1CF30`, `33FE0`, `30640`, `334D0`, `435D0`, `48C60`, fragment 1/4/8/23/31, GB MBC, `amCreateAudioMgr`, etc. | Byte-identical ROM is the decomp source of truth. Prefer pret *control flow* over local rewrite when they conflict. |
| BSS / data maps | `6A40`, `dp_intro`, `3D140`, `4A3E0`, fragment 25/30/31/35/62, libultra, libleo | Overlay and `.bss` layout must stay explicit for matching *and* for N64Recomp ELF metadata. |
| Audio | Imported **libnumus** (`MusInitialize`, `MusBankInitialize`, `audio_heap`) | The real audio driver is no longer a pile of `func_80038xxx` stubs. |
| JPEG | Dedicated `jpeg_decoder.c` + stream marker parse in `3FB0.c` | Unblocks RSP JPEG task understanding. |
| Tooling | Removed the `tools/n64splat` git submodule; splat is `splat64` from `requirements.txt`. macOS archiver fix. | `make extract` no longer needs a splat submodule clone. |

### How conflicts were resolved

Overlapping files were almost always **pret matching C + this fork’s symbol names**.

Examples:

- `DLBuf_*` stays (not `func_80005F5C`), but `6A40.c` now *defines* `.bss` (`gDisplayListHead`, `D_800A7428`, `D_800A7440`) the way pret mapped it.
- `Display_*` / `Sched_*` / `GBTower_*` / `Archive_*` names stay.
- Fragment 8 keeps `RattataMinigame_*` entrypoints on top of pret’s matched hurdle math.
- Audio bank load is pret’s `MusInitialize` path; the idle-thread starter is named `Audio_StartThread` (`func_8000D564`) so it no longer collides with `Audio_Init` in `373A0.c`.
- `gScheduler` is an alias of `D_800A62E0`. Call sites may use either.

**Not verified in this environment:** a matching `make` (no US baserom in the workspace). That is still the first machine check after merge.

---

## 2. Audit: fork vs pret vs the existing RE notes

### 2.1 What the fork already did well

The local work is a **symbolic / systems map**, not a matching-first tree:

- Named boot, DMA, memory pools, scheduler, display, GB Tower/Pak, archive/fragment loader, Kids Club minigame shells, and a first pass on battle actors (`BattleActorState` / `BattleMove`).
- Scene-graph promotion in `12D80.h` (`GraphNode_Process*`, `Renderer_*`) — this is exactly the “mid-level names” the [partial-systems blocker guide](POKEMON_STADIUM_USA_PARTIAL_SYSTEMS_BLOCKER_GUIDE.md) asked for.
- Docs: `AI_MIPS_HEADER_GUIDE.md`, blocker guide, README recomp roadmap.

That is the right contribution *on top of* pret. It should not be thrown away for address names.

### 2.2 What the fork got wrong or left unsafe

1. **Renames without `symbol_addrs` updates.**  
   C functions are `DLBuf_Init` / `JPEG_Decompress` / … while `linker_scripts/us/symbol_addrs_code.txt` still lists `func_80005E40` / `func_80003680`. Matching bytes can still work; maps, diffs, and N64Recomp symbol quality will not. This is now the highest-leverage cleanup.

2. **Duplicate `Audio_Init`.**  
   Both `func_8000D564` (start audio thread) and `func_800373D8` (load banks / `MusInitialize`) were named `Audio_Init`. Idle boot must call the thread starter. This merge splits them: `Audio_StartThread` vs `Audio_Init`.

3. **Incomplete type promotion.**  
   `fragment8.c` used `RattataPlayerState` without a header typedef (now aliased). `DisplayCtx` padding was guessed; pret’s field layout (mesg queues, nested `FrameConfig`s, embedded `RSPTask`) is more accurate and is what we kept.

4. **Matching-hostile rewrites.**  
   Local `func_80018C40` indexing vs pret’s pointer increment is the classic IDO hazard. When pret has a match, take pret’s shape and only rename identifiers.

5. **Docs vs tree.**  
   README still listed `src/19840.c` / `gbi.h` as a hard `make` blocker and “174 NONMATCH stubs”. Pret has since matched `19840` and many other TUs. Counts and blockers need a living progress command, not a frozen table.

6. **Fragment 62 is still the battle-shell blocker.**  
   pret mapped fragment 62 *data*; this fork started effect/actor comments. Neither side has a structural `BattleScene_` / `BattleTurn_` / `BattleAI_` pass on the fragment 62 C files. The blocker guide is still the correct next RE task.

7. **`12D80.c` is only half-promoted.**  
   Headers have GraphNode buckets; many helpers remain `func_80014xxx`. Traversal vs node-type vs renderer submit is still mixed.

### 2.3 N64Recomp / README blockers, re-scored

| Old README blocker | Status after pret merge |
|---|---|
| Clean `make` / `19840.c` + `gbi.h` | pret matched `19840`. Re-verify `make` with a US 1.0 ROM. Treat remaining IDO include issues as environment, not unknown C. |
| Archive a known-good ELF | Still required. `KEEP_MDEBUG ?= 1` is already on. Do this immediately after a green matching build. |
| Name anonymous main-text TUs | Partially done here (scheduler/display/audio/GB). Dozens of address-named files remain. |
| ~174 NONMATCH/GLOBAL_ASM | pret reduced this (notably `33FE0`, `30640`, `334D0`, fragments 1/8/31, MBC). Re-count from `progress.py` after extract. |
| Fragment/overlay audit | Relocation + `FRAGMENT` magic are understood; fragment *roles* are not catalogued. |
| RSP microcode audit | Still undone. Need to confirm only fixed F3DEX2 (+ JPEG ucode) blobs, no RSP overlays. |
| Runtime glue | Unchanged: renderer/input/audio output are project-side after a clean ELF. |

---

## 3. Improvement plan (do in this order)

### P0 — Make the merged tree honest and buildable

1. **Matching build.** Place US 1.0 `baseroms/us/baserom.z64`, `make init`, `make`. Diff any non-match against pret’s current objects before changing names further.
2. **One symbol policy.** For every promoted C name, add the same name (or a splat alias) in `symbol_addrs_code.txt` / `symbol_addrs.txt` at the original address. Stop leaving `func_*` as the only linker identity.
3. **Document the split audio API** in a one-line comment at `Audio_StartThread` / `Audio_Init` so it is not re-merged by accident.

### P1 — Finish the two systems the blocker guide named

Follow [POKEMON_STADIUM_USA_PARTIAL_SYSTEMS_BLOCKER_GUIDE.md](POKEMON_STADIUM_USA_PARTIAL_SYSTEMS_BLOCKER_GUIDE.md) *without* inventing mechanics:

1. **Fragment 62 structural map** (10–25 functions): entry, substate dispatch, turn order, command exec, AI-shaped helpers, result/exit. Prefixes: `BattleScene_`, `BattleTurn_`, `BattleAI_`, `BattleEvent_`, `BattleResult_`. Keep `func_86xxxxx` in comments.
2. **`12D80.c` traversal skeleton:** root traverse, node-type switch, child/sibling walk, matrix push/pop, callback / DL submit. Do not put battle words on generic graph nodes.
3. Only then name shared context fields (`BattleActorState` members, graph-node structs) that those passes actually touch.

### P2 — Overlay / RSP / ELF (recomp path)

1. **Fragment catalog.** Table of fragment 1–77: load site, game state (`STATE_*`), likely role (menu, battle shell, minigame, lab, GB tower). Start from the dispatcher the blocker guide already lists (62/63/64).
2. **RSP audit.** Trace `rsp_init` / `RSPTask_*` / JPEG task. Confirm no overlay-style RSP loads. Write the finding into this file or the recomp notes.
3. **ELF snapshot.** After P0, keep `build/pokestadium-us.elf` + `.map` as artifacts (not in git). Optional: a small script that dumps overlay bounds + named functions into a first-pass N64Recomp TOML.

### P3 — Matching debt (easy wins first)

Pret’s recent style is: match a whole file’s remaining `GLOBAL_ASM`, then map its `.bss`. Continue that, preferring files this fork already named:

- Remaining `12D80.c` NONMATCH (scene graph).
- Remaining fragment 62 NONMATCH (battle shell).
- `4A3E0.c`, `fragment1_7F9A0.c` / `86CB0.c` (still high stub counts).
- Do **not** rewrite matched functions to “cleaner C” unless `diff.py` still matches.

### P4 — Hygiene (parallel, low risk)

- Generate README progress from `progress.py` instead of hand-edited tables.
- Align `oldnotes/decomp_orientation.md` with current names (`VI_SetMode`, `Sched_Init`, `Audio_StartThread`, `Game_Thread` in `main.c`).
- Kids Club: keep `RattataMinigame_*` / shared `Minigame_*` / `Stage_*` consistent across fragments 7–9, 16, 28; add header typedefs when `.c` uses a promoted struct name.
- Assets stay `bin` until code naming is stable; Yay0 / PRESJPEG / FRAGMENT headers are already sketched in `3FB0.h`.

---

## 4. What not to do

- Do not rename libnumus / libultra / libleo symbols away from pret/upstream names.
- Do not jump from `func_86xxxxx` to move-effect-specific names inside fragment 62.
- Do not treat this fork as something to upstream wholesale: pret wants matches and conservative names. Offer *matches* upstream; keep *maps and docs* here until they are proven.

---

## 5. Suggested next agent task

```text
After a matching make (US 1.0 ROM):
1) Sync symbol_addrs_code.txt with current C function names (aliases at original VAs).
2) Structural naming pass on fragment 62 + remaining 12D80.c helpers per
   POKEMON_STADIUM_USA_PARTIAL_SYSTEMS_BLOCKER_GUIDE.md.
3) Write a fragment 1–77 role table from the game-state dispatcher.
```

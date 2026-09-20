# Upstream catch-up, audit, and major-gaps plan

Last updated: 2026-09-20  
Upstream: [pret/pokestadium](https://github.com/pret/pokestadium) `0b614c2`  
This fork: `awest813/Pokemon-Stadium` @ `9fe0bde`

This is the living audit. Counts below are from **source**, not `progress.py` (that script needs a matching map file from `make`). Recheck after extract.

---

## 0. What changed since the last audit

The previous pass (PR #2) imported pret matching C (squash, not a git merge) and named the scene-graph processors plus the Fragment 62 battle *shell*. Docs were not recounted.

This pass:

- Recounted `GLOBAL_ASM`, named vs hex TUs, and Fragment 62 / `12D80.c` promotion.
- Closed the leftover P0 audio-API comment (`Audio_StartThread` vs `Audio_Init`).
- Removed duplicate `#include`s, fixed `unk_D_800ABB28` field offsets.
- Wrote the RSP finding (no overlay-style RSP loads).
- Filled remaining first-pass fragment IDs in [FRAGMENT_ROLES.md](FRAGMENT_ROLES.md).
- Replaced stale README / blocker / orientation numbers with the tables below.

---

## 1. Honest current state

### Matching / split

| Item | Count | Notes |
|---|---|---|
| `GLOBAL_ASM` stubs in `src/` | **93** in **38** files | README previously said 174. pret HEAD has the same high-count files (`33FE0` = 8, `fragment1_7F9A0` = 15). |
| Overlay fragments | **77** dirs, **210** C files | All split. Roles: [FRAGMENT_ROLES.md](FRAGMENT_ROLES.md). |
| Named src-root `.c` files | **22** | `main`, `rsp`, `dma`, `dp_intro`, `memmap`, `memory_main`, `memory`, `util`, `reset`, `controller`, `crash_screen`, `profiler`, `math_util`, `hal_libc`, `gb_tower`, `gb_mbc`, `jpegutils`, `jpeg_decoder`, `geo_layout`, `stage_loader`, `heap`, `bss_pad` |
| Hex-named src-root `.c` files | **55** | Many already have named *functions* (see P2 file-rename list). |

Highest remaining `GLOBAL_ASM` files:

| Stubs | File | Likely system |
|---|---|---|
| 15 | `fragments/1/fragment1_7F9A0.c` | GB Tower emulator |
| 8 | `fragments/1/fragment1_86CB0.c` | GB Tower emulator |
| 8 | `33FE0.c` | Main-text (pret also still has these 8) |
| 5 | `3D140.c` | Audio manager (`amCreateAudioMgr`) |
| 4 | `4A3E0.c` | Audio / synthesis adjacent |
| 3×3 | fragment 23 `1A9780` / `1AE680` / `1B4EA0` | Lab / GB-Tower shell |
| 2 | `12D80.c` | `func_80012870` (vtx helper), `GraphNode_ProcessLight` (`func_80013D34`) |
| 11 total | Fragment 62 (8 files) | Battle shell leftovers |

### Symbolic promotion

| System | Status |
|---|---|
| Boot / idle / `Game_Thread` | Done (`main.c`, `29BA0.c`) |
| Scheduler / RSP task / VI / display | Functions named; TUs still hex (`5580.c`, `dp_intro.c`, `6A40.c`, `6BC0.c`) |
| Audio thread vs bank init | Split: `Audio_StartThread` (`DDC0.c`) vs `Audio_Init` (`373A0.c` + libnumus) |
| Scene graph traversal (`12D80.c`) | **Skeleton done.** ~105 named processors / renderer helpers. 2 `GLOBAL_ASM`. Structs still `unk_*`. |
| Fragment 62 battle shell | **Shell done** (~86 named defs): `BattleScene_*`, `BattleTurn_*`, `BattleAI_*`, `BattleEvent_*`, `BattleEffect_*`. **~1926 `func_843*` defs remain.** |
| Overlay catalog | First pass complete, including 24/29/30/48/51/52. |
| RSP microcode | **Audited.** Fixed F3DEX2 + `njpgdspMain` + `aspMain` + `rspboot`. No RSP overlays. |
| Assets | All `bin`. Yay0 / PRESJPEG / FRAGMENT magics documented in `3FB0.h` / orientation notes. |
| Matching `make` | **Not verified here** (no US 1.0 `baserom.z64`). |

### pret vs this fork

Content is at pret `0b614c2` for matching C, brought in via squash (`9fe0bde`), **not** as git merge parents. `git merge-base HEAD pret/master` is still `756f7e3`.

Consequences:

- File-level matching debt matches pret on the big stub files.
- Future pret catch-up cannot be a clean `git merge`; rebase/cherry-pick or a one-off merge with rename conflicts.
- Keep this fork’s *names* (`GraphNode_*`, `BattleScene_*`, `DLBuf_*`, …) on top of pret *control flow*.

---

## 2. RSP audit (P2 from last plan — closed)

Task setup sites:

| Ucode | Where assigned | Role |
|---|---|---|
| `rspboot` | `dp_intro.c` `RSPTask_Init`, `3FB0.c` JPEG task, `3D140.c` audio task | Boot |
| `F3DEX2` (`_binary_assets_us_F3DEX2_bin_start` + `F3DEX2_data_bin`) | `dp_intro.c` | Graphics |
| `njpgdspMainTextStart` / `DataStart` | `3FB0.c` `RSPTask_InitJpeg` | JPEG decode |
| `aspMainTextStart` / `DataStart` | `3D140.c` audio mgr | Audio |

All are static asset blobs. `osSpTaskLoad` / `osSpTaskStartGo` in `5580.c` run whatever `OSTask` the game queued. No fragment-style RSP relocator.

**N64Recomp implication:** the “RSP overlays not supported” limitation does not block Stadium.

---

## 3. Major gaps (do in this order)

### P0 — Prove the tree still matches

1. Place US 1.0 `baseroms/us/baserom.z64` (`md5 ed1378bc12115f71209a77844965ba50`).
2. `make init && make` with `NON_MATCHING=0`.
3. If a TU fails, diff against pret’s object *before* changing names.
4. Archive `build/pokestadium-us.elf` + `.map` out of git. `KEEP_MDEBUG ?= 1` is already on.

Without this, every later rename is guesswork against an unproven link.

### P1 — Finish the two named systems (structure, not mechanics)

[Blocker guide](POKEMON_STADIUM_USA_PARTIAL_SYSTEMS_BLOCKER_GUIDE.md) still applies, but the *frontier moved*.

**`12D80.c` (almost unblocked)**

- Match `GraphNode_ProcessLight` (`func_80013D34`) and `func_80012870`.
- Name `func_80012960` (vertex generator used by the texture-primitive path).
- Then — and only then — promote node structs (`unk_D_86002F34_*`, `unk_D_800ABB10` / `28`) from fields the processors actually read.

**Fragment 62 (shell done, body is the gap)**

Named already: overlay entry / tick / frame loop, substate dispatch, turn build/execute, AI RNG + damage sim + `BattleAI_ChooseMove`, event queue/script, a large `BattleEffect_*` set.

Do **not** invent move names for remaining `func_843*`. Next 10–25 names, by file:

| File | Next bucket | Why |
|---|---|---|
| `fragment62_315D50.c` | Remaining `BattleEvent_*` opcode *handlers* (keep numeric ids; `OpenNop` / `CloseNop` already mark unused slots) | Script VM is the presentation glue |
| `fragment62_3020D0.c` | Helpers around `BattleTurn_BuildOrder` / `Battle_QueueTurnMessage` | Turn-order internals |
| `fragment62_359F90.c` | Helpers around `BattleTurn_Execute` / `BattleTurn_CheckInterrupts` | Action execution |
| `fragment62_361050.c` | Rest of AI after `BattleAI_ChooseMove` (two `GLOBAL_ASM` left) | Decision vs simulation |
| `fragment62_2EA8E0.c` | Overlay/setup leftovers only | Entry is already named |

Leave `fragment62_3055E0.c` (huge actor/camera/geo helpers) until the turn/event/AI buckets are readable. Prefixes stay `BattleScene_` / `BattleTurn_` / `BattleAI_` / `BattleEvent_` / `BattleResult_`. Keep `func_843xxxxx` in comments.

### P2 — Rename TUs that are already symbolically identified

These hex files already export named APIs. Renaming the *file* (yaml + includes + splat) is the highest-leverage map cleanup after P0:

| Current | Named API | Suggested stem |
|---|---|---|
| `12D80.c` | `SceneGraph_*` / `GraphNode_*` / `Renderer_*` | `scene_graph` |
| `11BA0.c` | `GraphNode_Init` / `Camera_Set*` | `graph_node` |
| `5580.c` | `Sched_*` / `RSPTask_*` | `sched` |
| `6A40.c` | `DLBuf_*` | `dl_buf` |
| `6BC0.c` | `ColorBuffer_*` / `GFX_SetScissor` | `color_buffer` |
| `DDC0.c` | `Audio_StartThread` / `AudioThread_Main` | `audio_thread` |
| `373A0.c` | `Audio_Init` | `audio` |
| `29BA0.c` | `Game_Thread` / `GameState_*` | `game` |
| `19840.c` | `FragmentLoader_InitThread` | `fragment_loader` |
| `3FB0.c` | `JpegStream_*` / `RSPTask_InitJpeg` | `jpeg` |
| `2FEA0.c` | `KidsClub_Dispatch` | `kids_club` |
| `E1C0.c` | `GBTower_Start` | `gb_tower_thread` (file is not `gb_tower.c`) |
| `4B940.c` | `Dma_Init` / `Dma_ROMRead` | already next to `dma.c` — confirm role before merging |
| `51740.c` | `Yay0_Decompress` | `yay0` |
| `F420.c` | `Vec3f_*` / `MtxF_*` | `mtx` / keep adjacent to `math_util.c` |

One rename per PR, splat yaml + `#include`s + `symbol_addrs` together. Re-run `tools/sync_promoted_symbol_addrs.py` after each.

### P3 — Matching debt (easy files first)

Prefer files this fork already named, then pret’s remaining stubs:

1. `12D80.c` (2 stubs) — unblocks P1 structs.
2. Fragment 62’s 11 stubs.
3. `4A3E0.c` (4), `3D140.c` (5).
4. Fragment 1 GB emulator (`7F9A0` + `86CB0`) — largest remaining C stubs; isolate from battle naming.
5. Fragment 23 lab shell.

Do **not** rewrite a matched function to “cleaner C” unless `diff.py` still matches.

### P4 — Recomp path (after a green `make`)

1. Keep the ELF + map as build artifacts.
2. Small script: overlay VRAM bounds from `yamls/us/rom.yaml` + named functions from the map → first-pass N64Recomp TOML.
3. Runtime glue is still project-side (VI/RDP output, input, audio out). N64ModernRuntime covers threads / queues / DMA / overlays.
4. Assets stay `bin` until code names stabilize. Document Yay0 / PRESJPEG / FRAGMENT only; do not start a format decomp in parallel with P1.

### P5 — Hygiene (parallel, low risk)

- Keep this file’s tables in sync when you add `GLOBAL_ASM` or a file rename. Do not hand-edit README counts without recounting.
- `oldnotes/decomp_orientation.md` and `oldnotes/recomp64_readiness.md` are cheat-sheets; they must not contradict `main.c` names.
- Kids Club: keep `RattataMinigame_*` / `Minigame_*` / `Stage_*` consistent; add header typedefs when `.c` uses a promoted struct.
- Add `pret` as a remote and document squash-vs-merge so the next catch-up is intentional.

---

## 4. What not to do

- Do not rename libnumus / libultra / libleo away from pret names.
- Do not jump from `func_86xxxxx` / `func_843xxxxx` to move-effect-specific names.
- Do not treat this fork as something to upstream wholesale: offer *matches*; keep *maps and docs* here until proven.
- Do not start N64Recomp TOML or asset extractors before P0 (green matching `make`).
- Do not `git merge pret/master` without a dedicated conflict plan; history is squash-imported.

---

## 5. Suggested next agent task

```text
Requires US 1.0 baserom (not in this workspace):
1) make init && make; archive pokestadium-us.elf + .map
2) Match 12D80.c GLOBAL_ASM (func_80012870, GraphNode_ProcessLight)
3) Name 10–25 remaining Fragment 62 functions in
   fragment62_315D50.c (event opcode handlers, keep ids) and
   fragment62_3020D0.c / fragment62_359F90.c (turn helpers)
4) One TU rename from the P2 table (suggest 5580.c -> sched.c)
   with yaml + includes + symbol_addrs sync
```

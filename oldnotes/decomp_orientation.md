# Pokémon Stadium Decomp – Orientation & Operating Notes

Last updated: 2026-09-20
Cheat-sheet rules active.

## 1. Boot Sequence (Confirmed)

```
ROM entry ? Main() [main.c]
  osInitialize / osCartRomInit
  Util_InitMainPools()
  ? Idle_ThreadEntry()
      VI_SetMode()
      crash_screen_init()
      rsp_init()
      Sched_Init()            /* 5580.c */
      Audio_StartThread()     /* DDC0.c — thread only; banks are Audio_Init in 373A0.c */
      Display_Init()
      SoftReset_CreateThread()
      ? Game_Thread (Thread 6)  /* 29BA0.c */
```

Do not re-merge `Audio_StartThread` and `Audio_Init`.

## 2. Key Recovered Structs

`unk_func_80007444` = RenderContext
  width, height, scissor coords, fullres_flag, num_buffers, depth_flag
  fade_mode (0=off 1=solid 2=fadeout 3=fadein), fade_duration, fade_counter
  fill_color (RGBA16), buf_index, ColorBuffer* bufs[3]

`unk_D_80068BB0` = ColorBuffer
  fmt, size, width, height, img_p (ALIGN64), depth_p

Asset magic words:
  PERS-SZP = Yay0-compressed
  PRESJPEG = JPEG texture
  FRAGMENT = Overlay fragment

## 3. Priority Attack

See [UPSTREAM_AUDIT_AND_PLAN.md](../UPSTREAM_AUDIT_AND_PLAN.md). Short version:

1. Matching `make` with US 1.0 baserom (P0).
2. Fragment 62 body + `12D80.c` remaining stubs/structs (P1).
3. Rename hex TUs that already have named APIs (`5580.c` ? sched, `12D80.c` ? scene_graph, …).

## 4. Red Lines

Do NOT casually touch: scheduler matching control flow, libnumus / libnaudio, DMA irq, save data, volatile accesses, GB emu internals (`fragments/1`). Prefer pret shape when a function already matches.

## 5. Recomp Status

Done: boot path, memory model, segment model, asset magics, framebuffer/fade, GameState enum, RSP dispatch, RSP ucode audit (fixed F3DEX2 + JPEG + aspMain, no RSP overlays), overlay role catalog, scene-graph *skeleton*, battle *shell*.

TODO: green matching ELF, Fragment 62 body names, graph-node structs, hex TU renames, N64Recomp TOML from ELF.

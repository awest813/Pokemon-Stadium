# Pokémon Stadium Decomp – Orientation & Operating Notes

Last updated: 2026-04-03
Cheat-sheet rules active.

## 1. Boot Sequence (Confirmed)
ROM entry ? Main() [main.c]
  osInitialize / osCartRomInit
  Util_InitMainPools()
  ? Idle_ThreadEntry()
      func_80001474()          <- VI mode setup (UNNAMED)
      crash_screen_init()
      rsp_init()
      func_800052B4()          <- unknown init (scheduler?)
      func_8000D564()          <- unknown init (controller?)
      func_800019C8()          <- unknown init (DMA queue?)
      SoftReset_CreateThread()
      ? Game_Thread (Thread 6) <- MAIN GAME THREAD (NOT YET LOCATED)

## 2. Key Recovered Structs
unk_func_80007444 = RenderContext
  width, height, scissor coords, fullres_flag, num_buffers, depth_flag
  fade_mode (0=off 1=solid 2=fadeout 3=fadein), fade_duration, fade_counter
  fill_color (RGBA16), buf_index, ColorBuffer* bufs[3]

unk_D_80068BB0 = ColorBuffer
  fmt, size, width, height, img_p (ALIGN64), depth_p

Asset magic words:
  PERS-SZP = Yay0-compressed
  PRESJPEG = JPEG texture
  FRAGMENT = Overlay fragment

## 3. Priority Attack
Tier 1: Name functions in stage_loader.c (fade overlay, frame begin/end, alloc screen)
Tier 2: Locate Game_Thread
Tier 3: Finalize RenderContext / ColorBuffer typedefs
Tier 4: Orient one fragment overlay to a GameState

## 4. Red Lines
Do NOT casually touch: scheduler, audio (libnaudio/), DMA irq, save data, volatile accesses, GB emu internals

## 5. Recomp Status
Done: boot path, memory model, segment model, asset formats, framebuffer, fade system, GameState enum, RSP dispatch
TODO: Game_Thread, scene dispatcher, gfx task flow, audio task flow, overlay load/unload

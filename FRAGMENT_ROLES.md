# Fragment roles (first pass)

Sources:

- `Game_Thread` / `GameState_*` in `src/29BA0.c`
- `KidsClub_Dispatch` in `src/2FEA0.c`
- Gallery loop `func_8002FA40` in `src/30640.c`

Confidence: **Verified** where a load sits in a named `GameState_*` or named dispatcher; **Likely** where the fragment is only a shared helper loaded from many overlays.

| Fragment | Role | Evidence |
|---|---|---|
| 1 | GB Tower emulator overlay | `E1C0.c` |
| 2 | Shared UI / common overlay | Loaded with 3/34 around menus; Kids Club start |
| 3 | Shared UI / common overlay | Kids Club minigames 9+; title demos |
| 4 | Kids Club helper (group A) | Before fragments 6–9 |
| 5 | Kids Club helper (group B) | Before fragments 10–13, 16 |
| 6 | Magikarp minigame | `MagikarpMinigame_Entry` |
| 7 | Clefairy minigame | `ClefairyMinigame_Entry` |
| 8 | Rattata minigame | `RattataMinigame_Entry` |
| 9 | Jigglypuff minigame | `JigglypuffMinigame_Entry` |
| 10 | Drowzee minigame | `DrowzeeMinigame_Entry` |
| 11 | Pikachu minigame | `PikachuMinigame_Entry` |
| 12 | Sample minigame 7 (model viewer) | `SampleMinigame7_Entry` |
| 13 | Sample minigame 8 (model viewer) | `SampleMinigame8_Entry` |
| 14 | Sushi-Go-Round minigame | `SushiGoRoundMinigame_Entry` |
| 15 | Snap mode | `SnapMode_Entry`; also gallery submenu |
| 16 | Kids Club credits / intro helper | `KidsClubCredits_Entry`; `Game_RunIntroSequence` |
| 17 | Demo cutscene | `DemoCutscene_Entry`; `Game_RunDemo` |
| 18 | Ekans minigame | `EkansMinigame_Entry` |
| 19 | Unused minigame | `UnusedMinigame_Entry` |
| 20 | Metapod minigame | `MetapodMinigame_Entry` |
| 21 | Sandshrew minigame | `SandshrewMinigame_Entry` |
| 22 | Lab / GB-Tower menu shell | Loads 23 / 28; `2E460.c`, fragment 30 |
| 23 | Lab / GB-Tower sub-shell | Loaded from fragment 22; loads 25/27 |
| 25–27 | Lab / GB-Tower helpers | Loaded from 22/23/24/28 |
| 28 | Lab / GB-Tower sub-shell | Loaded from fragment 22 |
| 31 | Shared 3D / stage helper | Loaded from many overlays (not a game state) |
| 32 | Helper called from `Game_Thread` | `FRAGMENT_LOAD_AND_CALL(fragment32, …)` |
| 33 | GB Tower gate / setup | `GameState_GBTower`; fragment 51 wrapper |
| 34 | Battle shared overlay | Loaded immediately before `BattleScene_Run` |
| 35 | N64 logo / intro shell | `GameState_Intro`, `GameState_FastIntro` |
| 36 | Title screen | `GameState_TitleScreen` |
| 37 | Area select | `GameState_AreaSelect` |
| 38 | Event Battle menu | `GameState_EventBattle` |
| 39 | Kids Club hub | `KidsClub_Dispatch` selection loop |
| 40 | Gym Leader Castle extra | After GLC battle if flags set |
| 41 | Menu helper | `2E460.c` |
| 42, 44–47, 49 | Gallery submenus | `func_8002FA40` (`GameState_Gallery`) |
| 43 | Shared submenu / gallery chrome | Gallery; `GameState_FastIntro` |
| 50 | Victory Palace | `GameState_VictoryPalace` |
| 53 | GB Tower session companion | `GameState_GBTower` before `GBTower_Start` |
| 54, 58–61 | Battle setup / rule shells | Gate `BattleShell_Begin` |
| 55 | Setup helper | Loaded from fragment 54 / 38 |
| 56 | Options | `GameState_Options` |
| 57 | Menu select | `GameState_MenuSelect` |
| 62 | **Battle scene / turn engine** | Overlay yaml name `BattleScene_Run`; C entry `BattleScene_OverlayEntry` (`func_84301A2C`). Geo callback `BattleScene_Run` is only substate 5. |
| 63 | Battle shell begin | `BattleShell_Begin` |
| 64 | Battle shell tick | `BattleShell_Tick` |
| 65 | Gym Leader Castle mid-step | `GymLeaderCastle_RunLoop` case 2 |
| 66 | Post-battle / results helper | After battle shell (Stadium) |
| 67 | Debug / stub overlay | `GameState_Debug`; 68–77 call it |
| 68–74, 76–77 | Thin wrappers | Each `FRAGMENT_LOAD_AND_CALL2(fragment67, …)` |
| 75 | Fast-intro helper | `GameState_FastIntro` with 34/35/43 |

Battle funnel (verified from Stadium / Free Battle / Vs Mewtwo / Battle Now / Event / Fast Battle / Gym Leader Castle):

```
BattleShell_Begin (frag 63)
  -> BattleShell_Tick (frag 64)
  -> fragment34 + BattleScene_Run (frag 62)
  -> BattleShell_Begin(2)  // teardown / results
```

Gym Leader Castle loop (verified): fragment 54 → 65 → 61, then the same battle funnel.

Kids Club funnel (verified): fragment 2 + 39 hub → helper 3/4/5 + minigame 6–21.

Unlisted numbers (24, 29, 30, 48, 51, 52, …) are still only known as load sites from other overlays. Do not fill them from file size.

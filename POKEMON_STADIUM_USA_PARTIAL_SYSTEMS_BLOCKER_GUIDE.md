# Pokémon Stadium USA RE Guide: Battle Engine (Partial), Scene Graph (Partial), Remaining Blocker

This guide is a focused reverse-engineering brief for the current Pokémon Stadium USA decomp state.

Counts and the ordered repo-wide plan live in **[UPSTREAM_AUDIT_AND_PLAN.md](UPSTREAM_AUDIT_AND_PLAN.md)**. This file is the *how* for the two remaining central systems.

It targets three active problem areas:

1. **Battle Engine (PARTIAL — shell named)**  
   Overlay entry, substate dispatch, turn build/execute, AI choose/simulate, event queue, and a `BattleEffect_*` set are named. The body of Fragment 62 is still ~1926 `func_843*` helpers.

2. **Scene Graph (PARTIAL — traversal named)**  
   `12D80.c` processors in `D_8006F0A4` are named (`GraphNode_Process*`, `SceneGraph_*`, `Renderer_*`). Two `GLOBAL_ASM` functions and the `unk_*` node structs remain.

3. **Remaining Blocker**  
   Mid-level names exist for the *skeleton*. Cleaner promotion is blocked by unnamed turn/event/AI internals (Fragment 62) and un-promoted graph-node structs (`12D80` / `11BA0`).

This guide is written for AI coding agents and human reverse engineers.

---

## 1. Scope statement

This is **not** a full engine map.

It is a **promotion guide** for the current frontier:

- **Fragment 62** as the likely core shared battle scene / turn-execution shell
- **`12D80.c`** as a scene-graph / render traversal hotspot
- the naming and structural passes needed to unblock cleaner decomp

Use this document when deciding:
- what to rename next
- what to leave address-based
- which functions deserve symbolic names first
- how to avoid over-naming or inventing semantics too early

---

## 2. Battle Engine (PARTIAL)

### Current status
The strongest currently exposed battle convergence point is **Fragment 62**.

Named already (do not redo): `BattleScene_OverlayEntry` / `Tick` / `FrameLoop` / `SubstateDispatch` / `Init` / `ResetState` / `SetupCamera` / `UpdateFrame`, `BattleTurn_BuildOrder` / `Execute` / `CheckInterrupts` / `CheckPostAction`, `BattleAI_ChooseMove` / damage-sim helpers, `BattleEvent_PlayScript` / `QueueOpen*` / `QueueClose*` / `OpenNop` / `CloseNop`, plus `BattleEffect_*` in `fragment62_35DF70.c`.

Multiple battle-capable flows eventually funnel into a stack that includes:
- `fragment63`
- `fragment64`
- `fragment62`

This happens in:
- Stadium menu battle flow
- Free Battle flow
- Vs. Mewtwo flow
- Battle Now flow
- Event Battle flow
- Fast Battle flow

That makes **Fragment 62** the highest-probability shared battle-scene block currently exposed by the dispatcher.

### Why Fragment 62 matters
The top-level game-state code repeatedly routes into `fragment62` after battle setup shells and before outcome/result handling. This strongly suggests Fragment 62 is not just a wrapper, but a central execution block for battle scene behavior.

### Current partial state
The move-effects side may already be well-promoted, but the following categories likely remain undernamed inside Fragment 62:

- battle AI selection logic
- turn-order / turn-phase control
- command dispatch
- target selection / action validation
- transition between battle substates
- battle result flag writes
- animation/event orchestration glued to turn resolution

### Promotion goal
The immediate goal is **not** “fully solve battle AI.”

The immediate goal is to perform a **symbolic naming pass** that separates the likely roles into readable buckets.

---

## 3. Fragment 62 naming-pass strategy

### Step 1: Treat Fragment 62 as a shell first
Do not assume every function in Fragment 62 is “AI.”

Start with neutral bucket names:
- `BattleScene_Run`
- `BattleScene_Init`
- `BattleScene_Update`
- `BattleScene_Draw`
- `BattleScene_Exit`
- `BattleScene_HandleResult`
- `BattleScene_AdvanceSubstate`

Then refine from there.

### Step 2: Split logic by control role
Every function in Fragment 62 should first be sorted into one of these categories:

#### A. State/substate control
Likely functions that:
- switch on a state field
- return small status codes
- call several helper phases in order
- write result bits or transition flags

Suggested naming:
- `BattleScene_SubstateDispatch`
- `BattleScene_SubstateAdvance`
- `BattleScene_CheckExit`
- `BattleScene_HandleOutcome`

#### B. Turn-order / phase control
Likely functions that:
- iterate over battlers
- compare speed/priority/order data
- build or consume command order
- repeatedly call command helpers

Suggested naming:
- `BattleTurn_BuildOrder`
- `BattleTurn_BeginPhase`
- `BattleTurn_AdvancePhase`
- `BattleTurn_ExecuteAction`
- `BattleTurn_FinishPhase`

#### C. AI decision logic
Likely functions that:
- inspect moves, targets, or battle context
- choose a move or action
- assign command bytes/struct fields
- branch heavily on status/battle conditions

Suggested naming:
- `BattleAI_ChooseAction`
- `BattleAI_ChooseMove`
- `BattleAI_SelectTarget`
- `BattleAI_ScoreAction`
- `BattleAI_FallbackAction`

Use low-confidence names at first.

#### D. Animation / event orchestration
Likely functions that:
- enqueue event-like behavior
- trigger effect helpers
- coordinate battle actions with presentation timing
- write flags consumed by rendering or UI shells

Suggested naming:
- `BattleEvent_BeginAction`
- `BattleEvent_Advance`
- `BattleEvent_QueueAnimation`
- `BattleEvent_HandleCompletion`

### Step 3: Preserve address identity
Use staged naming:
- `fragment62_func_XXXXXX`
- `BattleScene_func_XXXXXX`
- `BattleTurn_ExecuteAction`
- `BattleAI_ChooseAction`

Do not jump from raw address names to ultra-specific gameplay labels without evidence.

---

## 4. Battle Engine symbolic pass checklist

For every target function in Fragment 62, document:

### Minimum header
```c
/*
 * BattleScene_func_XXXXXXXX [proposed]
 *
 * Summary:
 *     Unknown battle-related helper inside Fragment 62.
 *
 * Verified:
 *     - Called from Fragment 62 battle path
 *     - Reads or writes shared battle context
 *
 * Likely:
 *     - AI / turn / event / state helper
 *
 * Unknown:
 *     - Exact phase ownership
 *     - Whether result is local or global
 */
```

### Questions to answer
1. Is it called once per battle, once per turn, or once per frame?
2. Does it return a status code or mutate a context struct in place?
3. Does it iterate battlers, commands, or events?
4. Does it write result/transition flags?
5. Is it decision logic, execution logic, or presentation glue?

### Good early categories
- `BattleScene_*`
- `BattleTurn_*`
- `BattleAI_*`
- `BattleEvent_*`
- `BattleResult_*`

### Unsafe early categories
- `AIUseSuperEffectiveMoveAgainstLowHPTarget`
- `ResolveParalysisSkipTurnLogic`
- `PlayCriticalHitCameraAnimation`

unless directly proven.

---

## 5. Fragment 62 practical workflow

### Best-first order
1. Identify the top-level Fragment 62 entrypoint.
2. Locate the highest-level loop or dispatch function.
3. Name the substate/phase controller.
4. Identify functions that:
   - build commands
   - choose actions
   - order battlers
   - execute actions
5. Separate AI-like helpers from event/render glue.
6. Only after that, promote specific mechanic names.

### Good outcome for this pass
At the end of the symbolic pass, Fragment 62 should be readable as:

- battle scene shell
- state / phase controller
- command generation
- AI selection helpers
- turn execution helpers
- result / exit handling

That alone is enough to massively improve the repo, even before perfect mechanic naming.

---

## 6. Scene Graph (PARTIAL)

### Current status
The renderer traversal logic in **`12D80.c`** has a named skeleton:

- entry: `SceneGraph_ProcessRoot`
- child walk: `SceneGraph_VisitChildren` (`0x80013330`)
- dispatch table: `D_8006F0A4` → `GraphNode_Process*` (ortho, projection, background, fog, light, display list, object, matrix, switch, billboard, shadow, …)
- translucent: `RenderGraph_HandleTranslucentNode` (`0x80014124`)
- material/layer cache: `Renderer_*` (`SetViewport` / `ResetViewport` at `0x8001638C` / `0x8001660C`)

Still address-based: `func_80012870` and `func_80012960` (vertex helpers), `GraphNode_ProcessLight` body (`GLOBAL_ASM` at `func_80013D34`), and almost every node struct (`unk_D_86002F34_*`, `unk_D_800ABB10` / `28`).

### Promotion goal
The goal here is not “name every draw function.”

The goal is to impose **scene graph structure** on `12D80.c`:

- traversal entrypoint
- node dispatch
- child/sibling traversal
- matrix/state propagation
- render callback execution
- graph-node-type handling

---

## 7. Scene graph naming strategy for `12D80.c`

### Use structure-first names
Start with broad, honest names like:
- `SceneGraph_Traverse`
- `SceneGraph_TraverseNode`
- `SceneGraph_ProcessNode`
- `SceneGraph_VisitChildren`
- `SceneGraph_VisitSibling`
- `SceneGraph_PushState`
- `SceneGraph_PopState`
- `SceneGraph_ApplyTransform`
- `SceneGraph_RunCallback`

### Node-type buckets
If node-type dispatch is visible, use staged names like:
- `SceneGraph_ProcessTransformNode`
- `SceneGraph_ProcessDisplayListNode`
- `SceneGraph_ProcessSwitchNode`
- `SceneGraph_ProcessSelectorNode`
- `SceneGraph_ProcessAnimatedNode`

Only keep these if the node behavior is actually supported by code shape.

### What not to do
Do not rename everything as if the full node taxonomy is already known.

Avoid:
- `GeoLayoutMarioStyleBoneTraversal`
- `PokemonBattleArenaLayeredRenderNode`
- `AdvancedTransparencySortPass`

unless proven.

---

## 8. `12D80.c` symbolic pass checklist

For each major function in `12D80.c`, answer:

1. Is this an entrypoint or helper?
2. Does it recurse?
3. Does it iterate linked nodes?
4. Does it switch on a node type?
5. Does it mutate matrix/render state?
6. Does it submit display lists or only prepare state?
7. Does it invoke callbacks or node-local function pointers?

### Suggested minimum buckets
- `SceneGraph_*`
- `RenderGraph_*`
- `GraphNode_*`
- `GraphTraversal_*`
- `RenderState_*`

### Likely structural groups
#### A. Entry/traversal
- `SceneGraph_Traverse`
- `SceneGraph_TraverseRoot`
- `SceneGraph_TraverseNode`

#### B. Node processing
- `SceneGraph_ProcessNode`
- `SceneGraph_ProcessNodeType`

#### C. State transforms
- `SceneGraph_ApplyTransform`
- `SceneGraph_UpdateMatrix`
- `SceneGraph_ApplyBillboard`
- `SceneGraph_ApplyScale`

#### D. Callback/display execution
- `SceneGraph_RunNodeCallback`
- `SceneGraph_SubmitDisplayList`
- `SceneGraph_ProcessRenderable`

---

## 9. How battle and scene graph work connect

These two partial systems are related in practice:

- Fragment 62 likely owns high-level battle scene behavior.
- `12D80.c` likely provides lower-level scene graph traversal or render-graph execution used by battle and other scenes.

That means your naming pass should avoid smuggling battle-specific semantics into `12D80.c` too early.

A good boundary is:

### Fragment 62
Owns:
- battle state
- turn flow
- AI
- event sequencing
- battle-local scene decisions

### `12D80.c`
Owns:
- generalized graph traversal
- generalized node processing
- generalized render-state propagation

If a function is reused across many systems, it belongs in the scene-graph/render bucket, not battle-specific naming.

---

## 10. Remaining Blocker

### Plain-language blocker
The remaining blocker is **not** that the shells are unknown.

The shells are named. What is left is the *body*:

1. **Fragment 62**
   - scene / turn / AI / event *entrypoints* are named
   - opcode handlers, turn helpers, and ~1926 other `func_843*` functions are not

2. **`12D80.c`**
   - traversal and node-type processors are named
   - two helpers still `GLOBAL_ASM`, and graph-node structs are still `unk_*`

### Practical blocker definition
The *shell* buckets exist. The remaining blocker is unnamed **bodies and structs**:

- Fragment 62 opcode handlers, turn internals, leftover AI, result/exit
- `12D80.c` two `GLOBAL_ASM` helpers and `unk_*` graph-node layouts

### What counts as “unblocked”
You can consider these subsystems unblocked when:

#### Battle side
- The 10–25 next helpers around `BattleTurn_*` / `BattleEvent_*` / `BattleAI_ChooseMove` are named
- Remaining `GLOBAL_ASM` in fragment 62 is gone or isolated
- Result/exit still may be `BattleResult_*` stubs — that is OK

#### Scene graph side
- `func_80012870` and `GraphNode_ProcessLight` match
- `func_80012960` is named
- The node structs those processors read have field names (even if incomplete)

Once those buckets exist, deeper promotion becomes much easier.

---

## 11. Recommended next-pass plan

### Pass 1: Fragment 62 body (shell is done)
Next 10–25 names, not a mechanic-perfect decomp:
- `fragment62_315D50.c` event opcode handlers (keep numeric ids)
- `fragment62_3020D0.c` / `fragment62_359F90.c` turn helpers around the named entrypoints
- `fragment62_361050.c` remaining AI after `BattleAI_ChooseMove`

### Pass 2: `12D80.c` matching + structs
- Match `func_80012870` and `GraphNode_ProcessLight`
- Name `func_80012960`
- Promote node structs from fields those processors actually read

### Pass 3: Shared data/context labeling
Only after the two skeletons are readable, name:
- shared battle context fields
- scene graph node structs
- turn/AI tables
- render-state stacks

This order minimizes false naming.

---

## 12. Suggested repo task wording

Use wording like this for an AI agent:

```text
Perform a structural symbolic naming pass on Fragment 62 and 12D80.c.

For Fragment 62:
- identify the likely top-level battle scene entrypoint
- separate state control, AI selection, turn execution, event sequencing, and result handling
- apply cautious names using BattleScene_, BattleTurn_, BattleAI_, BattleEvent_, and BattleResult_ prefixes
- preserve original address identity in comments or symbol aliases

For 12D80.c:
- identify the traversal entrypoint and recursive helpers
- separate node dispatch, child/sibling traversal, transform/state propagation, and callback/display submission
- apply cautious names using SceneGraph_, GraphNode_, GraphTraversal_, and RenderState_ prefixes
- do not invent game-specific semantics unless directly supported

Document unknowns at each step and prefer honest structural names over specific guessed names.
```

---

## 13. Quick symbolic naming cheat sheet

### Fragment 62
Safe early names:
- `BattleScene_Run`
- `BattleScene_SubstateDispatch`
- `BattleScene_Update`
- `BattleTurn_Begin`
- `BattleTurn_BuildOrder`
- `BattleTurn_ExecuteAction`
- `BattleAI_ChooseAction`
- `BattleEvent_Advance`
- `BattleResult_HandleExit`

### `12D80.c`
Safe early names:
- `SceneGraph_Traverse`
- `SceneGraph_TraverseNode`
- `SceneGraph_ProcessNode`
- `SceneGraph_VisitChildren`
- `SceneGraph_VisitSibling`
- `SceneGraph_ApplyTransform`
- `SceneGraph_RunCallback`
- `SceneGraph_SubmitDisplayList`

---

## 14. Final recommendation

### If you only do one thing next
Do **not** rename every `func_843*` or every graph-node field.

Do this:

1. Match `12D80.c` `GLOBAL_ASM` (`func_80012870`, `GraphNode_ProcessLight`).
2. Name 10–25 Fragment 62 helpers in `fragment62_315D50.c` (event opcodes, keep ids) and the turn files around `BattleTurn_BuildOrder` / `BattleTurn_Execute`.

That is how you remove the current blocker. Full order: [UPSTREAM_AUDIT_AND_PLAN.md](UPSTREAM_AUDIT_AND_PLAN.md).

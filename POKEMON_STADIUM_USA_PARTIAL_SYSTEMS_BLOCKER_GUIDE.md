# Pokémon Stadium USA RE Guide: Battle Engine (Partial), Scene Graph (Partial), Remaining Blocker

This guide is a focused reverse-engineering brief for the current Pokémon Stadium USA decomp state.

It targets three active problem areas:

1. **Battle Engine (PARTIAL)**  
   Move effects are mostly promoted, but the core AI and turn-logic in **Fragment 62** still need a symbolic naming pass.

2. **Scene Graph (PARTIAL)**  
   The renderer traversal logic in **`12D80.c`** remains largely address-based and needs structural naming.

3. **Remaining Blocker**  
   A concrete next-step definition of what is still preventing clean promotion of the battle and rendering subsystems.

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
The renderer traversal logic in **`12D80.c`** is still largely address-based.

That usually means:
- many `func_800XXXXX` names still remain
- graph-node roles may not be clearly named
- traversal phases are recognizable but not symbolically organized
- node handlers, recursion helpers, and dispatch callbacks are probably still mixed together

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
The remaining blocker is **not** that nothing is understood.

The blocker is that the two remaining central systems still lack enough symbolic structure to make the codebase read naturally:

1. **Fragment 62**
   - battle flow is identifiable
   - but the AI / turn / phase functions are still not separated and named clearly enough

2. **`12D80.c`**
   - traversal behavior is identifiable
   - but the scene graph / node-processing structure is still too address-based

### Practical blocker definition
The repo is currently blocked by a lack of **mid-level symbolic names**.

Not:
- raw disassembly access
- fragment loading
- relocation
- state routing

But:
- symbolic grouping of the battle shell
- symbolic grouping of traversal and node-handling logic

### What counts as “unblocked”
You can consider these subsystems unblocked when:

#### Battle side
- Fragment 62 has named buckets for:
  - scene state
  - turn state
  - AI selection
  - action execution
  - result handling

#### Scene graph side
- `12D80.c` has named buckets for:
  - root traversal
  - node dispatch
  - child/sibling recursion
  - state propagation
  - callback/display submission

Once those buckets exist, deeper promotion becomes much easier.

---

## 11. Recommended next-pass plan

### Pass 1: Fragment 62 structural naming
Create a naming map, not a mechanic-perfect decomp.
Goal:
- 10–25 key functions renamed into honest buckets

### Pass 2: `12D80.c` traversal skeleton
Turn address soup into a traversal model.
Goal:
- identify entrypoint
- identify recursion
- identify node dispatch
- identify render callback functions

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

If you only do one thing next, do this:

### Choose structure over specificity
For both Fragment 62 and `12D80.c`, the fastest real win is a **structural naming pass**.

That means:
- sort functions by role
- assign honest mid-level names
- keep addresses visible
- document unknowns

Do **not** try to perfectly name every gameplay mechanic or render node yet.

That is how you remove the current blocker.

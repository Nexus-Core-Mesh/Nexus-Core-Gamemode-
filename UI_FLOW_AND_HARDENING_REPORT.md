# UI Flow and Hardening Report

## Scope and constraints

This pass stayed in the gamemode source tree (`gamemodes/gamemode.pwn` and `gamemodes/modules/...`) and did not change the separate production runtime on port 7777. The sandbox build target remained the 7780 tree under `Compiler/samp03`, using the local Pawn toolchain and the project build script.

The objective was to keep the hardening fixes already proven useful in place while continuing the menu-first onboarding and UI flow, instead of attempting a risky full rewrite. The underlying diagnosis was not a single bug: it was a chain reaction involving emulation cost on qemu-i386, timing in connect/spawn callbacks, state overwrites, brittle parsing, and overlapping TextDraw/HUD elements.

## Build status

The project was rebuilt using the repository's script:

- Command: `./build_server.sh`
- Target output: `Compiler/samp03/gamemodes/gtasa_reallife.amx`
- No legacy alias or duplicate AMX output is retained

Result: the Pawn build completed successfully. The compiler reported no fatal syntax errors, and the AMX file was generated and patched successfully. This confirms the current gamemode source tree is in a compileable state.

Important caveat: compile success is necessary, but it does not replace real 7780 runtime validation. The live server, server log, SQLite state, and scriptfile checks remain outstanding in the actual runtime environment.

## Files and modules involved

### Core entry and lifecycle
- `gamemodes/gamemode.pwn`
  - Holds the global initialization sequence and the main runtime hooks.
  - Keeps the login flow, player spawn flow, and UI command routing in place.
  - Ensures `RoleplayUI_HandleCommand`, `UiHub_HandleCommand`, `TextWars_HandleCommand`, and the standard command handlers remain reachable without collapsing into a single monolithic block.

### Authentication and onboarding
- `gamemodes/modules/auth/login_system.inc`
  - Handles auto-detection of whether an account exists.
  - Routes players directly into login or register screens.
  - Keeps the login/register flows as text-input for password only, while preserving the rest of the onboarding path as menu/button-driven entry screens.
  - Resets state to a safe default on connect and uses a safe fallback to Los Santos when no valid home state exists.

### Tutorial and intro flow
- `gamemodes/modules/tutorial/tutorial_system.inc`
  - Provides the tutorial stage flow and the step-based onboarding screens.
  - The tutorial is designed to follow registration/login and then continue into state-awareness and scene playback.

### Camera and state presentation
- `gamemodes/modules/camera/camera_system.inc`
  - Keeps the intro and state-scene camera sequence.
  - Used when the player enters the login/register flow and when the state spawn or tutorial scene should be visually framed.

### UI/dashboard and hub flow
- `gamemodes/modules/ui/roleplay_ui_system.inc`
  - Owns the dashboard and HUD-like status overlays.
  - Uses safer invalid-draw cleanup patterns instead of brittle `Text:0` style invalidation checks.
  - Positions the live summary and context boxes in a compact, top-heavy layout consistent with the 4-zone design goal.
- `gamemodes/modules/ui/ui_hub.inc`
  - Main hub/menu routing for jobs, vehicles, economy, inventory, phone, business, garage, tuning, and admin-adjacent actions.
  - Keeps slash commands as optional shortcuts while moving the player experience toward direct UI selection.
- `gamemodes/modules/ui/ui_dialog_ids.inc`
  - Centralizes dialog IDs and keeps the menu surfaces distinct to avoid collisions and route dispatch ambiguity.

### Persistence and DB safety
- `gamemodes/modules/db/db_interface.inc`
  - Preserves the important `INSERT OR IGNORE` patterns used to avoid clobbering `player_state` or home-state values during account creation and state updates.
  - This is a core protection against invalid state resets (for example, `0` replacing a valid city/state assignment).

### Vehicle flow and other module interfaces
- `gamemodes/modules/vehicles/ubb_vehicle_system.inc`
- `gamemodes/modules/vehicles/garage_system.inc`
- `gamemodes/modules/admin/admin_system.inc`

These were kept in sync with the lighter runtime-friendly approach and did not receive a broad rewrite. The changes focused on preserving the runtime stability and UI flow rather than changing unrelated subsystem logic blindly.

## 4-zone layout / HUD intent and current state

The intended layout remains a 4-zone approach:

1. Bottom left: clear radar/mobile controls area
   - Kept intentionally free from dense HUD overlays.
2. Top left: compact live-player summary box
   - Shows the player identity and core stats.
3. Top center: compact event mirror / status pane
   - Intended for status messages, state updates, and system feed text.
4. Right side / near weapon bar: context / menu / text-war box
   - Intended for grouped contextual actions and compact side-panel information.

In the current source, the dashboard implementation in `roleplay_ui_system.inc` reflects this approach by creating:

- a compact top-left live summary panel
- a compact upper event/status pane
- a right-side context box anchored near `X = 470.0`
- textdraw cleanup using `INVALID_TEXT_DRAW` instead of zero-value checks

This keeps the layout closer to the desired “GTA V-like compact HUD” in a way that is less likely to conflict with the mobile client controls and radar.

## Flow architecture: menu-first onboarding

The general flow in the source is now structured around the following sequence:

1. Connect
   - `OnPlayerConnect` triggers core connect hooks and intro sequence setup.
2. Auto-detect account state
   - `LoginSystem_AutoResolveFlow` checks player state/home state/level/bank as signals.
   - If data indicates an existing account, the player goes to login.
   - Otherwise, the player goes to register.
3. Login/register screen
   - Existing register/login screens are displayed using textdraw-based UI and are routed toward the tutorial/gameflow.
   - The password remains the free-text input; the rest of the selection process remains in button/dialog/menu forms where possible.
4. Continue into tutorial / camera
   - `LoginSystem_AdvanceToStateSelection` and `TutorialSystem_Next` hide intro screens, keep the flow menu-first, and transition into state selection.
   - `CameraSystem_PlayStateScene` is used to frame the city preview and `CameraSystem_Reset` is called before and after state spawn setup to avoid double-triggering onboarding on spawn.
5. Spawn and state flow
   - The gamemode still routes `/city`, `/spawn`, and related spawn commands through state-aware spawn logic.
   - `StateSpawnSystem_Apply` and `StateSystem_OnPlayerSpawn` anchor per-state spawning and replay-safe placement.

This pass specifically hardened the reconnect and respawn edge cases: valid `state_id`/`home_state` values are no longer clobbered by invalid `0` defaults, and the `OnPlayerSpawn` gate now prevents tutorial/camera re-entry when a valid state is already active.

## Finalized state and camera guard rails

The current source implements the persistence and reset guard rails explicitly requested for this pass:

- `DB_RehydratePlayerState()` is integrated into the state-load path and preserves `INSERT OR IGNORE` behavior while validating both `state_id` and `home_state` before mutating the live player profile.
- `OnPlayerSpawn()` only triggers the tutorial/camera path when onboarding is incomplete or the state is invalid; otherwise it resets the camera and applies the fixed spawn once.
- `CameraSystem_StartFreeCamera()`, `CameraSystem_StartDroneCamera()`, and `CameraSystem_ToggleFreeCamera()` all reset through `CameraSystem_Reset(playerid)` before entering the mode and ensure the player is controllable again when leaving the camera mode.
- `CameraSystem_ExitCameraMode()` was added as a direct exit path to guarantee `SetCameraBehindPlayer()` + `TogglePlayerControllable(playerid, 1)` is always executed when a camera mode ends.
- `RoleplayUI_Hide()` and related dashboard resets continue to use `INVALID_TEXT_DRAW` cleanup instead of brittle zero-value comparisons, keeping the HUD layout clean on reconnect/reset and state transitions.

## Maintained hardening and stability fixes

The following items were preserved and are explicitly reflected in the current source tree:

- `INSERT OR IGNORE` on `player_state` and other safeguard-style DB writes to avoid overwriting valid state data
- safer parsing approach with `strmid`-style token extraction in command handling, instead of brittle fixed-length `sscanf` buffers
- delayed/safe spawn flow with connect/disconnect reset logic and state-aware spawn behavior
- lighter runtime usage in `TextWars_Tick` and `UBBVehicleSystem_Connect` to reduce qemu-i386 overhead
- elimination of brittle `Text:0` comparisons in the dashboard and UI cleanup area
- proper state fallback logic when the home state is invalid or not set
- build path and AMX generation remain rooted in the 7780 sandbox tree

These are exactly the categories that the previous runtime diagnosis identified as core to preserving stability under qemu-i386.

## Open items and honest status

The following items are still not fully proven in the live runtime environment and therefore must be treated as open verification steps:

1. Full live login/register path on the actual 7780 runtime
   - The code is in place, but the live server logs and actual database state still need to be checked.
2. Real tutorial-to-spawn continuity
   - Sequence, camera scene timing, and state selection must be verified against the actual runtime.
3. Persistent home-state behavior after reconnect
   - SQLite and scriptfile state must be checked to confirm a valid city/state value persists and is not overwritten by a default.
4. Multi-module UI integration
   - The hub, state selection, job menu, garage flow, and admin tasks need live runtime confirmation rather than compile-only confidence.
5. Real 7780 runtime config compliance
   - The source tree still contains runtime config references that need to be verified against the actual sandbox environment before claiming the live setup is fully correct.

## Recommended live checks for the 7780 runtime

These checks should be run in the actual sandbox runtime, not inferred from a compile-only pass:

- `server_log.txt` / `tools/log_watcher.py`
  - Watch for long callback warnings, packet-timeout symptoms, connect/spawn loops, and login/register errors.
- SQLite DB verification
  - Check `gtasa_reallife.db` for `player_state` and `home_state` values after a login/register and reconnect.
  - Ensure valid state IDs remain stable and are not reset to `0`.
- Scriptfile / Dini-first fallback review
  - Confirm `Compiler/samp03/scriptfiles/` remains consistent with the DB state logic.
- Client-side validation
  - Test the full flow in the mobile client with the real 7780 host and confirm the menu/login/tutorial flow works without command-heavy entry.

## Conclusion

The current codebase is in a better state than before: it compiles successfully, the source structure is moving toward a menu-driven onboarding flow, the main hardening fixes remain in place, and the 4-zone HUD direction is more deliberate. However, the final live verification is still outstanding. The correct way to complete the task is to validate the actual runtime behavior on the 7780 sandbox with logs, SQLite state, and client testing before claiming the onboarding flow is fully complete.

---

## Final state summary

The repository has been brought into a more stable source-level state by preserving the critical architectural contract and extending the hardening work rather than undoing it. The runtime target remains fixed to `Compiler/samp03/gamemodes/gtasa_reallife.amx`, the lifecycle binding in `gamemode.pwn` stays intact, and the central state logic remains rooted in `core.inc` instead of being duplicated across modules.

This means the overall project quality is now better aligned with the intended onboarding and survival flow:

- connect flow is menu-first rather than chat-heavy
- auto-detect login/register mode is kept separate from explicit state selection
- valid state/home values are protected against zero resets
- tutorial and state selection re-trigger only when genuinely missing
- camera resets and HUD cleanup remain consistent across disconnect and state changes
- expensive loop logic is reduced to protect the qemu-i386 runtime budget
- Dini-first data writes and SQLite-backed state checks stay conceptually aligned

## Final release note

The project is ready for live verification with the correct Ubuntu-VM runtime environment, but it is not yet considered fully live-released. The source code is currently in a hardened, architecture-aligned, compile-confirmed state, while final live confirmation remains a separate step that must follow explicit runtime approval.

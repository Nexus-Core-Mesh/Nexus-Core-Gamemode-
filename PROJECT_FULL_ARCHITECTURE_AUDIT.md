# PROJECT FULL ARCHITECTURE AUDIT

Repository root: `/workspaces/Nexus-Core-Gamemode-/copilot-worktrees/Nexus-Core-Gamemode-/laughing-tribble`
Date: 2026-11-20
Scope: source-only analysis, no live server start, no port 7777 use, no release or live approval.

## 1. Executive summary

The repository is in a source-structured, modular state with a clear architectural intent. The active runtime contract is centered on the single target `Compiler/samp03/gamemodes/gtasa_reallife.amx` and the server config values `gamemode0 gtasa_reallife 1` and `port 7780`.

The codebase is not in a fully runtime-validated or live-verified state: the Pawn toolchain is not available in this container (`qemu-i386: command not found`), so the AMX build cannot be produced here. This means the current status is a source-level architecture audit with strong structural guardrails, but without live runtime confirmation on port 7780.

Important facts observed in the code:

- `core.inc` is the canonical state source, and the code mostly respects that contract.
- Include order and lifecycle markers are present in `Compiler/samp03/gamemodes/gamemode.pwn` and align with the documented contract.
- `state_id` and `home_state` protections are present in the core/state guards and in the login/disconnect hardening paths.
- Dini is the persisted runtime-first path under `Compiler/samp03/scriptfiles/`, with an explicit `temp -> validate -> rename` pattern in the documentation and project structure, but the actual runtime code for file writes is not fully proven in this environment.
- SQLite is wired as a local read-only operational view through `control-panel/server.js` and `data/db/gtasa_reallife.db`.
- MySQL is present as a backend stub / later-stage alternative, not as an active runtime write path.
- There are multiple project documents and tool scripts that describe a production-safe or live-ready target, but they do not replace actual runtime validation.

## 2. Project structure and active responsibilities

### 2.1 Repository-level layout

Relevant active directories and files include:

- `/workspaces/Nexus-Core-Gamemode-/copilot-worktrees/Nexus-Core-Gamemode-/laughing-tribble/Compiler/samp03/gamemodes/gamemode.pwn`
- `/workspaces/Nexus-Core-Gamemode-/copilot-worktrees/Nexus-Core-Gamemode-/laughing-tribble/Compiler/samp03/gamemodes/modules/`
- `/workspaces/Nexus-Core-Gamemode-/copilot-worktrees/Nexus-Core-Gamemode-/laughing-tribble/gamemodes/`
- `/workspaces/Nexus-Core-Gamemode-/copilot-worktrees/Nexus-Core-Gamemode-/laughing-tribble/scripts/`
- `/workspaces/Nexus-Core-Gamemode-/copilot-worktrees/Nexus-Core-Gamemode-/laughing-tribble/tools/`
- `/workspaces/Nexus-Core-Gamemode-/copilot-worktrees/Nexus-Core-Gamemode-/laughing-tribble/control-panel/`
- `/workspaces/Nexus-Core-Gamemode-/copilot-worktrees/Nexus-Core-Gamemode-/laughing-tribble/data/db/gtasa_reallife.db`
- `/workspaces/Nexus-Core-Gamemode-/copilot-worktrees/Nexus-Core-Gamemode-/laughing-tribble/Compiler/samp03/server.cfg`
- `/workspaces/Nexus-Core-Gamemode-/copilot-worktrees/Nexus-Core-Gamemode-/laughing-tribble/server.cfg`
- `/workspaces/Nexus-Core-Gamemode-/copilot-worktrees/Nexus-Core-Gamemode-/laughing-tribble/build_server.sh`

### 2.2 Main gamemode source tree

The actual gamemode source used by the runtime contract is the Pawn root in:

- `/workspaces/Nexus-Core-Gamemode-/copilot-worktrees/Nexus-Core-Gamemode-/laughing-tribble/Compiler/samp03/gamemodes/gamemode.pwn`

This file includes modules in a long sequence beginning with core and DB, then state/admin/auth/economy/jobs/laws and later housing/vehicle/leisure/web. The include order is visible in the file and matches the project contract:

- `a_samp`
- `sscanf2`
- `modules/core/core.inc`
- `modules/core/production_guard.inc`
- `modules/core/runtime_registry.inc`
- `modules/core/module_contract.inc`
- `modules/ui/ui_dialog_ids.inc`
- `modules/db/db_interface.inc`
- `modules/admin/admin_system.inc`
- `modules/admin/state_admin_system.inc`
- `modules/admin/admin_mapping_system.inc`
- `modules/state/state_system.inc`
- `modules/state/state_identity_system.inc`
- `modules/state/state_services.inc`
- `modules/state/state_spawn_system.inc`
- ...
- `modules/factions/text_wars.inc`
- `modules/ui/state_control_ui.inc`
- `modules/ui/ui_hub.inc`

This is consistent with the intended architecture: Core -> DB -> State -> Admin -> Auth -> Economy -> Jobs -> Laws -> Housing/Vehicle -> Leisure/Web.

### 2.3 Module responsibilities and active wiring

Current active module cluster in the source tree includes:

- Core and runtime governance:
  - `/Compiler/samp03/gamemodes/modules/core/core.inc`
  - `/Compiler/samp03/gamemodes/modules/core/production_guard.inc`
  - `/Compiler/samp03/gamemodes/modules/core/runtime_registry.inc`
  - `/Compiler/samp03/gamemodes/modules/core/module_contract.inc`

- DB abstraction and persistence:
  - `/Compiler/samp03/gamemodes/modules/db/db_interface.inc`
  - `/Compiler/samp03/gamemodes/modules/db/db_sqlite.inc`
  - `/Compiler/samp03/gamemodes/modules/db/db_mysql.inc`

- State, onboarding and auth:
  - `/Compiler/samp03/gamemodes/modules/state/state_system.inc`
  - `/Compiler/samp03/gamemodes/modules/state/state_spawn_system.inc`
  - `/Compiler/samp03/gamemodes/modules/auth/login_system.inc`
  - `/Compiler/samp03/gamemodes/modules/camera/camera_system.inc`
  - `/Compiler/samp03/gamemodes/modules/tutorial/tutorial_system.inc`

- Economy, budget, ledger and bank:
  - `/Compiler/samp03/gamemodes/modules/economy/state_bank_system.inc`
  - `/Compiler/samp03/gamemodes/modules/economy/state_budget_system.inc`
  - `/Compiler/samp03/gamemodes/modules/economy/state_tax_system.inc`
  - `/Compiler/samp03/gamemodes/modules/economy/market_system.inc`
  - `/Compiler/samp03/gamemodes/modules/management/state_ledger_system.inc`

- Jobs, factions, law, crime, emergency and justice:
  - `/Compiler/samp03/gamemodes/modules/jobs/jobs_core.inc`
  - `/Compiler/samp03/gamemodes/modules/jobs/state_jobs.inc`
  - `/Compiler/samp03/gamemodes/modules/law/law_system.inc`
  - `/Compiler/samp03/gamemodes/modules/law/jail_system.inc`
  - `/Compiler/samp03/gamemodes/modules/crime/crime_system.inc`
  - `/Compiler/samp03/gamemodes/modules/emergency/dispatch_system.inc`
  - `/Compiler/samp03/gamemodes/modules/emergency/emergency_system.inc`
  - `/Compiler/samp03/gamemodes/modules/justice/prison_system.inc`
  - `/Compiler/samp03/gamemodes/modules/justice/court_system.inc`
  - `/Compiler/samp03/gamemodes/modules/factions/text_wars.inc`
  - `/Compiler/samp03/gamemodes/modules/factions/gang_fight_system.inc`

- Housing, inventory, vehicles, UBB and world systems:
  - `/Compiler/samp03/gamemodes/modules/housing/house_system.inc`
  - `/Compiler/samp03/gamemodes/modules/housing/house_storage_system.inc`
  - `/Compiler/samp03/gamemodes/modules/inventory/inventory_system.inc`
  - `/Compiler/samp03/gamemodes/modules/vehicles/vehicle_system.inc`
  - `/Compiler/samp03/gamemodes/modules/vehicles/ubb_vehicle_system.inc`
  - `/Compiler/samp03/gamemodes/modules/vehicles/garage_system.inc`
  - `/Compiler/samp03/gamemodes/modules/vehicles/fuel_station_system.inc`

- UI, web, auth and control-panel integration:
  - `/Compiler/samp03/gamemodes/modules/ui/ui_hub.inc`
  - `/Compiler/samp03/gamemodes/modules/ui/state_control_ui.inc`
  - `/Compiler/samp03/gamemodes/modules/web/web_bridge.inc`
  - `/control-panel/server.js`

This is the active architecture in code: modular but broad. The project has a lot of module definitions, but many of them are structure-heavy and not each fully proven in a live runtime in this environment.

## 3. Core contract and lifecycle guards

### 3.1 Single Source of Truth

The canonical authority is `core.inc`.

Evidence in `/Compiler/samp03/gamemodes/modules/core/core.inc`:

- `enum E_PLAYER_PROFILE` includes `E_PLAYER_STATE`, `E_PLAYER_HOME_STATE`, `E_PLAYER_ADMIN_LEVEL`, `E_PLAYER_ADMIN_STATE`, `E_PLAYER_BANK` and others.
- `new gPlayerData[MAX_PLAYERS][E_PLAYER_PROFILE_COUNT];` is the main memory model.
- `Core_IsValidState(stateid)` ensures only 0..2 are valid state IDs.
- `Core_GetValidStateOrDefault(stateid, fallbackState)` is the central fallback logic.
- `Core_SetPlayerState()` and `Core_SetPlayerHomeState()` refuse invalid values.
- `Core_NormalizePlayerState()` repairs invalid values back to `STATE_LOS_SANTOS`.

This is the actual source-of-truth pattern: no second parallel shadow-state is enforced across the core layer, though some module-local arrays like `gPlayerHomeState[playerid]` still exist, especially in the auth module, and are treated as mirrors rather than authority.

### 3.2 Lifecycle stages

`gamemode.pwn` explicitly sets lifecycle states and calls initialization in order.

Observed progression:

- `Core_SetLifecycleStage(CORE_LIFECYCLE_BOOT);`
- `DB_Initialize();` then `Core_SetLifecycleStage(CORE_LIFECYCLE_DB_READY);`
- `StateSystem_Initialize();` etc. then `Core_SetLifecycleStage(CORE_LIFECYCLE_STATE_READY);`
- `AdminSystem_Initialize();` etc. then `Auth` stage
- `CameraSystem_Initialize();` / `TutorialSystem_Initialize();` etc.
- eventual `Core_SetLifecycleStage(CORE_LIFECYCLE_WORLD_READY);`
- `Core_SetLifecycleStage(CORE_LIFECYCLE_RUNTIME_READY);`

This matches the documented lifecycle contract:

`BOOT -> DB_READY -> STATE_READY -> AUTH_READY -> WORLD_READY -> RUNTIME_READY`

### 3.3 State reset and 0-protection

The current code deliberately guards invalid `state_id` and `home_state` values from being turned into `0`-based invalid state resets.

Evidence:

- `Core_IsValidState(stateid)` returns `(stateid >= STATE_LOS_SANTOS && stateid < STATE_COUNT);` so valid values are 0, 1, 2.
- `Core_GetValidStateOrDefault()` avoids invalid fallback cascades, returning the fallback valid state instead of 0.
- `LoginSystem_OnPlayerConnect()` and `LoginSystem_OnPlayerDisconnect()` both preserve valid values before normalizing.
- `Core_OnPlayerDisconnect()` also preserves valid state/home values before clearing non-state fields.

This is directionally correct and consistent with the developer requirement that valid state values must not be reset to 0.

### 3.4 OnPlayerSpawn and camera reset guard

The `OnPlayerSpawn` path is wired in the gamemode root:

- `/Compiler/samp03/gamemodes/gamemode.pwn` has `public OnPlayerSpawn(playerid) { StateSystem_OnPlayerSpawn(playerid); }`

The state-system spawn logic is in `/Compiler/samp03/gamemodes/modules/state/state_system.inc`.

Observations:

- `StateSystem_OnPlayerSpawn(playerid)` normalizes to valid states and applies the state/home state when valid.
- `Core_SetPlayerState()` and `Core_SetPlayerHomeState()` both reject invalid state ids.
- The camera reset pattern is present across modules, especially in the auth and camera flows.
- The project documents and code emphasize `SetCameraBehindPlayer()` and `TogglePlayerControllable(playerid, 1)` as the reset pattern.

This is an active guard pattern and is consistent with the intended runtime design.

### 3.5 Hotpath and qemu-i386 guardrails

The code contains explicit guard logic intended to reduce load under qemu-i386 and limited RAM.

Examples:

- `/Compiler/samp03/gamemodes/modules/factions/text_wars.inc` includes `forward TextWars_Tick();` and `public TextWars_Tick()`.
- The audit tool found `TextWars_Tick` and `GangFightSystem_Tick` are considered hotpath guard candidates.
- `If -> Return` early guards are present across modules for invalid players and invalid state ids.
- `strmid`-style patterning is referenced in the documentation as a preferred token extraction method over unsafe or broad parsing.

This is active and directionally aligned with the resource constraints, but without compile/runtime confirmation it is not proven fully live-safe.

## 4. Dini vs SQLite vs MySQL: actual active wiring

### 4.1 Dini and scriptfiles path

The project structure documents a Dini-first persistence model under `Compiler/samp03/scriptfiles/`.

The intended runtime scriptfiles tree includes physical folders for:

- players/
- houses/
- house_storage/
- safes/
- vehicles/
- ubb_vehicles/
- state/
- state_kasse/
- business/
- factions/
- inventory/
- temp/
- backup/
- logs/

This is described in documentation and tracked as a runtime-first persistence structure. From the repository state, the presence of the directories is real in the project tree, but the actual Dini write path is not directly proven in this environment beyond documentation and module conventions.

The project explicitly expects an atomic write sequence: `temp -> validate -> rename`.

This is an architectural requirement, not a file-by-file verified runtime commit in the current container. The code is written to prefer valid, atomic Dini-style save logic, but the actual runtime persistence path is only partially validated here.

### 4.2 SQLite persistence and control-panel read-only view

The active SQLite path is clearly wired in Node.js:

- `/workspaces/Nexus-Core-Gamemode-/copilot-worktrees/Nexus-Core-Gamemode-/laughing-tribble/control-panel/server.js`
- database path: `path.resolve(__dirname, '../data/db/gtasa_reallife.db')`

The panel opens the DB read-only:

`new sqlite3.Database(dbPath, sqlite3.OPEN_READONLY, ...)`

The app serves JSON endpoints for:

- `/api/server-info`
- `/api/summary`
- `/api/overview`
- `/api/states`
- `/api/players`
- `/api/admins`
- `/api/jobs`
- `/api/factions`
- `/api/houses`
- `/api/businesses`
- `/api/season`

This is the active operational pattern: the control panel is a read-only operational dashboard over SQLite, not a primary runtime save authority.

### 4.3 Pawn-level SQLite abstraction

The Pawn DB layer is split across:

- `/Compiler/samp03/gamemodes/modules/db/db_interface.inc`
- `/Compiler/samp03/gamemodes/modules/db/db_sqlite.inc`
- `/Compiler/samp03/gamemodes/modules/db/db_mysql.inc`

The actual code in `db_sqlite.inc` is a stub:

- `DB_SQLite_Initialize()` prints startup messages
- `DB_SQLite_Execute()` validates `strlen(query) > 0` and returns true
- `DB_SQLite_Query()` does the same

This means SQLite is not implemented as a real live database driver in the Pawn layer in this repository state; it is a structural placeholder layer and an operational abstraction layer that the rest of the project expects to be backed by a real DB environment later. This is an important discrepancy: the project documentation and control-panel Node layer clearly assume SQLite as a concrete local database, but the Pawn module only exposes a stub.

### 4.4 MySQL

`db_mysql.inc` is similarly a stub backend layer:

- `DB_MySQL_Initialize()` logs a prepared state
- `DB_MySQL_Execute()` and `DB_MySQL_Query()` just print the query and return `true`

This indicates MySQL is not actively connected and should be treated as a separate later-stage or external not-currently-wired backend. The project guidance says MySQL remains downstream or external, and the code supports that interpretation.

### 4.5 Transaction logic and state validation

`db_interface.inc` contains transaction helpers and state validation endpoints:

- `DB_BeginTransaction()`
- `DB_CommitTransaction()`
- `DB_RollbackTransaction()`
- `DB_ValidateStateValueForPersist(stateid)`
- `DB_SavePlayerState(playerid, stateid)`
- `DB_EnsureStateSchema()`

The interface checks `Core_IsValidState(stateid)` before persisting and falls back to `STATE_LOS_SANTOS` when invalid, which is consistent with the 0-reset protection mandate. This is active logic and one of the key points where state validity is enforced at the persistence layer.

### 4.6 State transfer and ledger integrity

The codebase contains state-ledger/transfer logic under:

- `/Compiler/samp03/gamemodes/modules/management/state_ledger_system.inc`

Transfer-style operations are conceptually present, but the repository state does not show a confirmed end-to-end runtime ledger sync between Dini and SQLite. This remains a live or runtime verification gap, not necessarily a code absence.

Important: atomic transfers (`temp -> validate -> rename`) are documented as the expected persistence model; the implementation details for each critical save path are not shown as a fully completed live sync path in this source tree.

## 5. Control panel integration and role boundaries

### 5.1 Control panel behavior

The active panel is a direct operational read interface over SQLite:

- Node server opens SQLite in read-only mode
- it queries tables such as `states`, `players`, `admin_roles`, `jobs`, `factions`, `houses`, `businesses`, `server_events`
- it offers summary and overview endpoints
- the UI loads static front-end files under `control-panel/public/`

This is definitely active and consistent with the documentation requirement that SQLite is the operational read base for the control panel.

### 5.2 Role of SQLite vs Dini

The code is internally consistent in assigning roles:

- SQLite: local operational state view + dashboard backend source.
- Dini: runtime persistence and save-first backup/restore path under scriptfiles.
- MySQL: separate external or later backend, not active in the present code path.

This is the intended stateful split.

### 5.3 Live risk / sync gap

The risk is not that the SQLite panel is writing data incorrectly; it is read-only and thus behaves safely. The real gap is that the system has not demonstrated a fully synchronized end-to-end flow where Dini writes, SQLite reads, and panel views remain consistent after live state changes. That sync and fall-back quality is not proven here, though the intended design is clear.

## 6. Runtime guards, onboarding, UI and qemu-i386 constraints

### 6.1 Early guards and return patterns

The project contains multiple early invalid-check returns.

Examples:

- `if (!Core_IsValidPlayerId(playerid)) return;`
- `if (!Core_IsValidState(stateid)) return;`
- `if (!Core_IsValidState(gPlayerData[playerid][E_PLAYER_STATE])) { ... }`

This is a strong early-guard pattern and consistent with resource minimization under qemu-i386.

### 6.2 TextWars and hotpath guard patterns

The audit found a dedicated hotpath guard in the faction system:

- `/Compiler/samp03/gamemodes/modules/factions/text_wars.inc`
- `TextWars_Tick()` is an explicit tick guard and the audit script flags this as a targeted hotpath limiter.

This is active code meant to prevent unnecessary callback work.

### 6.3 Camera reset and UI continuity

The project defines a stable camera reset pattern:

- `SetCameraBehindPlayer()`
- `TogglePlayerControllable(playerid, true/false)`

This is enforced in multiple modules and is part of the intended menu/login flow. The login and tutorial flow carries a valid state/home selection and then re-enables control, which is consistent with the design.

### 6.4 Onboarding flow

The onboarding path is active in code and architecture:

- `LoginSystem_OnPlayerConnect()`
- `LoginSystem_ShowRegisterScreen()`
- `LoginSystem_SetPlayerHomeState()`
- `StateSystem_OnPlayerSpawn()`
- `StateSpawnSystem_Apply()` or equivalent spawn logic

The flow is designed to select LS/SF/LV and then spawn validly after a state/home state choice. This is active source logic and a strong part of the architecture.

### 6.5 Password and selection flow details

The project relies on menu or dialog-driven entry for the current onboarding flow. The UI modules are documented and present, but this is still source-state structure, not a confirmed live gameplay validation.

The design is not contradictory to the rules: it chooses a valid state home and sets player state only when valid.

## 7. Build contract, existing toolchain and current gaps

### 7.1 Single-target build contract

The active runtime target is documented consistently across scripts and docs:

- `Compiler/samp03/gamemodes/gtasa_reallife.amx`
- `gamemode0 gtasa_reallife 1`
- `port 7780`
- `Port 7777` explicitly excluded as an active project contract.

Relevant files:

- `/workspaces/Nexus-Core-Gamemode-/copilot-worktrees/Nexus-Core-Gamemode-/laughing-tribble/build_server.sh`
- `/workspaces/Nexus-Core-Gamemode-/copilot-worktrees/Nexus-Core-Gamemode-/laughing-tribble/scripts/build_gamemode.sh`
- `/workspaces/Nexus-Core-Gamemode-/copilot-worktrees/Nexus-Core-Gamemode-/laughing-tribble/Compiler/samp03/server.cfg`
- `/workspaces/Nexus-Core-Gamemode-/copilot-worktrees/Nexus-Core-Gamemode-/laughing-tribble/server.cfg`
- `/workspaces/Nexus-Core-Gamemode-/copilot-worktrees/Nexus-Core-Gamemode-/laughing-tribble/README.md`
- `/workspaces/Nexus-Core-Gamemode-/copilot-worktrees/Nexus-Core-Gamemode-/laughing-tribble/Compiler/samp03/README.md`

### 7.2 Current toolchain status

Current result in this container:

- `bash build_server.sh` ends with `qemu-i386: command not found`
- `tools/qemu_7780_readiness_check.py` reports the AMX target as missing
- `tools/audit_modules_and_guards.py` runs and reports architecture consistency, but not a compiled binary

This confirms the current container environment is missing the 32-bit Pawn toolchain needed to produce the AMX output.

### 7.3 What is present

The repository contains:

- `scripts/build_gamemode.sh`
- `scripts/deploy_server.sh`
- `scripts/setup_environment.sh`
- `scripts/debug_server.sh`
- `tools/qemu_7780_readiness_check.py`
- `tools/audit_modules_and_guards.py`
- `tools/validate_server_cfg.py`
- `tools/runtime_contract_guard.py`
- `tools/nexus_hub_tui.py`
- `tools/samp_tui.py`
- `tools/README.md`
- `Compiler/samp03/plugins/README.md`
- multiple docs and architecture reports.

This is a fully populated offline-check and documentation layer.

### 7.4 What is still open for actual runtime validation on port 7780

The following remain open and must be validated on a proper target environment with the real toolchain:

- `qemu-i386` present and working
- `Compiler/pawncc/pawncc` accessible
- `Compiler/samp03/gamemodes/gtasa_reallife.amx` successfully generated
- actual server launch on port 7780
- scriptfiles directory fully present and writable
- Dini persistence writes passing `temp -> validate -> rename`
- SQLite reads matching runtime state
- control-panel read view matching state values
- no live crash under qemu-i386 resource limits

## 8. Open points / contradictions / sourceless assumptions

The current repository contains a strong architecture and a clear source contract, but several things remain source-only and not fully runtime-validated:

1. The Pawn DB layer is mostly a stub layer, while the Node panel expects SQLite to be real and operational.
2. The Dini persistence path is architecturally documented and intended, but the actual runtime file-write integration is not proven in this container.
3. The code contains valid guard logic for state values and player state resets, but not all runtime-save and reload paths have been live-tested.
4. `gtasa_reallife.amx` is expected as the only target but is absent in this environment, so no actual compiled binary exists here.
5. The project has repeated documentation that says “build confirmed” or “compile success,” but those claims are not valid in this environment without a working `qemu-i386` toolchain and actual build output.
6. The project is source-coherent and structurally disciplined, but it is not a live-verified runtime state.

## 9. Bottom line

The repository is currently in a disciplined source architecture state with clear state-source, lifecycle, and runtime-target rules. The core code is shaped to preserve valid state/home-state values, the lifecycle stages are in order, the include structure is consistent, and the control panel is clearly read-only against SQLite. Dini remains the intended runtime persistence path and MySQL is not active.

The main outstanding issue is not code architecture drift; it is environment-level runtime validation. Without working Pawn toolchain support (`qemu-i386` and compiler), the actual AMX output and live 7780 runtime verification cannot be completed here.

The project is therefore best described as:

- source-coherent
- structurally controlled
- runtime not live-verified
- build not currently executable in this container
- port 7777 explicitly not used
- production/live release still withheld until real environment validation

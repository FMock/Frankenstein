# Frankenstein Engine Backlog

Tracked shortcomings that make Frankenstein not yet a "general-purpose" 2D engine.
Each entry is written so it can be pasted directly into a GitHub Issue (title = the
heading). Ordered roughly by impact. Items already addressed are noted at the bottom.

---

## 1. Decouple the engine from the TestApp's game-specific entities
**Problem:** `Game` hardcodes `Player`, `StoreClerk`, `vector<Skeleton>`, and `TextString`
as members, constructs them in `Game::LoadData`, and draws/updates them by name in
`UpdateGame`/`GenerateOutput`. The engine *is* the demo.
**Why it matters:** A real engine should let a game define its own entities; you can't
build a different game without editing the engine DLL.
**Sketch:** Introduce a generic `Scene`/entity container (e.g. `std::vector<std::unique_ptr<GameObject>>`
with virtual `update(dt)`/`draw()`), have `Game` own a `Scene` and iterate it generically,
and move `Player`/`StoreClerk`/`Skeleton`/the demo wiring out of the engine into the
TestApp project.

## 2. Input abstraction / action mapping
**Problem:** `Game::ProcessInput` is hardwired to the player (reads arrow keys and calls
`m_player->moveLeft()` etc.), and relies on a subtle `memcpy`-before-pump edge-detection trick.
**Why it matters:** Games can't rebind keys or react to input without editing the engine,
and the engine shouldn't know what a "player" is.
**Sketch:** An `Input` subsystem exposing per-frame key state + edge queries
(`wasPressed`/`isDown`), plus an action-mapping layer so games bind keys → actions and
entities subscribe. Removes player-specific logic from the engine loop.

## 3. Audio subsystem (FMOD is linked but unused)
**Problem:** FMOD libs are linked in the vcxproj, but there is **no audio code** — only a
commented-out `//fmod_sys->update();` in `Game::UpdateGame`.
**Why it matters:** No sound at all; the dependency is dead weight.
**Sketch:** An `Audio` module that initializes an FMOD system, loads/plays sounds, and is
ticked once per frame from the game loop. Or drop FMOD if audio isn't wanted.

## 4. Collision response (not just detection)
**Problem:** `Sprite::AABBIntersect` exists and observers print messages on overlap, but
there is no actual collision *resolution* (no blocking, no physics response).
**Why it matters:** Most games need entities to collide with the world/each other.
**Sketch:** A collision/physics step that detects overlaps and resolves them (separation,
callbacks/events), decoupled from the Observer "print a line" demo behavior.

## 5. Logging facility
**Problem:** Diagnostics are scattered `printf`/`fprintf(stderr, ...)` calls.
**Why it matters:** No levels, no way for a host app to capture/redirect engine logs.
**Sketch:** A small leveled logger (info/warn/error) the engine uses everywhere and the
host can hook.

## 6. Asset root / path handling
**Problem:** Asset paths are hardcoded `../../config/...` and `../../images/...`, resolved
against the process working directory (which must sit two levels under the repo root).
**Why it matters:** Brittle; breaks if launched from anywhere else; not shippable.
**Sketch:** An asset-root concept (configurable base directory, or located relative to the
executable) and a path helper, so nothing depends on cwd depth.

## 7. Fixed-timestep update / render decoupling
**Problem:** Even with vsync now enabled, the simulation advances by the raw frame delta,
so physics/animation are tied to frame rate and a stall produces a large `dt` step.
**Why it matters:** Determinism and stability across machines.
**Sketch:** A fixed-timestep accumulator for `update()` with interpolated rendering.

## 8. General text / UI / font system
**Problem:** `TextString` is a single hardcoded font-sheet layout (16×6 glyph grid) with a
fixed glyph-advance; there is no general font/UI abstraction.
**Why it matters:** Can't do multiple fonts, alignment, wrapping, or HUD layout.
**Sketch:** A font/text module (load a font definition, measure/lay out strings) and a HUD
layer drawn in screen space.

## 9. Refactor `facingDirection`-as-animation-index coupling
**Problem:** `AnimatedSprite::facingDirection` doubles as the animation index and only maps
onto the `LEFT/RIGHT/UP/DOWN` enum because `PlayerAnimationParams.csv`'s first four rows are
ordered left/right/up/down. Reordering the CSV silently breaks the stop-facing logic.
**Why it matters:** Fragile, surprising coupling between data file order and engine logic.
**Sketch:** Store facing direction explicitly (separate from animation index) and look up
animations by name/role rather than relying on row order.

## 10. Camera enhancements
**Problem:** The new `Camera` is minimal (instant follow + clamp).
**Why it matters:** Real games want smoother/configurable camera behavior.
**Sketch:** Add zoom, smoothed (lerp) follow, a dead-zone, multiple cameras/layers, and
parallax background layers.

## 11. Real tilemap / world representation
**Problem:** The "world" is empty space with a debug grid drawn over it; there is no map
data structure.
**Why it matters:** Games need actual level/world content.
**Sketch:** A tilemap (tileset + layer grids) with a loader and a culled renderer that
draws only tiles within the camera view.

---

## Already addressed
- **GL texture leak** — fixed via `TextureManager` (caches by filename, de-duplicates
  loads, and frees all textures on shutdown).
- **Camera / scrolling world** — `Camera` follows the player across a world larger than the
  window (clamped at edges); the player now stops at world edges instead of bouncing.
- **Uncapped game loop** — vsync enabled (`SDL_GL_SetSwapInterval(1)`).
- **`exit()` in library code** — `Player::SetupAnimation` now throws `std::runtime_error`,
  caught in `Game::LoadData` so a missing asset fails the load gracefully instead of killing
  the host process.
- **Magic world-bound `800`** — replaced by the configured world size (`<World>` in
  `Game-Settings.xml`, exposed via `Game::getWorldWidth/Height`).

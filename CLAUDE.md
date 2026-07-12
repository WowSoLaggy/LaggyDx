# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build

- MSBuild solution: `LaggyDx.sln` produces a static library (`LaggyDx.lib`). There is no executable target — this library is consumed by other projects (e.g. `AllodsApp`).
- Toolset: `v143` (Visual Studio 2022). C++ standard: **C++20** (`/std:c++20`), `ConformanceMode=true`, **`TreatWarningAsError=true`** at WarningLevel 3 — warnings WILL break the build.
- Configurations: `Debug|x64`, `Release|x64`, `Debug|Win32`, `Release|Win32`.
- Precompiled header: `stdafx.h` / `stdafx.cpp`. Every `.cpp` in the project must `#include "stdafx.h"` first. `stdafx.cpp` is the only TU with `PrecompiledHeader=Create`. `stdafx.h` pulls in Direct3D 11, DirectXMath, and a wide swath of DirectXTK headers, plus `using namespace DirectX; using namespace SimpleMath;` — those names are globally available in every TU.

Command-line build (from repo root):

```
msbuild LaggyDx.sln /p:Configuration=Debug /p:Platform=x64
```

No test project, no lint configuration.

### External dependencies (sibling repos)

The project's include path is `$(SolutionDir)..;$(SolutionDir)..\LaggySdk` (see `LaggyDx/Project.props`), and the `.vcxproj` references sibling solutions by relative path:

- `..\..\LaggySdk\LaggySdk\LaggySdk.vcxproj` — companion utility library; pervasive (event system, contracts, math, Win32 wrappers). Headers are included as `<LaggySdk/Foo.h>`.
- `..\..\DirectXTK\DirectXTK_Desktop_2022_Win10.vcxproj` — Microsoft's DirectXTK, used heavily (SpriteBatch, SpriteFont, SimpleMath, Model loading, Keyboard/Mouse, etc.).

These repos must exist at `..\LaggySdk` and `..\DirectXTK` relative to this one or the build will fail to resolve project references.

### HLSL shader compilation

The HLSL shaders (`OceanShader`, `SimpleShader`, `SkydomeShader`, `SpriteShader`, `TerrainShader` — each `.vs.hlsl` + `.ps.hlsl` — plus `InstancedShader.vs.hlsl` and `InstancedDepthShader.vs.hlsl`, which have no pixel shader of their own and reuse `g_simplePs` / `g_depthPs` respectively, and `DepthShader.{vs,ps}.hlsl` for the shadow-map pass) are compiled by MSBuild's `FxCompile` step into C arrays in `LaggyDx/Generated/*.gen.{vs,ps}.h` (e.g. `g_simpleVs`, `g_oceanPs`). Those generated headers are then `#include`d by the corresponding `*Shader.cpp`. The `Generated/` directory is build output — don't edit it. If you add a new shader, declare it under `<FxCompile>` in `LaggyDx.vcxproj` with `ShaderType`, `VariableName`, `HeaderFileOutput`, and `ShaderModel` (currently 5.0).

## Architecture

`LaggyDx` is a thin DirectX 11 application/engine framework: it owns the Win32 window, input, render device, resource cache, 2D and 3D renderers, and a retained-mode GUI tree. Everything lives in the `Dx` namespace (with a `Dx::thd` sub-namespace for thermodynamic/tile simulation pieces).

Source is flat under `LaggyDx/` — there is no subfolder structure.

### `App`: the engine singleton and main loop

`App` (`App.h`/`App.cpp`) is the central engine class. It is a singleton (`App::get()` returns `*s_this`, set in the constructor) and inherits `Sdk::EventHandler`. The constructor builds, in order:

1. `Sdk::Window` (Win32 window from `LaggySdk`).
2. `IRenderDevice::create(...)` → owned `unique_ptr`.
3. `IResourceController::create(...)` → owned `unique_ptr`, takes the render device and the assets folder from `AppSettings`.
4. `IRenderer2d::create(...)` → owned `unique_ptr`.
5. `IInputDevice::create(...)` → owned `unique_ptr`, then `connectTo(*d_inputDevice)` wires it into the SDK event graph.
6. A root GUI `Form` (the top of the control tree).

`run()` calls the virtual `onStart()`, then loops `mainloop()` while `continueLoop()` returns true. `mainloop()` ticks the timer, dispatches keyboard/mouse state from the input device through `handleKeyboard`/`handleMouse`, calls `notify(OnUpdate(dt))` to fan out to event listeners, then `update(dt)` → `updateGui(dt)` → render block (scoped under a `Sdk::Locker(*d_renderDevice)` and `Sdk::ScopeGuard(*d_renderDevice)`) → `render()` + `renderGui()`.

**To build an app on top of this library, derive from `App` and override the protected virtuals**: `onStart`, `onEnd`, `update`, `render`, `onKeyPressed`/`onKeyReleased`/`onKeyCurrent`, `onMouseMove`/`onMouseWheel`/`onMouseClick`/`onMouseRelease`. The default `update`/`render` are empty; `updateGui` and `renderGui` already drive the form tree.

### Interface + factory pattern

The big subsystems follow a uniform pattern: an abstract `IFoo` header with a `static std::unique_ptr<IFoo> create(...)` factory, paired with a concrete `Foo` implementation in `Foo.h`/`Foo.cpp`. Callers should hold and pass the `IFoo&` interface. Examples: `IRenderDevice` / `RenderDevice`, `IResourceController` / `ResourceController`, `IRenderer2d` / `Renderer2d`, `IRenderer3d` / `Renderer3d`, `ISimpleRenderer` / `SimpleRenderer`, `IInputDevice` / `InputDevice`, `IMeshResourceCmo` / `MeshResourceCmo`, `ITexture` / `Texture`, `IPsResource`, `IVsResource`, `IFontResource`, `IFbxResource`, `IGltfResource`, `ICamera3`, plus per-shader `ISimpleShader`/`ISkydomeShader`/`ISpriteShader`/`IOceanShader`.

This means: never `#include "RenderDevice.h"` from a client header — include `IRenderDevice.h` (or the forward decl from `LaggyDxFwd.h`) and let the cpp call the factory.

### Forward declarations live in `LaggyDxFwd.h`

`LaggyDxFwd.h` is the canonical forward-declaration header for the entire `Dx` namespace plus a few D3D/ofbx types. Includes it in headers instead of pulling full type headers. It also defines the small public aliases `using Color = Sdk::Vector4F;`, `using Action = std::function<void()>;`, the `ConsoleLineType` enum and `ConsoleLine` / `ConsoleLines`, and the `Dx::thd` aliases (`GasId`, `GasName`, `GasesMap`, `GasPrototypePtr`, etc.).

### Resource pipeline

`IResourceController` is the asset cache. Construction scans `AppSettings::assetsFolder` and indexes files into a `std::unordered_map<std::string, std::shared_ptr<ILoadableResource>>`. Concrete resource types (`MeshResourceCmo`, `Texture`, `PsBinResource`/`PsHlslResource`/`PsResource`, `VsBinResource`/`VsHlslResource`/`VsResource`, `FontResource`, `FbxResource`, `GltfResource`) all implement `ILoadableResource` (`loadIfNeeded`/`unloadIfNeeded`). Callers fetch by path: `getTexture("name")`, `getMeshCmo(...)`, `getPs(...)`, etc. Use `hasResource(...)` to probe.

Format support: `.cmo` (DirectXTK Compiled Mesh Object) via `MeshResourceCmo` + `MeshLoader`; FBX via the bundled `ofbx.cpp/h` + `OfbxSceneWrapper`; glTF via bundled `tiny_gltf.h`; image loading via bundled `stb_image.h` / `stb_image_write.h`; ZIP via bundled `miniz.cpp/h`; JSON via bundled `json.hpp`. These bundled third-party single-file libraries are vendored — don't reformat them.

### Rendering

Two parallel pipelines:

- **2D**: `IRenderer2d` wraps DirectXTK `SpriteBatch` + primitive batches. `beginScene(...)` overloads accept translation/rotation/scale; `renderSprite`, `renderLine`, `renderRect`, `renderText` are the primary ops. `Renderer2dGuard` is a RAII begin/end pair — use it instead of manual `beginScene`/`endScene` where possible (the engine itself uses it for `renderGui`).
- **3D**: `IRenderer3d` (created with an `ICamera3&`) renders objects given either a `VertexBuffer`+`IndexBuffer`+`MaterialSpan`s, or a `IMeshResourceCmo` plus optional animation controller. `ISimpleRenderer` is a lower-level alternative.

Per-shader wrappers (`SimpleShader`, `InstancedShader`, `SkydomeShader`, `SpriteShader`, `OceanShader`) bind the generated VS/PS blobs plus constant buffers from `ShaderBuffers.h`. `ShaderBase` is the common base; `ShaderWrapper` and `_ShaderUtils.h` hold helpers. `TerrainShader` also has an optional world-space grid overlay (`ITerrainShader::setGridCellSize`, 0 hides it; line tunables in `TerrainShader.ps.hlsl`).

**Hardware instancing**: `IInstancedShader` draws one model N times per call (`draw(object, instanceBuffer)`, `DrawIndexedInstanced`). Per-instance transforms (`InstancePosRotScale` in `InstanceTypes.h`: position + Y-rotation + uniform scale) live in an `IInstanceBuffer` bound to vertex-buffer slot 1 via `getVertexLayoutPos3NormTextInstanced()`. The instanced VS applies the per-instance transform instead of the world matrix and outputs the same struct as `SimpleShader.ps.hlsl`, which it reuses as its pixel stage — keep the two in sync. Instance buffers are immutable; recreate to change the instance set.

The render device's `begin`/`end` and `Sdk::Locker`/`Sdk::ScopeGuard` integration come from `Sdk::ILockable` — `IRenderDevice` inherits it so the `App::mainloop` render block can wrap rendering in a single RAII scope.

**Shadow mapping**: `ShadowCamera` (plain value class) holds the directional light's view + texel-snapped orthographic projection around a focus point; `IShadowMap` / `ShadowMap` is the offscreen non-MSAA depth target (`bind()` redirects rendering into it, `unbind()` restores the backbuffer). `IDepthShader` / `IInstancedDepthShader` render `IObject3`s / instanced models into it depth-only (shared alpha-clip PS so cutout textures cast cutout shadows; rasterizer depth-bias constants at the top of `DepthShader.cpp` / `InstancedDepthShader.cpp` are the acne knobs). `TerrainShader`, `SimpleShader`, and `InstancedShader` receive shadows: `setShadowMap(...)` + `setShadowCamera(...)` bind the map (terrain t3, simple/instanced t1; comparison sampler at s1, light matrix cbuffer at b2) and the PS applies 3x3 PCF; they default to `white.png` (fully lit) until wired. The game drives the pass: update the `ShadowCamera` focus, `shadowMap.bind()`, draw casters with the depth shaders, `unbind()`, then render normally.

### GUI: retained-mode control tree

`IControl` (in `IControl.h`) is the GUI base interface. It inherits both `Sdk::TreeNode` (parent/child hierarchy from `LaggySdk`) and `Sdk::EventHandler` — meaning every control is both a tree node and an event participant. The concrete base is `Control`, which holds position, size, opacity, visibility, and a `std::vector<std::shared_ptr<IGuiEffect>>` of effects.

`Form` is the empty top-level control owned by `App` (`App::getForm()`). The standard widget set: `Label`, `Panel`, `Layout`, `Button`, `Checkbox`, `Slider`, `RadioButton`, `RadioGroup`, `Grid` (with `GridItem`), `Text`. Effects live in `IGuiEffect` / `GuiEffect` / `FadeOutEffect`.

`App::updateGui` calls `getForm().update(dt)` and `App::renderGui` walks the tree via `Renderer2dGuard`. Mouse events bubble through `onMouseMove`/`onMouseClick`/`onMouseRelease` and short-circuit: if the form consumes the click, the global `ActionsMap` is not invoked.

### Input

`IInputDevice` polls the keyboard/mouse and produces `KeyboardState` / `MouseState`. Keys are the `KeyboardKey` enum (`KeyboardKeys.h`) and `MouseKey` (`MouseKeys.h`); `KeyUtils.h` has helpers. `MouseMode` toggles between absolute and relative — in relative mode the app re-centers the cursor each frame (`resetRelativeMouseState`).

`ActionsMap` (`ActionsMap.h`) maps `(KeyboardKey, ActionType)` and `(MouseKey, ActionType)` pairs to `Action` (i.e. `std::function<void()>`) callbacks. `ActionType` is `OnPress` / `OnRelease` / `Continuous`. `App::onKeyPressed`/`onKeyReleased`/`onKeyCurrent` (and the mouse equivalents) both `notify(...)` an event AND look up an action in the map. Set the map via `App::setActionsMap(...)`.

The input device is also connected into the SDK event graph (`connectTo(*d_inputDevice)` in `App`'s constructor), so events like `MouseModeChangedEvent` propagate to `App::processEvent`.

### Collision

`ICollider` is the base, with concrete `StaticCollider`, `DynamicCollider`, `KinematicCollider`. `CollisionManager` coordinates checks. Shapes split 2D/3D: `IShape2d` (with `Shape2d`) and `IShape3d` (with `Shape3d`, plus per-shape-family cpp files `IShape3d_planes.cpp`, `IShape3d_cuboids.cpp`, `IShape3d_spheroids.cpp`, `IShape3d_cylinders.cpp` — the last adds `IShape3d::cylinder(radius, height, slices, …)`, a Y-axis cylinder base-anchored at `y=0`). `Aabb`, `Obb`, `Tri` are geometric primitives. `CollShapeMover` integrates motion against shapes; `CollisionFunctor`/`CollisionInfo`/`CollisionPointNormal` are the callback/result types.

### Cameras

`ICamera2` (2D) and `ICamera3` (3D) interfaces, plus concrete `Camera2`, `Camera3Base`, `FirstPersonCamera`, `ThirdPersonCamera`, and free-fly controllers `FreeCamera2Controller` / `FreeCamera3Controller`. `Projector.h` projects between world and screen.

### Object hierarchies

`IObject` is the most generic base; `IObject2` (with `Object2Base`) is the 2D-positioned object, `IObject3` (with `Object3`) is the 3D-positioned object. `ObjectCollection.h` defines container aliases.

### Sprites & 2D scene

`ISprite` / `Sprite` and `AnimatedSprite` (+ `Animation`, `Animation2Player`, `ImageAnimation`, `ImageDescription`, `Channels`/`ChannelInfo`) form the 2D-animation system. `Sprite_save.cpp` / `AnimatedSprite_save.cpp` handle persistence.

### Terrain / heightmap / ROAM

`HeightMap`, `Roam` (real-time optimally-adapting meshes), and `Tri` together implement an LOD'd terrain mesher. `DefaultMeshes` provides built-in primitives (cube, plane, sphere). `Geometry.h` has free math helpers.

### `Dx::thd`: thermodynamic tile simulation

A separate sub-namespace under `Dx::thd` for a tile-based thermal/gas simulation. `Simulation::update(dt, ITileCollection&)` iterates tiles and calls `exchangeHeat` / `exchangeGas` between neighbors. `IThdObject` / `ITile` / `ITileCollection` are the abstractions; `GasUnit` + `GasPrototype` model gas mixtures.

### Console & FPS

`Console` is a debug overlay with `ConsoleLineType` (Debug/Info/Error) entries; `App::getFpsCounter()` exposes the `Sdk::FpsCounter`.

## Building a 2D game with LaggyDx

The sibling `AllodsApp` project (`..\AllodsApp\AllodsApp`) is the canonical example. The pattern below is taken from it.

### Entry point and `App` subclass

`WinMain` is one line — construct your `App` subclass and call `run()`:

```cpp
int WinMain(HINSTANCE, HINSTANCE, LPSTR, int) { Game().run(); return 0; }
```

Subclass `Dx::App`, pass an `AppSettings` (resolution, assets folder) to the base constructor, and override the protected virtuals. The minimum set for a 2D game is `onStart`, `update(double dt)`, `render()`, plus whichever input hooks you need (`onMouseMove`, `onMouseClick`, `onMouseRelease` — note both click hooks return `bool` where `true` means consumed):

```cpp
class Game : public Dx::App {
public:
  Game() : Dx::App(makeSettings()), d_gui(*this), d_actions(*this) {}
private:
  void onStart() override;
  void update(double dt) override;
  void render() override;
  bool onMouseClick(Dx::MouseKey k) override;
};
```

### Main loop (what `App::run()` already does for you)

You do not write the loop. `App::run()` calls `onStart()`, then per frame: ticks the timer, polls `IInputDevice` and dispatches `onKey*`/`onMouse*` (which both fire SDK events AND look up callbacks in the `ActionsMap`), calls `notify(OnUpdate(dt))`, then `update(dt)` → `updateGui(dt)` → render block → `render()` → `renderGui()`. The render block is already wrapped in a `Sdk::Locker(*renderDevice)` + `Sdk::ScopeGuard(*renderDevice)`, so your `render()` override just issues draws.

Your `update(double dt)` typically forwards to your simulation: tick the session, the GUI manager, and the view controller. Your `render()` typically forwards to the view controller. See `AllodsApp/Game.cpp` for the canonical shape.

### Input: `ActionsMap` and direct overrides

Two ways to consume input, used together:

1. **`Dx::ActionsMap`** — declarative map of `(KeyboardKey | MouseKey, ActionType)` → `Action` (`std::function<void()>`). `ActionType` is `OnPress` / `OnRelease` / `Continuous`. Build a map and install it with `App::setActionsMap(std::move(map))`. Swap maps when game state changes (main menu vs in-game vs dev mode). Example from `AllodsApp/ActionsController.cpp`:

   ```cpp
   Dx::ActionsMap actions;
   actions.setAction(Dx::KeyboardKey::Escape, [&]{ session.onEscape(); }, Dx::ActionType::OnPress);
   actions.setAction(Dx::KeyboardKey::F1, [&]{ session.enableDevMode(); }, Dx::ActionType::OnPress);
   d_game.setActionsMap(std::move(actions));
   ```

2. **Override the virtuals** when you need to inspect the event, consume it conditionally, or route it through layered systems (GUI first, world second). The GUI tree already short-circuits mouse events: if a control consumes the click, `App::onMouseClick` returns `true` and the `ActionsMap` is NOT invoked. Your override should follow the same protocol — call the base first, then your world handler:

   ```cpp
   bool Game::onMouseClick(Dx::MouseKey k) {
     if (Dx::App::onMouseClick(k)) return true;       // GUI ate it
     if (d_session && d_session->onMouseClick(k)) return true;
     return false;
   }
   ```

Cursor position comes from `Dx::CursorUtils::getPosition()` (returns `Vector2I`). For relative-mouse / FPS-style input, switch `MouseMode` and the engine re-centers each frame.

### Rendering 2D objects

Use `Dx::ISpriteShader` for sprite-based 2D rendering. Create it once per scene with a camera:

```cpp
d_shader = Dx::ISpriteShader::create(&camera);  // ICamera2&
```

Then per frame, build a `Dx::Sprite` or `Dx::AnimatedSprite`, configure it, and call `shader.draw(sprite)`. Textures come from the resource controller — fetch by filename relative to `AppSettings::assetsFolder`:

```cpp
const auto* tex = Dx::App::get().getResourceController().getTexture("Space.png");
Dx::AnimatedSprite sprite;
sprite.setTexture(tex);
sprite.resetSizeToTexture();
sprite.setCurrentFrame(animFrame);
sprite.setPosition(worldPos - sprite.getSize() / 2);  // Vector2F
shader.draw(sprite);
```

For scrolling backgrounds, pass a `Dx::UvOffset` to `shader.draw(sprite, &uvOffset, /*ignoreCamera=*/true)` — see `BackgroundView.cpp`. The camera (`ICamera2`) gives you `getOffset()` in world space; sprites' positions are world-space `Vector2F` and the shader applies the camera transform.

Typical organization: one `View` class per kind of thing (`TileView`, `ObjectView`, `BackgroundView`, …) holding only render code. A central `ViewController::render()` walks the world and calls the views in z-order. State (positions, animation frames) lives on the game-side objects, not the views.

### GUI: building the control tree

The root `Dx::Form` is owned by `App` and reachable via `App::getForm()`. Build a UI by constructing controls as `std::shared_ptr`, configuring them, and `addChild`-ing them to a parent. The engine handles updating and rendering automatically (`App::updateGui` / `App::renderGui`).

A small factory module per project (see `AllodsApp/GuiCreator.cpp`) is the recommended pattern — it lets you apply a consistent skin (fonts, button textures) in one place:

```cpp
Dx::Button& GuiCreator::createMenuButton(Dx::IControl& parent) {
  auto ctrl = std::make_shared<Dx::Button>();
  parent.addChild(ctrl);
  ctrl->setFont(Fonts::getMenuFont());
  ctrl->setTextureName("Button.png",        Dx::ButtonState::Normal);
  ctrl->setTextureName("ButtonLight.png",   Dx::ButtonState::Hovered);
  ctrl->setTextureName("ButtonPressed.png", Dx::ButtonState::Pressed);
  ctrl->resetSizeToTexture();
  return *ctrl;
}
```

Example main-menu construction (from `GuiManager_mainMenu.cpp`):

```cpp
auto& bg = GuiCreator::createPanel(d_game.getForm());
bg.setTexture("Black.png");
bg.setSize(getResolution());

auto& layout = GuiCreator::createLayout(d_game.getForm());
layout.setSize(getResolution());
layout.setAlign(Dx::LayoutAlign::TopToBottom_Center);

auto& btn = GuiCreator::createMenuButton(layout);
btn.setText("Start New Game");
btn.setOnPress(std::bind(&Game::newSession, &d_game));
```

Tear it down with `d_game.getForm().removeChildren()` when switching screens. Available widgets are listed in the "GUI: retained-mode control tree" section above (`Label`, `Panel`, `Layout`, `Button`, `Checkbox`, `Slider`, `RadioButton`, `RadioGroup`, `Grid`, `Text`). Screen resolution for layout sizing: `App::get().getRenderDevice().getResolution()` (returns `Vector2I` — convert with `.getVector<float>()`).

### Wiring subsystems together: events

Most cross-subsystem communication uses `Sdk::EventHandler`. Your `App` subclass is already an `EventHandler`; controllers and managers `connectTo(parent)` in their constructor and override `processEvent(const Sdk::IEvent&)` with a `dynamic_cast` chain. This is how `ActionsController`, `GuiManager`, and `ViewController` all keep their state in sync with `Game` and `Session` without owning each other directly.

## Conventions worth matching

Same as the sibling `LaggySdk` project:

- Parameter naming: `i_` for in, `o_` for out, `a_` for accumulator/in-out, `d_` for data members, `s_` for statics. Stick to these.
- Two-space indentation, Allman braces, `namespace Dx { ... } // ns Dx` footer comment on the closing brace.
- Headers use `#pragma once`. Prefer including `LaggyDxFwd.h` over full headers in other headers.
- Use the `CONTRACT_ASSERT` / `CONTRACT_EXPECT` / `CONTRACT_ENSURE` / `CONTRACT_THROW` / `SAFE_DEREF` macros from `<LaggySdk/Contracts.h>` instead of `assert`, raw `throw`, or ad-hoc null checks. They are already used pervasively (see `App.cpp`).
- For subsystems with a factory: include the `IFoo` header and call `IFoo::create(...)`. Don't include the concrete `Foo.h` from client code.
- Because `TreatWarningAsError=true`, even `-Wsign-compare`-style warnings fail the build — clean them as you go.

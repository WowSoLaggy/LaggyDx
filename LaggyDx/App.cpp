#include "stdafx.h"
#include "App.h"

#include "AppEvents.h"
#include "AppSettings.h"
#include "Form.h"
#include "ICollider.h"
#include "InputEvents.h"
#include "IObject.h"
#include "IObject2.h"
#include "IObject3.h"
#include "KeyboardState.h"
#include "KeyUtils.h"
#include "Renderer2dGuard.h"
#include "TextRenderer.h"

#include <LaggySdk/Cursor.h>
#include <LaggySdk/HandleMessages.h>
#include <LaggySdk/Locker.h>
#include <LaggySdk/Random.h>
#include <LaggySdk/ScopeGuard.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  App* App::s_this = nullptr;
  App& App::get()
  {
    CONTRACT_ASSERT(s_this);
    return *s_this;
  }

  App::App(std::unique_ptr<AppSettings> i_settings)
    : d_settings(std::move(i_settings))
  {
    s_this = this;

    const auto hres = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    CONTRACT_ASSERT(!FAILED(hres));

    const Sdk::Vector2I resolution = { getSettings().screenWidth, getSettings().screenHeight };

    d_window = std::make_unique<Sdk::Window>(resolution, getSettings().applicationName);

    d_renderDevice = IRenderDevice::create(d_window->getHWnd(), resolution, getSettings().refreshRate, getSettings().debugMode);
    CONTRACT_ENSURE(d_renderDevice);

    d_resourceController = IResourceController::create(*d_renderDevice, getSettings().assetsFolder);
    CONTRACT_ENSURE(d_resourceController);

    d_renderer2d = IRenderer2d::create(*d_renderDevice, resolution);
    CONTRACT_ENSURE(d_renderer2d);

    d_textRenderer = std::make_unique<TextRenderer>();
    CONTRACT_ENSURE(d_textRenderer);

    d_inputDevice = IInputDevice::create(d_window->getHWnd());
    CONTRACT_ENSURE(d_inputDevice);
    connectTo(*d_inputDevice);

    d_form = std::make_unique<Form>();

    Sdk::setCursorToCenter(*d_window);
    d_window->show();
  }

  App::~App()
  {
    CoUninitialize();
  }


  const AppSettings& App::getSettings() const
  {
    return SAFE_DEREF(d_settings);
  }

  IInputDevice& App::getInputDevice() { return SAFE_DEREF(d_inputDevice); }
  const IInputDevice& App::getInputDevice() const { return SAFE_DEREF(d_inputDevice); }
  IRenderDevice& App::getRenderDevice() { return SAFE_DEREF(d_renderDevice); }
  const IRenderDevice& App::getRenderDevice() const { return SAFE_DEREF(d_renderDevice); }
  IResourceController& App::getResourceController() { return SAFE_DEREF(d_resourceController); }
  const IResourceController& App::getResourceController() const { return SAFE_DEREF(d_resourceController); }
  IRenderer2d& App::getRenderer2d() { return SAFE_DEREF(d_renderer2d); }
  const IRenderer2d& App::getRenderer2d() const { return SAFE_DEREF(d_renderer2d); }
  ITextRenderer& App::getTextRenderer() { return SAFE_DEREF(d_textRenderer); }
  const ITextRenderer& App::getTextRenderer() const { return SAFE_DEREF(d_textRenderer); }

  ActionsMap& App::getActionsMap() { return d_actionsMap; }
  const ActionsMap& App::getActionsMap() const { return d_actionsMap; }
  void App::setActionsMap(ActionsMap i_actionsMap) { d_actionsMap = std::move(i_actionsMap); }

  IControl& App::getForm() { return SAFE_DEREF(d_form); }
  const IControl& App::getForm() const { return SAFE_DEREF(d_form); }

  double App::getGlobalTime() const { return d_globalTime; }
  const Sdk::FpsCounter& App::getFpsCounter() const { return d_fpsCounter; }

  const MouseState& App::getMouseState() const { return d_mouseState; }


  void App::processEvent(const Sdk::IEvent& i_event)
  {
    if (const auto* event = dynamic_cast<const MouseModeChangedEvent*>(&i_event))
    {
      if (event->getMode() == MouseMode::Relative)
        resetRelativeMouseState();
    }
  }


  void App::run()
  {
    onStart();

    d_timer.start();

    while (continueLoop())
      mainloop();

    onEnd();
  }

  void App::stop()
  {
    d_stop = true;
  }


  void App::onStart()
  {
  }

  void App::onEnd()
  {
  }

  bool App::continueLoop()
  {
    if (d_stop)
      return false;

    CONTRACT_EXPECT(d_inputDevice);
    if (!Sdk::handleMessages(
      std::bind(&IInputDevice::processMessage, std::ref(*d_inputDevice), std::placeholders::_1)))
    {
      return false;
    }

    return true;
  }

  void App::mainloop()
  {
    double dt = d_timer.restart();
    d_globalTime += dt;
    d_fpsCounter.udpate(dt);

    handleKeyboard(d_inputDevice->checkKeyboard());
    handleMouse(d_inputDevice->checkMouse());

    notify(OnUpdate(dt));
    update(dt);
    updateGui(dt);

    {
      CONTRACT_ASSERT(d_renderDevice);
      const Sdk::Locker scopeLocker(*d_renderDevice);
      Sdk::ScopeGuard scopeGuardRenderDevice(*d_renderDevice);
      Sdk::ScopeGuard scopeGuardTextRenderer(SAFE_DEREF(d_textRenderer));

      render();
      renderGui();
    }
  }


  void App::update(const double i_dt)
  {
  }

  void App::updateGui(const double i_dt)
  {
    getForm().update(i_dt);
  }


  void App::render()
  {
  }

  void App::renderGui()
  {
    Renderer2dGuard renderer2dGuard(*d_renderer2d);
    getForm().render(*d_renderer2d);
  }


  void App::handleKeyboard(const KeyboardState& i_keyboardState)
  {
    for (const auto key : getKeys(i_keyboardState.getPressedKeys()))
      onKeyPressed(key);

    for (const auto key : getKeys(i_keyboardState.getReleasedKeys()))
      onKeyReleased(key);

    for (const auto key : getKeys(i_keyboardState.getCurrentKeys()))
      onKeyCurrent(key);
  }

  void App::handleMouse(MouseState i_mouseState)
  {
    if (
      i_mouseState.getMode() == d_mouseState.getMode() &&
      i_mouseState.getPosition() != d_mouseState.getPosition())
      onMouseMove(i_mouseState.getPosition() - d_mouseState.getPosition());

    if (i_mouseState.getWheelPositionChange() != 0)
      onMouseWheel(i_mouseState.getWheelPositionChange());

    for (const auto key : { MouseKey::Left, MouseKey::Right, MouseKey::Middle, MouseKey::X1, MouseKey::X2 })
    {
      const auto state = i_mouseState.getButtonState(key);
      if (state == MouseButtonState::Pressed)
        onMouseClick(key);
      else if (state == MouseButtonState::Released)
        onMouseRelease(key);
    }

    d_mouseState = std::move(i_mouseState);
    if (d_mouseState.getMode() == MouseMode::Relative)
      resetRelativeMouseState();
  }


  void App::onKeyPressed(KeyboardKey i_key)
  {
    notify(OnKeyPressedEvent(i_key));
    if (const auto* action = d_actionsMap.getAction(i_key, ActionType::OnPress))
      action->operator()();
  }

  void App::onKeyReleased(KeyboardKey i_key)
  {
    notify(OnKeyReleasedEvent(i_key));
    if (const auto* action = d_actionsMap.getAction(i_key, ActionType::OnRelease))
      action->operator()();
  }

  void App::onKeyCurrent(KeyboardKey i_key)
  {
    notify(OnKeyCurrentEvent(i_key));
    if (const auto* action = d_actionsMap.getAction(i_key, ActionType::Continuous))
      action->operator()();
  }


  void App::onMouseMove(Sdk::Vector2I i_moveDiff)
  {
    notify(OnMouseMovedEvent(std::move(i_moveDiff)));
    getForm().onMouseMove();
  }

  void App::onMouseWheel(const int i_distance)
  {
    const auto mkey = i_distance > 0 ? MouseKey::WheelUp : MouseKey::WheelDown;
    if (const auto* action = d_actionsMap.getAction(mkey, ActionType::OnPress))
      action->operator()();
  }

  bool App::onMouseClick(MouseKey i_key)
  {
    if (getForm().onMouseClick(i_key))
      return true;

    if (const auto* action = d_actionsMap.getAction(i_key, ActionType::OnPress))
    {
      action->operator()();
      return true;
    }

    return false;
  }

  bool App::onMouseRelease(MouseKey i_key)
  {
    if (getForm().onMouseRelease(i_key))
      return true;

    if (const auto* action = d_actionsMap.getAction(i_key, ActionType::OnRelease))
    {
      action->operator()();
      return true;
    }

    return false;
  }


  void App::resetRelativeMouseState()
  {
    d_mouseState.setMode(MouseMode::Relative);
    d_mouseState.resetPosition();
  }

} // ns Dx

#include "stdafx.h"
#include "Game.h"

#include "Form.h"
#include "GameSettings.h"
#include "GameEvents.h"
#include "ICollider.h"
#include "InputEvents.h"
#include "IObject.h"
#include "IObject2.h"
#include "IObject3.h"
#include "KeyboardState.h"
#include "KeyUtils.h"
#include "Renderer2dGuard.h"

#include <LaggySdk/Cursor.h>
#include <LaggySdk/HandleMessages.h>
#include <LaggySdk/Locker.h>
#include <LaggySdk/Random.h>
#include <LaggySdk/Vector.h>

namespace Dx
{
  Game* Game::s_this = nullptr;
  Game& Game::get()
  {
    CONTRACT_ASSERT(s_this);
    return *s_this;
  }

  Game::Game(const GameSettings& i_gameSettings)
  {
    const auto hres = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    CONTRACT_ASSERT(!FAILED(hres));

    const Sdk::Vector2I resolution = { i_gameSettings.screenWidth, i_gameSettings.screenHeight };

    d_window = std::make_unique<Sdk::Window>(resolution, i_gameSettings.applicationName);

    d_renderDevice = IRenderDevice::create(d_window->getHWnd(), resolution, i_gameSettings.refreshRate, i_gameSettings.debugMode);
    CONTRACT_ENSURE(d_renderDevice);

    d_resourceController = IResourceController::create(*d_renderDevice, i_gameSettings.assetsFolder);
    CONTRACT_ENSURE(d_resourceController);

    d_renderer2d = IRenderer2d::create(*d_renderDevice, resolution);
    CONTRACT_ENSURE(d_renderer2d);

    d_inputDevice = IInputDevice::create(d_window->getHWnd());
    CONTRACT_ENSURE(d_inputDevice);
    connectTo(*d_inputDevice);

    d_form = std::make_unique<Form>();

    Sdk::setCursorToCenter();
    d_window->show();

    s_this = this;
  }

  Game::~Game()
  {
    CoUninitialize();
  }


  IInputDevice& Game::getInputDevice() { return *d_inputDevice; }
  const IInputDevice& Game::getInputDevice() const { return *d_inputDevice; }
  IRenderDevice& Game::getRenderDevice() { return *d_renderDevice; }
  const IRenderDevice& Game::getRenderDevice() const { return *d_renderDevice; }
  IResourceController& Game::getResourceController() { return *d_resourceController; }
  const IResourceController& Game::getResourceController() const { return *d_resourceController; }
  IRenderer2d& Game::getRenderer2d() { return *d_renderer2d; }
  const IRenderer2d& Game::getRenderer2d() const { return *d_renderer2d; }

  ObjectCollection<IObject2>& Game::getObject2Collection() { return d_object2Collection; }
  const ObjectCollection<IObject2>& Game::getObject2Collection() const { return d_object2Collection; }

  ActionsMap& Game::getActionsMap() { return d_actionsMap; }
  const ActionsMap& Game::getActionsMap() const { return d_actionsMap; }
  void Game::setActionsMap(ActionsMap i_actionsMap) { d_actionsMap = std::move(i_actionsMap); }

  IControl& Game::getForm() { return *d_form; }
  const IControl& Game::getForm() const { return *d_form; }

  double Game::getGlobalTime() const { return d_globalTime; }
  const Sdk::FpsCounter& Game::getFpsCounter() const { return d_fpsCounter; }

  const MouseState& Game::getMouseState() const { return d_mouseState; }


  void Game::processEvent(const Sdk::IEvent& i_event)
  {
    if (const auto* event = dynamic_cast<const MouseModeChangedEvent*>(&i_event))
    {
      if (event->getMode() == MouseMode::Relative)
        resetRelativeMouseState();
    }
  }


  void Game::run()
  {
    onGameStart();

    d_timer.start();

    while (continueLoop())
      mainloop();

    onGameEnd();
  }

  void Game::stop()
  {
    d_stop = true;
  }


  void Game::onGameStart()
  {
  }

  void Game::onGameEnd()
  {
  }

  bool Game::continueLoop()
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

  void Game::mainloop()
  {
    double dt = d_timer.restart();
    d_globalTime += dt;
    d_fpsCounter.udpate(dt);

    handleKeyboard(d_inputDevice->checkKeyboard());
    handleMouse(d_inputDevice->checkMouse());

    update(dt);

    {
      const Sdk::Locker scopeLocker(*d_renderDevice);

      d_renderDevice->beginScene();
      render();
      d_renderDevice->endScene();
    }
  }


  void Game::update(const double i_dt)
  {
    notify(OnGameUpdate(i_dt));
    updateObjects(i_dt);
    updateGui(i_dt);
  }

  void Game::updateObjects(const double i_dt)
  {
    for (const auto& obj : d_object2Collection.getObjects())
    {
      beforeObjectUpdate(*obj, i_dt);
      obj->update(i_dt);
      if (const auto collider = obj->getCollider())
        collider->update(i_dt);
      afterObjectUpdate(*obj, i_dt);
    }

    d_collisionManager.checkCollisions(d_object2Collection.getObjects());
  }

  void Game::beforeObjectUpdate(IObject& i_obj, double i_dt)
  {
  }

  void Game::afterObjectUpdate(IObject& i_obj, double i_dt)
  {
  }

  void Game::updateGui(const double i_dt)
  {
    getForm().update(i_dt);
  }


  void Game::render()
  {
    renderObjects();
    renderGui();
  }

  void Game::renderObjects()
  {
    for (const auto& obj : d_object2Collection.getObjects())
      renderObject(*obj);
  }

  void Game::renderObject(const IObject& i_obj)
  {
    if (const auto* obj2 = dynamic_cast<const IObject2*>(&i_obj))
      obj2->render(*d_renderer2d);
  }


  void Game::renderGui()
  {
    Renderer2dGuard renderer2dGuard(*d_renderer2d);
    getForm().render(*d_renderer2d);
  }


  void Game::handleKeyboard(const KeyboardState& i_keyboardState)
  {
    for (const auto key : getKeys(i_keyboardState.getPressedKeys()))
    {
      notify(OnKeyPressedEvent(key));
      if (const auto* action = d_actionsMap.getAction(key, ActionType::OnPress))
        action->operator()();
    }

    for (const auto key : getKeys(i_keyboardState.getReleasedKeys()))
    {
      notify(OnKeyReleasedEvent(key));
      if (const auto* action = d_actionsMap.getAction(key, ActionType::OnRelease))
        action->operator()();
    }

    for (const auto key : getKeys(i_keyboardState.getCurrentKeys()))
    {
      notify(OnKeyCurrentEvent(key));
      if (const auto* action = d_actionsMap.getAction(key, ActionType::Continuous))
        action->operator()();
    }
  }

  void Game::handleMouse(MouseState i_mouseState)
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

  void Game::onMouseMove(Sdk::Vector2I i_move)
  {
    notify(OnMouseMovedEvent(std::move(i_move)));
    getForm().onMouseMove();
  }

  void Game::onMouseWheel(const int i_distance)
  {
    const auto mkey = i_distance > 0 ? MouseKey::WheelUp : MouseKey::WheelDown;
    if (const auto* action = d_actionsMap.getAction(mkey, ActionType::OnPress))
      action->operator()();
  }

  void Game::onMouseClick(MouseKey i_key)
  {
    getForm().onMouseClick(i_key);

    if (const auto* action = d_actionsMap.getAction(i_key, ActionType::OnPress))
      action->operator()();
  }

  void Game::onMouseRelease(MouseKey i_key)
  {
    getForm().onMouseRelease(i_key);

    if (const auto* action = d_actionsMap.getAction(i_key, ActionType::OnRelease))
      action->operator()();
  }


  void Game::resetRelativeMouseState()
  {
    d_mouseState.setMode(MouseMode::Relative);
    d_mouseState.resetPosition();
  }

} // ns Dx

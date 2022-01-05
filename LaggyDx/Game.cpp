#include "stdafx.h"
#include "Game.h"

#include "ICollider.h"
#include "GameSettings.h"
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
    CONTRACT_ASSERT(hres == S_OK);

    Sdk::randomize();

    const Sdk::Vector2I resolution = { i_gameSettings.screenWidth, i_gameSettings.screenHeight };

    d_window = std::make_unique<Sdk::Window>(resolution, i_gameSettings.applicationName);

    d_renderDevice = IRenderDevice::create(d_window->getHWnd(), resolution);
    CONTRACT_ENSURE(d_renderDevice);

    d_resourceController = IResourceController::create(*d_renderDevice, i_gameSettings.assetsFolder);
    CONTRACT_ENSURE(d_resourceController);

    d_renderer2d = IRenderer2d::create(*d_renderDevice, resolution);
    CONTRACT_ENSURE(d_renderer2d);

    d_inputDevice = IInputDevice::create(d_window->getHWnd());
    CONTRACT_ENSURE(d_inputDevice);

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

  Control& Game::getForm() { return d_form; }
  const Control& Game::getForm() const { return d_form; }


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
    if (!Sdk::handleMessages(std::bind(&Dx::IInputDevice::processMessage, std::ref(*d_inputDevice), std::placeholders::_1)))
      return false;

    return true;
  }

  void Game::mainloop()
  {
    double dt = d_timer.restart();

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
    d_form.update(i_dt);
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
    d_form.render(*d_renderer2d);
  }


  void Game::handleKeyboard(const Dx::KeyboardState& i_keyboardState)
  {
    for (const auto key : getKeys(i_keyboardState.getPressedKeys()))
    {
      if (const auto* action = d_actionsMap.getAction(key, ActionType::OnPress))
        action->operator()();
    }

    for (const auto key : getKeys(i_keyboardState.getReleasedKeys()))
    {
      if (const auto* action = d_actionsMap.getAction(key, ActionType::OnRelease))
        action->operator()();
    }

    for (const auto key : getKeys(i_keyboardState.getCurrentKeys()))
    {
      if (const auto* action = d_actionsMap.getAction(key, ActionType::Continuous))
        action->operator()();
    }
  }

  void Game::handleMouse(const Dx::MouseState& i_mouseState)
  {
    if (i_mouseState.getPosition() != d_mouseState.getPosition())
      onMouseMove();

    for (const auto key : { MouseKey::Left, MouseKey::Right, MouseKey::Middle, MouseKey::X1, MouseKey::X2 })
    {
      const auto state = i_mouseState.getButtonState(key);
      if (state == Dx::MouseButtonState::Pressed)
        onMouseClick(key);
      else if (state == Dx::MouseButtonState::Released)
        onMouseRelease(key);
    }

    d_mouseState = i_mouseState;
  }

  void Game::onMouseMove()
  {
    d_form.onMouseMove();
  }

  void Game::onMouseClick(MouseKey i_key)
  {
    d_form.onMouseClick(i_key);
  }

  void Game::onMouseRelease(MouseKey i_key)
  {
    d_form.onMouseRelease(i_key);
  }

} // ns Dx

#include "stdafx.h"
#include "Game.h"

#include "GameSettings.h"
#include "IObject.h"
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


  [[nodiscard]] IInputDevice& Game::getInputDevice() { return *d_inputDevice; }
  [[nodiscard]] const IInputDevice& Game::getInputDevice() const { return *d_inputDevice; }
  [[nodiscard]] IRenderDevice& Game::getRenderDevice() { return *d_renderDevice; }
  [[nodiscard]] const IRenderDevice& Game::getRenderDevice() const { return *d_renderDevice; }
  [[nodiscard]] IResourceController& Game::getResourceController() { return *d_resourceController; }
  [[nodiscard]] const IResourceController& Game::getResourceController() const { return *d_resourceController; }
  [[nodiscard]] IRenderer2d& Game::getRenderer2d() { return *d_renderer2d; }
  [[nodiscard]] const IRenderer2d& Game::getRenderer2d() const { return *d_renderer2d; }

  [[nodiscard]] ObjectCollection& Game::getObjectCollection() { return d_objectCollection; }
  [[nodiscard]] const ObjectCollection& Game::getObjectCollection() const { return d_objectCollection; }

  [[nodiscard]] ActionsMap& Game::getActionsMap() { return d_actionsMap; }
  [[nodiscard]] const ActionsMap& Game::getActionsMap() const { return d_actionsMap; }
  void Game::setActionsMap(ActionsMap i_actionsMap) { d_actionsMap = std::move(i_actionsMap); }

  [[nodiscard]] Control& Game::getForm() { return d_form; }
  [[nodiscard]] const Control& Game::getForm() const { return d_form; }


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

    update(dt);

    {
      const Sdk::Locker scopeLocker(*d_renderDevice);

      d_renderDevice->beginScene();
      render();
      d_renderDevice->endScene();
    }

    std::this_thread::sleep_for(1ms);
  }


  void Game::update(const double i_dt)
  {
    updateObjects(i_dt);
    updateGui(i_dt);
  }

  void Game::updateObjects(const double i_dt)
  {
    for (const auto& obj : d_objectCollection.getObjects())
      updateObject(*obj, i_dt);
  }

  void Game::updateObject(IObject& i_obj, const double i_dt)
  {
    i_obj.update(i_dt);
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
    for (const auto& obj : d_objectCollection.getObjects())
      renderObject(*obj);
  }

  void Game::renderObject(const IObject& i_obj)
  {
    i_obj.render(*d_renderer2d);
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

} // ns Dx

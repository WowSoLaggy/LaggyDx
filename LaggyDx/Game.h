#pragma once

#include "ActionsMap.h"
#include "LaggyDxFwd.h"
#include "ObjectCollection.h"

#include <LaggyDx/IInputDevice.h>
#include <LaggyDx/IRenderDevice.h>
#include <LaggyDx/IRenderer2d.h>
#include <LaggyDx/IResourceController.h>
#include <LaggySdk/Timer.h>
#include <LaggySdk/Window.h>


namespace Dx
{
  class Game
  {
  public:
    static Game& get();

  public:
    explicit Game(const GameSettings& i_gameSettings);
    virtual ~Game();

    void run();

    [[nodiscard]] IInputDevice& getInputDevice();
    [[nodiscard]] IRenderDevice& getRenderDevice();
    [[nodiscard]] IResourceController& getResourceController();
    [[nodiscard]] IRenderer2d& getRenderer2d();

    [[nodiscard]] ObjectCollection& getObjectCollection();

    [[nodiscard]] ActionsMap& getActionsMap();
    [[nodiscard]] const ActionsMap& getActionsMap() const;
    void setActionsMap(ActionsMap i_actionsMap);

  protected:
    virtual void onGameStart();
    virtual void onGameEnd();

    [[nodiscard]] virtual bool continueLoop();

    virtual void update(double i_dt);
    virtual void updateObject(IObject& i_obj, double i_dt);

    virtual void renderObjects();
    virtual void renderObject(const IObject& i_obj);

    virtual void renderGui();

  private:
    static Game* s_this;

    Sdk::Timer d_timer;
    std::unique_ptr<Sdk::Window> d_window;
    std::unique_ptr<IInputDevice> d_inputDevice;
    std::unique_ptr<IRenderDevice> d_renderDevice;
    std::unique_ptr<IResourceController> d_resourceController;
    std::unique_ptr<IRenderer2d> d_renderer2d;

    ObjectCollection d_objectCollection;
    ActionsMap d_actionsMap;

    void mainloop();
    void handleKeyboard(const Dx::KeyboardState& i_keyboardState);
  };

} // ns Dx

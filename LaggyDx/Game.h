#pragma once

#include "ActionsMap.h"
#include "CollisionManager.h"
#include "IInputDevice.h"
#include "IRenderDevice.h"
#include "IRenderer2d.h"
#include "IResourceController.h"
#include "LaggyDxFwd.h"
#include "MouseState.h"
#include "ObjectCollection.h"

#include <LaggySdk/EventHandler.h>
#include <LaggySdk/FpsCounter.h>
#include <LaggySdk/Timer.h>
#include <LaggySdk/Vector.h>
#include <LaggySdk/Window.h>


namespace Dx
{
  class Game : public Sdk::EventHandler
  {
  public:
    static Game& get();

  public:
    explicit Game(const GameSettings& i_gameSettings);
    virtual ~Game();

    void run();
    void stop();

    void processEvent(const Sdk::IEvent& i_event) override;

    [[nodiscard]] IInputDevice& getInputDevice();
    [[nodiscard]] const IInputDevice& getInputDevice() const;
    [[nodiscard]] IRenderDevice& getRenderDevice();
    [[nodiscard]] const IRenderDevice& getRenderDevice() const;
    [[nodiscard]] IResourceController& getResourceController();
    [[nodiscard]] const IResourceController& getResourceController() const;
    [[nodiscard]] IRenderer2d& getRenderer2d();
    [[nodiscard]] const IRenderer2d& getRenderer2d() const;

    [[nodiscard]] ActionsMap& getActionsMap();
    [[nodiscard]] const ActionsMap& getActionsMap() const;
    void setActionsMap(ActionsMap i_actionsMap);

    [[nodiscard]] IControl& getForm();
    [[nodiscard]] const IControl& getForm() const;

    [[nodiscard]] double getGlobalTime() const;
    [[nodiscard]] const Sdk::FpsCounter& getFpsCounter() const;

    [[nodiscard]] const MouseState& getMouseState() const;

  protected:
    virtual void onGameStart();
    virtual void onGameEnd();

    [[nodiscard]] virtual bool continueLoop();

    virtual void update(double i_dt);
    virtual void updateGui(double i_dt);

    virtual void render();
    virtual void renderGui();

    virtual void onMouseMove(Sdk::Vector2I i_move);
    virtual void onMouseWheel(int i_distance);
    virtual void onMouseClick(MouseKey i_key);
    virtual void onMouseRelease(MouseKey i_key);

  private:
    static Game* s_this;

    bool d_stop = false;

    Sdk::Timer d_timer;
    double d_globalTime = 0;
    Sdk::FpsCounter d_fpsCounter;
    std::unique_ptr<Sdk::Window> d_window;
    std::unique_ptr<IInputDevice> d_inputDevice;
    std::unique_ptr<IRenderDevice> d_renderDevice;
    std::unique_ptr<IResourceController> d_resourceController;
    std::unique_ptr<IRenderer2d> d_renderer2d;

    ActionsMap d_actionsMap;

    MouseState d_mouseState;

    std::unique_ptr<IControl> d_form;

    void mainloop();
    void handleKeyboard(const KeyboardState& i_keyboardState);
    void handleMouse(MouseState i_mouseState);
    void resetRelativeMouseState();
  };

} // ns Dx

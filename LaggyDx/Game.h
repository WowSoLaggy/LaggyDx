#pragma once

#include "ActionsMap.h"
#include "Form.h"
#include "IInputDevice.h"
#include "IRenderDevice.h"
#include "IRenderer2d.h"
#include "IResourceController.h"
#include "LaggyDxFwd.h"
#include "ObjectCollection.h"

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
    void stop();

    [[nodiscard]] IInputDevice& getInputDevice();
    [[nodiscard]] const IInputDevice& getInputDevice() const;
    [[nodiscard]] IRenderDevice& getRenderDevice();
    [[nodiscard]] const IRenderDevice& getRenderDevice() const;
    [[nodiscard]] IResourceController& getResourceController();
    [[nodiscard]] const IResourceController& getResourceController() const;
    [[nodiscard]] IRenderer2d& getRenderer2d();
    [[nodiscard]] const IRenderer2d& getRenderer2d() const;

    [[nodiscard]] ObjectCollection& getObjectCollection();
    [[nodiscard]] const ObjectCollection& getObjectCollection() const;

    [[nodiscard]] ActionsMap& getActionsMap();
    [[nodiscard]] const ActionsMap& getActionsMap() const;
    void setActionsMap(ActionsMap i_actionsMap);

    [[nodiscard]] Control& getForm();
    [[nodiscard]] const Control& getForm() const;

  protected:
    virtual void onGameStart();
    virtual void onGameEnd();

    [[nodiscard]] virtual bool continueLoop();

    virtual void update(double i_dt);
    virtual void updateObjects(double i_dt);
    virtual void updateObject(IObject& i_obj, double i_dt);
    virtual void updateGui(double i_dt);

    virtual void render();
    virtual void renderObjects();
    virtual void renderObject(const IObject& i_obj);
    virtual void renderGui();

  private:
    static Game* s_this;

    bool d_stop = false;

    Sdk::Timer d_timer;
    std::unique_ptr<Sdk::Window> d_window;
    std::unique_ptr<IInputDevice> d_inputDevice;
    std::unique_ptr<IRenderDevice> d_renderDevice;
    std::unique_ptr<IResourceController> d_resourceController;
    std::unique_ptr<IRenderer2d> d_renderer2d;

    ObjectCollection d_objectCollection;
    ActionsMap d_actionsMap;

    Form d_form;

    void mainloop();
    void handleKeyboard(const Dx::KeyboardState& i_keyboardState);
  };

} // ns Dx

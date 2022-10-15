#pragma once

#include "IInputController.h"
#include "LaggyDxFwd.h"

#include <LaggySdk/EventHandler.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  class FirstPersonController : public IInputController, public Sdk::EventHandler
  {
  public:
    FirstPersonController(Game& i_game, ICamera& i_camera);
    ~FirstPersonController();

    virtual void processEvent(const Sdk::IEvent& i_event) override;

    float getYaw() const;
    float getPitch() const;

  private:
    Game& d_game;
    FirstPersonCamera& d_camera;

    bool d_wasCursorOriginallyShown = false;

    bool d_moveLeft = false;
    bool d_moveRight = false;
    bool d_moveForward = false;
    bool d_moveBackward = false;
    bool d_moveUp = false;
    bool d_moveDown = false;

    void onGameUpdate(double i_dt);

    void onMouseMoved(const Sdk::Vector2I& i_move);

    void onStartMoveLeft();
    void onStopMoveLeft();
    void onStartMoveRight();
    void onStopMoveRight();
    void onStartMoveForward();
    void onStopMoveForward();
    void onStartMoveBackward();
    void onStopMoveBackward();
    void onStartMoveUp();
    void onStopMoveUp();
    void onStartMoveDown();
    void onStopMoveDown();
  };

} // ns Dx

#pragma once

#include "IInputController.h"
#include "LaggyDxFwd.h"
#include "MouseMode.h"

#include <LaggySdk/EventHandler.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  class FreeCamera3Controller : public IInputController, public Sdk::EventHandler
  {
  public:
    FreeCamera3Controller(ICamera3& i_camera);
    ~FreeCamera3Controller();

    virtual void processEvent(const Sdk::IEvent& i_event) override;

    float getYaw() const;
    float getPitch() const;

    void setCameraSpeed(double i_speed);
    double getCameraSpeed() const;

  private:
    FirstPersonCamera& d_camera;

    double d_cameraSpeed = 5.0;

    MouseMode d_originalMouseMode = MouseMode::Absolute;

    bool d_moveLeft = false;
    bool d_moveRight = false;
    bool d_moveForward = false;
    bool d_moveBackward = false;
    bool d_moveUp = false;
    bool d_moveDown = false;

    void onAppUpdate(double i_dt);

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

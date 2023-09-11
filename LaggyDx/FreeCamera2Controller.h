#pragma once

#include "IInputController.h"
#include "LaggyDxFwd.h"

#include <LaggySdk/EventHandler.h>


namespace Dx
{
  class FreeCamera2Controller : public IInputController, public Sdk::EventHandler
  {
  public:
    FreeCamera2Controller(ICamera2& i_camera);
    ~FreeCamera2Controller();

    virtual void processEvent(const Sdk::IEvent& i_event) override;

    void setCameraSpeed(float i_cameraSpeed);

  private:
    ICamera2& d_camera;

    const float d_cameraDefaultSpeed = 1000;
    const float d_cameraFastSpeed = d_cameraDefaultSpeed * 5;
    float d_cameraSpeed = d_cameraDefaultSpeed;

    bool d_moveLeft = false;
    bool d_moveRight = false;
    bool d_moveUp = false;
    bool d_moveDown = false;

    void onAppUpdate(double i_dt);

    void onStartMoveLeft();
    void onStopMoveLeft();
    void onStartMoveRight();
    void onStopMoveRight();
    void onStartMoveUp();
    void onStopMoveUp();
    void onStartMoveDown();
    void onStopMoveDown();
    
    void onStartRun();
    void onStopRun();
  };

} // ns Dx

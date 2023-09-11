#include "stdafx.h"
#include "FreeCamera2Controller.h"

#include "App.h"
#include "AppEvents.h"
#include "ICamera2.h"
#include "InputEvents.h"


namespace Dx
{
  FreeCamera2Controller::FreeCamera2Controller(ICamera2& i_camera)
    : d_camera(i_camera)
  {
    connectTo(App::get());
  }

  FreeCamera2Controller::~FreeCamera2Controller()
  {
    disconnectFrom(App::get());
  }


  void FreeCamera2Controller::processEvent(const Sdk::IEvent& i_event)
  {
    if (const auto* event = dynamic_cast<const OnUpdate*>(&i_event))
    {
      onAppUpdate(event->dt());
    }

    else if (const auto* event = dynamic_cast<const OnKeyPressedEvent*>(&i_event))
    {
      if (event->key() == KeyboardKey::A)
        onStartMoveLeft();
      else if (event->key() == KeyboardKey::D)
        onStartMoveRight();
      else if (event->key() == KeyboardKey::W)
        onStartMoveUp();
      else if (event->key() == KeyboardKey::S)
        onStartMoveDown();
      else if (event->key() == KeyboardKey::LeftShift)
        onStartRun();
    }

    else if (const auto* event = dynamic_cast<const OnKeyReleasedEvent*>(&i_event))
    {
      if (event->key() == KeyboardKey::A)
        onStopMoveLeft();
      else if (event->key() == KeyboardKey::D)
        onStopMoveRight();
      else if (event->key() == KeyboardKey::W)
        onStopMoveUp();
      else if (event->key() == KeyboardKey::S)
        onStopMoveDown();
      else if (event->key() == KeyboardKey::LeftShift)
        onStopRun();
    }
  }


  void FreeCamera2Controller::onAppUpdate(double i_dt)
  {
    if (d_moveLeft && !d_moveRight)
    {
      auto offset = d_camera.getOffset();
      offset.x -= (int)((float)i_dt * d_cameraSpeed);
      d_camera.setOffset(offset);
    }
    else if (d_moveRight && !d_moveLeft)
    {
      auto offset = d_camera.getOffset();
      offset.x += (int)((float)i_dt * d_cameraSpeed);
      d_camera.setOffset(offset);
    }

    if (d_moveUp && !d_moveDown)
    {
      auto offset = d_camera.getOffset();
      offset.y -= (int)((float)i_dt * d_cameraSpeed);
      d_camera.setOffset(offset);
    }
    else if (d_moveDown && !d_moveUp)
    {
      auto offset = d_camera.getOffset();
      offset.y += (int)((float)i_dt * d_cameraSpeed);
      d_camera.setOffset(offset);
    }
  }


  void FreeCamera2Controller::setCameraSpeed(float i_cameraSpeed)
  {
    d_cameraSpeed = i_cameraSpeed;
  }


  void FreeCamera2Controller::onStartMoveLeft() { d_moveLeft = true; }
  void FreeCamera2Controller::onStopMoveLeft() { d_moveLeft = false; }
  void FreeCamera2Controller::onStartMoveRight() { d_moveRight = true; }
  void FreeCamera2Controller::onStopMoveRight() { d_moveRight = false; }
  void FreeCamera2Controller::onStartMoveUp() { d_moveUp = true; }
  void FreeCamera2Controller::onStopMoveUp() { d_moveUp = false; }
  void FreeCamera2Controller::onStartMoveDown() { d_moveDown = true; }
  void FreeCamera2Controller::onStopMoveDown() { d_moveDown = false; }


  void FreeCamera2Controller::onStartRun()
  {
    setCameraSpeed(d_cameraFastSpeed);
  }

  void FreeCamera2Controller::onStopRun()
  {
    setCameraSpeed(d_cameraDefaultSpeed);
  }

} // ns Dx

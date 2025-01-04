#include "stdafx.h"
#include "FreeCamera3Controller.h"

#include "App.h"
#include "AppEvents.h"
#include "FirstPersonCamera.h"
#include "InputEvents.h"

#include <LaggySdk/Math.h>


namespace Dx
{
  FreeCamera3Controller::FreeCamera3Controller(ICamera3& i_camera)
    : d_camera(dynamic_cast<FirstPersonCamera&>(i_camera))
  {
    auto& app = App::get();

    d_originalMouseMode = app.getInputDevice().getMouseMode();
    app.getInputDevice().setMouseRelativeMode();

    connectTo(app);
  }

  FreeCamera3Controller::~FreeCamera3Controller()
  {
    auto& app = App::get();

    disconnectFrom(app);
    
    app.getInputDevice().setMouseMode(d_originalMouseMode);
  }


  void FreeCamera3Controller::processEvent(const Sdk::IEvent& i_event)
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
        onStartMoveForward();
      else if (event->key() == KeyboardKey::S)
        onStartMoveBackward();
      else if (event->key() == KeyboardKey::V)
        onStartMoveUp();
      else if (event->key() == KeyboardKey::B)
        onStartMoveDown();
    }

    else if (const auto* event = dynamic_cast<const OnKeyReleasedEvent*>(&i_event))
    {
      if (event->key() == KeyboardKey::A)
        onStopMoveLeft();
      else if (event->key() == KeyboardKey::D)
        onStopMoveRight();
      else if (event->key() == KeyboardKey::W)
        onStopMoveForward();
      else if (event->key() == KeyboardKey::S)
        onStopMoveBackward();
      else if (event->key() == KeyboardKey::V)
        onStopMoveUp();
      else if (event->key() == KeyboardKey::B)
        onStopMoveDown();
    }

    else if (const auto* event = dynamic_cast<const OnMouseMovedEvent*>(&i_event))
    {
      onMouseMoved(event->getMove());
    }
  }


  void FreeCamera3Controller::onAppUpdate(double i_dt)
  {
    if (d_moveLeft && !d_moveRight)
    {
      auto pos = d_camera.getPosition();
      pos += d_camera.getLeft() * (float)i_dt * (float)getCameraSpeed();
      d_camera.setPosition(pos);
    }
    else if (d_moveRight && !d_moveLeft)
    {
      auto pos = d_camera.getPosition();
      pos += d_camera.getRight() * (float)i_dt * (float)getCameraSpeed();
      d_camera.setPosition(pos);
    }


    if (d_moveForward && !d_moveBackward)
    {
      auto pos = d_camera.getPosition();
      pos += d_camera.getForward() * (float)i_dt * (float)getCameraSpeed();
      d_camera.setPosition(pos);
    }
    else if (d_moveBackward && !d_moveForward)
    {
      auto pos = d_camera.getPosition();
      pos += d_camera.getBackward() * (float)i_dt * (float)getCameraSpeed();
      d_camera.setPosition(pos);
    }


    if (d_moveUp && !d_moveDown)
    {
      auto pos = d_camera.getPosition();
      pos += d_camera.getWorldUp() * (float)i_dt * (float)getCameraSpeed();
      d_camera.setPosition(pos);
    }
    else if (d_moveDown && !d_moveUp)
    {
      auto pos = d_camera.getPosition();
      pos += d_camera.getWorldDown() * (float)i_dt * (float)getCameraSpeed();
      d_camera.setPosition(pos);
    }
  }


  void FreeCamera3Controller::onMouseMoved(const Sdk::Vector2I& i_move)
  {
    constexpr float Sensitivity = 0.01f;
    constexpr float MaxPitch = Sdk::degToRad<float>(89.0f);

    const float newYaw = d_camera.getYaw() + (float)i_move.x * Sensitivity;
    const float newPitch = Sdk::clamp(
      d_camera.getPitch() - (float)i_move.y * Sensitivity,
      -MaxPitch, MaxPitch);

    d_camera.setYaw(newYaw);
    d_camera.setPitch(newPitch);
  }


  void FreeCamera3Controller::onStartMoveLeft()
  {
    d_moveLeft = true;
  }
  void FreeCamera3Controller::onStopMoveLeft()
  {
    d_moveLeft = false;
  }
  void FreeCamera3Controller::onStartMoveRight()
  {
    d_moveRight = true;
  }
  void FreeCamera3Controller::onStopMoveRight()
  {
    d_moveRight = false;
  }
  void FreeCamera3Controller::onStartMoveForward()
  {
    d_moveForward = true;
  }
  void FreeCamera3Controller::onStopMoveForward()
  {
    d_moveForward = false;
  }
  void FreeCamera3Controller::onStartMoveBackward()
  {
    d_moveBackward = true;
  }
  void FreeCamera3Controller::onStopMoveBackward()
  {
    d_moveBackward = false;
  }
  void FreeCamera3Controller::onStartMoveUp()
  {
    d_moveUp = true;
  }
  void FreeCamera3Controller::onStopMoveUp()
  {
    d_moveUp = false;
  }
  void FreeCamera3Controller::onStartMoveDown()
  {
    d_moveDown = true;
  }
  void FreeCamera3Controller::onStopMoveDown()
  {
    d_moveDown = false;
  }


  float FreeCamera3Controller::getYaw() const
  {
    return d_camera.getYaw();
  }

  float FreeCamera3Controller::getPitch() const
  {
    return d_camera.getPitch();
  }


  void FreeCamera3Controller::setCameraSpeed(const double i_speed)
  {
    d_cameraSpeed = i_speed;
  }

  double FreeCamera3Controller::getCameraSpeed() const
  {
    return d_cameraSpeed;
  }

} // ns Dx

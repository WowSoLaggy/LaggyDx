#include "stdafx.h"
#include "FreeCameraController.h"

#include "App.h"
#include "AppEvents.h"
#include "FirstPersonCamera.h"
#include "InputEvents.h"

#include <LaggySdk/Math.h>


namespace Dx
{
  FreeCameraController::FreeCameraController(App& i_app, ICamera3& i_camera)
    : d_app(i_app)
    , d_camera(dynamic_cast<FirstPersonCamera&>(i_camera))
  {
    d_originalMouseMode = d_app.getInputDevice().getMouseMode();
    d_app.getInputDevice().setMouseRelativeMode();

    connectTo(d_app);
  }

  FreeCameraController::~FreeCameraController()
  {
    disconnectFrom(d_app);
    
    d_app.getInputDevice().setMouseMode(d_originalMouseMode);
  }


  void FreeCameraController::processEvent(const Sdk::IEvent& i_event)
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
      onMouseMoved(event->move());
    }
  }


  void FreeCameraController::onAppUpdate(double i_dt)
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


  void FreeCameraController::onMouseMoved(const Sdk::Vector2I& i_move)
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


  void FreeCameraController::onStartMoveLeft()
  {
    d_moveLeft = true;
  }
  void FreeCameraController::onStopMoveLeft()
  {
    d_moveLeft = false;
  }
  void FreeCameraController::onStartMoveRight()
  {
    d_moveRight = true;
  }
  void FreeCameraController::onStopMoveRight()
  {
    d_moveRight = false;
  }
  void FreeCameraController::onStartMoveForward()
  {
    d_moveForward = true;
  }
  void FreeCameraController::onStopMoveForward()
  {
    d_moveForward = false;
  }
  void FreeCameraController::onStartMoveBackward()
  {
    d_moveBackward = true;
  }
  void FreeCameraController::onStopMoveBackward()
  {
    d_moveBackward = false;
  }
  void FreeCameraController::onStartMoveUp()
  {
    d_moveUp = true;
  }
  void FreeCameraController::onStopMoveUp()
  {
    d_moveUp = false;
  }
  void FreeCameraController::onStartMoveDown()
  {
    d_moveDown = true;
  }
  void FreeCameraController::onStopMoveDown()
  {
    d_moveDown = false;
  }


  float FreeCameraController::getYaw() const
  {
    return d_camera.getYaw();
  }

  float FreeCameraController::getPitch() const
  {
    return d_camera.getPitch();
  }


  void FreeCameraController::setCameraSpeed(const double i_speed)
  {
    d_cameraSpeed = i_speed;
  }

  double FreeCameraController::getCameraSpeed() const
  {
    return d_cameraSpeed;
  }

} // ns Dx

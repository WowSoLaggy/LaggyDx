#include "stdafx.h"
#include "FreeCameraController.h"

#include "FirstPersonCamera.h"
#include "Game.h"
#include "GameEvents.h"
#include "InputEvents.h"


namespace Dx
{
  FreeCameraController::FreeCameraController(Game& i_game, ICamera& i_camera)
    : d_game(i_game)
    , d_camera(dynamic_cast<FirstPersonCamera&>(i_camera))
  {
    d_originalMouseMode = d_game.getInputDevice().getMouseMode();
    d_game.getInputDevice().setMouseRelativeMode();

    connectTo(d_game);
  }

  FreeCameraController::~FreeCameraController()
  {
    disconnectFrom(d_game);
    
    d_game.getInputDevice().setMouseMode(d_originalMouseMode);
  }


  void FreeCameraController::processEvent(const Sdk::IEvent& i_event)
  {
    if (const auto* event = dynamic_cast<const OnGameUpdate*>(&i_event))
    {
      onGameUpdate(event->dt());
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


  void FreeCameraController::onGameUpdate(double i_dt)
  {
    constexpr float MoveSpeed = 5.0f;

    if (d_moveLeft && !d_moveRight)
    {
      auto pos = d_camera.getPosition();
      pos += d_camera.getLeft() * (float)i_dt * MoveSpeed;
      d_camera.setPosition(pos);
    }
    else if (d_moveRight && !d_moveLeft)
    {
      auto pos = d_camera.getPosition();
      pos += d_camera.getRight() * (float)i_dt * MoveSpeed;
      d_camera.setPosition(pos);
    }


    if (d_moveForward && !d_moveBackward)
    {
      auto pos = d_camera.getPosition();
      pos += d_camera.getForward() * (float)i_dt * MoveSpeed;
      d_camera.setPosition(pos);
    }
    else if (d_moveBackward && !d_moveForward)
    {
      auto pos = d_camera.getPosition();
      pos += d_camera.getBackward() * (float)i_dt * MoveSpeed;
      d_camera.setPosition(pos);
    }


    if (d_moveUp && !d_moveDown)
    {
      auto pos = d_camera.getPosition();
      pos += d_camera.getWorldUp() * (float)i_dt * MoveSpeed;
      d_camera.setPosition(pos);
    }
    else if (d_moveDown && !d_moveUp)
    {
      auto pos = d_camera.getPosition();
      pos += d_camera.getWorldDown() * (float)i_dt * MoveSpeed;
      d_camera.setPosition(pos);
    }
  }


  void FreeCameraController::onMouseMoved(const Sdk::Vector2I& i_move)
  {
    constexpr float Sensitivity = 0.01f;

    d_camera.setYaw(d_camera.getYaw() + (float)i_move.x * Sensitivity);
    d_camera.setPitch(d_camera.getPitch() - (float)i_move.y * Sensitivity);
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

} // ns Dx

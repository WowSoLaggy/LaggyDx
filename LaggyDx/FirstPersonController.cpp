#include "stdafx.h"
#include "FirstPersonController.h"

#include "FirstPersonCamera.h"
#include "Game.h"
#include "GameEvents.h"
#include "InputEvents.h"


namespace Dx
{
  FirstPersonController::FirstPersonController(Game& i_game, ICamera& i_camera)
    : d_game(i_game)
    , d_camera(dynamic_cast<FirstPersonCamera&>(i_camera))
  {
    connectTo(d_game);

    d_wasCursorOriginallyShown = d_game.getInputDevice().isCursorShown();

    d_game.getInputDevice().hideCursor();
    d_game.getInputDevice().setMouseRelativeMode();
  }

  FirstPersonController::~FirstPersonController()
  {
    d_game.getInputDevice().switchCursorVisibility(d_wasCursorOriginallyShown);

    disconnectFrom(d_game);
  }


  void FirstPersonController::processEvent(const Sdk::IEvent& i_event)
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


  void FirstPersonController::onGameUpdate(double i_dt)
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


  void FirstPersonController::onMouseMoved(const Sdk::Vector2I& i_move)
  {
    constexpr float Sensitivity = 0.01f;

    d_camera.setYaw(d_camera.getYaw() + (float)i_move.x * Sensitivity);
    d_camera.setPitch(d_camera.getPitch() - (float)i_move.y * Sensitivity);
  }


  void FirstPersonController::onStartMoveLeft()
  {
    d_moveLeft = true;
  }
  void FirstPersonController::onStopMoveLeft()
  {
    d_moveLeft = false;
  }
  void FirstPersonController::onStartMoveRight()
  {
    d_moveRight = true;
  }
  void FirstPersonController::onStopMoveRight()
  {
    d_moveRight = false;
  }
  void FirstPersonController::onStartMoveForward()
  {
    d_moveForward = true;
  }
  void FirstPersonController::onStopMoveForward()
  {
    d_moveForward = false;
  }
  void FirstPersonController::onStartMoveBackward()
  {
    d_moveBackward = true;
  }
  void FirstPersonController::onStopMoveBackward()
  {
    d_moveBackward = false;
  }
  void FirstPersonController::onStartMoveUp()
  {
    d_moveUp = true;
  }
  void FirstPersonController::onStopMoveUp()
  {
    d_moveUp = false;
  }
  void FirstPersonController::onStartMoveDown()
  {
    d_moveDown = true;
  }
  void FirstPersonController::onStopMoveDown()
  {
    d_moveDown = false;
  }


  float FirstPersonController::getYaw() const
  {
    return d_camera.getYaw();
  }

  float FirstPersonController::getPitch() const
  {
    return d_camera.getPitch();
  }

} // ns Dx

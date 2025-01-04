#pragma once

#include "KeyboardKeys.h"
#include "MouseState.h"

#include <LaggySdk/IEvent.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  class KeyEvent : public Sdk::IEvent
  {
  public:
    KeyEvent(KeyboardKey i_key)
      : d_key(i_key)
    {
    }

    KeyboardKey key() const { return d_key; }

  private:
    const KeyboardKey d_key;
  };

  class OnKeyPressedEvent : public KeyEvent
  {
  public:
    using KeyEvent::KeyEvent;
  };

  class OnKeyReleasedEvent : public KeyEvent
  {
  public:
    using KeyEvent::KeyEvent;
  };

  class OnKeyCurrentEvent : public KeyEvent
  {
  public:
    using KeyEvent::KeyEvent;
  };


  class OnMouseMovedEvent : public Sdk::IEvent
  {
  public:
    OnMouseMovedEvent(Sdk::Vector2I i_move)
      : d_move(std::move(i_move))
    {
    }

    const Sdk::Vector2I& getMove() const { return d_move; }

  private:
    const Sdk::Vector2I d_move;
  };


  class MouseModeChangedEvent : public Sdk::IEvent
  {
  public:
    MouseModeChangedEvent(MouseMode i_mode)
      : d_mode(i_mode)
    {
    }

    MouseMode getMode() const { return d_mode; }

  private:
    const MouseMode d_mode = MouseMode::Absolute;
  };

} // ns Dx

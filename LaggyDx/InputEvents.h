#pragma once

#include "KeyboardKeys.h"

#include <LaggySdk/IEvent.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  class KeyEvent : public Sdk::IEvent
  {
  public:
    KeyEvent(Dx::KeyboardKey i_key)
      : d_key(i_key)
    {
    }

    Dx::KeyboardKey key() const { return d_key; }

  private:
    const Dx::KeyboardKey d_key;
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

    const Sdk::Vector2I& move() const { return d_move; }

  private:
    const Sdk::Vector2I d_move;
  };

} // ns Dx

#include "stdafx.h"
#include "KeyboardState.h"


namespace Dx
{
  KeyboardState::KeyboardState()
  {
    reset();
  }


  void KeyboardState::update(const KeyboardKeys& i_state)
  {
    auto currPtr = reinterpret_cast<const uint32_t*>(&i_state);
    auto prevPtr = reinterpret_cast<const uint32_t*>(&currentState);
    auto releasedPtr = reinterpret_cast<uint32_t*>(&released);
    auto pressedPtr = reinterpret_cast<uint32_t*>(&pressed);
    for (size_t j = 0; j < (256 / 32); ++j)
    {
      *pressedPtr = *currPtr & ~(*prevPtr);
      *releasedPtr = ~(*currPtr) & *prevPtr;

      ++currPtr;
      ++prevPtr;
      ++releasedPtr;
      ++pressedPtr;
    }

    currentState = i_state;
  }

  void KeyboardState::reset()
  {
    memset(this, 0, sizeof(KeyboardState));
  }

} // ns Dx

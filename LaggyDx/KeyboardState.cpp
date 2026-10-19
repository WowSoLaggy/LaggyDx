#include "stdafx.h"
#include "KeyboardState.h"


namespace Dx
{
  KeyboardState::KeyboardState()
  {
    reset();
  }


  void KeyboardState::latch(const KeyboardKeysState& i_state)
  {
    auto currPtr = reinterpret_cast<const uint32_t*>(&i_state);
    auto latchedPtr = reinterpret_cast<uint32_t*>(&d_latchedKeys);
    for (size_t j = 0; j < (256 / 32); ++j)
    {
      *latchedPtr |= *currPtr;

      ++currPtr;
      ++latchedPtr;
    }
  }

  void KeyboardState::update(const KeyboardKeysState& i_state)
  {
    auto currPtr = reinterpret_cast<const uint32_t*>(&i_state);
    auto prevPtr = reinterpret_cast<const uint32_t*>(&d_currentState);
    auto latchedPtr = reinterpret_cast<const uint32_t*>(&d_latchedKeys);
    auto releasedPtr = reinterpret_cast<uint32_t*>(&d_releasedKeys);
    auto pressedPtr = reinterpret_cast<uint32_t*>(&d_pressedKeys);
    for (size_t j = 0; j < (256 / 32); ++j)
    {
      const uint32_t wasDown = *currPtr | *latchedPtr;
      *pressedPtr = wasDown & ~(*prevPtr);
      *releasedPtr = ~(*currPtr) & (*prevPtr | *latchedPtr);

      ++currPtr;
      ++prevPtr;
      ++latchedPtr;
      ++releasedPtr;
      ++pressedPtr;
    }

    d_currentState = i_state;
    memset(&d_latchedKeys, 0, sizeof(d_latchedKeys));
  }

  void KeyboardState::reset()
  {
    memset(this, 0, sizeof(KeyboardState));
  }

} // ns Dx

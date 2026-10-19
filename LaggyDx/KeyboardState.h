#pragma once

#include "KeyboardKeysState.h"


namespace Dx
{
  class KeyboardState
  {
  public:
    KeyboardState();

    const KeyboardKeysState& getPressedKeys() const { return d_pressedKeys; }
    const KeyboardKeysState& getCurrentKeys() const { return d_currentState; }
    const KeyboardKeysState& getReleasedKeys() const { return d_releasedKeys; }

    // Remembers keys down between updates, so taps shorter than a frame still count as pressed and released
    void latch(const KeyboardKeysState& i_state);
    void update(const KeyboardKeysState& i_state);
    void reset();

  private:
    KeyboardKeysState d_pressedKeys;
    KeyboardKeysState d_releasedKeys;
    KeyboardKeysState d_currentState;
    KeyboardKeysState d_latchedKeys;
  };

} // ns Dx

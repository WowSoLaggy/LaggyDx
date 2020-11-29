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

    void update(const KeyboardKeysState& i_state);
    void reset();

  private:
    KeyboardKeysState d_pressedKeys;
    KeyboardKeysState d_releasedKeys;
    KeyboardKeysState d_currentState;
  };

} // ns Dx

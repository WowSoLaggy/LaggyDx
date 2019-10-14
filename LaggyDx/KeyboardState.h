#pragma once

#include "KeyboardKeys.h"


namespace Dx
{
  class KeyboardState
  {
  public:
    KeyboardState();

    const KeyboardKeys& getPressedKeys() const { return d_pressedKeys; }
    const KeyboardKeys& getCurrentKeys() const { return d_currentState; }
    const KeyboardKeys& getReleasedKeys() const { return d_releasedKeys; }

    void update(const KeyboardKeys& i_state);
    void reset();

  private:
    KeyboardKeys d_pressedKeys;
    KeyboardKeys d_releasedKeys;
    KeyboardKeys d_currentState;
  };

} // ns Dx

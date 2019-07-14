#pragma once

#include "MouseKeys.h"

namespace Dx
{
  enum class ButtonState
  {
    Up        = 0,  // Button is up
    Held      = 1,  // Button is held down
    Released  = 2,  // Button was just released
    Pressed   = 3,  // Buton was just pressed
  };

  struct MousePosition
  {
    int x;
    int y;
    int z;
  };

  class MouseState
  {
  public:
    MouseState();

    const ButtonState& getLeftButtonState() const { return d_leftButtonState; }
    const ButtonState& getMiddleButtonState() const { return d_middleButtonState; }
    const ButtonState& getRightButtonState() const { return d_rightButtonState; }
    const ButtonState& getXButton1State() const { return d_xButton1State; }
    const ButtonState& getXButton2State() const { return d_xButton2State; }

    const MousePosition& getMousePosition() const { return d_mousePosition; }

    void update(const MouseKeys& i_state);
    void reset();

  private:
    ButtonState d_leftButtonState;
    ButtonState d_middleButtonState;
    ButtonState d_rightButtonState;
    ButtonState d_xButton1State;
    ButtonState d_xButton2State;

    MousePosition d_mousePosition;

    MouseKeys d_currentState;
  };

} // ns Dx

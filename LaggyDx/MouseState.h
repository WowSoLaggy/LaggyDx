#pragma once

#include "MouseKeys.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  enum class MouseMode
  {
    Absolute = 0,
    Relative = 1,
  };

  enum class ButtonState
  {
    Up        = 0,  // Button is up
    Held      = 1,  // Button is held down
    Released  = 2,  // Button was just released
    Pressed   = 3,  // Buton was just pressed
  };

  class MouseState
  {
  public:
    MouseState();

    ButtonState getLeftButtonState() const { return d_leftButtonState; }
    ButtonState getMiddleButtonState() const { return d_middleButtonState; }
    ButtonState getRightButtonState() const { return d_rightButtonState; }
    ButtonState getXButton1State() const { return d_xButton1State; }
    ButtonState getXButton2State() const { return d_xButton2State; }

    const Sdk::Vector2I& getPosition() const { return d_mousePosition; }
    int getWheelPosition() const { return d_wheelPosition; }
    int getWheelPositionChange() const { return d_wheelPositionChange; }

    void update(const MouseKeys& i_state);
    void reset();

    void setMode(MouseMode i_mode) { d_mode = i_mode; }
    const MouseMode getMode() const { return d_mode; }

  private:
    ButtonState d_leftButtonState;
    ButtonState d_middleButtonState;
    ButtonState d_rightButtonState;
    ButtonState d_xButton1State;
    ButtonState d_xButton2State;

    Sdk::Vector2I d_mousePosition;
    int d_wheelPosition = 0;
    int d_wheelPositionChange = 0;

    MouseMode d_mode = MouseMode::Absolute;

    MouseKeys d_currentState;
  };

} // ns Dx

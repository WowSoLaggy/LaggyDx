#pragma once

#include "MouseMode.h"
#include "MouseKeys.h"
#include "MouseKeysState.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  enum class MouseButtonState
  {
    Up        = 0,  // Button is up (not pressed)
    Held      = 1,  // Button is held down
    Released  = 2,  // Button was just released
    Pressed   = 3,  // Buton was just pressed
  };

  class MouseState
  {
  public:
    MouseState();

    MouseButtonState getLeftButtonState() const { return d_leftButtonState; }
    MouseButtonState getMiddleButtonState() const { return d_middleButtonState; }
    MouseButtonState getRightButtonState() const { return d_rightButtonState; }
    MouseButtonState getXButton1State() const { return d_xButton1State; }
    MouseButtonState getXButton2State() const { return d_xButton2State; }

    MouseButtonState getButtonState(MouseKey i_key) const;

    const Sdk::Vector2I& getPosition() const { return d_mousePosition; }
    void resetPosition() { d_mousePosition = {}; }

    int getWheelPosition() const { return d_wheelPosition; }
    int getWheelPositionChange() const { return d_wheelPositionChange; }

    void update(const MouseKeysState& i_state);
    void reset();

    void setMode(MouseMode i_mode) { d_mode = i_mode; }
    const MouseMode getMode() const { return d_mode; }

  private:
    MouseButtonState d_leftButtonState;
    MouseButtonState d_middleButtonState;
    MouseButtonState d_rightButtonState;
    MouseButtonState d_xButton1State;
    MouseButtonState d_xButton2State;

    Sdk::Vector2I d_mousePosition;
    int d_wheelPosition = 0;
    int d_wheelPositionChange = 0;

    MouseMode d_mode = MouseMode::Absolute;

    MouseKeysState d_currentState;
  };

} // ns Dx

#include "stdafx.h"
#include "MouseState.h"

#include <LaggySdk/Contracts.h>


namespace Dx
{
  namespace
  {
    MouseButtonState getNewButtonState(const bool i_wasDown, const bool i_isDown, const bool i_wentDown)
    {
      if (i_isDown)
        return i_wasDown ? MouseButtonState::Held : MouseButtonState::Pressed;
      if (i_wasDown)
        return MouseButtonState::Released;
      return i_wentDown ? MouseButtonState::Clicked : MouseButtonState::Up;
    }

  } // anonym NS


  MouseState::MouseState()
  {
    reset();
  }


  void MouseState::latch(const MouseKeysState& i_state)
  {
    d_latchedState.leftButton |= i_state.leftButton;
    d_latchedState.middleButton |= i_state.middleButton;
    d_latchedState.rightButton |= i_state.rightButton;
    d_latchedState.xButton1 |= i_state.xButton1;
    d_latchedState.xButton2 |= i_state.xButton2;
  }

  void MouseState::update(const MouseKeysState& i_state)
  {
    d_leftButtonState = getNewButtonState(d_currentState.leftButton, i_state.leftButton, d_latchedState.leftButton);
    d_middleButtonState = getNewButtonState(d_currentState.middleButton, i_state.middleButton, d_latchedState.middleButton);
    d_rightButtonState = getNewButtonState(d_currentState.rightButton, i_state.rightButton, d_latchedState.rightButton);
    d_xButton1State = getNewButtonState(d_currentState.xButton1, i_state.xButton1, d_latchedState.xButton1);
    d_xButton2State = getNewButtonState(d_currentState.xButton2, i_state.xButton2, d_latchedState.xButton2);

    d_currentState = i_state;
    d_latchedState = {};

    d_mousePosition = { i_state.x, i_state.y };

    // 120 is a WHEEL_DELTA const
    d_wheelPositionChange = (i_state.scrollWheelValue - d_wheelPosition) / 120;
    d_wheelPosition = i_state.scrollWheelValue;
  }

  void MouseState::reset()
  {
    memset(this, 0, sizeof(MouseState));
  }


  MouseButtonState MouseState::getButtonState(MouseKey i_key) const
  {
    switch (i_key)
    {
    case MouseKey::Left:
      return getLeftButtonState();
    case MouseKey::Middle:
      return getMiddleButtonState();
    case MouseKey::Right:
      return getRightButtonState();
    case MouseKey::X1:
      return getXButton1State();
    case MouseKey::X2:
      return getXButton2State();
    }

    CONTRACT_EXPECT(false);
  }

} // ns Dx

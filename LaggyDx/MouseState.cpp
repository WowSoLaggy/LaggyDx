#include "stdafx.h"
#include "MouseState.h"

#include <LaggySdk/Contracts.h>


namespace Dx
{
  MouseState::MouseState()
  {
    reset();
  }


  void MouseState::update(const MouseKeysState& i_state)
  {
#define UPDATE_BUTTON_STATE(field) static_cast<MouseButtonState>((!!i_state.field) | \
    ((!!i_state.field ^ !!d_currentState.field) << 1));

    d_leftButtonState = UPDATE_BUTTON_STATE(leftButton);

    CONTRACT_ENSURE((!i_state.leftButton && !d_currentState.leftButton) == (d_leftButtonState == MouseButtonState::Up));
    CONTRACT_ENSURE((i_state.leftButton && d_currentState.leftButton) == (d_leftButtonState == MouseButtonState::Held));
    CONTRACT_ENSURE((!i_state.leftButton && d_currentState.leftButton) == (d_leftButtonState == MouseButtonState::Released));
    CONTRACT_ENSURE((i_state.leftButton && !d_currentState.leftButton) == (d_leftButtonState == MouseButtonState::Pressed));

    d_middleButtonState = UPDATE_BUTTON_STATE(middleButton);
    d_rightButtonState = UPDATE_BUTTON_STATE(rightButton);
    d_xButton1State = UPDATE_BUTTON_STATE(xButton1);
    d_xButton2State = UPDATE_BUTTON_STATE(xButton2);

    d_currentState = i_state;

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

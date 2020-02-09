#include "stdafx.h"
#include "MouseState.h"

#include <Sdk/Contracts.h>


namespace Dx
{
  MouseState::MouseState()
  {
    reset();
  }


  void MouseState::update(const MouseKeys& i_state)
  {
#define UPDATE_BUTTON_STATE(field) static_cast<ButtonState>((!!i_state.field) | \
    ((!!i_state.field ^ !!d_currentState.field) << 1));

    d_leftButtonState = UPDATE_BUTTON_STATE(leftButton);

    CONTRACT_ENSURE((!i_state.leftButton && !d_currentState.leftButton) == (d_leftButtonState == ButtonState::Up));
    CONTRACT_ENSURE((i_state.leftButton && d_currentState.leftButton) == (d_leftButtonState == ButtonState::Held));
    CONTRACT_ENSURE((!i_state.leftButton && d_currentState.leftButton) == (d_leftButtonState == ButtonState::Released));
    CONTRACT_ENSURE((i_state.leftButton && !d_currentState.leftButton) == (d_leftButtonState == ButtonState::Pressed));

    d_middleButtonState = UPDATE_BUTTON_STATE(middleButton);
    d_rightButtonState = UPDATE_BUTTON_STATE(rightButton);
    d_xButton1State = UPDATE_BUTTON_STATE(xButton1);
    d_xButton2State = UPDATE_BUTTON_STATE(xButton2);

    d_currentState = i_state;

    d_mousePosition = { i_state.x, i_state.y };

    d_wheelPositionChange = i_state.scrollWheelValue - d_wheelPosition;
    d_wheelPosition = i_state.scrollWheelValue;
  }

  void MouseState::reset()
  {
    memset(this, 0, sizeof(MouseState));
  }

} // ns Dx

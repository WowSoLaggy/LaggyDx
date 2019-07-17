#include "stdafx.h"
#include "InputDevice.h"


namespace Dx
{
  InputDevice::InputDevice(HWND i_hWnd)
  {
    d_mouse = std::make_unique<Mouse>();
    d_mouse->SetWindow(i_hWnd);
    d_mouse->SetVisible(false);

    d_keyboard = std::make_unique<Keyboard>();
    d_keyboardState.reset();
  }

  void InputDevice::dispose()
  {
    d_mouse->SetVisible(true);
    d_keyboard.reset();
    d_mouse.reset();
  }


  const KeyboardState& InputDevice::checkKeyboard()
  {
    const auto state = d_keyboard->GetState();

    KeyboardKeys keys;
    memcpy(&keys, &state, sizeof(keys));

    d_keyboardState.update(keys);

    return d_keyboardState;
  }

  const MouseState& InputDevice::checkMouse()
  {
    const auto state = d_mouse->GetState();

    MouseKeys keys;
    memcpy(&keys, &state, sizeof(keys));

    d_mouseState.update(keys);

    return d_mouseState;
  }

} // ns Dx

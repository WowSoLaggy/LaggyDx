#include "stdafx.h"
#include "InputDevice.h"


namespace Dx
{
  InputDevice::InputDevice(HWND i_hWnd)
  {
    d_mouse = std::make_unique<Mouse>();
    d_mouse->SetWindow(i_hWnd);
    d_mouse->SetVisible(false);
    d_mouse->SetMode(Mouse::Mode::MODE_ABSOLUTE);

    d_keyboard = std::make_unique<Keyboard>();
    d_keyboardState.reset();
  }

  InputDevice::~InputDevice()
  {
    d_mouse->SetVisible(true);
  }


  const KeyboardState& InputDevice::checkKeyboard()
  {
    const auto state = d_keyboard->GetState();

    KeyboardKeysState keys;
    memcpy(&keys, &state, sizeof(keys));

    d_keyboardState.update(keys);

    return d_keyboardState;
  }

  const MouseState& InputDevice::checkMouse()
  {
    const auto state = d_mouse->GetState();

    MouseKeysState keys;
    memcpy(&keys, &state, sizeof(keys));

    d_mouseState.update(keys);
    d_mouseState.setMode(static_cast<Dx::MouseMode>(state.positionMode));

    return d_mouseState;
  }


  void InputDevice::showCursor()
  {
    d_mouse->SetVisible(true);
  }

  void InputDevice::hideCursor()
  {
    d_mouse->SetVisible(false);
  }

} // ns Dx

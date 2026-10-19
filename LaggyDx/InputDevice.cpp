#include "stdafx.h"
#include "InputDevice.h"

#include "InputEvents.h"


namespace Dx
{
  namespace
  {
    KeyboardKeysState toKeyboardKeys(const Keyboard::State& i_state)
    {
      KeyboardKeysState keys;
      memcpy(&keys, &i_state, sizeof(keys));
      return keys;
    }

    MouseKeysState toMouseKeys(const Mouse::State& i_state)
    {
      MouseKeysState keys;
      memcpy(&keys, &i_state, sizeof(keys));
      return keys;
    }

  } // anonym NS


  InputDevice::InputDevice(HWND i_hWnd)
  {
    d_mouse = std::make_unique<Mouse>();
    d_mouse->SetWindow(i_hWnd);
    
    setMouseAbsoluteMode();

    d_keyboard = std::make_unique<Keyboard>();
    d_keyboardState.reset();
  }

  InputDevice::~InputDevice()
  {
    showCursor();
  }


  const KeyboardState& InputDevice::checkKeyboard()
  {
    d_keyboardState.update(toKeyboardKeys(d_keyboard->GetState()));
    return d_keyboardState;
  }

  const MouseState& InputDevice::checkMouse()
  {
    const auto state = d_mouse->GetState();
    d_mouseState.update(toMouseKeys(state));
    d_mouseState.setMode(static_cast<MouseMode>(state.positionMode));
    return d_mouseState;
  }


  void InputDevice::latchKeyboard()
  {
    d_keyboardState.latch(toKeyboardKeys(d_keyboard->GetState()));
  }

  void InputDevice::latchMouse()
  {
    d_mouseState.latch(toMouseKeys(d_mouse->GetState()));
  }


  const Sdk::Vector2I& InputDevice::getMousePosition() const
  {
    return d_mouseState.getPosition();
  }


  void InputDevice::showCursor()
  {
    d_mouse->SetVisible(true);
    d_cursorShown = true;
  }

  void InputDevice::hideCursor()
  {
    d_mouse->SetVisible(false);
    d_cursorShown = false;
  }

  void InputDevice::switchCursorVisibility(const bool i_show)
  {
    if (i_show)
      showCursor();
    else
      hideCursor();
  }

  bool InputDevice::isCursorShown()
  {
    return d_cursorShown;
  }


  void InputDevice::setMouseAbsoluteMode()
  {
    setMouseMode(MouseMode::Absolute);
  }

  void InputDevice::setMouseRelativeMode()
  {
    setMouseMode(MouseMode::Relative);
  }

  void InputDevice::setMouseMode(const MouseMode i_mode)
  {
    if (i_mode == getMouseMode())
      return;

    d_mouseMode = i_mode;

    notify(MouseModeChangedEvent(getMouseMode()));

    try
    {
      d_mouse->SetMode(static_cast<DirectX::Mouse::Mode>(d_mouseMode));
    }
    catch (std::system_error&)
    {
      // May throw on killing app (invalid window handler to send event to) - that's fine, ignore
    }
  }

  MouseMode InputDevice::getMouseMode() const
  {
    return d_mouseMode;
  }

} // ns Dx

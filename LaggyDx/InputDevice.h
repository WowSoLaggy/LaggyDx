#pragma once

#include "IInputDevice.h"
#include "KeyboardState.h"
#include "MouseState.h"


namespace Dx
{
  class InputDevice : public IInputDevice
  {
  public:
    InputDevice(HWND i_hWnd);
    virtual ~InputDevice() override;

    virtual void processMessage(const Sdk::Message& i_inputMessage) override;

    virtual const MouseState& checkMouse() override;
    virtual const KeyboardState& checkKeyboard() override;

    virtual void showCursor() override;
    virtual void hideCursor() override;

  private:

    std::unique_ptr<Mouse> d_mouse;
    MouseState d_mouseState;

    std::unique_ptr<Keyboard> d_keyboard;
    KeyboardState d_keyboardState;

  };

} // ns Dx

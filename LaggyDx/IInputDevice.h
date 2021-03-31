#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/SdkFwd.h>


namespace Dx
{
  class IInputDevice
  {
  public:
    static std::unique_ptr<IInputDevice> create(HWND i_hWnd);

  public:
    virtual ~IInputDevice() = default;

    virtual void processMessage(const Sdk::Message& i_inputMessage) = 0;

    virtual const MouseState& checkMouse() = 0;
    virtual const KeyboardState& checkKeyboard() = 0;

    virtual void showCursor() = 0;
    virtual void hideCursor() = 0;
  };

} // ns Dx

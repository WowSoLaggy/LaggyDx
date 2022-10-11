#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/SdkFwd.h>
#include <LaggySdk/Vector.h>


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
    virtual void switchCursorVisibility(bool i_show) = 0;
    virtual bool isCursorShown() = 0;

    virtual void setMouseAbsoluteMode() = 0;
    virtual void setMouseRelativeMode() = 0;

    virtual const Sdk::Vector2I& getMousePosition() const = 0;
  };

} // ns Dx

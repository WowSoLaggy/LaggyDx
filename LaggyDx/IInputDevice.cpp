#include "stdafx.h"
#include "IInputDevice.h"

#include "InputDevice.h"


namespace Dx
{
  std::unique_ptr<IInputDevice> IInputDevice::create(HWND i_hWnd)
  {
    return std::make_unique<InputDevice>(i_hWnd);
  }

} // ns Dx

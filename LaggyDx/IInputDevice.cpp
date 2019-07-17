#include "stdafx.h"
#include "IInputDevice.h"

#include "InputDevice.h"


namespace Dx
{
  std::unique_ptr<IInputDevice> IInputDevice::create()
  {
    return std::make_unique<InputDevice>();
  }

} // ns Dx

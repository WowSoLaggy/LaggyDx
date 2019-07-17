#include "stdafx.h"
#include "IRenderDevice.h"

#include "RenderDevice.h"


namespace Dx
{
  std::unique_ptr<IRenderDevice> IRenderDevice::create()
  {
    return std::make_unique<RenderDevice>();
  }

} // ns Dx

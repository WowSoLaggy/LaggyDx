#include "stdafx.h"
#include "IRenderDevice.h"

#include "RenderDevice.h"


namespace Dx
{
  std::unique_ptr<IRenderDevice> IRenderDevice::create(HWND i_hWnd, int i_resolutionX, int i_resolutionY)
  {
    return std::make_unique<RenderDevice>(i_hWnd, i_resolutionX, i_resolutionY);
  }

} // ns Dx

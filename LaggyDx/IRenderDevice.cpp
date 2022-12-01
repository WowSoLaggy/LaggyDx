#include "stdafx.h"
#include "IRenderDevice.h"

#include "RenderDevice.h"


namespace Dx
{
  std::unique_ptr<IRenderDevice> IRenderDevice::create(
    HWND i_hWnd, const Sdk::Vector2I i_resolution, const bool i_debugMode)
  {
    return std::make_unique<RenderDevice>(i_hWnd, std::move(i_resolution), i_debugMode);
  }

} // ns Dx

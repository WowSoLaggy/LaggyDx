#include "stdafx.h"
#include "ICamera.h"

#include "Camera.h"


namespace Dx
{
  std::unique_ptr<ICamera> ICamera::createCamera(Sdk::Vector2I i_viewportResolution)
  {
    return std::make_unique<Camera>(std::move(i_viewportResolution));
  }

} // ns Dx

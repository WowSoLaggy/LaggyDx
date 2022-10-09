#include "stdafx.h"
#include "ICamera.h"

#include "FirstPersonCamera.h"
#include "ThirdPersonCamera.h"


namespace Dx
{
  std::unique_ptr<ICamera> ICamera::createFirstPersonCamera(Sdk::Vector2I i_viewportResolution)
  {
    return std::make_unique<FirstPersonCamera>(std::move(i_viewportResolution));
  }

  std::unique_ptr<ICamera> ICamera::createThirdPersonCamera(Sdk::Vector2I i_viewportResolution)
  {
    return std::make_unique<ThirdPersonCamera>(std::move(i_viewportResolution));
  }

} // ns Dx

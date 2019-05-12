#include "stdafx.h"
#include "ICamera.h"

#include "Camera.h"


namespace Dx
{
  std::shared_ptr<ICamera> ICamera::createCamera(int i_screenWidth, int i_screenHeight)
  {
    return std::make_shared<Camera>(i_screenWidth, i_screenHeight);
  }

} // ns Dx

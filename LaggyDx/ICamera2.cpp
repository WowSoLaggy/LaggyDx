#include "stdafx.h"
#include "ICamera2.h"

#include "Camera2.h"


namespace Dx
{
  std::unique_ptr<ICamera2> ICamera2::create()
  {
    return std::make_unique<Camera2>();
  }

} // ns Dx

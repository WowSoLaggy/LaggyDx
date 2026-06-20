#include "stdafx.h"
#include "IRoadShader.h"

#include "RoadShader.h"


namespace Dx
{
  std::shared_ptr<IRoadShader> IRoadShader::create(const ICamera3& i_camera)
  {
    return std::make_shared<RoadShader>(i_camera);
  }

} // ns Dx

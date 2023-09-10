#include "stdafx.h"
#include "IOceanShader.h"

#include "OceanShader.h"


namespace Dx
{
  std::unique_ptr<IOceanShader> IOceanShader::create(const ICamera3& i_camera)
  {
    return std::make_unique<OceanShader>(i_camera);
  }

} // ns Dx

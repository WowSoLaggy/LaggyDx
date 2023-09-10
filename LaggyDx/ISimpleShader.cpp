#include "stdafx.h"
#include "ISimpleShader.h"

#include "SimpleShader.h"


namespace Dx
{
  std::unique_ptr<ISimpleShader> ISimpleShader::create(const ICamera3& i_camera)
  {
    return std::make_unique<SimpleShader>(i_camera);
  }

} // ns Dx

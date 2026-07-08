#include "stdafx.h"
#include "IInstancedShader.h"

#include "InstancedShader.h"


namespace Dx
{
  std::shared_ptr<IInstancedShader> IInstancedShader::create(const ICamera3& i_camera)
  {
    return std::make_shared<InstancedShader>(i_camera);
  }

} // ns Dx

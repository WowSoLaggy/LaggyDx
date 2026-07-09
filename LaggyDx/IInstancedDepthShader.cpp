#include "stdafx.h"
#include "IInstancedDepthShader.h"

#include "InstancedDepthShader.h"


namespace Dx
{
  std::shared_ptr<IInstancedDepthShader> IInstancedDepthShader::create(const ShadowCamera& i_shadowCamera)
  {
    return std::make_shared<InstancedDepthShader>(i_shadowCamera);
  }

} // ns Dx

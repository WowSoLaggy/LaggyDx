#include "stdafx.h"
#include "IDepthShader.h"

#include "DepthShader.h"


namespace Dx
{
  std::shared_ptr<IDepthShader> IDepthShader::create(const ShadowCamera& i_shadowCamera)
  {
    return std::make_shared<DepthShader>(i_shadowCamera);
  }

} // ns Dx

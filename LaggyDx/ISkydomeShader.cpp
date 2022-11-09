#include "stdafx.h"
#include "ISkydomeShader.h"

#include "SkydomeShader.h"


namespace Dx
{
  std::unique_ptr<ISkydomeShader> ISkydomeShader::create(
    IRenderDevice& i_renderDevice,
    const ICamera& i_camera,
    const IResourceController& i_resourceController)
  {
    return std::make_unique<SkydomeShader>(i_renderDevice, i_camera, i_resourceController);
  }

} // ns Dx

#include "stdafx.h"
#include "IOceanShader.h"

#include "OceanShader.h"


namespace Dx
{
  std::unique_ptr<IOceanShader> IOceanShader::create(
    IRenderDevice& i_renderDevice,
    const ICamera& i_camera,
    const IResourceController& i_resourceController)
  {
    return std::make_unique<OceanShader>(i_renderDevice, i_camera, i_resourceController);
  }

} // ns Dx

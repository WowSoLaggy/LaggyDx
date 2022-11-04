#include "stdafx.h"
#include "ISkyboxShader.h"

#include "SkyboxShader.h"


namespace Dx
{
  std::unique_ptr<ISkyboxShader> ISkyboxShader::create(
    IRenderDevice& i_renderDevice,
    const ICamera& i_camera,
    const IResourceController& i_resourceController)
  {
    return std::make_unique<SkyboxShader>(i_renderDevice, i_camera, i_resourceController);
  }

} // ns Dx

#include "stdafx.h"
#include "ISimpleShader.h"

#include "SimpleShader.h"


namespace Dx
{
  std::unique_ptr<ISimpleShader> ISimpleShader::create(
    IRenderDevice& i_renderDevice,
    const ICamera& i_camera,
    const IResourceController& i_resourceController)
  {
    return std::make_unique<SimpleShader>(i_renderDevice, i_camera, i_resourceController);
  }

} // ns Dx

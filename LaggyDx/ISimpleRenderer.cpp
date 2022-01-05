#include "stdafx.h"
#include "ISimpleRenderer.h"

#include "SimpleRenderer.h"


namespace Dx
{
  ISimpleRenderer& ISimpleRenderer::getOrCreate(
    IRenderDevice& i_renderDevice,
    const ICamera& i_camera,
    const IResourceController& i_resourceController)
  {
    static SimpleRenderer simpleRenderer(i_renderDevice, i_camera, i_resourceController);
    return simpleRenderer;
  }

} // ns Dx

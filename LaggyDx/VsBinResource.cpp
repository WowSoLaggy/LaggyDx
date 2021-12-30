#include "stdafx.h"
#include "VsBinResource.h"

#include "RenderDevice.h"

#include <LaggySdk/StringUtils.h>


namespace Dx
{
  VsBinResource::VsBinResource(fs::path i_shaderFilePath)
    : VsResource(std::move(i_shaderFilePath))
  {
  }


  void VsBinResource::load(IRenderDevice& i_renderDevice)
  {
    ID3D10Blob* vsBuffer = nullptr;
    D3DReadFileToBlob(d_shaderFilePath.wstring().c_str(), &vsBuffer);

    loadFromBuffer(i_renderDevice, vsBuffer);

    vsBuffer->Release();
  }

} // ns Dx

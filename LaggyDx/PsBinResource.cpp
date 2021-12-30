#include "stdafx.h"
#include "PsBinResource.h"

#include "RenderDevice.h"

#include <LaggySdk/StringUtils.h>


namespace Dx
{
  PsBinResource::PsBinResource(fs::path i_shaderFilePath)
    : PsResource(std::move(i_shaderFilePath))
  {
  }


  void PsBinResource::load(IRenderDevice& i_renderDevice)
  {
    ID3D10Blob* psBuffer = nullptr;
    D3DReadFileToBlob(d_shaderFilePath.wstring().c_str(), &psBuffer);

    loadFromBuffer(i_renderDevice, psBuffer);

    psBuffer->Release();
  }

} // ns Dx

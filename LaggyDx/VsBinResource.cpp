#include "stdafx.h"
#include "VsBinResource.h"

#include <LaggySdk/StringUtils.h>


namespace Dx
{
  VsBinResource::VsBinResource(fs::path i_shaderFilePath)
    : VsResource(std::move(i_shaderFilePath))
  {
  }


  void VsBinResource::load()
  {
    ID3D10Blob* vsBuffer = nullptr;
    D3DReadFileToBlob(d_shaderFilePath.wstring().c_str(), &vsBuffer);

    loadFromBuffer(vsBuffer);

    vsBuffer->Release();
  }

} // ns Dx

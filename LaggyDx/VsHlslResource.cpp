#include "stdafx.h"
#include "VsHlslResource.h"

#include "_ShaderUtils.h"


namespace Dx
{
  VsHlslResource::VsHlslResource(fs::path i_shaderFilePath)
    : VsResource(std::move(i_shaderFilePath))
  {
  }


  void VsHlslResource::load(IRenderDevice& i_renderDevice)
  {
    ID3D10Blob* vsBuffer = nullptr;
    compileShader(d_shaderFilePath, &vsBuffer, "vs", "vs_5_0");

    loadFromBuffer(i_renderDevice, vsBuffer);

    vsBuffer->Release();
  }

} // ns Dx

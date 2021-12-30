#include "stdafx.h"
#include "PsHlslResource.h"

#include "_ShaderUtils.h"


namespace Dx
{
  PsHlslResource::PsHlslResource(fs::path i_shaderFilePath)
    : PsResource(std::move(i_shaderFilePath))
  {
  }


  void PsHlslResource::load(IRenderDevice& i_renderDevice)
  {
    ID3D10Blob* psBuffer = nullptr;
    compileShader(d_shaderFilePath, &psBuffer, "ps", "ps_5_0");

    loadFromBuffer(i_renderDevice, psBuffer);

    psBuffer->Release();
  }

} // ns Dx

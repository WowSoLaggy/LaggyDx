#include "stdafx.h"
#include "PsHlslResource.h"

#include "_ShaderUtils.h"


namespace Dx
{
  PsHlslResource::PsHlslResource(fs::path i_shaderFilePath)
    : PsResource(std::move(i_shaderFilePath))
  {
  }


  void PsHlslResource::load()
  {
    ID3D10Blob* psBuffer = nullptr;
    compileShader(d_shaderFilePath, &psBuffer, "ps", "ps_5_0");

    loadFromBuffer(psBuffer);

    psBuffer->Release();
  }

} // ns Dx

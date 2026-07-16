#include "stdafx.h"
#include "VsHlslResource.h"

#include "_ShaderUtils.h"


namespace Dx
{
  VsHlslResource::VsHlslResource(fs::path i_shaderFilePath)
    : VsResource(std::move(i_shaderFilePath))
  {
  }


  void VsHlslResource::load()
  {
    ID3D10Blob* vsBuffer = nullptr;
    compileShader(d_shaderFilePath, &vsBuffer, "vs", "vs_5_0");

    loadFromBuffer(vsBuffer);

    vsBuffer->Release();
  }

} // ns Dx

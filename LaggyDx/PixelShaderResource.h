#pragma once

#include "IPixelShaderResource.h"


namespace Dx
{
  class PixelShaderResource : public IPixelShaderResource
  {
  public:
    PixelShaderResource(fs::path i_shaderFilePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
    virtual void unload() override;

    ID3D11PixelShader* getPtr() const { return d_pixelShader; }
    ID3D11SamplerState* getSampleStatePtr() const { return d_sampleState; }

  private:
    const fs::path d_shaderFilePath;

    ID3D11PixelShader* d_pixelShader = nullptr;
    ID3D11SamplerState* d_sampleState = nullptr;
  };
} // ns Dx

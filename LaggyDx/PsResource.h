#pragma once

#include "IPsResource.h"


namespace Dx
{
  class PsResource : public IPsResource
  {
  public:
    PsResource(fs::path i_shaderFilePath);

    virtual void unload() override;

    ID3D11PixelShader* getPtr() const { return d_pixelShader; }
    ID3D11SamplerState* getSampleStatePtr() const { return d_sampleState; }

  protected:
    const fs::path d_shaderFilePath;
    void loadFromBuffer(IRenderDevice& i_renderDevice, ID3D10Blob* i_psBuffer);

  private:
    ID3D11PixelShader* d_pixelShader = nullptr;
    ID3D11SamplerState* d_sampleState = nullptr;
  };
} // ns Dx

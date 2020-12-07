#pragma once

#include "IPixelShaderResource.h"
#include "LoadableResource.h"


namespace Dx
{
  class PixelShaderResource : public LoadableResource, public IPixelShaderResource
  {
  public:
    PixelShaderResource(std::string i_shaderFilePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
    virtual void unload() override;

    ID3D11PixelShader* getPtr() const { return d_pixelShader; }
    ID3D11SamplerState* getSampleStatePtr() const { return d_sampleState; }

  private:
    const std::string d_shaderFilePath;

    ID3D11PixelShader* d_pixelShader;
    ID3D11SamplerState* d_sampleState;
  };
} // ns Dx

#pragma once

#include "ILoadableResource.h"
#include "IPixelShaderResource.h"


namespace Dx
{
  class PixelShaderResource : public ILoadableResource, public IPixelShaderResource
  {
  public:
    PixelShaderResource(std::string i_shaderFilePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
    virtual void unload() override;

    virtual bool isLoaded() const override { return d_loaded; }

    ID3D11PixelShader* getPtr() const { return d_pixelShader; }
    ID3D11SamplerState* getSampleStatePtr() const { return d_sampleState; }

  private:
    bool d_loaded = false;
    const std::string d_shaderFilePath;

    ID3D11PixelShader* d_pixelShader;
    ID3D11SamplerState* d_sampleState;
  };
} // ns Dx

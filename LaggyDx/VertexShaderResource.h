#pragma once

#include "IVertexShaderResource.h"
#include "LoadableResource.h"


namespace Dx
{
  class VertexShaderResource : public LoadableResource, public IVertexShaderResource
  {
  public:
    VertexShaderResource(fs::path i_shaderFilePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
    virtual void unload() override;

    ID3D11VertexShader* getPtr() const { return d_vertexShader; }
    ID3D11InputLayout* getLayoutPtr() const { return d_layout; }

  private:
    const fs::path d_shaderFilePath;

    ID3D11VertexShader* d_vertexShader = nullptr;
    ID3D11InputLayout* d_layout = nullptr;
  };
} // ns Dx

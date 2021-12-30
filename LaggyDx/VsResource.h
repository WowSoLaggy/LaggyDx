#pragma once

#include "IVsResource.h"


namespace Dx
{
  class VsResource : public IVsResource
  {
  public:
    VsResource(fs::path i_shaderFilePath);

    virtual void unload() override;

    ID3D11VertexShader* getPtr() const { return d_vertexShader; }
    ID3D11InputLayout* getLayoutPtr() const { return d_layout; }

  protected:
    const fs::path d_shaderFilePath;
    void loadFromBuffer(IRenderDevice& i_renderDevice, ID3D10Blob* i_psBuffer);

  private:
    ID3D11VertexShader* d_vertexShader = nullptr;
    ID3D11InputLayout* d_layout = nullptr;
  };
} // ns Dx

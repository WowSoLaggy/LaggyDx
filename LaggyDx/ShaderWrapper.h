#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class ShaderWrapper
  {
  public:
    ShaderWrapper(const RenderDevice& i_renderDevice);
    ~ShaderWrapper();

    void initVs(const void* i_shaderBytes, int i_shaderSize);
    void initPs(const void* i_shaderBytes, int i_shaderSize);

    ID3D11VertexShader* getVs() const;
    ID3D11PixelShader* getPs() const;
    ID3D11InputLayout* getLayout() const;

  private:
    const RenderDevice& d_renderDevice;

    ID3D11VertexShader* d_vs = nullptr;
    ID3D11PixelShader* d_ps = nullptr;
    ID3D11InputLayout* d_layout = nullptr;
  };

} // ns Dx

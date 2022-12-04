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
    void addSampler(bool i_wrapCoords);

    ID3D11VertexShader* getVs() const;
    ID3D11PixelShader* getPs() const;
    ID3D11InputLayout* getLayout() const;

    const std::vector<ID3D11SamplerState*>& getSamplers() const;

  private:
    const RenderDevice& d_renderDevice;

    ID3D11VertexShader* d_vs = nullptr;
    ID3D11PixelShader* d_ps = nullptr;
    ID3D11InputLayout* d_layout = nullptr;

    std::vector<ID3D11SamplerState*> d_samplers;
  };

} // ns Dx

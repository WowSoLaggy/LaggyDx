#include "stdafx.h"
#include "ShaderWrapper.h"

#include "RenderDevice.h"
#include "VertexLayout.h"


namespace Dx
{
  ShaderWrapper::ShaderWrapper(const RenderDevice& i_renderDevice)
    : d_renderDevice(i_renderDevice)
  {
    
  }

  ShaderWrapper::~ShaderWrapper()
  {
    auto dispose = [](auto** i_res) {
      if (*i_res)
      {
        (*i_res)->Release();
        *i_res = nullptr;
      }
    };

    dispose(&d_vs);
    dispose(&d_layout);
    dispose(&d_ps);

    for (auto& sampler : d_samplers)
      dispose(&sampler);
  }


  ID3D11VertexShader* ShaderWrapper::getVs() const { return d_vs; }
  ID3D11PixelShader* ShaderWrapper::getPs() const { return d_ps; }
  ID3D11InputLayout* ShaderWrapper::getLayout() const { return d_layout; }
  const std::vector<ID3D11SamplerState*>& ShaderWrapper::getSamplers() const { return d_samplers; }


  void ShaderWrapper::initVs(const void* i_shaderBytes, int i_shaderSize)
  {
    HRESULT hRes = d_renderDevice.getDevicePtr()->CreateVertexShader(
      i_shaderBytes, i_shaderSize, NULL, &d_vs);
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(d_vs != nullptr);

    const auto& layout = getVertexLayout();
    hRes = d_renderDevice.getDevicePtr()->CreateInputLayout(layout.data(), (int)layout.size(),
      i_shaderBytes, i_shaderSize, &d_layout);
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(d_layout != nullptr);
  }

  void ShaderWrapper::initPs(const void* i_shaderBytes, int i_shaderSize)
  {
    HRESULT hRes = d_renderDevice.getDevicePtr()->CreatePixelShader(
      i_shaderBytes, i_shaderSize, NULL, &d_ps);
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(d_ps != nullptr);
  }

  void ShaderWrapper::addSampler(bool i_wrapCoords)
  {
    const auto mode = i_wrapCoords ?
      D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP :
      D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;

    D3D11_SAMPLER_DESC samplerDesc{};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = mode;
    samplerDesc.AddressV = mode;
    samplerDesc.AddressW = mode;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    ID3D11SamplerState* sampler = nullptr;
    HRESULT hRes = d_renderDevice.getDevicePtr()->CreateSamplerState(&samplerDesc, &sampler);
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(sampler != nullptr);

    d_samplers.push_back(sampler);
  }

} // ns Dx

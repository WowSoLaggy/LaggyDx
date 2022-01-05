#include "stdafx.h"
#include "PsResource.h"

#include "RenderDevice.h"

#include <LaggySdk/StringUtils.h>


namespace Dx
{
  PsResource::PsResource(fs::path i_shaderFilePath)
    : d_shaderFilePath(std::move(i_shaderFilePath))
  {
  }


  void PsResource::loadFromBuffer(IRenderDevice& i_renderDevice, ID3D10Blob* i_psBuffer)
  {
    auto& renderDevice = dynamic_cast<RenderDevice&>(i_renderDevice);

    renderDevice.getDevicePtr()->CreatePixelShader(i_psBuffer->GetBufferPointer(),
      i_psBuffer->GetBufferSize(), NULL, &d_pixelShader);
    CONTRACT_ASSERT(d_pixelShader != nullptr);

    // Sampler state

    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    renderDevice.getDevicePtr()->CreateSamplerState(&samplerDesc, &d_sampleState);
    CONTRACT_ASSERT(d_sampleState != nullptr);
  }

  void PsResource::unload()
  {
    d_sampleState->Release();
    d_pixelShader->Release();
  }

} // ns Dx

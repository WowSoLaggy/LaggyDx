#include "stdafx.h"
#include "PixelShaderResource.h"

#include "RenderDevice.h"

#include <LaggySdk/StringUtils.h>


namespace Dx
{
  PixelShaderResource::PixelShaderResource(std::string i_shaderFilePath)
    : d_shaderFilePath(std::move(i_shaderFilePath))
  {
  }


  void PixelShaderResource::load(IRenderDevice& i_renderDevice)
  {
    auto& renderDevice = dynamic_cast<RenderDevice&>(i_renderDevice);

    // Pixel Shader

    ID3D10Blob* pixelShaderBuffer = nullptr;
    D3DReadFileToBlob(Sdk::getWString(d_shaderFilePath).c_str(), &pixelShaderBuffer);

    renderDevice.getDevicePtr()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
      pixelShaderBuffer->GetBufferSize(), NULL, &d_pixelShader);

    pixelShaderBuffer->Release();

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

    d_loaded = true;
  }

  void PixelShaderResource::unload()
  {
    d_loaded = false;

    d_sampleState->Release();
    d_pixelShader->Release();
  }

} // ns Dx

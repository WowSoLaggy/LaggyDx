#include "stdafx.h"
#include "ShadowMap.h"

#include "RenderDevice.h"
#include "TextureView.h"


namespace Dx
{
  ShadowMap::ShadowMap(IRenderDevice& i_renderDevice, const int i_resolution)
    : d_renderDevice(dynamic_cast<RenderDevice&>(i_renderDevice))
    , d_resolution(i_resolution)
  {
    CONTRACT_EXPECT(i_resolution > 0);
    createResources();
  }


  int ShadowMap::getResolution() const
  {
    return d_resolution;
  }

  const ITexture& ShadowMap::getTexture() const
  {
    CONTRACT_ASSERT(d_texture);
    return *d_texture;
  }


  void ShadowMap::createResources()
  {
    // Single non-MSAA texture usable both as a depth target and a shader resource
    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = d_resolution;
    textureDesc.Height = d_resolution;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    HRESULT hRes = d_renderDevice.getDevicePtr()->CreateTexture2D(&textureDesc, NULL, d_texture2D.getPp());
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(d_texture2D.isNotNullptr());

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
    depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    hRes = d_renderDevice.getDevicePtr()->CreateDepthStencilView(
      d_texture2D.get(), &depthStencilViewDesc, d_depthStencilView.getPp());
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(d_depthStencilView.isNotNullptr());

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
    shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    hRes = d_renderDevice.getDevicePtr()->CreateShaderResourceView(
      d_texture2D.get(), &shaderResourceViewDesc, d_shaderResourceView.getPp());
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(d_shaderResourceView.isNotNullptr());

    d_texture = std::make_shared<TextureView>(d_shaderResourceView.get(), textureDesc);
  }


  void ShadowMap::bind()
  {
    auto* contextPtr = d_renderDevice.getDeviceContextPtr();

    // Unbind the shadow SRV from pixel shaders before using it as a depth target
    ID3D11ShaderResourceView* nullSrvs[4] = {};
    contextPtr->PSSetShaderResources(0, 4, nullSrvs);

    contextPtr->OMSetRenderTargets(0, nullptr, d_depthStencilView.get());
    contextPtr->ClearDepthStencilView(d_depthStencilView.get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

    D3D11_VIEWPORT viewport = {};
    viewport.Width = (float)d_resolution;
    viewport.Height = (float)d_resolution;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    contextPtr->RSSetViewports(1, &viewport);
  }

  void ShadowMap::unbind()
  {
    d_renderDevice.bindDepthBuffer();
    d_renderDevice.attachViewport();
  }

} // ns Dx

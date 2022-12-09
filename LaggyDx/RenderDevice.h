#pragma once

#include "IRenderDevice.h"


namespace Dx
{
  class RenderDevice : public IRenderDevice
  {
  public:
    RenderDevice(HWND i_hWnd, Sdk::Vector2I i_resolution, bool i_debugMode);
    virtual ~RenderDevice() override;

    virtual void beginScene() override;
    virtual void endScene() override;

    virtual const Sdk::Vector2I& getResolution() const override;
    virtual void setClearColor(const Sdk::Vector4F& i_clearColor) override;
    virtual void resetState() override;

    virtual void prepareDepthBufferTexture() const override;
    virtual const ITexture& getDepthBufferTexture() const override;
    virtual void bindDepthBuffer() const override;
    virtual void unbindDepthBuffer() const override;

    ID3D11Device* getDevicePtr() const { return d_device; }
    ID3D11DeviceContext* getDeviceContextPtr() const { return d_deviceContext; }

    const D3D11_RASTERIZER_DESC& getRasterizerState() const;
    const D3D11_DEPTH_STENCIL_DESC& getDepthStencilState() const;
    const D3D11_BLEND_DESC& getBlendState() const;

    void setRasterizerState(D3D11_RASTERIZER_DESC i_state);
    void setDepthStencilState(D3D11_DEPTH_STENCIL_DESC i_state);
    void setBlendState(D3D11_BLEND_DESC i_state);

  private:
    Sdk::Vector2I d_resolution;
    float d_clearColor[4] = { 0.396f, 0.612f, 0.937f, 1.0f };

    HWND d_hWnd = nullptr;

    char d_videoCardDescription[128];

    IDXGISwapChain* d_swapChain = nullptr;
    ID3D11Device* d_device = nullptr;
    ID3D11DeviceContext* d_deviceContext = nullptr;

    ID3D11RenderTargetView* d_renderTargetView = nullptr;

    D3D11_TEXTURE2D_DESC d_depthStencilDesc = {};
    ID3D11Texture2D* d_depthBufferTexture2D = nullptr;
    ID3D11Texture2D* d_depthBufferTexture2DCopy = nullptr;
    ID3D11DepthStencilView* d_depthStencilView = nullptr;
    ID3D11ShaderResourceView* d_depthStencilTextureView = nullptr;

    ID3D11DepthStencilState* d_depthStencilState = nullptr;
    ID3D11RasterizerState* d_rasterState = nullptr;
    ID3D11BlendState* d_blendState = nullptr;

    D3D11_RASTERIZER_DESC d_rasterizerDescription = {};
    D3D11_DEPTH_STENCIL_DESC d_depthStencilDescription = {};
    D3D11_BLEND_DESC d_blendDescription = {};

    std::shared_ptr<ITexture> d_depthBufferTexture;

    void applyRasterizerState();
    void applyDepthStencilState();
    void applyBlendState();
  };

} // ns Dx

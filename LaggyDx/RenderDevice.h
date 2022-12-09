#pragma once

#include "DxResourceWrapper.h"
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
    virtual void bindDepthBuffer() override;
    virtual void unbindDepthBuffer() override;

    ID3D11Device* getDevicePtr() const { return d_device.get(); }
    ID3D11DeviceContext* getDeviceContextPtr() const { return d_deviceContext.get(); }

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

    DxResourceWrapper<ID3D11Device> d_device;
    DxResourceWrapper<ID3D11DeviceContext> d_deviceContext;
    DxResourceWrapper<IDXGISwapChain> d_swapChain;

    DxResourceWrapper<ID3D11RenderTargetView> d_renderTargetView;

    DxResourceWrapper<ID3D11Texture2D> d_depthBufferTexture2D;
    DxResourceWrapper<ID3D11Texture2D> d_depthBufferTexture2DCopy;
    DxResourceWrapper<ID3D11DepthStencilView> d_depthStencilView;
    DxResourceWrapper<ID3D11ShaderResourceView> d_depthStencilTextureView;

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

    void createDeviceAndSwapChain(const RefreshRate& i_refreshRate, bool i_debugMode);
    void createRenderTargetView();
    void createDepthBuffer();
  };

} // ns Dx

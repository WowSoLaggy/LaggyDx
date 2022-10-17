#pragma once

#include "IRenderDevice.h"


struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11RasterizerState;


namespace Dx
{
  class RenderDevice : public IRenderDevice
  {
  public:
    RenderDevice(HWND i_hWnd, Sdk::Vector2I i_resolution, bool i_debugMode = false);
    virtual ~RenderDevice() override;

    virtual void beginScene() override;
    virtual void endScene() override;

    virtual void switchFillMode() override;
    virtual void setFillMode(FillMode i_fillMode) override;

    virtual void setClearColor(const Sdk::Vector4F& i_clearColor) override;

    virtual const Sdk::Vector2I& getResolution() const override;

    ID3D11Device* getDevicePtr() const { return d_device; }
    ID3D11DeviceContext* getDeviceContextPtr() const { return d_deviceContext; }

    void resetState();

  private:
    const bool c_vSyncEnabled = true;
    const bool c_fullScreen = false;

    enum class MsaaMode
    {
      None = 1,
      Two = 2,
      Four = 4,
      Eight = 8,
    };
    const MsaaMode c_msaaMode = MsaaMode::Two;
    const bool isMsaaEnabled() const { return c_msaaMode != MsaaMode::None; }

  private:
    Sdk::Vector2I d_resolution;
    float d_clearColor[4] = { 0.396f, 0.612f, 0.937f, 1.0f };

    FillMode d_fillMode = FillMode::Solid;

    HWND d_hWnd = nullptr;

    int d_videoCardMemory = 0;
    char d_videoCardDescription[128];
    IDXGISwapChain* d_swapChain = nullptr;
    ID3D11Device* d_device = nullptr;
    ID3D11DeviceContext* d_deviceContext = nullptr;
    ID3D11RenderTargetView* d_renderTargetView = nullptr;
    ID3D11Texture2D* d_depthStencilBuffer = nullptr;
    ID3D11DepthStencilState* d_depthStencilState = nullptr;
    ID3D11DepthStencilView* d_depthStencilView = nullptr;
    ID3D11RasterizerState* d_rasterState = nullptr;
    ID3D11BlendState* d_blendState = nullptr;
  };

} // ns Dx

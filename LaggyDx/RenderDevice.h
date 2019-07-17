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
    RenderDevice(HWND i_hWnd, int i_resolutionX, int i_resolutionY);
    virtual ~RenderDevice() override;

    virtual void beginScene() override;
    virtual void endScene() override;

    virtual void switchFillMode() override;
    virtual void setFillMode(FillMode i_fillMode) override;

    ID3D11Device* getDevicePtr() { return d_device; }
    ID3D11DeviceContext* getDeviceContextPtr() { return d_deviceContext; }

    void resetState();

  private:
    const bool c_vSyncEnabled = true;
    const bool c_fullScreen = false;
    const float c_clearColor[4] = { 0.396f, 0.612f, 0.937f, 1.0f };

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
    FillMode d_fillMode;

    HWND d_hWnd;

    int d_videoCardMemory;
    char d_videoCardDescription[128];
    IDXGISwapChain* d_swapChain;
    ID3D11Device* d_device;
    ID3D11DeviceContext* d_deviceContext;
    ID3D11RenderTargetView* d_renderTargetView;
    ID3D11Texture2D* d_depthStencilBuffer;
    ID3D11DepthStencilState* d_depthStencilState;
    ID3D11DepthStencilView* d_depthStencilView;
    ID3D11RasterizerState* d_rasterState;
  };

} // ns Dx

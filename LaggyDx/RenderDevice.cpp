#include "stdafx.h"
#include "RenderDevice.h"

#include "RefreshRate.h"
#include "TextureView.h"

#include <dxgi1_2.h>


namespace Dx
{
  namespace
  {
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
    const bool isMsaaEnabled() { return c_msaaMode != MsaaMode::None; }
    
    
    D3D11_RASTERIZER_DESC getDefaultRasterizerDescription()
    {
      D3D11_RASTERIZER_DESC rasterizerDescription = {};

      rasterizerDescription.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
      rasterizerDescription.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
      rasterizerDescription.FrontCounterClockwise = true;
      rasterizerDescription.DepthBias = 0;
      rasterizerDescription.DepthBiasClamp = 0.0f;
      rasterizerDescription.SlopeScaledDepthBias = 0.0f;
      rasterizerDescription.DepthClipEnable = true;
      rasterizerDescription.ScissorEnable = false;
      rasterizerDescription.MultisampleEnable = isMsaaEnabled();
      rasterizerDescription.AntialiasedLineEnable = false;

      return rasterizerDescription;
    }

    D3D11_DEPTH_STENCIL_DESC getDefaultDepthStencilDescription()
    {
      D3D11_DEPTH_STENCIL_DESC depthStencilDescription = {};

      // Set up the description of the stencil state
      depthStencilDescription.DepthEnable = true;
      depthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
      depthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

      depthStencilDescription.StencilEnable = FALSE;
      depthStencilDescription.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
      depthStencilDescription.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

      depthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
      depthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
      depthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
      depthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

      depthStencilDescription.BackFace = depthStencilDescription.FrontFace;

      return depthStencilDescription;
    }

    D3D11_BLEND_DESC getDefaultBlendDescription()
    {
      D3D11_BLEND_DESC blendDescription = {};

      blendDescription.RenderTarget[0].BlendEnable = true;
      blendDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
      blendDescription.RenderTarget[0].SrcBlendAlpha = blendDescription.RenderTarget[0].SrcBlend;
      blendDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
      blendDescription.RenderTarget[0].DestBlendAlpha = blendDescription.RenderTarget[0].DestBlend;

      blendDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
      blendDescription.RenderTarget[0].BlendOpAlpha = blendDescription.RenderTarget[0].BlendOp;

      blendDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

      return blendDescription;
    }

  } // anonym NS


  RenderDevice::RenderDevice(HWND i_hWnd, Sdk::Vector2I i_resolution, const int i_refreshRate, const bool i_debugMode)
    : d_resolution(std::move(i_resolution))
    , d_hWnd(i_hWnd)
  {
    createDeviceAndSwapChain(i_refreshRate, i_debugMode);
    createRenderTargetView();
    createDepthBuffer();

    bindDepthBuffer();
    resetState();
    attachViewport();
  }

  RenderDevice::~RenderDevice()
  {
    // Before shutting down set to windowed mode or when you release the swap chain it will throw an exception
    if (d_swapChain.isNotNullptr())
      d_swapChain->SetFullscreenState(false, NULL);
  }


  void RenderDevice::begin()
  {
    // Clear the back buffer
    d_deviceContext->ClearRenderTargetView(d_renderTargetView.get(), d_clearColor);

    // Clear the depth buffer
    d_deviceContext->ClearDepthStencilView(d_depthStencilView.get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
  }

  void RenderDevice::end()
  {
    // Resolve the offscreen MSAA color into the single-sample back buffer before presenting.
    d_deviceContext->ResolveSubresource(
      d_backBufferTexture2D.get(), 0, d_msaaColorTexture2D.get(), 0, DXGI_FORMAT_R8G8B8A8_UNORM);

    // Present the back buffer; with the flip model Present(1,...) paces the loop to the refresh rate.
    unsigned int syncInterval = c_vSyncEnabled ? 1 : 0;
    d_swapChain->Present(syncInterval, 0);
  }


  void RenderDevice::resetState()
  {
    setRasterizerState(getDefaultRasterizerDescription());
    setDepthStencilState(getDefaultDepthStencilDescription());
    setBlendState(getDefaultBlendDescription());
    resetShaders();
  }

  void RenderDevice::resetShaders()
  {
    d_deviceContext->VSSetShader(nullptr, nullptr, 0);
    d_deviceContext->PSSetShader(nullptr, nullptr, 0);
  }


  const D3D11_RASTERIZER_DESC& RenderDevice::getRasterizerState() const
  {
    return d_rasterizerDescription;
  }

  const D3D11_DEPTH_STENCIL_DESC& RenderDevice::getDepthStencilState() const
  {
    return d_depthStencilDescription;
  }

  const D3D11_BLEND_DESC& RenderDevice::getBlendState() const
  {
    return d_blendDescription;
  }

  void RenderDevice::setRasterizerState(D3D11_RASTERIZER_DESC i_state)
  {
    d_rasterizerDescription = std::move(i_state);
    applyRasterizerState();
  }

  void RenderDevice::setDepthStencilState(D3D11_DEPTH_STENCIL_DESC i_state)
  {
    d_depthStencilDescription = std::move(i_state);
    applyDepthStencilState();
  }

  void RenderDevice::setBlendState(D3D11_BLEND_DESC i_state)
  {
    d_blendDescription = std::move(i_state);
    applyBlendState();
  }

  void RenderDevice::applyRasterizerState()
  {
    HRESULT result = d_device->CreateRasterizerState(&d_rasterizerDescription, d_rasterState.getPp());
    CONTRACT_ASSERT(!FAILED(result));
    CONTRACT_ASSERT(d_rasterState.isNotNullptr());

    d_deviceContext->RSSetState(d_rasterState.get());
  }

  void RenderDevice::applyDepthStencilState()
  {
    HRESULT result = d_device->CreateDepthStencilState(&d_depthStencilDescription, d_depthStencilState.getPp());
    CONTRACT_ASSERT(!FAILED(result));
    CONTRACT_ASSERT(d_depthStencilState.isNotNullptr());

    d_deviceContext->OMSetDepthStencilState(d_depthStencilState.get(), 1);
  }

  void RenderDevice::applyBlendState()
  {
    HRESULT result = d_device->CreateBlendState(&d_blendDescription, d_blendState.getPp());
    CONTRACT_ASSERT(!FAILED(result));
    CONTRACT_ASSERT(d_blendState.isNotNullptr());

    d_deviceContext->OMSetBlendState(d_blendState.get(), nullptr, 0xffffffff);
  }


  void RenderDevice::setClearColor(const Sdk::Vector4F& i_clearColor)
  {
    d_clearColor[0] = i_clearColor.x;
    d_clearColor[1] = i_clearColor.y;
    d_clearColor[2] = i_clearColor.z;
    d_clearColor[3] = i_clearColor.w;
  }

  const Sdk::Vector2I& RenderDevice::getResolution() const
  {
    return d_resolution;
  }


  void RenderDevice::prepareDepthBufferTexture() const
  {
    d_deviceContext->CopyResource(d_depthBufferTexture2DCopy.get(), d_depthBufferTexture2D.get());
  }

  const ITexture& RenderDevice::getDepthBufferTexture() const
  {
    CONTRACT_ASSERT(d_depthBufferTexture);
    return *d_depthBufferTexture;
  }

  void RenderDevice::bindDepthBuffer()
  {
    d_deviceContext->OMSetRenderTargets(1, d_renderTargetView.getPp(), d_depthStencilView.get());
  }

  void RenderDevice::unbindDepthBuffer()
  {
    d_deviceContext->OMSetRenderTargets(1, d_renderTargetView.getPp(), nullptr);
  }


  void RenderDevice::createDeviceAndSwapChain(const int i_refreshRate, const bool i_debugMode)
  {
    // Flip-model back buffers must be single-sample; MSAA is done on an offscreen target and resolved before present.
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;

    UINT flags = 0;
    if (i_debugMode)
      flags |= D3D11_CREATE_DEVICE_DEBUG;

    HRESULT hRes = D3D11CreateDevice(
      NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
      flags, &featureLevel, 1,
      D3D11_SDK_VERSION, d_device.getPp(), NULL, d_deviceContext.getPp());
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(d_device.isNotNullptr());
    CONTRACT_ASSERT(d_deviceContext.isNotNullptr());

    // Reach the DXGI factory through the device to create a flip-model swap chain for the window.
    DxResourceWrapper<IDXGIDevice> dxgiDevice;
    hRes = d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)dxgiDevice.getPp());
    CONTRACT_ASSERT(!FAILED(hRes));

    DxResourceWrapper<IDXGIAdapter> dxgiAdapter;
    hRes = dxgiDevice->GetAdapter(dxgiAdapter.getPp());
    CONTRACT_ASSERT(!FAILED(hRes));

    DxResourceWrapper<IDXGIFactory2> dxgiFactory;
    hRes = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)dxgiFactory.getPp());
    CONTRACT_ASSERT(!FAILED(hRes));

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = d_resolution.x;
    swapChainDesc.Height = d_resolution.y;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    // Flip model needs at least two buffers; the present queue is what paces the loop to the refresh rate.
    swapChainDesc.BufferCount = 2;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
    swapChainDesc.Flags = 0;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc = {};
    fullscreenDesc.RefreshRate.Numerator = i_refreshRate;
    fullscreenDesc.RefreshRate.Denominator = 1;
    fullscreenDesc.Windowed = !c_fullScreen;

    hRes = dxgiFactory->CreateSwapChainForHwnd(
      d_device.get(), d_hWnd, &swapChainDesc, &fullscreenDesc, nullptr, d_swapChain.getPp());
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(d_swapChain.isNotNullptr());
  }

  void RenderDevice::createRenderTargetView()
  {
    // Keep a handle to the single-sample back buffer - it is only ever the resolve destination.
    HRESULT hRes = d_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)d_backBufferTexture2D.getPp());
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(d_backBufferTexture2D.isNotNullptr());

    // The scene renders into an offscreen MSAA color target matching the back buffer's format and size.
    D3D11_TEXTURE2D_DESC colorDesc = {};
    colorDesc.Width = d_resolution.x;
    colorDesc.Height = d_resolution.y;
    colorDesc.MipLevels = 1;
    colorDesc.ArraySize = 1;
    colorDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    colorDesc.SampleDesc.Count = static_cast<int>(c_msaaMode);
    colorDesc.SampleDesc.Quality = 0;
    colorDesc.Usage = D3D11_USAGE_DEFAULT;
    colorDesc.BindFlags = D3D11_BIND_RENDER_TARGET;

    hRes = d_device->CreateTexture2D(&colorDesc, NULL, d_msaaColorTexture2D.getPp());
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(d_msaaColorTexture2D.isNotNullptr());

    // The render target view targets the offscreen MSAA color, not the back buffer.
    hRes = d_device->CreateRenderTargetView(d_msaaColorTexture2D.get(), NULL, d_renderTargetView.getPp());
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(d_renderTargetView.isNotNullptr());
  }

  void RenderDevice::createDepthBuffer()
  {
    // Set up the description of the depth buffer
    D3D11_TEXTURE2D_DESC depthStencilDesc = {};
    depthStencilDesc.Width = d_resolution.x;
    depthStencilDesc.Height = d_resolution.y;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    depthStencilDesc.SampleDesc.Count = static_cast<int>(c_msaaMode);
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    // Create the texture for the depth buffer using the filled out description
    HRESULT hRes = d_device->CreateTexture2D(&depthStencilDesc, NULL, d_depthBufferTexture2D.getPp());
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(d_depthBufferTexture2D.isNotNullptr());

    // Create copy for using in shaders
    D3D11_TEXTURE2D_DESC depthTextureDesc = depthStencilDesc;
    depthTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    hRes = d_device->CreateTexture2D(&depthTextureDesc, NULL, d_depthBufferTexture2DCopy.getPp());
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(d_depthBufferTexture2DCopy.isNotNullptr());

    // Set up the depth stencil view description
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
    depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthStencilViewDesc.ViewDimension =
      isMsaaEnabled() ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // Create the depth stencil view
    hRes = d_device->CreateDepthStencilView(
      d_depthBufferTexture2D.get(), &depthStencilViewDesc, d_depthStencilView.getPp());
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(d_depthStencilView.isNotNullptr());

    // Create ID3D11ShaderResourceView from depth stencil texture
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
    shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
    shaderResourceViewDesc.ViewDimension =
      isMsaaEnabled() ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    hRes = d_device->CreateShaderResourceView(
      d_depthBufferTexture2DCopy.get(), &shaderResourceViewDesc, d_depthStencilTextureView.getPp());
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(d_depthStencilTextureView.isNotNullptr());

    // Create depth buffer texture
    d_depthBufferTexture = std::make_shared<TextureView>(d_depthStencilTextureView.get(), depthTextureDesc);
  }

  void RenderDevice::attachViewport()
  {
    // Setup the viewport for rendering
    D3D11_VIEWPORT viewport = {};
    viewport.Width = (float)d_resolution.x;
    viewport.Height = (float)d_resolution.y;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    // Create the viewport
    d_deviceContext->RSSetViewports(1, &viewport);
  }

} // ns Dx

#include "stdafx.h"
#include "RenderDevice.h"

#include "MemoryTexture.h"


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

      rasterizerDescription.FillMode = D3D11_FILL_SOLID;
      rasterizerDescription.CullMode = D3D11_CULL_BACK;
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


  RenderDevice::RenderDevice(HWND i_hWnd, const Sdk::Vector2I i_resolution, const bool i_debugMode)
    : d_resolution(i_resolution)
    , d_hWnd(i_hWnd)
  {
    // Create a DirectX graphics interface factory
    IDXGIFactory* factory = nullptr;
    HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    CONTRACT_ASSERT(!FAILED(result));

    // Use the factory to create an adapter for the primary graphics interface (video card)
    IDXGIAdapter* adapter = nullptr;
    result = factory->EnumAdapters(0, &adapter);
    CONTRACT_ASSERT(!FAILED(result));

    // Enumerate the primary adapter output (monitor)
    IDXGIOutput* adapterOutput = nullptr;
    result = adapter->EnumOutputs(0, &adapterOutput);
    CONTRACT_ASSERT(!FAILED(result));

    // Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor)
    unsigned int numModes = 0;
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
    CONTRACT_ASSERT(!FAILED(result));

    // Create a list to hold all the possible display modes for this monitor/video card combination
    DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
    CONTRACT_ASSERT(displayModeList != nullptr);

    // Now fill the display mode list structures
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
    CONTRACT_ASSERT(!FAILED(result));

    // Now go through all the display modes and find the one that matches the screen width and height.
    // When a match is found store the numerator and denominator of the refresh rate for that monitor.
    unsigned int numerator = 0;
    unsigned int denominator = 1;
    for (unsigned int i = 0; i < numModes; ++i)
    {
      if (displayModeList[i].Width == (unsigned int)d_resolution.x)
      {
        if (displayModeList[i].Height == (unsigned int)d_resolution.y)
        {
          numerator = displayModeList[i].RefreshRate.Numerator;
          denominator = displayModeList[i].RefreshRate.Denominator;
          break;
        }
      }
    }

    // Get the adapter (video card) description
    DXGI_ADAPTER_DESC adapterDesc = {};
    result = adapter->GetDesc(&adapterDesc);
    CONTRACT_ASSERT(!FAILED(result));

    // Store the dedicated video card memory in megabytes
    d_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

    // Convert the name of the video card to a character array and store it
    size_t stringLength = 0;
    int error = wcstombs_s(&stringLength, d_videoCardDescription, 128, adapterDesc.Description, 128);
    CONTRACT_ASSERT(error == 0);

    // Release the display mode list
    delete[] displayModeList;
    displayModeList = 0;

    // Release the adapter output
    adapterOutput->Release();
    adapterOutput = 0;

    // Release the adapter
    adapter->Release();
    adapter = 0;

    // Release the factory
    factory->Release();
    factory = 0;

    // Initialize the swap chain description
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

    // Set to a single back buffer
    swapChainDesc.BufferCount = 1;

    // Set the width and height of the back buffer
    swapChainDesc.BufferDesc.Width = d_resolution.x;
    swapChainDesc.BufferDesc.Height = d_resolution.y;

    // Set regular 32-bit surface for the back buffer
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    // Set the refresh rate of the back buffer
    if (c_vSyncEnabled)
    {
      swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
      swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
    }
    else
    {
      swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
      swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    }

    // Set the usage of the back buffer
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    // Set the handle for the window to render to
    swapChainDesc.OutputWindow = d_hWnd;

    // Turn multisampling off
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    // Set to full screen or windowed mode
    if (c_fullScreen)
      swapChainDesc.Windowed = false;
    else
      swapChainDesc.Windowed = true;

    // Set the scan line ordering and scaling to unspecified
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    // Discard the back buffer contents after presenting
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // Don't set the advanced flags
    swapChainDesc.Flags = 0;

    // MSAA settings
    swapChainDesc.SampleDesc.Count = static_cast<int>(c_msaaMode);

    // Set the feature level to DirectX 11
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;

    UINT flags = 0;
    if (i_debugMode)
      flags |= D3D11_CREATE_DEVICE_DEBUG;

    // Create the swap chain, Direct3D device, and Direct3D device context
    result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
      flags, &featureLevel, 1,
      D3D11_SDK_VERSION, &swapChainDesc, &d_swapChain, &d_device, NULL, &d_deviceContext);
    CONTRACT_ASSERT(!FAILED(result));

    // Get the pointer to the back buffer
    ID3D11Texture2D* backBufferPtr = nullptr;
    result = d_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
    CONTRACT_ASSERT(!FAILED(result));

    // Create the render target view with the back buffer pointer
    result = d_device->CreateRenderTargetView(backBufferPtr, NULL, &d_renderTargetView);
    CONTRACT_ASSERT(!FAILED(result));

    // Release pointer to the back buffer as we no longer need it
    backBufferPtr->Release();
    backBufferPtr = 0;

    // Initialize the description of the depth buffer
    d_depthStencilDesc = {};

    // Set up the description of the depth buffer
    d_depthStencilDesc.Width = d_resolution.x;
    d_depthStencilDesc.Height = d_resolution.y;
    d_depthStencilDesc.MipLevels = 1;
    d_depthStencilDesc.ArraySize = 1;
    d_depthStencilDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    d_depthStencilDesc.SampleDesc.Count = static_cast<int>(c_msaaMode);
    d_depthStencilDesc.SampleDesc.Quality = 0;
    d_depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    d_depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    d_depthStencilDesc.CPUAccessFlags = 0;
    d_depthStencilDesc.MiscFlags = 0;

    // Create the texture for the depth buffer using the filled out description
    result = d_device->CreateTexture2D(&d_depthStencilDesc, NULL, &d_depthBufferTexture2D);
    CONTRACT_ASSERT(!FAILED(result));

    // Initailze the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};

    // Set up the depth stencil view description
    depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthStencilViewDesc.ViewDimension =
      isMsaaEnabled() ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // Create the depth stencil view
    result = d_device->CreateDepthStencilView(d_depthBufferTexture2D, &depthStencilViewDesc, &d_depthStencilView);
    CONTRACT_ASSERT(!FAILED(result));

    // Bind the render target view and depth stencil buffer to the output render pipeline
    bindDepthBuffer();

    // Create ID3D11ShaderResourceView from depth stencil texture
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
    shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
    shaderResourceViewDesc.ViewDimension = 
      isMsaaEnabled() ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    result = d_device->CreateShaderResourceView(
      d_depthBufferTexture2D, &shaderResourceViewDesc, &d_depthStencilTextureView);
    CONTRACT_ASSERT(!FAILED(result));

    // Create depth buffer texture
    d_depthBufferTexture = std::make_shared<MemoryTexture>(d_depthStencilTextureView, d_depthStencilDesc);

    // Reset states
    resetState();

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

  RenderDevice::~RenderDevice()
  {
    // Before shutting down set to windowed mode or when you release the swap chain it will throw an exception
    if (d_swapChain)
      d_swapChain->SetFullscreenState(false, NULL);

    auto release = [](auto** i_res) {
      if (*i_res)
      {
        (*i_res)->Release();
        (*i_res) = nullptr;
      }
    };

    release(&d_blendState);
    release(&d_rasterState);
    release(&d_depthStencilTextureView);
    release(&d_depthStencilView);
    release(&d_depthStencilState);
    release(&d_depthBufferTexture2D);
    release(&d_renderTargetView);
    release(&d_deviceContext);
    release(&d_device);
    release(&d_swapChain);
  }


  void RenderDevice::beginScene()
  {
    // Clear the back buffer
    d_deviceContext->ClearRenderTargetView(d_renderTargetView, d_clearColor);

    // Clear the depth buffer
    d_deviceContext->ClearDepthStencilView(d_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
  }

  void RenderDevice::endScene()
  {
    // Present the back buffer to the screen since rendering is complete
    unsigned int nominator = c_vSyncEnabled ? 1 : 0;
    d_swapChain->Present(nominator, 0);
  }


  void RenderDevice::resetState()
  {
    setRasterizerState(getDefaultRasterizerDescription());
    setDepthStencilState(getDefaultDepthStencilDescription());
    setBlendState(getDefaultBlendDescription());
  }

  D3D11_RASTERIZER_DESC RenderDevice::getRasterizerState() const
  {
    return d_rasterizerDescription;
  }

  D3D11_DEPTH_STENCIL_DESC RenderDevice::getDepthStencilState() const
  {
    return d_depthStencilDescription;
  }

  D3D11_BLEND_DESC RenderDevice::getBlendState() const
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
    HRESULT result = d_device->CreateRasterizerState(&d_rasterizerDescription, &d_rasterState);
    CONTRACT_ASSERT(!FAILED(result));
    d_deviceContext->RSSetState(d_rasterState);
  }

  void RenderDevice::applyDepthStencilState()
  {
    HRESULT result = d_device->CreateDepthStencilState(&d_depthStencilDescription, &d_depthStencilState);
    CONTRACT_ASSERT(!FAILED(result));
    d_deviceContext->OMSetDepthStencilState(d_depthStencilState, 1);
  }

  void RenderDevice::applyBlendState()
  {
    HRESULT result = d_device->CreateBlendState(&d_blendDescription, &d_blendState);
    CONTRACT_ASSERT(!FAILED(result));
    d_deviceContext->OMSetBlendState(d_blendState, nullptr, 0xffffffff);
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


  const ITexture& RenderDevice::getDepthBufferTexture() const
  {
    CONTRACT_ASSERT(d_depthBufferTexture);
    return *d_depthBufferTexture;
  }

  void RenderDevice::bindDepthBuffer() const
  {
    d_deviceContext->OMSetRenderTargets(1, &d_renderTargetView, d_depthStencilView);
  }

  void RenderDevice::unbindDepthBuffer() const
  {
    d_deviceContext->OMSetRenderTargets(1, &d_renderTargetView, nullptr);
  }

} // ns Dx

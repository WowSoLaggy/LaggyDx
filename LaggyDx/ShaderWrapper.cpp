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
  }


  ID3D11VertexShader* ShaderWrapper::getVs() const { return d_vs; }
  ID3D11PixelShader* ShaderWrapper::getPs() const { return d_ps; }
  ID3D11InputLayout* ShaderWrapper::getLayout() const { return d_layout; }


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

} // ns Dx

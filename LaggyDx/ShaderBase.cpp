#include "stdafx.h"
#include "ShaderBase.h"

#include "App.h"
#include "RenderDevice.h"


namespace Dx
{
  ShaderBase::ShaderBase()
    : d_shaders(getRenderDevice())
  {
  }


  ShaderWrapper& ShaderBase::getShaders() { return d_shaders; }
  const ShaderWrapper& ShaderBase::getShaders() const { return d_shaders; }


  RenderDevice& ShaderBase::getRenderDevice() const
  {
    return dynamic_cast<RenderDevice&>(App::get().getRenderDevice());
  }

  IResourceController& ShaderBase::getResourceController() const
  {
    return App::get().getResourceController();
  }


  bool ShaderBase::getFillMode() const
  {
    return d_solidFillMode;
  }

  void ShaderBase::setFillMode(const bool i_solid)
  {
    d_solidFillMode = i_solid;
  }


  void ShaderBase::setRenderStates() const
  {
    getRenderDevice().resetState();
    
    if (!d_solidFillMode)
    {
      auto rasterizerState = getRenderDevice().getRasterizerState();
      rasterizerState.FillMode = D3D11_FILL_WIREFRAME;
      getRenderDevice().setRasterizerState(std::move(rasterizerState));
    }

    if (!d_enableDepthBuffer)
    {
      auto depthStencilState = getRenderDevice().getDepthStencilState();
      depthStencilState.DepthEnable = false;
      getRenderDevice().setDepthStencilState(depthStencilState);
    }
  }

  void ShaderBase::setShaders() const
  {
    getRenderDevice().getDeviceContextPtr()->IASetInputLayout(getShaders().getLayout());
    getRenderDevice().getDeviceContextPtr()->VSSetShader(getShaders().getVs(), nullptr, 0);
    getRenderDevice().getDeviceContextPtr()->PSSetShader(getShaders().getPs(), nullptr, 0);

    const auto& samplers = getShaders().getSamplers();
    getRenderDevice().getDeviceContextPtr()->PSSetSamplers(
      0, (int)samplers.size(), samplers.data());
  }


  void ShaderBase::resetVsResources(const int i_numSlots) const
  {
    std::vector<ID3D11ShaderResourceView*> slots(i_numSlots, nullptr);
    getRenderDevice().getDeviceContextPtr()->VSSetShaderResources(0, i_numSlots, slots.data());
  }

  void ShaderBase::resetPsResources(const int i_numSlots) const
  {
    std::vector<ID3D11ShaderResourceView*> slots(i_numSlots, nullptr);
    getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(0, i_numSlots, slots.data());
  }


  void ShaderBase::drawIndexed(const int i_indexCount, const int i_startIndex) const
  {
    getRenderDevice().getDeviceContextPtr()->DrawIndexed(i_indexCount, i_startIndex, 0);
  }


  void ShaderBase::enableDepthBuffer()
  {
    d_enableDepthBuffer = true;
  }

  void ShaderBase::disableDepthBuffer()
  {
    d_enableDepthBuffer = false;
  }

} // ns Dx

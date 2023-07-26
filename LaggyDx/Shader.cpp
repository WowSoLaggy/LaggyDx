#include "stdafx.h"
#include "Shader.h"

#include "App.h"
#include "RenderDevice.h"


namespace Dx
{
  Shader::Shader()
    : d_shaders(getRenderDevice())
  {
  }


  ShaderWrapper& Shader::getShaders() { return d_shaders; }
  const ShaderWrapper& Shader::getShaders() const { return d_shaders; }


  RenderDevice& Shader::getRenderDevice() const
  {
    return dynamic_cast<RenderDevice&>(App::get().getRenderDevice());
  }

  IResourceController& Shader::getResourceController() const
  {
    return App::get().getResourceController();
  }


  bool Shader::getFillMode() const
  {
    return d_solidFillMode;
  }

  void Shader::setFillMode(const bool i_solid)
  {
    d_solidFillMode = i_solid;
  }


  void Shader::setRenderStates() const
  {
    getRenderDevice().resetState();
    if (!d_solidFillMode)
    {
      auto rasterizerState = getRenderDevice().getRasterizerState();
      rasterizerState.FillMode = D3D11_FILL_WIREFRAME;
      getRenderDevice().setRasterizerState(std::move(rasterizerState));
    }
  }

  void Shader::setShaders() const
  {
    getRenderDevice().getDeviceContextPtr()->IASetInputLayout(getShaders().getLayout());
    getRenderDevice().getDeviceContextPtr()->VSSetShader(getShaders().getVs(), nullptr, 0);
    getRenderDevice().getDeviceContextPtr()->PSSetShader(getShaders().getPs(), nullptr, 0);

    const auto& samplers = getShaders().getSamplers();
    getRenderDevice().getDeviceContextPtr()->PSSetSamplers(
      0, (int)samplers.size(), samplers.data());
  }


  void Shader::resetVsResources(const int i_numSlots) const
  {
    std::vector<ID3D11ShaderResourceView*> slots(i_numSlots, nullptr);
    getRenderDevice().getDeviceContextPtr()->VSSetShaderResources(0, i_numSlots, slots.data());
  }

  void Shader::resetPsResources(const int i_numSlots) const
  {
    std::vector<ID3D11ShaderResourceView*> slots(i_numSlots, nullptr);
    getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(0, i_numSlots, slots.data());
  }


  void Shader::drawIndexed(const int i_count, const int i_startIndex) const
  {
    getRenderDevice().getDeviceContextPtr()->DrawIndexed(i_count, i_startIndex, 0);
  }

} // ns Dx

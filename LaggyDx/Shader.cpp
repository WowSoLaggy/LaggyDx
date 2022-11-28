#include "stdafx.h"
#include "Shader.h"

#include "RenderDevice.h"


namespace Dx
{
  Shader::Shader(IRenderDevice& i_renderDevice)
    : d_renderDevice(dynamic_cast<RenderDevice&>(i_renderDevice))
    , d_shaders(d_renderDevice)
  {
  }


  RenderDevice& Shader::getRenderDevice() const { return d_renderDevice; }
  ShaderWrapper& Shader::getShaders() { return d_shaders; }
  const ShaderWrapper& Shader::getShaders() const { return d_shaders; }


  bool Shader::getFillMode() const
  {
    return d_solidFillMode;
  }

  void Shader::setFillMode(bool i_solid)
  {
    d_solidFillMode = i_solid;
  }


  void Shader::setRenderStates() const
  {
    getRenderDevice().resetState();
    if (!d_solidFillMode)
      getRenderDevice().setFillMode(false);
  }

} // ns Dx

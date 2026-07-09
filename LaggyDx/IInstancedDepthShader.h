#pragma once

#include "LaggyDxFwd.h"
#include "ShaderBase.h"


namespace Dx
{
  // Depth-only instanced shader rendering a model many times into a shadow map
  class IInstancedDepthShader : public ShaderBase
  {
  public:
    static std::shared_ptr<IInstancedDepthShader> create(const ShadowCamera& i_shadowCamera);

  public:
    virtual void draw(const IObject3& i_object, const IInstanceBuffer& i_instances) const = 0;
  };

} // ns Dx

#pragma once

#include "LaggyDxFwd.h"
#include "Shader3d.h"


namespace Dx
{
  // Depth-only shader rendering IObject3s into a shadow map from the light's viewpoint
  class IDepthShader : public Shader3d
  {
  public:
    static std::shared_ptr<IDepthShader> create(const ShadowCamera& i_shadowCamera);
  };

} // ns Dx

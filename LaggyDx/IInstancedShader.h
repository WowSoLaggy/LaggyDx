#pragma once

#include "LaggyDxFwd.h"
#include "ShaderBase.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  // Draws one model many times per call, per-instance transforms coming from an IInstanceBuffer.
  class IInstancedShader : public ShaderBase
  {
  public:
    static std::shared_ptr<IInstancedShader> create(const ICamera3& i_camera);

  public:
    virtual void setLightDirection(Sdk::Vector3D i_direction) = 0;
    virtual void setLightColor(const Sdk::Vector4D& i_color) = 0;
    virtual void setAmbientStrength(double i_strength) = 0;

    virtual void setShadowMap(const ITexture& i_shadowMap) = 0;
    virtual void setShadowCamera(const ShadowCamera& i_shadowCamera) = 0;

    virtual void draw(const IObject3& i_object, const IInstanceBuffer& i_instances) const = 0;
  };

} // ns Dx

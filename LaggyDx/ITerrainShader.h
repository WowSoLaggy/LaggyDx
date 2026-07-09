#pragma once

#include "Shader3d.h"
#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class ITerrainShader : public Shader3d
  {
  public:
    static std::shared_ptr<ITerrainShader> create(const ICamera3& i_camera);

  public:
    virtual void setLightDirection(Sdk::Vector3D i_direction) = 0;
    virtual void setLightColor(const Sdk::Vector4D& i_color) = 0;
    virtual void setAmbientStrength(double i_strength) = 0;

    virtual void setShadowMap(const ITexture& i_shadowMap) = 0;
    virtual void setShadowCamera(const ShadowCamera& i_shadowCamera) = 0;
  };

} // ns Dx

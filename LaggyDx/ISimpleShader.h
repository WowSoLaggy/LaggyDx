#pragma once

#include "Shader3d.h"
#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class ISimpleShader : public Shader3d
  {
  public:
    static std::shared_ptr<ISimpleShader> create(const ICamera3& i_camera);

  public:
    virtual void setLightDirection(Sdk::Vector3D i_direction) = 0;
    virtual void setLightColor(const Sdk::Vector4D& i_color) = 0;
    virtual void setAmbientStrength(double i_strength) = 0;

    // Wires one shadow cascade (0 = nearest); the PS samples the first cascade containing the pixel
    virtual void setShadowCascade(int i_cascade, const ShadowCamera& i_camera, const ITexture& i_map) = 0;
  };

} // ns Dx

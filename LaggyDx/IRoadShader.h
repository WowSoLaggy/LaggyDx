#pragma once

#include "Shader3d.h"
#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  // Draws road ribbons (and crossroad patches) draped over the terrain. Like the
  // terrain shader it applies diffuse + specular lighting, but it samples a single
  // texture by the mesh's baked UVs (so the asphalt follows the spline) instead of
  // tiling by world XZ. Objects may carry their own texture (IObject3::getTexture)
  // to override the default road texture - that is how crossroad patches get their
  // own image while sharing this shader.
  class IRoadShader : public Shader3d
  {
  public:
    static std::shared_ptr<IRoadShader> create(const ICamera3& i_camera);

  public:
    virtual void setLightDirection(Sdk::Vector3D i_direction) = 0;
    virtual void setLightColor(const Sdk::Vector4D& i_color) = 0;
    virtual void setAmbientStrength(double i_strength) = 0;
  };

} // ns Dx

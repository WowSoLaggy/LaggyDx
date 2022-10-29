#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class ISimpleShader
  {
  public:
    static std::unique_ptr<ISimpleShader> create(
      IRenderDevice& i_renderDevice,
      const ICamera& i_camera,
      const IResourceController& i_resourceController);

  public:
    virtual ~ISimpleShader() = default;

    virtual void setLightDirection(Sdk::Vector3D i_direction) = 0;
    virtual void setLightColor(const Sdk::Vector4D& i_color) = 0;
    virtual void setAmbientStrength(double i_strength) = 0;

    virtual void draw(const IObject3& i_object) const = 0;
  };

} // ns Dx

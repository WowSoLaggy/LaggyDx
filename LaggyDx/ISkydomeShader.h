#pragma once

#include "Shader.h"
#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class ISkydomeShader : public Shader
  {
  public:
    static std::unique_ptr<ISkydomeShader> create(
      IRenderDevice& i_renderDevice,
      const ICamera& i_camera,
      const IResourceController& i_resourceController);

  public:
    using Shader::Shader;

    virtual void setSunDirection(Sdk::Vector3D i_sunDir) = 0;
    virtual void setSunRadiusInternal(float i_radius) = 0;
    virtual void setSunRadiusExternal(float i_radius) = 0;

    virtual void setGlobalTime(double i_time) = 0;

    virtual void setWindSpeed(double i_speed) = 0;
    virtual void setWindDirection(Sdk::Vector2D i_windDir) = 0;
  };

} // ns Dx

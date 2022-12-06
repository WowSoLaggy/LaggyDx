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

    virtual void setWindSpeed1(double i_speed) = 0;
    virtual void setWindSpeed2(double i_speed) = 0;
    virtual void setWindDirection1(Sdk::Vector2D i_windDir) = 0;
    virtual void setWindDirection2(Sdk::Vector2D i_windDir) = 0;
    virtual void setOvercast(double i_overcast) = 0;
    virtual void setCutoff(double i_cutoff) = 0;
  };

} // ns Dx

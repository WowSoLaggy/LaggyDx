#pragma once

#include "Shader3d.h"
#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class ISkydomeShader : public Shader3d
  {
  public:
    static std::unique_ptr<ISkydomeShader> create(const ICamera3& i_camera);

  public:
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

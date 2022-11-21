#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class IOceanShader
  {
  public:
    static std::unique_ptr<IOceanShader> create(
      IRenderDevice& i_renderDevice,
      const ICamera& i_camera,
      const IResourceController& i_resourceController);

  public:
    virtual ~IOceanShader() = default;

    virtual void setGlobalTime(double i_time) = 0;

    virtual void setWaterColor(const Sdk::Vector4D& i_color) = 0;

    virtual void setWindDirection(int i_waveIndex, Sdk::Vector2D i_direction) = 0;
    virtual void setWavesSteepness(int i_waveIndex, double i_steepness) = 0;
    virtual void setWavesLength(int i_waveIndex, double i_length) = 0;

    virtual void setLightDirection(Sdk::Vector3D i_direction) = 0;
    virtual void setLightColor(const Sdk::Vector4D& i_color) = 0;
    virtual void setAmbientStrength(double i_strength) = 0;

    virtual void setFillMode(bool i_solid) = 0;

    virtual void draw(const IObject3& i_object) const = 0;
  };

} // ns Dx

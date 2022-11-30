#pragma once

#include "Shader.h"
#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class IOceanShader : public Shader
  {
  public:
    static std::unique_ptr<IOceanShader> create(
      IRenderDevice& i_renderDevice,
      const ICamera& i_camera,
      const IResourceController& i_resourceController);

  public:
    using Shader::Shader;

    virtual void setGlobalTime(double i_time) = 0;

    virtual void setWindDirection(int i_waveIndex, Sdk::Vector2D i_direction) = 0;
    virtual void setWavesSteepness(int i_waveIndex, double i_steepness) = 0;
    virtual void setWavesLength(int i_waveIndex, double i_length) = 0;

    virtual void setLightDirection(Sdk::Vector3D i_direction) = 0;
    virtual void setLightColor(const Sdk::Vector4D& i_color) = 0;
    virtual void setAmbientStrength(double i_strength) = 0;

    virtual void setTexturesDisplacementSettings(
      double i_scale1, double i_scale2,
      const Sdk::Vector2D& i_speed1, const Sdk::Vector2D& i_speed2) = 0;
  };

} // ns Dx

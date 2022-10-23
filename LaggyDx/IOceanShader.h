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
    virtual void setTextureCoef(double i_coef) = 0;

    virtual void setWindDirection(int i_waveIndex, Sdk::Vector2D i_direction) = 0;
    virtual void setWavesSteepness(int i_waveIndex, double i_steepness) = 0;
    virtual void setWavesLength(int i_waveIndex, double i_length) = 0;
    
    virtual void draw(const IObject3& i_object) const = 0;
  };

} // ns Dx

#pragma once

#include "CBuffer.h"
#include "ISkydomeShader.h"
#include "ShaderBuffers.h"


namespace Dx
{
  struct SkydomeSettings
  {
    XMFLOAT3 cameraPosition{ 0, 0, 0 };
    float sunRadiusInternal{ 0 };
    XMFLOAT3 sunDirection{ 0, 0, 0 };
    float sunRadiusExternal{ 0 };
  };


  class SkydomeShader : public ISkydomeShader
  {
  public:
    SkydomeShader(
      IRenderDevice& i_renderDevice,
      const ICamera& i_camera,
      const IResourceController& i_resourceController);

    virtual void setSunDirection(Sdk::Vector3D i_sunDir) override;
    virtual void setSunRadiusInternal(float i_radius) override;
    virtual void setSunRadiusExternal(float i_radius) override;

    virtual void setGlobalTime(double i_time) override;

    virtual void setWindSpeed1(double i_speed) override;
    virtual void setWindSpeed2(double i_speed) override;
    virtual void setWindDirection1(Sdk::Vector2D i_windDir) override;
    virtual void setWindDirection2(Sdk::Vector2D i_windDir) override;
    virtual void setOvercast(double i_overcast) override;

    virtual void draw(const IObject3& i_object) const override;

  private:
    const IResourceController& d_resourceController;
    const ICamera& d_camera;

    const ITexture& d_mainTexture;
    const ITexture& d_horizonHazeTexture;
    const ITexture& d_aroundSunTexture;
    const ITexture& d_cloudsTexture1;
    const ITexture& d_cloudsTexture2;

    SkydomeSettings d_skyDomeSettings;
    TimeDesc d_timeDesc;
    WindDesc d_windDesc;

    CBuffer d_matrixBuffer;
    CBuffer d_skyDomeBuffer;
    CBuffer d_timeBuffer;
    CBuffer d_windBuffer;

    virtual void setRenderStates() const override;
    void setGeometryBuffers(const IMesh& i_mesh) const;
    void setXfmMatrices(const IObject3& i_object) const;
    void setCBuffers() const;
    void setTextures() const;
  };

} // ns Dx

#pragma once

#include "CBuffer.h"
#include "IOceanShader.h"
#include "ShaderBuffers.h"


namespace Dx
{
  class OceanShader : public IOceanShader
  {
  public:
    OceanShader(const ICamera& i_camera);

    virtual void setGlobalTime(double i_time) override;

    virtual void setWindDirection(int i_waveIndex, Sdk::Vector2D i_direction) override;
    virtual void setWavesSteepness(int i_waveIndex, double i_steepness) override;
    virtual void setWavesLength(int i_waveIndex, double i_length) override;

    virtual void setLightDirection(Sdk::Vector3D i_direction) override;
    virtual void setLightColor(const Sdk::Vector4D& i_color) override;
    virtual void setAmbientStrength(double i_strength) override;

    virtual void setFogDepthStart(double i_depthStart) override;
    virtual void setFogDepthEnd(double i_depthEnd) override;
    virtual void setFogMinPower(double i_minPower) override;
    virtual void setFogMaxPower(double i_maxPower) override;

    virtual void setTexturesDisplacementSettings(
      double i_scale1, double i_scale2,
      const Sdk::Vector2D& i_speed1, const Sdk::Vector2D& i_speed2) override;

    virtual void draw(const IObject3& i_object) const override;

  private:
    TimeDesc d_timeDesc;
    WaveDesc d_waveDesc;
    LightDesc d_lightDesc;
    DepthFogDesc d_depthFogDesc;
    TextureDisplacementDesc d_texturesDisplacementDesc;

    const ICamera& d_camera;
    const ITexture& d_emptyTexture;
    const ITexture& d_bumpTexture;

    CBuffer d_matrixBuffer;
    CBuffer d_cameraBuffer;
    CBuffer d_lightBuffer;
    CBuffer d_depthFogBuffer;
    CBuffer d_viewportBuffer;
    CBuffer d_timeBuffer;
    CBuffer d_waveBuffer;
    CBuffer d_texturesDisplacementBuffer;

    void setGeometryBuffers(const IMesh& i_mesh) const;
    void setXfmMatrices(const IObject3& i_object) const;
    void setCBuffers() const;
    void setCommonTextures() const;
    void setTexture(const IObject3& i_object) const;
    void setTexture(const Material& i_material) const;
    void setMaterial(const Material& i_material) const;

    XMFLOAT4& getWaveByIndex(int i_waveIndex);
  };

} // ns Dx

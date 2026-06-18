#pragma once

#include "CBuffer.h"
#include "ITerrainShader.h"
#include "ShaderBuffers.h"


namespace Dx
{
  class TerrainShader : public ITerrainShader
  {
  public:
    TerrainShader(const ICamera3& i_camera);

    virtual void setLightDirection(Sdk::Vector3D i_direction) override;
    virtual void setLightColor(const Sdk::Vector4D& i_color) override;
    virtual void setAmbientStrength(double i_strength) override;

    virtual void draw(const IObject3& i_object) const override;

  private:
    const ICamera3& d_camera;

    // Four terrain layers blended by height + slope in the pixel shader,
    // bound to registers t0..t3 respectively.
    const ITexture& d_sandTexture;
    const ITexture& d_grassTexture;
    const ITexture& d_rockTexture;
    const ITexture& d_cliffTexture;

    LightDesc d_lightDesc;

    CBuffer d_matrixBuffer;
    CBuffer d_cameraBuffer;
    CBuffer d_lightBuffer;

    void setGeometryBuffers(const IMesh& i_mesh) const;
    void setXfmMatrices(const IObject3& i_object) const;
    void setCBuffers() const;
    void setTextures() const;
    void setMaterial(const Material& i_material) const;
  };

} // ns Dx

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

    virtual void setShadowMap(const ITexture& i_shadowMap) override;
    virtual void setShadowCamera(const ShadowCamera& i_shadowCamera) override;

    virtual void setGridCellSize(double i_cellSize) override;

    virtual void draw(const IObject3& i_object) const override;

  private:
    const ICamera3& d_camera;

    // Three terrain layers blended by the shore mask + slope in the pixel shader,
    // bound to registers t0..t2 respectively.
    const ITexture& d_sandTexture;
    const ITexture& d_grassTexture;
    const ITexture& d_cliffTexture;

    // Shadow map at t3; defaults to white.png so receivers stay lit until one is set
    const ITexture* d_shadowMapTexture;
    const ShadowCamera* d_shadowCamera = nullptr;

    LightDesc d_lightDesc;
    float d_gridCellSize = 0.0f;

    CBuffer d_matrixBuffer;
    CBuffer d_cameraBuffer;
    CBuffer d_lightBuffer;
    CBuffer d_shadowMatrixBuffer;
    CBuffer d_gridBuffer;

    void setGeometryBuffers(const IMesh& i_mesh) const;
    void setXfmMatrices(const IObject3& i_object) const;
    void setCBuffers() const;
    void setShadowCBuffer() const;
    void setGridCBuffer() const;
    void setTextures() const;
    void setMaterial(const Material& i_material) const;
  };

} // ns Dx

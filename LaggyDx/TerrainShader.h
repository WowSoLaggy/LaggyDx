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

    virtual void setShadowCascade(int i_cascade, const ShadowCamera& i_camera, const ITexture& i_map) override;

    virtual void setGridCellSize(double i_cellSize) override;
    virtual void setForestMask(const ITexture& i_mask, const Sdk::Vector2D& i_mapWorldSize) override;

    virtual void draw(const IObject3& i_object) const override;

  private:
    const ICamera3& d_camera;

    // Five terrain layers blended by the shore mask + slope + dry-patch noise + forest mask in
    // the pixel shader: sand/grass/cliff at t0..t2, dirt at t4, forest bed at t7 (t3, t5, t6 are the shadow cascades).
    const ITexture& d_sandTexture;
    const ITexture& d_grassTexture;
    const ITexture& d_cliffTexture;
    const ITexture& d_dirtTexture;
    const ITexture& d_forestBedTexture;

    // Cascade maps at t3, t5, t6; default to white.png so receivers stay lit until wired
    const ITexture* d_shadowMapTextures[c_shadowCascadesCount];
    const ShadowCamera* d_shadowCameras[c_shadowCascadesCount] = {};

    // Tree-cover mask at t8; ignored by the PS until invMapSize is set from setForestMask
    const ITexture* d_forestMaskTexture = nullptr;

    LightDesc d_lightDesc;
    float d_gridCellSize = 0.0f;
    XMFLOAT2 d_invMapSize = { 0.0f, 0.0f };

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

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
    virtual void setPaintLayers(const std::vector<TerrainPaintLayer>& i_layers,
      const std::vector<const ITexture*>& i_masks, const Sdk::Vector2D& i_mapWorldSize) override;

    virtual void draw(const IObject3& i_object) const override;

  private:
    const ICamera3& d_camera;

    // Grass base at t0; every other ground layer comes in via setPaintLayers
    const ITexture& d_grassTexture;

    // Fallback for unset paint/mask slots; the enabled flag mutes their contribution
    const ITexture& d_defaultTexture;

    // Cascade maps at t1..t3; default to white.png so receivers stay lit until wired
    const ITexture* d_shadowMapTextures[c_shadowCascadesCount];
    const ShadowCamera* d_shadowCameras[c_shadowCascadesCount] = {};

    // Paint textures at t4..t11, their RGBA masks at t12..t13; layer i reads channel i%4 of mask i/4
    TerrainPaintLayer d_paintLayers[c_maxTerrainPaintLayers];
    const ITexture* d_paintMaskTextures[c_terrainPaintMasksCount] = {};

    LightDesc d_lightDesc;
    float d_gridCellSize = 0.0f;
    XMFLOAT2 d_invMapSize = { 0.0f, 0.0f };

    CBuffer d_matrixBuffer;
    CBuffer d_cameraBuffer;
    CBuffer d_lightBuffer;
    CBuffer d_shadowMatrixBuffer;
    CBuffer d_gridBuffer;
    CBuffer d_paintBuffer;

    void setGeometryBuffers(const IMesh& i_mesh) const;
    void setXfmMatrices(const IObject3& i_object) const;
    void setCBuffers() const;
    void setShadowCBuffer() const;
    void setGridCBuffer() const;
    void setPaintCBuffer() const;
    void setTextures() const;
    void setMaterial(const Material& i_material) const;
  };

} // ns Dx

#pragma once

#include "Shader3d.h"
#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class ITerrainShader : public Shader3d
  {
  public:
    static std::shared_ptr<ITerrainShader> create(const ICamera3& i_camera);

  public:
    virtual void setLightDirection(Sdk::Vector3D i_direction) = 0;
    virtual void setLightColor(const Sdk::Vector4D& i_color) = 0;
    virtual void setAmbientStrength(double i_strength) = 0;

    // Wires one shadow cascade (0 = nearest); the PS samples the first cascade containing the pixel
    virtual void setShadowCascade(int i_cascade, const ShadowCamera& i_camera, const ITexture& i_map) = 0;

    // World-space grid overlay with the given cell size; 0 or less hides it
    virtual void setGridCellSize(double i_cellSize) = 0;

    // Map-covering tree-cover mask; forest_bed.png is blended over grass by its red channel
    virtual void setForestMask(const ITexture& i_mask, const Sdk::Vector2D& i_mapWorldSize) = 0;
  };

} // ns Dx

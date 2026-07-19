#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  // One externally-authored terrain paint layer, blended over the grass base by its baked mask channel
  struct TerrainPaintLayer
  {
    const ITexture* texture = nullptr;      // paint albedo; the caller keeps it alive
    double tile = 8.0;                      // world units per texture repeat
    double tileCoarse = 0.0;                // second anti-tiling scale; 0 disables the two-scale mix
    double coarseMix = 0.0;                 // 0 = detail only, 1 = coarse only
    Sdk::Vector3D tint = { 1.0, 1.0, 1.0 }; // albedo multiplier
    bool triplanar = false;                 // sample triplanar (for steep faces) instead of planar XZ
  };

} // ns Dx

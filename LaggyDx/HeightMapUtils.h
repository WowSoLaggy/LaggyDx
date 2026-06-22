#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/SdkFwd.h>
#include <LaggySdk/Vector.h>

#include <optional>


namespace Dx
{
  /// Ray-marches the camera's view ray (through i_screenPoint) against the real
  /// heightfield and returns the first (nearest) world-space point where it crosses
  /// the surface, or std::nullopt if the ray misses the terrain. The march is
  /// clipped to the terrain's 3D AABB first, so distant or near-parallel rays only
  /// walk the slab the terrain can actually occupy.
  std::optional<Sdk::Vector3F> pickPoint(
    const HeightMap& i_heightMap,
    const ICamera3& i_camera,
    const Sdk::Vector2I& i_screenPoint);

} // ns Dx

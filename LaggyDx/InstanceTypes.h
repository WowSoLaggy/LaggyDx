#pragma once

#include <LaggySdk/Vector.h>


namespace Dx
{
  // Per-instance data for instanced rendering; layout matches the per-instance elements of the instanced vertex layout.
  struct InstancePosRotScale
  {
    Sdk::Vector3F position;
    float rotationY = 0.0f;
    float scale = 1.0f;
  };

} // ns Dx

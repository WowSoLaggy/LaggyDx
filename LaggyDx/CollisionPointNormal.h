#pragma once

#include <LaggySdk/Vector.h>


namespace Dx
{
  struct CollisionPointNormal
  {
    Sdk::Vector2D point;
    Sdk::Vector2D normal;

    CollisionPointNormal inverse()
    {
      return { point, -normal };
    }
  };

} // ns Dx

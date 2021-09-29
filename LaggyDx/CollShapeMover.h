#pragma once

#include "CollisionShape.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class CollShapeMover
  {
  public:
    CollShapeMover(Sdk::Vector2D i_parentObjectPosition);

    CollisionShape operator()(const Sdk::Vector2D& i_point);
    CollisionShape operator()(const Sdk::CircleD& i_circle);
    CollisionShape operator()(const Sdk::RectD& i_rect);

  private:
    Sdk::Vector2D d_parentObjectPosition;
  };

} // Dx

#include "stdafx.h"
#include "CollShapeMover.h"

namespace Dx
{
  CollShapeMover::CollShapeMover(Sdk::Vector2D i_parentObjectPosition)
    : d_parentObjectPosition(i_parentObjectPosition)
  {
  }

  CollisionShape CollShapeMover::operator()(const Sdk::Vector2D& i_point)
  {
    return i_point + d_parentObjectPosition;
  }

  CollisionShape CollShapeMover::operator()(const Sdk::CircleD& i_circle)
  {
    auto posCircle = i_circle;
    posCircle.setCenter(d_parentObjectPosition);
    return posCircle;
  }

  CollisionShape CollShapeMover::operator()(const Sdk::RectD& i_rect)
  {
    auto posRect = i_rect;
    posRect.move(d_parentObjectPosition);
    return posRect;
  }

} // ns Dx

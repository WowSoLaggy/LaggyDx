#pragma once

#include "CollisionPointNormal.h"

#include <LaggySdk/Circle.h>
#include <LaggySdk/Rect.h>
#include <LaggySdk/Vector.h>

namespace Dx
{
  class CollisionFunctor
  {
  public:
    std::optional<CollisionPointNormal> operator()(
      const Sdk::Vector2D& i_point1, const Sdk::Vector2D& i_point2) const;
    std::optional<CollisionPointNormal> operator()(
      const Sdk::Vector2D& i_point, const Sdk::CircleD& i_circle) const;
    std::optional<CollisionPointNormal> operator()(
      const Sdk::Vector2D& i_point, const Sdk::RectD& i_rect) const;

    std::optional<CollisionPointNormal> operator()(
      const Sdk::CircleD& i_circle, const Sdk::Vector2D& i_point) const;
    std::optional<CollisionPointNormal> operator()(
      const Sdk::CircleD& i_circle1, const Sdk::CircleD& i_circle2) const;
    std::optional<CollisionPointNormal> operator()(
      const Sdk::CircleD& i_circle, const Sdk::RectD& i_rect) const;

    std::optional<CollisionPointNormal> operator()(
      const Sdk::RectD& i_rect, const Sdk::Vector2D& i_point) const;
    std::optional<CollisionPointNormal> operator()(
      const Sdk::RectD& i_rect, const Sdk::CircleD& i_circle) const;
    std::optional<CollisionPointNormal> operator()(
      const Sdk::RectD& i_rect1, const Sdk::RectD& i_rect2) const;
  };

} // ns Dx

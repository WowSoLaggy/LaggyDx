#include "stdafx.h"
#include "Collider.h"

#include <LaggySdk/Math.h>


namespace Dx
{
  std::optional<CollisionPointNormal> Collider::operator()(
    const Sdk::Vector2D& i_point1, const Sdk::Vector2D& i_point2) const
  {
    if (i_point1 == i_point2)
      return CollisionPointNormal{ i_point1, Sdk::Vector2D::zero() };

    return std::nullopt;
  }

  std::optional<CollisionPointNormal> Collider::operator()(
    const Sdk::Vector2D& i_point, const Sdk::CircleD& i_circle) const
  {
    const auto dist = i_point - i_circle.getCenter();
    if (dist.lengthSq() <= i_circle.getRadiusSq())
      return CollisionPointNormal{ i_point, Sdk::normalize(dist) };

    return std::nullopt;
  }

  std::optional<CollisionPointNormal> Collider::operator()(
    const Sdk::Vector2D& i_point, const Sdk::RectD& i_rect) const
  {
    throw std::runtime_error("Not implemented yet");
  }


  std::optional<CollisionPointNormal> Collider::operator()(
    const Sdk::CircleD& i_circle, const Sdk::Vector2D& i_point) const
  {
    if (auto collisionNormalPointOpt = operator()(i_point, i_circle))
      return collisionNormalPointOpt->inverse();

    return std::nullopt;
  }

  std::optional<CollisionPointNormal> Collider::operator()(
    const Sdk::CircleD& i_circle1, const Sdk::CircleD& i_circle2) const
  {
    const auto dist = i_circle1.getCenter() - i_circle2.getCenter();
    if (dist.length() <= i_circle1.getRadius() + i_circle2.getRadius())
    {
      const auto point = i_circle2.getCenter() + dist / 2;
      const auto normal = Sdk::normalize(dist);
      return CollisionPointNormal{ point, normal };
    }

    return std::nullopt;
  }

  std::optional<CollisionPointNormal> Collider::operator()(
    const Sdk::CircleD& i_circle, const Sdk::RectD& i_rect) const
  {
    auto dist = i_circle.getCenter() - i_rect.center();
    dist.x = Sdk::clamp(dist.x, -i_rect.width2(), i_rect.width2());
    dist.y = Sdk::clamp(dist.y, -i_rect.height2(), i_rect.height2());

    const auto closestPoint = i_rect.center() + dist;
    const auto distToPoint = closestPoint - i_circle.getCenter();

    if (distToPoint.lengthSq() <= i_circle.getRadiusSq())
    {
      const auto normal = closestPoint - i_circle.getCenter();
      return CollisionPointNormal{ closestPoint, Sdk::normalize(normal) };
    }

    return std::nullopt;
  }


  std::optional<CollisionPointNormal> Collider::operator()(
    const Sdk::RectD& i_rect, const Sdk::Vector2D& i_point) const
  {
    if (auto collisionNormalPointOpt = operator()(i_point, i_rect))
      return collisionNormalPointOpt->inverse();

    return std::nullopt;
  }

  std::optional<CollisionPointNormal> Collider::operator()(
    const Sdk::RectD& i_rect, const Sdk::CircleD& i_circle) const
  {
    if (auto collisionNormalPointOpt = operator()(i_circle, i_rect))
      return collisionNormalPointOpt->inverse();

    return std::nullopt;
  }

  std::optional<CollisionPointNormal> Collider::operator()(
    const Sdk::RectD& i_rect1, const Sdk::RectD& i_rect2) const
  {
    if (!i_rect1.intersectRect(i_rect2))
      return std::nullopt;

    //return CollisionPointNormal{};
    throw std::runtime_error("Not implemented yet");
  }

} // ns Dx

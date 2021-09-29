#pragma once

#include <LaggySdk/Circle.h>
#include <LaggySdk/Rect.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  using CollisionShape = std::variant<Sdk::Vector2D, Sdk::CircleD, Sdk::RectD>;

} // ns Dx

#pragma once

#include <LaggySdk/Vector.h>


namespace Dx
{
  struct VertexTypePosTexNorm
  {
    Sdk::Vector3F position;
    Sdk::Vector2F texture;
    Sdk::Vector3F normal;
  };

} // ns Dx

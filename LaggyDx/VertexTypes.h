#pragma once

#include <LaggySdk/Vector.h>


namespace Dx
{
  struct VertexTypePosTexNorm
  {
    Sdk::Vector3 position;
    Sdk::Vector2 texture;
    Sdk::Vector3 normal;
  };

} // ns Dx

#pragma once

#include <LaggySdk/Vector.h>


namespace Dx
{
  struct VertexPosNormText
  {
    static VertexPosNormText pos(Sdk::Vector3F i_pos);

    Sdk::Vector3F position;
    Sdk::Vector3F normal;
    Sdk::Vector2F texture;
  };

} // ns Dx

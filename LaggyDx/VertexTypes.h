#pragma once

#include <LaggySdk/Vector.h>


namespace Dx
{
  struct VertexPos3NormText
  {
    static VertexPos3NormText pos(Sdk::Vector3F i_pos);

    Sdk::Vector3F position;
    Sdk::Vector3F normal;
    Sdk::Vector2F texture;
  };

  struct VertexPos2Text
  {
    static VertexPos2Text posTex(Sdk::Vector2F i_position, Sdk::Vector2F i_texture);

    Sdk::Vector2F position;
    Sdk::Vector2F texture;
  };

} // ns Dx

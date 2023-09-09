#include "stdafx.h"
#include "VertexTypes.h"


namespace Dx
{
  VertexPos3NormText VertexPos3NormText::pos(Sdk::Vector3F i_pos)
  {
    VertexPos3NormText v;
    v.position = std::move(i_pos);
    return v;
  }


  VertexPos2Text VertexPos2Text::posTex(Sdk::Vector2F i_position, Sdk::Vector2F i_texture)
  {
    VertexPos2Text v;
    v.position = std::move(i_position);
    v.texture = std::move(i_texture);
    return v;
  }

} // ns Dx

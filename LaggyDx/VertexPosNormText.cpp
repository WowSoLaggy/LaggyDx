#include "stdafx.h"
#include "VertexPosNormText.h"


namespace Dx
{
  VertexPosNormText VertexPosNormText::pos(Sdk::Vector3F i_pos)
  {
    VertexPosNormText p;
    p.position = std::move(i_pos);
    return p;
  }

} // ns Dx

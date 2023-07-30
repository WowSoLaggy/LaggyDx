#include "stdafx.h"
#include "VertexTypes.h"


namespace Dx
{
  VertexPos3NormText VertexPos3NormText::pos(Sdk::Vector3F i_pos)
  {
    VertexPos3NormText p;
    p.position = std::move(i_pos);
    return p;
  }

} // ns Dx

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

} // ns Dx

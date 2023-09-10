#include "stdafx.h"
#include "Camera2.h"


namespace Dx
{
  const Sdk::Vector2I& Camera2::getOffset() const
  {
    return d_offset;
  }

  void Camera2::setOffset(Sdk::Vector2I i_offset)
  {
    d_offset = std::move(i_offset);
  }

} // ns Dx

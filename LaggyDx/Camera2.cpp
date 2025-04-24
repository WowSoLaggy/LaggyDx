#include "stdafx.h"
#include "Camera2.h"


namespace Dx
{
  const Sdk::Vector2F& Camera2::getOffset() const
  {
    return d_offset;
  }

  void Camera2::setOffset(Sdk::Vector2F i_offset)
  {
    d_offset = std::move(i_offset);
  }

} // ns Dx

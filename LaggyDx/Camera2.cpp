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

  void Camera2::move(const Sdk::Vector2F& i_offset)
  {
    d_offset += i_offset;
  }


  float Camera2::getZoom() const
  {
    return d_zoom;
  }

  void Camera2::setZoom(const float i_zoom)
  {
    CONTRACT_EXPECT(i_zoom > 0);
    d_zoom = i_zoom;
  }

} // ns Dx

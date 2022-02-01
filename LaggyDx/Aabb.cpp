#include "stdafx.h"
#include "Aabb.h"


namespace Dx
{
  Aabb::Aabb()
    : Aabb(0, 0, 0, 0, 0, 0)
  {
  }

  Aabb::Aabb(
    const float i_x0, const float i_x1,
    const float i_y0, const float i_y1,
    const float i_z0, const float i_z1)
  {
    d_xMin = std::min(i_x0, i_x1);
    d_xMax = std::max(i_x0, i_x1);
    d_yMin = std::min(i_y0, i_y1);
    d_yMax = std::max(i_y0, i_y1);
    d_zMin = std::min(i_z0, i_z1);
    d_zMax = std::max(i_z0, i_z1);
  }


  void Aabb::mergeWith(const Aabb& i_other)
  {
    d_xMin = std::min(d_xMin, i_other.d_xMin);
    d_xMax = std::max(d_xMax, i_other.d_xMax);
    d_yMin = std::min(d_yMin, i_other.d_yMin);
    d_yMax = std::max(d_yMax, i_other.d_yMax);
    d_zMin = std::min(d_zMin, i_other.d_zMin);
    d_zMax = std::max(d_zMax, i_other.d_zMax);
  }


  float Aabb::getMinX() const { return d_xMin; }
  float Aabb::getMaxX() const { return d_xMax; }
  float Aabb::getMinY() const { return d_yMin; }
  float Aabb::getMaxY() const { return d_yMax; }
  float Aabb::getMinZ() const { return d_zMin; }
  float Aabb::getMaxZ() const { return d_zMax; }

} // ns Dx

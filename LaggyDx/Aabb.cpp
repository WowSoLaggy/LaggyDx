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


  float Aabb::getMinX() const { return d_xMin; }
  float Aabb::getMaxX() const { return d_xMax; }
  float Aabb::getMinY() const { return d_yMin; }
  float Aabb::getMaxY() const { return d_yMax; }
  float Aabb::getMinZ() const { return d_zMin; }
  float Aabb::getMaxZ() const { return d_zMax; }


  void Aabb::mergeWith(const Aabb& i_other)
  {
    d_xMin = std::min(d_xMin, i_other.d_xMin);
    d_xMax = std::max(d_xMax, i_other.d_xMax);
    d_yMin = std::min(d_yMin, i_other.d_yMin);
    d_yMax = std::max(d_yMax, i_other.d_yMax);
    d_zMin = std::min(d_zMin, i_other.d_zMin);
    d_zMax = std::max(d_zMax, i_other.d_zMax);
  }

  std::optional<double> Aabb::intersect(const Sdk::RayF& i_ray) const
  {
    double tx1 = (d_xMin - i_ray.getPoint().x) * i_ray.getDirInv().x;
    double tx2 = (d_xMax - i_ray.getPoint().x) * i_ray.getDirInv().x;

    double tmin = std::min(tx1, tx2);
    double tmax = std::max(tx1, tx2);

    double ty1 = (d_yMin - i_ray.getPoint().y) * i_ray.getDirInv().y;
    double ty2 = (d_yMax - i_ray.getPoint().y) * i_ray.getDirInv().y;

    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));

    double tz1 = (d_zMin - i_ray.getPoint().z) * i_ray.getDirInv().z;
    double tz2 = (d_zMax - i_ray.getPoint().z) * i_ray.getDirInv().z;

    tmin = std::max(tmin, std::min(tz1, tz2));
    tmax = std::min(tmax, std::max(tz1, tz2));

    if (tmax >= tmin)
      return tmin;
    else
      return std::nullopt;
  }

  void Aabb::translate(const Sdk::Vector3F& i_translation)
  {
    d_xMin += i_translation.x;
    d_xMax += i_translation.x;

    d_yMin += i_translation.y;
    d_yMax += i_translation.y;

    d_zMin += i_translation.z;
    d_zMax += i_translation.z;
  }

} // ns Dx

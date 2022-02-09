#pragma once

#include <LaggySdk/Ray.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  class Aabb
  {
  public:
    Aabb();
    Aabb(
      float i_x0, float i_x1,
      float i_y0, float i_y1,
      float i_z0, float i_z1);

    float getMinX() const;
    float getMaxX() const;
    float getMinY() const;
    float getMaxY() const;
    float getMinZ() const;
    float getMaxZ() const;

    void mergeWith(const Aabb& i_other);
    bool intersect(const Sdk::RayF& i_ray) const;
    void translate(const Sdk::Vector3F& i_translation);

  private:
    float d_xMin = 0.0f;
    float d_xMax = 0.0f;
    float d_yMin = 0.0f;
    float d_yMax = 0.0f;
    float d_zMin = 0.0f;
    float d_zMax = 0.0f;
  };

} // ns Dx

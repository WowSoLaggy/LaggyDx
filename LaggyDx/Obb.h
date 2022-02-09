#pragma once

#include "Aabb.h"

#include <LaggySdk/Ray.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  class Obb
  {
  public:
    Obb();
    Obb(Aabb i_aabb, Sdk::Vector3F i_translation, Sdk::Vector3F i_rotation);

    const Aabb& getAabb() const;
    const Sdk::Vector3F& getRotation() const;

    void setAabb(Aabb i_aabb);
    void setRotation(Sdk::Vector3F i_rotation);

    bool intersect(Sdk::RayF i_ray) const;

  private:
    Aabb d_aabb;
    Sdk::Vector3F d_rotation;
    Sdk::Vector3F d_translation;
  };

} // ns Dx

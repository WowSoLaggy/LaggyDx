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
    Obb(Aabb i_aabb, Sdk::Vector3F i_translation = {}, Sdk::Vector3F i_rotation = {});

    const Aabb& getAabb() const;
    const Sdk::Vector3F& getTranslation() const;
    const Sdk::Vector3F& getRotation() const;

    // World-space centre of the box (translation plus the rotated AABB midpoint).
    Sdk::Vector3F getCenter() const;

    void setAabb(Aabb i_aabb);
    void setTranslation(Sdk::Vector3F i_translation);
    void setRotation(Sdk::Vector3F i_rotation);

    // Places the box so its world-space centre lands at i_center, keeping the current rotation.
    void setCenter(const Sdk::Vector3F& i_center);

    std::optional<double> intersect(Sdk::RayF i_ray) const;
    bool intersect(const Obb& i_other) const;

  private:
    Aabb d_aabb;
    Sdk::Vector3F d_rotation;
    Sdk::Vector3F d_translation;
  };

} // ns Dx

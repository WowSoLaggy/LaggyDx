#include "stdafx.h"
#include "Obb.h"


namespace Dx
{
  namespace
  {
    // Rotates a ray into the box's local frame. Handles yaw (Y) and roll (Z):
    // roads lie in the XZ plane and are oriented by their Y rotation, while the
    // original 2D-sprite users only set Z. Pitch (X) is not modeled.
    Sdk::RayF rotateRay(const Sdk::RayF& i_ray, float i_yRotation, float i_zRotation)
    {
      auto getVector = [](XMFLOAT3 i_vector)
      {
        return XMLoadFloat3(&i_vector);
      };
      static const auto yVector = getVector({ 0, 1, 0 });
      static const auto zVector = getVector({ 0, 0, 1 });

      const auto qRotation = XMQuaternionMultiply(
        XMQuaternionRotationAxis(zVector, i_zRotation),
        XMQuaternionRotationAxis(yVector, i_yRotation));

      XMFLOAT3 point{ i_ray.getPoint().x, i_ray.getPoint().y, i_ray.getPoint().z };
      XMFLOAT3 dir{ i_ray.getDir().x, i_ray.getDir().y, i_ray.getDir().z };
      XMStoreFloat3(&point, XMVector3Rotate(XMLoadFloat3(&point), qRotation));
      XMStoreFloat3(&dir, XMVector3Rotate(XMLoadFloat3(&dir), qRotation));

      return Sdk::RayF::createFromPointAndDir({ point.x, point.y, point.z }, { dir.x, dir.y, dir.z });
    }


    // The box's world-space frame: its centre, the three orthonormal axes (the
    // local X/Y/Z directions rotated into world space), and the half-extents
    // along each. Mirrors the rotation convention used by rotateRay / intersect:
    // local -> world rotates by Y (yaw) then Z (roll); X (pitch) is not modeled.
    struct BoxFrame
    {
      Sdk::Vector3F center;
      Sdk::Vector3F axis[3];   // world-space orientation axes (unit length)
      float halfExtent[3];     // half-size along each axis
    };

    // local -> world rotation (inverse of the world -> local rotation in rotateRay): yaw (Y) then roll (Z).
    Sdk::Vector3F rotateLocalToWorld(const Sdk::Vector3F& i_v, const Sdk::Vector3F& i_rotation)
    {
      static const XMVECTOR yVector = XMVectorSet(0, 1, 0, 0);
      static const XMVECTOR zVector = XMVectorSet(0, 0, 1, 0);

      const XMVECTOR qRotation = XMQuaternionMultiply(
        XMQuaternionRotationAxis(yVector, i_rotation.y),
        XMQuaternionRotationAxis(zVector, i_rotation.z));

      XMFLOAT3 v{ i_v.x, i_v.y, i_v.z };
      XMStoreFloat3(&v, XMVector3Rotate(XMLoadFloat3(&v), qRotation));
      return Sdk::Vector3F{ v.x, v.y, v.z };
    }

    // The local box may be off-origin: its centre is the midpoint of its bounds.
    Sdk::Vector3F localCenter(const Aabb& i_aabb)
    {
      return {
        (i_aabb.getMinX() + i_aabb.getMaxX()) * 0.5f,
        (i_aabb.getMinY() + i_aabb.getMaxY()) * 0.5f,
        (i_aabb.getMinZ() + i_aabb.getMaxZ()) * 0.5f };
    }

    BoxFrame makeFrame(const Aabb& i_aabb, const Sdk::Vector3F& i_translation, const Sdk::Vector3F& i_rotation)
    {
      auto rotate = [&](const Sdk::Vector3F& i_v)
      {
        return rotateLocalToWorld(i_v, i_rotation);
      };

      BoxFrame frame;
      frame.center = i_translation + rotate(localCenter(i_aabb));
      frame.axis[0] = rotate({ 1, 0, 0 });
      frame.axis[1] = rotate({ 0, 1, 0 });
      frame.axis[2] = rotate({ 0, 0, 1 });
      frame.halfExtent[0] = i_aabb.getLengthX() * 0.5f;
      frame.halfExtent[1] = i_aabb.getLengthY() * 0.5f;
      frame.halfExtent[2] = i_aabb.getLengthZ() * 0.5f;
      return frame;
    }

    // Separating Axis Theorem for two oriented boxes: they are disjoint iff some
    // axis separates them. It suffices to test each box's 3 face normals and the
    // 9 pairwise cross-products of their axes (15 axes total). Near-parallel edge
    // pairs yield a degenerate (~zero) cross product and are skipped.
    bool boxesOverlap(const BoxFrame& i_a, const BoxFrame& i_b)
    {
      constexpr float Epsilon = 1e-6f;

      const Sdk::Vector3F t = i_b.center - i_a.center;

      // Projection radius of a box onto an axis = sum of |halfExtent_i * (axis_i . n)|.
      auto separated = [&](const Sdk::Vector3F& i_axis)
      {
        if (i_axis.lengthSq() < Epsilon)
          return false; // degenerate axis carries no separation information

        const float dist = std::abs(t.dot(i_axis));

        float ra = 0.0f;
        float rb = 0.0f;
        for (int i = 0; i < 3; ++i)
        {
          ra += i_a.halfExtent[i] * std::abs(i_a.axis[i].dot(i_axis));
          rb += i_b.halfExtent[i] * std::abs(i_b.axis[i].dot(i_axis));
        }

        return dist > ra + rb;
      };

      // 3 + 3 face normals.
      for (int i = 0; i < 3; ++i)
      {
        if (separated(i_a.axis[i]) || separated(i_b.axis[i]))
          return false;
      }

      // 9 edge-edge cross products.
      for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
          if (separated(i_a.axis[i].cross(i_b.axis[j])))
            return false;
        }

      return true; // no separating axis found -> the boxes overlap
    }

  } // anon NS


  Obb::Obb()
  {
  }

  Obb::Obb(Aabb i_aabb, Sdk::Vector3F i_translation, Sdk::Vector3F i_rotation)
    : d_aabb(std::move(i_aabb))
    , d_translation(std::move(i_translation))
    , d_rotation(std::move(i_rotation))
  {
  }


  const Aabb& Obb::getAabb() const
  {
    return d_aabb;
  }

  const Sdk::Vector3F& Obb::getTranslation() const
  {
    return d_translation;
  }

  const Sdk::Vector3F& Obb::getRotation() const
  {
    return d_rotation;
  }

  Sdk::Vector3F Obb::getCenter() const
  {
    return d_translation + rotateLocalToWorld(localCenter(d_aabb), d_rotation);
  }


  void Obb::setAabb(Aabb i_aabb)
  {
    d_aabb = std::move(i_aabb);
  }

  void Obb::setTranslation(Sdk::Vector3F i_translation)
  {
    d_translation = std::move(i_translation);
  }

  void Obb::setRotation(Sdk::Vector3F i_rotation)
  {
    d_rotation = std::move(i_rotation);
  }

  void Obb::setCenter(const Sdk::Vector3F& i_center)
  {
    d_translation = i_center - rotateLocalToWorld(localCenter(d_aabb), d_rotation);
  }


  std::optional<double> Obb::intersect(Sdk::RayF i_ray) const
  {
    i_ray.translate(-d_translation);
    return d_aabb.intersect(rotateRay(i_ray, -d_rotation.y, -d_rotation.z));
  }

  bool Obb::intersect(const Obb& i_other) const
  {
    const auto a = makeFrame(d_aabb, d_translation, d_rotation);
    const auto b = makeFrame(i_other.d_aabb, i_other.d_translation, i_other.d_rotation);
    return boxesOverlap(a, b);
  }

} // ns Dx

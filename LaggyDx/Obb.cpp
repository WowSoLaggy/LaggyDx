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

  const Sdk::Vector3F& Obb::getRotation() const
  {
    return d_rotation;
  }


  void Obb::setAabb(Aabb i_aabb)
  {
    d_aabb = std::move(i_aabb);
  }

  void Obb::setRotation(Sdk::Vector3F i_rotation)
  {
    d_rotation = std::move(i_rotation);
  }


  std::optional<double> Obb::intersect(Sdk::RayF i_ray) const
  {
    i_ray.translate(-d_translation);
    return d_aabb.intersect(rotateRay(i_ray, -d_rotation.y, -d_rotation.z));
  }

} // ns Dx

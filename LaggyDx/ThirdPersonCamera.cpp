#include "stdafx.h"
#include "ThirdPersonCamera.h"

#include <LaggySdk/Math.h>


namespace Dx
{
  namespace
  {

    XMVECTOR toXmVector(const Sdk::Vector3F& i_vector)
    {
      return XMVectorSet(i_vector.x, i_vector.y, i_vector.z, 1);
    }

    XMVECTOR toXmVector(const Sdk::Vector2I& i_vector, const float i_z)
    {
      return XMVectorSet((float)i_vector.x, (float)i_vector.y, i_z, 1);
    }

    Sdk::Vector3F fromXmVector(const XMFLOAT3& i_vector)
    {
      return { i_vector.x, i_vector.y, i_vector.z };
    }

  } // anonymous NS


  ThirdPersonCamera::ThirdPersonCamera(Sdk::Vector2I i_viewportResolution)
    : d_yaw(0)
    , d_pitch(Sdk::degToRad(45.0f))
    , d_distance(10)
    , d_lookAt{ 0, 0, 0 }
    , d_up{ 0, 1, 0 }
    , d_viewportResolution(std::move(i_viewportResolution))
  {
    updateProjectionMatrix();
    updateViewMatrix();
  }


  void ThirdPersonCamera::setFovAngle(const float i_fovAngle)
  {
    d_fovAngle = i_fovAngle;
    updateProjectionMatrix();
  }

  void ThirdPersonCamera::setViewportMinZ(const float i_viewportMinZ)
  {
    d_viewportMinZ = i_viewportMinZ;
    updateProjectionMatrix();
  }

  void ThirdPersonCamera::setViewportMaxZ(const float i_viewportMaxZ)
  {
    d_viewportMaxZ = i_viewportMaxZ;
    updateProjectionMatrix();
  }


  void ThirdPersonCamera::setYaw(const float i_yaw)
  {
    d_yaw = i_yaw;
    updateViewMatrix();
  }

  void ThirdPersonCamera::setPitch(const float i_pitch)
  {
    d_pitch = i_pitch;
    updateViewMatrix();
  }

  void ThirdPersonCamera::setDistance(const float i_distance)
  {
    d_distance = i_distance;
    updateViewMatrix();
  }

  void ThirdPersonCamera::setLookAt(Sdk::Vector3F i_lookAt)
  {
    d_lookAt = std::move(i_lookAt);
    updateViewMatrix();
  }

  void ThirdPersonCamera::setUp(Sdk::Vector3F i_up)
  {
    i_up.normalize();
    d_up = std::move(i_up);
    updateViewMatrix();
  }


  Sdk::Vector3F ThirdPersonCamera::getPosition() const
  {
    auto getVector = [](XMFLOAT3 i_vector)
    {
      return XMLoadFloat3(&i_vector);
    };

    static const auto yVector = getVector({ 0, -1, 0 });
    static const auto zVector = getVector({ 0, 0, 1 });

    XMFLOAT3 unitVector{ 1, 0, 0 };

    const auto qRotationY = XMQuaternionRotationAxis(yVector, d_pitch);
    const auto qRotationZ = XMQuaternionRotationAxis(zVector, d_yaw);

    XMStoreFloat3(&unitVector, XMVector3Rotate(XMLoadFloat3(&unitVector), qRotationY));
    XMStoreFloat3(&unitVector, XMVector3Rotate(XMLoadFloat3(&unitVector), qRotationZ));

    Sdk::Vector3F relativePosition{ unitVector.x * d_distance, unitVector.y * d_distance, unitVector.z * d_distance };
    return d_lookAt + relativePosition;
  }

  void ThirdPersonCamera::setPosition(const Sdk::Vector3F& i_pos)
  {
    const auto diff = i_pos - getPosition();
    setLookAt(getLookAt() + diff);
  }


  Sdk::Vector3F ThirdPersonCamera::getLeft() const
  {
    auto left = getUp();
    left = left.cross(getForward());
    left.normalize();
    return left;
  }

  Sdk::Vector3F ThirdPersonCamera::getRight() const
  {
    return -getLeft();
  }

  Sdk::Vector3F ThirdPersonCamera::getForward() const
  {
    return getLookAt() - getPosition();
  }

  Sdk::Vector3F ThirdPersonCamera::getBackward() const
  {
    return -getForward();
  }


  void ThirdPersonCamera::updateProjectionMatrix()
  {
    const float screenAspect = (float)d_viewportResolution.x / (float)d_viewportResolution.y;
    d_projectionMatrix = XMMatrixPerspectiveFovRH(d_fovAngle, screenAspect,
      d_viewportMinZ, d_viewportMaxZ);
  }

  void ThirdPersonCamera::updateViewMatrix()
  {
    d_viewMatrix = XMMatrixLookAtRH(toXmVector(getPosition()), toXmVector(d_lookAt), toXmVector(d_up));
  }


  Sdk::Vector2F ThirdPersonCamera::worldToScreen(const Sdk::Vector3F& i_point) const
  {
    auto worldMatrix = XMMatrixIdentity();

    auto res = XMVector3Project(toXmVector(i_point), 0.0f, 0.0f,
      (float)d_viewportResolution.x, (float)d_viewportResolution.y, d_viewportMinZ, d_viewportMaxZ,
      d_projectionMatrix, d_viewMatrix, worldMatrix);

    XMFLOAT3 tempVector;
    XMStoreFloat3(&tempVector, res);
    return { tempVector.x, tempVector.y };
  }

  Sdk::RayF ThirdPersonCamera::screenToWorld(const Sdk::Vector2I& i_point) const
  {
    const auto worldMatrix = XMMatrixIdentity();

    const auto resNear = XMVector3Unproject(toXmVector(i_point, 0.0f), 0.0f, 0.0f,
      (float)d_viewportResolution.x, (float)d_viewportResolution.y, d_viewportMinZ, d_viewportMaxZ,
      d_projectionMatrix, d_viewMatrix, worldMatrix);
    const auto resFar = XMVector3Unproject(toXmVector(i_point, 1.0f), 0.0f, 0.0f,
      (float)d_viewportResolution.x, (float)d_viewportResolution.y, d_viewportMinZ, d_viewportMaxZ,
      d_projectionMatrix, d_viewMatrix, worldMatrix);

    XMFLOAT3 tempVectorNear;
    XMStoreFloat3(&tempVectorNear, resNear);
    XMFLOAT3 tempVectorFar;
    XMStoreFloat3(&tempVectorFar, resFar);

    return Sdk::RayF::createFromTwoPoints(fromXmVector(tempVectorNear), fromXmVector(tempVectorFar));
  }

} // ns Dx

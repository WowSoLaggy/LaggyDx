#include "stdafx.h"
#include "Camera.h"

#include <LaggySdk/Math.h>


namespace Dx
{
  namespace
  {

    XMVECTOR toXmVector(const Sdk::Vector3F& i_vector)
    {
      return XMVectorSet(i_vector.x, i_vector.y, i_vector.z, 1);
    }

  } // anonymous NS


  Camera::Camera(Sdk::Vector2I i_viewportResolution)
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


  void Camera::setFovAngle(const float i_fovAngle)
  {
    d_fovAngle = i_fovAngle;
    updateProjectionMatrix();
  }

  void Camera::setViewportMinZ(const float i_viewportMinZ)
  {
    d_viewportMinZ = i_viewportMinZ;
    updateProjectionMatrix();
  }

  void Camera::setViewportMaxZ(const float i_viewportMaxZ)
  {
    d_viewportMaxZ = i_viewportMaxZ;
    updateProjectionMatrix();
  }


  void Camera::setYaw(const float i_yaw)
  {
    d_yaw = i_yaw;
    updateViewMatrix();
  }

  void Camera::setPitch(const float i_pitch)
  {
    d_pitch = i_pitch;
    updateViewMatrix();
  }

  void Camera::setDistance(const float i_distance)
  {
    d_distance = i_distance;
    updateViewMatrix();
  }

  void Camera::setLookAt(Sdk::Vector3F i_lookAt)
  {
    d_lookAt = std::move(i_lookAt);
    updateViewMatrix();
  }

  void Camera::setUp(Sdk::Vector3F i_up)
  {
    i_up.normalize();
    d_up = std::move(i_up);
    updateViewMatrix();
  }


  Sdk::Vector3F Camera::getPosition() const
  {
    auto getVector = [](XMFLOAT3 i_vector)
    {
      return XMLoadFloat3(&i_vector);
    };

    static const auto yVector = getVector({ 0, -1, 0 });
    static const auto zVector = getVector({ 0, 0, -1 });

    XMFLOAT3 unitVector{ 1, 0, 0 };

    const auto qRotationY = XMQuaternionRotationAxis(yVector, d_pitch);
    const auto qRotationZ = XMQuaternionRotationAxis(zVector, d_yaw);

    XMStoreFloat3(&unitVector, XMVector3Rotate(XMLoadFloat3(&unitVector), qRotationY));
    XMStoreFloat3(&unitVector, XMVector3Rotate(XMLoadFloat3(&unitVector), qRotationZ));

    Sdk::Vector3F relativePosition{ unitVector.x * d_distance, unitVector.y * d_distance, unitVector.z * d_distance };
    return d_lookAt + relativePosition;
  }


  Sdk::Vector3F Camera::getLeft() const
  {
    auto left = getUp();
    left.cross(getForward());
    left.normalize();
    return left;
  }

  Sdk::Vector3F Camera::getRight() const
  {
    return -getLeft();
  }

  Sdk::Vector3F Camera::getForward() const
  {
    return getLookAt() - getPosition();
  }

  Sdk::Vector3F Camera::getBackward() const
  {
    return -getForward();
  }


  void Camera::updateProjectionMatrix()
  {
    const float screenAspect = (float)d_viewportResolution.x / (float)d_viewportResolution.y;
    d_projectionMatrix = XMMatrixPerspectiveFovRH(d_fovAngle, screenAspect,
      d_viewportMinZ, d_viewportMaxZ);
  }

  void Camera::updateViewMatrix()
  {
    d_viewMatrix = XMMatrixLookAtRH(toXmVector(getPosition()), toXmVector(d_lookAt), toXmVector(d_up));
  }


  Sdk::Vector2F Camera::worldToScreen(const Sdk::Vector3F& i_point) const
  {
    auto worldMatrix = XMMatrixIdentity();

    auto res = XMVector3Project(toXmVector(i_point), 0.0f, 0.0f,
      (float)d_viewportResolution.x, (float)d_viewportResolution.y, d_viewportMinZ, d_viewportMaxZ,
      d_projectionMatrix, d_viewMatrix, worldMatrix);

    XMFLOAT3 tempVector;
    XMStoreFloat3(&tempVector, res);
    return { tempVector.x, tempVector.y };
  }

} // ns Dx

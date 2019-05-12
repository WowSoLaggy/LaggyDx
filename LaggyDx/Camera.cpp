#include "stdafx.h"
#include "Camera.h"

#include <LaggySdk/Math.h>


namespace
{

  XMVECTOR toXmVector(const Sdk::Vector3& i_vector)
  {
    return XMVectorSet(i_vector.x, i_vector.y, i_vector.z, 1);
  }

} // anonymous NS


Camera::Camera(int i_screenWidth, int i_screenHeight)
  : d_yaw(0)
  , d_pitch(Sdk::degToRad(45.0f))
  , d_distance(10)
  , d_lookAt{ 0, 0, 0 }
  , d_up{ 0, 1, 0 }
  , d_viewportWidth(i_screenWidth)
  , d_viewportHeight(i_screenHeight)
{
  updateProjectionMatrix();
}


void Camera::setYaw(float i_yaw)
{
  d_yaw = i_yaw;
  updateViewMatrix();
}

void Camera::setPitch(float i_pitch)
{
  d_pitch = i_pitch;
  updateViewMatrix();
}

void Camera::setDistance(float i_distance)
{
  d_distance = i_distance;
  updateViewMatrix();
}

void Camera::setLookAt(Sdk::Vector3 i_lookAt)
{
  d_lookAt = std::move(i_lookAt);
  updateViewMatrix();
}

void Camera::setUp(Sdk::Vector3 i_up)
{
  i_up = Sdk::normalize(i_up);
  d_up = std::move(i_up);
  updateViewMatrix();
}


Sdk::Vector3 Camera::getPosition() const
{
  XMFLOAT3 unitVector{ 1, 0, 0 };

  auto qRotation = XMQuaternionRotationRollPitchYaw(0, d_yaw, d_pitch);
  XMStoreFloat3(&unitVector, XMVector3Rotate(XMLoadFloat3(&unitVector), qRotation));

  Sdk::Vector3 relativePosition{ unitVector.x * d_distance, unitVector.y * d_distance, unitVector.z * d_distance };
  return d_lookAt + relativePosition;
}


Sdk::Vector3 Camera::getLeft() const
{
  return normalize(cross(getUp(), getForward()));
}

Sdk::Vector3 Camera::getRight() const
{
  return -getLeft();
}

Sdk::Vector3 Camera::getForward() const
{
  return getLookAt() - getPosition();
}

Sdk::Vector3 Camera::getBackward() const
{
  return -getForward();
}


void Camera::updateProjectionMatrix()
{
  float screenAspect = (float)d_viewportWidth / (float)d_viewportHeight;
  d_projectionMatrix = XMMatrixPerspectiveFovRH(FovAngle, screenAspect,
                                                ViewportMinZ, ViewportMaxZ);
}

void Camera::updateViewMatrix()
{
  d_viewMatrix = XMMatrixLookAtRH(toXmVector(getPosition()), toXmVector(d_lookAt), toXmVector(d_up));
}


Sdk::Vector2 Camera::worldToScreen(const Sdk::Vector3& i_point) const
{
  auto worldMatrix = XMMatrixIdentity();

  auto res = XMVector3Project(toXmVector(i_point), 0.0f, 0.0f,
                              (float)d_viewportWidth, (float)d_viewportHeight, ViewportMinZ, ViewportMaxZ,
                              d_projectionMatrix, d_viewMatrix, worldMatrix);

  XMFLOAT3 tempVector;
  XMStoreFloat3(&tempVector, res);
  return { tempVector.x, tempVector.y };
}

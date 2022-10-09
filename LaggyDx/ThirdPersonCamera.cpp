#include "stdafx.h"
#include "ThirdPersonCamera.h"

#include <LaggySdk/Math.h>


namespace Dx
{
  ThirdPersonCamera::ThirdPersonCamera(Sdk::Vector2I i_viewportResolution)
    : CameraBase(std::move(i_viewportResolution))
    , d_yaw(0)
    , d_pitch(Sdk::degToRad(45.0f))
    , d_distance(10)
  {
    updateProjectionMatrix();
    updateViewMatrix();
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
    return getLookAt() + relativePosition;
  }

  void ThirdPersonCamera::setPosition(Sdk::Vector3F i_pos)
  {
    const auto diff = i_pos - getPosition();
    setLookAt(getLookAt() + diff);
  }

} // ns Dx

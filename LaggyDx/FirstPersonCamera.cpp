#include "stdafx.h"
#include "FirstPersonCamera.h"

#include <LaggySdk/Math.h>


namespace Dx
{
  namespace
  {
    DirectX::XMVECTOR xmVec(const DirectX::XMFLOAT3& i_vector)
    {
      return XMLoadFloat3(&i_vector);
    }

    DirectX::XMVECTOR xmVec(const float x, const float y, const float z)
    {
      return xmVec(DirectX::XMFLOAT3{ x, y, z });
    }

    DirectX::XMVECTOR xmVec(const Sdk::Vector3F& i_vector)
    {
      return xmVec(DirectX::XMFLOAT3{ i_vector.x, i_vector.y, i_vector.z });
    }

  } // anonym NS


  void FirstPersonCamera::setYaw(float i_yaw)
  {
    d_yaw = i_yaw;
    updateViewMatrix();
  }

  void FirstPersonCamera::setPitch(float i_pitch)
  {
    d_pitch = i_pitch;
    updateViewMatrix();
  }


  Sdk::Vector3F FirstPersonCamera::getLookAt() const
  {
    static const auto yVector = xmVec(0, 1, 0);

    XMFLOAT3 finalVector{ 1, 0, 0 };
    XMFLOAT3 zVector{ 0, 0, 1 };

    const auto qRotationY = XMQuaternionRotationAxis(yVector, -d_yaw);
    XMStoreFloat3(&finalVector, XMVector3Rotate(xmVec(finalVector), qRotationY));
    XMStoreFloat3(&zVector, XMVector3Rotate(xmVec(zVector), qRotationY));

    const auto qRotationZ = XMQuaternionRotationAxis(xmVec(zVector), d_pitch);
    XMStoreFloat3(&finalVector, XMVector3Rotate(xmVec(finalVector), qRotationZ));

    const Sdk::Vector3F relativePosition{ finalVector.x, finalVector.y, finalVector.z };
    return getPosition() + relativePosition;
  }

  void FirstPersonCamera::setLookAt(Sdk::Vector3F i_lookAt)
  {
    static const auto xVector = xmVec(1, 0, 0);

    const auto dir = i_lookAt - getPosition();
    const float proj = dir.dot(getWorldUp());
    const auto upProjVector = getWorldUp() * proj;
    const auto dirFlat = dir - upProjVector;

    XMFLOAT3 res;
    XMStoreFloat3(&res, XMVector3AngleBetweenVectors(xVector, xmVec(dirFlat)));
    setYaw(res.x);

    XMStoreFloat3(&res, XMVector3AngleBetweenVectors(xmVec(dir), xmVec(dirFlat)));
    setPitch(res.x * Sdk::sign<float>(proj));
  }

} // ns Dx

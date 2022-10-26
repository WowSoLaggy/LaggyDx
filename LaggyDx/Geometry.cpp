#include "stdafx.h"
#include "Geometry.h"


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

  } // anonym NS


  Sdk::Vector3D getVectorByYawAndPitch(double i_yaw, double i_pitch)
  {
    static const auto yVector = xmVec(0, 1, 0);

    XMFLOAT3 finalVector{ 1, 0, 0 };
    XMFLOAT3 zVector{ 0, 0, 1 };

    const auto qRotationY = XMQuaternionRotationAxis(yVector, -(float)i_yaw);
    XMStoreFloat3(&finalVector, XMVector3Rotate(xmVec(finalVector), qRotationY));
    XMStoreFloat3(&zVector, XMVector3Rotate(xmVec(zVector), qRotationY));

    const auto qRotationZ = XMQuaternionRotationAxis(xmVec(zVector), (float)i_pitch);
    XMStoreFloat3(&finalVector, XMVector3Rotate(xmVec(finalVector), qRotationZ));

    return { finalVector.x, finalVector.y, finalVector.z };
  }

} // ns Dx

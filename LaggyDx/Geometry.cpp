#include "stdafx.h"
#include "Geometry.h"

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

    template <typename T>
    DirectX::XMVECTOR xmVec(const Sdk::Vector3<T>& i_vec)
    {
      return xmVec(DirectX::XMFLOAT3{ i_vec.x, i_vec.y, i_vec.z });
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

  Sdk::Vector3F getYawAndPitchFromVector(const Sdk::Vector3F& i_v)
  {
    static const auto xVector = xmVec(1, 0, 0);
    static const auto zVector = Sdk::Vector3F{ 0, 0, 1 };
    static const auto WorldUp = Sdk::Vector3F{ 0, 1, 0 };
    const float projUp = i_v.dot(WorldUp);
    const float projZ = i_v.dot(zVector);
    const auto upProjVector = WorldUp * projUp;
    const auto dirFlat = i_v - upProjVector;

    Sdk::Vector3F retval;

    XMFLOAT3 res;
    XMStoreFloat3(&res, XMVector3AngleBetweenVectors(xVector, xmVec(dirFlat)));
    retval.y = res.x * Sdk::sign<float>(projZ);

    XMStoreFloat3(&res, XMVector3AngleBetweenVectors(xmVec(i_v), xmVec(dirFlat)));
    retval.z = res.x * Sdk::sign<float>(projUp);

    return retval;
  }

} // ns Dx

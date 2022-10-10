#include "stdafx.h"
#include "CameraBase.h"


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


  CameraBase::CameraBase(Sdk::Vector2I i_viewportResolution)
    : d_viewportResolution(i_viewportResolution)
    , d_projectionMatrix(DirectX::XMMATRIX())
    , d_viewMatrix(DirectX::XMMATRIX())
    , d_position{ 0.0f, 0.0f, 0.0f }
    , d_lookAt{ 1.0f, 0.0f, 0.0f }
    , d_up{ 0.0f, 1.0f, 0.0f }
  {
    updateProjectionMatrix();
    updateViewMatrix();
  }


  void CameraBase::setFovAngle(const float i_fovAngle)
  {
    d_fovAngle = i_fovAngle;
    updateProjectionMatrix();
  }

  void CameraBase::setViewportMinZ(const float i_viewportMinZ)
  {
    d_viewportMinZ = i_viewportMinZ;
    updateProjectionMatrix();
  }

  void CameraBase::setViewportMaxZ(const float i_viewportMaxZ)
  {
    d_viewportMaxZ = i_viewportMaxZ;
    updateProjectionMatrix();
  }


  void CameraBase::setLookAt(Sdk::Vector3F i_lookAt)
  {
    d_lookAt = std::move(i_lookAt);
    updateViewMatrix();
  }

  void CameraBase::setUp(Sdk::Vector3F i_up)
  {
    i_up.normalize();
    d_up = std::move(i_up);
    updateViewMatrix();
  }

  void CameraBase::setPosition(Sdk::Vector3F i_pos)
  {
    d_position = std::move(i_pos);
    updateViewMatrix();
  }


  Sdk::Vector3F CameraBase::getLeft() const
  {
    auto left = getUp();
    left = left.cross(getForward());
    left.normalize();
    return left;
  }

  Sdk::Vector3F CameraBase::getRight() const
  {
    return -getLeft();
  }

  Sdk::Vector3F CameraBase::getForward() const
  {
    return getLookAt() - getPosition();
  }

  Sdk::Vector3F CameraBase::getBackward() const
  {
    return -getForward();
  }


  void CameraBase::updateProjectionMatrix()
  {
    const float screenAspect = (float)d_viewportResolution.x / (float)d_viewportResolution.y;
    d_projectionMatrix = XMMatrixPerspectiveFovRH(getFovAngle(), screenAspect,
      getViewportMinZ(), getViewportMaxZ());
  }

  void CameraBase::updateViewMatrix()
  {
    d_viewMatrix = XMMatrixLookAtRH(
      toXmVector(getPosition()), toXmVector(getLookAt()), toXmVector(getUp()));
  }


  Sdk::Vector2F CameraBase::worldToScreen(const Sdk::Vector3F& i_point) const
  {
    const auto worldMatrix = XMMatrixIdentity();

    auto res = XMVector3Project(toXmVector(i_point), 0.0f, 0.0f,
      (float)d_viewportResolution.x, (float)d_viewportResolution.y, getViewportMinZ(), getViewportMaxZ(),
      getProjectionMatrix(), getViewMatrix(), worldMatrix);

    XMFLOAT3 tempVector;
    XMStoreFloat3(&tempVector, res);
    return { tempVector.x, tempVector.y };
  }

  Sdk::RayF CameraBase::screenToWorld(const Sdk::Vector2I& i_point) const
  {
    const auto worldMatrix = XMMatrixIdentity();

    const auto resNear = XMVector3Unproject(toXmVector(i_point, 0.0f), 0.0f, 0.0f,
      (float)d_viewportResolution.x, (float)d_viewportResolution.y, getViewportMinZ(), getViewportMaxZ(),
      getProjectionMatrix(), getViewMatrix(), worldMatrix);
    const auto resFar = XMVector3Unproject(toXmVector(i_point, 1.0f), 0.0f, 0.0f,
      (float)d_viewportResolution.x, (float)d_viewportResolution.y, getViewportMinZ(), getViewportMaxZ(),
      getProjectionMatrix(), getViewMatrix(), worldMatrix);

    XMFLOAT3 tempVectorNear;
    XMStoreFloat3(&tempVectorNear, resNear);
    XMFLOAT3 tempVectorFar;
    XMStoreFloat3(&tempVectorFar, resFar);

    return Sdk::RayF::createFromTwoPoints(fromXmVector(tempVectorNear), fromXmVector(tempVectorFar));
  }

} //ns Dx

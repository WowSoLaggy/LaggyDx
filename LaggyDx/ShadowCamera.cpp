#include "stdafx.h"
#include "ShadowCamera.h"


namespace Dx
{
  ShadowCamera::ShadowCamera()
  {
    updateMatrices();
  }


  void ShadowCamera::setLightDirection(Sdk::Vector3F i_direction)
  {
    CONTRACT_EXPECT(i_direction.lengthSq() > 0);
    d_lightDirection = std::move(i_direction);
    updateMatrices();
  }

  void ShadowCamera::setOrthoSize(const float i_size)
  {
    CONTRACT_EXPECT(i_size > 0);
    d_orthoSize = i_size;
    updateMatrices();
  }

  void ShadowCamera::setDepthRange(const float i_range)
  {
    CONTRACT_EXPECT(i_range > 0);
    d_depthRange = i_range;
    updateMatrices();
  }

  void ShadowCamera::setShadowMapResolution(const int i_resolution)
  {
    CONTRACT_EXPECT(i_resolution > 0);
    d_resolution = i_resolution;
    updateMatrices();
  }

  void ShadowCamera::setFocusPoint(const Sdk::Vector3F& i_focus)
  {
    d_focus = i_focus;
    updateMatrices();
  }


  void ShadowCamera::updateMatrices()
  {
    const XMVECTOR dir = XMVector3Normalize(
      XMVectorSet(d_lightDirection.x, d_lightDirection.y, d_lightDirection.z, 0.0f));

    // Fall back to a Z-up when the light is nearly vertical
    XMFLOAT3 dirNorm;
    XMStoreFloat3(&dirNorm, dir);
    const XMVECTOR up = std::abs(dirNorm.y) > 0.99f ?
      XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f) : XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    // Pure rotation view so light-space texels stay pinned to world positions
    d_viewMatrix = XMMatrixLookToLH(XMVectorZero(), dir, up);

    // Snap the ortho box center to whole shadow-texel steps to prevent edge shimmer
    const XMVECTOR focusLs = XMVector3Transform(
      XMVectorSet(d_focus.x, d_focus.y, d_focus.z, 1.0f), d_viewMatrix);
    const float texel = d_orthoSize / (float)d_resolution;

    XMFLOAT3 center;
    XMStoreFloat3(&center, focusLs);
    center.x = std::floor(center.x / texel) * texel;
    center.y = std::floor(center.y / texel) * texel;
    center.z = std::floor(center.z / texel) * texel;

    const float half = d_orthoSize / 2.0f;
    const float halfDepth = d_depthRange / 2.0f;
    d_projectionMatrix = XMMatrixOrthographicOffCenterLH(
      center.x - half, center.x + half,
      center.y - half, center.y + half,
      center.z - halfDepth, center.z + halfDepth);
  }

} // ns Dx

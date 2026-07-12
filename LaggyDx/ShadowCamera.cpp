#include "stdafx.h"
#include "ShadowCamera.h"


namespace
{
  // Quantize the fitted box size so the texel grid stays stable while the box only pans
  constexpr float SizeQuantum = 8.0f;

} // anonym NS


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
    d_focusArea.clear();
    updateMatrices();
  }

  void ShadowCamera::setFocusArea(std::vector<Sdk::Vector3F> i_worldPoints)
  {
    CONTRACT_EXPECT(!i_worldPoints.empty());
    d_focusArea = std::move(i_worldPoints);
    updateMatrices();
  }


  // Fixed-size light-space box around the single focus point
  void ShadowCamera::getFocusPointBox(XMFLOAT3& o_min, XMFLOAT3& o_max) const
  {
    XMFLOAT3 center;
    XMStoreFloat3(&center, XMVector3Transform(
      XMVectorSet(d_focus.x, d_focus.y, d_focus.z, 1.0f), d_viewMatrix));

    const float half = d_orthoSize / 2.0f;
    const float halfDepth = d_depthRange / 2.0f;
    o_min = { center.x - half, center.y - half, center.z - halfDepth };
    o_max = { center.x + half, center.y + half, center.z + halfDepth };
  }

  // Light-space box fitted to the focus area; the depth range pads it to catch off-screen casters
  void ShadowCamera::getFocusAreaBox(XMFLOAT3& o_min, XMFLOAT3& o_max) const
  {
    constexpr float maxFloat = std::numeric_limits<float>::max();
    o_min = { maxFloat, maxFloat, maxFloat };
    o_max = { -maxFloat, -maxFloat, -maxFloat };

    for (const auto& point : d_focusArea)
    {
      XMFLOAT3 pt;
      XMStoreFloat3(&pt, XMVector3Transform(
        XMVectorSet(point.x, point.y, point.z, 1.0f), d_viewMatrix));

      o_min.x = std::min(o_min.x, pt.x);
      o_min.y = std::min(o_min.y, pt.y);
      o_min.z = std::min(o_min.z, pt.z);
      o_max.x = std::max(o_max.x, pt.x);
      o_max.y = std::max(o_max.y, pt.y);
      o_max.z = std::max(o_max.z, pt.z);
    }

    const float halfDepth = d_depthRange / 2.0f;
    o_min.z -= halfDepth;
    o_max.z += halfDepth;
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

    XMFLOAT3 boxMin, boxMax;
    if (d_focusArea.empty())
      getFocusPointBox(boxMin, boxMax);
    else
      getFocusAreaBox(boxMin, boxMax);

    // Quantize the size and snap the center to whole shadow-texel steps to prevent edge shimmer
    const float sizeX = std::ceil(std::max(boxMax.x - boxMin.x, SizeQuantum) / SizeQuantum) * SizeQuantum;
    const float sizeY = std::ceil(std::max(boxMax.y - boxMin.y, SizeQuantum) / SizeQuantum) * SizeQuantum;
    const float texelX = sizeX / (float)d_resolution;
    const float texelY = sizeY / (float)d_resolution;
    const float centerX = std::floor((boxMin.x + boxMax.x) / 2.0f / texelX) * texelX;
    const float centerY = std::floor((boxMin.y + boxMax.y) / 2.0f / texelY) * texelY;

    d_projectionMatrix = XMMatrixOrthographicOffCenterLH(
      centerX - sizeX / 2.0f, centerX + sizeX / 2.0f,
      centerY - sizeY / 2.0f, centerY + sizeY / 2.0f,
      boxMin.z, boxMax.z);
  }

} // ns Dx

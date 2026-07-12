#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>

#include <DirectXMath.h>


namespace Dx
{
  // Light-space camera for shadow mapping: fixed-rotation view + texel-snapped ortho projection
  class ShadowCamera
  {
  public:
    ShadowCamera();

    Sdk::Vector3F getLightDirection() const { return d_lightDirection; }
    void setLightDirection(Sdk::Vector3F i_direction);

    void setOrthoSize(float i_size);
    void setDepthRange(float i_range);
    void setShadowMapResolution(int i_resolution);

    void setFocusPoint(const Sdk::Vector3F& i_focus);
    // Fits the ortho box to the light-space bounds of the given world points
    void setFocusArea(std::vector<Sdk::Vector3F> i_worldPoints);

    const DirectX::XMMATRIX& getViewMatrix() const { return d_viewMatrix; }
    const DirectX::XMMATRIX& getProjectionMatrix() const { return d_projectionMatrix; }

  private:
    Sdk::Vector3F d_lightDirection{ 0.0f, -1.0f, 0.0f };
    float d_orthoSize = 100.0f;
    float d_depthRange = 100.0f;
    int d_resolution = 1024;
    Sdk::Vector3F d_focus;
    std::vector<Sdk::Vector3F> d_focusArea;

    DirectX::XMMATRIX d_viewMatrix;
    DirectX::XMMATRIX d_projectionMatrix;

    void getFocusPointBox(DirectX::XMFLOAT3& o_min, DirectX::XMFLOAT3& o_max) const;
    void getFocusAreaBox(DirectX::XMFLOAT3& o_min, DirectX::XMFLOAT3& o_max) const;
    void updateMatrices();
  };

} // ns Dx

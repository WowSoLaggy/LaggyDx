#pragma once

#include "ICamera.h"

#include <LaggySdk/Math.h>


namespace Dx
{
  class CameraBase : public ICamera
  {
  public:
    CameraBase(Sdk::Vector2I i_viewportResolution);

    virtual float getFovAngle() const override { return d_fovAngle; }
    virtual void setFovAngle(float i_fovAngle) override;

    virtual float getViewportMinZ() const override { return d_viewportMinZ; }
    virtual void setViewportMinZ(float i_viewportMinZ) override;

    virtual float getViewportMaxZ() const override { return d_viewportMaxZ; }
    virtual void setViewportMaxZ(float i_viewportMaxZ) override;

    virtual Sdk::Vector3F getLookAt() const override { return d_lookAt; }
    virtual void setLookAt(Sdk::Vector3F i_lookAt) override;

    virtual Sdk::Vector3F getWorldUp() const override { return d_worldUp; }
    virtual void setWorldUp(Sdk::Vector3F i_up) override;

    virtual Sdk::Vector3F getPosition() const override { return d_position; }
    virtual void setPosition(Sdk::Vector3F i_pos) override;

    virtual Sdk::Vector3F getLeft() const override;
    virtual Sdk::Vector3F getRight() const override;
    virtual Sdk::Vector3F getForward() const override;
    virtual Sdk::Vector3F getBackward() const override;
    virtual Sdk::Vector3F getUp() const override;
    virtual Sdk::Vector3F getDown() const override;

    virtual Sdk::Vector2F worldToScreen(const Sdk::Vector3F& i_point) const override;
    virtual Sdk::RayF screenToWorld(const Sdk::Vector2I& i_point) const override;

    virtual const DirectX::XMMATRIX& getProjectionMatrix() const override { return d_projectionMatrix; }
    virtual const DirectX::XMMATRIX& getViewMatrix() const override { return d_viewMatrix; }

  protected:
    void updateProjectionMatrix();
    void updateViewMatrix();

  private:
    float d_fovAngle = (float)Sdk::Pi / 4.0f;
    float d_viewportMinZ = 0.1f;
    float d_viewportMaxZ = 1000.0f;

    Sdk::Vector3F d_position;
    Sdk::Vector3F d_lookAt;
    Sdk::Vector3F d_worldUp;

    Sdk::Vector2I d_viewportResolution;

    DirectX::XMMATRIX d_projectionMatrix;
    DirectX::XMMATRIX d_viewMatrix;
  };

} // ns Dx

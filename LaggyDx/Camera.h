#pragma once

#include "ICamera.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class Camera : public ICamera
  {
  public:

    Camera(Sdk::Vector2I i_viewportResolution);

    virtual float getFovAngle() const override { return d_fovAngle; }
    virtual void setFovAngle(float i_fovAngle) override;

    virtual float getViewportMinZ() const override { return d_viewportMinZ; }
    virtual void setViewportMinZ(float i_viewportMinZ) override;

    virtual float getViewportMaxZ() const override { return d_viewportMaxZ; }
    virtual void setViewportMaxZ(float i_viewportMaxZ) override;

    virtual float getYaw() const override { return d_yaw; }
    virtual void setYaw(float i_yaw) override;

    virtual float getPitch() const override { return d_pitch; }
    virtual void setPitch(float i_pitch) override;

    virtual float getDistance() const override { return d_distance; }
    virtual void setDistance(float i_distance) override;

    virtual Sdk::Vector3F getLookAt() const override { return d_lookAt; }
    virtual void setLookAt(Sdk::Vector3F i_lookAt) override;

    virtual Sdk::Vector3F getUp() const override { return d_up; }
    virtual void setUp(Sdk::Vector3F i_up) override;

    virtual Sdk::Vector3F getPosition() const override;
    virtual void setPosition(const Sdk::Vector3F& i_pos) override;

    virtual Sdk::Vector3F getLeft() const override;
    virtual Sdk::Vector3F getRight() const override;
    virtual Sdk::Vector3F getForward() const override;
    virtual Sdk::Vector3F getBackward() const override;

    virtual Sdk::Vector2F worldToScreen(const Sdk::Vector3F& i_point) const override;
    virtual Sdk::RayF screenToWorld(const Sdk::Vector2I& i_point) const override;

    virtual const DirectX::XMMATRIX& getProjectionMatrix() const override { return d_projectionMatrix; }
    virtual const DirectX::XMMATRIX& getViewMatrix() const override { return d_viewMatrix; }

  private:

    float d_fovAngle = (float)DirectX::XM_PI / 4.0f;
    float d_viewportMinZ = 0.1f;
    float d_viewportMaxZ = 1000.0f;

    Sdk::Vector2I d_viewportResolution;

    float d_yaw = 0.0;
    float d_pitch = 0.0;
    float d_distance = 0.0;
    Sdk::Vector3F d_lookAt;
    Sdk::Vector3F d_up;

    DirectX::XMMATRIX d_projectionMatrix;
    DirectX::XMMATRIX d_viewMatrix;

    void updateProjectionMatrix();
    void updateViewMatrix();
  };

} // ns Dx

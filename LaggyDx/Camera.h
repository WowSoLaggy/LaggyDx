#pragma once

#include "ICamera.h"

#include <Sdk/Vector.h>


namespace Dx
{
  class Camera : public ICamera
  {
  public:

    Camera(int i_screenWidth, int i_screenHeight);

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

    virtual Sdk::Vector3F getLeft() const override;
    virtual Sdk::Vector3F getRight() const override;
    virtual Sdk::Vector3F getForward() const override;
    virtual Sdk::Vector3F getBackward() const override;

    virtual Sdk::Vector2F worldToScreen(const Sdk::Vector3F& i_point) const override;

    const XMMATRIX& getProjectionMatrix() const { return d_projectionMatrix; }
    const XMMATRIX& getViewMatrix() const { return d_viewMatrix; }

  private:

    const float FovAngle = (float)DirectX::XM_PI / 4.0f;
    const float ViewportMinZ = 0.1f;
    const float ViewportMaxZ = 1000.0f;

    int d_viewportWidth;
    int d_viewportHeight;

    float d_yaw;
    float d_pitch;
    float d_distance;
    Sdk::Vector3F d_lookAt;
    Sdk::Vector3F d_up;

    XMMATRIX d_projectionMatrix;
    XMMATRIX d_viewMatrix;

    void updateProjectionMatrix();
    void updateViewMatrix();

    Sdk::Vector3F getPosition() const;

  };

} // ns Dx

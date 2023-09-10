#pragma once

#include "Camera3Base.h"


namespace Dx
{
  class ThirdPersonCamera : public Camera3Base
  {
  public:
    ThirdPersonCamera(Sdk::Vector2I i_viewportResolution);

    float getYaw() const { return d_yaw; }
    void setYaw(float i_yaw);

    float getPitch() const { return d_pitch; }
    void setPitch(float i_pitch);

    float getDistance() const { return d_distance; }
    void setDistance(float i_distance);

    virtual Sdk::Vector3F getPosition() const override;
    virtual void setPosition(Sdk::Vector3F i_pos) override;

  private:
    float d_yaw = 0.0;
    float d_pitch = 0.0;
    float d_distance = 0.0;
  };

} // ns Dx

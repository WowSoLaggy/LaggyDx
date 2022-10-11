#pragma once

#include "CameraBase.h"


namespace Dx
{
  class FirstPersonCamera : public CameraBase
  {
  public:
    using CameraBase::CameraBase;

    float getYaw() const { return d_yaw; }
    void setYaw(float i_yaw);

    float getPitch() const { return d_pitch; }
    void setPitch(float i_pitch);

    virtual Sdk::Vector3F getLookAt() const override;
    virtual void setLookAt(Sdk::Vector3F i_lookAt) override;

  private:
    float d_yaw = 0.0f;
    float d_pitch = 0.0f;
  };

} // ns Dx

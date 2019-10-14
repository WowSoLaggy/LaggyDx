#pragma once

#include <LaggySdk/SdkFwd.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  class ICamera
  {
  public:

    static std::shared_ptr<ICamera> createCamera(int i_screenWidth, int i_screenHeight);

  public:

    virtual ~ICamera() = default;

    virtual float getYaw() const = 0;
    virtual void setYaw(float i_yaw) = 0;

    virtual float getPitch() const = 0;
    virtual void setPitch(float i_pitch) = 0;

    virtual float getDistance() const = 0;
    virtual void setDistance(float i_distance) = 0;

    virtual Sdk::Vector3F getLookAt() const = 0;
    virtual void setLookAt(Sdk::Vector3F i_direction) = 0;

    virtual Sdk::Vector3F getUp() const = 0;
    virtual void setUp(Sdk::Vector3F i_up) = 0;

    virtual Sdk::Vector3F getLeft() const = 0;
    virtual Sdk::Vector3F getRight() const = 0;
    virtual Sdk::Vector3F getForward() const = 0;
    virtual Sdk::Vector3F getBackward() const = 0;

    virtual Sdk::Vector2F worldToScreen(const Sdk::Vector3F& i_point) const = 0;
  };

} // ns Dx

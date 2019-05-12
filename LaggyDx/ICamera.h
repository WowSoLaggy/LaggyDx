#pragma once

#include <LaggySdk/SdkFwd.h>


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

  virtual Sdk::Vector3 getLookAt() const = 0;
  virtual void setLookAt(Sdk::Vector3 i_direction) = 0;

  virtual Sdk::Vector3 getUp() const = 0;
  virtual void setUp(Sdk::Vector3 i_up) = 0;

  virtual Sdk::Vector3 getLeft() const = 0;
  virtual Sdk::Vector3 getRight() const = 0;
  virtual Sdk::Vector3 getForward() const = 0;
  virtual Sdk::Vector3 getBackward() const = 0;

  virtual Sdk::Vector2 worldToScreen(const Sdk::Vector3& i_point) const = 0;
};

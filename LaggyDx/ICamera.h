#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Ray.h>
#include <LaggySdk/SdkFwd.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  class ICamera
  {
  public:

    static std::unique_ptr<ICamera> createCamera(Sdk::Vector2I i_viewportResolution);

  public:

    virtual ~ICamera() = default;

    virtual float getFovAngle() const = 0;
    virtual void setFovAngle(float i_fovAngle) = 0;

    virtual float getViewportMinZ() const = 0;
    virtual void setViewportMinZ(float i_viewportMinZ) = 0;

    virtual float getViewportMaxZ() const = 0;
    virtual void setViewportMaxZ(float i_viewportMaxZ) = 0;

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

    virtual Sdk::Vector3F getPosition() const = 0;
    virtual void setPosition(const Sdk::Vector3F& i_pos) = 0;

    virtual Sdk::Vector3F getLeft() const = 0;
    virtual Sdk::Vector3F getRight() const = 0;
    virtual Sdk::Vector3F getForward() const = 0;
    virtual Sdk::Vector3F getBackward() const = 0;

    virtual Sdk::Vector2F worldToScreen(const Sdk::Vector3F& i_point) const = 0;
    virtual Sdk::RayF screenToWorld(const Sdk::Vector2I& i_point) const = 0;

    virtual const DirectX::XMMATRIX& getProjectionMatrix() const = 0;
    virtual const DirectX::XMMATRIX& getViewMatrix() const = 0;
  };

} // ns Dx

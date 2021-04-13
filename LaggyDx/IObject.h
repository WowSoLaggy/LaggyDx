#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class IObject
  {
  public:
    virtual ~IObject() = default;

    virtual void setTexture(const std::string& i_textureName) = 0;

    virtual void setPosition(Sdk::Vector2D i_position) = 0;
    virtual const Sdk::Vector2D& getPosition() const = 0;
    virtual Sdk::Vector2F getPositionF() const = 0;

    virtual void setSpeed(Sdk::Vector2D i_speed) = 0;
    virtual const Sdk::Vector2D& getSpeed() const = 0;
    virtual Sdk::Vector2F getSpeedF() const = 0;

    virtual void setRotation(double i_rotation) = 0;
    virtual double getRotation() const = 0;

    virtual void setColor(Sdk::Vector4F i_color) = 0;
    virtual const Sdk::Vector4F& getColor() const = 0;

    virtual void setScale(Sdk::Vector2D i_scale) = 0;
    virtual const Sdk::Vector2D& getScale() const = 0;

    virtual void render(IRenderer2d& i_renderer) const = 0;
    virtual void update(double i_dt) = 0;
  };

} // ns Dx

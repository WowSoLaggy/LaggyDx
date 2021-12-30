#pragma once

#include "CollisionShape.h"
#include "IObject.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class IObject2 : public IObject
  {
  public:
    virtual void setTexture(const std::string& i_textureName) = 0;

    virtual void setPosition(Sdk::Vector2D i_position) = 0;
    virtual const Sdk::Vector2D& getPosition() const = 0;
    virtual Sdk::Vector2F getPositionF() const = 0;

    virtual void setSpeed(Sdk::Vector2D i_speed) = 0;
    virtual const Sdk::Vector2D& getSpeed() const = 0;
    virtual Sdk::Vector2F getSpeedF() const = 0;

    virtual void setAccel(Sdk::Vector2D i_speed) = 0;
    virtual const Sdk::Vector2D& getAccel() const = 0;
    virtual Sdk::Vector2F getAccelF() const = 0;

    virtual void setRotation(double i_rotation) = 0;
    virtual double getRotation() const = 0;

    virtual void setColor(Sdk::Vector4F i_color) = 0;
    virtual const Sdk::Vector4F& getColor() const = 0;

    virtual void setScale(Sdk::Vector2D i_scale) = 0;
    virtual const Sdk::Vector2D& getScale() const = 0;

    virtual const Sprite& getSprite() const = 0;

    virtual void onCollide(const CollisionInfo& i_collInfo) = 0;
    virtual CollisionShape getPositionedCollisionShape() const = 0;

    virtual std::shared_ptr<ICollider> getCollider() const = 0;
    virtual void setCollider(std::shared_ptr<ICollider> i_collider) = 0;

    virtual void render(IRenderer2d& i_renderer) const = 0;

  };

} // ns Dx

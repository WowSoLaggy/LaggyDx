#pragma once

#include "CollisionShape.h"


namespace Dx
{
  class ICollider
  {
  public:
    virtual ~ICollider() = default;

    virtual bool getReceiveCollision() const = 0;

    virtual const CollisionShape& getCollisionShape() const = 0;
    virtual void setCollisionShape(CollisionShape i_collisionShape) = 0;

    virtual void update(double i_dt) = 0;

  };

} // ns Dx

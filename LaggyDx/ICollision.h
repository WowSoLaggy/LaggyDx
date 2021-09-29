#pragma once

#include "CollisionShape.h"


namespace Dx
{
  class ICollision
  {
  public:
    virtual ~ICollision() = default;

    virtual bool getSendColl() const = 0;
    virtual void setSendColl(bool i_sendColl) = 0;
    virtual bool getReceiveColl() const = 0;
    virtual void setReceiveColl(bool i_receiveColl) = 0;

    virtual const CollisionShape& getCollisionShape() const = 0;
    virtual void setCollisionShape(CollisionShape i_collisionShape) = 0;

  };

} // ns Dx

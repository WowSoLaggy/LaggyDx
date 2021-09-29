#pragma once

#include "ICollision.h"


namespace Dx
{
  class Collision : public ICollision
  {
  public:
    Collision();

    virtual bool getSendColl() const override;
    virtual void setSendColl(bool i_sendColl) override;

    virtual bool getReceiveColl() const override;
    virtual void setReceiveColl(bool i_receiveColl) override;

    virtual const CollisionShape& getCollisionShape() const override;
    virtual void setCollisionShape(CollisionShape i_collisionShape) override;

  private:
    bool d_sendCollision = false;
    bool d_receiveCollision = false;

    CollisionShape d_collisionShape;
  };

} // ns Dx

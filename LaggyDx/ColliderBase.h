#pragma once

#include "ICollider.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  class ColliderBase : public ICollider
  {
  public:
    ColliderBase(IObject2& io_object);

    virtual const CollisionShape& getCollisionShape() const override;
    virtual void setCollisionShape(CollisionShape i_collisionShape) override;

  protected:
    IObject2& getObject();
    const IObject2& getObject() const;

  private:
    IObject2& d_object;
    CollisionShape d_collisionShape;

  };

} // ns Dx

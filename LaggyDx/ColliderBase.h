#pragma once

#include "ICollider.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  class ColliderBase : public ICollider
  {
  public:
    ColliderBase(IObject& io_object);

    virtual const CollisionShape& getCollisionShape() const override;
    virtual void setCollisionShape(CollisionShape i_collisionShape) override;

  protected:
    IObject& getObject();
    const IObject& getObject() const;

  private:
    IObject& d_object;
    CollisionShape d_collisionShape;

  };

} // ns Dx

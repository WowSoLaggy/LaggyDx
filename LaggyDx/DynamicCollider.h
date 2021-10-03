#pragma once

#include "ColliderBase.h"


namespace Dx
{
  class DynamicCollider : public ColliderBase
  {
  public:
    DynamicCollider(IObject& io_object);

    virtual bool getReceiveCollision() const override;

    virtual void update(double i_dt) override;

  };

} // ns Dx

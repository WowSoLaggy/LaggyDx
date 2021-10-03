#pragma once

#include "ColliderBase.h"


namespace Dx
{
  class StaticCollider : public ColliderBase
  {
  public:
    StaticCollider(IObject& io_object);

    virtual bool getReceiveCollision() const override;

    virtual void update(double i_dt) override;

  };

} // ns Dx

#include "stdafx.h"
#include "DynamicCollider.h"

#include "IObject2.h"


namespace Dx
{
  DynamicCollider::DynamicCollider(IObject2& io_object)
    : ColliderBase(io_object)
  {
  }


  bool DynamicCollider::getReceiveCollision() const
  {
    return true;
  }


  void DynamicCollider::update(const double i_dt)
  {
    const auto accel = getObject().getAccel();
    if (accel.isNotZero(0.001 * 0.001))
      getObject().setSpeed(getObject().getSpeed() + accel * i_dt);

    const auto speed = getObject().getSpeed();
    if (speed.isNotZero(0.001 * 0.001))
      getObject().setPosition(getObject().getPosition() + speed * i_dt);
  }

} // ns Dx

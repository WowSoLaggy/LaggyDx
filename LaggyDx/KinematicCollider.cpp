#include "stdafx.h"
#include "KinematicCollider.h"

#include "IObject.h"


namespace Dx
{
  KinematicCollider::KinematicCollider(IObject& io_object)
    : ColliderBase(io_object)
  {
  }


  bool KinematicCollider::getReceiveCollision() const
  {
    return false;
  }


  void KinematicCollider::update(const double i_dt)
  {
    const auto accel = getObject().getAccel();
    if (accel.isNotZero(0.001 * 0.001))
      getObject().setSpeed(getObject().getSpeed() + accel * i_dt);

    const auto speed = getObject().getSpeed();
    if (speed.isNotZero(0.001 * 0.001))
      getObject().setPosition(getObject().getPosition() + speed * i_dt);
  }

} // ns Dx

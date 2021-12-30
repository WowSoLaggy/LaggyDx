#include "stdafx.h"
#include "StaticCollider.h"

namespace Dx
{
  StaticCollider::StaticCollider(IObject2& io_object)
    : ColliderBase(io_object)
  {
  }


  bool StaticCollider::getReceiveCollision() const
  {
    return false;
  }


  void StaticCollider::update(const double i_dt)
  {
  }

} // ns Dx

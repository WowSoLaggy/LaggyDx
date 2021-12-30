#include "stdafx.h"
#include "ColliderBase.h"


namespace Dx
{
  ColliderBase::ColliderBase(IObject2& io_object)
    : d_object(io_object)
    , d_collisionShape(Sdk::Vector2D::zero())
  {
  }


  const CollisionShape& ColliderBase::getCollisionShape() const
  {
    return d_collisionShape;
  }

  void ColliderBase::setCollisionShape(CollisionShape i_collisionShape)
  {
    d_collisionShape = std::move(i_collisionShape);
  }


  IObject2& ColliderBase::getObject() { return d_object; }
  const IObject2& ColliderBase::getObject() const { return d_object; }

} // ns Dx

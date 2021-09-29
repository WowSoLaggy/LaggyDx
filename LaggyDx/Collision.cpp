#include "stdafx.h"
#include "Collision.h"


namespace Dx
{
  Collision::Collision()
    : d_collisionShape(Sdk::Vector2D())
  {
  }


  bool Collision::getSendColl() const
  {
    return d_sendCollision;
  }

  void Collision::setSendColl(const bool i_sendColl)
  {
    d_sendCollision = i_sendColl;
  }


  bool Collision::getReceiveColl() const
  {
    return d_receiveCollision;
  }

  void Collision::setReceiveColl(const bool i_receiveColl)
  {
    d_receiveCollision = i_receiveColl;
  }


  const CollisionShape& Collision::getCollisionShape() const
  {
    return d_collisionShape;
  }

  void Collision::setCollisionShape(CollisionShape i_collisionShape)
  {
    d_collisionShape = std::move(i_collisionShape);
  }

} // ns Dx

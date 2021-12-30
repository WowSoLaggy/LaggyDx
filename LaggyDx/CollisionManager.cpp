#include "stdafx.h"
#include "CollisionManager.h"

#include "CollisionFunctor.h"
#include "ICollider.h"
#include "IObject2.h"


namespace Dx
{
  void CollisionManager::checkCollisions(const std::vector<std::shared_ptr<IObject2>>& i_objects)
  {
    for (const auto& objPtr1 : i_objects)
    {
      for (const auto& objPtr2 : i_objects)
        checkCollision(*objPtr1, *objPtr2);
    }
  }


  void CollisionManager::checkCollision(IObject2& i_sender, IObject2& i_receiver)
  {
    if (&i_sender == &i_receiver)
      return;

    if (const auto collInfoOpt = getCollisionInfoIfExists(i_sender, i_receiver))
      i_receiver.onCollide(*collInfoOpt);
  }


  std::optional<CollisionInfo> CollisionManager::getCollisionInfoIfExists(
    const IObject2& i_sender,
    const IObject2& i_receiver)
  {
    if (!i_sender.getCollider() || !i_receiver.getCollider())
      return std::nullopt;
    if (!i_receiver.getCollider()->getReceiveCollision())
      return std::nullopt;

    if (const auto collisionPointNormalOpt = std::visit(CollisionFunctor(),
        i_sender.getPositionedCollisionShape(),
        i_receiver.getPositionedCollisionShape()))
    {
      return CollisionInfo{ i_sender, i_receiver, *collisionPointNormalOpt };
    }

    return std::nullopt;
  }

} // ns Dx

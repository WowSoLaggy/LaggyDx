#include "stdafx.h"
#include "CollisionManager.h"

#include "Collider.h"
#include "ICollision.h"
#include "IObject.h"


namespace Dx
{
  void CollisionManager::checkCollisions(const std::vector<std::shared_ptr<IObject>>& i_objects)
  {
    for (const auto& objPtr1 : i_objects)
    {
      for (const auto& objPtr2 : i_objects)
        checkCollision(*objPtr1, *objPtr2);
    }
  }


  void CollisionManager::checkCollision(IObject& i_sender, IObject& i_receiver)
  {
    if (&i_sender == &i_receiver)
      return;

    if (const auto collInfoOpt = getCollisionInfoIfExists(i_sender, i_receiver))
      i_receiver.onCollide(*collInfoOpt);
  }


  std::optional<CollisionInfo> CollisionManager::getCollisionInfoIfExists(
    const IObject& i_obj1,
    const IObject& i_obj2)
  {
    if (!i_obj1.getCollision().getSendColl() || !i_obj2.getCollision().getReceiveColl())
      return std::nullopt;

    if (const auto collisionPointNormalOpt = std::visit(Collider(),
        i_obj1.getPositionedCollisionShape(),
        i_obj2.getPositionedCollisionShape()))
    {
      return CollisionInfo{ i_obj1, i_obj2, *collisionPointNormalOpt };
    }

    return std::nullopt;
  }

} // ns Dx

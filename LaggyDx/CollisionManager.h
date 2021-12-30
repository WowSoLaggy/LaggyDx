#pragma once

#include "CollisionInfo.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  class CollisionManager
  {
  public:
    void checkCollisions(const std::vector<std::shared_ptr<IObject2>>& i_objects);

  private:
    void checkCollision(IObject2& i_sender, IObject2& i_receiver);

    std::optional<CollisionInfo> getCollisionInfoIfExists(
      const IObject2& i_sender,
      const IObject2& i_receiver);

  };

} // ns Dx

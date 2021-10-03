#pragma once

#include "CollisionInfo.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  class CollisionManager
  {
  public:
    void checkCollisions(const std::vector<std::shared_ptr<IObject>>& i_objects);

  private:
    void checkCollision(IObject& i_sender, IObject& i_receiver);

    std::optional<CollisionInfo> getCollisionInfoIfExists(
      const IObject& i_sender,
      const IObject& i_receiver);

  };

} // ns Dx

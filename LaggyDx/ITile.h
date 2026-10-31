#pragma once

#include "IThdObject.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  namespace thd
  {
    class ITile
    {
    public:
      virtual ~ITile() = default;

      // Adds all objects that participate in the heat exchange for this tile (o_all)
      // and those that participate in the heat or gas exchange with the neighbor tile (o_external)
      virtual void collectThdObjects(std::vector<IThdObject*>& o_all, std::vector<IThdObject*>& o_external) const = 0;

      // Called before the tile's objects take part in an exchange
      virtual void beforeExchange() const {}
    };

  } // ns thd
} // ns Dx

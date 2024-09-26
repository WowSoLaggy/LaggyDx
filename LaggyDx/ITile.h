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

      // Returns all objects that participate in the heat exchange for this tile
      virtual std::vector<IThdObject*> getThdObjectsAll() const = 0;

      // Returns all objects that participate in the heat or gas exchange with the neighbor tile
      virtual std::vector<IThdObject*> getThdObjectsExternal() const = 0;
    };

  } // ns thd
} // ns Dx

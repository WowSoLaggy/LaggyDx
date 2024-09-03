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

      // Returns all objects that participate in the heat exchange
      virtual std::vector<IThdObject*> getThdObjects() const = 0;
    };

  } // ns thd
} // ns Dx

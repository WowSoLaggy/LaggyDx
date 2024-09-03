#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Rect.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  namespace thd
  {
    class ITileCollection
    {
    public:
      virtual ~ITileCollection() = default;

      // Returns area of tiles to be calculated.
      // Tiles outside this area are not calculated
      virtual const Sdk::RectI& getRect() const = 0;

      virtual std::shared_ptr<ITile> getTile(const Sdk::Vector2I& i_coords) const = 0;
    };

  } // ns thd
} // ns Dx

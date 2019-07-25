#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class ITextureResource
  {
  public:
    virtual ~ITextureResource() = default;

    virtual const ImageDescription& getDescription() const = 0;
    virtual const AnimationsMap& getAnimationsMap() const = 0;
  };
} // ns Dx

#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class ITextureResource
  {
  public:
    virtual ~ITextureResource() = default;

    virtual const ImageAnnotation& getAnnotation() const = 0;
  };
} // ns Dx

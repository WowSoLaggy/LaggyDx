#pragma once

#include "LaggyDxFwd.h"

#include <Sdk/Vector.h>


namespace Dx
{
  class ITextureResource
  {
  public:
    virtual ~ITextureResource() = default;

    virtual fs::path getFilename() const = 0;

    virtual const ImageDescription& getDescription() const = 0;
    virtual const AnimationsMap& getAnimationsMap() const = 0;

    virtual bool checkAlpha(Sdk::Vector2I i_coords, int i_frame = 0) const = 0;
  };
} // ns Dx

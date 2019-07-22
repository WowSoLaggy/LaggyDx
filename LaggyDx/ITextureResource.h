#pragma once

#include <LaggySdk/Vector.h>


namespace Dx
{
  class ITextureResource
  {
  public:
    virtual ~ITextureResource() = default;

    virtual const Sdk::Vector2I& getSize() const = 0;
  };
} // ns Dx

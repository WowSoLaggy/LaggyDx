#pragma once

#include <LaggySdk/SdkFwd.h>


namespace Dx
{
  class ITextureResource
  {
  public:
    virtual ~ITextureResource() = default;

    virtual const Sdk::Vector2& getSize() const = 0;
  };
} // ns Dx

#pragma once

#include <LaggySdk/SdkFwd.h>


class ITextureResource
{
public:
  virtual ~ITextureResource() = default;

  virtual const Sdk::Vector2& getSize() const = 0;
};

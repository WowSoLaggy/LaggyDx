#pragma once

#include "LoadableResource.h"

#include <LaggySdk/Rect.h>

#include <string>


namespace Dx
{
  class IFontResource : public LoadableResource
  {
  public:
    virtual Sdk::RectI getStringRect(const std::string& i_string) const = 0;
  };
} // ns Dx

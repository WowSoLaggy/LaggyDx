#pragma once

#include <Sdk/Rect.h>

#include <string>


namespace Dx
{
  class IFontResource
  {
  public:
    virtual ~IFontResource() = default;

    virtual Sdk::RectI getStringRect(const std::string& i_string) const = 0;
  };
} // ns Dx

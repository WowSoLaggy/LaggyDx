#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class TextureUtils
  {
  public:
    TextureUtils() = delete;

    static const Dx::ITexture& getTexture(const fs::path& i_name);
  };

} // ns Dx

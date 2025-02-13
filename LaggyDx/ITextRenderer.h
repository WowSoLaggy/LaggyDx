#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class ITextRenderer
  {
  public:
    virtual void begin() = 0;
    virtual void end() = 0;

    virtual void render(
      const std::string& i_text,
      const IFontResource& i_fontResource,
      const Sdk::Vector2F& i_translation,
      const Sdk::Vector4F& i_color,
      float i_scale) const = 0;
  };

} // ns Dx

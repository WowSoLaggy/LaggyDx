#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class Text
  {
  public:
    void setText(std::string i_text);
    const std::string& getText() const;

    void setFont(const std::string& i_fontName);

    void render(IRenderer2d& i_renderer, const Sdk::Vector2F& i_position) const;

  private:
    std::string d_text;
    const IFontResource* d_fontResource = nullptr;
  };

} // Dx

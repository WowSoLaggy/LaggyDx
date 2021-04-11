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
    const IFontResource* getFontResource() const;

    void setColor(Sdk::Vector4F i_color);
    const Sdk::Vector4F& getColor() const;

    void setScale(float i_scale);
    float getScale() const;

    void render(IRenderer2d& i_renderer, const Sdk::Vector2F& i_position) const;

  private:
    std::string d_text;
    const IFontResource* d_fontResource = nullptr;
    Sdk::Vector4F d_color = Sdk::Vector4F::identity();
    float d_scale = 1.0f;
  };

} // Dx

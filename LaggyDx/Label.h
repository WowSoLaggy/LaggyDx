#pragma once

#include "Control.h"
#include "Text.h"


namespace Dx
{
  class Label : public Control
  {
  public:
    virtual Sdk::Vector2F getSize() const override;

    virtual void setText(std::string i_text);
    const std::string& getText() const;

    void setFont(const std::string& i_fontName);
    const IFontResource* getFontResource() const;

    void setTextColor(Sdk::Vector4F i_color);
    const Sdk::Vector4F& getTextColor() const;

    void setTextOffset(Sdk::Vector2F i_offset);

    void setTextScale(float i_scale);
    float getTextScale() const;

    virtual void setOpacity(double i_opacity) override;

    virtual void render(IRenderer2d& i_renderer) const override;

  private:
    Text d_text;
    Sdk::Vector4F d_originalTextColor = Sdk::Vector4F::identity();
    Sdk::Vector2F d_textOffset;
  };

} // Dx

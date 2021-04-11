#pragma once

#include "Control.h"
#include "Text.h"


namespace Dx
{
  class Label : public Control
  {
  public:
    void setText(std::string i_text);
    const std::string& getText() const;

    void setFont(const std::string& i_fontName);
    const IFontResource* getFontResource() const;

    void setTextColor(Sdk::Vector4F i_color);
    const Sdk::Vector4F& getTextColor() const;

    virtual void setOpacity(double i_opacity) override;

    virtual void render(IRenderer2d& i_renderer, const Sdk::Vector2F& i_parentPos) const override;

  private:
    Text d_text;
    Sdk::Vector4F d_originalTextColor = Sdk::Vector4F::identity();
  };

} // Dx

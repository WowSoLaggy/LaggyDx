#include "stdafx.h"
#include "Label.h"

#include "Colors.h"


namespace Dx
{
  Sdk::Vector2F Label::getSize() const
  {
    return d_text.getSize() + d_textOffsetLeftTop + d_textOffsetRightBottom;
  }

  void Label::setText(std::string i_text)
  {
    d_text.setText(i_text);
    setSize(getSize());
  }

  const std::string& Label::getText() const
  {
    return d_text.getText();
  }


  void Label::setFont(const std::string& i_fontName)
  {
    d_text.setFont(i_fontName);
  }

  const IFontResource* Label::getFontResource() const
  {
    return d_text.getFontResource();
  }


  void Label::setTextColor(Sdk::Vector4F i_color)
  {
    d_originalTextColor = std::move(i_color);
    d_text.setColor(colorWithAlpha(d_originalTextColor, (float)getOpacity()));
  }

  const Sdk::Vector4F& Label::getTextColor() const
  {
    return d_originalTextColor;
  }


  void Label::setTextOffsetLeftTop(Sdk::Vector2F i_offset) { d_textOffsetLeftTop = std::move(i_offset); }
  void Label::setTextOffsetRightBottom(Sdk::Vector2F i_offset) { d_textOffsetRightBottom = std::move(i_offset); }


  void Label::setTextScale(const float i_scale)
  {
    d_text.setScale(i_scale);
  }

  float Label::getTextScale() const
  {
    return d_text.getScale();
  }


  void Label::setOpacity(const double i_opacity)
  {
    Control::setOpacity(i_opacity);
    d_text.setColor(colorWithAlpha(d_originalTextColor, (float)getOpacity()));
  }


  void Label::render(IRenderer2d& i_renderer) const
  {
    d_text.render(i_renderer, d_textOffsetLeftTop + getPositionAbsolute());
  }

} // ns Dx

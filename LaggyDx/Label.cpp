#include "stdafx.h"
#include "Label.h"

#include "Colors.h"


namespace Dx
{
  void Label::setText(std::string i_text)
  {
    d_text.setText(i_text);
  }

  const std::string& Label::getText() const
  {
    return d_text.getText();
  }


  void Label::setFont(const std::string& i_fontName)
  {
    d_text.setFont(i_fontName);
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


  void Label::setOpacity(const double i_opacity)
  {
    Control::setOpacity(i_opacity);
    d_text.setColor(colorWithAlpha(d_originalTextColor, (float)getOpacity()));
  }


  void Label::render(IRenderer2d& i_renderer, const Sdk::Vector2F& i_parentPos) const
  {
    d_text.render(i_renderer, i_parentPos + getPosition());
  }

} // ns Dx

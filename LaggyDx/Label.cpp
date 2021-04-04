#include "stdafx.h"
#include "Label.h"


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


  void Label::render(IRenderer2d& i_renderer, const Sdk::Vector2F& i_parentPos) const
  {
    d_text.render(i_renderer, i_parentPos + getPosition());
  }

} // ns Dx

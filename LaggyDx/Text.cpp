#include "stdafx.h"
#include "Text.h"

#include "Game.h"
#include "IRenderer2d.h"
#include "IResourceController.h"


namespace Dx
{
  void Text::setText(std::string i_text)
  {
    d_text = std::move(i_text);
  }

  const std::string& Text::getText() const
  {
    return d_text;
  }

  void Text::setFont(const std::string& i_fontName)
  {
    d_fontResource = &Game::get().getResourceController().getFontResource(i_fontName);
  }

  const IFontResource* Text::getFontResource() const
  {
    return d_fontResource;
  }


  void Text::setColor(Sdk::Vector4F i_color)
  {
    d_color = std::move(i_color);
  }

  const Sdk::Vector4F& Text::getColor() const
  {
    return d_color;
  }


  void Text::setScale(const float i_scale)
  {
    d_scale = i_scale;
  }

  float Text::getScale() const
  {
    return d_scale;
  }


  void Text::render(IRenderer2d& i_renderer, const Sdk::Vector2F& i_position) const
  {
    if (d_fontResource && !d_text.empty())
      i_renderer.renderText(d_text, *d_fontResource, i_position, d_color, d_scale);
  }

} // Dx

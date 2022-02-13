#include "stdafx.h"
#include "Panel.h"

#include "Game.h"
#include "IResourceController.h"


namespace Dx
{
  void Panel::setTexture(const std::string& i_textureName)
  {
    const auto& rc = Game::get().getResourceController();
    d_sprite.setTexture(rc.getTexture(i_textureName));
  }


  void Panel::setColor(Sdk::Vector4F i_color)
  {
    d_sprite.setColor(std::move(i_color));
  }

  
  void Panel::setSize(Sdk::Vector2F i_size)
  {
    d_sprite.setSize({ (int)i_size.x, (int)i_size.y });
    Control::setSize(i_size);
  }


  void Panel::render(IRenderer2d& i_renderer) const
  {
    i_renderer.setTranslation(getPosition());
    i_renderer.renderSprite(d_sprite);

    Control::render(i_renderer);
  }

} // ns Dx

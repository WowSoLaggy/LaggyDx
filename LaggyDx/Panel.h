#pragma once

#include "Control.h"
#include "Sprite.h"


namespace Dx
{
  class Panel : public Control
  {
  public:
    void setTexture(const std::string& i_textureName);

    void setColor(Sdk::Vector4F i_color);
    
    virtual void setSize(Sdk::Vector2F i_size) override;

    virtual void render(IRenderer2d& i_renderer, const Sdk::Vector2F& i_parentPos) const override;

  private:
    Sprite d_sprite;
  };

} // ns Dx

#pragma once

#include "Control.h"
#include "LaggyDxFwd.h"
#include "Sprite.h"


namespace Dx
{
  class Panel : public Control
  {
  public:
    void setTexture(const ITextureResource& i_texture);

    void setColor(Sdk::Vector4F i_color);
    
    virtual void setSize(Sdk::Vector2F i_size) override;

    virtual void render(IRenderer2d& i_renderer) const override;

  private:
    Sprite d_sprite;
  };

} // ns Dx

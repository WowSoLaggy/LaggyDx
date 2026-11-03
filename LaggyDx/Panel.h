#pragma once

#include "Control.h"
#include "LaggyDxFwd.h"
#include "AnimatedSprite.h"


namespace Dx
{
  class Panel : public Control
  {
  public:
    void setTexture(const ITexture& i_texture);
    void setTexture(const std::string& i_textureName);
    void setColor(Sdk::Vector4F i_color);
    void setRotation(float i_rotation);
    void setRotationOrigin(Sdk::Vector2F i_rotationOrigin);
    // Of a texture with several frames, the one shown
    void setFrame(int i_frame);
    
    virtual void setSize(Sdk::Vector2F i_size) override;

    virtual void render(IRenderer2d& i_renderer) const override;

  private:
    AnimatedSprite d_sprite;
  };

} // ns Dx

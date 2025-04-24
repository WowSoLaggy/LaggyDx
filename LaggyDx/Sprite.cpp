#include "stdafx.h"
#include "Sprite.h"

#include "ImageDescription.h"
#include "ITexture.h"


namespace Dx
{
  Sprite::Sprite(const ITexture* i_texture /*= nullptr*/, Sdk::Vector2F i_position /*= Sdk::Vector2I::zero()*/,
                 Sdk::Vector2F i_size /*= Sdk::Vector2I::zero()*/, Sdk::Vector4F i_color /*= Sdk::Vector4F::identity()*/)
    : d_texture(i_texture)
    , d_position(i_position)
    , d_size(i_size)
    , d_color(i_color)
  {
  }


  void Sprite::setTexture(const ITexture* i_texture)
  {
    d_texture = i_texture;
  }

  void Sprite::setTexture(const ITexture& i_texture)
  {
    setTexture(&i_texture);
  }


  void Sprite::resetSizeToTexture()
  {
    const auto size = d_texture ? d_texture->getDescription().frameSize : Sdk::Vector2I{ 0, 0 };
    d_size = size.getVector<float>();
  }


  RECT Sprite::getSourceRect() const
  {
    return { 0, 0, (int)d_size.x, (int)d_size.y };
  }


  Sdk::RectF Sprite::getRect() const
  {
    return { d_position, d_position + d_size };
  }


  bool Sprite::hasAnimation() const
  {
    return false;
  }

  int Sprite::getCurrentFrame() const
  {
    CONTRACT_EXPECT(false);
  }

  float Sprite::getFrameRatio() const
  {
    CONTRACT_EXPECT(false);
  }

} // ns Dx

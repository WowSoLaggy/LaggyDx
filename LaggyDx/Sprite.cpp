#include "stdafx.h"
#include "Sprite.h"

#include "ImageDescription.h"
#include "ITextureResource.h"


namespace Dx
{
  Sprite::Sprite(const ITextureResource* i_texture /*= nullptr*/, Sdk::Vector2I i_position /*= Sdk::Vector2I::zero()*/,
                 Sdk::Vector2I i_size /*= Sdk::Vector2I::zero()*/, Sdk::Vector4F i_color /*= Sdk::Vector4F::identity()*/)
    : d_texture(i_texture)
    , d_position(i_position)
    , d_size(i_size)
    , d_color(i_color)
  {
  }


  void Sprite::setTexture(const ITextureResource* i_texture)
  {
    d_texture = i_texture;
  }

  void Sprite::setTexture(const ITextureResource& i_texture)
  {
    setTexture(&i_texture);
  }


  void Sprite::resetSizeToTexture()
  {
    d_size = d_texture ? d_texture->getDescription().size() : Sdk::Vector2I{ 0, 0 };
  }


  RECT Sprite::getSourceRect() const
  {
    return { 0, 0, d_size.x, d_size.y };
  }


  Sdk::RectI Sprite::getRect() const
  {
    return { d_position, d_position + d_size };
  }
} // ns Dx

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
    d_size = d_texture->getDescription().size();
  }


  RECT Sprite::getSourceRect() const
  {
    return { 0, 0, d_texture->getDescription().width - 1, d_texture->getDescription().height - 1 };
  }
} // ns Dx

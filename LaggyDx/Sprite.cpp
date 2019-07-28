#include "stdafx.h"
#include "Sprite.h"

#include "ImageDescription.h"
#include "ITextureResource.h"


namespace Dx
{
  void Sprite::setTexture(const ITextureResource* i_texture)
  {
    d_texture = i_texture;
    d_size = {
      d_texture->getDescription().width,
      d_texture->getDescription().height };
  }


  RECT Sprite::getSourceRect() const
  {
    return { 0, 0, d_texture->getDescription().width - 1, d_texture->getDescription().height - 1 };
  }
} // ns Dx

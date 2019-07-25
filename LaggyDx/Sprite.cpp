#include "stdafx.h"
#include "Sprite.h"

#include "ImageDescription.h"
#include "ITextureResource.h"


namespace Dx
{
  RECT Sprite::getSourceRect() const
  {
    return { 0, 0, d_texture->getDescription().width, d_texture->getDescription().height };
  }

  const Sdk::Vector4F& Sprite::getColor() const
  {
    return d_color;
  }
} // ns Dx

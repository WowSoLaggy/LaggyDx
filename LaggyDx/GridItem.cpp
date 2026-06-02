#include "stdafx.h"
#include "GridItem.h"

#include "TextureUtils.h"


namespace Dx
{
  void GridItem::setTexture(const Dx::ITexture* i_texture)
  {
    d_texture = i_texture;
  }

  void GridItem::setTexture(const Dx::ITexture& i_texture)
  {
    setTexture(&i_texture);
  }

  void GridItem::setTexture(const std::string& i_textureName)
  {
    setTexture(TextureUtils::getTexture(i_textureName));
  }

  const Dx::ITexture* GridItem::getTexture() const
  {
    return d_texture;
  }

} // ns Dx

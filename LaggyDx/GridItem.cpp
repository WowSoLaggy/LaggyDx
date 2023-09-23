#include "stdafx.h"
#include "GridItem.h"


namespace Dx
{
  void GridItem::setTextureName(std::string i_textureName)
  {
    d_textureName = std::move(i_textureName);
  }

  const std::string& GridItem::getTextureName() const
  {
    return d_textureName;
  }

} // ns Dx

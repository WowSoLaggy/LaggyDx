#include "stdafx.h"
#include "AnimatedSprite.h"

#include "ImageDescription.h"
#include "ITexture.h"


namespace Dx
{
  RECT AnimatedSprite::getSourceRect() const
  {
    const int width = d_texture->getDescription().width;
    const int height = d_texture->getDescription().height;

    return { d_curFrame * width, 0, (d_curFrame + 1) * width, height };
  }


  void AnimatedSprite::setCurrentFrame(const int i_curFrame)
  {
    d_curFrame = i_curFrame;
  }

} // ns Dx

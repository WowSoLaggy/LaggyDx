#include "stdafx.h"
#include "AnimatedSprite.h"

#include "ImageDescription.h"
#include "ITexture.h"


namespace Dx
{
  RECT AnimatedSprite::getSourceRect() const
  {
    const int frameWidth = d_texture->getDescription().frameWidth;
    const int frameHeight = d_texture->getDescription().frameHeight;

    return { d_curFrame * frameWidth, 0, (d_curFrame + 1) * frameWidth, frameHeight };
  }


  void AnimatedSprite::setCurrentFrame(const int i_curFrame)
  {
    d_curFrame = i_curFrame;
  }

} // ns Dx

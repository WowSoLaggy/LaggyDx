#include "stdafx.h"
#include "AnimatedSprite.h"

#include "ImageDescription.h"
#include "ITexture.h"


namespace Dx
{
  RECT AnimatedSprite::getSourceRect() const
  {
    CONTRACT_EXPECT(d_texture);

    const int frameWidth = d_texture->getDescription().frameSize.x;
    const int frameHeight = d_texture->getDescription().frameSize.y;

    return { d_curFrame * frameWidth, 0, (d_curFrame + 1) * frameWidth, frameHeight };
  }


  void AnimatedSprite::setCurrentFrame(const int i_curFrame)
  {
    d_curFrame = i_curFrame;
  }


  bool AnimatedSprite::hasAnimation() const
  {
    return true;
  }

  int AnimatedSprite::getCurrentFrame() const
  {
    return d_curFrame;
  }

  float AnimatedSprite::getFrameRatio() const
  {
    CONTRACT_EXPECT(d_texture);
    return (float)d_texture->getDescription().frameSize.x / d_texture->getDescription().size.x;
  }

} // ns Dx

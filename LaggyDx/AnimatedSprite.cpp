#include "stdafx.h"
#include "AnimatedSprite.h"

#include "ImageDescription.h"
#include "ITextureResource.h"

#include <LaggySdk/Contracts.h>


namespace Dx
{
  void AnimatedSprite::update(double i_dt)
  {
    if (!d_curAnimation)
      return;

    d_curFrameTime += i_dt;
    if (d_curFrameTime >= d_curAnimation->frameTime)
      advanceFrame();
  }


  RECT AnimatedSprite::getSourceRect() const
  {
    const int width = d_texture->getDescription().width;
    const int height = d_texture->getDescription().height;

    return { d_curFrame * width, 0, (d_curFrame + 1) * width, height };
  }


  void AnimatedSprite::playAnimation(const std::string& i_animation)
  {
    const auto& animations = d_texture->getAnimationsMap();
    const auto it = animations.find(i_animation);
    CONTRACT_EXPECT(it != animations.cend());

    d_curAnimation = it->second;
    d_curFrame = d_curAnimation->start;
    d_curFrameTime = 0;
  }

  void AnimatedSprite::stopAnimation()
  {
    d_curAnimation.reset();
  }

  bool AnimatedSprite::isPlaying() const
  {
    return d_curAnimation.has_value();
  }

  int AnimatedSprite::getFrame() const
  {
    return d_curFrame;
  }

  void AnimatedSprite::setFrame(int i_frame)
  {
    d_curFrame = i_frame;
  }

  void AnimatedSprite::setFrameEnd()
  {
    if (!d_curAnimation)
      return;
    d_curFrame = d_curAnimation->end;
    stopAnimation();
  }


  void AnimatedSprite::advanceFrame()
  {
    if (d_curFrame == d_curAnimation->end)
    {
      stopAnimation();
      return;
    }

    d_curFrame += getAnimationDirectionSign();
    d_curFrameTime = 0;
  }

  int AnimatedSprite::getAnimationDirectionSign()
  {
    return d_curAnimation->end > d_curAnimation->start ? 1 : -1;
  }
} // ns Dx

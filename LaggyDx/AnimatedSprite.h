#pragma once

#include "ImageAnimation.h"
#include "Sprite.h"


namespace Dx
{
  class AnimatedSprite : public Sprite
  {
  public:
    void update(double i_dt);

    virtual RECT getSourceRect() const override;

    void playAnimation(const std::string& i_animation);
    void stopAnimation();
    bool isPlaying() const;

  private:
    int d_curFrame = 0;
    std::optional<ImageAnimation> d_curAnimation;
    double d_curFrameTime = 0;

    void advanceFrame();
    int getAnimationDirectionSign();
  };
} // ns Dx

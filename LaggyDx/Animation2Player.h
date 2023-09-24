#pragma once

#include "ImageAnimation.h"


namespace Dx
{
  class Animation2Player
  {
  public:
    void update(double i_dt);

    void playAnimation(ImageAnimation i_animation, std::optional<int> i_times);
    void stopAnimation();

    int getCurrentFrame() const;

  private:
    std::optional<ImageAnimation> d_animation;
    double d_animationTime = 0;
    int d_curFrame = 0;
    bool d_isForwardAnimation = true;
    std::optional<int> d_timesLeftToPlay;

    void advanceFrame();
  };

} // ns Dx

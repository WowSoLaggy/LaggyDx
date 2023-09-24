#include "stdafx.h"
#include "Animation2Player.h"


namespace Dx
{
  void Animation2Player::update(const double i_dt)
  {
    if (!d_animation)
      return;

    d_animationTime += i_dt;
    while (d_animation && (d_animationTime > d_animation->frameTime))
      advanceFrame();
  }


  void Animation2Player::playAnimation(ImageAnimation i_animation, std::optional<int> i_times)
  {
    d_animation = std::move(i_animation);
    d_animationTime = 0;
    d_isForwardAnimation = d_animation->end >= d_animation->start;
    d_timesLeftToPlay = i_times;
  }

  void Animation2Player::stopAnimation()
  {
    d_animation.reset();
    d_animationTime = 0;
  }


  int Animation2Player::getCurrentFrame() const
  {
    return d_curFrame;
  }


  void Animation2Player::advanceFrame()
  {
    CONTRACT_EXPECT(d_animation);

    if (d_curFrame != d_animation->end)
    {
      // Not final frame - just iterate to the next one

      d_curFrame = d_isForwardAnimation ? d_curFrame + 1 : d_curFrame - 1;
      d_animationTime -= d_animation->frameTime;
    }
    else
    {
      // Already final frame
      
      if (!d_timesLeftToPlay || *d_timesLeftToPlay > 1)
      {
        // Restart animation

        d_curFrame = d_animation->start;
        d_animationTime -= d_animation->frameTime;

        if (d_timesLeftToPlay)
          *d_timesLeftToPlay -= 1;
      }
      else
      {
        // Stop animation at the current frame
        stopAnimation();
      }
    }
  }

} // ns Dx

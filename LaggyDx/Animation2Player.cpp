#include "stdafx.h"
#include "Animation2Player.h"

#include "AnimationEvents.h"


namespace Dx
{
  void Animation2Player::pushFields()
  {
    pushField("animationName", d_animationName);
    pushField("animationTime", d_animationTime);
    pushField("curFrame", d_curFrame);
    pushOptional("timesLeftToPlay", d_timesLeftToPlay);
  }


  void Animation2Player::update(const double i_dt)
  {
    if (!d_animation)
      return;

    d_animationTime += i_dt;
    while (d_animation && (d_animationTime > d_animation->frameTime))
      advanceFrame();
  }


  void Animation2Player::playAnimation(
    const ImageAnimation* i_animation, std::optional<int> i_times, const bool i_resetTime)
  {
    CONTRACT_EXPECT(i_animation);

    d_animation = i_animation;
    d_animationName = d_animation->name;
    d_isForwardAnimation = d_animation->end >= d_animation->start;
    d_timesLeftToPlay = i_times;

    if (i_resetTime)
    {
      d_animationTime = 0;
      setFrame(d_animation->start);
    }
  }

  void Animation2Player::stopAnimation()
  {
    d_animation = nullptr;
    d_animationName.clear();
    d_animationTime = 0;
    notify(AnimationStoppedEvent());
  }


  int Animation2Player::getCurrentFrame() const
  {
    return d_curFrame;
  }

  bool Animation2Player::isPlaying() const
  {
    return d_animation;
  }


  const std::string& Animation2Player::getAnimationName() const
  {
    return d_animationName;
  }

  int Animation2Player::getTimesLeft() const
  {
    if (!d_timesLeftToPlay.has_value())
      return 1;

    return std::max(0, *d_timesLeftToPlay);
  }


  void Animation2Player::setFrame(const int i_frame)
  {
    d_curFrame = i_frame;
    notify(AnimationFrameChangedEvent(i_frame, SAFE_DEREF(d_animation).getFrameCount()));
  }


  void Animation2Player::advanceFrame()
  {
    CONTRACT_EXPECT(d_animation);

    if (d_curFrame != d_animation->end)
    {
      // Not final frame - just iterate to the next one

      setFrame(d_isForwardAnimation ? d_curFrame + 1 : d_curFrame - 1);
      d_animationTime -= d_animation->frameTime;
    }
    else
    {
      // Already final frame
      
      if (!d_timesLeftToPlay || *d_timesLeftToPlay > 1)
      {
        // Restart animation

        setFrame(d_animation->start);
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

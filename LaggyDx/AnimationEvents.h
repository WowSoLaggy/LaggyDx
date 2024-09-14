#pragma once

#include <LaggySdk/IEvent.h>


namespace Dx
{
  class AnimationStoppedEvent : public Sdk::IEvent
  {
  };

  class AnimationFrameChangedEvent : public Sdk::IEvent
  {
  public:
    AnimationFrameChangedEvent(const int i_currentFrame, const int i_animationLength)
      : d_currentFrame(i_currentFrame), d_animationLength(i_animationLength)
    {
    }

    [[nodiscard]] int getCurrentFrame() const { return d_currentFrame; }
    [[nodiscard]] int getAnimationLength() const { return d_animationLength; }

  private:
    int d_currentFrame = 0;
    int d_animationLength = 0;
  };

} // Dx

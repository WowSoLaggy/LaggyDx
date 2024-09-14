#pragma once

#include "ImageAnimation.h"

#include <LaggySdk/EventHandler.h>
#include <LaggySdk/ISerializable.h>


namespace Dx
{
  class Animation2Player : public Sdk::EventHandler, public Sdk::ISerializable
  {
  public:
    virtual void pushFields() override;

    void update(double i_dt);

    void playAnimation(const ImageAnimation* i_animation, std::optional<int> i_times, bool i_resetTime = true);
    void stopAnimation();

    int getCurrentFrame() const;
    bool isPlaying() const;

    const std::string& getAnimationName() const;
    int getTimesLeft() const;

  private:
    const ImageAnimation* d_animation = nullptr;
    std::string d_animationName;
    double d_animationTime = 0;
    int d_curFrame = 0;
    bool d_isForwardAnimation = true;
    std::optional<int> d_timesLeftToPlay;

    void setFrame(int i_frame);
    void advanceFrame();
  };

} // ns Dx

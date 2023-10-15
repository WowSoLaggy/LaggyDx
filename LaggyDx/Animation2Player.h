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

    void playAnimation(const ImageAnimation* i_animation, std::optional<int> i_times);
    void stopAnimation();

    int getCurrentFrame() const;
    bool isPlaying() const;

  private:
    const ImageAnimation* d_animation = nullptr;
    double d_animationTime = 0;
    int d_curFrame = 0;
    bool d_isForwardAnimation = true;
    std::optional<int> d_timesLeftToPlay;

    void advanceFrame();
  };

} // ns Dx

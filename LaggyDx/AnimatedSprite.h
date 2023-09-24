#pragma once

#include "ImageAnimation.h"
#include "Sprite.h"


namespace Dx
{
  class AnimatedSprite : public Sprite
  {
  public:
    virtual RECT getSourceRect() const override;

    void setCurrentFrame(int i_curFrame);

    virtual bool hasAnimation() const override;
    virtual int getCurrentFrame() const override;
    virtual float getFrameRatio() const override;

  private:
    int d_curFrame = 0;

  public:
    virtual void writeTo(std::ostream& io_stream) const override;
    virtual void readFrom(std::istream& io_stream, IResourceController& i_resourceController) override;
  };
} // ns Dx

#include "stdafx.h"
#include "AnimatedSprite.h"

#include <LaggySdk/Streams.h>


namespace Dx
{
  void AnimatedSprite::writeTo(std::ostream& io_stream) const
  {
    Sprite::writeTo(io_stream);

    // Specific

    Sdk::write(io_stream, d_curFrame);

    bool hasAnimation = d_curAnimation.has_value();
    Sdk::write(io_stream, hasAnimation);
    if (hasAnimation)
    {
      Sdk::write(io_stream, d_curAnimation->start);
      Sdk::write(io_stream, d_curAnimation->end);
      Sdk::write(io_stream, d_curAnimation->frameTime);
    }

    Sdk::write(io_stream, d_curFrameTime);
  }

  void AnimatedSprite::readFrom(std::istream& io_stream, IResourceController& i_resourceController)
  {
    Sprite::readFrom(io_stream, i_resourceController);

    // Specific

    Sdk::read(io_stream, d_curFrame);

    bool hasAnimation;
    Sdk::read(io_stream, hasAnimation);
    if (hasAnimation)
    {
      d_curAnimation = ImageAnimation{};

      Sdk::read(io_stream, d_curAnimation->start);
      Sdk::read(io_stream, d_curAnimation->end);
      Sdk::read(io_stream, d_curAnimation->frameTime);
    }

    Sdk::read(io_stream, d_curFrameTime);
  }

} // ns Dx

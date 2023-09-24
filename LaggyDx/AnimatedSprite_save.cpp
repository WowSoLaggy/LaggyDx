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
  }

  void AnimatedSprite::readFrom(std::istream& io_stream, IResourceController& i_resourceController)
  {
    Sprite::readFrom(io_stream, i_resourceController);

    // Specific

    Sdk::read(io_stream, d_curFrame);
  }

} // ns Dx

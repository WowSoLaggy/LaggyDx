#include "stdafx.h"
#include "Sprite.h"

#include "ITextureResource.h"

#include <LaggySdk/Streams.h>


namespace Dx
{
  std::ostream& operator<<(std::ostream& io_stream, const Sprite& i_sprite)
  {
    const std::string textureFilename = i_sprite.getTexture() ? i_sprite.getTexture()->getFilename().string() : "";
    Sdk::write(io_stream, (int)textureFilename.length());
    io_stream << textureFilename;

    io_stream << i_sprite.d_position;
    io_stream << i_sprite.d_size;
    io_stream << i_sprite.d_color;

    return io_stream;
  }

} // ns Dx

#include "stdafx.h"
#include "Sprite.h"

#include "IResourceController.h"
#include "ITextureResource.h"

#include <LaggySdk/Streams.h>


namespace Dx
{
  void Sprite::writeTo(std::ostream& io_stream) const
  {
    const std::string textureFilename = getTexture() ? getTexture()->getFilename().string() : "";
    Sdk::writeString(io_stream, textureFilename);

    io_stream << d_position;
    io_stream << d_size;
    io_stream << d_color;
  }

  void Sprite::readFrom(std::istream& io_stream, IResourceController& i_resourceController)
  {
    const auto textureName = Sdk::readString(io_stream);
    setTexture(&i_resourceController.getTextureResource(textureName));

    io_stream >> d_position;
    io_stream >> d_size;
    io_stream >> d_color;
  }
} // ns Dx

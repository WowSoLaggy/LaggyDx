#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  namespace TextureUtils
  {
    enum class GlChannels
    {
      R = 1,
      RG = 2,
      RGB = 3,
      RGBA = 4
    };

    const Dx::ITexture& getTexture(const fs::path& i_name);

    std::shared_ptr<MemoryTexture> createMemoryTexture(
      const IRenderDevice& i_renderDevice,
      const unsigned char* i_imageData,
      int i_width,
      int i_height,
      GlChannels i_channels);

  } // ns TextureUtils
} // ns Dx

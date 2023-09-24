#pragma once

#include "LaggyDxFwd.h"
#include "LoadableResource.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class ITexture : public LoadableResource
  {
  public:
    virtual fs::path getFilename() const = 0;

    virtual const ImageDescription& getDescription() const = 0;
    virtual const Animations2Map& getAnimationsMap() const = 0;

    virtual bool hasAlpha() const = 0;
    virtual bool checkAlpha(Sdk::Vector2I i_coords, int i_frame = 0) const = 0;

    virtual const std::shared_ptr<IBitmap> getBitmap(IRenderDevice& i_renderDevice) const = 0;

    virtual ID3D11ShaderResourceView* getTexturePtr() const = 0;
    virtual const D3D11_TEXTURE2D_DESC& getTextureDesc() const = 0;

  };
  
} // ns Dx

#pragma once

#include "ITexture.h"


namespace Dx
{
  class TextureBase : public ITexture
  {
  public:
    virtual const std::shared_ptr<IBitmap> getBitmap(IRenderDevice& i_renderDevice) const override;
  };

} // ns Dx

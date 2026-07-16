#pragma once

#include "ITexture.h"


namespace Dx
{
  class TextureBase : public ITexture
  {
  public:
    virtual const std::shared_ptr<IBitmap> getBitmap() const override;
  };

} // ns Dx

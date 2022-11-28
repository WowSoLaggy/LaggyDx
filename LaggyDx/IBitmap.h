#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class IBitmap
  {
  public:
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    virtual int getStride() const = 0;

    virtual const unsigned char* getData() const = 0;

    virtual const Channels& getChannelsInfo() const = 0;
  };

} // ns Dx

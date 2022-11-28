#pragma once

#include "ChannelInfo.h"
#include "IBitmap.h"


namespace Dx
{
  class Bitmap : public IBitmap
  {
  public:
    virtual int getWidth() const override;
    virtual int getHeight() const override;
    virtual int getStride() const override;
    
    virtual const unsigned char* getData() const override;

    virtual const Channels& getChannelsInfo() const override;

    unsigned char* getData();
    void resize(int i_width, int i_height, int i_stride);

  private:
    int d_width = 0;
    int d_height = 0;
    int d_stride = 0;

    mutable std::optional<Channels> d_channels;

    std::vector<unsigned char> d_data;

    void calculateChannels() const;
  };

} // ns Dx

#include "stdafx.h"
#include "Bitmap.h"


namespace Dx
{
  int Bitmap::getWidth() const { return d_width; }
  int Bitmap::getHeight() const { return d_height; }
  int Bitmap::getStride() const { return d_stride; }

  unsigned char* Bitmap::getData() { return d_data.data(); }
  const unsigned char* Bitmap::getData() const { return d_data.data(); }


  const Channels& Bitmap::getChannelsInfo() const
  {
    if (!d_channels)
      calculateChannels();

    CONTRACT_EXPECT(d_channels);
    return *d_channels;
  }

  void Bitmap::calculateChannels() const
  {
    auto initChannel = [&](ChannelInfo& i_info, unsigned char i_value) {
      i_info.min = i_value;
      i_info.max = i_value;
      i_info.mean = i_value;
    };

    auto createChannels = [&](unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
      d_channels = Channels();
      initChannel(d_channels->r, r);
      initChannel(d_channels->g, g);
      initChannel(d_channels->b, b);
      initChannel(d_channels->a, a);
    };

    auto addValue = [&](ChannelInfo& i_info, unsigned char i_value) {
      if (i_value < i_info.min)
        i_info.min = i_value;
      if (i_value > i_info.max)
        i_info.max = i_value;
    };

    auto setMean = [&](ChannelInfo& i_info, int i_sum) {
      i_info.mean = (double)i_sum / (d_width * d_height);
    };

    int rSum = 0;
    int gSum = 0;
    int bSum = 0;
    int aSum = 0;


    for (int y = 0; y < (int)d_height; ++y)
    {
      const unsigned char* rowStart = d_data.data() + d_stride * y;
      for (int x = 0; x < (int)d_width; ++x, rowStart += 4)
      {
        const unsigned char r = *(rowStart + 0);
        const unsigned char g = *(rowStart + 1);
        const unsigned char b = *(rowStart + 2);
        const unsigned char a = *(rowStart + 3);

        if (!d_channels)
          createChannels(r, g, b, a);
        else
        {
          addValue(d_channels->r, r);
          addValue(d_channels->g, g);
          addValue(d_channels->b, b);
          addValue(d_channels->a, a);
        }

        rSum += r;
        gSum += g;
        bSum += b;
        aSum += a;
      }
    }

    setMean(d_channels->r, rSum);
    setMean(d_channels->g, gSum);
    setMean(d_channels->b, bSum);
    setMean(d_channels->a, aSum);
  }


  void Bitmap::resize(const int i_width, const int i_height, const int i_stride)
  {
    d_data.resize(i_stride * i_height);
    d_width = i_width;
    d_height = i_height;
    d_stride = i_stride;
  }

} // ns Dx

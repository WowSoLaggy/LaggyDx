#pragma once


namespace Dx
{
  class ChannelInfo
  {
  public:
    int min = 0;
    int max = 0;
    double mean = 0;
  };

  class Channels
  {
  public:
    ChannelInfo r;
    ChannelInfo g;
    ChannelInfo b;
    ChannelInfo a;
  };

} // Dx

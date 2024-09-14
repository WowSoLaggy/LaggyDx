#pragma once


namespace Dx
{
  struct ImageAnimation
  {
    std::string name;

    int start = 0;
    int end = 0;
    double frameTime = 0;

    int getFrameCount() const { return std::abs(end - start) + 1; }
  };
} // ns Dx

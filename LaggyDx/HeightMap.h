#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class HeightMap
  {
  public:
    static HeightMap fromBitmap(const IBitmap& i_bmp);

  public:
    int getWidth() const;
    int getHeight() const;
    
    double getMinHeight() const;
    double getMaxHeight() const;

    void setHeight(int i_x, int i_y, double i_height);
    double getHeight(double i_x, double i_y) const;

    void resize(int i_width, int i_height);

    /// Scales all heigths to the [i_minHeight, i_maxHeight] interval
    void normalize(double i_minHeight, double i_maxHeight);

  private:
    int d_width = 0;
    int d_height = 0;
    std::vector<double> d_heights;

    double d_minHeight = 0;
    double d_maxHeight = 0;

    template <typename T>
    bool coordsAreValid(const T i_x, const T i_y) const
    {
      return
        0 <= i_x && i_x <= d_width - 1 &&
        0 <= i_y && i_y <= d_height - 1;
    }
    int getIndex(int i_x, int i_y) const;

    void updateMinMaxHeights(double i_height);
  };

} // ns Dx

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
    const Sdk::Vector2I& getSize() const;
    
    double getMinHeight() const;
    double getMaxHeight() const;

    void setHeight(int i_x, int i_y, double i_height);
    void addHeight(int i_x, int i_y, double i_height);
    double getHeight(double i_x, double i_y) const;

    void resize(int i_width, int i_height, double i_defaultHeight = 0);

    /// Scales all heigths to the [i_minHeight, i_maxHeight] interval
    void normalize(double i_minHeight, double i_maxHeight);

  private:
    Sdk::Vector2I d_size;
    std::vector<double> d_heights;

    double d_minHeight = 0;
    double d_maxHeight = 0;

    template <typename T>
    bool coordsAreValid(const T i_x, const T i_y) const
    {
      return
        0 <= i_x && i_x <= d_size.x - 1 &&
        0 <= i_y && i_y <= d_size.y - 1;
    }
    int getIndex(int i_x, int i_y) const;

    void updateMinMaxHeights(double i_height);
  };

} // ns Dx

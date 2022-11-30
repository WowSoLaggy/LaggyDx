#include "stdafx.h"
#include "HeightMap.h"

#include "IBitmap.h"


namespace Dx
{
  HeightMap HeightMap::fromBitmap(const IBitmap& i_bmp)
  {
    HeightMap heightMap;
    heightMap.resize(i_bmp.getWidth(), i_bmp.getHeight());

    heightMap.d_minHeight = (double)*i_bmp.getData();
    heightMap.d_maxHeight = (double)*i_bmp.getData();

    int ind = 0;
    for (int y = 0; y < i_bmp.getHeight(); ++y)
    {
      const auto* data = i_bmp.getData() + i_bmp.getStride() * y;
      for (int x = 0; x < i_bmp.getWidth(); ++x, data += 4, ++ind)
      {
        const double value = (double)*data;
        heightMap.d_heights[ind] = value;

        heightMap.updateMinMaxHeights(value);
      }
    }

    return heightMap;
  }


  int HeightMap::getWidth() const { return d_width; }
  int HeightMap::getHeight() const { return d_height; }

  double HeightMap::getMinHeight() const { return d_minHeight; }
  double HeightMap::getMaxHeight() const { return d_maxHeight; }


  void HeightMap::setHeight(const int i_x, const int i_y, const double i_height)
  {
    CONTRACT_EXPECT(coordsAreValid(i_x, i_y));
    d_heights.at(getIndex(i_x, i_y)) = i_height;

    updateMinMaxHeights(i_height);
  }

  double HeightMap::getHeight(const double i_x, const double i_y) const
  {
    CONTRACT_EXPECT(coordsAreValid(i_x, i_y));

    const int xBase = (int)std::floor(i_x);
    const int yBase = (int)std::floor(i_y);

    const bool xBorder = xBase == d_width - 1;
    const bool yBorder = yBase == d_height - 1;

    const int x0 = xBorder ? xBase - 1 : xBase;
    const int x1 = x0 + 1;

    const int y0 = yBorder ? yBase - 1 : yBase;
    const int y1 = y0 + 1;

    const double f00 = d_heights.at(getIndex(x0, y0));
    const double f10 = d_heights.at(getIndex(x1, y0));
    const double f11 = d_heights.at(getIndex(x1, y1));
    const double f01 = d_heights.at(getIndex(x0, y1));

    const double xRatio = (i_x - x0) / (x1 - x0);
    const double yRatio = (i_y - y0) / (y1 - y0);

    const double f0 = f00 + yRatio * (f01 - f00);
    const double f1 = f10 + yRatio * (f11 - f10);

    const double f = f0 + xRatio * (f1 - f0);

    return f;
  }


  void HeightMap::resize(const int i_width, const int i_height)
  {
    d_width = i_width;
    d_height = i_height;
    d_heights.resize(d_width * d_height);
  }


  void HeightMap::normalize(const double i_minHeight, const double i_maxHeight)
  {
    const double actualMinMaxDiff = d_maxHeight - d_minHeight;
    const double newMinMaxDiff = i_maxHeight - i_minHeight;

    for (auto& height : d_heights)
    {
      const double ratio = actualMinMaxDiff != 0 ?
        (height - d_minHeight) / actualMinMaxDiff : 0.5;
      height = i_minHeight + ratio * newMinMaxDiff;
    }

    d_minHeight = i_minHeight;
    d_maxHeight = i_maxHeight;
  }


  int HeightMap::getIndex(const int i_x, const int i_y) const
  {
    return i_x + i_y * d_width;
  }


  void HeightMap::updateMinMaxHeights(double i_height)
  {
    d_minHeight = std::min(d_minHeight, i_height);
    d_maxHeight = std::max(d_maxHeight, i_height);
  }

} // ns Dx

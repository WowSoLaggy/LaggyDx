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
        const double value = (double)(
          *data + *(data + 1) + *(data + 2)) / 3;
        heightMap.d_heights[ind] = value;

        heightMap.updateMinMaxHeights(value);
      }
    }

    return heightMap;
  }


  int HeightMap::getWidth() const { return d_size.x; }
  int HeightMap::getHeight() const { return d_size.y; }
  const Sdk::Vector2I& HeightMap::getSize() const { return d_size; }

  double HeightMap::getMinHeight() const { return d_minHeight; }
  double HeightMap::getMaxHeight() const { return d_maxHeight; }


  void HeightMap::setHeight(const int i_x, const int i_y, const double i_height)
  {
    CONTRACT_EXPECT(coordsAreValid(i_x, i_y));
    d_heights[getIndex(i_x, i_y)] = i_height;

    updateMinMaxHeights(i_height);
  }

  void HeightMap::addHeight(const int i_x, const int i_y, const double i_height)
  {
    CONTRACT_EXPECT(coordsAreValid(i_x, i_y));

    const double newHeight = d_heights[getIndex(i_x, i_y)] + i_height;
    d_heights[getIndex(i_x, i_y)] = newHeight;

    updateMinMaxHeights(newHeight);
  }

  double HeightMap::getHeight(const double i_x, const double i_y) const
  {
    const Cell cell = getCell(i_x, i_y);

    const double f0 = cell.f00 + cell.yRatio * (cell.f01 - cell.f00);
    const double f1 = cell.f10 + cell.yRatio * (cell.f11 - cell.f10);

    const double f = f0 + cell.xRatio * (f1 - f0);

    return f;
  }

  Sdk::Vector3F HeightMap::getNormal(const double i_x, const double i_y) const
  {
    const Cell cell = getCell(i_x, i_y);

    // Partial derivatives of the bilinear patch getHeight interpolates, so the normal
    // matches the surface height queries actually report. The grid step is 1 in both axes.
    const double dhdx =
      (cell.f10 - cell.f00) + cell.yRatio * (cell.f11 - cell.f01 - cell.f10 + cell.f00);
    const double dhdy =
      (cell.f01 - cell.f00) + cell.xRatio * (cell.f11 - cell.f01 - cell.f10 + cell.f00);

    // Map y is world z; the slope-vs-up ratio matches the mesh normals TerrainMipMap builds.
    Sdk::Vector3F normal{ (float)-dhdx, 1.0f, (float)-dhdy };
    normal.normalize();
    return normal;
  }


  void HeightMap::resize(const int i_width, const int i_height, const double i_defaultHeight)
  {
    d_size = { i_width, i_height };
    d_heights.assign(static_cast<size_t>(i_width) * i_height, i_defaultHeight);

    d_minHeight = i_defaultHeight;
    d_maxHeight = i_defaultHeight;
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
    return i_x + i_y * d_size.x;
  }


  HeightMap::Cell HeightMap::getCell(const double i_x, const double i_y) const
  {
    CONTRACT_EXPECT(coordsAreValid(i_x, i_y));

    const int xBase = (int)std::floor(i_x);
    const int yBase = (int)std::floor(i_y);

    // The far border has no cell of its own, so it is served by the last one before it.
    const bool xBorder = xBase == d_size.x - 1;
    const bool yBorder = yBase == d_size.y - 1;

    const int x0 = xBorder ? xBase - 1 : xBase;
    const int y0 = yBorder ? yBase - 1 : yBase;

    Cell cell;
    cell.f00 = d_heights[getIndex(x0, y0)];
    cell.f10 = d_heights[getIndex(x0 + 1, y0)];
    cell.f11 = d_heights[getIndex(x0 + 1, y0 + 1)];
    cell.f01 = d_heights[getIndex(x0, y0 + 1)];
    cell.xRatio = i_x - x0;
    cell.yRatio = i_y - y0;

    return cell;
  }


  void HeightMap::updateMinMaxHeights(double i_height)
  {
    d_minHeight = std::min(d_minHeight, i_height);
    d_maxHeight = std::max(d_maxHeight, i_height);
  }

} // ns Dx

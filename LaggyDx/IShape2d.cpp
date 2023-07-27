#include "stdafx.h"
#include "IShape2d.h"

#include "Shape2d.h"

#include <LaggySdk/Shapes.h>


namespace Dx
{
  std::unique_ptr<IShape2d> IShape2d::createCustom(
    const std::vector<Sdk::Vector2F>& i_verts,
    const std::vector<int>& i_inds)
  {
    return std::make_unique<Shape2d>(i_verts, i_inds);
  }

  std::unique_ptr<IShape2d> IShape2d::createCircle(const float i_radius, const int i_numPoints)
  {
    auto points = Sdk::getPointsOnCircle(i_radius, i_numPoints);
    points.insert(points.begin(), { 0, 0 });

    std::vector<int> inds(i_numPoints * 3, 0);
    for (int i = 0; i < i_numPoints; ++i)
    {
      inds[i * 3 + 1] = i + 1;
      inds[i * 3 + 2] = i + 2;
    }
    inds.back() = 1;

    return createCustom(points, inds);
  }

  std::unique_ptr<IShape2d> IShape2d::createCircle(const float i_radius, const int i_numPoints,
                                               const float i_startAngle, const float i_endAngle)
  {
    auto points = Sdk::getPointsOnCircle(i_radius, i_numPoints, i_startAngle, i_endAngle);
    points.insert(points.begin(), { 0, 0 });

    std::vector<int> inds((i_numPoints - 1) * 3, 0);
    for (int i = 0; i < i_numPoints - 1; ++i)
    {
      inds[i * 3 + 1] = i + 1;
      inds[i * 3 + 2] = i + 2;
    }

    return createCustom(points, inds);
  }


  std::unique_ptr<IShape2d> IShape2d::createRect(float i_width, float i_height)
  {
    const std::vector<Sdk::Vector2F> points{
      { 0, 0 },
      { i_width, 0 },
      { i_width, i_height },
      { 0, i_height },
    };

    const std::vector<int> inds{ 0, 1, 2, 0, 2, 3 };

    return createCustom(points, inds);
  }

} // ns Dx

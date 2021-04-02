#include "stdafx.h"
#include "IShape.h"

#include "Shape.h"

#include <LaggySdk/Shapes.h>


namespace Dx
{
  std::shared_ptr<IShape> IShape::createCustom(
    const std::vector<Sdk::Vector2F>& i_verts,
    const std::vector<int>& i_inds)
  {
    return std::make_shared<Shape>(i_verts, i_inds);
  }

  std::shared_ptr<IShape> IShape::createCircle(const float i_radius, const int i_numPoints)
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

    return std::make_shared<Shape>(points, inds);
  }

  std::shared_ptr<IShape> IShape::createCircle(const float i_radius, const int i_numPoints,
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

    return std::make_shared<Shape>(points, inds);
  }

} // ns Dx

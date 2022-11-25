#include "stdafx.h"
#include "DynamicSurface.h"


namespace Dx
{
  struct Tri
  {
    Tri(const int i_ind1, const int i_ind2, const int i_ind3)
      : ind1(i_ind1)
      , ind2(i_ind2)
      , ind3(i_ind3)
    {
    }

    const int ind1 = 0;
    const int ind2 = 0;
    const int ind3 = 0;
  };


  DynamicSurface::DynamicSurface(const double i_size)
  {
    d_points.push_back(VertexPosNormText::pos({ 0, 0, 0 }));
    d_points.push_back(VertexPosNormText::pos({ 0, 0, (float)i_size }));
    d_points.push_back(VertexPosNormText::pos({ (float)i_size, 0, (float)i_size }));
    d_points.push_back(VertexPosNormText::pos({ (float)i_size, 0, 0 }));

    tri1 = std::make_shared<Tri>(0, 1, 3);
    tri2 = std::make_shared<Tri>(2, 3, 1);

    collectInds();
  }


  void DynamicSurface::collectInds()
  {
    d_inds = collectInds(*tri1);
    const auto inds2 = collectInds(*tri2);

    d_inds.insert(d_inds.end(), inds2.begin(), inds2.end());
  }

  std::vector<int> DynamicSurface::collectInds(const Tri& i_tri)
  {
    return { i_tri.ind1, i_tri.ind2, i_tri.ind3 };
  }


  const std::vector<VertexPosNormText>& DynamicSurface::getPoints() const
  {
    return d_points;
  }

  const std::vector<int>& DynamicSurface::getInds() const
  {
    return d_inds;
  }

} // Dx
